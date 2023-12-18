/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/compartment/fluid/SELiquidCompartmentGraph.h"

#include "cdm/properties/SEScalarMass.h"
#include "cdm/properties/SEScalarMassPerVolume.h"

void SELiquidCompartmentGraph::BalanceByIntensive()
{
  for (auto cmpt : GetCompartments())
    cmpt->Balance(BalanceLiquidBy::Concentration);
}

void SELiquidCompartmentGraph::AddGraph(SELiquidCompartmentGraph& graph)
{
  for (SELiquidCompartment* cmpt : graph.GetCompartments())
    AddCompartment(*cmpt);
  for (SELiquidCompartmentLink* lnk : graph.GetLinks())
    AddLink(*lnk);
}

template class SECompartmentGraph<SELiquidCompartment, SELiquidCompartmentLink>;
