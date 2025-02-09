/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class InversePressureUnit : Unit
  {
    private InversePressureUnit(string v) : base(v) { }

    public static InversePressureUnit Inverse_Pa = new InversePressureUnit("1/Pa");
    public static InversePressureUnit Inverse_mmHg = new InversePressureUnit("1/mmHg");
    public static InversePressureUnit Inverse_cmH2O = new InversePressureUnit("1/cmH2O");
    public static InversePressureUnit Inverse_atm = new InversePressureUnit("1/atm");

    public static InversePressureUnit FromString(string u)
    {
      if (u == InversePressureUnit.Inverse_Pa.ToString())
        return InversePressureUnit.Inverse_Pa;
      if (u == InversePressureUnit.Inverse_mmHg.ToString())
        return InversePressureUnit.Inverse_mmHg;
      if (u == InversePressureUnit.Inverse_cmH2O.ToString())
        return InversePressureUnit.Inverse_cmH2O;
      if (u == InversePressureUnit.Inverse_atm.ToString())
        return InversePressureUnit.Inverse_atm;

      throw new System.ArgumentException(u + " is not a know inverse pressure unit");
    }
  }

  public class SEScalarInversePressure : SEScalar
  {
    protected InversePressureUnit unit;

    public SEScalarInversePressure() : base()
    {

    }

    public SEScalarInversePressure(SEScalarInversePressure from)
    {
      Set(from);
    }

    public SEScalarInversePressure(double value, InversePressureUnit unit)
    {
      SetValue(value, unit);
    }

    public void Set(SEScalarInversePressure from)
    {
      if (from == null)
      {
        Invalidate();
        return;
      }
      SetValue(from.value, from.unit);
    }

    public new void SetValue(double value)
    {
      throw new System.NotSupportedException("You must provide a unit");
    }

    public void SetValue(double value, InversePressureUnit unit)
    {
      this.value = value;
      this.unit = unit;
    }

    public double GetValue(InversePressureUnit unit)
    {
      return UnitConverter.Convert(this.value, this.unit, unit);
    }

    public InversePressureUnit GetUnit()
    {
      return unit;
    }

    public new string ToString()
    {
      return this.value + "(" + this.unit.ToString() + ")";
    }
  }
}
