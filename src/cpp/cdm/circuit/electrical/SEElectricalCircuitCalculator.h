/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/circuit/SECircuitCalculator.h"
#include "cdm/circuit/electrical/SEElectricalCircuit.h"

using SEElectricalCircuitCalculator = SECircuitCalculator<SEElectricalCircuit, SEElectricalCircuitNode, SEElectricalCircuitPath, ElectricCapacitanceUnit, ElectricCurrentUnit, ElectricInductanceUnit, ElectricPotentialUnit, ElectricChargeUnit, ElectricResistanceUnit>;

#define DefaultElectricalCircuitCalculatorUnits ElectricCapacitanceUnit::F, ElectricCurrentUnit::A, ElectricInductanceUnit::H, ElectricPotentialUnit::V, ElectricChargeUnit::C, ElectricResistanceUnit::Ohm
