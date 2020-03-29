# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

class SEScalar():
    __slots__ = ["_value"]

    def __init__(self, value: float=None):
        if value is None:
            self._value = None
        else:
            self._value = value

    def set(self, scalar):
        if isinstance(SEScalar, scalar):
            self._value = scalar._value
        else:
            raise Exception("Provided argument must be a SEScalar")

    def is_valid(self):
        return self._value is not None

    def is_positive(self):
        return self._value is not None and self._value > 0

    def is_negative(self):
        return self._value is not None and self._value < 0

    def get_value(self):
        return self._value
    def set_value(self, value: float):
        self._value = value
    def invalidate(self):
        self._value = None

    def __repr__(self):
        return " {} """.format(self._value)

class SEScalar0To1(SEScalar):
    def set_value(self, value: float):
        if value>1.0 or value<0.0:
            raise Exception("Scalar0To1 must be [0,1]")
        self._value = value

class SEScalarNegative1To1(SEScalar):
    def set_value(self, value: float):
        if value>1 or value<-1:
            raise Exception("ScalarNegative1To1 must be [-1,1]")
        self._value = value

class SEScalarQuantity(SEScalar):
    __slots__ = ["_units"]

    def is_valid(self):
        return self._value is not None and self._units is not None

    def get_unit(self):
        return self._units

    def invalidate(self):
        self._value = None
        self._units = None

    def __repr__(self):
        return "{}{}".format(self._value, self._units)

class SEScalarUnit():
    __slots__ = ["_string"]

    def __init__(self, string: str):
        self._string = string

    def get_string(self):
        return self._string

    def __repr__(self):
        return "({})".format(self._string)

class AreaUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string == AreaUnit.cm2.get_string():
            return AreaUnit.cm2
        if string == AreaUnit.m2.get_string():
            return AreaUnit.m2
        raise Exception("No AreaUnit defined for " + string)
AreaUnit.cm2 = AreaUnit("cm^2")
AreaUnit.m2 = AreaUnit("m^2")
class SEScalarArea(SEScalarQuantity):

    def __init__(self, value:float=None, units:AreaUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarArea, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarArea")

    def get_value(self, units: AreaUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: AreaUnit):
        if isinstance(units, AreaUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a AreaUnit")

class FrequencyUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string == FrequencyUnit.Per_min.get_string():
            return FrequencyUnit.Per_min
        if string == FrequencyUnit.Per_s.get_string():
            return FrequencyUnit.Per_s
        if string == FrequencyUnit.Hz.get_string():
            return FrequencyUnit.Hz
        raise Exception("No FrequencyUnit defined for " + string)
FrequencyUnit.Per_min = FrequencyUnit("1/min")
FrequencyUnit.Per_s = FrequencyUnit("1/s")
FrequencyUnit.Hz = FrequencyUnit("Hz")
class SEScalarFrequency(SEScalarQuantity):

    def __init__(self, value:float=None, units:FrequencyUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarFrequency, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarFrequency")

    def get_value(self, units: FrequencyUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: FrequencyUnit):
        if isinstance(units, FrequencyUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a FrequencyUnit")

class HeatResistanceAreaUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string == HeatResistanceAreaUnit.rsi.get_string():
            return HeatResistanceAreaUnit.rsi
        if string == HeatResistanceAreaUnit.clo.get_string():
            return HeatResistanceAreaUnit.clo
        if string == HeatResistanceAreaUnit.rValue.get_string():
            return HeatResistanceAreaUnit.rValue
        if string == HeatResistanceAreaUnit.tog.get_string():
            return HeatResistanceAreaUnit.tog
        raise Exception("No HeatResistanceAreaUnit defined for " + string)
HeatResistanceAreaUnit.rsi = HeatResistanceAreaUnit("rsi")
HeatResistanceAreaUnit.clo = HeatResistanceAreaUnit("clo")
HeatResistanceAreaUnit.rValue = HeatResistanceAreaUnit("rValue")
HeatResistanceAreaUnit.tog = HeatResistanceAreaUnit("tog")
class SEScalarHeatResistanceArea(SEScalarQuantity):

    def __init__(self, value:float=None, units:HeatResistanceAreaUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarHeatResistanceArea, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarHeatResistanceArea")

    def get_value(self, units: HeatResistanceAreaUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: HeatResistanceAreaUnit):
        if isinstance(units, HeatResistanceAreaUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a HeatResistanceAreaUnit")

class InversePressureUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string == InversePressureUnit.Inverse_Pa.get_string():
            return InversePressureUnit.Inverse_Pa
        if string == InversePressureUnit.Inverse_mmHg.get_string():
            return InversePressureUnit.Inverse_mmHg
        if string == InversePressureUnit.Inverse_cmH2O.get_string():
            return InversePressureUnit.Inverse_cmH2O
        if string == InversePressureUnit.Inverse_atm.get_string():
            return InversePressureUnit.Inverse_atm
        raise Exception("No InversePressureUnit defined for " + string)
InversePressureUnit.Inverse_Pa = InversePressureUnit("1/Pa")
InversePressureUnit.Inverse_mmHg = InversePressureUnit("1/mmHg")
InversePressureUnit.Inverse_cmH2O = InversePressureUnit("1/cmH2O")
InversePressureUnit.Inverse_atm = InversePressureUnit("1/atm")
class SEScalarInversePressure(SEScalarQuantity):

    def __init__(self, value:float=None, units:InversePressureUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarInversePressure, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarInversePressure")

    def get_value(self, units: InversePressureUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: InversePressureUnit):
        if isinstance(units, InversePressureUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a InversePressureUnit")

class LengthUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string == LengthUnit.m.get_string():
            return LengthUnit.m
        if string == LengthUnit.cm.get_string():
            return LengthUnit.cm
        if string == LengthUnit.mm.get_string():
            return LengthUnit.mm
        if string == LengthUnit.um.get_string():
            return LengthUnit.um
        if string == LengthUnit.inch.get_string():
            return LengthUnit.inch
        if string == LengthUnit.ft.get_string():
            return LengthUnit.ft
        raise Exception("No LengthUnit defined for " + string)
LengthUnit.m = LengthUnit("m")
LengthUnit.cm = LengthUnit("cm")
LengthUnit.mm = LengthUnit("mm")
LengthUnit.um = LengthUnit("um")
LengthUnit.inch = LengthUnit("in")
LengthUnit.ft = LengthUnit("ft")
class SEScalarLength(SEScalarQuantity):

    def __init__(self, value:float=None, units:LengthUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarLength, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarLength")

    def get_value(self, units: LengthUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: LengthUnit):
        if isinstance(units, LengthUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a LengthUnit")

class LengthPerTimeUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string == LengthPerTimeUnit.m_Per_s.get_string():
            return LengthPerTimeUnit.m_Per_s
        if string == LengthPerTimeUnit.cm_Per_s.get_string():
            return LengthPerTimeUnit.cm_Per_s
        if string == LengthPerTimeUnit.m_Per_min.get_string():
            return LengthPerTimeUnit.m_Per_min
        if string == LengthPerTimeUnit.cm_Per_min.get_string():
            return LengthPerTimeUnit.cm_Per_min
        if string == LengthPerTimeUnit.ft_Per_s.get_string():
            return LengthPerTimeUnit.ft_Per_s
        if string == LengthPerTimeUnit.ft_Per_min.get_string():
            return LengthPerTimeUnit.ft_Per_min
        if string == LengthPerTimeUnit.km_Per_hr.get_string():
            return LengthPerTimeUnit.km_Per_hr
        raise Exception("No LengthPerTimeUnit defined for " + string)
LengthPerTimeUnit.m_Per_s = LengthPerTimeUnit("m/s")
LengthPerTimeUnit.cm_Per_s = LengthPerTimeUnit("cm/s")
LengthPerTimeUnit.m_Per_min = LengthPerTimeUnit("m/min")
LengthPerTimeUnit.cm_Per_min = LengthPerTimeUnit("cm/min")
LengthPerTimeUnit.ft_Per_s = LengthPerTimeUnit("ft/s")
LengthPerTimeUnit.ft_Per_min = LengthPerTimeUnit("ft/min")
LengthPerTimeUnit.km_Per_hr = LengthPerTimeUnit("km/hr")
class SEScalarLengthPerTime(SEScalarQuantity):

    def __init__(self, value:float=None, units:LengthPerTimeUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarLengthPerTime, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarLengthPerTime")

    def get_value(self, units: LengthPerTimeUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: LengthPerTimeUnit):
        if isinstance(units, LengthPerTimeUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a LengthUnit")


class MassUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string == MassUnit.g.get_string():
            return MassUnit.g
        if string == MassUnit.ug.get_string():
            return MassUnit.ug
        if string == MassUnit.mg.get_string():
            return MassUnit.mg
        if string == MassUnit.kg.get_string():
            return MassUnit.kg
        if string == MassUnit.lb.get_string():
            return MassUnit.lb
        raise Exception("No MassUnit defined for " + string)
MassUnit.g = MassUnit("g")
MassUnit.ug = MassUnit("ug")
MassUnit.mg = MassUnit("mg")
MassUnit.kg = MassUnit("kg")
MassUnit.lb = MassUnit("lb")
class SEScalarMass(SEScalarQuantity):

    def __init__(self, value:float=None, units:MassUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarMass, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarMass")

    def get_value(self, units: MassUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: MassUnit):
        if isinstance(units, MassUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a MassUnit")

class MassPerAmountUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string == MassPerAmountUnit.g_Per_ct.get_string():
            return MassPerAmountUnit.g_Per_ct
        if string == MassPerAmountUnit.g_Per_mol.get_string():
            return MassPerAmountUnit.g_Per_mol
        if string == MassPerAmountUnit.g_Per_mmol.get_string():
            return MassPerAmountUnit.g_Per_mmol
        if string == MassPerAmountUnit.g_Per_umol.get_string():
            return MassPerAmountUnit.g_Per_umol
        if string == MassPerAmountUnit.mg_Per_mol.get_string():
            return MassPerAmountUnit.mg_Per_mol
        if string == MassPerAmountUnit.mg_Per_mmol.get_string():
            return MassPerAmountUnit.mg_Per_mmol
        if string == MassPerAmountUnit.kg_Per_mol.get_string():
            return MassPerAmountUnit.kg_Per_mol
        if string == MassPerAmountUnit.ug_Per_mmol.get_string():
            return MassPerAmountUnit.ug_Per_mmol
        if string == MassPerAmountUnit.ug_Per_mol.get_string():
            return MassPerAmountUnit.ug_Per_mol
        if string == MassPerAmountUnit.ug_Per_ct.get_string():
            return MassPerAmountUnit.ug_Per_ct
        if string == MassPerAmountUnit.pg_Per_ct.get_string():
            return MassPerAmountUnit.pg_Per_ct
        raise Exception("No MassPerAmountUnit defined for " + string)
MassPerAmountUnit.g_Per_ct = MassPerAmountUnit("g/ct")
MassPerAmountUnit.g_Per_mol = MassPerAmountUnit("g/mol")
MassPerAmountUnit.g_Per_mmol = MassPerAmountUnit("g/mmol")
MassPerAmountUnit.g_Per_umol = MassPerAmountUnit("g/umol")
MassPerAmountUnit.mg_Per_mol = MassPerAmountUnit("mg/mol")
MassPerAmountUnit.mg_Per_mmol = MassPerAmountUnit("mg/mmol")
MassPerAmountUnit.kg_Per_mol = MassPerAmountUnit("kg/mol")
MassPerAmountUnit.ug_Per_mmol = MassPerAmountUnit("ug/mmol")
MassPerAmountUnit.ug_Per_mol = MassPerAmountUnit("ug/mol")
MassPerAmountUnit.ug_Per_ct = MassPerAmountUnit("ug/ct")
MassPerAmountUnit.pg_Per_ct = MassPerAmountUnit("pg/ct")
class SEScalarMassPerAmount(SEScalarQuantity):

    def __init__(self, value:float=None, units:MassPerAmountUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarMassPerAmount, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarMassPerAmount")

    def get_value(self, units: MassPerAmountUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: MassPerAmountUnit):
        if isinstance(units, MassPerAmountUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a MassPerAmountUnit")

class MassPerAreaTimeUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string == MassPerAreaTimeUnit.g_Per_cm2_s.get_string():
            return MassPerAreaTimeUnit.g_Per_cm2_s
        raise Exception("No MassPerAreaTimeUnit defined for " + string)
MassPerAreaTimeUnit.g_Per_cm2_s = MassPerAreaTimeUnit("g/cm^2 s")
class SEScalarMassPerAreaTime(SEScalarQuantity):

    def __init__(self, value:float=None, units:MassPerAreaTimeUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarMassPerAreaTime, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarMassPerAreaTime")

    def get_value(self, units: MassPerAreaTimeUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: MassPerAreaTimeUnit):
        if isinstance(units, MassPerAreaTimeUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a MassPerAreaTimeUnit")

class MassPerTimeUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string == MassPerTimeUnit.g_Per_s.get_string():
            return MassPerTimeUnit.g_Per_s
        if string == MassPerTimeUnit.g_Per_min.get_string():
            return MassPerTimeUnit.g_Per_min
        if string == MassPerTimeUnit.g_Per_day.get_string():
            return MassPerTimeUnit.g_Per_day
        if string == MassPerTimeUnit.mg_Per_s.get_string():
            return MassPerTimeUnit.mg_Per_s
        if string == MassPerTimeUnit.mg_Per_min.get_string():
            return MassPerTimeUnit.mg_Per_min
        if string == MassPerTimeUnit.ug_Per_s.get_string():
            return MassPerTimeUnit.ug_Per_s
        if string == MassPerTimeUnit.kg_Per_s.get_string():
            return MassPerTimeUnit.kg_Per_s
        if string == MassPerTimeUnit.ug_Per_min.get_string():
            return MassPerTimeUnit.ug_Per_min
        raise Exception("No MassPerTimeUnit defined for " + string)
MassPerTimeUnit.g_Per_s = MassPerTimeUnit("g/s")
MassPerTimeUnit.g_Per_min = MassPerTimeUnit("g/min")
MassPerTimeUnit.g_Per_day = MassPerTimeUnit("g/day")
MassPerTimeUnit.mg_Per_s = MassPerTimeUnit("mg/s")
MassPerTimeUnit.mg_Per_min = MassPerTimeUnit("mg/min")
MassPerTimeUnit.ug_Per_s = MassPerTimeUnit("ug/s")
MassPerTimeUnit.kg_Per_s = MassPerTimeUnit("kg/s")
MassPerTimeUnit.ug_Per_min = MassPerTimeUnit("ug/min")
class SEScalarMassPerTime(SEScalarQuantity):

    def __init__(self, value:float=None, units:MassPerTimeUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarMassPerTime, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarMassPerTime")

    def get_value(self, units: MassPerTimeUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: MassPerTimeUnit):
        if isinstance(units, MassPerTimeUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a MassPerTimeUnit")

class MassPerVolumeUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string == MassPerVolumeUnit.g_Per_dL.get_string():
            return MassPerVolumeUnit.g_Per_dL
        if string == MassPerVolumeUnit.g_Per_cm3.get_string():
            return MassPerVolumeUnit.g_Per_cm3
        if string == MassPerVolumeUnit.g_Per_m3.get_string():
            return MassPerVolumeUnit.g_Per_m3
        if string == MassPerVolumeUnit.ug_Per_mL.get_string():
            return MassPerVolumeUnit.ug_Per_mL
        if string == MassPerVolumeUnit.mg_Per_m3.get_string():
            return MassPerVolumeUnit.mg_Per_m3
        if string == MassPerVolumeUnit.kg_Per_m3.get_string():
            return MassPerVolumeUnit.kg_Per_m3
        if string == MassPerVolumeUnit.ug_Per_L.get_string():
            return MassPerVolumeUnit.ug_Per_L
        if string == MassPerVolumeUnit.g_Per_L.get_string():
            return MassPerVolumeUnit.g_Per_L
        if string == MassPerVolumeUnit.g_Per_mL.get_string():
            return MassPerVolumeUnit.g_Per_mL
        if string == MassPerVolumeUnit.mg_Per_mL.get_string():
            return MassPerVolumeUnit.mg_Per_mL
        if string == MassPerVolumeUnit.mg_Per_L.get_string():
            return MassPerVolumeUnit.mg_Per_L
        if string == MassPerVolumeUnit.mg_Per_dL.get_string():
            return MassPerVolumeUnit.mg_Per_dL
        if string == MassPerVolumeUnit.kg_Per_mL.get_string():
            return MassPerVolumeUnit.kg_Per_mL
        if string == MassPerVolumeUnit.kg_Per_L.get_string():
            return MassPerVolumeUnit.kg_Per_L
        raise Exception("No MassPerVolumeUnit defined for " + string)
MassPerVolumeUnit.g_Per_dL = MassPerVolumeUnit("g/dL")
MassPerVolumeUnit.g_Per_cm3 = MassPerVolumeUnit("g/cm^3")
MassPerVolumeUnit.g_Per_m3 = MassPerVolumeUnit("g/m^3")
MassPerVolumeUnit.ug_Per_mL = MassPerVolumeUnit("ug/mL")
MassPerVolumeUnit.mg_Per_m3 = MassPerVolumeUnit("mg/m^3")
MassPerVolumeUnit.kg_Per_m3 = MassPerVolumeUnit("kg/m^3")
MassPerVolumeUnit.ug_Per_L = MassPerVolumeUnit("ug/L")
MassPerVolumeUnit.g_Per_L = MassPerVolumeUnit("g/L")
MassPerVolumeUnit.g_Per_mL = MassPerVolumeUnit("g/mL")
MassPerVolumeUnit.mg_Per_mL = MassPerVolumeUnit("mg/mL")
MassPerVolumeUnit.mg_Per_L = MassPerVolumeUnit("mg/L")
MassPerVolumeUnit.mg_Per_dL = MassPerVolumeUnit("mg/dL")
MassPerVolumeUnit.kg_Per_mL = MassPerVolumeUnit("kg/mL")
MassPerVolumeUnit.kg_Per_L = MassPerVolumeUnit("kg/L")
class SEScalarMassPerVolume(SEScalarQuantity):

    def __init__(self, value:float=None, units:MassPerVolumeUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarMassPerVolume, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarMassPerVolume")

    def get_value(self, units: MassPerVolumeUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: MassPerVolumeUnit):
        if isinstance(units, MassPerVolumeUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a MassPerVolumeUnit")

class PowerUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string == PowerUnit.W.get_string():
            return PowerUnit.W
        if string == PowerUnit.kcal_Per_s.get_string():
            return PowerUnit.kcal_Per_s
        if string == PowerUnit.kcal_Per_hr.get_string():
            return PowerUnit.kcal_Per_hr
        if string == PowerUnit.kcal_Per_day.get_string():
            return PowerUnit.kcal_Per_day
        if string == PowerUnit.J_Per_s.get_string():
            return PowerUnit.J_Per_s
        if string == PowerUnit.BTU_Per_hr.get_string():
            return PowerUnit.BTU_Per_hr
        raise Exception("No PowerUnit defined for " + string)
PowerUnit.W = PowerUnit("W")
PowerUnit.kcal_Per_s = PowerUnit("kcal/s")
PowerUnit.kcal_Per_hr = PowerUnit("kcal/hr")
PowerUnit.kcal_Per_day = PowerUnit("kcal/day")
PowerUnit.J_Per_s = PowerUnit("J/s")
PowerUnit.BTU_Per_hr = PowerUnit("BTU/hr")
class SEScalarPower(SEScalarQuantity):

    def __init__(self, value:float=None, units:PowerUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarPower, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarPower")

    def get_value(self, units: PowerUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: PowerUnit):
        if isinstance(units, PowerUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a PowerUnit")

class PressureUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string == PressureUnit.Pa.get_string():
            return PressureUnit.Pa
        if string == PressureUnit.mmHg.get_string():
            return PressureUnit.mmHg
        if string == PressureUnit.cmH2O.get_string():
            return PressureUnit.cmH2O
        if string == PressureUnit.psi.get_string():
            return PressureUnit.psi
        if string == PressureUnit.atm.get_string():
            return PressureUnit.atm
        raise Exception("No PressureUnit defined for " + string)
PressureUnit.Pa = PressureUnit("Pa")
PressureUnit.mmHg = PressureUnit("mmHg")
PressureUnit.cmH2O = PressureUnit("cmH2O")
PressureUnit.psi = PressureUnit("psi")
PressureUnit.atm = PressureUnit("atm")
class SEScalarPressure(SEScalarQuantity):

    def __init__(self, value:float=None, units:PressureUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarPressure, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarPressure")

    def get_value(self, units: PressureUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: PressureUnit):
        if isinstance(units, PressureUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a PressureUnit")

class TemperatureUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string == TemperatureUnit.F.get_string():
            return TemperatureUnit.F
        if string == TemperatureUnit.C.get_string():
            return TemperatureUnit.C
        if string == TemperatureUnit.K.get_string():
            return TemperatureUnit.K
        if string == TemperatureUnit.R.get_string():
            return TemperatureUnit.R
        raise Exception("No TemperatureUnit defined for " + string)
TemperatureUnit.F = TemperatureUnit("degF")
TemperatureUnit.C = TemperatureUnit("degC")
TemperatureUnit.K = TemperatureUnit("K")
TemperatureUnit.R = TemperatureUnit("degR")
class SEScalarTemperature(SEScalarQuantity):

    def __init__(self, value:float=None, units:TemperatureUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarTemperature, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarTemperature")

    def get_value(self, units: TemperatureUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: TemperatureUnit):
        if isinstance(units, TemperatureUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a TemperatureUnit")

class TimeUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string == TimeUnit.s.get_string():
            return TimeUnit.s
        if string == TimeUnit.min.get_string():
            return TimeUnit.min
        if string == TimeUnit.hr.get_string():
            return TimeUnit.hr
        if string == TimeUnit.day.get_string():
            return TimeUnit.day
        if string == TimeUnit.yr.get_string():
            return TimeUnit.yr
        raise Exception("No TimeUnit defined for " + string)
TimeUnit.s = TimeUnit("s")
TimeUnit.min = TimeUnit("min")
TimeUnit.hr = TimeUnit("hr")
TimeUnit.day = TimeUnit("day")
TimeUnit.yr = TimeUnit("yr")
class SEScalarTime(SEScalarQuantity):

    def __init__(self, value:float=None, units:TimeUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarTime, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarTime")

    def get_value(self, units: TimeUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: TimeUnit):
        if isinstance(units, TimeUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a TimeUnit")

class VolumeUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string == VolumeUnit.L.get_string():
            return VolumeUnit.L
        if string == VolumeUnit.mL.get_string():
            return VolumeUnit.mL
        if string == VolumeUnit.dL.get_string():
            return VolumeUnit.dL
        if string == VolumeUnit.m3.get_string():
            return VolumeUnit.m3
        raise Exception("No VolumeUnit defined for " + string)
VolumeUnit.L = VolumeUnit("L")
VolumeUnit.mL = VolumeUnit("mL")
VolumeUnit.dL = VolumeUnit("dL")
VolumeUnit.m3 = VolumeUnit("m^3")
class SEScalarVolume(SEScalarQuantity):

    def __init__(self, value:float=None, units:VolumeUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarVolume, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarVolume")

    def get_value(self, units: VolumeUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: VolumeUnit):
        if isinstance(units, VolumeUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a VolumeUnit")

class VolumePerTimeUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string is VolumePerTimeUnit.L_Per_s.get_string():
            return VolumePerTimeUnit.L_Per_s
        if string is VolumePerTimeUnit.L_Per_day.get_string():
            return VolumePerTimeUnit.L_Per_day
        if string is VolumePerTimeUnit.mL_Per_s.get_string():
            return VolumePerTimeUnit.mL_Per_s
        if string is VolumePerTimeUnit.mL_Per_day.get_string():
            return VolumePerTimeUnit.mL_Per_day
        if string is VolumePerTimeUnit.L_Per_min.get_string():
            return VolumePerTimeUnit.L_Per_min
        if string is VolumePerTimeUnit.m3_Per_s.get_string():
            return VolumePerTimeUnit.m3_Per_s
        if string is VolumePerTimeUnit.mL_Per_min.get_string():
            return VolumePerTimeUnit.mL_Per_min
        if string is VolumePerTimeUnit.mL_Per_hr.get_string():
            return VolumePerTimeUnit.mL_Per_hr
        raise Exception("No VolumePerTimeUnit defined for "+string)
VolumePerTimeUnit.L_Per_s = VolumePerTimeUnit("L/s")
VolumePerTimeUnit.L_Per_day = VolumePerTimeUnit("L/day")
VolumePerTimeUnit.mL_Per_s = VolumePerTimeUnit("mL/s")
VolumePerTimeUnit.mL_Per_day = VolumePerTimeUnit("mL/day")
VolumePerTimeUnit.L_Per_min = VolumePerTimeUnit("L/min")
VolumePerTimeUnit.m3_Per_s = VolumePerTimeUnit("m^3/s")
VolumePerTimeUnit.mL_Per_min = VolumePerTimeUnit("mL/min")
VolumePerTimeUnit.mL_Per_hr = VolumePerTimeUnit("mL/hr")
class SEScalarVolumePerTime(SEScalarQuantity):

    def __init__(self, value:float=None, units:VolumePerTimeUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarVolumePerTime, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarVolumePerTime")

    def get_value(self, units: VolumePerTimeUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: VolumePerTimeUnit):
        if isinstance(units, VolumePerTimeUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a VolumePerTimeUnit")

class VolumePerTimeMassUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string == VolumePerTimeMassUnit.L_Per_s_g.get_string():
            return VolumePerTimeMassUnit.L_Per_s_g
        if string == VolumePerTimeMassUnit.mL_Per_s_g.get_string():
            return VolumePerTimeMassUnit.mL_Per_s_g
        if string == VolumePerTimeMassUnit.mL_Per_min_kg.get_string():
            return VolumePerTimeMassUnit.mL_Per_min_kg
        if string == VolumePerTimeMassUnit.mL_Per_s_kg.get_string():
            return VolumePerTimeMassUnit.mL_Per_s_kg
        if string == VolumePerTimeMassUnit.uL_Per_min_kg.get_string():
            return VolumePerTimeMassUnit.uL_Per_min_kg
        raise Exception("No VolumePerTimeMassUnit defined for " + string)
VolumePerTimeMassUnit.L_Per_s_g = VolumePerTimeMassUnit("L/s g")
VolumePerTimeMassUnit.mL_Per_s_g = VolumePerTimeMassUnit("mL/s g")
VolumePerTimeMassUnit.mL_Per_min_kg = VolumePerTimeMassUnit("mL/min kg")
VolumePerTimeMassUnit.mL_Per_s_kg = VolumePerTimeMassUnit("mL/s kg")
VolumePerTimeMassUnit.uL_Per_min_kg = VolumePerTimeMassUnit("uL/min kg")
class SEScalarVolumePerTimeMass(SEScalarQuantity):

    def __init__(self, value:float=None, units:VolumePerTimeMassUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarVolumePerTimeMass, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarVolumePerTimeMass")

    def get_value(self, units: VolumePerTimeMassUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: VolumePerTimeMassUnit):
        if isinstance(units, VolumePerTimeMassUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a VolumePerTimeMassUnit")

class VolumePerTimePressureUnit(SEScalarUnit):
    @staticmethod
    def from_string(string: str):
        if string == VolumePerTimePressureUnit.L_Per_s_mmHg.get_string():
            return VolumePerTimePressureUnit.L_Per_s_mmHg
        if string == VolumePerTimePressureUnit.mL_Per_s_mmHg.get_string():
            return VolumePerTimePressureUnit.mL_Per_s_mmHg
        if string == VolumePerTimePressureUnit.L_Per_min_mmHg.get_string():
            return VolumePerTimePressureUnit.L_Per_min_mmHg
        if string == VolumePerTimePressureUnit.mL_Per_min_mmHg.get_string():
            return VolumePerTimePressureUnit.mL_Per_min_mmHg
        raise Exception("No VolumePerTimePressureUnit defined for " + string)
VolumePerTimePressureUnit.L_Per_s_mmHg = VolumePerTimePressureUnit("L/s mmHg")
VolumePerTimePressureUnit.mL_Per_s_mmHg = VolumePerTimePressureUnit("mL/s mmHg")
VolumePerTimePressureUnit.L_Per_min_mmHg = VolumePerTimePressureUnit("L/min mmHg")
VolumePerTimePressureUnit.mL_Per_min_mmHg = VolumePerTimePressureUnit("mL/min mmHg")
class SEScalarVolumePerTimePressure(SEScalarQuantity):

    def __init__(self, value:float=None, units:VolumePerTimePressureUnit=None):
        if value is None or units is None:
            self.invalidate()
        else:
            self.set_value(value,units)

    def set(self, scalar):
        if isinstance(SEScalarVolumePerTimePressure, scalar):
            self._value = scalar._value
            self._units = scalar._units
        else:
            raise Exception("Provided argument must be a SEScalarVolumePerTimePressure")

    def get_value(self, units: VolumePerTimePressureUnit=None):
        if self.is_valid() is False:
            return None
        if units is None:
            return self._value
        if self._units is units:
            return self._value
        else:
            raise Exception("Unit converter not connected")

    def set_value(self, value: float, units: VolumePerTimePressureUnit):
        if isinstance(units, VolumePerTimePressureUnit):
            self._value = value
            self._units = units
        else:
            raise Exception("Provided argument must be a VolumePerTimePressureUnit")