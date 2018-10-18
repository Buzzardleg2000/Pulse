/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.physiology.datamodel.system.equipment.electrocardiogram;

import com.kitware.physiology.cdm.ElectroCardioGram.ElectroCardioGramWaveformData;
import com.kitware.physiology.cdm.ElectroCardioGramEnums.eElectroCardioGram;
import com.kitware.physiology.cdm.PhysiologyEnums.eHeartRhythm;

import com.kitware.physiology.datamodel.properties.SEFunctionElectricPotentialVsTime;
import com.kitware.physiology.datamodel.properties.SEScalarTime;
import com.kitware.physiology.utilities.Log;

public class SEElectroCardioGramWaveform
{
  protected eElectroCardioGram.WaveformLead   lead;
  protected eHeartRhythm                      rhythm;
  protected SEFunctionElectricPotentialVsTime data;
  protected SEScalarTime                      timeStep;
  
  public SEElectroCardioGramWaveform()
  {
    reset();
  }
  
  public void reset()
  {
    lead = eElectroCardioGram.WaveformLead.NullLead;
    rhythm = null;
    data = null;
    timeStep = null;
  }
  
  public static void load(ElectroCardioGramWaveformData src, SEElectroCardioGramWaveform dst)
  {
    dst.reset();
    if(src.getRhythm()!=eHeartRhythm.UNRECOGNIZED)
      dst.setRhythm(src.getRhythm());
    if(src.getLead()!=eElectroCardioGram.WaveformLead.UNRECOGNIZED)
     dst.setLead(src.getLead());
    if(src.hasData())
      SEFunctionElectricPotentialVsTime.load(src.getData(),dst.getData());
    if(src.hasTimeStep())
      SEScalarTime.load(src.getTimeStep(),dst.getTimeStep()); 
  }
  public static ElectroCardioGramWaveformData unload(SEElectroCardioGramWaveform src)
  {
    ElectroCardioGramWaveformData.Builder dst =  ElectroCardioGramWaveformData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEElectroCardioGramWaveform src, ElectroCardioGramWaveformData.Builder dst)
  {
    if(src.hasLead())
      dst.setLead(src.lead);
    if(src.hasRhythm())
      dst.setRhythm(src.rhythm);
    if(src.hasData())
      dst.setData(SEFunctionElectricPotentialVsTime.unload(src.data));
    if(src.hasTimeStep())
      dst.setTimeStep(SEScalarTime.unload(src.timeStep));
  }
  
  public eElectroCardioGram.WaveformLead getLead()
  {
    return lead;
  }
  public void setLead(eElectroCardioGram.WaveformLead l)
  {
    this.lead = l;
  }
  public boolean hasLead()
  {
    return lead==null ? false : lead != eElectroCardioGram.WaveformLead.NullLead;
  }
  public void removeLead()
  {
    this.lead = null;
  }
  
  public eHeartRhythm getRhythm()
  {
    return rhythm;
  }
  public void setRhythm(eHeartRhythm r)
  {
    this.rhythm = r;
  }
  public boolean hasRhythm()
  {
    return rhythm == null ? false : true;
  }
  
  public boolean hasData()
  {
    return data == null ? false : data.isValid();
  }
  public SEFunctionElectricPotentialVsTime getData()
  {
    if (data == null)
      data = new SEFunctionElectricPotentialVsTime();
    return data;
  }

  public boolean hasTimeStep()
  {
    return timeStep == null ? false : timeStep.isValid();
  }
  public SEScalarTime getTimeStep()
  {
    if (timeStep == null)
      timeStep = new SEScalarTime();
    return timeStep;
  }
}
