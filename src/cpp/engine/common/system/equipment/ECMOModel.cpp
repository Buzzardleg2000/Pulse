/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "engine/PulseConfiguration.h"
#include "engine/common/controller/CircuitManager.h"
#include "engine/common/controller/CompartmentManager.h"
#include "engine/common/controller/SubstanceManager.h"
#include "engine/common/system/equipment/ECMOModel.h"
#include "engine/common/system/physiology/Saturation.h"

#include "cdm/system/equipment/ecmo/actions/SEECMOConfiguration.h"
#include "cdm/engine/SEActionManager.h"
#include "cdm/engine/SEEquipmentActionCollection.h"
#include "cdm/engine/SEPatientActionCollection.h"
#include "cdm/engine/SEEventManager.h"
#include "cdm/circuit/fluid/SEFluidCircuit.h"
#include "cdm/compartment/fluid/SEGasCompartment.h"
#include "cdm/compartment/fluid/SELiquidCompartment.h"
#include "cdm/compartment/fluid/SELiquidCompartmentGraph.h"
#include "cdm/compartment/substances/SEGasSubstanceQuantity.h"
#include "cdm/compartment/substances/SELiquidSubstanceQuantity.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceCompound.h"
#include "cdm/substance/SESubstanceFraction.h"
#include "cdm/substance/SESubstanceConcentration.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalarFrequency.h"
#include "cdm/properties/SEScalarMassPerVolume.h"
#include "cdm/properties/SEScalarPressure.h"
#include "cdm/properties/SEScalarPressureTimePerVolume.h"
#include "cdm/properties/SEScalarVolumePerTime.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/properties/SERunningAverage.h"
#include "cdm/utils/GeneralMath.h"

namespace pulse
{
  /*
  ========================
    Constructors
  ========================
  */
  ECMOModel::ECMOModel(Data& data) : ECMOSystem(data.GetLogger()), Model(data)
  {
    Clear();
  }

  ECMOModel::~ECMOModel()
  {
    Clear();
  }

  void ECMOModel::Clear()
  {
    ECMOSystem::Clear();
    m_CurrentInflowLocation = eECMO_CannulationLocation::NullCannulationLocation;
    m_CurrentOutflowLocation = eECMO_CannulationLocation::NullCannulationLocation;

    m_InflowCmpt = nullptr;
    m_lVascularToBloodSamplingPort = nullptr;
    m_cBloodSamplingPort = nullptr;
    m_cOxygenator = nullptr;
    m_lOxygenatorToVascular = nullptr;
    m_OutflowCmpt = nullptr;

    m_InflowNode = nullptr;
    m_pVascularToBloodSamplingPort = nullptr;
    m_nBloodSamplingPort = nullptr;
    m_nOxygenator = nullptr;
    m_pOxygenatorToVascular = nullptr;
    m_OutflowNode = nullptr;
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Initializes system properties to valid homeostatic values.
  //--------------------------------------------------------------------------------------------------
  void ECMOModel::Initialize()
  {
    Model::Initialize();

    GetSettings().GetOxygenatorVolume().Set(m_nOxygenator->GetVolumeBaseline());
    //StateChange(); // TODO should not need, even when serializing a state with ECMO on
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// Initializes parameters for the ECMO class
  ///
  /// \details
  /// Initializes member variables and system level values on the common data model.
  //--------------------------------------------------------------------------------------------------
  void ECMOModel::SetUp()
  {
    m_pVascularToBloodSamplingPort = m_data.GetCircuits().GetFluidPath(pulse::ECMOPath::VascularToBloodSamplingPort);
    if (m_pVascularToBloodSamplingPort != nullptr)
      m_InflowNode = &m_pVascularToBloodSamplingPort->GetSourceNode();
    m_nBloodSamplingPort = m_data.GetCircuits().GetFluidNode(pulse::ECMONode::BloodSamplingPort);
    m_nBloodSamplingPort->GetNextVolume().Set(m_nBloodSamplingPort->GetVolumeBaseline());
    m_pBloodSamplingPortToGround = m_data.GetCircuits().GetFluidPath(pulse::ECMOPath::BloodSamplingPortToGround);
    m_pGroundToOxygenator = m_data.GetCircuits().GetFluidPath(pulse::ECMOPath::GroundToOxygenator);
    m_nOxygenator = m_data.GetCircuits().GetFluidNode(pulse::ECMONode::Oxygenator);
    m_pOxygenatorToVascular = m_data.GetCircuits().GetFluidPath(pulse::ECMOPath::OxygenatorToVasculature);
    if (m_pOxygenatorToVascular != nullptr)
      m_OutflowNode = &m_pOxygenatorToVascular->GetTargetNode();

    m_lVascularToBloodSamplingPort = m_data.GetCompartments().GetLiquidLink(pulse::ECMOLink::VasculatureToBloodSamplingPort);
    if (m_lVascularToBloodSamplingPort != nullptr)
      m_InflowCmpt = &m_lVascularToBloodSamplingPort->GetSourceCompartment();
    m_cBloodSamplingPort = m_data.GetCompartments().GetLiquidCompartment(pulse::ECMOCompartment::BloodSamplingPort);
    m_lBloodSamplingPortToOxygenator = m_data.GetCompartments().GetLiquidLink(pulse::ECMOLink::BloodSamplingPortToOxygenator);
    m_cOxygenator = m_data.GetCompartments().GetLiquidCompartment(pulse::ECMOCompartment::Oxygenator);
    m_lOxygenatorToVascular = m_data.GetCompartments().GetLiquidLink(pulse::ECMOLink::OxygenatorToVasculature);
    if (m_lOxygenatorToVascular != nullptr)
      m_OutflowCmpt = &m_lOxygenatorToVascular->GetTargetCompartment();
  }

  void ECMOModel::StateChange()
  {
    SEECMOSettings& s = GetSettings();

    // Update these settings if we are connected or not
    if (!s.HasOxygenatorVolume())
    {
      s.GetOxygenatorVolume().Set(m_nOxygenator->GetVolumeBaseline());
      Warning("Oxygenator not provided an initial volume, defaulting to " + m_nOxygenator->GetVolumeBaseline().ToString());
    }

    // Check our Off states
    if (s.GetInflowLocation() == eECMO_CannulationLocation::NullCannulationLocation ||
        s.GetOutflowLocation() == eECMO_CannulationLocation::NullCannulationLocation ||
        (s.HasTransfusionFlow() && s.GetTransfusionFlow().IsZero()))
    {
      //Disconnect ECMO;
      // We can leave the oxygenator volume and its substances alone
      // We can leave the locations the same as well

      s.SetInflowLocation(eECMO_CannulationLocation::NullCannulationLocation);
      s.SetOutflowLocation(eECMO_CannulationLocation::NullCannulationLocation);

      DisconnectECMO();
      m_data.GetCircuits().GetActiveCardiovascularCircuit().StateChange();
      m_data.GetCompartments().GetActiveCardiovascularGraph().StateChange();
      m_data.GetCompartments().StateChange();
      return;
    }

      // Connect to the circuit/graph
    if (s.GetInflowLocation()  != m_CurrentInflowLocation ||
        s.GetOutflowLocation() != m_CurrentOutflowLocation)
    {
      DisconnectECMO();

      switch (s.GetInflowLocation())
      {
      case eECMO_CannulationLocation::InternalJugular:
        m_InflowNode = m_data.GetCircuits().GetFluidNode(pulse::CardiovascularNode::VenaCava1);
        m_InflowCmpt = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::VenaCava);
        break;
      case eECMO_CannulationLocation::LeftFemoralVein:
        m_InflowNode = m_data.GetCircuits().GetFluidNode(pulse::CardiovascularNode::LeftLeg2);
        m_InflowCmpt = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::LeftLeg);
        break;
      case eECMO_CannulationLocation::LeftSubclavianVein:
        m_InflowNode = m_data.GetCircuits().GetFluidNode(pulse::CardiovascularNode::LeftArm2);
        m_InflowCmpt = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::LeftArm);
        break;
      case eECMO_CannulationLocation::RightFemoralVein:
        m_InflowNode = m_data.GetCircuits().GetFluidNode(pulse::CardiovascularNode::RightLeg2);
        m_InflowCmpt = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::RightLeg);
        break;
      case eECMO_CannulationLocation::RightSubclavianVein:
        m_InflowNode = m_data.GetCircuits().GetFluidNode(pulse::CardiovascularNode::RightArm2);
        m_InflowCmpt = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::RightArm);
        break;
      case eECMO_CannulationLocation::NullCannulationLocation:
        Error("Cannot connect ECMO to body, in flow location not specified");
        return;
      default:
        Error("Unknown InflowLocation not specified");
        return;
      }
      
      switch (s.GetOutflowLocation())
      {
      case eECMO_CannulationLocation::InternalJugular:
        m_OutflowNode = m_data.GetCircuits().GetFluidNode(pulse::CardiovascularNode::VenaCava1);
        m_OutflowCmpt = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::VenaCava);
        break;
      case eECMO_CannulationLocation::LeftFemoralVein:
        m_OutflowNode = m_data.GetCircuits().GetFluidNode(pulse::CardiovascularNode::LeftLeg2);
        m_OutflowCmpt = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::LeftLeg);
        break;
      case eECMO_CannulationLocation::LeftSubclavianVein:
        m_OutflowNode = m_data.GetCircuits().GetFluidNode(pulse::CardiovascularNode::LeftArm2);
        m_OutflowCmpt = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::LeftArm);
        break;
      case eECMO_CannulationLocation::RightFemoralVein:
        m_OutflowNode = m_data.GetCircuits().GetFluidNode(pulse::CardiovascularNode::RightLeg2);
        m_OutflowCmpt = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::RightLeg);
        break;
      case eECMO_CannulationLocation::RightSubclavianVein:
        m_OutflowNode = m_data.GetCircuits().GetFluidNode(pulse::CardiovascularNode::RightArm2);
        m_OutflowCmpt = m_data.GetCompartments().GetLiquidCompartment(pulse::VascularCompartment::RightArm);
        break;
      case eECMO_CannulationLocation::NullCannulationLocation:
        Error("Cannot connect ECMO to body, out flow location not specified");
        return;
      default:
        Error("Unknown InflowLocation not specified");
        return;
      }

      // Add in vasculature to blood sampling to ground
      m_pVascularToBloodSamplingPort = &m_data.GetCircuits().CreateFluidPath(*m_InflowNode, *m_nBloodSamplingPort, pulse::ECMOPath::VascularToBloodSamplingPort);
      m_data.GetCircuits().GetActiveCardiovascularCircuit().AddNode(*m_nBloodSamplingPort);
      m_data.GetCircuits().GetActiveCardiovascularCircuit().AddPath(*m_pVascularToBloodSamplingPort);
      m_data.GetCircuits().GetActiveCardiovascularCircuit().AddPath(*m_pBloodSamplingPortToGround);
      m_pBloodSamplingPortToGround->GetNextFlowSource().Set(s.GetTransfusionFlow());
      m_pBloodSamplingPortToGround->GetFlowSourceBaseline().Set(s.GetTransfusionFlow());

      // Add in the oxygenator to vasculature
      m_pOxygenatorToVascular = &m_data.GetCircuits().CreateFluidPath(*m_nOxygenator, *m_OutflowNode, pulse::ECMOPath::OxygenatorToVasculature);
      m_data.GetCircuits().GetActiveCardiovascularCircuit().AddNode(*m_nOxygenator);
      m_data.GetCircuits().GetActiveCardiovascularCircuit().AddPath(*m_pOxygenatorToVascular);
      m_data.GetCircuits().GetActiveCardiovascularCircuit().AddPath(*m_pGroundToOxygenator);
      m_pGroundToOxygenator->GetNextFlowSource().Set(s.GetTransfusionFlow());
      m_pGroundToOxygenator->GetFlowSourceBaseline().Set(s.GetTransfusionFlow());
      // Update Circuit
      m_data.GetCircuits().GetActiveCardiovascularCircuit().StateChange();

      m_lVascularToBloodSamplingPort = &m_data.GetCompartments().CreateLiquidLink(*m_InflowCmpt, *m_cBloodSamplingPort, pulse::ECMOLink::VasculatureToBloodSamplingPort);
      m_lVascularToBloodSamplingPort->MapPath(*m_pVascularToBloodSamplingPort);

      m_lOxygenatorToVascular = &m_data.GetCompartments().CreateLiquidLink(*m_cOxygenator, *m_OutflowCmpt, pulse::ECMOLink::OxygenatorToVasculature);
      m_lOxygenatorToVascular->MapPath(*m_pOxygenatorToVascular);

      // Update Oxygenator Volume
      m_nOxygenator->GetNextVolume().Set(s.GetOxygenatorVolume());

      // Copy the substance values from the vasulature to the ECMO machine
      for (SELiquidSubstanceQuantity* srcSubQ : m_InflowCmpt->GetSubstanceQuantities())
      {
        if (!srcSubQ->HasConcentration())
          continue;
        Info("Initial ECMO Concentration: " + srcSubQ->GetSubstance().GetName() + " " + srcSubQ->GetConcentration().ToString());

        SELiquidSubstanceQuantity* tgtSubQ;
        tgtSubQ = m_cBloodSamplingPort->GetSubstanceQuantity(srcSubQ->GetSubstance());
        tgtSubQ->GetConcentration().Set(srcSubQ->GetConcentration());
        tgtSubQ->Balance(BalanceLiquidBy::Concentration);

        tgtSubQ = m_cOxygenator->GetSubstanceQuantity(srcSubQ->GetSubstance());
        tgtSubQ->GetConcentration().Set(srcSubQ->GetConcentration());
        tgtSubQ->Balance(BalanceLiquidBy::Concentration);
      }
      m_cOxygenator->GetPH().Set(m_InflowCmpt->GetPH());
      m_cBloodSamplingPort->GetPH().Set(m_InflowCmpt->GetPH());
      m_data.GetSaturationCalculator().CalculateBloodGasDistribution(*m_cBloodSamplingPort);
      m_data.GetSaturationCalculator().CalculateBloodGasDistribution(*m_cOxygenator);

      m_lBloodSamplingPortToOxygenator->GetFlow().Set(s.GetTransfusionFlow());

      m_data.GetCompartments().GetActiveCardiovascularGraph().AddCompartment(*m_cBloodSamplingPort);
      m_data.GetCompartments().GetActiveCardiovascularGraph().AddCompartment(*m_cOxygenator);
      m_data.GetCompartments().GetActiveCardiovascularGraph().AddLink(*m_lVascularToBloodSamplingPort);
      m_data.GetCompartments().GetActiveCardiovascularGraph().AddLink(*m_lBloodSamplingPortToOxygenator);
      m_data.GetCompartments().GetActiveCardiovascularGraph().AddLink(*m_lOxygenatorToVascular);
      // Update Graph
      m_data.GetCompartments().StateChange();
      m_data.GetCompartments().GetActiveCardiovascularGraph().StateChange();
      
      m_CurrentInflowLocation = GetSettings().GetInflowLocation();
      m_CurrentOutflowLocation = GetSettings().GetOutflowLocation();
      Info("ECMO Attached. Transfusing from the "+eECMO_CannulationLocation_Name(s.GetInflowLocation())+" to the "+eECMO_CannulationLocation_Name(s.GetOutflowLocation()));
    }

    
  }
  void ECMOModel::DisconnectECMO()
  {
    if (m_pVascularToBloodSamplingPort != nullptr)
    {
      m_data.GetCircuits().GetActiveCardiovascularCircuit().RemovePath(*m_pVascularToBloodSamplingPort);
      m_data.GetCircuits().DeleteFluidPath(m_pVascularToBloodSamplingPort->GetName());
      m_pVascularToBloodSamplingPort = nullptr;
    }
    if (m_pOxygenatorToVascular != nullptr)
    {
      m_data.GetCircuits().GetActiveCardiovascularCircuit().RemovePath(*m_pOxygenatorToVascular);
      m_data.GetCircuits().DeleteFluidPath(m_pOxygenatorToVascular->GetName());
      m_pOxygenatorToVascular = nullptr;
    }
    m_data.GetCircuits().GetActiveCardiovascularCircuit().RemovePath(*m_pBloodSamplingPortToGround);
    m_data.GetCircuits().GetActiveCardiovascularCircuit().RemovePath(*m_pGroundToOxygenator);

    if (m_lVascularToBloodSamplingPort != nullptr)
    {
      m_InflowCmpt->RemoveLink(*m_lVascularToBloodSamplingPort);
      m_cBloodSamplingPort->RemoveLink(*m_lVascularToBloodSamplingPort);
      m_data.GetCompartments().GetActiveCardiovascularGraph().RemoveLink(*m_lVascularToBloodSamplingPort);
      m_data.GetCompartments().DeleteLiquidLink(m_lVascularToBloodSamplingPort->GetName());
      m_lVascularToBloodSamplingPort = nullptr;
    }
    if (m_lOxygenatorToVascular != nullptr)
    {
      m_OutflowCmpt->RemoveLink(*m_lOxygenatorToVascular);
      m_cOxygenator->RemoveLink(*m_lOxygenatorToVascular);
      m_data.GetCompartments().GetActiveCardiovascularGraph().RemoveLink(*m_lOxygenatorToVascular);
      m_data.GetCompartments().DeleteLiquidLink(m_lOxygenatorToVascular->GetName());
      m_lOxygenatorToVascular = nullptr;
      Info("ECMO Detached");
    }
    m_data.GetCompartments().GetActiveCardiovascularGraph().RemoveLink(*m_lBloodSamplingPortToOxygenator);

    m_data.GetCircuits().GetActiveCardiovascularCircuit().RemoveNode(*m_nBloodSamplingPort);
    m_data.GetCircuits().GetActiveCardiovascularCircuit().RemoveNode(*m_nOxygenator);
    m_data.GetCompartments().GetActiveCardiovascularGraph().RemoveCompartment(*m_cBloodSamplingPort);
    m_data.GetCompartments().GetActiveCardiovascularGraph().RemoveCompartment(*m_cOxygenator);
  }


  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// ECMO preprocess function
  ///
  /// \details
  /// Applies all the settings and calculates the instantaneous driver value.
  //--------------------------------------------------------------------------------------------------
  void ECMOModel::PreProcess()
  {
    if (m_data.GetActions().GetEquipmentActions().HasECMOConfiguration())
    {
      GetSettings().ProcessConfiguration(m_data.GetActions().GetEquipmentActions().GetECMOConfiguration(), m_data.GetSubstances());
      m_data.GetActions().GetEquipmentActions().RemoveECMOConfiguration();
      StateChange();
    }

    // We keep the oxygenator volume and concentrations consistent while on
    SEECMOSettings& s = GetSettings();
    if (s.GetInflowLocation() != eECMO_CannulationLocation::NullCannulationLocation &&
        s.GetOutflowLocation() != eECMO_CannulationLocation::NullCannulationLocation)
    {
      m_nOxygenator->GetNextVolume().Set(s.GetOxygenatorVolume());

      bool saturate = false;
      if (s.HasSubstanceCompound())
      {
        saturate = true;
        for (const SESubstanceConcentration* sc : s.GetSubstanceCompound()->GetComponents())
        {
          m_data.GetSubstances().AddActiveSubstance(sc->GetSubstance());
          SELiquidSubstanceQuantity* sq = m_cOxygenator->GetSubstanceQuantity(sc->GetSubstance());
          sq->GetConcentration().Set(*sc->GetConcentration());
          sq->Balance(BalanceLiquidBy::Concentration);
        }
      }

      if (s.HasSubstanceConcentrations())
      {
        saturate = true;
        for (const SESubstanceConcentration* sc : s.GetSubstanceConcentrations())
        {
          m_data.GetSubstances().AddActiveSubstance(sc->GetSubstance());
          SELiquidSubstanceQuantity* sq = m_cOxygenator->GetSubstanceQuantity(sc->GetSubstance());
          sq->GetConcentration().Set(*sc->GetConcentration());
          sq->Balance(BalanceLiquidBy::Concentration);
        }
      }
      if (saturate)
      {
        m_data.GetSaturationCalculator().CalculateBloodGasDistribution(*m_cBloodSamplingPort);
        m_data.GetSaturationCalculator().CalculateBloodGasDistribution(*m_cOxygenator);
      }
    }
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// ECMO process function
  ///
  /// \details
  /// TBD
  //--------------------------------------------------------------------------------------------------
  void ECMOModel::Process(bool /*solve_and_transport*/)
  {
  }

  //--------------------------------------------------------------------------------------------------
  /// \brief
  /// ECMO postprocess function
  ///
  /// \details
  /// TBD
  //--------------------------------------------------------------------------------------------------
  void ECMOModel::PostProcess(bool /*solve_and_transport*/)
  {
  }

  void ECMOModel::ComputeExposedModelParameters()
  {
  }

END_NAMESPACE
