/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/compartment/SECompartment.h"
#include "cdm/circuit/fluid/SEFluidCircuitNode.h"
#include "cdm/compartment/SECompartmentNodes.h"
#include "cdm/substance/SESubstanceTransport.h"

class SESubstance;
class SERunningAverage;
template<typename EdgeType, typename VertexType, typename CompartmentType> class SEFluidCompartmentLink;

#define FLUID_COMPARTMENT_TEMPLATE typename LinkType, typename VertexType, typename TransportSubstanceType, typename SubstanceQuantityType
#define FLUID_COMPARTMENT_TYPES LinkType, VertexType, TransportSubstanceType, SubstanceQuantityType

template<FLUID_COMPARTMENT_TEMPLATE>
class SEFluidCompartment : public SECompartment, public VertexType
{
  friend class PBCompartment;//friend the serialization class
  template<typename CompartmentType, typename CompartmentLinkType> friend class SECompartmentGraph;
protected:
  SEFluidCompartment(const std::string& name, Logger* logger);
public:
  virtual ~SEFluidCompartment();

  void Clear() override;

  std::string GetName() const override { return m_Name; }

  const SEScalar* GetScalar(const std::string& name) override;

  bool HasChildren() const override { return !m_FluidChildren.empty(); }

  virtual bool HasNodeMapping() const { return m_Nodes.HasMapping(); }
  virtual  SECompartmentNodes<FLUID_COMPARTMENT_NODE>& GetNodeMapping() { return m_Nodes; }
  virtual void MapNode(SEFluidCircuitNode& node);

  virtual void Sample(bool CycleStart);
  virtual void SampleFlow(); // We can add other sampling support as needed, just follow the pattern

  virtual bool HasInFlow() const;
  virtual const SEScalarVolumePerTime& GetInFlow() const;
  virtual double GetInFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasAverageInFlow() const;
  virtual const SEScalarVolumePerTime& GetAverageInFlow() const;
  virtual double GetAverageInFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasOutFlow() const;
  virtual const SEScalarVolumePerTime& GetOutFlow() const;
  virtual double GetOutFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasAverageOutFlow() const;
  virtual const SEScalarVolumePerTime& GetAverageOutFlow() const;
  virtual double GetAverageOutFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasPressure() const;
  virtual SEScalarPressure& GetPressure();
  virtual double GetPressure(const PressureUnit& unit) const;

  virtual bool HasVolume() const;
  virtual SEScalarVolume& GetVolume();
  virtual double GetVolume(const VolumeUnit& unit) const;

  virtual bool HasSubstanceQuantities() const;
  virtual bool HasSubstanceQuantity(const SESubstance& substance) const;
  virtual SubstanceQuantityType* GetSubstanceQuantity(const SESubstance& substance) const;
  virtual const std::vector<SubstanceQuantityType*>& GetSubstanceQuantities() const;
  virtual void ZeroSubstanceQuantities();

  virtual void AddLink(LinkType& link);
  virtual void RemoveLink(LinkType& link);
  virtual void RemoveLinks();
  virtual const std::vector<LinkType*>& GetLinks();

  virtual bool HasChild(const SEFluidCompartment& cmpt);

protected:
  virtual void RemoveSubstanceQuantity(const SESubstance& substance);

  bool HasQuantity() const override { return HasVolume(); }
  SEScalarVolume& GetQuantity() override { return GetVolume(); }

  virtual double CalculateInFlow_mL_Per_s() const;
  virtual double CalculateOutFlow_mL_Per_s() const;

  std::vector<TransportSubstanceType*>& GetTransportSubstances() override { return m_TransportSubstances; }

  mutable SEScalarVolumePerTime* m_InFlow;
  mutable SEScalarVolumePerTime* m_OutFlow;
  bool                           m_SampleFlow;
  mutable SEScalarVolumePerTime* m_AverageInFlow;
  mutable SEScalarVolumePerTime* m_AverageOutFlow;
  mutable SERunningAverage*      m_AverageInFlow_mL_Per_s;
  mutable SERunningAverage*      m_AverageOutFlow_mL_Per_s;

  SEScalarPressure*              m_Pressure;
  SEScalarVolume*                m_Volume;

  std::vector<SubstanceQuantityType*>  m_SubstanceQuantities;
  std::vector<TransportSubstanceType*> m_TransportSubstances;

  std::vector<LinkType*> m_Links;
  std::vector<LinkType*> m_IncomingLinks;
  std::vector<LinkType*> m_OutgoingLinks;
  std::vector<SEFluidCompartment*> m_FluidChildren;
  SECompartmentNodes<FLUID_COMPARTMENT_NODE>  m_Nodes;
};

#include "cdm/compartment/fluid/SEFluidCompartment.hxx"
