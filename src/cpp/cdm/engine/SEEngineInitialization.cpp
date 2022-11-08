/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/engine/SEEngineInitialization.h"
#include "cdm/engine/SEDataRequestManager.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/io/protobuf/PBEngine.h"

SEEngineInitialization::SEEngineInitialization(Logger* logger) : Loggable(logger)
{
  m_PatientConfiguration = nullptr;
  m_DataRequestManager = nullptr;
  Clear();
}

SEEngineInitialization::~SEEngineInitialization()
{
  SAFE_DELETE(m_PatientConfiguration);
  SAFE_DELETE(m_DataRequestManager);
}

bool SEEngineInitialization::SerializeToString(std::string& output, eSerializationFormat m) const
{
  return PBEngine::SerializeToString(*this, output, m);
}
bool SEEngineInitialization::SerializeFromString(const std::string& src, eSerializationFormat m, const SESubstanceManager& subMgr)
{
  return PBEngine::SerializeFromString(src, *this, m, subMgr);
}
bool SEEngineInitialization::SerializeFromString(const std::string& src, std::vector<SEEngineInitialization*>& dst, eSerializationFormat m, const SESubstanceManager& subMgr)
{
  return PBEngine::SerializeFromString(src, dst, m, subMgr);
}

void SEEngineInitialization::Clear()
{
  if (m_PatientConfiguration)
    m_PatientConfiguration->Clear();
  m_StateFilename = "";
  m_State = "";
  if (m_DataRequestManager)
    m_DataRequestManager->Clear();
  m_LogFilename = "";
  m_KeepLogMessages = false;
  m_KeepEventChanges = false;
}

void SEEngineInitialization::Copy(const SEEngineInitialization& from, const SESubstanceManager& subMgr)
{
  PBEngine::Copy(from, *this, subMgr);
}

bool SEEngineInitialization::IsValid() const
{
  return (HasPatientConfiguration() || HasStateFilename() || HasState());
}

bool SEEngineInitialization::HasPatientConfiguration() const
{
  return m_PatientConfiguration != nullptr;
}
SEPatientConfiguration& SEEngineInitialization::GetPatientConfiguration()
{
  if (m_PatientConfiguration == nullptr)
    m_PatientConfiguration = new SEPatientConfiguration(GetLogger());
  return *m_PatientConfiguration;
}
const SEPatientConfiguration* SEEngineInitialization::GetPatientConfiguration() const 
{
  return m_PatientConfiguration;
}

std::string SEEngineInitialization::GetStateFilename() const
{
  return m_StateFilename;
}
void SEEngineInitialization::SetStateFilename(const std::string& StateFilename)
{
  m_StateFilename = StateFilename;
}
bool SEEngineInitialization::HasStateFilename() const
{
  return !m_StateFilename.empty();
}

bool SEEngineInitialization::HasState() const
{
  return !m_State.empty();
}
std::string SEEngineInitialization::GetState() const
{
  return m_State;
}
void SEEngineInitialization::SetState(const std::string& State, eSerializationFormat fmt)
{
  m_State = State;
  m_StateFormat = fmt;
}
eSerializationFormat SEEngineInitialization::GetStateFormat() const
{
  return m_StateFormat;
}

bool SEEngineInitialization::HasDataRequestManager() const
{
  return m_DataRequestManager != nullptr;
}
SEDataRequestManager& SEEngineInitialization::GetDataRequestManager()
{
  if (m_DataRequestManager == nullptr)
    m_DataRequestManager = new SEDataRequestManager(GetLogger());
  return *m_DataRequestManager;
}
const SEDataRequestManager* SEEngineInitialization::GetDataRequestManager() const
{
  return m_DataRequestManager;
}

std::string SEEngineInitialization::GetLogFilename() const
{
  return m_LogFilename;
}
void SEEngineInitialization::SetLogFilename(const std::string& LogFilename)
{
  m_LogFilename = LogFilename;
}
bool SEEngineInitialization::HasLogFilename() const
{
  return !m_LogFilename.empty();
}

bool SEEngineInitialization::KeepLogMessages() const
{
  return m_KeepLogMessages;
}
void SEEngineInitialization::KeepLogMessages(bool b)
{
  m_KeepLogMessages = b;
}

bool SEEngineInitialization::KeepEventChanges() const
{
  return m_KeepEventChanges;
}
void SEEngineInitialization::KeepEventChanges(bool b)
{
  m_KeepEventChanges = b;
}

//////////////////////////////////
// SEEngineInitializationStatus //
//////////////////////////////////

SEEngineInitializationStatus::SEEngineInitializationStatus(Logger* logger) : Loggable(logger)
{
  m_LogMessages = nullptr;
  Clear();
}

SEEngineInitializationStatus::~SEEngineInitializationStatus()
{
  SAFE_DELETE(m_LogMessages);
}

bool SEEngineInitializationStatus::SerializeToString(std::string& output, eSerializationFormat m) const
{
  return PBEngine::SerializeToString(*this, output, m);
}
bool SEEngineInitializationStatus::SerializeFromString(const std::string& src, eSerializationFormat m)
{
  return PBEngine::SerializeFromString(src, *this, m);
}
bool SEEngineInitializationStatus::SerializeFromString(const std::string& src, std::vector<SEEngineInitializationStatus*>& dst, eSerializationFormat m, Logger* logger)
{
  return PBEngine::SerializeFromString(src, dst, m, logger);
}

void SEEngineInitializationStatus::Clear()
{
  m_ID = -1;
  if (m_LogMessages)
    m_LogMessages->Clear();
  m_Created = false;
}

void SEEngineInitializationStatus::Copy(const SEEngineInitializationStatus& from)
{
  PBEngine::Copy(from, *this);
}

int SEEngineInitializationStatus::GetID() const
{
  return m_ID;
}
void SEEngineInitializationStatus::SetID(int id)
{
  m_ID = id;
}

bool SEEngineInitializationStatus::Created() const
{
  return m_Created;
}
void SEEngineInitializationStatus::Created(bool b)
{
  m_Created = b;
}

bool SEEngineInitializationStatus::HasLogMessages() const
{
  return m_LogMessages != nullptr;
}
LogMessages& SEEngineInitializationStatus::GetLogMessages()
{
  if (m_LogMessages == nullptr)
    m_LogMessages = new LogMessages();
  return *m_LogMessages;
}
const LogMessages* SEEngineInitializationStatus::GetLogMessages() const
{
  return m_LogMessages;
}
