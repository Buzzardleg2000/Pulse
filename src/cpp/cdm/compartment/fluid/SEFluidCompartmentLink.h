/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/compartment/SECompartmentLink.h"
#include "cdm/substance/SESubstanceTransport.h"
#include "cdm/compartment/fluid/SEFluidCompartment.h"
#include "cdm/circuit/fluid/SEFluidCircuitPath.h"

#define FLUID_COMPARTMENT_LINK_TEMPLATE typename EdgeType, typename VertexType, typename CompartmentType
#define FLUID_COMPARTMENT_LINK_TYPES EdgeType, VertexType, CompartmentType

template<FLUID_COMPARTMENT_LINK_TEMPLATE>
class SEFluidCompartmentLink : public SECompartmentLink, public EdgeType
{
  friend class PBCompartment;//friend the serialization class
protected:
  SEFluidCompartmentLink(CompartmentType& src, CompartmentType & tgt, const std::string& name);
public:
  virtual ~SEFluidCompartmentLink();

  void Clear() override;

  const SEScalar* GetScalar(const std::string& name) override;

  std::string GetName() const override { return m_Name; }
  
  virtual bool HasCompliance() const;
  virtual double GetCompliance(const VolumePerPressureUnit& unit) const;

  virtual bool HasFlow() const;
  virtual SEScalarVolumePerTime& GetFlow();
  virtual double GetFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasResistance() const;
  virtual double GetResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual CompartmentType& GetSourceCompartment() { return m_SourceCmpt; }
  virtual CompartmentType& GetTargetCompartment() { return m_TargetCmpt; }

  virtual bool HasPath() { return m_Path != nullptr; }  
  virtual SEFluidCircuitPath* GetPath() { return m_Path; }
  virtual void RemovePath() { m_Path = nullptr; }
  virtual void MapPath(SEFluidCircuitPath& path) { Clear();  m_Path = &path; }

protected:
  // For Transport
  bool HasFlux() const override { return HasFlow(); }
  SEScalarVolumePerTime& GetFlux() override { return GetFlow(); }
  virtual double GetFlux(const VolumePerTimeUnit& unit) const { return GetFlow(unit); }

  VertexType& GetSourceVertex() override { return m_SourceVertex; }
  VertexType& GetTargetVertex() override { return m_TargetVertex; }

  SEScalarVolumePerTime*  m_Flow;
  CompartmentType&        m_SourceCmpt;
  CompartmentType&        m_TargetCmpt;
  VertexType&             m_SourceVertex;
  VertexType&             m_TargetVertex;
  SEFluidCircuitPath*     m_Path;
};

#include "cdm/compartment/fluid/SEFluidCompartmentLink.hxx"
