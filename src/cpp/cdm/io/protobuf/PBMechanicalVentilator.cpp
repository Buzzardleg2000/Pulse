/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
PUSH_PROTO_WARNINGS
#include "pulse/cdm/bind/MechanicalVentilator.pb.h"
POP_PROTO_WARNINGS
#include "cdm/io/protobuf/PBMechanicalVentilator.h"
#include "cdm/io/protobuf/PBSubstance.h"
#include "cdm/io/protobuf/PBProperties.h"
#include "cdm/io/protobuf/PBUtils.h"
#include "cdm/system/equipment/mechanical_ventilator/SEMechanicalVentilator.h"
#include "cdm/substance/SESubstance.h"
#include "cdm/substance/SESubstanceManager.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/utils/FileUtils.h"

void PBMechanicalVentilator::Load(const CDM_BIND::MechanicalVentilatorData& src, SEMechanicalVentilator& dst, const SESubstanceManager& subMgr)
{
  dst.Clear();
  PBMechanicalVentilator::Serialize(src, dst, subMgr);
  dst.StateChange();
}
void PBMechanicalVentilator::Serialize(const CDM_BIND::MechanicalVentilatorData& src, SEMechanicalVentilator& dst, const SESubstanceManager& subMgr)
{
  if (src.has_airwaypressure())
    PBProperty::Load(src.airwaypressure(), dst.GetAirwayPressure());
  if (src.has_dynamicpulmonarycompliance())
    PBProperty::Load(src.dynamicpulmonarycompliance(), dst.GetDynamicPulmonaryCompliance());
  if (src.has_endtidalcarbondioxidefraction())
    PBProperty::Load(src.endtidalcarbondioxidefraction(), dst.GetEndTidalCarbonDioxideFraction());
  if (src.has_endtidalcarbondioxidepressure())
    PBProperty::Load(src.endtidalcarbondioxidepressure(), dst.GetEndTidalCarbonDioxidePressure());
  if (src.has_endtidaloxygenfraction())
    PBProperty::Load(src.endtidaloxygenfraction(), dst.GetEndTidalOxygenFraction());
  if (src.has_endtidaloxygenpressure())
    PBProperty::Load(src.endtidaloxygenpressure(), dst.GetEndTidalOxygenPressure());
  if (src.has_expiratoryflow())
    PBProperty::Load(src.expiratoryflow(), dst.GetExpiratoryFlow());
  if (src.has_expiratorytidalvolume())
    PBProperty::Load(src.expiratorytidalvolume(), dst.GetExpiratoryTidalVolume());
  if (src.has_inspiratoryexpiratoryratio())
    PBProperty::Load(src.inspiratoryexpiratoryratio(), dst.GetInspiratoryExpiratoryRatio());
  if (src.has_inspiratoryflow())
    PBProperty::Load(src.inspiratoryflow(), dst.GetInspiratoryFlow());
  if (src.has_inspiratorytidalvolume())
    PBProperty::Load(src.inspiratorytidalvolume(), dst.GetInspiratoryTidalVolume());
  if (src.has_intrinsicpositiveendexpiredpressure())
    PBProperty::Load(src.intrinsicpositiveendexpiredpressure(), dst.GetIntrinsicPositiveEndExpiredPressure());
  if (src.has_leakfraction())
    PBProperty::Load(src.leakfraction(), dst.GetLeakFraction());
  if (src.has_meanairwaypressure())
    PBProperty::Load(src.meanairwaypressure(), dst.GetMeanAirwayPressure());
  if (src.has_peakinspiratorypressure())
    PBProperty::Load(src.peakinspiratorypressure(), dst.GetPeakInspiratoryPressure());
  if (src.has_plateaupressure())
    PBProperty::Load(src.plateaupressure(), dst.GetPlateauPressure());
  if (src.has_positiveendexpiratorypressure())
    PBProperty::Load(src.positiveendexpiratorypressure(), dst.GetPositiveEndExpiratoryPressure());
  if (src.has_pulmonaryresistance())
    PBProperty::Load(src.pulmonaryresistance(), dst.GetPulmonaryResistance());
  if (src.has_respirationrate())
    PBProperty::Load(src.respirationrate(), dst.GetRespirationRate());
  if (src.has_staticpulmonarycompliance())
    PBProperty::Load(src.staticpulmonarycompliance(), dst.GetStaticPulmonaryCompliance());
  if (src.has_tidalvolume())
    PBProperty::Load(src.tidalvolume(), dst.GetTidalVolume());
  if (src.has_totallungvolume())
    PBProperty::Load(src.totallungvolume(), dst.GetTotalLungVolume());
  if (src.has_totalpulmonaryventilation())
    PBProperty::Load(src.totalpulmonaryventilation(), dst.GetTotalPulmonaryVentilation());

  if (src.has_settings())
    PBMechanicalVentilator::Load(src.settings(), dst.GetSettings(), subMgr);
}
CDM_BIND::MechanicalVentilatorData* PBMechanicalVentilator::Unload(const SEMechanicalVentilator& src)
{
  CDM_BIND::MechanicalVentilatorData* dst = new CDM_BIND::MechanicalVentilatorData();
  PBMechanicalVentilator::Serialize(src, *dst);
  return dst;
}
void PBMechanicalVentilator::Serialize(const SEMechanicalVentilator& src, CDM_BIND::MechanicalVentilatorData& dst)
{
  if (src.HasAirwayPressure())
    dst.set_allocated_airwaypressure(PBProperty::Unload(*src.m_AirwayPressure));
  if (src.HasDynamicPulmonaryCompliance())
    dst.set_allocated_dynamicpulmonarycompliance(PBProperty::Unload(*src.m_DynamicPulmonaryCompliance));
  if (src.HasEndTidalCarbonDioxideFraction())
    dst.set_allocated_endtidalcarbondioxidefraction(PBProperty::Unload(*src.m_EndTidalCarbonDioxideFraction));
  if (src.HasEndTidalCarbonDioxidePressure())
    dst.set_allocated_endtidalcarbondioxidepressure(PBProperty::Unload(*src.m_EndTidalCarbonDioxidePressure));
  if (src.HasEndTidalOxygenFraction())
    dst.set_allocated_endtidaloxygenfraction(PBProperty::Unload(*src.m_EndTidalOxygenFraction));
  if (src.HasEndTidalOxygenPressure())
    dst.set_allocated_endtidaloxygenpressure(PBProperty::Unload(*src.m_EndTidalOxygenPressure));
  if (src.HasExpiratoryFlow())
    dst.set_allocated_expiratoryflow(PBProperty::Unload(*src.m_ExpiratoryFlow));
  if (src.HasExpiratoryTidalVolume())
    dst.set_allocated_expiratorytidalvolume(PBProperty::Unload(*src.m_ExpiratoryTidalVolume));
  if (src.HasInspiratoryExpiratoryRatio())
    dst.set_allocated_inspiratoryexpiratoryratio(PBProperty::Unload(*src.m_InspiratoryExpiratoryRatio));
  if (src.HasInspiratoryFlow())
    dst.set_allocated_inspiratoryflow(PBProperty::Unload(*src.m_InspiratoryFlow));
  if (src.HasInspiratoryTidalVolume())
    dst.set_allocated_inspiratorytidalvolume(PBProperty::Unload(*src.m_InspiratoryTidalVolume));
  if (src.HasIntrinsicPositiveEndExpiredPressure())
    dst.set_allocated_intrinsicpositiveendexpiredpressure(PBProperty::Unload(*src.m_IntrinsicPositiveEndExpiredPressure));
  if (src.HasLeakFraction())
    dst.set_allocated_leakfraction(PBProperty::Unload(*src.m_LeakFraction));
  if (src.HasMeanAirwayPressure())
    dst.set_allocated_meanairwaypressure(PBProperty::Unload(*src.m_MeanAirwayPressure));
  if (src.HasPeakInspiratoryPressure())
    dst.set_allocated_peakinspiratorypressure(PBProperty::Unload(*src.m_PeakInspiratoryPressure));
  if (src.HasPlateauPressure())
    dst.set_allocated_plateaupressure(PBProperty::Unload(*src.m_PlateauPressure));
  if (src.HasPositiveEndExpiratoryPressure())
    dst.set_allocated_positiveendexpiratorypressure(PBProperty::Unload(*src.m_PositiveEndExpiratoryPressure));
  if (src.HasPulmonaryResistance())
    dst.set_allocated_pulmonaryresistance(PBProperty::Unload(*src.m_PulmonaryResistance));
  if (src.HasRespirationRate())
    dst.set_allocated_respirationrate(PBProperty::Unload(*src.m_RespirationRate));
  if (src.HasStaticPulmonaryCompliance())
    dst.set_allocated_staticpulmonarycompliance(PBProperty::Unload(*src.m_StaticPulmonaryCompliance));
  if (src.HasTidalVolume())
    dst.set_allocated_tidalvolume(PBProperty::Unload(*src.m_TidalVolume));
  if (src.HasTotalLungVolume())
    dst.set_allocated_totallungvolume(PBProperty::Unload(*src.m_TotalLungVolume));
  if (src.HasTotalPulmonaryVentilation())
    dst.set_allocated_totalpulmonaryventilation(PBProperty::Unload(*src.m_TotalPulmonaryVentilation));

  if (src.HasSettings())
    dst.set_allocated_settings(PBMechanicalVentilator::Unload(*src.m_Settings));
}

void PBMechanicalVentilator::Load(const CDM_BIND::MechanicalVentilatorSettingsData& src, SEMechanicalVentilatorSettings& dst, const SESubstanceManager& subMgr)
{
  dst.Clear();
  PBMechanicalVentilator::Serialize(src, dst, subMgr);
}
void PBMechanicalVentilator::Serialize(const CDM_BIND::MechanicalVentilatorSettingsData& src, SEMechanicalVentilatorSettings& dst, const SESubstanceManager& subMgr)
{
  dst.m_Connection = (eSwitch)src.connection();
  if (src.has_connectionvolume())
    PBProperty::Load(src.connectionvolume(), dst.GetConnectionVolume());
  if (src.has_compliance())
    PBProperty::Load(src.compliance(), dst.GetCompliance());
  if (src.has_driverdampingparameter())
    PBProperty::Load(src.driverdampingparameter(), dst.GetDriverDampingParameter());

  if (src.has_positiveendexpiredpressure())
    PBProperty::Load(src.positiveendexpiredpressure(), dst.GetPositiveEndExpiredPressure());
  if (src.has_functionalresidualcapacity())
    PBProperty::Load(src.functionalresidualcapacity(), dst.GetFunctionalResidualCapacity());

  if (src.has_expirationcycleflow())
    PBProperty::Load(src.expirationcycleflow(), dst.GetExpirationCycleFlow());
  if (src.has_expirationcyclepressure())
    PBProperty::Load(src.expirationcyclepressure(), dst.GetExpirationCyclePressure());
  if (src.has_expirationcyclevolume())
    PBProperty::Load(src.expirationcyclevolume(), dst.GetExpirationCycleVolume());
  if (src.has_expirationcycletime())
    PBProperty::Load(src.expirationcycletime(), dst.GetExpirationCycleTime());
  dst.m_ExpirationCycleRespiratoryModel = (eSwitch)src.expirationcyclerespiratorymodel();

  if (src.has_expirationlimbvolume())
    PBProperty::Load(src.expirationlimbvolume(), dst.GetExpirationLimbVolume());
  if (src.has_expirationtuberesistance())
    PBProperty::Load(src.expirationtuberesistance(), dst.GetExpirationTubeResistance());
  if (src.has_expirationvalveresistance())
    PBProperty::Load(src.expirationvalveresistance(), dst.GetExpirationValveResistance());
  if (src.has_expirationvalvevolume())
    PBProperty::Load(src.expirationvalvevolume(), dst.GetExpirationValveVolume());
  dst.SetExpirationWaveform((eMechanicalVentilator_DriverWaveform)src.expirationwaveform());
  if (src.has_expirationwaveformperiod())
    PBProperty::Load(src.expirationwaveformperiod(), dst.GetExpirationWaveformPeriod());

  if (src.has_inspirationlimitflow())
    PBProperty::Load(src.inspirationlimitflow(), dst.GetInspirationLimitFlow());
  if (src.has_inspirationlimitpressure())
    PBProperty::Load(src.inspirationlimitpressure(), dst.GetInspirationLimitPressure());
  if (src.has_inspirationlimitvolume())
    PBProperty::Load(src.inspirationlimitvolume(), dst.GetInspirationLimitVolume());

  if (src.has_inspirationpausetime())
    PBProperty::Load(src.inspirationpausetime(), dst.GetInspirationPauseTime());

  if (src.has_peakinspiratorypressure())
    PBProperty::Load(src.peakinspiratorypressure(), dst.GetPeakInspiratoryPressure());
  if (src.has_inspirationtargetflow())
    PBProperty::Load(src.inspirationtargetflow(), dst.GetInspirationTargetFlow());

  if (src.has_inspirationmachinetriggertime())
    PBProperty::Load(src.inspirationmachinetriggertime(), dst.GetInspirationMachineTriggerTime());

  if (src.has_inspirationpatienttriggerflow())
    PBProperty::Load(src.inspirationpatienttriggerflow(), dst.GetInspirationPatientTriggerFlow());
  if (src.has_inspirationpatienttriggerpressure())
    PBProperty::Load(src.inspirationpatienttriggerpressure(), dst.GetInspirationPatientTriggerPressure());
  dst.m_InspirationPatientTriggerRespiratoryModel = (eSwitch)src.inspirationpatienttriggerrespiratorymodel();

  if (src.has_inspirationlimbvolume())
    PBProperty::Load(src.inspirationlimbvolume(), dst.GetInspirationLimbVolume());
  if (src.has_inspirationtuberesistance())
    PBProperty::Load(src.inspirationtuberesistance(), dst.GetInspirationTubeResistance());
  if (src.has_inspirationvalveresistance())
    PBProperty::Load(src.inspirationvalveresistance(), dst.GetInspirationValveResistance());
  if (src.has_inspirationvalvevolume())
    PBProperty::Load(src.inspirationvalvevolume(), dst.GetInspirationValveVolume());
  dst.SetInspirationWaveform((eMechanicalVentilator_DriverWaveform)src.inspirationwaveform());
  if (src.has_inspirationwaveformperiod())
    PBProperty::Load(src.inspirationwaveformperiod(), dst.GetInspirationWaveformPeriod());

  if (src.has_ypiecevolume())
    PBProperty::Load(src.ypiecevolume(), dst.GetYPieceVolume());

  const SESubstance* sub;
  for (int i = 0; i < src.fractioninspiredgas_size(); i++)
  {
    const CDM_BIND::SubstanceFractionData& sfData = src.fractioninspiredgas()[i];
    sub = subMgr.GetSubstance(sfData.name());
    if (sub == nullptr)
    {
      dst.Error("Ignoring a mechanical ventilator fraction inspired gas that was not found : " + sfData.name());
      continue;
    }
    if (sub->GetState() != eSubstance_State::Gas)
    {
      dst.Error("Ignoring a mechanical ventilator fraction gas that is not a gas : " + sfData.name());
      continue;
    }
    PBSubstance::Load(sfData, dst.GetFractionInspiredGas(*sub));
  }

  for (int i = 0; i < src.concentrationinspiredaerosol_size(); i++)
  {
    const CDM_BIND::SubstanceConcentrationData& scData = src.concentrationinspiredaerosol()[i];
    sub = subMgr.GetSubstance(scData.name());
    if (sub == nullptr)
    {
      dst.Error("Ignoring an mechanical ventilator aerosol concentration that was not found : " + scData.name());
      continue;
    }
    if (sub->GetState() != eSubstance_State::Liquid && sub->GetState() != eSubstance_State::Solid)
    {
      dst.Error("Ignoring an mechanical ventilator aerosol concentration that is not a gas : " + scData.name());
      continue;
    }
    PBSubstance::Load(scData, dst.GetConcentrationInspiredAerosol(*sub));
  }
}

CDM_BIND::MechanicalVentilatorSettingsData* PBMechanicalVentilator::Unload(const SEMechanicalVentilatorSettings& src)
{
  CDM_BIND::MechanicalVentilatorSettingsData* dst = new CDM_BIND::MechanicalVentilatorSettingsData();
  PBMechanicalVentilator::Serialize(src, *dst);
  return dst;
}
void PBMechanicalVentilator::Serialize(const SEMechanicalVentilatorSettings& src, CDM_BIND::MechanicalVentilatorSettingsData& dst)
{
  dst.set_connection((CDM_BIND::eSwitch)src.m_Connection);
  if (src.HasConnectionVolume())
    dst.set_allocated_connectionvolume(PBProperty::Unload(*src.m_ConnectionVolume));
  if (src.HasCompliance())
    dst.set_allocated_compliance(PBProperty::Unload(*src.m_Compliance));
  if (src.HasDriverDampingParameter())
    dst.set_allocated_driverdampingparameter(PBProperty::Unload(*src.m_DriverDampingParameter));

  if (src.HasPositiveEndExpiredPressure())
    dst.set_allocated_positiveendexpiredpressure(PBProperty::Unload(*src.m_PositiveEndExpiredPressure));
  if (src.HasFunctionalResidualCapacity())
    dst.set_allocated_functionalresidualcapacity(PBProperty::Unload(*src.m_FunctionalResidualCapacity));

  if (src.HasExpirationCycleFlow())
    dst.set_allocated_expirationcycleflow(PBProperty::Unload(*src.m_ExpirationCycleFlow));
  if (src.HasExpirationCyclePressure())
    dst.set_allocated_expirationcyclepressure(PBProperty::Unload(*src.m_ExpirationCyclePressure));
  if (src.HasExpirationCycleVolume())
    dst.set_allocated_expirationcyclevolume(PBProperty::Unload(*src.m_ExpirationCycleVolume));
  if (src.HasExpirationCycleTime())
    dst.set_allocated_expirationcycletime(PBProperty::Unload(*src.m_ExpirationCycleTime));

  if (src.HasExpirationLimbVolume())
    dst.set_allocated_expirationlimbvolume(PBProperty::Unload(*src.m_ExpirationLimbVolume));
  if (src.HasExpirationTubeResistance())
    dst.set_allocated_expirationtuberesistance(PBProperty::Unload(*src.m_ExpirationTubeResistance));
  if (src.HasExpirationValveVolume())
    dst.set_allocated_expirationvalvevolume(PBProperty::Unload(*src.m_ExpirationValveVolume));
  if (src.HasExpirationValveResistance())
    dst.set_allocated_expirationvalveresistance(PBProperty::Unload(*src.m_ExpirationValveResistance));
  dst.set_expirationwaveform((CDM_BIND::MechanicalVentilatorSettingsData::eDriverWaveform)src.m_ExpirationWaveform);
  if (src.HasExpirationWaveformPeriod())
    dst.set_allocated_expirationwaveformperiod(PBProperty::Unload(*src.m_ExpirationWaveformPeriod));

  if (src.HasInspirationLimitFlow())
    dst.set_allocated_inspirationlimitflow(PBProperty::Unload(*src.m_InspirationLimitFlow));
  if (src.HasInspirationLimitPressure())
    dst.set_allocated_inspirationlimitpressure(PBProperty::Unload(*src.m_InspirationLimitPressure));
  if (src.HasInspirationLimitVolume())
    dst.set_allocated_inspirationlimitvolume(PBProperty::Unload(*src.m_InspirationLimitVolume));

  if (src.HasInspirationPauseTime())
    dst.set_allocated_inspirationpausetime(PBProperty::Unload(*src.m_InspirationPauseTime));

  if (src.HasPeakInspiratoryPressure())
    dst.set_allocated_peakinspiratorypressure(PBProperty::Unload(*src.m_PeakInspiratoryPressure));
  if (src.HasInspirationTargetFlow())
    dst.set_allocated_inspirationtargetflow(PBProperty::Unload(*src.m_InspirationTargetFlow));

  if (src.HasInspirationMachineTriggerTime())
    dst.set_allocated_inspirationmachinetriggertime(PBProperty::Unload(*src.m_InspirationMachineTriggerTime));

  if (src.HasInspirationPatientTriggerFlow())
    dst.set_allocated_inspirationpatienttriggerflow(PBProperty::Unload(*src.m_InspirationPatientTriggerFlow));
  if (src.HasInspirationPatientTriggerPressure())
    dst.set_allocated_inspirationpatienttriggerpressure(PBProperty::Unload(*src.m_InspirationPatientTriggerPressure));  

  if (src.HasInspirationTubeResistance())
    dst.set_allocated_inspirationtuberesistance(PBProperty::Unload(*src.m_InspirationTubeResistance));
  if (src.HasInspirationValveResistance())
    dst.set_allocated_inspirationvalveresistance(PBProperty::Unload(*src.m_InspirationValveResistance));
  if (src.HasInspirationLimbVolume())
    dst.set_allocated_inspirationlimbvolume(PBProperty::Unload(*src.m_InspirationLimbVolume));
  if (src.HasInspirationValveVolume())
    dst.set_allocated_inspirationvalvevolume(PBProperty::Unload(*src.m_InspirationValveVolume));
  dst.set_inspirationwaveform((CDM_BIND::MechanicalVentilatorSettingsData::eDriverWaveform)src.m_InspirationWaveform);
  if (src.HasInspirationWaveformPeriod())
    dst.set_allocated_inspirationwaveformperiod(PBProperty::Unload(*src.m_InspirationWaveformPeriod));

  if (src.HasYPieceVolume())
    dst.set_allocated_ypiecevolume(PBProperty::Unload(*src.m_YPieceVolume));

  for (SESubstanceFraction* sf : src.m_FractionInspiredGases)
    dst.mutable_fractioninspiredgas()->AddAllocated(PBSubstance::Unload(*sf));

  for (SESubstanceConcentration* sc : src.m_ConcentrationInspiredAerosols)
    dst.mutable_concentrationinspiredaerosol()->AddAllocated(PBSubstance::Unload(*sc));
}

bool PBMechanicalVentilator::SerializeToString(const SEMechanicalVentilatorSettings& src, std::string& output, eSerializationFormat m)
{
  CDM_BIND::MechanicalVentilatorSettingsData data;
  PBMechanicalVentilator::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBMechanicalVentilator::SerializeToFile(const SEMechanicalVentilatorSettings& src, const std::string& filename)
{
  CDM_BIND::MechanicalVentilatorSettingsData data;
  PBMechanicalVentilator::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}

bool PBMechanicalVentilator::SerializeFromString(const std::string& src, SEMechanicalVentilatorSettings& dst, eSerializationFormat m, const SESubstanceManager& subMgr)
{
  CDM_BIND::MechanicalVentilatorSettingsData data;
  if (!PBUtils::SerializeFromString(src, data, m, dst.GetLogger()))
    return false;
  PBMechanicalVentilator::Load(data, dst, subMgr);
  return true;
}
bool PBMechanicalVentilator::SerializeFromFile(const std::string& filename, SEMechanicalVentilatorSettings& dst, const SESubstanceManager& subMgr)
{
  CDM_BIND::MechanicalVentilatorSettingsData data;
  if (!PBUtils::SerializeFromFile(filename, data, dst.GetLogger()))
    return false;
  PBMechanicalVentilator::Load(data, dst, subMgr);
  return true;
}
