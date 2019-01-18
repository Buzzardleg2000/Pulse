/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "io/protobuf/PBElectroCardioGram.h"
#include "io/protobuf/PBProperties.h"
#include "io/protobuf/PBUtils.h"
#include "bind/cpp/cdm/ElectroCardioGram.pb.h"
#include "system/equipment/electrocardiogram/SEElectroCardioGram.h"
#include "system/equipment/electrocardiogram/SEElectroCardioGramWaveform.h"
#include "system/equipment/electrocardiogram/SEElectroCardioGramWaveformInterpolator.h"
#include "utils/FileUtils.h"


void PBElectroCardioGram::Load(const cdm::ElectroCardioGramData& src, SEElectroCardioGram& dst)
{
  PBElectroCardioGram::Serialize(src, dst);
}
void PBElectroCardioGram::Serialize(const cdm::ElectroCardioGramData& src, SEElectroCardioGram& dst)
{
  if (src.has_lead1electricpotential())
    PBProperty::Load(src.lead1electricpotential(), dst.GetLead1ElectricPotential());
  if (src.has_lead2electricpotential())
    PBProperty::Load(src.lead2electricpotential(), dst.GetLead2ElectricPotential());
  if (src.has_lead3electricpotential())
    PBProperty::Load(src.lead3electricpotential(), dst.GetLead3ElectricPotential());
  if (src.has_lead4electricpotential())
    PBProperty::Load(src.lead4electricpotential(), dst.GetLead4ElectricPotential());
  if (src.has_lead5electricpotential())
    PBProperty::Load(src.lead5electricpotential(), dst.GetLead5ElectricPotential());
  if (src.has_lead6electricpotential())
    PBProperty::Load(src.lead6electricpotential(), dst.GetLead6ElectricPotential());
  if (src.has_lead7electricpotential())
    PBProperty::Load(src.lead7electricpotential(), dst.GetLead7ElectricPotential());
  if (src.has_lead8electricpotential())
    PBProperty::Load(src.lead8electricpotential(), dst.GetLead8ElectricPotential());
  if (src.has_lead9electricpotential())
    PBProperty::Load(src.lead9electricpotential(), dst.GetLead9ElectricPotential());
  if (src.has_lead10electricpotential())
    PBProperty::Load(src.lead10electricpotential(), dst.GetLead10ElectricPotential());
  if (src.has_lead11electricpotential())
    PBProperty::Load(src.lead11electricpotential(), dst.GetLead11ElectricPotential());
  if (src.has_lead12electricpotential())
    PBProperty::Load(src.lead12electricpotential(), dst.GetLead12ElectricPotential());
}
cdm::ElectroCardioGramData* PBElectroCardioGram::Unload(const SEElectroCardioGram& src)
{
  cdm::ElectroCardioGramData* dst = new cdm::ElectroCardioGramData();
  PBElectroCardioGram::Serialize(src, *dst);
  return dst;
}
void PBElectroCardioGram::Serialize(const SEElectroCardioGram& src, cdm::ElectroCardioGramData& dst)
{
  if (src.HasLead1ElectricPotential())
    dst.set_allocated_lead1electricpotential(PBProperty::Unload(*src.m_Lead1ElectricPotential));
  if (src.HasLead2ElectricPotential())
    dst.set_allocated_lead2electricpotential(PBProperty::Unload(*src.m_Lead2ElectricPotential));
  if (src.HasLead3ElectricPotential())
    dst.set_allocated_lead3electricpotential(PBProperty::Unload(*src.m_Lead3ElectricPotential));
  if (src.HasLead4ElectricPotential())
    dst.set_allocated_lead4electricpotential(PBProperty::Unload(*src.m_Lead4ElectricPotential));
  if (src.HasLead5ElectricPotential())
    dst.set_allocated_lead5electricpotential(PBProperty::Unload(*src.m_Lead5ElectricPotential));
  if (src.HasLead6ElectricPotential())
    dst.set_allocated_lead6electricpotential(PBProperty::Unload(*src.m_Lead6ElectricPotential));
  if (src.HasLead7ElectricPotential())
    dst.set_allocated_lead7electricpotential(PBProperty::Unload(*src.m_Lead7ElectricPotential));
  if (src.HasLead8ElectricPotential())
    dst.set_allocated_lead8electricpotential(PBProperty::Unload(*src.m_Lead8ElectricPotential));
  if (src.HasLead9ElectricPotential())
    dst.set_allocated_lead9electricpotential(PBProperty::Unload(*src.m_Lead9ElectricPotential));
  if (src.HasLead10ElectricPotential())
    dst.set_allocated_lead10electricpotential(PBProperty::Unload(*src.m_Lead10ElectricPotential));
  if (src.HasLead11ElectricPotential())
    dst.set_allocated_lead11electricpotential(PBProperty::Unload(*src.m_Lead11ElectricPotential));
  if (src.HasLead12ElectricPotential())
    dst.set_allocated_lead12electricpotential(PBProperty::Unload(*src.m_Lead12ElectricPotential));
}

void PBElectroCardioGram::Load(const cdm::ElectroCardioGramWaveformData& src, SEElectroCardioGramWaveform& dst)
{
  PBElectroCardioGram::Serialize(src, dst);
}
void PBElectroCardioGram::Serialize(const cdm::ElectroCardioGramWaveformData& src, SEElectroCardioGramWaveform& dst)
{
  dst.Clear();
  dst.m_Rhythm = (eHeartRhythm)src.rhythm();
  dst.m_LeadNumber = (eElectroCardioGram_WaveformLead)src.lead();
  if (src.has_timestep())
    PBProperty::Load(src.timestep(), dst.GetTimeStep());
  if (src.has_data())
    PBProperty::Load(src.data(), dst.GetData());
  for (int i = 0; i < src.activeindicies_size(); i++)
    dst.m_ActiveIndicies.push_back(src.activeindicies()[i]);
}
cdm::ElectroCardioGramWaveformData* PBElectroCardioGram::Unload(const SEElectroCardioGramWaveform& src)
{
  cdm::ElectroCardioGramWaveformData* dst = new cdm::ElectroCardioGramWaveformData();
  PBElectroCardioGram::Serialize(src, *dst);
  return dst;
}
void PBElectroCardioGram::Serialize(const SEElectroCardioGramWaveform& src, cdm::ElectroCardioGramWaveformData& dst)
{
  dst.set_rhythm((cdm::eHeartRhythm)src.m_Rhythm);
  dst.set_lead((cdm::eElectroCardioGram_WaveformLead)src.m_LeadNumber);
  if (src.HasData())
  {
    dst.set_allocated_data(PBProperty::Unload(*src.m_Data));
    for (int i : src.m_ActiveIndicies)
      dst.add_activeindicies(i);
  }
  if (src.HasTimeStep())
    dst.set_allocated_timestep(PBProperty::Unload(*src.m_TimeStep));
}

void PBElectroCardioGram::Load(const cdm::ElectroCardioGramWaveformListData& src, SEElectroCardioGramWaveformInterpolator& dst)
{
  PBElectroCardioGram::Serialize(src, dst);
}
void PBElectroCardioGram::Serialize(const cdm::ElectroCardioGramWaveformListData& src, SEElectroCardioGramWaveformInterpolator& dst)
{
  dst.Clear();
  for (int i = 0; i < src.waveform_size(); i++)
  {
    SEElectroCardioGramWaveform* waveform = new SEElectroCardioGramWaveform(dst.GetLogger());
    PBElectroCardioGram::Load(src.waveform()[i], *waveform);
    dst.m_Waveforms[waveform->GetLeadNumber()][waveform->GetRhythm()] = waveform;
  }
}
cdm::ElectroCardioGramWaveformListData* PBElectroCardioGram::Unload(const SEElectroCardioGramWaveformInterpolator& src)
{
  cdm::ElectroCardioGramWaveformListData* dst = new cdm::ElectroCardioGramWaveformListData();
  PBElectroCardioGram::Serialize(src, *dst);
  return dst;
}
void PBElectroCardioGram::Serialize(const SEElectroCardioGramWaveformInterpolator& src, cdm::ElectroCardioGramWaveformListData& dst)
{
  for (auto i : src.m_Waveforms)
    for (auto j : i.second)
      dst.mutable_waveform()->AddAllocated(PBElectroCardioGram::Unload(*j.second));
}
void PBElectroCardioGram::Copy(const SEElectroCardioGramWaveformInterpolator& src, SEElectroCardioGramWaveformInterpolator& dst)
{
  cdm::ElectroCardioGramWaveformListData data;
  PBElectroCardioGram::Serialize(src, data);
  PBElectroCardioGram::Serialize(data, dst);
}

bool PBElectroCardioGram::SerializeToString(const SEElectroCardioGramWaveformInterpolator& src, std::string& output, SerializationFormat m)
{
  cdm::ElectroCardioGramWaveformListData data;
  PBElectroCardioGram::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBElectroCardioGram::SerializeToFile(const SEElectroCardioGramWaveformInterpolator& src, const std::string& filename, SerializationFormat m)
{
  cdm::ElectroCardioGramWaveformListData data;
  PBElectroCardioGram::Serialize(src, data);
  std::string content;
  PBElectroCardioGram::SerializeToString(src, content, m);
  return WriteFile(content, filename, m);
}
bool PBElectroCardioGram::SerializeFromString(const std::string& src, SEElectroCardioGramWaveformInterpolator& dst, SerializationFormat m)
{
  cdm::ElectroCardioGramWaveformListData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBElectroCardioGram::Load(data, dst);
  return true;
}
bool PBElectroCardioGram::SerializeFromFile(const std::string& filename, SEElectroCardioGramWaveformInterpolator& dst, SerializationFormat m)
{
  std::string content = ReadFile(filename, m);
  if (content.empty())
    return false;
  return PBElectroCardioGram::SerializeFromString(content, dst, m);
}

