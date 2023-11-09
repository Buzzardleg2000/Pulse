/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/actions/SEPatientAction.h"
class SECardiovascularMechanicsModifiers;

class CDM_DECL SECardiovascularMechanicsModification : public SEPatientAction
{
  friend class PBPatientAction;//friend the serialization class
public:

  SECardiovascularMechanicsModification(Logger* logger=nullptr);
  virtual ~SECardiovascularMechanicsModification();

  static constexpr char const* Name = "Cardiovascular Mechanics Modification";
  std::string GetName() const override { return Name; }

  void Clear() override;
  void Copy(const SECardiovascularMechanicsModification& src, bool /*preserveState*/=false);

  const SEScalar* GetScalar(const std::string& name) override;

  bool IsValid() const override;
  bool IsActive() const override;
  void Deactivate() override;

  bool HasModifiers() const;
  SECardiovascularMechanicsModifiers& GetModifiers();
  const SECardiovascularMechanicsModifiers* GetModifiers() const;

  virtual std::string GetModifiersFile() const;
  virtual void SetModifiersFile(const std::string& fileName);
  virtual bool HasModifiersFile() const;

  virtual eMergeType GetMergeType() const;
  virtual void SetMergeType(eMergeType m);

protected:
  eMergeType                           m_MergeType;
  std::string                          m_ModifiersFile;
  SECardiovascularMechanicsModifiers*  m_Modifiers;
};
