/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "engine/CommonDefs.h"

class SESubstance;
class SESubstanceManager;
class SELiquidCompartment;
class SELiquidSubstanceQuantity;

namespace pulse
{
  /**
  * @brief
  * The %SaturationCalculator class holds the blood gas distribution model.
  */
  class PULSE_DECL SaturationCalculator : public Loggable
  {
    struct CompartmentState
    {
      SELiquidSubstanceQuantity* subO2Q = nullptr;
      SELiquidSubstanceQuantity* subCO2Q = nullptr;
      SELiquidSubstanceQuantity* subCOQ = nullptr;
      SELiquidSubstanceQuantity* subHbCOQ = nullptr;
      SELiquidSubstanceQuantity* subHCO3Q = nullptr;
      SELiquidSubstanceQuantity* subHbQ = nullptr;
      SELiquidSubstanceQuantity* subHbO2Q = nullptr;
      SELiquidSubstanceQuantity* subHbCO2Q = nullptr;
      SELiquidSubstanceQuantity* subHbO2CO2Q = nullptr;
    };
    friend class Controller;
    friend struct error_functor;
    friend class EngineTest;
  public:
    SaturationCalculator(Data& data);
    virtual ~SaturationCalculator();

    bool Setup();

    void SetBodyState(const SEScalarMassPerVolume& AlbuminConcentration, const SEScalar0To1& Hematocrit, const SEScalarTemperature& Temperature, const SEScalarAmountPerVolume& StrongIonDifference, const SEScalarAmountPerVolume& Phosphate);
    void CalculateBloodGasDistribution(SELiquidCompartment& cmpt) const;
    void CalculateCarbonMonoxideSpeciesDistribution(CompartmentState& cs, SELiquidCompartment& cmpt) const;

  protected:
    // Stewart Model + Dash-Bassingthwaighte Model + Henderson-Hasselbach Model
    void CalculateHemoglobinSaturations(CompartmentState& cs, double O2PartialPressureGuess_mmHg, double CO2PartialPressureGuess_mmHg, double pH, double temperature_C, double  hematocrit, double& OxygenSaturation, double& CarbonDioxideSaturation, double CO2_scaling_factor) const;
    bool DistributeHemoglobinBySaturation(CompartmentState& cs) const;

    Data& m_data;
    std::stringstream ss;
    // All properties are stateless and are set by either the Initialize method or SetBodyState method
    SESubstance* m_O2;
    SESubstance* m_Hb;
    SESubstance* m_HbO2;
    SESubstance* m_HbO2CO2;
    SESubstance* m_CO2;
    SESubstance* m_CO;
    SESubstance* m_HbCO;
    SESubstance* m_HCO3;
    SESubstance* m_HbCO2;
    // The current state of the body to balance to
    double m_albumin_g_per_L;
    double m_hematocrit;
    double m_temperature_C;
    // Here is the stuff that will need to go into the CDM
    double m_StrongIonDifference_mmol_Per_L; // BloodChemistrySystemData mmol/L
    double m_Phosphate_mmol_Per_L; //BloodChemistryData mmol/L
  };
END_NAMESPACE
