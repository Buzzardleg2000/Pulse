/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
PULSE_BIND_DECL(BloodChemistry)
PULSE_BIND_DECL(Cardiovascular)
PULSE_BIND_DECL(Drug)
PULSE_BIND_DECL(Endocrine)
PULSE_BIND_DECL(Energy)
PULSE_BIND_DECL(Gastrointestinal)
PULSE_BIND_DECL(Hepatic)
PULSE_BIND_DECL(Immune)
PULSE_BIND_DECL(Nervous)
PULSE_BIND_DECL(Renal)
PULSE_BIND_DECL(Respiratory)
PULSE_BIND_DECL(Tissue)
#include "engine/common/system/physiology/BloodChemistryModel.h"
#include "engine/common/system/physiology/CardiovascularModel.h"
#include "engine/common/system/physiology/DrugModel.h"
#include "engine/common/system/physiology/EndocrineModel.h"
#include "engine/common/system/physiology/EnergyModel.h"
#include "engine/common/system/physiology/GastrointestinalModel.h"
#include "engine/common/system/physiology/HepaticModel.h"
#include "engine/common/system/physiology/ImmuneModel.h"
#include "engine/common/system/physiology/NervousModel.h"
#include "engine/common/system/physiology/RenalModel.h"
#include "engine/common/system/physiology/RespiratoryModel.h"
#include "engine/common/system/physiology/TissueModel.h"

namespace pulse
{
  class PULSE_DECL PBPhysiology
  {
  public:

    static void Load(const PULSE_BIND::BloodChemistryData& src, BloodChemistryModel& dst);
    static PULSE_BIND::BloodChemistryData* Unload(const BloodChemistryModel& src);
    static void Serialize(const PULSE_BIND::BloodChemistryData& src, BloodChemistryModel& dst);
    static void Serialize(const BloodChemistryModel& src, PULSE_BIND::BloodChemistryData& dst);

    static void Load(const PULSE_BIND::CardiovascularData& src, CardiovascularModel& dst);
    static PULSE_BIND::CardiovascularData* Unload(const CardiovascularModel& src);
    static void Serialize(const PULSE_BIND::CardiovascularData& src, CardiovascularModel& dst);
    static void Serialize(const CardiovascularModel& src, PULSE_BIND::CardiovascularData& dst);

    static void Load(const PULSE_BIND::DrugData& src, DrugModel& dst);
    static PULSE_BIND::DrugData* Unload(const DrugModel& src);
    static void Serialize(const PULSE_BIND::DrugData& src, DrugModel& dst);
    static void Serialize(const DrugModel& src, PULSE_BIND::DrugData& dst);

    static void Load(const PULSE_BIND::EndocrineData& src, EndocrineModel& dst);
    static PULSE_BIND::EndocrineData* Unload(const EndocrineModel& src);
    static void Serialize(const PULSE_BIND::EndocrineData& src, EndocrineModel& dst);
    static void Serialize(const EndocrineModel& src, PULSE_BIND::EndocrineData& dst);

    static void Load(const PULSE_BIND::EnergyData& src, EnergyModel& dst);
    static PULSE_BIND::EnergyData* Unload(const EnergyModel& src);
    static void Serialize(const PULSE_BIND::EnergyData& src, EnergyModel& dst);
    static void Serialize(const EnergyModel& src, PULSE_BIND::EnergyData& dst);

    static void Load(const PULSE_BIND::GastrointestinalData& src, GastrointestinalModel& dst);
    static PULSE_BIND::GastrointestinalData* Unload(const GastrointestinalModel& src);
    static void Serialize(const PULSE_BIND::GastrointestinalData& src, GastrointestinalModel& dst);
    static void Serialize(const GastrointestinalModel& src, PULSE_BIND::GastrointestinalData& dst);

    static void Load(const PULSE_BIND::HepaticData& src, HepaticModel& dst);
    static PULSE_BIND::HepaticData* Unload(const HepaticModel& src);
    static void Serialize(const PULSE_BIND::HepaticData& src, HepaticModel& dst);
    static void Serialize(const HepaticModel& src, PULSE_BIND::HepaticData& dst);

    static void Load(const PULSE_BIND::ImmuneData& src, ImmuneModel& dst);
    static PULSE_BIND::ImmuneData* Unload(const ImmuneModel& src);
    static void Serialize(const PULSE_BIND::ImmuneData& src, ImmuneModel& dst);
    static void Serialize(const ImmuneModel& src, PULSE_BIND::ImmuneData& dst);

    static void Load(const PULSE_BIND::NervousData& src, NervousModel& dst);
    static PULSE_BIND::NervousData* Unload(const NervousModel& src);
    static void Serialize(const PULSE_BIND::NervousData& src, NervousModel& dst);
    static void Serialize(const NervousModel& src, PULSE_BIND::NervousData& dst);

    static void Load(const PULSE_BIND::RenalData& src, RenalModel& dst);
    static PULSE_BIND::RenalData* Unload(const RenalModel& src);
    static void Serialize(const PULSE_BIND::RenalData& src, RenalModel& dst);
    static void Serialize(const RenalModel& src, PULSE_BIND::RenalData& dst);

    static void Load(const PULSE_BIND::RespiratoryData& src, RespiratoryModel& dst);
    static PULSE_BIND::RespiratoryData* Unload(const RespiratoryModel& src);
    static void Serialize(const PULSE_BIND::RespiratoryData& src, RespiratoryModel& dst);
    static void Serialize(const RespiratoryModel& src, PULSE_BIND::RespiratoryData& dst);

    static void Load(const PULSE_BIND::TissueData& src, TissueModel& dst);
    static PULSE_BIND::TissueData* Unload(const TissueModel& src);
    static void Serialize(const PULSE_BIND::TissueData& src, TissueModel& dst);
    static void Serialize(const TissueModel& src, PULSE_BIND::TissueData& dst);
  };
}