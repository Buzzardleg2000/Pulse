/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

#include "cdm/properties/SEScalar.h"
#include "cdm/circuit/SECircuitNode.h"
#include "cdm/circuit/SECircuitPath.h"

#define BLACK_BOX_TEMPLATE typename NodeType, typename PathType, \
                           typename PotentialScalar, typename QuantityScalar, typename FluxScalar, \
                           typename PotentialUnit, typename QuantityUnit, typename FluxUnit

#define BLACK_BOX_TYPES NodeType, PathType, \
                        PotentialScalar, QuantityScalar, FluxScalar, \
                        PotentialUnit,QuantityUnit, FluxUnit

#define ELECTRICAL_BLACK_BOX SEElectricalCircuitNode, SEElectricalCircuitPath, \
                             SEScalarElectricPotential, SEScalarElectricCharge, SEScalarElectricCurrent, \
                             ElectricPotentialUnit, ElectricChargeUnit, ElectricCurrentUnit

#define MAP_ELECTRICAL_BLACK_BOX SEElectricalBlackBox,SEElectricalCircuitNode, SEElectricalCircuitPath

#define FLUID_BLACK_BOX SEFluidCircuitNode, SEFluidCircuitPath, \
                        SEScalarPressure, SEScalarVolume, SEScalarVolumePerTime, \
                        PressureUnit, VolumeUnit, VolumePerTimeUnit

#define MAP_FLUID_BLACK_BOX SEFluidBlackBox,SEFluidCircuitNode, SEFluidCircuitPath

#define THERMAL_BLACK_BOX SEThermalCircuitNode, SEThermalCircuitPath, \
                          SEScalarTemperature, SEScalarEnergy, SEScalarPower, \
                          TemperatureUnit, EnergyUnit, PowerUnit

#define MAP_THERMAL_BLACK_BOX SEThermalBlackBox,SEThermalCircuitNode, SEThermalCircuitPath

#define CCT typename CT, typename NT, typename PT, \
            typename CU, typename FU, typename IU, \
            typename PU, typename QU, typename RU

template<BLACK_BOX_TEMPLATE>
class CDM_DECL SEBlackBox : public Loggable
{
  friend class SEBlackBoxManager;
  friend class CommonDataModelTest;
  template<CCT> friend class SECircuitCalculator;
protected:
  SEBlackBox(const std::string& name, Logger* logger) : Loggable(logger), m_Name(name) {}
public:
  virtual ~SEBlackBox() {}

  virtual void Clear() { }

  virtual std::string GetName() const { return m_Name; }

  virtual void RemoveImposedPotential() { m_MiddleNode->RemoveImposedPotential(); }
  virtual bool IsPotentialImposed() const { return m_MiddleNode->IsPotentialImposed(); }
  virtual void ImposePotential(double v, const PotentialUnit& unit) { m_MiddleNode->ImposePotential(v, unit); }
  virtual void ImposePotential(const PotentialScalar& s) { return m_MiddleNode->ImposePotential(s); }
  virtual double GetPotential(const PotentialUnit& unit) const { return m_MiddleNode->GetNextPotential().GetValue(unit); }

  virtual void RemoveImposedSourcePotential() { m_SourceNode->RemoveImposedPotential(); }
  virtual bool IsSourcePotentialImposed() const { return m_SourceNode->IsPotentialImposed(); }
  virtual void ImposeSourcePotential(double v, const PotentialUnit& unit) { m_SourceNode->ImposePotential(v, unit); }
  virtual void ImposeSourcePotential(const PotentialScalar& s) { return m_SourceNode->ImposePotential(s); }
  virtual double GetSourcePotential(const PotentialUnit& unit) const { return m_SourceNode->GetNextPotential().GetValue(unit); }

  virtual void RemoveImposedTargetPotential() { m_TargetNode->RemoveImposedPotential(); }
  virtual bool IsTargetPotentialImposed() const { return m_TargetNode->IsPotentialImposed(); }
  virtual void ImposeTargetPotential(double v, const PotentialUnit& unit) { m_TargetNode->ImposePotential(v, unit); }
  virtual void ImposeTargetPotential(const PotentialScalar& s) { return m_TargetNode->ImposePotential(s); }
  virtual double GetTargetPotential(const PotentialUnit& unit) const { return m_TargetNode->GetNextPotential().GetValue(unit); }

  virtual void RemoveImposedSourceFlux() { m_SourcePath->RemoveImposedFlux(); }
  virtual bool IsSourceFluxImposed() const { return m_SourcePath->IsFluxImposed(); }
  virtual void ImposeSourceFlux(double v, const FluxUnit& unit) { m_SourcePath->ImposeFlux(v, unit); }
  virtual void ImposeSourceFlux(const FluxScalar& s) { m_SourcePath->ImposeFlux(s); }
  virtual double GetSourceFlux(const FluxUnit& unit) const { return m_SourcePath->GetNextFlux().GetValue(unit); }

  virtual void RemoveImposedTargetFlux() { m_TargetPath->RemoveImposedFlux(); }
  virtual bool IsTargetFluxImposed() const { return m_TargetPath->IsFluxImposed(); }
  virtual void ImposeTargetFlux(double v, const FluxUnit& unit) { m_TargetPath->ImposeFlux(v, unit); }
  virtual void ImposeTargetFlux(const FluxScalar& s) { m_TargetPath->ImposeFlux(s); }
  virtual double GetTargetFlux(const FluxUnit& unit) const { return m_TargetPath->GetNextFlux().GetValue(unit); }

  virtual void RemoveImposedQuantity() { m_MiddleNode->RemoveImposedQuantity(); }
  virtual bool IsQuantityImposed() const { return m_MiddleNode->IsQuantityImposed(); }
  virtual void ImposeQuantity(double v, const QuantityUnit& unit) { m_MiddleNode->ImposeQuantity(v, unit); }
  virtual void ImposeQuantity(const QuantityScalar& s) { m_MiddleNode->ImposeQuantity(s); }
  virtual double GetQuantity(const QuantityUnit& unit) const { return m_MiddleNode->GetNextQuantity().GetValue(unit); }

  // Circuit Elements (If available)
  virtual bool HasMiddleNode() const { return m_MiddleNode != nullptr; }
  virtual NodeType* GetMiddleNode() const { return m_MiddleNode; }

  virtual bool HasSourceNode() const { return m_SourceNode != nullptr; }
  virtual NodeType* GetSourceNode() const { return m_SourceNode; }

  virtual bool HasTargetNode() const { return m_TargetNode != nullptr; }
  virtual NodeType* GetTargetNode() const { return m_TargetNode; }

  virtual bool HasSourcePath() const { return m_SourcePath != nullptr; }
  virtual PathType* GetSourcePath() const { return m_SourcePath; }

  virtual bool HasTargetPath() const { return m_TargetPath != nullptr; }
  virtual PathType* GetTargetPath() const { return m_TargetPath; }

protected:
  // Set Circuit Elements
  virtual void SetMiddleNode(NodeType* n) { m_MiddleNode = n; }
  virtual void SetSourceNode(NodeType* n) { m_SourceNode = n; }
  virtual void SetTargetNode(NodeType* n) { m_TargetNode = n; }
  virtual void SetSourcePath(PathType* p) { m_SourcePath = p; }
  virtual void SetTargetPath(PathType* p) { m_TargetPath = p; }

  std::string      m_Name;
  // Circuit Elements
  NodeType* m_MiddleNode = nullptr;
  NodeType* m_SourceNode = nullptr;
  NodeType* m_TargetNode = nullptr;
  PathType* m_SourcePath = nullptr;
  PathType* m_TargetPath = nullptr;
};

#include "cdm/blackbox/SEBlackBox.hxx"