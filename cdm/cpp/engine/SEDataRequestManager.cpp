/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "SEDataRequestManager.h"
#include "substance/SESubstance.h"
#include "substance/SESubstanceManager.h"
#include "io/protobuf/PBEngine.h"

SEDataRequestManager::SEDataRequestManager(Logger* logger) : Loggable(logger)
{
  m_DefaultDecimalFormatting = nullptr;
  m_OverrideDecimalFormatting = nullptr;
  Clear();
}

SEDataRequestManager::~SEDataRequestManager()
{
  Clear();
}

void SEDataRequestManager::Clear()
{
  m_SamplesPerSecond = 0; // Sample every time step
  DELETE_VECTOR(m_Requests);
  SAFE_DELETE(m_DefaultDecimalFormatting);
  SAFE_DELETE(m_OverrideDecimalFormatting);
}

void SEDataRequestManager::Copy(const SEDataRequestManager& src, const SESubstanceManager& subMgr)
{
  PBEngine::Copy(src, *this, subMgr);
}

bool SEDataRequestManager::SerializeToString(std::string& output, SerializationFormat m) const
{
  return PBEngine::SerializeToString(*this, output, m);
}
bool SEDataRequestManager::SerializeToFile(const std::string& filename, SerializationFormat m) const
{
  return PBEngine::SerializeToFile(*this, filename, m);
}
bool SEDataRequestManager::SerializeFromString(const std::string& src, SerializationFormat m, const SESubstanceManager& subMgr)
{
  return PBEngine::SerializeFromString(src, *this, m, subMgr);
}
bool SEDataRequestManager::SerializeFromFile(const std::string& filename, SerializationFormat m, const SESubstanceManager& subMgr)
{
  return PBEngine::SerializeFromFile(filename, *this, m, subMgr);
}

bool SEDataRequestManager::HasDefaultDecimalFormatting() const
{
  return m_DefaultDecimalFormatting != nullptr;
}
SEDecimalFormat& SEDataRequestManager::GetDefaultDecimalFormatting()
{
  if (m_DefaultDecimalFormatting == nullptr)
    m_DefaultDecimalFormatting = new SEDecimalFormat();
  return *m_DefaultDecimalFormatting;
}
void SEDataRequestManager::RemoveDefaultDecimalFormatting()
{
  SAFE_DELETE(m_DefaultDecimalFormatting);
}

bool SEDataRequestManager::HasOverrideDecimalFormatting() const
{
  return m_OverrideDecimalFormatting != nullptr;
}
SEDecimalFormat& SEDataRequestManager::GetOverrideDecimalFormatting()
{
  if (m_OverrideDecimalFormatting == nullptr)
    m_OverrideDecimalFormatting = new SEDecimalFormat();
  return *m_OverrideDecimalFormatting;
}
void SEDataRequestManager::RemoveOverrideDecimalFormatting()
{
  SAFE_DELETE(m_OverrideDecimalFormatting);
}

SEDataRequest& SEDataRequestManager::CopyDataRequest(const SEDataRequest& dr)
{
  SEDataRequest* my_dr = new SEDataRequest(dr);
  m_Requests.push_back(my_dr);
  return *my_dr;
}

SEDataRequest& SEDataRequestManager::CreateDataRequest(eDataRequest_Category category, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = new SEDataRequest(category, dfault);
  m_Requests.push_back(dr);
  return *dr;
}

SEDataRequest& SEDataRequestManager::CreatePatientDataRequest(const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindPatientDataRequest(property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::Patient, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreatePatientDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindPatientDataRequest(property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::Patient, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}
SEDataRequest* SEDataRequestManager::FindPatientDataRequest(const std::string& property)
{
  for (SEDataRequest* dr : m_Requests)
  {
    if (dr->GetCategory() == eDataRequest_Category::Patient && dr->GetPropertyName() == property)
      return dr;
  }
  return nullptr;
}

SEDataRequest& SEDataRequestManager::CreatePhysiologyDataRequest(const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindPhysiologyDataRequest(property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::Physiology, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreatePhysiologyDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindPhysiologyDataRequest(property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::Physiology, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}
SEDataRequest* SEDataRequestManager::FindPhysiologyDataRequest(const std::string& property)
{
  for (SEDataRequest* dr : m_Requests)
  {
    if (dr->GetCategory() == eDataRequest_Category::Physiology && dr->GetPropertyName() == property)
      return dr;
  }
  return nullptr;
}

SEDataRequest& SEDataRequestManager::CreateEnvironmentDataRequest(const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindEnvironmentDataRequest(property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::Environment, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateEnvironmentDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindEnvironmentDataRequest(property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::Environment, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}
SEDataRequest* SEDataRequestManager::FindEnvironmentDataRequest(const std::string& property)
{
  for (SEDataRequest* dr : m_Requests)
  {
    if (dr->GetCategory() == eDataRequest_Category::Environment && dr->GetPropertyName() == property)
      return dr;
  }
  return nullptr;
}

SEDataRequest& SEDataRequestManager::CreateGasCompartmentDataRequest(const std::string& cmptName, const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindGasCompartmentDataRequest(cmptName, property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::GasCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetCompartmentName(cmptName);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateGasCompartmentDataRequest(const std::string& cmptName, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindGasCompartmentDataRequest(cmptName, property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::GasCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetCompartmentName(cmptName);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}
SEDataRequest* SEDataRequestManager::FindGasCompartmentDataRequest(const std::string& cmptName, const std::string& property)
{
  for (SEDataRequest* dr : m_Requests)
  {
    if (dr->GetCategory() == eDataRequest_Category::GasCompartment &&
      dr->GetPropertyName() == property &&
      dr->GetCompartmentName() == cmptName)
      return dr;
  }
  return nullptr;
}

SEDataRequest& SEDataRequestManager::CreateGasCompartmentDataRequest(const std::string& cmptName, const SESubstance& sub, const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindGasCompartmentDataRequest(cmptName, sub, property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::GasCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetCompartmentName(cmptName);
  dr->SetSubstanceName(sub.GetName());
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateGasCompartmentDataRequest(const std::string& cmptName, const SESubstance& sub, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindGasCompartmentDataRequest(cmptName, sub, property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::GasCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetCompartmentName(cmptName);
  dr->SetSubstanceName(sub.GetName());
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}
SEDataRequest* SEDataRequestManager::FindGasCompartmentDataRequest(const std::string& cmptName, const SESubstance& sub, const std::string property)
{
  for (SEDataRequest* dr : m_Requests)
  {
    if (dr->GetCategory() == eDataRequest_Category::GasCompartment &&
      dr->GetPropertyName() == property &&
      dr->GetCompartmentName() == cmptName &&
      dr->GetSubstanceName() == sub.GetName())
      return dr;
  }
  return nullptr;
}

SEDataRequest& SEDataRequestManager::CreateLiquidCompartmentDataRequest(const std::string& cmptName, const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindLiquidCompartmentDataRequest(cmptName, property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::LiquidCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetCompartmentName(cmptName);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateLiquidCompartmentDataRequest(const std::string& cmptName, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindLiquidCompartmentDataRequest(cmptName, property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::LiquidCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetCompartmentName(cmptName);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}
SEDataRequest* SEDataRequestManager::FindLiquidCompartmentDataRequest(const std::string& cmptName, const std::string& property)
{
  for (SEDataRequest* dr : m_Requests)
  {
    if (dr->GetCategory() == eDataRequest_Category::LiquidCompartment &&
      dr->GetPropertyName() == property &&
      dr->GetCompartmentName() == cmptName)
      return dr;
  }
  return nullptr;
}

SEDataRequest& SEDataRequestManager::CreateLiquidCompartmentDataRequest(const std::string& cmptName, const SESubstance& sub, const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindLiquidCompartmentDataRequest(cmptName, sub, property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::LiquidCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetCompartmentName(cmptName);
  dr->SetSubstanceName(sub.GetName());
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateLiquidCompartmentDataRequest(const std::string& cmptName, const SESubstance& sub, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindLiquidCompartmentDataRequest(cmptName, sub, property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::LiquidCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetCompartmentName(cmptName);
  dr->SetSubstanceName(sub.GetName());
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}
SEDataRequest* SEDataRequestManager::FindLiquidCompartmentDataRequest(const std::string& cmptName, const SESubstance& sub, const std::string property)
{
  for (SEDataRequest* dr : m_Requests)
  {
    if (dr->GetCategory() == eDataRequest_Category::LiquidCompartment && 
        dr->GetPropertyName() == property &&
        dr->GetCompartmentName() == cmptName &&
        dr->GetSubstanceName() == sub.GetName())
      return dr;
  }
  return nullptr;
}

SEDataRequest& SEDataRequestManager::CreateThermalCompartmentDataRequest(const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindThermalCompartmentDataRequest(property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::ThermalCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateThermalCompartmentDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindThermalCompartmentDataRequest(property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::ThermalCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}
SEDataRequest* SEDataRequestManager::FindThermalCompartmentDataRequest(const std::string& property)
{
  for (SEDataRequest* dr : m_Requests)
  {
    if (dr->GetCategory() == eDataRequest_Category::ThermalCompartment && dr->GetPropertyName() == property)
      return dr;
  }
  return nullptr;
}

SEDataRequest& SEDataRequestManager::CreateTissueCompartmentDataRequest(const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindTissueCompartmentDataRequest(property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::TissueCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateTissueCompartmentDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindTissueCompartmentDataRequest(property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::TissueCompartment, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}
SEDataRequest* SEDataRequestManager::FindTissueCompartmentDataRequest(const std::string& property)
{
  for (SEDataRequest* dr : m_Requests)
  {
    if (dr->GetCategory() == eDataRequest_Category::TissueCompartment && dr->GetPropertyName() == property)
      return dr;
  }
  return nullptr;
}

SEDataRequest& SEDataRequestManager::CreateSubstanceDataRequest(const SESubstance& sub, const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindSubstanceDataRequest(sub, property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::Substance, dfault);
  m_Requests.push_back(dr);
  dr->SetSubstanceName(sub.GetName());
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateSubstanceDataRequest(const SESubstance& sub, const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindSubstanceDataRequest(sub, property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::Substance, dfault);
  m_Requests.push_back(dr);
  dr->SetSubstanceName(sub.GetName());
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}
SEDataRequest* SEDataRequestManager::FindSubstanceDataRequest(const SESubstance& sub, const std::string& property)
{
  for (SEDataRequest* dr : m_Requests)
  {
    if (dr->GetCategory() == eDataRequest_Category::Substance && 
        dr->GetPropertyName() == property && 
        dr->GetSubstanceName() == sub.GetName())
      return dr;
  }
  return nullptr;
}

SEDataRequest& SEDataRequestManager::CreateAnesthesiaMachineDataRequest(const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindAnesthesiaMachineDataRequest(property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::AnesthesiaMachine, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateAnesthesiaMachineDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindAnesthesiaMachineDataRequest(property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::AnesthesiaMachine, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}
SEDataRequest* SEDataRequestManager::FindAnesthesiaMachineDataRequest(const std::string& property)
{
  for (SEDataRequest* dr : m_Requests)
  {
    if (dr->GetCategory() == eDataRequest_Category::AnesthesiaMachine && dr->GetPropertyName() == property)
      return dr;
  }
  return nullptr;
}

SEDataRequest& SEDataRequestManager::CreateECGDataRequest(const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindECGDataRequest(property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::ECG, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateECGDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindECGDataRequest(property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::ECG, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}
SEDataRequest* SEDataRequestManager::FindECGDataRequest(const std::string& property)
{
  for (SEDataRequest* dr : m_Requests)
  {
    if (dr->GetCategory() == eDataRequest_Category::ECG && dr->GetPropertyName() == property)
      return dr;
  }
  return nullptr;
}

SEDataRequest& SEDataRequestManager::CreateInhalerDataRequest(const std::string& property, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindInhalerDataRequest(property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::Inhaler, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  return *dr;
}
SEDataRequest& SEDataRequestManager::CreateInhalerDataRequest(const std::string& property, const CCompoundUnit& unit, const SEDecimalFormat* dfault)
{
  SEDataRequest* dr = FindInhalerDataRequest(property);
  if (dr != nullptr)
    return *dr;
  dr = new SEDataRequest(eDataRequest_Category::Inhaler, dfault);
  m_Requests.push_back(dr);
  dr->SetPropertyName(property);
  dr->SetUnit(unit);
  return *dr;
}
SEDataRequest* SEDataRequestManager::FindInhalerDataRequest(const std::string& property)
{
  for (SEDataRequest* dr : m_Requests)
  {
    if (dr->GetCategory() == eDataRequest_Category::Inhaler && dr->GetPropertyName() == property)
      return dr;
  }
  return nullptr;
}
