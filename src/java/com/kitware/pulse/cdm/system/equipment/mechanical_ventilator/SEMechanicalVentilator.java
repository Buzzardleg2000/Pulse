/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.system.equipment.mechanical_ventilator;

import com.kitware.pulse.cdm.bind.Enums.eBreathState;
import com.kitware.pulse.cdm.properties.*;
import com.kitware.pulse.cdm.system.equipment.SEEquipment;

public class SEMechanicalVentilator implements SEEquipment
{
  protected SEScalarPressure                  airwayPressure;
  protected eBreathState                      breathState;
  protected SEScalarVolumePerPressure         dynamicPulmonaryCompliance ;
  protected SEScalar0To1                      endTidalCarbonDioxideFraction;
  protected SEScalarPressure                  endTidalCarbonDioxidePressure;
  protected SEScalar0To1                      endTidalOxygenFraction;
  protected SEScalarPressure                  endTidalOxygenPressure;
  protected SEScalarVolumePerTime             expiratoryFlow;
  protected SEScalarVolume                    expiratoryTidalVolume;
  protected SEScalar                          inspiratoryExpiratoryRatio;
  protected SEScalarVolumePerTime             inspiratoryFlow;
  protected SEScalarVolume                    inspiratoryTidalVolume;
  protected SEScalarPressure                  intrinsicPositiveEndExpiredPressure;
  protected SEScalar0To1                      leakFraction;
  protected SEScalarPressure                  meanAirwayPressure;
  protected SEScalarPressure                  peakInspiratoryPressure;
  protected SEScalarPressure                  plateauPressure;
  protected SEScalarPressure                  positiveEndExpiratoryPressure;
  protected SEScalarPressureTimePerVolume     pulmonaryResistance;
  protected SEScalarFrequency                 respirationRate;
  protected SEScalarVolumePerPressure         staticPulmonaryCompliance;
  protected SEScalarVolume                    tidalVolume;
  protected SEScalarVolume                    totalLungVolume;
  protected SEScalarVolumePerTime             totalPulmonaryVentilation;

  public SEMechanicalVentilator()
  {
    airwayPressure = null;
    breathState = null;
    dynamicPulmonaryCompliance = null;
    endTidalCarbonDioxideFraction = null;
    endTidalCarbonDioxidePressure = null;
    endTidalOxygenFraction = null;
    endTidalOxygenPressure = null;
    expiratoryFlow = null;
    expiratoryTidalVolume = null;
    inspiratoryExpiratoryRatio = null;
    inspiratoryFlow = null;
    inspiratoryTidalVolume = null;
    intrinsicPositiveEndExpiredPressure = null;
    leakFraction = null;
    meanAirwayPressure = null;
    peakInspiratoryPressure = null;
    plateauPressure = null;
    positiveEndExpiratoryPressure = null;
    pulmonaryResistance = null;
    respirationRate = null;
    staticPulmonaryCompliance = null;
    tidalVolume = null;
    totalLungVolume = null;
    totalPulmonaryVentilation = null;
  }

  @Override
  public void clear()
  {
    if (airwayPressure != null)
      airwayPressure.invalidate();
    breathState = null;
    if (dynamicPulmonaryCompliance != null)
      dynamicPulmonaryCompliance.invalidate();
    if (endTidalCarbonDioxideFraction != null)
      endTidalCarbonDioxideFraction.invalidate();
    if (endTidalCarbonDioxidePressure != null)
      endTidalCarbonDioxidePressure.invalidate();
    if (endTidalOxygenFraction != null)
      endTidalOxygenFraction.invalidate();
    if (endTidalOxygenPressure != null)
      endTidalOxygenPressure.invalidate();
    if (expiratoryFlow != null)
      expiratoryFlow.invalidate();
    if (expiratoryTidalVolume != null)
      expiratoryTidalVolume.invalidate();
    if (inspiratoryExpiratoryRatio != null)
      inspiratoryExpiratoryRatio.invalidate();
    if (inspiratoryFlow != null)
      inspiratoryFlow.invalidate();
    if (inspiratoryTidalVolume != null)
      inspiratoryTidalVolume.invalidate();
    if (intrinsicPositiveEndExpiredPressure != null)
      intrinsicPositiveEndExpiredPressure.invalidate();
    if (leakFraction != null)
      leakFraction.invalidate();
    if (meanAirwayPressure != null)
      meanAirwayPressure.invalidate();
    if (peakInspiratoryPressure != null)
      peakInspiratoryPressure.invalidate();
    if (plateauPressure != null)
      plateauPressure.invalidate();
    if (positiveEndExpiratoryPressure != null)
      positiveEndExpiratoryPressure.invalidate();
    if (pulmonaryResistance != null)
      pulmonaryResistance.invalidate();
    if (respirationRate != null)
      respirationRate.invalidate();
    if (staticPulmonaryCompliance != null)
      staticPulmonaryCompliance.invalidate();
    if (tidalVolume != null)
      tidalVolume.invalidate();
    if (totalLungVolume != null)
      totalLungVolume.invalidate();
    if (totalPulmonaryVentilation != null)
      totalPulmonaryVentilation.invalidate();
  }
  
  public boolean hasAirwayPressure()
  {
    return airwayPressure == null ? false : airwayPressure.isValid();
  }
  public SEScalarPressure getAirwayPressure()
  {
    if (airwayPressure == null)
      airwayPressure = new SEScalarPressure();
    return airwayPressure;
  }
  
  public eBreathState getBreathState()
  {
    return breathState;
  }
  public void setBreathState(eBreathState b)
  {
    breathState = (b == eBreathState.UNRECOGNIZED) ? null : b;
  }
  public boolean hasBreathState()
  {
    return breathState != null;
  }
  
  public boolean hasDynamicPulmonaryCompliance()
  {
    return dynamicPulmonaryCompliance == null ? false : dynamicPulmonaryCompliance.isValid();
  }
  public SEScalarVolumePerPressure getDynamicPulmonaryCompliance()
  {
    if (dynamicPulmonaryCompliance == null)
      dynamicPulmonaryCompliance = new SEScalarVolumePerPressure();
    return dynamicPulmonaryCompliance;
  }

  public boolean hasEndTidalCarbonDioxideFraction()
  {
    return endTidalCarbonDioxideFraction == null ? false : endTidalCarbonDioxideFraction.isValid();
  }
  public SEScalar0To1 getEndTidalCarbonDioxideFraction()
  {
    if (endTidalCarbonDioxideFraction == null)
      endTidalCarbonDioxideFraction = new SEScalar0To1();
    return endTidalCarbonDioxideFraction;
  }

  public boolean hasEndTidalCarbonDioxidePressure()
  {
    return endTidalCarbonDioxidePressure == null ? false : endTidalCarbonDioxidePressure.isValid();
  }
  public SEScalarPressure getEndTidalCarbonDioxidePressure()
  {
    if (endTidalCarbonDioxidePressure == null)
      endTidalCarbonDioxidePressure = new SEScalarPressure();
    return endTidalCarbonDioxidePressure;
  }
  
  public boolean hasEndTidalOxygenFraction()
  {
    return endTidalOxygenFraction == null ? false : endTidalOxygenFraction.isValid();
  }
  public SEScalar0To1 getEndTidalOxygenFraction()
  {
    if (endTidalOxygenFraction == null)
      endTidalOxygenFraction = new SEScalar0To1();
    return endTidalOxygenFraction;
  }

  public boolean hasEndTidalOxygenPressure()
  {
    return endTidalOxygenPressure == null ? false : endTidalOxygenPressure.isValid();
  }
  public SEScalarPressure getEndTidalOxygenPressure()
  {
    if (endTidalOxygenPressure == null)
      endTidalOxygenPressure = new SEScalarPressure();
    return endTidalOxygenPressure;
  }

  public boolean hasExpiratoryFlow()
  {
    return expiratoryFlow == null ? false : expiratoryFlow.isValid();
  }
  public SEScalarVolumePerTime getExpiratoryFlow()
  {
    if (expiratoryFlow == null)
      expiratoryFlow = new SEScalarVolumePerTime();
    return expiratoryFlow;
  }

  public boolean hasExpiratoryTidalVolume()
  {
    return expiratoryTidalVolume == null ? false : expiratoryTidalVolume.isValid();
  }
  public SEScalarVolume getExpiratoryTidalVolume()
  {
    if (expiratoryTidalVolume == null)
      expiratoryTidalVolume = new SEScalarVolume();
    return expiratoryTidalVolume;
  }
  
  public boolean hasInspiratoryExpiratoryRatio()
  {
    return inspiratoryExpiratoryRatio == null ? false : inspiratoryExpiratoryRatio.isValid();
  }
  public SEScalar getInspiratoryExpiratoryRatio()
  {
    if (inspiratoryExpiratoryRatio == null)
      inspiratoryExpiratoryRatio = new SEScalar();
    return inspiratoryExpiratoryRatio;
  }

  public boolean hasInspiratoryFlow()
  {
    return inspiratoryFlow == null ? false : inspiratoryFlow.isValid();
  }
  public SEScalarVolumePerTime getInspiratoryFlow()
  {
    if (inspiratoryFlow == null)
      inspiratoryFlow = new SEScalarVolumePerTime();
    return inspiratoryFlow;
  }

  public boolean hasInspiratoryTidalVolume()
  {
    return inspiratoryTidalVolume == null ? false : inspiratoryTidalVolume.isValid();
  }
  public SEScalarVolume getInspiratoryTidalVolume()
  {
    if (inspiratoryTidalVolume == null)
      inspiratoryTidalVolume = new SEScalarVolume();
    return inspiratoryTidalVolume;
  }
  
  public boolean hasIntrinsicPositiveEndExpiredPressure()
  {
    return intrinsicPositiveEndExpiredPressure == null ? false : intrinsicPositiveEndExpiredPressure.isValid();
  }
  public SEScalarPressure getIntrinsicPositiveEndExpiredPressure()
  {
    if (intrinsicPositiveEndExpiredPressure == null)
      intrinsicPositiveEndExpiredPressure = new SEScalarPressure();
    return intrinsicPositiveEndExpiredPressure;
  }

  public boolean hasLeakFraction()
  {
    return leakFraction == null ? false : leakFraction.isValid();
  }
  public SEScalar0To1 getLeakFraction()
  {
    if (leakFraction == null)
      leakFraction = new SEScalar0To1();
    return leakFraction;
  }
  
  public boolean hasMeanAirwayPressure()
  {
    return meanAirwayPressure == null ? false : meanAirwayPressure.isValid();
  }
  public SEScalarPressure getMeanAirwayPressure()
  {
    if (meanAirwayPressure == null)
      meanAirwayPressure = new SEScalarPressure();
    return meanAirwayPressure;
  }
  
  public boolean hasPeakInspiratoryPressure()
  {
    return peakInspiratoryPressure == null ? false : peakInspiratoryPressure.isValid();
  }
  public SEScalarPressure getPeakInspiratoryPressure()
  {
    if (peakInspiratoryPressure == null)
      peakInspiratoryPressure = new SEScalarPressure();
    return peakInspiratoryPressure;
  }
  
  public boolean hasPlateauPressure()
  {
    return plateauPressure == null ? false : plateauPressure.isValid();
  }
  public SEScalarPressure getPlateauPressure()
  {
    if (plateauPressure == null)
      plateauPressure = new SEScalarPressure();
    return plateauPressure;
  }
  
  public boolean hasPositiveEndExpiratoryPressure()
  {
    return positiveEndExpiratoryPressure == null ? false : positiveEndExpiratoryPressure.isValid();
  }
  public SEScalarPressure getPositiveEndExpiratoryPressure()
  {
    if (positiveEndExpiratoryPressure == null)
      positiveEndExpiratoryPressure = new SEScalarPressure();
    return positiveEndExpiratoryPressure;
  }
  
  public boolean hasPulmonaryResistance()
  {
    return pulmonaryResistance == null ? false : pulmonaryResistance.isValid();
  }
  public SEScalarPressureTimePerVolume getPulmonaryResistance()
  {
    if (pulmonaryResistance == null)
      pulmonaryResistance = new SEScalarPressureTimePerVolume();
    return pulmonaryResistance;
  }

  public boolean hasRespirationRate()
  {
    return respirationRate == null ? false : respirationRate.isValid();
  }
  public SEScalarFrequency getRespirationRate()
  {
    if (respirationRate == null)
      respirationRate = new SEScalarFrequency();
    return respirationRate;
  }
  
  public boolean hasStaticPulmonaryCompliance()
  {
    return staticPulmonaryCompliance == null ? false : staticPulmonaryCompliance.isValid();
  }
  public SEScalarVolumePerPressure getStaticPulmonaryCompliance()
  {
    if (staticPulmonaryCompliance == null)
      staticPulmonaryCompliance = new SEScalarVolumePerPressure();
    return staticPulmonaryCompliance;
  }

  public boolean hasTidalVolume()
  {
    return tidalVolume == null ? false : tidalVolume.isValid();
  }
  public SEScalarVolume getTidalVolume()
  {
    if (tidalVolume == null)
      tidalVolume = new SEScalarVolume();
    return tidalVolume;
  }

  public boolean hasTotalLungVolume()
  {
    return totalLungVolume == null ? false : totalLungVolume.isValid();
  }
  public SEScalarVolume getTotalLungVolume()
  {
    if (totalLungVolume == null)
      totalLungVolume = new SEScalarVolume();
    return totalLungVolume;
  }
  
  public boolean hasTotalPulmonaryVentilation()
  {
    return totalPulmonaryVentilation == null ? false : totalPulmonaryVentilation.isValid();
  }
  public SEScalarVolumePerTime getTotalPulmonaryVentilation()
  {
    if (totalPulmonaryVentilation == null)
      totalPulmonaryVentilation = new SEScalarVolumePerTime();
    return totalPulmonaryVentilation;
  }
}