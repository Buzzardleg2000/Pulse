/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/compartment/fluid/SEFluidCompartment.h"
#include "cdm/compartment/fluid/SELiquidCompartmentLink.h"
#include "cdm/compartment/substances/SELiquidSubstanceQuantity.h"
class SELiquidBlackBox;

class CDM_DECL SELiquidCompartment : public SEFluidCompartment<SELiquidCompartmentLink, SELiquidTransportVertex, SELiquidTransportSubstance, SELiquidSubstanceQuantity>
{
  friend class PBCompartment;//friend the serialization class
  friend class SETissueCompartment;
  friend class SECompartmentManager;
  friend class SELiquidSubstanceQuantity;
protected:
  SELiquidCompartment(const std::string& name, Logger* logger);
public:
  virtual ~SELiquidCompartment();

  void Clear() override;

  const SEScalar* GetScalar(const std::string& name) override;

  void StateChange() override;

  virtual void Balance(BalanceLiquidBy e);// Balance all substances based on a specific property

  virtual bool HasPerfusion() const;
  virtual SEScalarVolumePerTime& GetPerfusion();
  virtual double GetPerfusion(const VolumePerTimeUnit& unit) const;

  virtual bool HasPH() const;
  virtual SEScalar& GetPH();
  virtual double GetPH() const;

  virtual bool HasWaterVolumeFraction() const;
  virtual SEScalar0To1& GetWaterVolumeFraction();
  virtual double GetWaterVolumeFraction() const;

  virtual void AddChild(SELiquidCompartment& child);
  virtual const std::vector<SELiquidCompartment*>& GetChildren() { return m_Children; }
  virtual const std::vector<SELiquidCompartment*>& GetLeaves() { return m_Leaves; }

protected:
  virtual SELiquidSubstanceQuantity& CreateSubstanceQuantity(SESubstance& substance, bool zeroOut=true);

  SEScalarVolumePerTime* m_Perfusion;
  SEScalar*              m_pH;
  SEScalar0To1*          m_WaterVolumeFraction;

  std::vector<SELiquidCompartment*> m_Children;
  std::vector<SELiquidCompartment*> m_Leaves;
};
