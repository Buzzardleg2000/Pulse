/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.engine;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import com.kitware.pulse.cdm.bind.Engine.DynamicStabilizationEngineConvergenceData;
import com.kitware.pulse.cdm.bind.Engine.DynamicStabilizationPropertyConvergenceData;
import com.kitware.pulse.cdm.properties.SEScalarTime;

public class SEDynamicStabilizationEngineConvergence
{
  public class PropertyConvergence
  {
    public SEDataRequest dataRequest;
    public double        percentDifference;
  }
  
  protected SEScalarTime convergenceTime;
  protected SEScalarTime minimumReactionTime;
  protected SEScalarTime maximumAllowedStabilizationTime;
  protected List<PropertyConvergence> properties=new ArrayList<>();
  
  public void clear()
  {
    this.convergenceTime=null;
    this.minimumReactionTime=null;
    this.maximumAllowedStabilizationTime=null;
    this.properties.clear();
  }

  public static void load(DynamicStabilizationEngineConvergenceData src, SEDynamicStabilizationEngineConvergence dst)
  {
    if(src.hasConvergenceTime())
      SEScalarTime.load(src.getConvergenceTime(), dst.getConvergenceTime());
    if(src.hasMinimumReactionTime())
      SEScalarTime.load(src.getMinimumReactionTime(),dst.getMinimumReactionTime());
    if(src.hasMaximumAllowedStabilizationTime())
      SEScalarTime.load(src.getMaximumAllowedStabilizationTime(),dst.getMaximumAllowedStabilizationTime());      
    for(DynamicStabilizationPropertyConvergenceData pcData : src.getPropertyConvergenceList())
    {
    	SEDataRequest dr = new SEDataRequest(pcData.getDataRequest().getCategory());
    	SEDataRequest.load(pcData.getDataRequest(), dr);
      dst.createProperty(pcData.getPercentDifference(),dr);
    }
  }
  public static DynamicStabilizationEngineConvergenceData unload(SEDynamicStabilizationEngineConvergence src)
  {
    DynamicStabilizationEngineConvergenceData.Builder dst = DynamicStabilizationEngineConvergenceData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  public static void unload(SEDynamicStabilizationEngineConvergence src, DynamicStabilizationEngineConvergenceData.Builder dst)
  {
    if(src.hasConvergenceTime())
      dst.setConvergenceTime(SEScalarTime.unload(src.convergenceTime));
    if(src.hasMinimumReactionTime())
      dst.setMinimumReactionTime(SEScalarTime.unload(src.minimumReactionTime));
    if(src.hasMaximumAllowedStabilizationTime())
      dst.setMaximumAllowedStabilizationTime(SEScalarTime.unload(src.maximumAllowedStabilizationTime));
    for(PropertyConvergence pc : src.properties)
    {
      DynamicStabilizationPropertyConvergenceData.Builder pcData = dst.addPropertyConvergenceBuilder();
      pcData.setDataRequest(SEDataRequest.unload(pc.dataRequest));
      pcData.setPercentDifference(pc.percentDifference);
    }
  }
  
  public boolean hasConvergenceTime()
  {
    return convergenceTime == null ? false : convergenceTime.isValid();
  }
  public SEScalarTime getConvergenceTime()
  {
    if (convergenceTime == null)
      convergenceTime = new SEScalarTime();
    return convergenceTime;
  }
  
  public boolean hasMinimumReactionTime()
  {
    return minimumReactionTime == null ? false : minimumReactionTime.isValid();
  }
  public SEScalarTime getMinimumReactionTime()
  {
    if (minimumReactionTime == null)
      minimumReactionTime = new SEScalarTime();
    return minimumReactionTime;
  }
  
  public boolean hasMaximumAllowedStabilizationTime()
  {
    return maximumAllowedStabilizationTime == null ? false : maximumAllowedStabilizationTime.isValid();
  }
  public SEScalarTime getMaximumAllowedStabilizationTime()
  {
    if (maximumAllowedStabilizationTime == null)
      maximumAllowedStabilizationTime = new SEScalarTime();
    return maximumAllowedStabilizationTime;
  }
  
  public void createProperty(double percentDifference, SEDataRequest dr)
  {
    PropertyConvergence pc = new PropertyConvergence();
    pc.dataRequest=dr;
    pc.percentDifference=percentDifference;
    this.properties.add(pc);
  }
  List<PropertyConvergence> getProperties()
  {
    return Collections.unmodifiableList(this.properties);
  }
}
