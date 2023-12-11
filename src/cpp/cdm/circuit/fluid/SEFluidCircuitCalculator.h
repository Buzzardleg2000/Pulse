/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
//CDM_BIND_DECL(FluidCircuitData)
#include "cdm/circuit/SECircuitCalculator.h"
#include "cdm/circuit/fluid/SEFluidCircuit.h"

using SEFluidCircuitCalculator = SECircuitCalculator<SEFluidCircuit, SEFluidCircuitNode, SEFluidCircuitPath, VolumePerPressureUnit, VolumePerTimeUnit, PressureTimeSquaredPerVolumeUnit, PressureUnit, VolumeUnit, PressureTimePerVolumeUnit>;

#define DefaultFluidCircuitCalculatorUnits VolumePerPressureUnit::m3_Per_Pa,VolumePerTimeUnit::m3_Per_s, PressureTimeSquaredPerVolumeUnit::Pa_s2_Per_m3,PressureUnit::Pa,VolumeUnit::m3,PressureTimePerVolumeUnit::Pa_s_Per_m3
