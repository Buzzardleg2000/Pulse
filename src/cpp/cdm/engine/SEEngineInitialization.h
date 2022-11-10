/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SEPatientConfiguration;
class SEDataRequestManager;
class SESubstanceManager;

class CDM_DECL SEEngineInitialization : public Loggable
{
  friend class PBEngine;//friend the serialization class
public:
  explicit SEEngineInitialization(Logger* logger=nullptr);
  virtual ~SEEngineInitialization();

  virtual void Copy(const SEEngineInitialization& from, const SESubstanceManager& subMgr);
  virtual void Clear(); //clear memory

  bool SerializeToString(std::string& output, eSerializationFormat m) const;
  bool SerializeFromString(const std::string& src, eSerializationFormat m, const SESubstanceManager& subMgr);
  static bool SerializeFromString(const std::string& src, std::vector<SEEngineInitialization*>& dst, eSerializationFormat m, const SESubstanceManager& subMgr);

  virtual bool IsValid()const;

  virtual bool HasPatientConfiguration() const;
  virtual SEPatientConfiguration& GetPatientConfiguration();
  virtual const SEPatientConfiguration* GetPatientConfiguration() const;

  virtual bool HasStateFilename() const;
  virtual std::string GetStateFilename() const;
  virtual void SetStateFilename(const std::string& filename);

  virtual bool HasState() const;
  virtual std::string GetState() const;
  virtual void SetState(const std::string& s, eSerializationFormat fmt);
  virtual eSerializationFormat GetStateFormat() const;

  virtual bool HasDataRequestManager() const;
  virtual SEDataRequestManager& GetDataRequestManager();
  virtual const SEDataRequestManager* GetDataRequestManager() const;

  virtual bool HasLogFilename() const;
  virtual std::string GetLogFilename() const;
  virtual void SetLogFilename(const std::string& filename);

  virtual bool KeepLogMessages() const;
  virtual void KeepLogMessages(bool b);

  virtual bool KeepEventChanges() const;
  virtual void KeepEventChanges(bool b);

protected:
  SEPatientConfiguration*         m_PatientConfiguration;
  std::string                     m_StateFilename;
  std::string                     m_State;
  eSerializationFormat            m_StateFormat;
  SEDataRequestManager*           m_DataRequestManager;
  std::string                     m_LogFilename;
  bool                            m_KeepLogMessages;
  bool                            m_KeepEventChanges;
};

class CDM_DECL SEEngineInitializationStatus : public LoggerForward
{
  friend class PBEngine; //friend the serialization class
public:
  explicit SEEngineInitializationStatus();
  virtual ~SEEngineInitializationStatus();

  SEEngineInitializationStatus(const SEEngineInitializationStatus&) = delete;
  SEEngineInitializationStatus& operator= (const SEEngineInitializationStatus&) = delete;

  virtual void Copy(const SEEngineInitializationStatus& from);
  virtual void Clear(); //clear memory

  bool SerializeToString(std::string& output, eSerializationFormat m) const;
  bool SerializeFromString(const std::string& src, eSerializationFormat m);
  static bool SerializeFromString(const std::string& src, std::vector<SEEngineInitializationStatus*>& dst, eSerializationFormat m, Logger* logger);

  virtual int GetID() const;
  virtual void SetID(int id);
  
  virtual bool Created() const;
  virtual void Created(bool b);

  virtual bool KeepLogMessages() const { return m_KeepLogMessages; }
  virtual void KeepLogMessages(bool b) { m_KeepLogMessages = b; }
  virtual LogMessages& GetLogMessages();
  virtual const LogMessages& GetLogMessages() const;

  void ForwardDebug(const std::string& msg) override;
  void ForwardInfo(const std::string& msg) override;
  void ForwardWarning(const std::string& msg) override;
  void ForwardError(const std::string& msg) override;
  void ForwardFatal(const std::string& msg) override;

protected:
  int                             m_ID;
  bool                            m_Created;
  bool                            m_KeepLogMessages;
  LogMessages                     m_LogMessages;
};
