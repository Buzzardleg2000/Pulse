/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/system/SESystem.h"

class CDM_DECL SEImmuneSystem : public SESystem
{
  friend class PBPhysiology;//friend the serialization class
public:

  SEImmuneSystem(Logger* logger);
  virtual ~SEImmuneSystem();

  virtual void Clear();// Deletes all members
  
  virtual const SEScalar* GetScalar(const std::string& name);
/*
  virtual bool HasActivatedPhagocytes() const;
  virtual SEScalarAmount& GetActivatedPhagocytes();
  virtual double GetActivatedPhagocytes(const AmountUnit& unit) const;

  virtual bool HasAntiInflammatoryMediators() const;
  virtual SEScalarAmount& GetAntiInflammatoryMediators();
  virtual double GetAntiInflammatoryMediators(const AmountUnit& unit) const;

  virtual bool HasPathogenCount() const;
  virtual SEScalarAmount& GetPathogenCount();
  virtual double GetPathogenCount(const AmountUnit& unit) const;

  virtual bool HasTissueDamage() const;
  virtual SEScalar0To1& GetTissueDamage();
  virtual double GetTissueDamage() const;

protected:

  SEScalarAmount* m_ActivatedPhagocytes;
  SEScalarAmount* m_AntiInflammatoryMediators;
  SEScalarAmount* m_PathogenCount;
  SEScalar0To1*   m_TissueDamage;
*/
};