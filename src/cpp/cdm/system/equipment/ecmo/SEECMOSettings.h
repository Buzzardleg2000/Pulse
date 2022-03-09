/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/CommonDefs.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/substance/SESubstanceConcentration.h"
#include "cdm/substance/SESubstanceFraction.h"
#include "cdm/engine/SEAction.h"

class SEECMOConfiguration;

// Keep enums in sync with appropriate schema/cdm/ECMO.proto file !!
enum class eECMO_CannulationLocation { NullCannulationLocation=0,
                                       InternalJugular,
                                       RightFemoralVein,
                                       LeftFemoralVein,
                                       RightSubclavianVein,
                                       LeftSubclavianVein };
extern const std::string& eECMO_CannulationLocation_Name(eECMO_CannulationLocation m);

class CDM_DECL SEECMOSettings : public Loggable
{
  friend class PBECMO;//friend the serialization class
public:

  SEECMOSettings(Logger* logger);
  virtual ~SEECMOSettings();

  virtual void Clear();

  bool SerializeToString(std::string& output, eSerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;
  bool SerializeFromString(const std::string& src, eSerializationFormat m, const SESubstanceManager& subMgr);
  bool SerializeFromFile(const std::string& filename, const SESubstanceManager& subMgr);

  virtual void Merge(const SEECMOSettings& from, SESubstanceManager& subMgr);
  virtual void ProcessConfiguration(SEECMOConfiguration& config, SESubstanceManager& subMgr);

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual eECMO_CannulationLocation GetInflowLocation() const;
  virtual void SetInflowLocation(eECMO_CannulationLocation c);

  virtual eECMO_CannulationLocation GetOutflowLocation() const;
  virtual void SetOutflowLocation(eECMO_CannulationLocation c);

  virtual bool HasOxygenatorVolume() const;
  virtual SEScalarVolume& GetOxygenatorVolume();
  virtual double GetOxygenatorVolume(const VolumeUnit& unit) const;

  virtual bool HasTransfusionFlow() const;
  virtual SEScalarVolumePerTime& GetTransfusionFlow();
  virtual double GetTransfusionFlow(const VolumePerTimeUnit& unit) const;

  bool HasSubstanceConcentrations() const;
  bool HasSubstanceConcentration(const SESubstance& substance) const;
  const std::vector<SESubstanceConcentration*>& GetSubstanceConcentrations();
  const std::vector<const SESubstanceConcentration*>& GetSubstanceConcentrations() const;
  SESubstanceConcentration& GetSubstanceConcentration(const SESubstance& substance);
  const SESubstanceConcentration* GetSubstanceConcentration(const SESubstance& substance) const;
  void RemoveSubstanceConcentration(const SESubstance& substance);
  void RemoveSubstanceConcentrations();

  // This will add/replace the concentrations
  // Substances not in the compound will not change
  void ApplyCompoundConcentrations(const SESubstanceCompound& compound);

protected:

  eECMO_CannulationLocation                    m_InflowLocation;
  eECMO_CannulationLocation                    m_OutflowLocation;
  SEScalarVolume*                              m_OxygenatorVolume;
  SEScalarVolumePerTime*                       m_TransfusionFlow;

  std::vector<SESubstanceConcentration*>       m_SubstanceConcentrations;
};
