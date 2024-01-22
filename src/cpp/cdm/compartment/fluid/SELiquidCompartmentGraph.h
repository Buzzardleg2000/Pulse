/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/compartment/fluid/SELiquidCompartment.h"
#include "cdm/compartment/fluid/SELiquidCompartmentLink.h"
#include "cdm/compartment/SECompartmentTransportGraph.h"

class SECompartmentManager;

using SELiquidTransportGraph = SESubstanceTransportGraph<SEScalarVolumePerTime, SEScalarVolume, SEScalarMass, SEScalarMassPerVolume>;
using SELiquidTransporter = SESubstanceTransporter<SELiquidTransportGraph, VolumePerTimeUnit, VolumeUnit, MassUnit, MassPerVolumeUnit>;
#define DefaultLiquidTransporterUnits VolumePerTimeUnit::L_Per_s, VolumeUnit::mL, MassUnit::ug, MassPerVolumeUnit::ug_Per_mL

class CDM_DECL SELiquidCompartmentGraph : public SECompartmentTransportGraph<SELiquidTransportGraph, SELiquidTransportVertex, SELiquidTransportEdge, SELiquidCompartment, SELiquidCompartmentLink>
{
  friend class PBCompartment;//friend the serialization class
  friend class SECompartmentManager;
protected:
  SELiquidCompartmentGraph(const std::string& name, Logger* logger) : SECompartmentTransportGraph(name, logger) {};
public:  
  virtual ~SELiquidCompartmentGraph() {}

  void AddGraph(SELiquidCompartmentGraph& graph);

protected:
  void BalanceByIntensive() override;
};
