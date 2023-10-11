# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from typing import List, Dict

from pulse.cdm.engine import SEDataRequestManager, SEDataRequest, SEDataRequested, SEDecimalFormat, \
                             SEConditionManager, SEEngineInitialization, SEValidationTarget, \
                             SESegmentValidationTarget, SESegmentValidationSegment, \
                             SETimeSeriesValidationTarget, SEPatientTimeSeriesValidation
from pulse.cdm.bind.Engine_pb2 import AnyActionData, \
                                      ActionListData, ActionMapData, \
                                      AnyConditionData, ConditionListData, \
                                      PatientConfigurationData, \
                                      DataRequestData, DataRequestListData, DataRequestManagerData, \
                                      DataRequestedData, DataRequestedListData, DecimalFormatData, \
                                      EngineInitializationData, EngineInitializationListData, \
                                      LogMessagesData, ValidationTargetData, \
                                      SegmentValidationTargetData, SegmentValidationSegmentData,\
                                      SegmentValidationSegmentListData, SegmentValidationPipelineConfigurationData, \
                                      TimeSeriesValidationTargetData, TimeSeriesValidationTargetListData, \
                                      PatientTimeSeriesValidationData, PatientTimeSeriesValidationListData
from pulse.cdm.bind.Events_pb2 import ActiveEventListData, EventChangeListData

from pulse.cdm.patient import SEPatientConfiguration
from pulse.cdm.equipment_actions import SEEquipmentAction
from pulse.cdm.engine import SEEventChange, eEvent, SESegmentValidationPipelineConfig, eDataRequest_category, \
                             eDecimalFormat_type
from pulse.cdm.scalars import get_unit

from pulse.cdm.io.action import *
from pulse.cdm.io.patient_actions import *
from pulse.cdm.io.patient_conditions import *
from pulse.cdm.io.environment_actions import *
from pulse.cdm.io.environment_conditions import *
from pulse.cdm.io.bag_valve_mask_actions import *
from pulse.cdm.io.ecmo_actions import *
from pulse.cdm.io.mechanical_ventilator_actions import *
from pulse.cdm.io.plots import *

def serialize_event_change_list_to_bind(src: [], dst: EventChangeListData):
    raise Exception("serialize_event_change_list_to_bind not implemented")
def serialize_event_change_list_to_string(src: []):
    raise Exception("serialize_event_change_list_to_string not implemented")

def serialize_event_change_list_from_string(string: str, fmt: eSerializationFormat):
    src = EventChangeListData()
    json_format.Parse(string, src)
    return serialize_event_change_list_from_bind(src)
def serialize_event_change_list_from_bind(src: EventChangeListData):
    event_changes = []
    for ecd in src.Change:
        ec = SEEventChange()
        ec.event = eEvent(ecd.Event).name
        ec.active = ecd.Active
        event_changes.append(ec)
    return event_changes

def serialize_active_event_list_to_bind(src: [], dst: ActiveEventListData):
    raise Exception("serialize_active_event_list_to_bind not implemented")
def serialize_active_event_list_to_string(src: []):
    raise Exception("serialize_active_event_list_to_string not implemented")

def serialize_active_event_list_from_string(string: str, fmt: eSerializationFormat):
    src = ActiveEventListData()
    json_format.Parse(string, src)
    return serialize_active_event_list_from_bind(src)
def serialize_active_event_list_from_bind(src: ActiveEventListData):
    active_events = {}
    for ae in src.ActiveEvent:
        active_events[eEvent(ae.Event).name] = ae.Duration.ScalarTime.Value
    return active_events

def serialize_log_messages_to_bind(src: {}, dst: LogMessagesData):
    raise Exception("serialize_log_messages_to_bind not implemented")
def serialize_log_messages_to_string(src: []):
    raise Exception("serialize_log_messages_to_string not implemented")

def serialize_log_messages_from_string(string: str, fmt: eSerializationFormat):
    src = LogMessagesData()
    json_format.Parse(string, src)
    return serialize_log_messages_from_bind(src)
def serialize_log_messages_from_bind(src: LogMessagesData):
    log_messages = { 'Debug':[], 'Info':[], 'Warning':[], 'Fatal':[], 'Error':[], }
    for msg in src.DebugMessages:
        log_messages['Debug'].append(msg)
    for msg in src.InfogMessages:
        log_messages['Info'].append(msg)
    for msg in src.WarningMessages:
        log_messages['Warning'].append(msg)
    for msg in src.FatalMessages:
        log_messages['Fatal'].append(msg)
    for msg in src.ErrorMessages:
        log_messages['Error'].append(msg)

    return log_messages

def serialize_condition_manager_to_string(condition_manager: SEConditionManager, fmt: eSerializationFormat):
    dst = ConditionListData()
    serialize_condition_manager_to_bind(condition_manager, dst)
    return json_format.MessageToJson(dst, True, True)

def serialize_condition_manager_to_bind(condition_manager: SEConditionManager, dst: ConditionListData):
    if (condition_manager.is_empty()):
        return ""
    if condition_manager.has_acute_respiratory_distress_syndrome():
        any_condition = AnyConditionData()
        serialize_acute_respiratory_distress_syndrome_to_bind(condition_manager.get_acute_respiratory_distress_syndrome(),
                                                            any_condition.PatientCondition.AcuteRespiratoryDistressSyndrome)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_chronic_anemia():
        any_condition = AnyConditionData()
        serialize_chronic_anemia_to_bind(condition_manager.get_chronic_anemia(),
                                         any_condition.PatientCondition.ChronicAnemia)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_chronic_obstructive_pulmonary_disease():
        any_condition = AnyConditionData()
        serialize_chronic_obstructive_pulmonary_disease_to_bind(condition_manager.get_chronic_obstructive_pulmonary_disease(),
                                                                any_condition.PatientCondition.ChronicObstructivePulmonaryDisease)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_chronic_pericardial_effusion():
        any_condition = AnyConditionData()
        serialize_chronic_pericardial_effusion_to_bind(condition_manager.get_chronic_pericardial_effusion(),
                                                       any_condition.PatientCondition.ChronicPericardialEffusion)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_chronic_renal_stenosis():
        any_condition = AnyConditionData()
        serialize_chronic_renal_stenosis_to_bind(condition_manager.get_chronic_renal_stenosis(),
                                                 any_condition.PatientCondition.ChronicRenalStenosis)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_chronic_ventricular_systolic_dysfunction():
        any_condition = AnyConditionData()
        serialize_chronic_ventricular_systolic_dysfunction_to_bind(condition_manager.get_chronic_ventricular_systolic_dysfunction(),
                                                                   any_condition.PatientCondition.ChronicVentricularSystolicDysfunction)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_impaired_alveolar_exchange():
        any_condition = AnyConditionData()
        serialize_impaired_alveolar_exchange_to_bind(condition_manager.get_impaired_alveolar_exchange(),
                                                     any_condition.PatientCondition.ImpairedAlveolarExchange)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_pneumonia():
        any_condition = AnyConditionData()
        serialize_pneumonia_to_bind(condition_manager.get_pneumonia(),
                                          any_condition.PatientCondition.Pneumonia)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_pulmonary_fibrosis():
        any_condition = AnyConditionData()
        serialize_pulmonary_fibrosis_to_bind(condition_manager.get_pulmonary_fibrosis(),
                                             any_condition.PatientCondition.PulmonaryFibrosis)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_pulmonary_shunt():
        any_condition = AnyConditionData()
        serialize_pulmonary_shunt_to_bind(condition_manager.get_pulmonary_shunt(),
                                          any_condition.PatientCondition.PulmonaryShunt)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_sepsis():
        any_condition = AnyConditionData()
        serialize_sepsis_to_bind(condition_manager.get_sepsis(),
                                 any_condition.PatientCondition.Sepsis)
        dst.AnyCondition.append(any_condition)
    if condition_manager.has_initial_environmental_conditions():
        any_condition = AnyConditionData()
        serialize_initial_environmental_conditions_to_bind(condition_manager.get_initial_environmental_conditions(),
                                                           any_condition.EnvironmentCondition.InitialEnvironmentalConditions)
        dst.AnyCondition.append(any_condition)

def serialize_actions_to_bind(src: [], dst: ActionListData):

    for action in src:
        if not action.is_valid():
            print("Skipping invalid action")
            continue
        #print(action)
        any_action = AnyActionData()
        if isinstance(action, SEAdvanceTime):
            serialize_advance_time_to_bind(action, any_action.AdvanceTime)
            dst.AnyAction.append(any_action)
            continue
        if isinstance(action, SEAdvanceUntilStable):
            serialize_advance_until_stable_to_bind(action, any_action.AdvanceUntilStable)
            dst.AnyAction.append(any_action)
            continue
        if isinstance(action, SESerializeRequested):
            serialize_serialize_requested_to_bind(action, any_action.SerializeRequested)
            dst.AnyAction.append(any_action)
            continue
        if isinstance(action, SESerializeState):
            serialize_serialize_state_to_bind(action, any_action.SerializeState)
            dst.AnyAction.append(any_action)
            continue
        if isinstance(action, SEPatientAction):
            if isinstance(action, SEAcuteRespiratoryDistressSyndromeExacerbation):
                serialize_acute_respiratory_distress_syndrome_exacerbation_to_bind(action, any_action.PatientAction.AcuteRespiratoryDistressSyndromeExacerbation)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEAcuteStress):
                serialize_acute_stress_to_bind(action, any_action.PatientAction.AcuteStress)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEAirwayObstruction):
                serialize_airway_obstruction_to_bind(action, any_action.PatientAction.AirwayObstruction)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEArrhythmia):
                serialize_arrhythmia_to_bind(action, any_action.PatientAction.Arrhythmia)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEArrhythmia):
                serialize_arrhythmia_to_bind(action, any_action.PatientAction.Arrhythmia)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEAsthmaAttack):
                serialize_asthma_attack_to_bind(action, any_action.PatientAction.AsthmaAttack)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEBrainInjury):
                serialize_brain_injury_to_bind(action, any_action.PatientAction.BrainInjury)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEBronchoconstriction):
                serialize_bronchoconstriction_to_bind(action, any_action.PatientAction.Bronchoconstriction)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEChestCompression):
                serialize_chest_compression_to_bind(action, any_action.PatientAction.ChestCompression)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEChestCompressionInstantaneous):
                serialize_chest_compression_instantaneous_to_bind(action, any_action.PatientAction.ChestCompressionInstantaneous)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEChestCompressionAutomated):
                serialize_chest_compression_automated_to_bind(action, any_action.PatientAction.ChestCompressionAutomated)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEChestOcclusiveDressing):
                serialize_chest_occlusive_dressing_to_bind(action, any_action.PatientAction.ChestOcclusiveDressing)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEChronicObstructivePulmonaryDiseaseExacerbation):
                serialize_chronic_obstructive_pulmonary_disease_exacerbation_to_bind(action, any_action.PatientAction.ChronicObstructivePulmonaryDiseaseExacerbation)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEConsciousRespiration):
                serialize_conscious_respiration_to_bind(action, any_action.PatientAction.ConsciousRespiration)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEConsumeNutrients):
                serialize_consume_nutrients_to_bind(action, any_action.PatientAction.ConsumeNutrients)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEDyspnea):
                serialize_dsypnea_to_bind(action, any_action.PatientAction.Dyspnea)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEExercise):
                serialize_exercise_to_bind(action, any_action.PatientAction.Exercise)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEHemorrhage):
                serialize_hemorrhage_to_bind(action, any_action.PatientAction.Hemorrhage)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEHemothorax):
                serialize_hemothorax_to_bind(action, any_action.PatientAction.Hemothorax)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEImpairedAlveolarExchangeExacerbation):
                serialize_impaired_alveolar_exchange_exacerbation_to_bind(action, any_action.PatientAction.ImpairedAlveolarExchangeExacerbation)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEIntubation):
                serialize_intubation_to_bind(action, any_action.PatientAction.Intubation)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEPneumoniaExacerbation):
                serialize_pneumonia_exacerbation_to_bind(action, any_action.PatientAction.PneumoniaExacerbation)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEMechanicalVentilation):
                serialize_mechanical_ventilation_to_bind(action, any_action.PatientAction.MechanicalVentilation)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SENeedleDecompression):
                serialize_needle_decompression_to_bind(action, any_action.PatientAction.NeedleDecompression)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEPericardialEffusion):
                serialize_pericardial_effusion_to_bind(action, any_action.PatientAction.PericardialEffusion)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEPulmonaryShuntExacerbation):
                serialize_pulmonary_shunt_exacerbation_to_bind(action, any_action.PatientAction.PulmonaryShuntExacerbation)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SERespiratoryFatigue):
                serialize_respiratory_fatigue_to_bind(action, any_action.PatientAction.RespiratoryFatigue)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SERespiratoryMechanicsConfiguration):
                serialize_respiratory_mechanics_configuration_to_bind(action, any_action.PatientAction.RespiratoryMechanicsConfiguration)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SESubstanceBolus):
                serialize_substance_bolus_to_bind(action, any_action.PatientAction.SubstanceBolus)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SESubstanceInfusion):
                serialize_substance_infusion_to_bind(action, any_action.PatientAction.SubstanceInfusion)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SESubstanceCompoundInfusion):
                serialize_substance_compound_infusion_to_bind(action, any_action.PatientAction.SubstanceCompoundInfusion)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SESupplementalOxygen):
                serialize_supplemental_oxygen_to_bind(action, any_action.PatientAction.SupplementalOxygen)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SETensionPneumothorax):
                serialize_tension_pneumothorax_to_bind(action, any_action.PatientAction.TensionPneumothorax)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SETubeThoracostomy):
                serialize_tube_thoracostomy_to_bind(action, any_action.PatientAction.TubeThoracostomy)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEUrinate):
                serialize_urinate_to_bind(action, any_action.PatientAction.Urinate)
                dst.AnyAction.append(any_action)
                continue
            print("Unknown Patient Action")

        if isinstance(action, SEEnvironmentAction):
            if isinstance(action, SEChangeEnvironmentalConditions):
                serialize_change_environmental_conditions_to_bind(action, any_action.EnvironmentAction.ChangeEnvironmentalConditions)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEThermalApplication):
                serialize_thermal_application_to_bind(action, any_action.EnvironmentAction.ThermalApplication)
                dst.AnyAction.append(any_action)
                continue
            print("Unknown Environment Action")

        if isinstance(action, SEEquipmentAction):
            if isinstance(action, SEBagValveMaskConfiguration):
                serialize_bag_valve_mask_configuration_to_bind(action, any_action.EquipmentAction.BagValveMaskConfiguration)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEBagValveMaskAutomated):
                serialize_bag_valve_mask_automated_to_bind(action, any_action.EquipmentAction.BagValveMaskAutomated)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEBagValveMaskInstantaneous):
                serialize_bag_valve_mask_instantaneous_to_bind(action, any_action.EquipmentAction.BagValveMaskInstantaneous)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEBagValveMaskSqueeze):
                serialize_bag_valve_mask_squeeze_to_bind(action, any_action.EquipmentAction.BagValveMaskSqueeze)
                dst.AnyAction.append(any_action)
                continue

            if isinstance(action, SEECMOConfiguration):
                serialize_ecmo_configuration_to_bind(action, any_action.EquipmentAction.ECMOConfiguration)
                dst.AnyAction.append(any_action)
                continue

#            if isinstance(action, SEInhalerConfiguration):
#                serialize_inhaler_configuration_to_bind(action, any_action.EquipmentAction.InhalerConfiguration)
#                dst.AnyAction.append(any_action)
#                continue

            if isinstance(action, SEMechanicalVentilatorConfiguration):
                serialize_mechanical_ventilator_configuration_to_bind(action, any_action.EquipmentAction.MechanicalVentilatorConfiguration)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEMechanicalVentilatorHold):
                serialize_mechanical_ventilator_hold_to_bind(action, any_action.EquipmentAction.MechanicalVentilatorHold)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEMechanicalVentilatorLeak):
                serialize_mechanical_ventilator_leak_to_bind(action, any_action.EquipmentAction.MechanicalVentilatorLeak)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEMechanicalVentilatorContinuousPositiveAirwayPressure):
                serialize_mechanical_ventilator_continuous_positive_airway_pressure_to_bind(action, any_action.EquipmentAction.MechanicalVentilatorContinuousPositiveAirwayPressure)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEMechanicalVentilatorPressureControl):
                serialize_mechanical_ventilator_pressure_control_to_bind(action, any_action.EquipmentAction.MechanicalVentilatorPressureControl)
                dst.AnyAction.append(any_action)
                continue
            if isinstance(action, SEMechanicalVentilatorVolumeControl):
                serialize_mechanical_ventilator_volume_control_to_bind(action, any_action.EquipmentAction.MechanicalVentilatorVolumeControl)
                dst.AnyAction.append(any_action)
                continue
            print("Unknown Equipment Action")

def serialize_actions_to_string(actions: [], fmt: eSerializationFormat):
    action_list = ActionListData()
    serialize_actions_to_bind(actions, action_list)
    return json_format.MessageToJson(action_list,True,True)

def serialize_action_map_to_string(src: {int, SEAction}, fmt: eSerializationFormat):
    dst = ActionMapData()
    for e in src:
        serialize_actions_to_bind(e.actions, dst.ActionMap[e.get_id()])
    return json_format.MessageToJson(dst, True, True)

# Patient Configuration
def serialize_patient_configuration_to_string(src: SEPatientConfiguration, fmt: eSerializationFormat):
    dst = PatientConfigurationData()
    serialize_patient_configuration_to_bind(src, dst)
    return json_format.MessageToJson(dst, True, True)

def serialize_patient_configuration_to_file(src: SEPatientConfiguration, filename: str):
    string = serialize_patient_configuration_to_string(src, eSerializationFormat.JSON)
    file = open(filename, "w")
    n = file.write(string)
    file.close()

def serialize_patient_configuration_from_string(string: str, dst: SEPatientConfiguration, fmt: eSerializationFormat):
    src = PatientConfigurationData()
    json_format.Parse(string, src)
    serialize_patient_configuration_from_bind(src,dst)

def serialize_patient_configuration_from_file(filename: str, dst: SEPatientConfiguration):
    with open(filename) as f:
        string = f.read()
    serialize_patient_configuration_from_string(string, dst, eSerializationFormat.JSON)

def serialize_patient_configuration_to_bind(src: SEPatientConfiguration, dst: PatientConfigurationData):
    if src.has_patient_file():
        dst.PatientFile = src.get_patient_file()
    elif src.has_patient():
        serialize_patient_to_bind(src.get_patient(), dst.Patient)
    if src.has_conditions():
        serialize_condition_manager_to_bind(src.get_conditions(), dst.Conditions)
    dst.DataRoot = src.get_data_root_dir()

def serialize_patient_configuration_from_bind(src: PatientConfigurationData, dst: SEPatientConfiguration):
    raise Exception("serialize_patient_configuration_from_bind not implemented")


# Data Requests and Manager
def serialize_data_request_to_bind(src: SEDataRequest, dst: DataRequestData):
    serialize_decimal_format_to_bind(src, dst.DecimalFormat)
    if src.has_action_name():
        dst.ActionName = src.get_action_name()
    if src.has_compartment_name():
        dst.CompartmentName = src.get_compartment_name()
    if src.has_substance_name():
        dst.SubstanceName = src.get_substance_name()
    if src.has_unit():
        dst.Unit = src.get_unit()
    dst.PropertyName = src.get_property_name()
    dst.Category = src.get_category().value

def serialize_data_request_from_bind(src: DataRequestData) -> SEDataRequest:
    return SEDataRequest(
        category=eDataRequest_category(src.Category),
        action=src.ActionName if src.ActionName else None,
        compartment=src.CompartmentName if src.CompartmentName else None,
        substance=src.SubstanceName if src.SubstanceName else None,
        property=src.PropertyName if src.PropertyName else None,
        unit=get_unit(src.Unit),
        precision=src.DecimalFormat.Precision if src.DecimalFormat.Precision else None,
        notation=eDecimalFormat_type(src.DecimalFormat.Type) if src.DecimalFormat.Type else None
    )

def serialize_data_request_list_to_bind(src: [SEDataRequest], dst: DataRequestListData):
    for dr in src:
        dr_data = DataRequestData()
        serialize_data_request_to_bind(dr, dr_data)
        dst.DataRequest.append(dr_data)
def serialize_data_request_list_to_string(src: [SEDataRequest], fmt: eSerializationFormat) -> str:
    dst = DataRequestListData()
    serialize_data_request_list_to_bind(src, dst)
    return json_format.MessageToJson(dst, True, True)
def serialize_data_request_list_to_file(src: [SEDataRequest], filename: str):
    string = serialize_data_request_list_to_string(src, eSerializationFormat.JSON)
    file = open(filename, "w")
    file.write(string)
    file.close()

def serialize_data_request_manager_from_file(filename: str, dst: SEDataRequestManager):
    with open(filename) as f:
        string = f.read()
    return serialize_data_request_manager_from_string(string, dst, eSerializationFormat.JSON)
def serialize_data_request_manager_to_file(src: SEDataRequestManager, filename: str):
    string = serialize_data_request_manager_to_string(src, eSerializationFormat.JSON)
    file = open(filename, "w")
    n = file.write(string)
    file.close()
def serialize_data_request_manager_to_string(src: SEDataRequestManager, fmt: eSerializationFormat):
    dst = DataRequestManagerData()
    serialize_data_request_manager_to_bind(src, dst)
    return json_format.MessageToJson(dst, True, True)

def serialize_data_request_manager_from_string(string: str, dst: SEDataRequestManager, fmt: eSerializationFormat):
    src = DataRequestManagerData()
    json_format.Parse(string, src)
    serialize_data_request_manager_from_bind(src, dst)

def serialize_data_request_manager_to_bind(src: SEDataRequestManager, dst: DataRequestManagerData):
    if src.has_data_requests():
        for dr in src.get_data_requests():
            dst_dr = DataRequestData()
            serialize_data_request_to_bind(dr, dst_dr)
            dst.DataRequest.append(dst_dr)
    dst.ResultsFilename = src.get_results_filename()
    dst.SamplesPerSecond = src.get_samples_per_second()

def serialize_data_request_manager_from_bind(src: DataRequestManagerData, dst: SEDataRequestManager):
    if src.ResultsFilename:
        dst.set_results_filename(src.ResultsFilename)
    dst.set_samples_per_second(src.SamplesPerSecond)
    dst.set_data_requests([serialize_data_request_from_bind(dr) for dr in src.DataRequest])

def serialize_engine_initialization_to_bind(src: SEEngineInitialization, dst: EngineInitializationData):
    if src.id is not None:
        dst.ID = src.id
    if src.patient_configuration is not None:
        serialize_patient_configuration_to_bind(src.patient_configuration, dst.PatientConfiguration)
    elif src.state_filename is not None:
        dst.StateFilename = src.state_filename
    elif src.state is not None:
        dst.State = src.state
    else:
        raise Exception("Invalid Engine Configuration, must provide StartType")
    if src.data_request_mgr is not None:
        serialize_data_request_manager_to_bind(src.data_request_mgr, dst.DataRequestManager)
    dst.LogFilename = src.log_filename
    dst.KeepLogMessages = src.keep_log_messages
    dst.KeepEventChanges = src.keep_event_changes

def serialize_engine_initialization_to_string(src: SEEngineInitialization, fmt: eSerializationFormat):
    dst = EngineInitializationData()
    serialize_engine_initialization_to_bind(src, dst)
    return json_format.MessageToJson(dst, True, True)

def serialize_engine_initializations_to_string(src: [SEEngineInitialization], fmt: eSerializationFormat):
    dst = EngineInitializationListData()
    for ec in src:
        bind = EngineInitializationData()
        serialize_engine_initialization_to_bind(ec, bind)
        dst.EngineInitialization.append(bind)
    return json_format.MessageToJson(dst, True, True)

def serialize_data_requested_result_from_bind(src: DataRequestedData, dst: SEDataRequested):
    dst.set_id(src.ID)
    dst.set_active(src.IsActive)
    dst.set_headers(list(src.Headers))
    for s in src.Segment:
        dst.add_segment(s.ID, s.SimTime_s, list(s.Value))

def serialize_data_requested_result_from_string(string: str, fmt: eSerializationFormat):
    src = DataRequestedData()
    json_format.Parse(string, src)
    dst = SEDataRequested()
    serialize_data_requested_result_from_bind(src, dst)
    return dst
def serialize_data_requested_result_from_file(filename: str):
    with open(filename) as f:
        string = f.read()
    return serialize_data_requested_result_from_string(string, eSerializationFormat.JSON)

def serialize_data_requested_list_from_string(src: str, dst: [SEDataRequested], fmt: eSerializationFormat):
    results = DataRequestedListData()
    json_format.Parse(str, results)
    for id,result in results:
        e = dst[id]
        if e is None:
            raise Exception(str(id)+" not found in destination pool")
        serialize_data_requested_result_from_bind(result, e)

def serialize_decimal_format_to_bind(src: SEDecimalFormat, dst: DecimalFormatData):
    if src.has_notation():
        dst.Type = src.get_notation().value
    if src.has_precision():
        dst.Precision = src.get_precision()
def serialize_decimal_format_from_bind(src: DecimalFormatData, dst: SEDecimalFormat):
    dst.clear()
    dst.set_notation(eDecimalFormat_type(dst.Type))
    dst.set_precision(dst.Precision)

def serialize_validation_target_to_bind(src: SEValidationTarget, dst: ValidationTargetData):
    dst.Header = src.get_header()
    dst.Reference = src.get_reference()
    dst.Notes = src.get_notes()
    dst.TableFormatting = src.get_table_formatting()
def serialize_validation_target_from_bind(src: ValidationTargetData, dst: SEValidationTarget):
    dst.clear()
    dst.set_header(src.Header)
    dst.set_reference(src.Reference)
    dst.set_notes(src.Notes)

def serialize_segment_validation_target_to_bind(src: SESegmentValidationTarget, dst: SegmentValidationTargetData):
    serialize_validation_target_to_bind(src, dst.ValidationTarget)
    if src.get_comparison_type() == SESegmentValidationTarget.eComparisonType.EqualToSegment:
        dst.EqualToSegment = src.get_target_segment()
    elif src.get_comparison_type() == SESegmentValidationTarget.eComparisonType.EqualToValue:
        dst.EqualToValue = src.get_target()
    elif src.get_comparison_type() == SESegmentValidationTarget.eComparisonType.GreaterThanSegment:
        dst.GreaterThanSegment = src.get_target_segment()
    elif src.get_comparison_type() == SESegmentValidationTarget.eComparisonType.GreaterThanValue:
        dst.GreaterThanValue = src.get_target()
    elif src.get_comparison_type() == SESegmentValidationTarget.eComparisonType.LessThanSegment:
        dst.LessThanSegment = src.get_target_segment()
    elif src.get_comparison_type() == SESegmentValidationTarget.eComparisonType.LessThanValue:
        dst.LessThanValue = src.get_target()
    elif src.get_comparison_type() == SESegmentValidationTarget.eComparisonType.TrendsToSegment:
        dst.TrendsToSegment = src.get_target_segment()
    elif src.get_comparison_type() == SESegmentValidationTarget.eComparisonType.TrendsToValue:
        dst.TrendsToValue = src.get_target()
    elif src.get_comparison_type() == SESegmentValidationTarget.eComparisonType.Range:
        dst.Range.Minimum = src.get_target_minimum()
        dst.Range.Maximum = src.get_target_maximum()
    elif src.get_comparison_type() == SESegmentValidationTarget.eComparisonType.NotValidating:
        pass
    else:
        raise ValueError(f"Unknown comparison type: {src.get_comparison_type()}")
def serialize_segment_validation_target_from_bind(src: SegmentValidationTargetData, dst: SESegmentValidationTarget):
    dst.clear()
    serialize_validation_target_from_bind(src.ValidationTarget, dst)
    if src.HasField("EqualToSegment"):
        dst.set_equal_to_segment(src.EqualToSegment)
    elif src.HasField("EqualToValue"):
        dst.set_equal_to_value(src.EqualToValue)
    elif src.HasField("GreaterThanSegment"):
        dst.set_greater_than_segment(src.GreaterThanSegment)
    elif src.HasField("GreaterThanValue"):
        dst.set_greater_than_value(src.GreaterThanValue)
    elif src.HasField("LessThanSegment"):
        dst.set_less_than_segment(src.LessThanSegment)
    elif src.HasField("LessThanValue"):
        dst.set_less_than_value(src.LessThanValue)
    elif src.HasField("TrendsToSegment"):
        dst.set_trends_to_segment(src.TrendsToSegment)
    elif src.HasField("TrendsToValue"):
        dst.set_trends_to_value(src.TrendsTo)
    elif src.HasField("Range"):
        dst.set_range(src.Range.Minimum, src.Range.Maximum)
    elif src.WhichOneof('Expected') is None:  # Not validating
        pass
    else:
        raise ValueError(f"Unknown expected field: {src.WhichOneOf('Expected')}")
def serialize_segment_validation_segment_to_bind(src: SESegmentValidationSegment, dst: SegmentValidationSegmentData):
    dst.Segment = src.get_segment_id()
    dst.Notes = src.get_notes()
    for tgt in src.get_validation_targets():
        serialize_segment_validation_target_to_bind(tgt, dst.SegmentValidationTarget.add())
def serialize_segment_validation_segment_from_bind(src: SegmentValidationSegmentData):
    dst = SESegmentValidationSegment()

    dst.set_notes(src.Notes)
    dst.set_segment_id(src.Segment)

    tgts = []
    for tgtData in src.SegmentValidationTarget:
        tgt = SESegmentValidationTarget()
        serialize_segment_validation_target_from_bind(tgtData, tgt)
        tgts.append(tgt)
    dst.set_validation_targets(tgts)

    return dst

def serialize_segment_validation_segment_list_to_bind(src: List[SESegmentValidationSegment], dst: SegmentValidationSegmentListData):
    for segment in src:
        serialize_segment_validation_segment_to_bind(segment, dst.SegmentValidationSegment.add())
def serialize_segment_validation_segment_list_from_bind(src: SegmentValidationSegmentListData):
    dst = []
    for segmentData in src.SegmentValidationSegment:
        dst.append(serialize_segment_validation_segment_from_bind(segmentData))

    return dst

def serialize_segment_validation_segment_list_to_string(src: List[SESegmentValidationSegment], fmt: eSerializationFormat):
    dst = SegmentValidationSegmentListData()
    serialize_segment_validation_segment_list_to_bind(src, dst)
    return json_format.MessageToJson(dst, True, True)
def serialize_segment_validation_segment_list_to_file(src: List[SESegmentValidationSegment], filename: str):
    string = serialize_segment_validation_segment_list_to_string(src, eSerializationFormat.JSON)
    file = open(filename, "w")
    n = file.write(string)
    file.close()
def serialize_segment_validation_segment_list_from_string(string: str, fmt: eSerializationFormat):
    src = SegmentValidationSegmentListData()
    json_format.Parse(string, src)
    return serialize_segment_validation_segment_list_from_bind(src)
def serialize_segment_validation_segment_list_from_file(filename: str):
    with open(filename) as f:
        string = f.read()
    return serialize_segment_validation_segment_list_from_string(string, eSerializationFormat.JSON)

def serialize_segment_validation_pipeline_config_from_file(filename: str, dst: SESegmentValidationPipelineConfig):
    with open(filename) as f:
        string = f.read()
    serialize_segment_validation_pipeline_config_from_string(string, dst, eSerializationFormat.JSON)
def serialize_segment_validation_pipeline_config_from_string(string: str, dst: SESegmentValidationPipelineConfig, fmt: eSerializationFormat):
    src = SegmentValidationPipelineConfigurationData()
    json_format.Parse(string, src)
    serialize_segment_validation_pipeline_config_from_bind(src, dst)
def serialize_segment_validation_pipeline_config_from_bind(src: SegmentValidationPipelineConfigurationData, dst: SESegmentValidationPipelineConfig):
    dst.clear()

    serialize_plotter_list_from_bind(src.Plots, dst.get_plotters())

def serialize_time_series_validation_target_to_bind(
    src: SETimeSeriesValidationTarget,
    dst: TimeSeriesValidationTargetData
) -> None:
    serialize_validation_target_to_bind(src, dst.ValidationTarget)
    dst.Type = src.get_target_type().value
    if src.has_patient_specific_setting():
        dst.PatientSpecific = src.is_patient_specific()

    if src.get_comparison_type() == SETimeSeriesValidationTarget.eComparisonType.EqualToValue:
        dst.EqualToValue = src.get_target()
    elif src.get_comparison_type() == SETimeSeriesValidationTarget.eComparisonType.Range:
        dst.Range.Minimum = src.get_target_minimum()
        dst.Range.Maximum = src.get_target_maximum()
    elif src.get_comparison_type() == SETimeSeriesValidationTarget.eComparisonType.NotValidating:
        pass
    else:
        raise ValueError(f"Unknown comparison type: {src.get_comparison_type()}")

    if src.has_computed_value():
        dst.ComputedValue = src.get_computed_value()
    if src.has_error_value():
        dst.Error = src.get_error_value()

def serialize_time_series_validation_target_from_bind(
    src: TimeSeriesValidationTargetData,
    dst: SETimeSeriesValidationTarget
) -> None:
    dst.clear()
    serialize_validation_target_from_bind(src.ValidationTarget, dst)
    dst.set_target_type(src.Type)
    dst.set_patient_specific_setting(src.PatientSpecific)

    if src.HasField("EqualToValue"):
        dst.set_equal_to(src.EqualToValue, SETimeSeriesValidationTarget.eTargetType(src.Type))
    elif src.HasField("Range"):
        dst.set_range(src.Range.Minimum, src.Range.Maximum, SETimeSeriesValidationTarget.eTargetType(src.Type))
    elif src.WhichOneof('Expected') is None:  # Not validating
        pass
    else:
        raise ValueError(f"Unknown expected field: {src.WhichOneOf('Expected')}")

    if src.HasField("ComputedValue"):
        dst.set_computed_value(src.ComputedValue)
    if src.HasField("Error"):
        dst.set_error_value(src.Error)

def serialize_time_series_validation_target_list_to_bind(
    src: List[SETimeSeriesValidationTarget],
    dst: TimeSeriesValidationTargetListData
) -> None:
    for tgt in src:
        serialize_time_series_validation_target_to_bind(tgt, dst.TimeSeriesValidationTarget.add())
def serialize_time_series_validation_target_list_from_bind(
    src: TimeSeriesValidationTargetListData,
    dst: List[SETimeSeriesValidationTarget]
) -> None:
    for tgtData in src.TimeSeriesValidationTarget:
        tgt = SETimeSeriesValidationTarget()
        serialize_time_series_validation_target_from_bind(tgtData, tgt)
        dst.append(tgt)
def serialize_time_series_validation_target_list_to_string(
    src: List[SETimeSeriesValidationTarget],
    fmt: eSerializationFormat
) -> str:
    dst = TimeSeriesValidationTargetListData()
    serialize_time_series_validation_target_list_to_bind(src, dst)
    return json_format.MessageToJson(dst, True, True)
def serialize_time_series_validation_target_list_to_file(
    src: List[SETimeSeriesValidationTarget],
    filename: str
) -> None:
    string = serialize_time_series_validation_target_list_to_string(src, eSerializationFormat.JSON)
    with open(filename, "w") as f:
        f.write(string)
def serialize_time_series_validation_target_list_from_string(
    string: str,
    dst: List[SETimeSeriesValidationTarget],
    fmt: eSerializationFormat
) -> None:
    src = TimeSeriesValidationTargetListData()
    json_format.Parse(string, src)
    serialize_time_series_validation_target_list_from_bind(src, dst)
def serialize_time_series_validation_target_list_from_file(filename: str, dst: List[SETimeSeriesValidationTarget]) -> None:
    with open(filename) as f:
        string = f.read()
    serialize_time_series_validation_target_list_from_string(string, dst, eSerializationFormat.JSON)

def serialize_patient_time_series_validation_to_bind(
    src: SEPatientTimeSeriesValidation,
    dst: PatientTimeSeriesValidationData
) -> None:
    if src.has_patient():
        serialize_patient_to_bind(src.get_patient(), dst.Patient)
    for tgt_dest, tgts in src.get_targets().items():
        serialize_time_series_validation_target_list_to_bind(tgts, dst.TimeSeriesValidationTargetMap[tgt_dest])
def serialize_patient_time_series_validation_to_string(
    src: SEPatientTimeSeriesValidation,
    fmt: eSerializationFormat
) -> str:
    dst = PatientTimeSeriesValidationData()
    serialize_patient_time_series_validation_to_bind(src, dst)
    return json_format.MessageToJson(dst, True, True)
def serialize_patient_time_series_validation_to_file(
    src: SEPatientTimeSeriesValidation,
    filename: str
) -> None:
    string = serialize_patient_time_series_validation_to_string(src, eSerializationFormat.JSON)
    with open(filename, "w") as f:
        f.write(string)
def serialize_patient_time_series_validation_from_bind(
    src: PatientTimeSeriesValidationData,
    dst: SEPatientTimeSeriesValidation
) -> None:
    dst.clear()
    serialize_patient_from_bind(src.Patient, dst.get_patient())
    tgt_map = dst.get_targets()
    for tgt_dest, tgts in src.TimeSeriesValidationTargetMap.items():
        tgt_map[tgt_dest] = serialize_time_series_validation_target_list_from_bind(tgts)
def serialize_patient_time_series_validation_from_string(
    string: str,
    dst: SEPatientTimeSeriesValidation,
    fmt: eSerializationFormat
) -> None:
    src = PatientTimeSeriesValidationData()
    json_format.Parse(string, src)
    serialize_patient_time_series_validation_from_bind(src, dst)
def serialize_patient_time_series_validation_from_file(filename: str, dst: SEPatientTimeSeriesValidation) -> None:
    with open(filename) as f:
        string = f.read()
    serialize_patient_time_series_validation_from_string(string, dst, eSerializationFormat.JSON)

def serialize_patient_time_series_validation_list_to_bind(
    src: List[SEPatientTimeSeriesValidation],
    dst: PatientTimeSeriesValidationListData
) -> None:
    for tgt_map in src:
        serialize_patient_time_series_validation_to_bind(tgt_map, dst.PatientTimeSeriesValidation.add())
def serialize_patient_time_series_validation_list_from_bind(
    src: PatientTimeSeriesValidationListData,
    dst: List[SEPatientTimeSeriesValidation]
) -> None:
    for tgtMapData in src.PatientTimeSeriesValidation:
        tgtMap = SEPatientTimeSeriesValidation()
        serialize_patient_time_series_validation_from_bind(tgtMapData, tgtMap)
        dst.append(tgtMap)

def serialize_patient_time_series_validation_list_to_string(
    src: List[SEPatientTimeSeriesValidation],
    fmt: eSerializationFormat
) -> str:
    dst = PatientTimeSeriesValidationListData()
    serialize_patient_time_series_validation_list_to_bind(src, dst)
    return json_format.MessageToJson(dst, True, True)
def serialize_patient_time_series_validation_list_to_file(
    src: List[SEPatientTimeSeriesValidation],
    filename: str
) -> None:
    string = serialize_patient_time_series_validation_list_to_string(src, eSerializationFormat.JSON)
    with open(filename, "w") as f:
        f.write(string)
def serialize_patient_time_series_validation_list_from_string(
    string: str,
    dst: List[SEPatientTimeSeriesValidation],
    fmt: eSerializationFormat
) -> None:
    src = PatientTimeSeriesValidationListData()
    json_format.Parse(string, src)
    serialize_patient_time_series_validation_list_from_bind(src, dst)
def serialize_patient_time_series_validation_list_from_file(
    filename: str,
    dst: List[SEPatientTimeSeriesValidation]
) -> None:
    with open(filename) as f:
        string = f.read()
    serialize_patient_time_series_validation_list_from_string(string, dst, eSerializationFormat.JSON)
