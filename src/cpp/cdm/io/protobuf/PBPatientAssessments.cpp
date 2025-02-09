/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
PUSH_PROTO_WARNINGS
#include "pulse/cdm/bind/PatientAssessments.pb.h"
POP_PROTO_WARNINGS
#include "cdm/io/protobuf/PBUtils.h"
#include "cdm/io/protobuf/PBPatientAssessments.h"
#include "cdm/io/protobuf/PBProperties.h"
#include "cdm/patient/assessments/SEArterialBloodGasTest.h"
#include "cdm/patient/assessments/SECompleteBloodCount.h"
#include "cdm/patient/assessments/SEComprehensiveMetabolicPanel.h"
#include "cdm/patient/assessments/SEUrinalysis.h"
#include "cdm/patient/assessments/SEUrinalysisMicroscopic.h"
#include "cdm/utils/FileUtils.h"


void PBPatientAssessment::Serialize(const CDM_BIND::PatientAssessmentData& /*src*/, SEPatientAssessment& /*dst*/)
{

}

void PBPatientAssessment::Serialize(const SEPatientAssessment& /*src*/, CDM_BIND::PatientAssessmentData& /*dst*/)
{

}

bool PBPatientAssessment::SerializeToString(const SEArterialBloodGasTest& src, std::string& output, eSerializationFormat m)
{
  CDM_BIND::ArterialBloodGasTestData data;
  PBPatientAssessment::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBPatientAssessment::SerializeToFile(const SEArterialBloodGasTest& src, const std::string& filename)
{
  CDM_BIND::ArterialBloodGasTestData data;
  PBPatientAssessment::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}

void PBPatientAssessment::Load(const CDM_BIND::ArterialBloodGasTestData& src, SEArterialBloodGasTest& dst)
{
  dst.Clear();
  PBPatientAssessment::Serialize(src, dst);
}
void PBPatientAssessment::Serialize(const CDM_BIND::ArterialBloodGasTestData& src, SEArterialBloodGasTest& dst)
{
  PBPatientAssessment::Serialize(src.patientassessment(), dst);
  if (src.has_bloodph())
    PBProperty::Load(src.bloodph(), dst.GetBloodPH());
  if (src.has_bicarbonate())
    PBProperty::Load(src.bicarbonate(), dst.GetBicarbonate());
  if (src.has_partialpressureofoxygen())
    PBProperty::Load(src.partialpressureofoxygen(), dst.GetPartialPressureOfOxygen());
  if (src.has_partialpressureofcarbondioxide())
    PBProperty::Load(src.partialpressureofcarbondioxide(), dst.GetPartialPressureOfCarbonDioxide());
  if (src.has_oxygensaturation())
    PBProperty::Load(src.oxygensaturation(), dst.GetOxygenSaturation());
}

CDM_BIND::ArterialBloodGasTestData* PBPatientAssessment::Unload(const SEArterialBloodGasTest& src)
{
  CDM_BIND::ArterialBloodGasTestData* dst = new CDM_BIND::ArterialBloodGasTestData();
  PBPatientAssessment::Serialize(src, *dst);
  return dst;
}
void PBPatientAssessment::Serialize(const SEArterialBloodGasTest& src, CDM_BIND::ArterialBloodGasTestData& dst)
{
  PBPatientAssessment::Serialize(src, *dst.mutable_patientassessment());
  if (src.HasBloodPH())
    dst.set_allocated_bloodph(PBProperty::Unload(*src.m_BloodPH));
  if (src.HasBicarbonate())
    dst.set_allocated_bicarbonate(PBProperty::Unload(*src.m_Bicarbonate));
  if (src.HasPartialPressureOfOxygen())
    dst.set_allocated_partialpressureofoxygen(PBProperty::Unload(*src.m_PartialPressureOfOxygen));
  if (src.HasPartialPressureOfCarbonDioxide())
    dst.set_allocated_partialpressureofcarbondioxide(PBProperty::Unload(*src.m_PartialPressureOfCarbonDioxide));
  if (src.HasOxygenSaturation())
    dst.set_allocated_oxygensaturation(PBProperty::Unload(*src.m_OxygenSaturation));
}

bool PBPatientAssessment::SerializeToString(const SECompleteBloodCount& src, std::string& output, eSerializationFormat m)
{
  CDM_BIND::CompleteBloodCountData data;
  PBPatientAssessment::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBPatientAssessment::SerializeToFile(const SECompleteBloodCount& src, const std::string& filename)
{
  CDM_BIND::CompleteBloodCountData data;
  PBPatientAssessment::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}

void PBPatientAssessment::Load(const CDM_BIND::CompleteBloodCountData& src, SECompleteBloodCount& dst)
{
  dst.Clear();
  PBPatientAssessment::Serialize(src, dst);
}
void PBPatientAssessment::Serialize(const CDM_BIND::CompleteBloodCountData& src, SECompleteBloodCount& dst)
{
  PBPatientAssessment::Serialize(src.patientassessment(), dst);
  if (src.has_hematocrit())
    PBProperty::Load(src.hematocrit(), dst.GetHematocrit());
  if (src.has_hemoglobin())
    PBProperty::Load(src.hemoglobin(), dst.GetHemoglobin());
  if (src.has_plateletcount())
    PBProperty::Load(src.plateletcount(), dst.GetPlateletCount());
  if (src.has_meancorpuscularhemoglobin())
    PBProperty::Load(src.meancorpuscularhemoglobin(), dst.GetMeanCorpuscularHemoglobin());
  if (src.has_meancorpuscularhemoglobinconcentration())
    PBProperty::Load(src.meancorpuscularhemoglobinconcentration(), dst.GetMeanCorpuscularHemoglobinConcentration());
  if (src.has_meancorpuscularvolume())
    PBProperty::Load(src.meancorpuscularvolume(), dst.GetMeanCorpuscularVolume());
  if (src.has_redbloodcellcount())
    PBProperty::Load(src.redbloodcellcount(), dst.GetRedBloodCellCount());
  if (src.has_whitebloodcellcount())
    PBProperty::Load(src.whitebloodcellcount(), dst.GetWhiteBloodCellCount());
}

CDM_BIND::CompleteBloodCountData* PBPatientAssessment::Unload(const SECompleteBloodCount& src)
{
  CDM_BIND::CompleteBloodCountData* dst = new CDM_BIND::CompleteBloodCountData();
  PBPatientAssessment::Serialize(src, *dst);
  return dst;
}
void PBPatientAssessment::Serialize(const SECompleteBloodCount& src, CDM_BIND::CompleteBloodCountData& dst)
{
  PBPatientAssessment::Serialize(src, *dst.mutable_patientassessment());
  if (src.HasHematocrit())
    dst.set_allocated_hematocrit(PBProperty::Unload(*src.m_Hematocrit));
  if (src.HasHemoglobin())
    dst.set_allocated_hemoglobin(PBProperty::Unload(*src.m_Hemoglobin));
  if (src.HasPlateletCount())
    dst.set_allocated_plateletcount(PBProperty::Unload(*src.m_PlateletCount));
  if (src.HasMeanCorpuscularHemoglobin())
    dst.set_allocated_meancorpuscularhemoglobin(PBProperty::Unload(*src.m_MeanCorpuscularHemoglobin));
  if (src.HasMeanCorpuscularHemoglobinConcentration())
    dst.set_allocated_meancorpuscularhemoglobinconcentration(PBProperty::Unload(*src.m_MeanCorpuscularHemoglobinConcentration));
  if (src.HasMeanCorpuscularVolume())
    dst.set_allocated_meancorpuscularvolume(PBProperty::Unload(*src.m_MeanCorpuscularVolume));
  if (src.HasRedBloodCellCount())
    dst.set_allocated_redbloodcellcount(PBProperty::Unload(*src.m_RedBloodCellCount));
  if (src.HasWhiteBloodCellCount())
    dst.set_allocated_whitebloodcellcount(PBProperty::Unload(*src.m_WhiteBloodCellCount));
}

bool PBPatientAssessment::SerializeToString(const SEComprehensiveMetabolicPanel& src, std::string& output, eSerializationFormat m)
{
  CDM_BIND::ComprehensiveMetabolicPanelData data;
  PBPatientAssessment::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBPatientAssessment::SerializeToFile(const SEComprehensiveMetabolicPanel& src, const std::string& filename)
{
  CDM_BIND::ComprehensiveMetabolicPanelData data;
  PBPatientAssessment::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}

void PBPatientAssessment::Load(const CDM_BIND::ComprehensiveMetabolicPanelData& src, SEComprehensiveMetabolicPanel& dst)
{
  dst.Clear();
  PBPatientAssessment::Serialize(src, dst);
}
void PBPatientAssessment::Serialize(const CDM_BIND::ComprehensiveMetabolicPanelData& src, SEComprehensiveMetabolicPanel& dst)
{
  PBPatientAssessment::Serialize(src.patientassessment(), dst);
  if (src.has_albumin())
    PBProperty::Load(src.albumin(), dst.GetAlbumin());
  if (src.has_alp())
    PBProperty::Load(src.alp(), dst.GetALP());
  if (src.has_alt())
    PBProperty::Load(src.alt(), dst.GetALT());
  if (src.has_ast())
    PBProperty::Load(src.ast(), dst.GetAST());
  if (src.has_bun())
    PBProperty::Load(src.bun(), dst.GetBUN());
  if (src.has_calcium())
    PBProperty::Load(src.calcium(), dst.GetCalcium());
  if (src.has_chloride())
    PBProperty::Load(src.chloride(), dst.GetChloride());
  if (src.has_co2())
    PBProperty::Load(src.co2(), dst.GetCO2());
  if (src.has_creatinine())
    PBProperty::Load(src.creatinine(), dst.GetCreatinine());
  if (src.has_glucose())
    PBProperty::Load(src.glucose(), dst.GetGlucose());
  if (src.has_potassium())
    PBProperty::Load(src.potassium(), dst.GetPotassium());
  if (src.has_sodium())
    PBProperty::Load(src.sodium(), dst.GetSodium());
  if (src.has_totalbilirubin())
    PBProperty::Load(src.totalbilirubin(), dst.GetTotalBilirubin());
  if (src.has_totalprotein())
    PBProperty::Load(src.totalprotein(), dst.GetTotalProtein());
}

CDM_BIND::ComprehensiveMetabolicPanelData* PBPatientAssessment::Unload(const SEComprehensiveMetabolicPanel& src)
{
  CDM_BIND::ComprehensiveMetabolicPanelData* dst = new CDM_BIND::ComprehensiveMetabolicPanelData();
  PBPatientAssessment::Serialize(src, *dst);
  return dst;
}
void PBPatientAssessment::Serialize(const SEComprehensiveMetabolicPanel& src, CDM_BIND::ComprehensiveMetabolicPanelData& dst)
{
  PBPatientAssessment::Serialize(src, *dst.mutable_patientassessment());
  if (src.HasAlbumin())
    dst.set_allocated_albumin(PBProperty::Unload(*src.m_Albumin));
  if (src.HasALP())
    dst.set_allocated_alp(PBProperty::Unload(*src.m_ALP));
  if (src.HasALT())
    dst.set_allocated_alt(PBProperty::Unload(*src.m_ALT));
  if (src.HasAST())
    dst.set_allocated_ast(PBProperty::Unload(*src.m_AST));
  if (src.HasBUN())
    dst.set_allocated_bun(PBProperty::Unload(*src.m_BUN));
  if (src.HasCalcium())
    dst.set_allocated_calcium(PBProperty::Unload(*src.m_Calcium));
  if (src.HasChloride())
    dst.set_allocated_chloride(PBProperty::Unload(*src.m_Chloride));
  if (src.HasCO2())
    dst.set_allocated_co2(PBProperty::Unload(*src.m_CO2));
  if (src.HasCreatinine())
    dst.set_allocated_creatinine(PBProperty::Unload(*src.m_Creatinine));
  if (src.HasGlucose())
    dst.set_allocated_glucose(PBProperty::Unload(*src.m_Glucose));
  if (src.HasPotassium())
    dst.set_allocated_potassium(PBProperty::Unload(*src.m_Potassium));
  if (src.HasSodium())
    dst.set_allocated_sodium(PBProperty::Unload(*src.m_Sodium));
  if (src.HasTotalBilirubin())
    dst.set_allocated_totalbilirubin(PBProperty::Unload(*src.m_TotalBilirubin));
  if (src.HasTotalProtein())
    dst.set_allocated_totalprotein(PBProperty::Unload(*src.m_TotalProtein));
}


bool PBPatientAssessment::SerializeToString(const SEUrinalysis& src, std::string& output, eSerializationFormat m)
{
  CDM_BIND::UrinalysisData data;
  PBPatientAssessment::Serialize(src, data);
  return PBUtils::SerializeToString(data, output, m, src.GetLogger());
}
bool PBPatientAssessment::SerializeToFile(const SEUrinalysis& src, const std::string& filename)
{
  CDM_BIND::UrinalysisData data;
  PBPatientAssessment::Serialize(src, data);
  return PBUtils::SerializeToFile(data, filename, src.GetLogger());
}

void PBPatientAssessment::Load(const CDM_BIND::UrinalysisData& src, SEUrinalysis& dst)
{
  dst.Clear();
  PBPatientAssessment::Serialize(src, dst);
}
void PBPatientAssessment::Serialize(const CDM_BIND::UrinalysisData& src, SEUrinalysis& dst)
{
  PBPatientAssessment::Serialize(src.patientassessment(), dst);
  dst.SetColor((eUrinalysis_UrineColor)src.color());
  dst.SetAppearance((eUrinalysis_ClarityIndicator)src.appearance());
  dst.SetGlucose((eUrinalysis_PresenceIndicator)src.glucose());
  dst.SetKetone((eUrinalysis_PresenceIndicator)src.ketone());
  dst.SetBlood((eUrinalysis_PresenceIndicator)src.blood());
  dst.SetProtein((eUrinalysis_PresenceIndicator)src.protein());
  dst.SetNitrite((eUrinalysis_PresenceIndicator)src.nitrite());
  dst.SetLeukocyteEsterase((eUrinalysis_PresenceIndicator)src.leukocyteesterase());
  if (src.has_bilirubin())
    PBProperty::Load(src.bilirubin(), dst.GetBilirubin());
  if (src.has_specificgravity())
    PBProperty::Load(src.specificgravity(), dst.GetSpecificGravity());
  if (src.has_ph())
    PBProperty::Load(src.ph(), dst.GetPH());
  if (src.has_urobilinogen())
    PBProperty::Load(src.urobilinogen(), dst.GetUrobilinogen());
  if (src.has_microscopic())
    PBPatientAssessment::Load(src.microscopic(), dst.GetMicroscopic());
}

CDM_BIND::UrinalysisData* PBPatientAssessment::Unload(const SEUrinalysis& src)
{
  CDM_BIND::UrinalysisData* dst = new CDM_BIND::UrinalysisData();
  PBPatientAssessment::Serialize(src, *dst);
  return dst;
}
void PBPatientAssessment::Serialize(const SEUrinalysis& src, CDM_BIND::UrinalysisData& dst)
{
  PBPatientAssessment::Serialize(src, *dst.mutable_patientassessment());
  if (src.HasColor())
    dst.set_color((CDM_BIND::UrinalysisData::eUrineColor)src.m_Color);
  if (src.HasAppearance())
    dst.set_appearance((CDM_BIND::UrinalysisData::eClarityIndicator)src.m_Appearance);
  if (src.HasGlucose())
    dst.set_glucose((CDM_BIND::UrinalysisData::ePresenceIndicator)src.m_Glucose);
  if (src.HasKetone())
    dst.set_ketone((CDM_BIND::UrinalysisData::ePresenceIndicator)src.m_Ketone);
  if (src.HasBlood())
    dst.set_blood((CDM_BIND::UrinalysisData::ePresenceIndicator)src.m_Blood);
  if (src.HasProtein())
    dst.set_protein((CDM_BIND::UrinalysisData::ePresenceIndicator)src.m_Protein);
  if (src.HasNitrite())
    dst.set_nitrite((CDM_BIND::UrinalysisData::ePresenceIndicator)src.m_Nitrite);
  if (src.HasBilirubin())
    dst.set_allocated_bilirubin(PBProperty::Unload(*src.m_Bilirubin));
  if (src.HasSpecificGravity())
    dst.set_allocated_specificgravity(PBProperty::Unload(*src.m_SpecificGravity));
  if (src.HasPH())
    dst.set_allocated_ph(PBProperty::Unload(*src.m_pH));
  if (src.HasUrobilinogen())
    dst.set_allocated_urobilinogen(PBProperty::Unload(*src.m_Urobilinogen));
  if (src.HasMicroscopic())
    dst.set_allocated_microscopic(PBPatientAssessment::Unload(*src.m_Microscopic));
}

void PBPatientAssessment::Load(const CDM_BIND::UrinalysisMicroscopicData& src, SEUrinalysisMicroscopic& dst)
{
  dst.Clear();
  PBPatientAssessment::Serialize(src, dst);
}
void PBPatientAssessment::Serialize(const CDM_BIND::UrinalysisMicroscopicData& src, SEUrinalysisMicroscopic& dst)
{
  dst.SetObservationType((eUrinalysis_MicroscopicObservationType)src.observationtype());
  dst.SetEpithelialCells((eUrinalysis_MicroscopicObservationAmount)src.epithelialcells());
  dst.SetCrystals((eUrinalysis_MicroscopicObservationAmount)src.crystals());
  dst.SetBacteria((eUrinalysis_MicroscopicObservationAmount)src.bacteria());
  dst.SetTrichomonads((eUrinalysis_MicroscopicObservationAmount)src.trichomonads());
  dst.SetYeast((eUrinalysis_MicroscopicObservationAmount)src.yeast());
  if (src.has_redbloodcells())
    PBProperty::Load(src.redbloodcells(), dst.GetRedBloodCells());
  if (src.has_whitebloodcells())
    PBProperty::Load(src.whitebloodcells(), dst.GetWhiteBloodCells());
  if (src.has_casts())
    PBProperty::Load(src.casts(), dst.GetCasts());
}

CDM_BIND::UrinalysisMicroscopicData* PBPatientAssessment::Unload(const SEUrinalysisMicroscopic& src)
{
  CDM_BIND::UrinalysisMicroscopicData* dst = new CDM_BIND::UrinalysisMicroscopicData();
  PBPatientAssessment::Serialize(src, *dst);
  return dst;
}
void PBPatientAssessment::Serialize(const SEUrinalysisMicroscopic& src, CDM_BIND::UrinalysisMicroscopicData& dst)
{
  if (src.HasObservationType())
    dst.set_observationtype((CDM_BIND::UrinalysisMicroscopicData::eObservationType)src.m_ObservationType);
  if (src.HasEpithelialCells())
    dst.set_epithelialcells((CDM_BIND::UrinalysisMicroscopicData::eObservationAmount)src.m_EpithelialCells);
  if (src.HasCrystals())
    dst.set_crystals((CDM_BIND::UrinalysisMicroscopicData::eObservationAmount)src.m_Crystals);
  if (src.HasBacteria())
    dst.set_bacteria((CDM_BIND::UrinalysisMicroscopicData::eObservationAmount)src.m_Bacteria);
  if (src.HasTrichomonads())
    dst.set_trichomonads((CDM_BIND::UrinalysisMicroscopicData::eObservationAmount)src.m_Trichomonads);
  if (src.HasYeast())
    dst.set_yeast((CDM_BIND::UrinalysisMicroscopicData::eObservationAmount)src.m_Yeast);
  if (src.HasRedBloodCells())
    dst.set_allocated_redbloodcells(PBProperty::Unload(*src.m_RedBloodCells));
  if (src.HasWhiteBloodCells())
    dst.set_allocated_whitebloodcells(PBProperty::Unload(*src.m_WhiteBloodCells));
  if (src.HasCasts())
    dst.set_allocated_casts(PBProperty::Unload(*src.m_Casts));
}
