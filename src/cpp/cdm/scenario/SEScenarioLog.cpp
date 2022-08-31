/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/scenario/SEScenarioLog.h"
#include "cdm/scenario/SEScenario.h"
#include "cdm/engine/SEAdvanceTime.h"
#include "cdm/engine/SEConditionManager.h"
#include "cdm/engine/SEPatientConfiguration.h"
#include "cdm/patient/SEPatient.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/utils/FileUtils.h"

#include <regex>

SEScenarioLog::SEScenarioLog(Logger* logger) : Loggable(logger)
{
  Clear();
}

SEScenarioLog::~SEScenarioLog()
{
  Clear();
}

void SEScenarioLog::Clear()
{
  m_Actions.clear();
  m_AdditionalTime.SetValue(3, TimeUnit::min);;
  m_Conditions.clear();
  m_EOL = "\n";
  m_FinalSimTime_s = 0;
  m_Patient = "";
  m_StateFilename = "";
  m_State = "";
}

bool SEScenarioLog::Convert(const std::string& logFilename, SEScenario& dst)
{
  Clear();
  if (!Extract(logFilename))
  {
    Error("Unable to extract data from log file : " + logFilename);
    return false;
  }

  dst.Clear();
  dst.SetName(logFilename);
  dst.SetDescription("Converted from log file : " + logFilename);
  if (!m_Patient.empty())
  {
    dst.GetPatientConfiguration().GetPatient().SerializeFromString(m_Patient, eSerializationFormat::TEXT);
    for (std::string condition : m_Conditions)
    {
      Error("TODO: Support Conditions");
    }
  }
  else if (!m_StateFilename.empty())
  {
    if (!FileExists(m_StateFilename))
    {
      Warning("Could not find the state file from the log : " + m_StateFilename);
      Warning("Update the scenario or ensure the state file is in the expected location");
    }
    dst.SetEngineStateFile(m_StateFilename);
  }
  else if (!m_State.empty())
  {
    // TODO
    Error("Inline state information not yet supported");
  }
  else
  {
    Warning("Unable to find a starting patient or state file in log.");
    Warning("Setting scenario to use the Standard Male");
    dst.GetPatientConfiguration().SetPatientFile("./patients/StandardMale.json");
  }

  double time_s = 0;
  for (auto itr : m_Actions)
  {
    double actionTime_s = itr.first;
    if (actionTime_s > time_s)
    {
      SEAdvanceTime adv;
      adv.GetTime().SetValue(actionTime_s - time_s, TimeUnit::s);
      time_s += actionTime_s - time_s;
      dst.AddAction(adv);
    }

    for (std::string s : itr.second)
    {
      SEAction* a = SEAction::SerializeFromString(s, eSerializationFormat::TEXT, dst.GetSubstanceManager());
      if (a == nullptr)
        dst.Error("Unable to serialize action : " + s);
      else
        dst.AddAction(*a);
      delete a;
    }
  }

  if (m_FinalSimTime_s > 0)
  {
    SEAdvanceTime adv;
    adv.GetTime().SetValue(m_FinalSimTime_s - time_s, TimeUnit::s);
    time_s += m_FinalSimTime_s - time_s;
    dst.AddAction(adv);
  }
  else
  {
    SEAdvanceTime adv;
    double additionalTime_s = m_AdditionalTime.GetValue(TimeUnit::s);
    adv.GetTime().SetValue(additionalTime_s, TimeUnit::s);
    time_s += additionalTime_s;
    dst.AddAction(adv);
  }
  return true;
}

void SEScenarioLog::AbsentAdditionalTime(const SEScalarTime& time)
{
  m_AdditionalTime = time;
  Info("Additional time set to " + m_AdditionalTime.ToString());
}

bool SEScenarioLog::Extract(const std::string& filename)
{
  std::string content;
  if (!ReadFile(filename, content))
  {
    Error("Failed to read file: " + filename);
    return false;
  }

  Clear();
  DetectEOL(content);

  if (!GetPatient(content))
  {
    Error("Failed to retrieve patient information");
    return false;
  }

  if (!GetSerializeFromFile(content))
  {
    Error("Failed to retrieve serializing from file information");
    return false;
  }

  if (!GetSerializeFromString(content))
  {
    Error("Failed to retrieve serializing from string information");
    return false;
  }

  if (!GetConditions(content))
  {
    Error("Failed to retrieve scenario conditions");
    return false;
  }

  if (!GetActions(content))
  {
    Error("Failed to retrieve scenario actions");
    return false;
  }

  if (!GetFinalSimTime(content))
  {
    Error("Failed to retrieve final sim time");
    return false;
  }

  return true;
}

void SEScenarioLog::DetectEOL(const std::string& content)
{
  std::string EOLchars = "\r\n";
  size_t foundFirst = content.find_first_of(EOLchars);
  if (foundFirst != std::string::npos)
  {
    if (foundFirst + 1 >= content.length())
      return;

    // Remove found character from possibilities so we don't end up getting something like \n\n
    EOLchars.erase(std::remove(EOLchars.begin(), EOLchars.end(), content[foundFirst]), EOLchars.end());
  
    size_t foundLast = content.substr(foundFirst+1).find_first_not_of(EOLchars);

    if (foundLast == std::string::npos)
      foundLast = foundFirst;
    else
      foundLast += foundFirst;

    std::string EOL = content.substr(foundFirst, foundLast + 1 - foundFirst);

    // Double escape escape-chars so that regex can understand
    m_EOL = "";
    for(char c : EOL)
    {
      if (c == '\r')
        m_EOL += "\\r";
      else if (c == '\n')
        m_EOL += "\\n";
      else
        m_EOL += c;
    }
  }
}

bool SEScenarioLog::ExtractTagStrings(const std::string& tag, const std::string& content, std::vector<std::string>& tagStrs)
{
  std::string tagPattern = R"(\[)" + tag + R"(\][ \t]*(?:\d*\.?\d*\(.*\),)?[ \t]*([^\{\n\r]*(\{?)))";
  std::string text = content;

  // Capture groups:
  //  0: Whole match
  //  1: Remainder of line after tag
  //  2: Open bracket if it exists
  std::smatch mTagBegin; 
  std::regex rTagBegin(tagPattern);
  while (std::regex_search(text, mTagBegin, rTagBegin))
  {
    std::string tagStr = "";

    // Locate end of tag string
    size_t endIdx = 0;
    if(mTagBegin[2].compare("{") == 0 && !IdentifyTagStringEnd(mTagBegin.suffix().str(), endIdx))
    {
      Error("Unable to identify tag string terminator: " + std::string(mTagBegin[0]));
      return false;
    }
    tagStr = text.substr(mTagBegin.position(1), mTagBegin.length(1) + endIdx);

    tagStrs.push_back(tagStr);
    text = mTagBegin.suffix().str();
  }

  return true;
}

bool SEScenarioLog::ExtractTagStrings(const std::string& tag, const std::string& content, std::map<double, std::vector<std::string>>& tagStrs)
{
  std::string tagPattern = R"((\[\d*\.?\d*\(.*\)\])\s*\[)" + tag + R"(\][ \t]*(?:\d*\.?\d*\(.*\),)?[ \t]*([^\{\n\r]*(\{?)))";
  std::string text = content;

  // Capture groups:
  //  0: Whole match
  //  1: [Time(unit)]
  //  2: Remainder of line after tag
  //  3: Open bracket if it exists
  std::smatch mTagBegin; 
  std::regex rTagBegin(tagPattern);
  while (std::regex_search(text, mTagBegin, rTagBegin))
  {
    std::string tagStr = "";

    // Parse time
    double time_s;
    ParseTime(mTagBegin[1], time_s);

    // Locate end of tag string
    size_t endIdx = 0;
    if(mTagBegin[3].compare("{") == 0 && !IdentifyTagStringEnd(mTagBegin.suffix().str(), endIdx))
    {
      Error("Unable to identify tag string terminator: " + std::string(mTagBegin[0]));
      return false;
    }
    tagStr = text.substr(mTagBegin.position(2), mTagBegin.length(2) + endIdx);

    if (tagStrs.find(time_s) == tagStrs.end())
    {
      tagStrs[time_s] = std::vector<std::string>();
    }
    tagStrs[time_s].push_back(tagStr);
    text = mTagBegin.suffix().str();
  }

  return true;
}

bool SEScenarioLog::IdentifyTagStringEnd(const std::string& content, size_t& endIdx)
{
  std::smatch mTagEnd;
  std::regex rTagEnd("\\}(" + m_EOL + "){2,}");
  if (std::regex_search(content, mTagEnd, rTagEnd))
  {
    endIdx = mTagEnd.position(0) + 1;
    return true;
  }
  else // Failed to find terminating bracket
  {
    return false;
  }

  return true;
}

bool SEScenarioLog::ParseTime(const std::string& timeStr, double& time_s)
{
  std::smatch mTime;
  std::string timePattern = R"((\d*\.?\d*)\((.*)\))";
  std::regex rTime(timePattern);

  if (!std::regex_search(timeStr, mTime, rTime))
    return false;

  // Capture groups:
  //  0: Whole match
  //  1: Time
  //  2: Time unit
  double rawTime = std::stod(mTime[1]);
  std::string rawUnit = mTime[2];

  // Convert to seconds
  SEScalarTime time;
  time.SetValue(rawTime, TimeUnit(rawUnit));
  time_s = time.GetValue(TimeUnit::s);

  return true;
}

bool SEScenarioLog::GetActions(const std::string& content)
{
  if (!ExtractTagStrings("Action", content, m_Actions))
  {
    Error("Unable to identify action strings");
    return false;
  }

  return true;
}

bool SEScenarioLog::GetConditions(const std::string& content)
{
  if (!ExtractTagStrings("Condition", content, m_Conditions))
  {
    Error("Unable to identify condition strings");
    return false;
  }

  return true;
}

bool SEScenarioLog::GetFinalSimTime(const std::string& content)
{
  std::vector<std::string> finalSimTimeStrs;
  if (!ExtractTagStrings("Final SimTime", content, finalSimTimeStrs))
  {
    m_FinalSimTime_s = 0;
    Error("Unable to identify Final SimTime string");
    return false;
  }

  if (finalSimTimeStrs.size() == 0)
  {
    m_FinalSimTime_s = 0;
    Info("No Final SimTime string provided in log");
    return true;
  }

  if (!ParseTime(finalSimTimeStrs[0], m_FinalSimTime_s))
  {
    Warning("Unable to parse Final SimTime string");
    return true;
  }

  return true;
}

bool SEScenarioLog::GetPatient(const std::string& content)
{
  // Capture groups:
  //  0: Whole match
  //  1: Remainder of line, left trimmed (name info)
  std::string patientPattern = R"(\[Patient\]\s*([^\n\r]*))";
  std::smatch mPatient; 
  std::regex rPatient(patientPattern);
  std::string text = content;
  if (!std::regex_search(text, mPatient, rPatient))
  {
    Info("No patient information");
    return true;
  }

  // Locate end of patient info
  size_t endIdx;
  if(!IdentifyTagStringEnd(mPatient.suffix().str(), endIdx))
  {
    Error("Unable to identify tag string terminator: " + std::string(mPatient[0]));
    return false;
  }
  m_Patient = text.substr(mPatient.position(1), mPatient.length(1) + endIdx);

  return true;
}

bool SEScenarioLog::GetSerializeFromFile(const std::string& content)
{
  std::vector<std::string> serializeFromFileStrs;
  if (!ExtractTagStrings("SerializingFromFile", content, serializeFromFileStrs))
  {
    Error("Unable to identify SerializingFromFile strings");
    return false;
  }

  if (serializeFromFileStrs.size() > 0)
  {
    m_StateFilename = serializeFromFileStrs[serializeFromFileStrs.size() - 1];
  }
  else
    Info("No State filename found");

  return true;
}

bool SEScenarioLog::GetSerializeFromString(const std::string& content)
{
  std::vector<std::string> serializeFromStringStrs;
  if (!ExtractTagStrings("SerializingFromString", content, serializeFromStringStrs))
  {
    Error("Unable to identify SerializingFromString strings");
    return false;
  }

  if (serializeFromStringStrs.size() > 0)
  {
    m_State = serializeFromStringStrs[serializeFromStringStrs.size() - 1];
  }
  else
    Info("No State string found");

  return true;
}


