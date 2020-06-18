/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.cdm.system.physiology;

import pulse.cdm.bind.Physiology.DrugSystemData;
import pulse.cdm.properties.SEScalar0To1;
import pulse.cdm.properties.SEScalarFrequency;
import pulse.cdm.properties.SEScalarNegative1To1;
import pulse.cdm.properties.SEScalarPressure;
import pulse.cdm.properties.SEScalarVolume;
import pulse.cdm.system.SESystem;

public class SEDrugSystem extends SEPhysiologySystem implements SESystem
{
  protected SEScalarNegative1To1 bronchodilationLevel;
  protected SEScalarFrequency    heartRateChange;
  protected SEScalarPressure     meanBloodPressureChange;
  protected SEScalar0To1         neuromuscularBlockLevel;
  protected SEScalarPressure     pulsePressureChange;
  protected SEPupillaryResponse  pupillaryResponse;
  protected SEScalarFrequency    respirationRateChange;
  protected SEScalar0To1         sedationLevel;
  protected SEScalarVolume       tidalVolumeChange;
  protected SEScalarNegative1To1 tubularPermeabilityChange;

  public SEDrugSystem()
  {
    bronchodilationLevel = null;
    heartRateChange = null;
    meanBloodPressureChange = null;
    neuromuscularBlockLevel = null;
    pulsePressureChange = null;
    pupillaryResponse = null;
    respirationRateChange = null;
    sedationLevel = null;
    tidalVolumeChange = null;
    tubularPermeabilityChange = null;
  }

  public void reset()
  {
    if (bronchodilationLevel != null)
      bronchodilationLevel.invalidate();
    if (heartRateChange != null)
      heartRateChange.invalidate();
    if (meanBloodPressureChange != null)
      meanBloodPressureChange.invalidate();
    if (neuromuscularBlockLevel != null)
      neuromuscularBlockLevel.invalidate();
    if (pulsePressureChange != null)
      pulsePressureChange.invalidate();
    if (pupillaryResponse != null)
      pupillaryResponse.reset();
    if (respirationRateChange != null)
      respirationRateChange.invalidate();
    if (sedationLevel != null)
      sedationLevel.invalidate();
    if (tidalVolumeChange != null)
      tidalVolumeChange.invalidate();
    if (tubularPermeabilityChange != null)
      tubularPermeabilityChange.invalidate();
  }

  public static void load(DrugSystemData src, SEDrugSystem dst)
  {
    if (src.hasBronchodilationLevel())
    	SEScalarNegative1To1.load(src.getBronchodilationLevel(),dst.getBronchodilationLevel());
    if (src.hasHeartRateChange())
      SEScalarFrequency.load(src.getHeartRateChange(),dst.getHeartRateChange());
    if (src.hasMeanBloodPressureChange())
      SEScalarPressure.load(src.getMeanBloodPressureChange(),dst.getMeanBloodPressureChange());
    if (src.hasNeuromuscularBlockLevel())
      SEScalar0To1.load(src.getNeuromuscularBlockLevel(),dst.getNeuromuscularBlockLevel());
    if (src.hasPulsePressureChange())
      SEScalarPressure.load(src.getPulsePressureChange(),dst.getPulsePressureChange());
    if (src.hasPupillaryResponse())
      SEPupillaryResponse.load(src.getPupillaryResponse(),dst.getPupillaryResponse());
    if (src.hasRespirationRateChange())
      SEScalarFrequency.load(src.getRespirationRateChange(),dst.getRespirationRateChange());
    if (src.hasSedationLevel())
      SEScalar0To1.load(src.getSedationLevel(),dst.getSedationLevel());
    if (src.hasTidalVolumeChange())
      SEScalarVolume.load(src.getTidalVolumeChange(),dst.getTidalVolumeChange());
    if (src.hasTubularPermeabilityChange())
    	SEScalarNegative1To1.load(src.getTubularPermeabilityChange(),dst.getTubularPermeabilityChange());
  }

  public static DrugSystemData unload(SEDrugSystem src)
  {
    DrugSystemData.Builder dst = DrugSystemData.newBuilder();
    unload(src,dst);
    return dst.build();
  }

  protected static void unload(SEDrugSystem src, DrugSystemData.Builder dst)
  {
    if (src.hasBronchodilationLevel())
      dst.setBronchodilationLevel(SEScalarNegative1To1.unload(src.getBronchodilationLevel()));
    if (src.hasHeartRateChange())
      dst.setHeartRateChange(SEScalarFrequency.unload(src.getHeartRateChange()));
    if (src.hasMeanBloodPressureChange())
      dst.setMeanBloodPressureChange(SEScalarPressure.unload(src.getMeanBloodPressureChange()));
    if (src.hasNeuromuscularBlockLevel())
      dst.setNeuromuscularBlockLevel(SEScalar0To1.unload(src.getNeuromuscularBlockLevel()));
    if (src.hasPulsePressureChange())
      dst.setPulsePressureChange(SEScalarPressure.unload(src.getPulsePressureChange()));
    if (src.hasPupillaryResponse())
      dst.setPupillaryResponse(SEPupillaryResponse.unload(src.getPupillaryResponse()));
    if (src.hasRespirationRateChange())
      dst.setRespirationRateChange(SEScalarFrequency.unload(src.getRespirationRateChange()));
    if (src.hasSedationLevel())
      dst.setSedationLevel(SEScalar0To1.unload(src.getSedationLevel()));
    if (src.hasTidalVolumeChange())
      dst.setTidalVolumeChange(SEScalarVolume.unload(src.getTidalVolumeChange()));
    if (src.hasTubularPermeabilityChange())
      dst.setTubularPermeabilityChange(SEScalarNegative1To1.unload(src.getTubularPermeabilityChange()));
  }
  
  public SEScalarNegative1To1 getBronchodilationLevel()
  {
    if (bronchodilationLevel == null)
      bronchodilationLevel = new SEScalarNegative1To1();
    return bronchodilationLevel;
  }
  public boolean hasBronchodilationLevel()
  {
    return bronchodilationLevel == null ? false : bronchodilationLevel.isValid();
  }
  
  public SEScalarFrequency getHeartRateChange()
  {
    if (heartRateChange == null)
      heartRateChange = new SEScalarFrequency();
    return heartRateChange;
  }
  public boolean hasHeartRateChange()
  {
    return heartRateChange == null ? false : heartRateChange.isValid();
  }
  
  public SEScalarPressure getMeanBloodPressureChange()
  {
    if (meanBloodPressureChange == null)
      meanBloodPressureChange = new SEScalarPressure();
    return meanBloodPressureChange;
  }
  public boolean hasMeanBloodPressureChange()
  {
    return meanBloodPressureChange == null ? false : meanBloodPressureChange.isValid();
  }
  
  public SEScalar0To1 getNeuromuscularBlockLevel()
  {
    if (neuromuscularBlockLevel == null)
      neuromuscularBlockLevel = new SEScalar0To1();
    return neuromuscularBlockLevel;
  }
  public boolean hasNeuromuscularBlockLevel()
  {
    return neuromuscularBlockLevel == null ? false : neuromuscularBlockLevel.isValid();
  }
  
  public SEScalarPressure getPulsePressureChange()
  {
    if (pulsePressureChange == null)
      pulsePressureChange = new SEScalarPressure();
    return pulsePressureChange;
  }
  public boolean hasPulsePressureChange()
  {
    return pulsePressureChange == null ? false : pulsePressureChange.isValid();
  }
  
  public SEPupillaryResponse getPupillaryResponse()
  {
    if (pupillaryResponse == null)
      pupillaryResponse = new SEPupillaryResponse();
    return pupillaryResponse;
  }
  public boolean hasPupillaryResponse()
  {
    return pupillaryResponse != null;
  }
  
  public SEScalarFrequency getRespirationRateChange()
  {
    if (respirationRateChange == null)
      respirationRateChange = new SEScalarFrequency();
    return respirationRateChange;
  }
  public boolean hasRespirationRateChange()
  {
    return respirationRateChange == null ? false : respirationRateChange.isValid();
  }
  
  public SEScalar0To1 getSedationLevel()
  {
    if (sedationLevel == null)
      sedationLevel = new SEScalar0To1();
    return sedationLevel;
  }
  public boolean hasSedationLevel()
  {
    return sedationLevel == null ? false : sedationLevel.isValid();
  }
  
  public SEScalarVolume getTidalVolumeChange()
  {
    if (tidalVolumeChange == null)
      tidalVolumeChange = new SEScalarVolume();
    return tidalVolumeChange;
  }
  public boolean hasTidalVolumeChange()
  {
    return tidalVolumeChange == null ? false : tidalVolumeChange.isValid();
  }
  
  public SEScalarNegative1To1 getTubularPermeabilityChange()
  {
    if (tubularPermeabilityChange == null)
      tubularPermeabilityChange = new SEScalarNegative1To1();
    return tubularPermeabilityChange;
  }
  public boolean hasTubularPermeabilityChange()
  {
    return tubularPermeabilityChange == null ? false : tubularPermeabilityChange.isValid();
  }
}
