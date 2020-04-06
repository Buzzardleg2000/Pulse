/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEEngineConfiguration;
class SEPatient;
class SEConditionManager;
class SESubstanceManager;

class CDM_DECL SEPatientConfiguration : public Loggable
{
  friend class PBEngine;//friend the serialization class
public:
  SEPatientConfiguration(SESubstanceManager& subMgr);
  virtual ~SEPatientConfiguration();
  virtual void Clear(); //clear memory

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename, SerializationFormat m) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m);
  bool SerializeFromFile(const std::string& filename, SerializationFormat m);

  virtual bool IsValid()const;

  virtual std::string GetPatientFile() const;
  virtual void SetPatientFile(const std::string& patientFile);
  virtual bool HasPatientFile() const;
  virtual void InvalidatePatientFile();

  virtual SEPatient& GetPatient();
  virtual const SEPatient* GetPatient() const;
  virtual bool HasPatient() const;
  virtual void InvalidatePatient();

  virtual SEConditionManager& GetConditions();
  virtual const SEConditionManager* GetConditions() const;
  virtual bool HasConditions() const;
  virtual void InvalidateConditions();

  virtual bool HasOverride() const;
  virtual void AddOverride(const std::string& name, double value);
  virtual std::map<std::string, double>& GetOverrides();
  virtual const std::map<std::string, double>& GetOverrides() const;
  virtual void RemoveOverrides();

protected:
  SEPatient*                      m_Patient;
  std::string                     m_PatientFile;
  SEConditionManager*             m_Conditions;
  SESubstanceManager&             m_SubMgr;
  std::map<std::string, double>   m_Overrides;
};