# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.io.action import serialize_action_from_bind, serialize_action_to_bind

from pulse.cdm.patient_actions import *
from pulse.cdm.bind.PatientActions_pb2 import *
from pulse.cdm.io.patient import *
from pulse.cdm.io.scalars import *

def serialize_patient_action_to_bind(src: SEPatientAction, dst: PatientActionData):
    serialize_action_to_bind(src,dst.Action)

def serialize_patient_action_from_bind(src: PatientActionData, dst: SEPatientAction):
    serialize_action_from_bind(src.Action, dst)

#################################################################

def serialize_asthma_attack_to_bind(src: SEAsthmaAttack, dst: AsthmaAttackData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_severity():
        serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)

def serialize_asthma_attack_from_bind(src: AsthmaAttackData, dst: SEAsthmaAttack):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_asthma_attack_from_bind not implemented")

#################################################################

def serialize_acute_respiratory_distress_syndrome_exacerbation_to_bind(src: SEAcuteRespiratoryDistressSyndromeExacerbation,
                                                                       dst: AcuteRespiratoryDistressSyndromeExacerbationData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_severity():
        serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)
    if src.has_left_lung_affected():
        serialize_scalar_0to1_to_bind(src.get_left_lung_affected(), dst.LeftLungAffected)
    if src.has_right_lung_affected():
        serialize_scalar_0to1_to_bind(src.get_right_lung_affected(), dst.RightLungAffected)

def serialize_acute_respiratory_distress_syndrome_exacerbation_from_bind(src: AcuteRespiratoryDistressSyndromeExacerbationData,
                                                                         dst: SEAcuteRespiratoryDistressSyndromeExacerbation):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_acute_respiratory_distress_syndrome_exacerbation_from_bind not implemented")

#################################################################

def serialize_acute_stress_to_bind(src: SEAcuteStress, dst: AcuteStressData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_severity():
        serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)

def serialize_acute_stress_from_bind(src:AcuteStressData, dst: SEAcuteStress):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_airway_obstruction_to_bind(src: SEAirwayObstruction, dst: AirwayObstructionData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_severity():
        serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)

def serialize_airway_obstruction_from_bind(src:AirwayObstructionData, dst: SEAirwayObstruction):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_brain_injury_to_bind(src:SEBrainInjury, dst: BrainInjuryData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_severity():
        serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)
    if src.has_injury_type():
        dst.Type = src.get_injury_type().value

def serialize_brain_injury_from_bind(src: BrainInjuryData, dst: SEBrainInjury):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_bronchoconstriction_to_bind(src: SEBronchoconstriction, dst: BronchoconstrictionData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_severity():
        serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)

def serialize_bronchoconstriction_from_bind(src: BronchoconstrictionData, dst: SEBronchoconstriction):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_cardiac_arrest_to_bind(src: SECardiacArrest, dst: CardiacArrestData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    dst.State = src.get_state().value

def serialize_cardiac_arrest_from_bind(src: CardiacArrestData, dst: SECardiacArrest):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_chest_compression_force_to_bind(src: SEChestCompressionForce, dst: ChestCompressionForceData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_force():
        serialize_scalar_force_to_bind(src.get_force(), dst.Force)

def serialize_chest_compression_force_from_bind(src: ChestCompressionForceData, dst: SEChestCompressionForce):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_chest_compression_force_from_bind not implemented")

#################################################################

def serialize_chest_compression_force_scale_to_bind(src: SEChestCompressionForceScale, dst: ChestCompressionForceScaleData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_force_period():
        serialize_scalar_time_to_bind(src.get_force_period(), dst.ForcePeriod)
    if src.has_force_scale():
        serialize_scalar_0to1_to_bind(src.get_force_scale(), dst.ForceScale)

def serialize_chest_compression_force_scale_from_bind(src: ChestCompressionForceScaleData, dst: SEChestCompressionForceScale):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_chest_compression_force_scale_from_bind not implemented")

#################################################################

def serialize_chest_occlusive_dressing_to_bind(src: SEChestOcclusiveDressing, dst: ChestOcclusiveDressingData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_side():
        dst.Side = src.get_side().value
    if src.has_state():
        dst.State = src.get_state().value

def serialize_chest_occlusive_dressing_from_bind(src:ChestOcclusiveDressingData, dst: SEChestOcclusiveDressing):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_chronic_obstructive_pulmonary_disease_exacerbation_to_bind(src: SEChronicObstructivePulmonaryDiseaseExacerbation,
                                                                         dst: ChronicObstructivePulmonaryDiseaseExacerbationData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_bronchitis_severity():
        serialize_scalar_0to1_to_bind(src.get_bronchitis_severity(), dst.BronchitisSeverity)
    if src.has_emphysema_severity():
        serialize_scalar_0to1_to_bind(src.get_emphysema_severity(), dst.EmphysemaSeverity)
def serialize_chronic_obstructive_pulmonary_disease_exacerbation_from_bind(src: ChronicObstructivePulmonaryDiseaseExacerbationData,
                                                                           dst: SEChronicObstructivePulmonaryDiseaseExacerbation):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_chronic_obstructive_pulmonary_disease_exacerbation_from_bind not implemented")

#################################################################

def serialize_conscious_respiration_to_bind(src: SEConsciousRespiration, dst: ConsciousRespirationData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    dst.StartImmediately = src.get_start_immediately()
    for command in src.get_commands():
        serialize_conscious_respiration_command_to_bind(command, dst.Command.add())

def serialize_conscious_respiration_from_bind(src:ConsciousRespirationData, dst: SEConsciousRespiration):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_conscious_respiration_from_bind not implemented")

    #################################################################

def serialize_forced_exhale_to_bind(src:SEForcedExhale, dst: ForcedExhaleData):
    serialize_scalar_time_to_bind(src.get_exhale_period(), dst.ExhalePeriod)
    if src.has_expiratory_capacity_fraction():
        serialize_scalar_0to1_to_bind( src.get_expiratory_capacity_fraction(), dst.ExpiratoryReserveVolumeFraction)
    if src.has_hold_period():
        serialize_scalar_time_to_bind(src.get_hold_period(), dst.HoldPeriod)
    if src.has_release_period():
        serialize_scalar_time_to_bind(src.get_release_period(), dst.ReleasePeriod)

def serialize_forced_exhale_from_bind(src: ForcedExhaleData, dst: SEForcedExhale):
    raise Exception("serialize_forced_exhale_from_bind not implemented")

    #################################################################

def serialize_forced_inhale_to_bind(src:SEForcedInhale, dst: ForcedInhaleData):
    serialize_scalar_time_to_bind(src.get_inhale_period(), dst.InhalePeriod)
    if src.has_inspiratory_capacity_fraction():
        serialize_scalar_0to1_to_bind( src.get_inspiratory_capacity_fraction(), dst.InspiratoryCapacityFraction)
    if src.has_hold_period():
        serialize_scalar_time_to_bind(src.get_hold_period(), dst.HoldPeriod)
    if src.has_release_period():
        serialize_scalar_time_to_bind(src.get_release_period(), dst.ReleasePeriod)

def serialize_forced_inhale_from_bind(src: ForcedInhaleData, dst: SEForcedInhale):
    raise Exception("serialize_forced_inhale_from_bind not implemented")

    #################################################################

def serialize_forced_pause_to_bind(src: SEForcedPause, dst: ForcedPauseData):
    if src.has_period():
        serialize_scalar_time_to_bind(src.get_period(), dst.Period)

def serialize_forced_pause_from_bind(src: ForcedPauseData, dst: SEForcedPause):
    raise Exception("serialize_forced_pause_from_bind not implemented")

    #################################################################

def serialize_conscious_respiration_command_to_bind(src, dst: AnyConsciousRespirationCommandData):
    if isinstance(src, SEForcedInhale):
        serialize_forced_inhale_to_bind(src, dst.ForcedInhale)
        return ForcedInhaleData
    elif isinstance(src, SEForcedExhale):
        serialize_forced_exhale_to_bind(src, dst.ForcedExhale)
        return ForcedExhaleData
    elif isinstance(src, SEForcedPause):
        serialize_forced_pause_to_bind(src, dst.ForcedPause)
        return ForcedPauseData
    else:
        raise Exception("Unable to add Conscious Respiration Command")

def serialize_conscious_respiration_command_from_bind(src: AnyConsciousRespirationCommandData, dst):
    raise Exception("serialize_conscious_respiration_command_from_bind not implemented")

#################################################################

def serialize_consume_nutrients_to_bind(src: SEConsumeNutrients, dst: ConsumeNutrientsData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_nutrition_file():
        dst.NutritionFile = src.get_nutrition_file()
    elif src.has_nutrition():
        serialize_nutrition_to_bind(src.get_nutrition(), dst.Nutrition)

def serialize_consume_nutrients_from_bind(src: ConsumeNutrientsData, dst: ConsumeNutrientsData):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_consume_nutrients_from_bind not implemented")

#################################################################

def serialize_dsypnea_to_bind(src: SEDyspnea, dst: DyspneaData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_severity():
        serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)

def serialize_dsypnea_from_bind(src:DyspneaData, dst: SEDyspnea):
    serialize_patient_action_from_bind(dst.PatientAction, src)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_exercise_to_bind(src: SEExercise, dst: ExerciseData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_intensity():
        serialize_scalar_0to1_to_bind(src.get_intensity(), dst.Intensity)

def serialize_exercise_from_bind(src: ExerciseData, dst: SEExercise):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_exercise_from_bind not implemented")

#################################################################

def serialize_hemorrhage_to_bind(src: SEHemorrhage, dst: HemorrhageData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    dst.Type = src.get_type().value
    if src.has_compartment():
        dst.Compartment = src.get_compartment()
    if src.has_rate():
        serialize_scalar_volume_per_time_to_bind(src.get_rate(), dst.Rate)

def serialize_hemorrhage_from_bind(src: HemorrhageData, dst: SEHemorrhage):
    raise Exception("serialize_hemorrhage_from_bind not implemented")

#################################################################

def serialize_intubation_to_bind(src:SEIntubation, dst:IntubationData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    dst.Type = src.get_type().value

def serialize_intubation_from_bind(src:IntubationData, dst:SEIntubation):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_impaired_alveolar_exchange_exacerbation_to_bind(src: SEImpairedAlveolarExchangeExacerbation , dst: ImpairedAlveolarExchangeExacerbationData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_impaired_fraction():
        serialize_scalar_0to1_to_bind(src.get_impaired_fraction(), dst.ImpairedFraction)
    if src.has_impaired_surface_area():
        serialize_scalar_area_to_bind(src.get_impaired_surface_area(), dst.ImpairedSurfaceArea)
    if src.has_severity():
        serialize_scalar_area_to_bind(src.get_severity(), dst.Severity)

def serialize_impaired_alveolar_exchange_exacerbation_from_bind(src: ImpairedAlveolarExchangeExacerbationData, dst: SEImpairedAlveolarExchangeExacerbation):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")


#################################################################

def serialize_lobar_pneumonia_exacerbation_to_bind(src:SELobarPneumoniaExacerbation, dst: LobarPneumoniaExacerbationData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_severity():
        serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)
    if src.has_right_lung_affected():
        serialize_scalar_0to1_to_bind(src.get_right_lung_affected(), dst.RightLungAffected)
    if src.has_left_lung_affected():
        serialize_scalar_0to1_to_bind(src.get_left_lung_affected(), dst.LeftLungAffected)

def serialize_lobar_pneumonia_exacerbation_from_bind(src:LobarPneumoniaExacerbationData, dst: SELobarPneumoniaExacerbation ):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_mechanical_ventilation_to_bind(src: SEMechanicalVentilation, dst: MechanicalVentilationData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_pressure():
        serialize_scalar_pressure_to_bind(src.get_pressure(), dst.Pressure)
    if src.has_flow():
        serialize_scalar_volume_per_time_to_bind(src.get_flow(), dst.Flow)
    dst.State = src.get_state().value

def serialize_needle_decompression_from_bind(src:MechanicalVentilationData, dst: SEMechanicalVentilation):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_needle_decompression_to_bind(src: SENeedleDecompression, dst: NeedleDecompressionData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_side():
        dst.Side = src.get_side().value
    if src.has_state():
        dst.State = src.get_state().value

def serialize_needle_decompression_from_bind(src:NeedleDecompressionData, dst: SENeedleDecompression):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_pericardial_effusion_to_bind(src: SEPericardialEffusion, dst:PericardialEffusionData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_effusion_rate():
        serialize_scalar_volume_per_time_to_bind(src.get_effusion_rate(), dst.EffusionRate)

def serialize_pericardial_effusion_from_bind(src: PericardialEffusionData, dst: SEPericardialEffusion):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_pulmonary_shunt_exacerbation_to_bind(src:SEPulmonaryShuntExacerbation, dst: PulmonaryShuntExacerbationData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_severity():
        serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)

def serialize_pulmonary_shunt_exacerbation_from_bind(src: PulmonaryShuntExacerbationData, dst: SEPulmonaryShuntExacerbation):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_respiratory_fatigue_to_bind(src:SERespiratoryFatigue, dst: RespiratoryFatigueData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_severity():
        serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)

def serialize_respiratory_fatigue_from_bind(src: RespiratoryFatigueData, dst: SERespiratoryFatigue):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_substance_bolus_to_bind(src:SESubstanceBolus, dst: SubstanceBolusData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_concentration():
        serialize_scalar_mass_per_volume_to_bind(src.get_concentration(), dst.Concentration)
    if src.has_dose():
        serialize_scalar_volume_to_bind(src.get_dose(), dst.Dose)
    if src.has_substance():
        dst.Substance = src.get_substance()
    if src.has_admin_route():
        dst.AdministrationRoute = src.get_admin_route().value

def serialize_substance_bolus_from_bind(src: SubstanceBolusData, dst: SESubstanceBolus):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_substance_compound_infusion_to_bind(src:SESubstanceCompoundInfusion, dst: SubstanceCompoundInfusionData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_bag_volume():
        serialize_scalar_volume_to_bind(src.get_bag_volume(), dst.BagVolume)
    if src.has_rate():
        serialize_scalar_volume_per_time_to_bind(src.get_rate(), dst.Rate)
    if src.has_compound():
        dst.SubstanceCompound = src.get_compound()

def serialize_substance_compound_infusion_from_bind(src: SubstanceCompoundInfusionData, dst: SESubstanceCompoundInfusion):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_substance_infusion_to_bind(src:SESubstanceInfusion, dst: SubstanceInfusionData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_concentration():
        serialize_scalar_mass_per_volume_to_bind(src.get_concentration(), dst.Concentration)
    if src.has_rate():
        serialize_scalar_volume_per_time_to_bind(src.get_rate(), dst.Rate)
    if src.has_substance():
        dst.Substance = src.get_substance()

def serialize_substance_infusion_from_bind(src: SubstanceInfusionData, dst: SESubstanceInfusion):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_supplemental_oxygen_to_bind(src: SESupplementalOxygen, dst: SupplementalOxygenData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_flow():
        serialize_scalar_volume_per_time_to_bind(src.get_flow(), dst.Flow)
    if src.has_volume():
        serialize_scalar_volume_to_bind(src.get_volume(), dst.Volume)
    dst.Device = src.get_device().value

def serialize_supplemental_oxygen_from_bind(src: SupplementalOxygenData, dst: SESupplementalOxygen):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_tension_pneumothorax_to_bind(src: SETensionPneumothorax, dst: TensionPneumothoraxData):
    serialize_patient_action_to_bind(src, dst.PatientAction)
    if src.has_severity():
        serialize_scalar_0to1_to_bind(src.get_severity(), dst.Severity)
    if src.has_side():
        dst.Side = src.get_side().value
    if src.has_type():
        dst.Type = src.get_type().value

def serialize_tension_pneumothorax_from_bind(src: TensionPneumothoraxData, dst: SETensionPneumothorax):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_patient_condition_from_bind not implemented")

#################################################################

def serialize_urinate_to_bind(src: SEUrinate, dst: UrinateData):
    serialize_patient_action_to_bind(src, dst.PatientAction)

def serialize_urinate_from_bind(src: UrinateData, dst: SEUrinate):
    serialize_patient_action_from_bind(src.PatientAction, dst)
    raise Exception("serialize_urinate_from_bind not implemented")
