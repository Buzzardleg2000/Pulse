/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.physiology.datamodel.patient.actions;

import java.util.ArrayList;
import java.util.List;

import com.kitware.physiology.cdm.PatientActions.ConsciousRespirationData;
import com.kitware.physiology.cdm.PatientActions.ConsciousRespirationData.AnyCommandData;

import com.kitware.physiology.datamodel.substance.SESubstanceFraction;

public class SEConsciousRespiration extends SEPatientAction
{
  protected List<SEConsciousRespirationCommand> commands;
  protected Boolean                             appendToPrevious;
  
  public SEConsciousRespiration()
  {
    appendToPrevious = null;
    commands = new ArrayList<SEConsciousRespirationCommand>();
  }
  
  public void copy(SEConsciousRespiration other)
  {
    if (this == other)
      return;
    super.copy(other);
    appendToPrevious = other.appendToPrevious;
    if (other.commands != null)
      {
      commands.clear();
        for (SEConsciousRespirationCommand command : other.commands)
        {
            commands.add(command.getCopy());
        }
      }
  }  
  
  public void reset()
  {
    super.reset();
    commands.clear();
    appendToPrevious = null;
  }
  
  public boolean isValid()
  {
    if(!hasCommands())
      return false;
    for(SEConsciousRespirationCommand c : this.commands)
      if(!c.isValid())
        return false;
    return true;
  }
  
  public static void load(ConsciousRespirationData src, SEConsciousRespiration dst)
  {
    SEPatientAction.load(src.getPatientAction(), dst);
    dst.appendToPrevious = src.getAppendToPrevious();
    dst.commands.clear();
    for (AnyCommandData cmd : src.getCommandList())
    {
      switch(cmd.getCommandCase())
      {
        case BREATHHOLD:
        {
          SEBreathHold command = new SEBreathHold();
          SEBreathHold.load(cmd.getBreathHold(),command);
          dst.commands.add(command);
          break;
        }
        case FORCEDEXHALE:
        {
          SEForcedExhale command = new SEForcedExhale();
          SEForcedExhale.load(cmd.getForcedExhale(),command);
          dst.commands.add(command);
          break;
        }
        case FORCEDINHALE:
        {
          SEForcedInhale command = new SEForcedInhale();
          SEForcedInhale.load(cmd.getForcedInhale(),command);
          dst.commands.add(command);
          break;
        }
        case USEINHALER:
        {
          SEUseInhaler command = new SEUseInhaler();
          SEUseInhaler.load(cmd.getUseInhaler(),command);
          dst.commands.add(command);
          break;
        }
      }    
    }
  }
  
  public static ConsciousRespirationData unload(SEConsciousRespiration src)
  {
    ConsciousRespirationData.Builder dst = ConsciousRespirationData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SEConsciousRespiration src, ConsciousRespirationData.Builder dst)
  {
    SEPatientAction.unload(src,dst.getPatientActionBuilder());
    if(src.appendToPrevious!=null)
      dst.setAppendToPrevious(src.appendToPrevious);
    for (SEConsciousRespirationCommand command : src.commands)
    {
      AnyCommandData.Builder cmd = dst.addCommandBuilder();
      if (command instanceof SEBreathHold)
      {
        cmd.setBreathHold(SEBreathHold.unload((SEBreathHold)command));
        continue;
      }
      if (command instanceof SEForcedExhale)
      {
        cmd.setForcedExhale(SEForcedExhale.unload((SEForcedExhale)command));
        continue;
      }
      if (command instanceof SEForcedInhale)
      {
        cmd.setForcedInhale(SEForcedInhale.unload((SEForcedInhale)command));
        continue;
      }
      if (command instanceof SEUseInhaler)
      {
        cmd.setUseInhaler(SEUseInhaler.unload((SEUseInhaler)command));
        continue;
      }
    }
  }
    
  public boolean hasAppendToPrevious() { return this.appendToPrevious!=null;}
  public boolean appendToPrevious() { return appendToPrevious; }
  
  
  public boolean hasCommands()
  {
    return commands != null && !commands.isEmpty();
  }
  public List<SEConsciousRespirationCommand> getCommands()
  {
    return commands;
  }
  
  public String toString()
  {
    String str = "ConsciousRespiration";    
    for (SEConsciousRespirationCommand command : commands)
    {
      str += "\n\t"+command.toString().replaceAll("\n\t", "\n\t\t");
    }
    
    return str;
  }
}
