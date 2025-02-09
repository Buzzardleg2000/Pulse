/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class AreaUnit : Unit
  {
    private AreaUnit(string v) : base(v) { }

    public static AreaUnit cm2 = new AreaUnit("cm^2");
    public static AreaUnit m2 = new AreaUnit("m^2");

    public static AreaUnit FromString(string u)
    {
      if (u == AreaUnit.m2.ToString())
        return AreaUnit.m2;
      if (u == AreaUnit.cm2.ToString())
        return AreaUnit.cm2;

      throw new System.ArgumentException(u + " is not a know area unit");
    }
  }

  public class SEScalarArea : SEScalar
  {
    protected AreaUnit unit;

    public SEScalarArea() : base()
    {

    }

    public SEScalarArea(SEScalarArea from)
    {
      Set(from);
    }

    public SEScalarArea(double value, AreaUnit unit)
    {
      SetValue(value, unit);
    }

    public void Set(SEScalarArea from)
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

    public void SetValue(double value, AreaUnit unit)
    {
      this.value = value;
      this.unit = unit;
    }

    public double GetValue(AreaUnit unit)
    {
      return UnitConverter.Convert(this.value, this.unit, unit);
    }

    public AreaUnit GetUnit()
    {
      return unit;
    }

    public new string ToString()
    {
      return this.value + "(" + this.unit.ToString() + ")";
    }
  }
}
