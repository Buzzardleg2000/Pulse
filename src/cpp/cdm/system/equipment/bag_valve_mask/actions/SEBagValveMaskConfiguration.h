/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
#pragma once
#include "cdm/system/equipment/bag_valve_mask/actions/SEBagValveMaskAction.h"
class SEBagValveMask;
class SESubstanceManager;

class CDM_DECL SEBagValveMaskConfiguration : public SEBagValveMaskAction
{
  friend class PBEquipmentAction;//friend the serialization class
public:

  SEBagValveMaskConfiguration(Logger* logger=nullptr);
  virtual ~SEBagValveMaskConfiguration();

  static constexpr char const* Name = "Configuration";
  virtual std::string GetName() const override { return Name; }

  virtual void Clear() override;
  virtual void Copy(const SEBagValveMaskConfiguration& src, const SESubstanceManager& subMgr, bool /*preserveState*/ = false);

  virtual bool IsValid() const override;
  virtual bool IsActive() const override;
  virtual void Deactivate() override;

  bool HasConfiguration() const;
  SEBagValveMask& GetConfiguration();
  const SEBagValveMask* GetConfiguration() const;

  virtual std::string GetConfigurationFile() const;
  virtual void SetConfigurationFile(const std::string& fileName);
  virtual bool HasConfigurationFile() const;

  virtual eMergeType GetMergeType() const;
  virtual void SetMergeType(eMergeType m);

  virtual const SEScalar* GetScalar(const std::string& name) override;

protected:

  std::string       m_ConfigurationFile;
  SEBagValveMask*   m_Configuration;
  eMergeType        m_MergeType;
};