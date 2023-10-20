/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
using System.Collections.Generic;
namespace Pulse.CDM
{
  public class PBEnvironmentAction
  {

    #region Any Environment Action

    public static SEEnvironmentAction Load(pulse.cdm.bind.AnyEnvironmentActionData any)
    {
      if (any.ChangeEnvironmentalConditions != null)
      {
        SEChangeEnvironmentalConditions dst = new SEChangeEnvironmentalConditions();
        PBEnvironmentAction.Load(any.ChangeEnvironmentalConditions, dst);
        return dst;
      }
      if (any.ThermalApplication != null)
      {
        SEThermalApplication dst = new SEThermalApplication();
        PBEnvironmentAction.Load(any.ThermalApplication, dst);
        return dst;
      }
      return null;
    }

    public static pulse.cdm.bind.AnyEnvironmentActionData Unload(SEEnvironmentAction action)
    {
      pulse.cdm.bind.AnyEnvironmentActionData any = new pulse.cdm.bind.AnyEnvironmentActionData();
      if (action.GetType().IsAssignableFrom(typeof(SEChangeEnvironmentalConditions)))
      {
        any.ChangeEnvironmentalConditions = Unload((SEChangeEnvironmentalConditions)action);
        return any;
      }

      if (action.GetType().IsAssignableFrom(typeof(SEThermalApplication)))
      {
        any.ThermalApplication = Unload((SEThermalApplication)action);
        return any;
      }
      return null;
    }


    #endregion

    #region SEEnvironmentAction

    public static void Serialize(pulse.cdm.bind.EnvironmentActionData src, SEEnvironmentAction dst)
    {
      if (src.Action != null)
        PBAction.Serialize(src.Action, dst);
    }
    public static void Serialize(SEEnvironmentAction src, pulse.cdm.bind.EnvironmentActionData dst)
    {
      dst.Action = new pulse.cdm.bind.ActionData();
      PBAction.Serialize(src, dst.Action);
    }

    #endregion

    #region SEChangeEnvironmentalConditions

    public static void Load(pulse.cdm.bind.ChangeEnvironmentalConditionsData src, SEChangeEnvironmentalConditions dst)
    {
      dst.Clear();
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ChangeEnvironmentalConditionsData src, SEChangeEnvironmentalConditions dst)
    {
      if (src.EnvironmentAction != null)
        Serialize(src.EnvironmentAction, dst);
      if (!string.IsNullOrEmpty(src.EnvironmentalConditionsFile))
        dst.SetEnvironmentalConditionsFile(src.EnvironmentalConditionsFile);
      else if (src.EnvironmentalConditions != null)
        PBEnvironment.Load(src.EnvironmentalConditions, dst.GetEnvironmentalConditions());
    }

    public static pulse.cdm.bind.ChangeEnvironmentalConditionsData Unload(SEChangeEnvironmentalConditions src)
    {
      pulse.cdm.bind.ChangeEnvironmentalConditionsData dst = new pulse.cdm.bind.ChangeEnvironmentalConditionsData();
      Serialize(src, dst);
      return dst;
    }
    static void Serialize(SEChangeEnvironmentalConditions src, pulse.cdm.bind.ChangeEnvironmentalConditionsData dst)
    {
      dst.EnvironmentAction = new pulse.cdm.bind.EnvironmentActionData();
      Serialize(src, dst.EnvironmentAction);
      if (src.HasEnvironmentalConditionsFile())
        dst.EnvironmentalConditionsFile = src.GetEnvironmentalConditionsFile();
      else if (src.HasEnvironmentalConditions())
        dst.EnvironmentalConditions = PBEnvironment.Unload(src.GetEnvironmentalConditions());
    }

    #endregion

    #region SEThermalApplication

    public static void Load(pulse.cdm.bind.ThermalApplicationData src, SEThermalApplication dst)
    {
      dst.Clear();
      Serialize(src, dst);
    }
    public static void Serialize(pulse.cdm.bind.ThermalApplicationData src, SEThermalApplication dst)
    {
      if (src.EnvironmentAction != null)
        Serialize(src.EnvironmentAction, dst);
      if (src.ActiveHeating != null)
        PBEnvironment.Load(src.ActiveHeating, dst.GetActiveHeating());
      if (src.ActiveCooling != null)
        PBEnvironment.Load(src.ActiveCooling, dst.GetActiveCooling());
      if (src.AppliedTemperature != null)
        PBEnvironment.Load(src.AppliedTemperature, dst.GetAppliedTemperature());
    }

    public static pulse.cdm.bind.ThermalApplicationData Unload(SEThermalApplication src)
    {
      pulse.cdm.bind.ThermalApplicationData dst = new pulse.cdm.bind.ThermalApplicationData();
      Serialize(src, dst);
      return dst;
    }
    public static void Serialize(SEThermalApplication src, pulse.cdm.bind.ThermalApplicationData dst)
    {

      dst.EnvironmentAction = new pulse.cdm.bind.EnvironmentActionData();
      Serialize(src, dst.EnvironmentAction);
      dst.AppendToPrevious = !src.GetClearContents();

      if (src.HasActiveHeating())
        dst.ActiveHeating = PBEnvironment.Unload(src.GetActiveHeating());
      if (src.HasActiveCooling())
        dst.ActiveCooling = PBEnvironment.Unload(src.GetActiveCooling());
      if (src.HasAppliedTemperature())
        dst.AppliedTemperature = PBEnvironment.Unload(src.GetAppliedTemperature());
    }

    #endregion
  }
}
