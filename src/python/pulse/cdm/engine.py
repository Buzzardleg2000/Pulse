# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.
from abc import ABC, abstractmethod
from enum import Enum
from pulse.cdm.scalars import SEScalarProperty

class eSerializationFormat(Enum):
    BINARY = 0
    JSON = 1

class eSide(Enum):
    NullSide = 0
    Left = 1
    Right = 2

class eGate(Enum):
    NullGate = 0
    Open = 1
    Closed = 2

class eSwitch(Enum):
    NullSwitch = 0
    Off = 1
    On = 2

class eCharge(Enum):
    NullCharge = 0
    Negative = 1
    Neutral = 2
    Positive = 3

class SEAction(ABC):
    __slots__ = ["_comment"]

    def __init__(self):
        self._comment = None

    def copy(self, action):
        self.clear()
        self._comment = action._comment

    def clear(self):
        self.invalidate_comment()

    def has_comment(self):
        return self._comment is not None
    def get_comment(self):
        return self._comment
    def set_comment(self, comment: str):
        self._comment = comment
    def invalidate_comment(self):
        self._comment = None

    @abstractmethod
    def is_valid(self):
        pass

class SEScalarOverride(SEAction):
    __slots__ = ["_action","_scalar_override"]

    def __init__(self):
        super().__init__()
        self._action = None
        self._scalar_overrides = []
    def clear(self):
        if self._action is not None:
            self._action.invalidate()
        if self._scalar_override is not None:
            self._scalar_override.invalidate()
    def set_action(self, action):
        self._action = action
    def has_action(self):
        return self._action is not None
    def get_action(self):
        return self._action
    def add_scalar_override(self, name: str, value: float, unit: str = ""):
        self._scalar_overrides.append(SEScalarProperty(name, value, unit))
    def has_scalar_overrides(self):
        return self._scalar_override is not None
    def get_scalar_overrides(self):
        return self._scalar_override

class SECondition(ABC):
    __slots__ = ["_comment"]

    def __init__(self):
        self._comment = None

    def copy(self, condition):
        self.clear()
        self._comment = condition._comment

    def clear(self):
        self.invalidate_comment()

    @abstractmethod
    def get_name(self):
        pass

    def has_comment(self):
        return self._comment is not None
    def get_comment(self):
        return self._comment
    def set_comment(self, comment: str):
        self._comment = comment
    def invalidate_comment(self):
        self._comment = None

    @abstractmethod
    def is_valid(self):
        pass

    @abstractmethod
    def is_active(self):
        pass

from pulse.cdm.environment_conditions import SEInitialEnvironmentalConditions
from pulse.cdm.patient_conditions import *

class SEConditionManager():
    __slots__ = ["_ards", "_anemia", "_copd", "_cvsd", "_impaired_alveolar_exchange",
                 "_pericardial_effusion", "_lobar_pneumonia",
                 "_pulmonary_fibrosis", "_renal_stenosis", "_sepsis",
                 "_initial_environmental_conditions"]

    def __init__(self):
        self.clear()

    def clear(self):
        self._ards = None
        self._anemia = None
        self._copd = None
        self._cvsd = None
        self._impaired_alveolar_exchange = None
        self._pericardial_effusion = None
        self._lobar_pneumonia = None
        self._pulmonary_fibrosis = None
        self._renal_stenosis = None
        self._sepsis = None
        self._initial_environmental_conditions = None

    def is_empty(self):
        if self.has_acute_respiratory_distress_syndrome():
            return False
        if self.has_chronic_anemia():
            return False
        if self.has_chronic_obstructive_pulmonary_disease():
            return False
        if self.has_chronic_ventricular_systolic_dysfunction():
            return False
        if self.has_chronic_pericardial_effusion():
            return False
        if self.has_chronic_renal_stenosis():
            return False
        if self.has_impaired_alveolar_exchange():
            return False
        if self.has_lobar_pneumonia():
            return False
        if self.has_pulmonary_fibrosis():
            return False
        if self.has_sepsis():
            return False

        if self.has_initial_environmental_conditions():
            return False;
        return True;
    
    def has_acute_respiratory_distress_syndrome(self):
        return False if self._ards is None else self._ards.is_valid()
    def get_acute_respiratory_distress_syndrome(self):
        if self._ards is None:
            self._ards = SEAcuteRespiratoryDistressSyndrome()
        return self._ards
    def remove_acute_respiratory_distress_syndrome(self):
        self._ards = None

    def has_chronic_anemia(self):
        return False if self._anemia is None else self._anemia.is_valid()
    def get_chronic_anemia(self):
        if self._anemia is None:
            self._anemia = SEChronicAnemia()
        return self._anemia
    def remove_chronic_anemia(self):
        self._anemia = None

    def has_chronic_obstructive_pulmonary_disease(self):
        return False if self._copd is None else self._copd.is_valid()
    def get_chronic_obstructive_pulmonary_disease(self):
        if self._copd is None:
            self._copd = SEChronicObstructivePulmonaryDisease()
        return self._copd
    def remove_chronic_obstructive_pulmonary_disease(self):
        self._copd = None

    def has_chronic_ventricular_systolic_dysfunction(self):
        return False if self._cvsd is None else self._cvsd.is_valid()
    def get_chronic_ventricular_systolic_dysfunction(self):
        if self._cvsd is None:
            self._cvsd = SEChronicVentricularSystolicDysfunction()
        return self._cvsd
    def remove_chronic_ventricular_systolic_dysfunction(self):
        self._cvsd = None

    def has_chronic_pericardial_effusion(self):
        return False if self._pericardial_effusion is None else self._pericardial_effusion.is_valid()
    def get_chronic_pericardial_effusion(self):
        if self._pericardial_effusion is None:
            self._pericardial_effusion = SEChronicPericardialEffusion()
        return self._pericardial_effusion
    def remove_chronic_pericardial_effusion(self):
        self._pericardial_effusion = None

    def has_chronic_renal_stenosis(self):
        return False if self._renal_stenosis is None else self._renal_stenosis.is_valid()
    def get_chronic_renal_stenosis(self):
        if self._renal_stenosis is None:
            self._renal_stenosis = SEChronicRenalStenosis()
        return self._renal_stenosis
    def remove_chronic_renal_stenosis(self):
        self._renal_stenosis = None

    def has_impaired_alveolar_exchange(self):
        return False if self._impaired_alveolar_exchange is None else self._impaired_alveolar_exchange.is_valid()
    def get_impaired_alveolar_exchange(self):
        if self._impaired_alveolar_exchange is None:
            self._impaired_alveolar_exchange = SEImpairedAlveolarExchange()
        return self._impaired_alveolar_exchange
    def remove_impaired_alveolar_exchange(self):
        self._impaired_alveolar_exchange = None

    def has_lobar_pneumonia(self):
        return False if self._lobar_pneumonia is None else self._lobar_pneumonia.is_valid()
    def get_lobar_pneumonia(self):
        if self._lobar_pneumonia is None:
            self._lobar_pneumonia = SELobarPneumonia()
        return self._lobar_pneumonia
    def remove_lobar_pneumonia(self):
        self._lobar_pneumonia = None

    def has_pulmonary_fibrosis(self):
        return False if self._pulmonary_fibrosis is None else self._pulmonary_fibrosis.is_valid()
    def get_pulmonary_fibrosis(self):
        if self._pulmonary_fibrosis is None:
            self._pulmonary_fibrosis = SEPulmonaryFibrosis()
        return self._pulmonary_fibrosis
    def remove_pulmonary_fibrosis(self):
        self._pulmonary_fibrosis = None

    def has_renal_stenosis(self):
        return False if self._renal_stenosis is None else self._renal_stenosis.is_valid()
    def get_renal_stenosis(self):
        if self._renal_stenosis is None:
            self._renal_stenosis = SEChronicRenalStenosis()
        return self._renal_stenosis
    def remove_renal_stenosis(self):
        self._renal_stenosis = None

    def has_sepsis(self):
        return False if self._sepsis is None else self._sepsis.is_valid()
    def get_sepsis(self):
        if self._sepsis is None:
            self._sepsis = SESepsis()
        return self._sepsis
    def remove_sepsis(self):
        self._sepsis = None

    def has_initial_environmental_conditions(self):
        return False if self._initial_environmental_conditions is None else self._initial_environmental_conditions.is_valid()
    def get_initial_environmental_conditions(self):
        if self._initial_environmental_conditions is None:
            self._initial_environmental_conditions = SEInitialEnvironmentalConditions()
        return self._initial_environmental_conditions
    def remove_initial_environmental_conditions(self):
        self._initial_environmental_conditions = None

class eDataRequest_category(Enum):
    Patient = 0
    Physiology = 1
    Environment = 2
    GasCompartment = 3
    LiquidCompartment = 4
    ThermalCompartment = 5
    TissueCompartment = 6
    Substance = 7
    AnesthesiaMachine = 8
    ECG = 9
    Inhaler = 10

class SEDataRequest:
    __slots__ = ['_category', '_compartment_name', '_substance_name', '_property_name', '_unit']

    def __init__(self, category: eDataRequest_category, compartment=None, substance=None, property=None, unit=None):
        if category is None:
            raise Exception("Must provide a Data Request Category")
        if property is None:
            raise Exception("Must provide a Data Request Property Name")
        if (compartment is None and (category is eDataRequest_category.GasCompartment or
                                     category is eDataRequest_category.LiquidCompartment or
                                     category is eDataRequest_category.ThermalCompartment or
                                     category is eDataRequest_category.TissueCompartment)):
            raise Exception("Must provide a Compartment Name for Compartment Data Requests")
        if (substance is None and category is eDataRequest_category.Substance):
            raise Exception("Must provide a Substance Name for Substance Data Requests")
        self._category = category
        self._compartment_name = compartment
        self._substance_name = substance
        self._property_name = property
        self._unit = unit

    def __repr__(self):
        out_string = ""
        if self.has_compartment_name():
            out_string += "{} - ".format(self._compartment_name)
        if self.has_substance_name():
            out_string += "{} - ".format(self._substance_name)
        out_string += "{} ({})".format(self._property_name, self._unit)
        return out_string

    def to_string(self):
        return self.__repr__()

    @classmethod
    def create_patient_request(cls, property, unit=None):
        return cls(eDataRequest_category.Patient, property=property,  unit=unit)
    @classmethod
    def create_physiology_request(cls, property, unit=None):
        return cls(eDataRequest_category.Physiology, property=property,  unit=unit)
    @classmethod
    def create_environment_request(cls, property, unit=None):
        return cls(eDataRequest_category.Environment, property=property,  unit=unit)
    @classmethod
    def create_gas_compartment_request(cls, compartment, property, unit=None):
        return cls(eDataRequest_category.GasCompartment, compartment=compartment, property=property,  unit=unit)
    @classmethod
    def create_gas_compartment_substance_request(cls, compartment, substance, property, unit=None):
        return cls(eDataRequest_category.GasCompartment,
                   compartment=compartment,
                   substance=substance,
                   property=property,
                   unit=unit)
    @classmethod
    def create_liquid_compartment_request(cls, compartment, property, unit=None):
        return cls(eDataRequest_category.LiquidCompartment, compartment=compartment, property=property,  unit=unit)
    @classmethod
    def create_liquid_compartment_substance_request(cls, compartment, substance, property, unit=None):
        return cls(eDataRequest_category.LiquidCompartment,
                   compartment=compartment,
                   substance=substance,
                   property=property,
                   unit=unit)
    @classmethod
    def create_thermal_compartment_request(cls, compartment, property, unit=None):
        return cls(eDataRequest_category.ThermalCompartment, compartment=compartment, property=property,  unit=unit)

    @classmethod
    def create_substance_request(cls, substance, property, unit=None):
        return cls(eDataRequest_category.Substance, substance=substance, property=property,  unit=unit)

    @classmethod
    def create_ecg_request(cls, property, unit=None):
        return cls(eDataRequest_category.ECG, property=property,  unit=unit)
    @classmethod
    def create_anesthesia_machine_request(cls, property, unit=None):
        return cls(eDataRequest_category.AnesthesiaMachine, property=property,  unit=unit)
    @classmethod
    def create_inhaler_request(cls, property, unit=None):
        return cls(eDataRequest_category.Inhaler, property=property,  unit=unit)


    def get_category(self):
        return self._category

    def has_compartment_name(self):
        return self._compartment_name is not None
    def get_compartment_name(self):
        return self._compartment_name
    def has_substance_name(self):
        return self._substance_name is not None
    def get_substance_name(self):
        return self._substance_name
    def has_property_name(self):
        return self._property_name is not None
    def get_property_name(self):
        return self._property_name
    def has_unit(self):
        return self._unit is not None
    def get_unit(self):
        return self._unit


class SEDataRequestManager:
    __slots__ = ["_results_filename", "_samples_per_second", "_data_requests"]

    def __init__(self, data_requests=None):
        self._data_requests = data_requests
        self._results_filename = None
        self._samples_per_second = 0

    def has_data_requests(self): return len(self._data_requests)
    def get_data_requests(self): return self._data_requests
    def set_data_requests(self, requests): self._data_requests = requests

    def has_results_filename(self): return self._results_filename is not None
    def get_results_filename(self): return self._results_filename
    def set_results_filename(self, filename): self._results_filename = filename

    def get_samples_per_second(self): return self._samples_per_second
    def set_samples_per_second(self, sample): self._samples_per_second = sample

    def to_console(self, data_values):
        for key in data_values:
            print("{}={}".format(key, data_values[key]))
