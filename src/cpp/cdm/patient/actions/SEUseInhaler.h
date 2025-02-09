/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEConsciousRespirationCommand.h"
class SESubstance;

class CDM_DECL SEUseInhaler : public SEConsciousRespirationCommand
{
  friend class PBPatientAction;//friend the serialization class
  friend class SEConsciousRespiration;
  SEUseInhaler(Logger* logger);
public:
  virtual ~SEUseInhaler();

  static constexpr char const* Name = "Use Inhaler";
  std::string GetName() const override { return Name; }

  void Clear() override;
  virtual void Copy(const SEUseInhaler& src);

  bool IsValid() const override;
  bool IsActive() const override;
};
