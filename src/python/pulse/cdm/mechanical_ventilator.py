# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from enum import Enum
from pulse.cdm.equipment import SEEquipment
from pulse.cdm.substance import SESubstanceFraction, \
                                SESubstanceConcentration
from pulse.cdm.scalars import SEScalarPressureTimePerVolume, SEScalarVolumePerTime, \
                              SEScalarPressure, SEScalarTime, SEScalarVolume, \
                              MassPerVolumeUnit

class eConnection(Enum):
    NullConnection = 0
    Off = 1
    Mask = 2
    Tube = 3

class eDriverWaveform(Enum):
    NullDriverWaveform = 0
    Square = 1
    Exponential = 2
    Ramp = 3
    Sinusoidal = 4
    Sigmoidal = 5

class SEMechanicalVentilator(SEEquipment):
    __slots__ = ["_connection",
                 # One of
                 "_positive_end_expired_pressure",
                 "_functional_residual_capacity",
                 # One of
                 "_expiration_cycle_flow",
                 "_expiration_cycle_pressure",
                 "_expiration_cycle_time",
                 "_expiration_cycle_volume",

                 "_expiration_tube_resistance",
                 "_expiration_valve_resistance",
                 "_expiration_waveform",
                 # One of
                 "_inspiration_limit_flow",
                 "_inspiration_limit_pressure",
                 "_inspiration_limit_volume",

                 "_inspiration_pause_time",
                 # One of
                 "_peak_inspiratory_pressure",
                 "_inspiration_target_flow",
                 # One of
                 "_inspiration_machine_trigger_time",
                 # One of
                 "_inspiration_patient_trigger_flow",
                 "_inspiration_patient_trigger_pressure",

                 "_inspiration_tube_resistance",
                 "_inspiration_valve_resistance",
                 "_inspiration_waveform",

                 "_fraction_inspired_gasses",
                 "_concentration_inspired_aerosol"]

    def __init__(self):
        super().__init__()
        self._connection = eConnection.NullConnection

        self._positive_end_expired_pressure = None
        self._functional_residual_capacity = None

        self._expiration_cycle_flow = None
        self._expiration_cycle_pressure = None
        self._expiration_cycle_time = None
        self._expiration_cycle_volume = None

        self._expiration_tube_resistance = None
        self._expiration_valve_resistance = None
        self._expiration_waveform = eDriverWaveform.NullDriverWaveform

        self._inspiration_limit_flow = None
        self._inspiration_limit_pressure = None
        self._inspiration_limit_volume = None

        self._inspiration_pause_time = None

        self._peak_inspiratory_pressure = None
        self._inspiration_target_flow = None

        self._inspiration_machine_trigger_time = None

        self._inspiration_patient_trigger_flow = None
        self._inspiration_patient_trigger_pressure = None

        self._inspiration_tube_resistance = None
        self._inspiration_valve_resistance = None
        self._inspiration_waveform = eDriverWaveform.NullDriverWaveform

        self._fraction_inspired_gasses = []
        self._concentration_inspired_aerosol = []

    def clear(self):
        self._connection = eConnection.NullConnection

        if self._positive_end_expired_pressure is not None: self._positive_end_expired_pressure.invalidate()
        if self._functional_residual_capacity is not None: self._functional_residual_capacity.invalidate()

        if self._expiration_cycle_flow is not None: self._expiration_cycle_flow.invalidate()
        if self._expiration_cycle_pressure is not None: self._expiration_cycle_pressure.invalidate()
        if self._expiration_cycle_time is not None: self._expiration_cycle_time.invalidate()
        if self._expiration_cycle_volume is not None: self._expiration_cycle_volume.invalidate()

        if self._expiration_tube_resistance is not None: self._expiration_tube_resistance.invalidate()
        if self._expiration_valve_resistance is not None: self._expiration_valve_resistance.invalidate()
        self._expiration_waveform = eDriverWaveform.NullDriverWaveform

        if self._inspiration_limit_flow is not None: self._inspiration_limit_flow.invalidate()
        if self._inspiration_limit_pressure is not None: self._inspiration_limit_pressure.invalidate()
        if self._inspiration_limit_volume is not None: self._inspiration_limit_volume.invalidate()

        if self._inspiration_pause_time is not None: self._inspiration_pause_time.invalidate()

        if self._peak_inspiratory_pressure is not None: self._peak_inspiratory_pressure.invalidate()
        if self._inspiration_target_flow is not None: self._inspiration_target_flow.invalidate()
        
        if self._inspiration_machine_trigger_time is not None: self._inspiration_machine_trigger_time.invalidate()

        if self._inspiration_patient_trigger_flow is not None: self._inspiration_patient_trigger_flow.invalidate()
        if self._inspiration_patient_trigger_pressure is not None: self._inspiration_patient_trigger_pressure.invalidate()
        
        if self._inspiration_tube_resistance is not None: self._inspiration_tube_resistance.invalidate()
        if self._inspiration_valve_resistance is not None: self._inspiration_valve_resistance.invalidate()
        self._inspiration_waveform = eDriverWaveform.NullDriverWaveform

        self._fraction_inspired_gasses = []
        self._concentration_inspired_aerosol = []

    def copy(self, src):
        if not isinstance(SEMechanicalVentilator, src):
            raise Exception("Provided argument must be a SEMechanicalVentilator")
        self.clear()
        self._connection = src._connection

        if src.has_positive_end_expired_pressure(): self.get_positive_end_expired_pressure().set(src._positive_end_expired_pressure)
        if src.has_functional_residual_capacity(): self.get_functional_residual_capacity().set(src._functional_residual_capacity)

        if src.has_expiration_cycle_flow(): self.get_expiration_cycle_flow().set(src._expiration_cycle_flow)
        if src.has_expiration_cycle_pressure(): self.get_expiration_cycle_pressure().set(src._expiration_cycle_pressure)
        if src.has_expiration_cycle_time(): self.get_expiration_cycle_time().set(src._expiration_cycle_time)
        if src.has_expiration_cycle_volume(): self.get_expiration_cycle_volume().set(src._expiration_cycle_volume)

        if src.has_expiration_tube_resistance(): self.get_expiration_tube_resistance().set(src._expiration_tube_resistance)
        if src.has_expiration_valve_resistance(): self.get_expiration_valve_resistance().set(src._expiration_valve_resistance)
        self._expiration_waveform = src._expiration_waveform

        if src.has_inspiration_limit_flow(): self.get_inspiration_limit_flow().set(src._inspiration_limit_flow)
        if src.has_inspiration_limit_pressure(): self.get_inspiration_limit_pressure().set(src._inspiration_limit_pressure)
        if src.has_inspiration_limit_volume(): self.get_inspiration_limit_volume().set(src._inspiration_limit_volume)

        if src.has_inspiration_pause_time(): self.get_inspiration_pause_time().set(src._inspiration_pause_time)

        if src.has_peak_inspiratory_pressure(): self.get_peak_inspiratory_pressure().set(src._peak_inspiratory_pressure)
        if src.has_inspiration_target_flow(): self.get_inspiration_target_flow().set(src._inspiration_target_flow)

        if src.has_inspiration_machine_trigger_time(): self.get_inspiration_machine_trigger_time().set(src._inspiration_machine_trigger_time)

        if src.has_inspiration_patient_trigger_flow(): self.get_inspiration_patient_trigger_flow().set(src._inspiration_patient_trigger_flow)
        if src.has_inspiration_patient_trigger_pressure(): self.get_inspiration_patient_trigger_pressure().set(src._inspiration_patient_trigger_pressure)
        
        if src.has_inspiration_tube_resistance(): self.get_inspiration_tube_resistance().set(src._inspiration_tube_resistance)
        if src.has_inspiration_valve_resistance(): self.get_inspiration_valve_resistance().set(src._inspiration_valve_resistance)
        self._inspiration_waveform = src._inspiration_waveform

        if src.has_fraction_inspired_gasses:
            self._fraction_inspired_gasses.append(src._fraction_inspired_gasses[:])
        if src.has_concentration_inspired_aerosol:
            self._concentration_inspired_aerosol.append(src._concentration_inspired_aerosol[:])

    def get_connection(self):
        return self._connection
    def set_connection(self, t: eConnection):
        self._connection = t
    
    def has_positive_end_expired_pressure(self):
        return False if self._positive_end_expired_pressure is None else self._positive_end_expired_pressure.is_valid()
    def get_positive_end_expired_pressure(self):
        if self._positive_end_expired_pressure is None:
            self._positive_end_expired_pressure = SEScalarPressure()
        return self._positive_end_expired_pressure
    
    def has_functional_residual_capacity(self):
        return False if self._functional_residual_capacity is None else self._functional_residual_capacity.is_valid()
    def get_functional_residual_capacity(self):
        if self._functional_residual_capacity is None:
            self._functional_residual_capacity = SEScalarPressure()
        return self._functional_residual_capacity
    
    def has_expiration_cycle_flow(self):
        return False if self._expiration_cycle_flow is None else self._expiration_cycle_flow.is_valid()
    def get_expiration_cycle_flow(self):
        if self._expiration_cycle_flow is None:
            self._expiration_cycle_flow = SEScalarVolumePerTime()
        return self._expiration_cycle_flow
    
    def has_expiration_cycle_pressure(self):
        return False if self._expiration_cycle_pressure is None else self._expiration_cycle_pressure.is_valid()
    def get_expiration_cycle_pressure(self):
        if self._expiration_cycle_pressure is None:
            self._expiration_cycle_pressure = SEScalarPressure()
        return self._expiration_cycle_pressure
    
    def has_expiration_cycle_time(self):
        return False if self._expiration_cycle_time is None else self._expiration_cycle_time.is_valid()
    def get_expiration_cycle_time(self):
        if self._expiration_cycle_time is None:
            self._expiration_cycle_time = SEScalarTime()
        return self._expiration_cycle_time
    
    def has_expiration_cycle_volume(self):
        return False if self._expiration_cycle_volume is None else self._expiration_cycle_volume.is_valid()
    def get_expiration_cycle_volume(self):
        if self._expiration_cycle_volume is None:
            self._expiration_cycle_volume = SEScalarVolume()
        return self._expiration_cycle_volume
    
    def has_expiration_tube_resistance(self):
        return False if self._expiration_tube_resistance is None else self._expiration_tube_resistance.is_valid()
    def get_expiration_tube_resistance(self):
        if self._expiration_tube_resistance is None:
            self._expiration_tube_resistance = SEScalarPressureTimePerVolume()
        return self._expiration_tube_resistance
    
    def has_expiration_valve_resistance(self):
        return False if self._expiration_valve_resistance is None else self._expiration_valve_resistance.is_valid()
    def get_expiration_valve_resistance(self):
        if self._expiration_valve_resistance is None:
            self._expiration_valve_resistance = SEScalarPressureTimePerVolume()
        return self._expiration_valve_resistance

    def get_expiration_waveform(self):
        return self._expiration_waveform
    def set_expiration_waveform(self, t: eDriverWaveform):
        self._expiration_waveform = t
        
    def has_inspiration_limit_flow(self):
        return False if self._inspiration_limit_flow is None else self._inspiration_limit_flow.is_valid()
    def get_inspiration_limit_flow(self):
        if self._inspiration_limit_flow is None:
            self._inspiration_limit_flow = SEScalarVolumePerTime()
        return self._inspiration_limit_flow

    def has_inspiration_limit_pressure(self):
        return False if self._inspiration_limit_pressure is None else self._inspiration_limit_pressure.is_valid()
    def get_inspiration_limit_pressure(self):
        if self._inspiration_limit_pressure is None:
            self._inspiration_limit_pressure = SEScalarPressure()
        return self._inspiration_limit_pressure

    def has_inspiration_limit_volume(self):
        return False if self._inspiration_limit_volume is None else self._inspiration_limit_volume.is_valid()
    def get_inspiration_limit_volume(self):
        if self._inspiration_limit_volume is None:
            self._inspiration_limit_volume = SEScalarVolume()
        return self._inspiration_limit_volume
        
    def has_inspiration_pause_time(self):
        return False if self._inspiration_pause_time is None else self._inspiration_pause_time.is_valid()
    def get_inspiration_pause_time(self):
        if self._inspiration_pause_time is None:
            self._inspiration_pause_time = SEScalarTime()
        return self._inspiration_pause_time

    def has_peak_inspiratory_pressure(self):
        return False if self._peak_inspiratory_pressure is None else self._peak_inspiratory_pressure.is_valid()
    def get_peak_inspiratory_pressure(self):
        if self._peak_inspiratory_pressure is None:
            self._peak_inspiratory_pressure = SEScalarPressure()
        return self._peak_inspiratory_pressure

    def has_inspiration_target_flow(self):
        return False if self._inspiration_target_flow is None else self._inspiration_target_flow.is_valid()
    def get_inspiration_target_flow(self):
        if self._inspiration_target_flow is None:
            self._inspiration_target_flow = SEScalarPressure()
        return self._inspiration_target_flow

    def has_inspiration_machine_trigger_time(self):
        return False if self._inspiration_machine_trigger_time is None else self._inspiration_machine_trigger_time.is_valid()
    def get_inspiration_machine_trigger_time(self):
        if self._inspiration_machine_trigger_time is None:
            self._inspiration_machine_trigger_time = SEScalarTime()
        return self._inspiration_machine_trigger_time

    def has_inspiration_patient_trigger_flow(self):
        return False if self._inspiration_patient_trigger_flow is None else self._inspiration_patient_trigger_flow.is_valid()
    def get_inspiration_patient_trigger_flow(self):
        if self._inspiration_patient_trigger_flow is None:
            self._inspiration_patient_trigger_flow = SEScalarVolumePerTime()
        return self._inspiration_patient_trigger_flow

    def has_inspiration_patient_trigger_pressure(self):
        return False if self._inspiration_patient_trigger_pressure is None else self._inspiration_patient_trigger_pressure.is_valid()
    def get_inspiration_patient_trigger_pressure(self):
        if self._inspiration_patient_trigger_pressure is None:
            self._inspiration_patient_trigger_pressure = SEScalarPressure()
        return self._inspiration_patient_trigger_pressure

    def has_inspiration_tube_resistance(self):
        return False if self._inspiration_tube_resistance is None else self._inspiration_tube_resistance.is_valid()
    def get_inspiration_tube_resistance(self):
        if self._inspiration_tube_resistance is None:
            self._inspiration_tube_resistance = SEScalarPressureTimePerVolume()
        return self._inspiration_tube_resistance

    def has_inspiration_valve_resistance(self):
        return False if self._inspiration_valve_resistance is None else self._inspiration_valve_resistance.is_valid()
    def get_inspiration_valve_resistance(self):
        if self._inspiration_valve_resistance is None:
            self._inspiration_valve_resistance = SEScalarPressureTimePerVolume()
        return self._inspiration_valve_resistance

    def get_inspiration_waveform(self):
        return self._inspiration_waveform
    def set_inspiration_waveform(self, t: eDriverWaveform):
        self._inspiration_waveform = t
        
    def has_fraction_inspired_gas(self, substance_name: str = None):
        if substance_name is None:
            return len(self._fraction_inspired_gasses) > 0
        else:
            for sf in self._fraction_inspired_gasses:
                if sf.get_substance() is substance_name:
                    return True
        return False
    def get_fraction_inspired_gas(self, substance_name: str):
        for sf in self._fraction_inspired_gasses:
            if sf.get_substance() is substance_name:
                return sf
        sf = SESubstanceFraction()
        sf.set_substance(substance_name)
        sf.get_fraction_amount().set_value(0)
        self._fraction_inspired_gasses.append(sf)
        return sf
    def get_fraction_inspired_gasses(self):
        return self._fraction_inspired_gasses
    def remove_fraction_inspired_gas(self, substance_name: str):
        for sf in self._fraction_inspired_gasses:
            if sf.get_substance() is substance_name:
                self._fraction_inspired_gasses.remove(sf)
    def remove_fraction_inspired_gasses(self):
        self._fraction_inspired_gasses = []

    def has_concentration_inspired_aerosol(self, substance_name: str=None):
        if substance_name is None:
            return len(self._concentration_inspired_aerosol) > 0
        else:
            for sf in self._concentration_inspired_aerosol:
                if sf.get_substance() is substance_name:
                    return True
        return False
    def get_concentration_inspired_aerosol(self, substance_name: str):
        for sf in self._concentration_inspired_aerosol:
            if sf.get_substance() is substance_name:
                return sf
        sf = SESubstanceConcentration()
        sf.set_substance(substance_name)
        sf.get_concentration().set_value(0, MassPerVolumeUnit.kg_Per_m3)
        self._concentration_inspired_aerosol.append(sf)
        return sf
    def get_concentration_inspired_aerosols(self):
        return self._concentration_inspired_aerosol
    def remove_concentration_inspired_aerosol(self, substance_name: str):
        for sf in self._concentration_inspired_aerosol:
            if sf.get_substance() is substance_name:
                self._concentration_inspired_aerosol.remove(sf)
    def remove_concentration_inspired_aerosol(self):
        self._concentration_inspired_aerosol = []