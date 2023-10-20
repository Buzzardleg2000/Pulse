/* Distributed under the Apache License, Version 2.0.SEInitialEnvironmentalConditions
   See accompanying NOTICE file for details.*/
using System.Collections.Generic;
namespace Pulse.CDM
{
  public class PBEnvironmentCondition
  {

    #region Any Environment Condition
    /** Create a new Condition based on the binding object, load that data into the new Condition, and return said Condition */
    public static SEEnvironmentCondition Load(pulse.cdm.bind.AnyEnvironmentConditionData any/*, SESubstanceManager subMgr*/)
    {
      if (any.InitialEnvironmentalConditions != null)
      {
        SEInitialEnvironmentalConditions iec = new SEInitialEnvironmentalConditions();
        Serialize(any.InitialEnvironmentalConditions, iec);
        return iec;
      }

      return null;
    }

    public static pulse.cdm.bind.AnyEnvironmentConditionData Unload(SEEnvironmentCondition Condition)
    {
      pulse.cdm.bind.AnyEnvironmentConditionData any = new pulse.cdm.bind.AnyEnvironmentConditionData();

      if (Condition.GetType().IsAssignableFrom(typeof(SEInitialEnvironmentalConditions)))
      {
        any.InitialEnvironmentalConditions = Unload((SEInitialEnvironmentalConditions)Condition);
        return any;
      }

      return any;
    }
      #endregion

      #region SEEnvironmentCondition

      public static void Serialize(pulse.cdm.bind.EnvironmentConditionData src, SEEnvironmentCondition dst)
    {
      if (src.Condition != null)
        PBCondition.Serialize(src.Condition, dst);
    }
    public static void Serialize(SEEnvironmentCondition src, pulse.cdm.bind.EnvironmentConditionData dst)
    {
      PBCondition.Serialize(src, dst.Condition);
    }

    #endregion

    #region SEInitialEnvironmentalConditions

    public static void Load(pulse.cdm.bind.InitialEnvironmentalConditionsData src, SEInitialEnvironmentalConditions dst)
    {
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.InitialEnvironmentalConditionsData src, SEInitialEnvironmentalConditions dst)
    {
      if (src.EnvironmentCondition != null)
        PBEnvironmentCondition.Serialize(src.EnvironmentCondition, dst);
      if (!string.IsNullOrEmpty(src.EnvironmentalConditionsFile))
        dst.SetEnvironmentalConditionsFile(src.EnvironmentalConditionsFile);
      else if (src.EnvironmentalConditions != null)
        PBEnvironment.Load(src.EnvironmentalConditions, dst.GetEnvironmentalConditions());
    }

    public static pulse.cdm.bind.InitialEnvironmentalConditionsData Unload(SEInitialEnvironmentalConditions src)
    {
      pulse.cdm.bind.InitialEnvironmentalConditionsData dst = new pulse.cdm.bind.InitialEnvironmentalConditionsData();
      Serialize(src, dst);
      return dst;
    }
    static void Serialize(SEInitialEnvironmentalConditions src, pulse.cdm.bind.InitialEnvironmentalConditionsData dst)
    {
      dst.EnvironmentCondition = new pulse.cdm.bind.EnvironmentConditionData();
      Serialize(src, dst.EnvironmentCondition);
      if (src.HasEnvironmentalConditionsFile())
        dst.EnvironmentalConditionsFile = src.GetEnvironmentalConditionsFile();
      else if (src.HasEnvironmentalConditions())
        dst.EnvironmentalConditions = PBEnvironment.Unload(src.GetEnvironmentalConditions());
    }

    #endregion

  }
}
