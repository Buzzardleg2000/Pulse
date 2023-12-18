/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/compartment/fluid/SEGasCompartmentGraph.h"
#include "cdm/compartment/substances/SEGasSubstanceQuantity.h"
#include "cdm/compartment/SECompartmentManager.h"
#include "cdm/properties/SEScalarVolume.h"
#include "cdm/properties/SEScalar0To1.h"

void SEGasCompartmentGraph::BalanceByIntensive()
{
  for (auto cmpt : GetCompartments())
  {
    // Make sure Volume Fractions sum to 1.0 and adjust accordingly
    double totalVolumeFraction = 0;
    for (SEGasSubstanceQuantity* subQ : cmpt->GetSubstanceQuantities())
    {
      if (subQ->HasVolumeFraction())
        totalVolumeFraction += subQ->GetVolumeFraction().GetValue();
    }
    if (totalVolumeFraction == 0.0)
    {
      for (SEGasSubstanceQuantity* subQ : cmpt->GetSubstanceQuantities())
      {
        if (subQ->HasVolumeFraction())
          subQ->SetToZero();
      }
      continue;
    }
    else
    {
      //Adjust to keep the volume fractions making sense
    //Make it a little more sensitive than the error check later just to be safe
      if (std::abs(1.0 - totalVolumeFraction) > (ZERO_APPROX / 10.0))
      {
        for (SEGasSubstanceQuantity* subQ : cmpt->GetSubstanceQuantities())
        {
          //Adjust everything the same amount to make sure the volume fraction is 1.0
      double volumeFractionErrorFraction = 1.0 / totalVolumeFraction;  //<1 = too high; >1 = too low
          subQ->GetVolumeFraction().SetValue(subQ->GetVolumeFraction().GetValue() * volumeFractionErrorFraction);
        }
      }
    }
    cmpt->Balance(BalanceGasBy::VolumeFraction);
  }
}

void SEGasCompartmentGraph::AddGraph(SEGasCompartmentGraph& graph)
{
  for (SEGasCompartment* cmpt : graph.GetCompartments())
    AddCompartment(*cmpt);
  for (SEGasCompartmentLink* lnk : graph.GetLinks())
    AddLink(*lnk);
}

template class SECompartmentGraph<SEGasCompartment, SEGasCompartmentLink>;
