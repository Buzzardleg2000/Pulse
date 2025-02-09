/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class MassPerAmountUnit : Unit
  {
    private MassPerAmountUnit(string v) : base(v) { }

    public static MassPerAmountUnit g_Per_ct = new MassPerAmountUnit("g/ct");
    public static MassPerAmountUnit g_Per_mol = new MassPerAmountUnit("g/mol");
    public static MassPerAmountUnit g_Per_mmol = new MassPerAmountUnit("g/mmol");
    public static MassPerAmountUnit g_Per_umol = new MassPerAmountUnit("g/umol");
    public static MassPerAmountUnit mg_Per_mol = new MassPerAmountUnit("mg/mol");
    public static MassPerAmountUnit mg_Per_mmol = new MassPerAmountUnit("mg/mmol");
    public static MassPerAmountUnit kg_Per_mol = new MassPerAmountUnit("kg/mol");
    public static MassPerAmountUnit ug_Per_mmol = new MassPerAmountUnit("ug/mmol");
    public static MassPerAmountUnit ug_Per_mol = new MassPerAmountUnit("ug/mol");
    public static MassPerAmountUnit ug_Per_ct = new MassPerAmountUnit("ug/ct");
    public static MassPerAmountUnit pg_Per_ct = new MassPerAmountUnit("pg/ct");

    public static MassPerAmountUnit FromString(string u)
    {
      if (u == MassPerAmountUnit.g_Per_ct.ToString())
        return MassPerAmountUnit.g_Per_ct;
      if (u == MassPerAmountUnit.g_Per_mol.ToString())
        return MassPerAmountUnit.g_Per_mol;
      if (u == MassPerAmountUnit.g_Per_mmol.ToString())
        return MassPerAmountUnit.g_Per_mmol;
      if (u == MassPerAmountUnit.g_Per_umol.ToString())
        return MassPerAmountUnit.g_Per_umol;
      if (u == MassPerAmountUnit.mg_Per_mol.ToString())
        return MassPerAmountUnit.mg_Per_mol;
      if (u == MassPerAmountUnit.mg_Per_mmol.ToString())
        return MassPerAmountUnit.mg_Per_mmol;
      if (u == MassPerAmountUnit.kg_Per_mol.ToString())
        return MassPerAmountUnit.kg_Per_mol;
      if (u == MassPerAmountUnit.ug_Per_mmol.ToString())
        return MassPerAmountUnit.ug_Per_mmol;
      if (u == MassPerAmountUnit.ug_Per_mol.ToString())
        return MassPerAmountUnit.ug_Per_mol;
      if (u == MassPerAmountUnit.ug_Per_ct.ToString())
        return MassPerAmountUnit.ug_Per_ct;
      if (u == MassPerAmountUnit.pg_Per_ct.ToString())
        return MassPerAmountUnit.pg_Per_ct;

      throw new System.ArgumentException(u + " is not a know mass per amount unit");
    }
  }

  public class SEScalarMassPerAmount : SEScalar
  {
    protected MassPerAmountUnit unit;

    public SEScalarMassPerAmount() : base()
    {

    }

    public SEScalarMassPerAmount(SEScalarMassPerAmount from)
    {
      Set(from);
    }

    public SEScalarMassPerAmount(double value, MassPerAmountUnit unit)
    {
      SetValue(value, unit);
    }

    public new void SetValue(double value)
    {
      throw new System.NotSupportedException("You must provide a unit");
    }

    public void Set(SEScalarMassPerAmount from)
    {
      if (from == null)
      {
        Invalidate();
        return;
      }
      SetValue(from.value, from.unit);
    }

    public void SetValue(double value, MassPerAmountUnit unit)
    {
      this.value = value;
      this.unit = unit;
    }

    public double GetValue(MassPerAmountUnit unit)
    {
      return UnitConverter.Convert(this.value, this.unit, unit);
    }

    public MassPerAmountUnit GetUnit()
    {
      return unit;
    }

    public new string ToString()
    {
      return this.value + "(" + this.unit.ToString() + ")";
    }
  }
}
