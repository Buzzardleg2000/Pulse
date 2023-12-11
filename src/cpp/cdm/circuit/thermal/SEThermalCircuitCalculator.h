/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/circuit/SECircuitCalculator.h"
#include "cdm/circuit/thermal/SEThermalCircuit.h"

using SEThermalCircuitCalculator = SECircuitCalculator<SEThermalCircuit, SEThermalCircuitNode, SEThermalCircuitPath, HeatCapacitanceUnit, PowerUnit, HeatInductanceUnit, TemperatureUnit, EnergyUnit, HeatResistanceUnit>;

#define DefaultThermalCircuitCalculatorUnits HeatCapacitanceUnit::J_Per_K, PowerUnit::W, HeatInductanceUnit::K_s_Per_W, TemperatureUnit::K, EnergyUnit::J,HeatResistanceUnit::K_Per_W
