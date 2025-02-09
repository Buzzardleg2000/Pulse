/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.patient;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;

import com.kitware.pulse.cdm.bind.Patient.PatientData;
import com.kitware.pulse.cdm.bind.Patient.PatientData.eSex;
import com.kitware.pulse.cdm.properties.SEScalar;
import com.kitware.pulse.cdm.properties.SEScalar0To1;
import com.kitware.pulse.cdm.properties.SEScalarArea;
import com.kitware.pulse.cdm.properties.SEScalarFrequency;
import com.kitware.pulse.cdm.properties.SEScalarLength;
import com.kitware.pulse.cdm.properties.SEScalarMass;
import com.kitware.pulse.cdm.properties.SEScalarMassPerVolume;
import com.kitware.pulse.cdm.properties.SEScalarPower;
import com.kitware.pulse.cdm.properties.SEScalarPressure;
import com.kitware.pulse.cdm.properties.SEScalarTime;
import com.kitware.pulse.cdm.properties.SEScalarVolume;
import com.kitware.pulse.utilities.FileUtils;

public class SEPatient
{
  protected String                    name;
  protected eSex                      sex;
  protected SEScalarTime              age;
  protected SEScalarMass              weight;
  protected SEScalarLength            height;
  protected SEScalarMassPerVolume     bodyDensity;
  protected SEScalar0To1              bodyFatFraction;
  protected SEScalar                  bodyMassIndex;
  protected SEScalarMass              leanBodyMass;
  protected SEScalarMass              idealBodyWeight;

  protected SEScalarArea              alveoliSurfaceArea;
  protected SEScalar0To1              rightLungRatio;
  protected SEScalarArea              skinSurfaceArea;
  
  protected SEScalarPower             basalMetabolicRate;
  protected SEScalarVolume            bloodVolumeBaseline;
  protected SEScalarPressure          diastolicArterialPressureBaseline;
  protected SEScalarFrequency         heartRateBaseline;
  protected SEScalarPressure          meanArterialPressureBaseline;
  protected SEScalarPressure          pulsePressureBaseline;
  protected SEScalarFrequency         respirationRateBaseline;
  protected SEScalarPressure          systolicArterialPressureBaseline;
  protected SEScalarVolume            tidalVolumeBaseline;

  protected SEScalarFrequency         heartRateMaximum;
  protected SEScalarFrequency         heartRateMinimum;
  protected SEScalarVolume            expiratoryReserveVolume;
  protected SEScalarVolume            functionalResidualCapacity;
  protected SEScalarVolume            inspiratoryCapacity;
  protected SEScalarVolume            inspiratoryReserveVolume;
  protected SEScalarVolume            residualVolume;
  protected SEScalarVolume            totalLungCapacity;
  protected SEScalarVolume            vitalCapacity;

  public SEPatient()
  {

  }

  public void clear()
  {
    this.name=null;
    this.sex=null;
    if(this.age!=null)
      this.age.invalidate();
    if(this.weight!=null)
      this.weight.invalidate();
    if(this.height!=null)
      this.height.invalidate();
    if(bodyDensity != null)
      this.bodyDensity.invalidate();
    if(bodyFatFraction != null)
      this.bodyFatFraction.invalidate();
    if(bodyMassIndex != null)
      this.bodyMassIndex.invalidate();
    if(leanBodyMass != null)
      this.leanBodyMass.invalidate();
    if(idealBodyWeight != null)
        this.idealBodyWeight.invalidate();
    
    if(alveoliSurfaceArea != null)
      alveoliSurfaceArea.invalidate();
    if(rightLungRatio != null)
      this.rightLungRatio.invalidate();
    if(skinSurfaceArea != null)
      this.skinSurfaceArea.invalidate();
      
    
    if(basalMetabolicRate != null)
      this.basalMetabolicRate.invalidate();
    if(bloodVolumeBaseline != null)
      this.bloodVolumeBaseline.invalidate();
    if(diastolicArterialPressureBaseline != null)
      this.diastolicArterialPressureBaseline.invalidate();
    if(heartRateBaseline != null)
      this.heartRateBaseline.invalidate();
    if(meanArterialPressureBaseline != null)
      this.meanArterialPressureBaseline.invalidate();
    if(pulsePressureBaseline != null)
      this.pulsePressureBaseline.invalidate();
    if(respirationRateBaseline != null)
      this.respirationRateBaseline.invalidate();
    if(systolicArterialPressureBaseline != null)
      this.systolicArterialPressureBaseline.invalidate();
    if(tidalVolumeBaseline != null)
      this.tidalVolumeBaseline.invalidate();
    
    if(heartRateMaximum != null)
      this.heartRateMaximum.invalidate();
    if(heartRateMinimum != null)
      this.heartRateMinimum.invalidate();
    if(expiratoryReserveVolume != null)
      this.expiratoryReserveVolume.invalidate();
    if(functionalResidualCapacity != null)
      this.functionalResidualCapacity.invalidate();
    if(inspiratoryCapacity != null)
      this.inspiratoryCapacity.invalidate();
    if(inspiratoryReserveVolume != null)
      this.inspiratoryReserveVolume.invalidate();
    if(residualVolume != null)
      this.residualVolume.invalidate();
    if(totalLungCapacity != null)
      this.totalLungCapacity.invalidate();
    if(vitalCapacity != null)
      this.vitalCapacity.invalidate();
  }
  
  public void readFile(String fileName) throws InvalidProtocolBufferException
  {
    PatientData.Builder builder = PatientData.newBuilder();
    JsonFormat.parser().merge(FileUtils.readFile(fileName), builder);
    SEPatient.load(builder.build(), this);
  }
  public void writeFile(String fileName) throws InvalidProtocolBufferException
  {
    PatientData pData = SEPatient.unload(this);
    FileUtils.writeFile(fileName, JsonFormat.printer().print(pData));
  }

  public static void load(PatientData src, SEPatient dst)
  {
    dst.clear();
    dst.setName(src.getName());
    if(src.getSex()!=eSex.UNRECOGNIZED)
      dst.setSex(src.getSex());
    if(src.hasAge())
      SEScalarTime.load(src.getAge(),dst.getAge());
    if(src.hasWeight())
      SEScalarMass.load(src.getWeight(),dst.getWeight());
    if(src.hasHeight())
      SEScalarLength.load(src.getHeight(),dst.getHeight());
    if(src.hasBodyDensity())
      SEScalarMassPerVolume.load(src.getBodyDensity(),dst.getBodyDensity());
    if(src.hasBodyFatFraction())
      SEScalar0To1.load(src.getBodyFatFraction(),dst.getBodyFatFraction());
    if(src.hasBodyMassIndex())
      SEScalar.load(src.getBodyMassIndex(),dst.getBodyMassIndex());
    if(src.hasLeanBodyMass())
      SEScalarMass.load(src.getLeanBodyMass(),dst.getLeanBodyMass());
    if(src.hasIdealBodyWeight())
      SEScalarMass.load(src.getIdealBodyWeight(),dst.getIdealBodyWeight());
    
    if(src.hasAlveoliSurfaceArea())
      SEScalarArea.load(src.getAlveoliSurfaceArea(),dst.getAlveoliSurfaceArea());    
    if(src.hasRightLungRatio())
      SEScalar0To1.load(src.getRightLungRatio(),dst.getRightLungRatio());
    if(src.hasSkinSurfaceArea())
      SEScalarArea.load(src.getSkinSurfaceArea(),dst.getSkinSurfaceArea());
    
    if(src.hasBasalMetabolicRate())
      SEScalarPower.load(src.getBasalMetabolicRate(),dst.getBasalMetabolicRate());
    if(src.hasBloodVolumeBaseline())
      SEScalarVolume.load(src.getBloodVolumeBaseline(),dst.getBloodVolumeBaseline());
    if(src.hasDiastolicArterialPressureBaseline())
      SEScalarPressure.load(src.getDiastolicArterialPressureBaseline(),dst.getDiastolicArterialPressureBaseline());
    if(src.hasHeartRateBaseline())
      SEScalarFrequency.load(src.getHeartRateBaseline(),dst.getHeartRateBaseline());
    if(src.hasMeanArterialPressureBaseline())
      SEScalarPressure.load(src.getMeanArterialPressureBaseline(),dst.getMeanArterialPressureBaseline());
    if(src.hasPulsePressureBaseline())
      SEScalarPressure.load(src.getPulsePressureBaseline(),dst.getPulsePressureBaseline());
    if(src.hasRespirationRateBaseline())
      SEScalarFrequency.load(src.getRespirationRateBaseline(),dst.getRespirationRateBaseline());
    if(src.hasSystolicArterialPressureBaseline())
      SEScalarPressure.load(src.getSystolicArterialPressureBaseline(),dst.getSystolicArterialPressureBaseline());
    if(src.hasTidalVolumeBaseline())
      SEScalarVolume.load(src.getTidalVolumeBaseline(),dst.getTidalVolumeBaseline());
    
    if(src.hasHeartRateMaximum())
      SEScalarFrequency.load(src.getHeartRateMaximum(),dst.getHeartRateMaximum());
    if(src.hasHeartRateMinimum())
      SEScalarFrequency.load(src.getHeartRateMinimum(),dst.getHeartRateMinimum());
    if(src.hasExpiratoryReserveVolume())
      SEScalarVolume.load(src.getExpiratoryReserveVolume(),dst.getExpiratoryReserveVolume());
    if(src.hasFunctionalResidualCapacity())
      SEScalarVolume.load(src.getFunctionalResidualCapacity(),dst.getFunctionalResidualCapacity());
    if(src.hasInspiratoryCapacity())
      SEScalarVolume.load(src.getInspiratoryCapacity(),dst.getInspiratoryCapacity());
    if(src.hasInspiratoryReserveVolume())
      SEScalarVolume.load(src.getInspiratoryReserveVolume(),dst.getInspiratoryReserveVolume());
    if(src.hasResidualVolume())
      SEScalarVolume.load(src.getResidualVolume(),dst.getResidualVolume());
    if(src.hasTotalLungCapacity())
      SEScalarVolume.load(src.getTotalLungCapacity(),dst.getTotalLungCapacity());
    if(src.hasVitalCapacity())
      SEScalarVolume.load(src.getVitalCapacity(),dst.getVitalCapacity());
  }

  public static PatientData unload(SEPatient src)
  {
    PatientData.Builder dst = PatientData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEPatient src, PatientData.Builder dst)
  {
    if(src.hasName())
      dst.setName(src.name);
    if(src.hasSex())
      dst.setSex(src.sex);
    if(src.hasAge())
      dst.setAge(SEScalarTime.unload(src.age));
    if(src.hasWeight())
      dst.setWeight(SEScalarMass.unload(src.weight));
    if(src.hasHeight())
      dst.setHeight(SEScalarLength.unload(src.height));
    if(src.hasBodyDensity())
      dst.setBodyDensity(SEScalarMassPerVolume.unload(src.bodyDensity));
    if(src.hasBodyFatFraction())
      dst.setBodyFatFraction(SEScalar0To1.unload(src.bodyFatFraction));
    if(src.hasBodyMassIndex())
      dst.setBodyMassIndex(SEScalar.unload(src.bodyMassIndex));
    if(src.hasLeanBodyMass())
      dst.setLeanBodyMass(SEScalarMass.unload(src.leanBodyMass));
    if(src.hasIdealBodyWeight())
      dst.setIdealBodyWeight(SEScalarMass.unload(src.idealBodyWeight));
    
    if(src.hasAlveoliSurfaceArea())
      dst.setAlveoliSurfaceArea(SEScalarArea.unload(src.alveoliSurfaceArea));    
    if(src.hasRightLungRatio())
      dst.setRightLungRatio(SEScalar0To1.unload(src.rightLungRatio));
    if(src.hasSkinSurfaceArea())
      dst.setSkinSurfaceArea(SEScalarArea.unload(src.skinSurfaceArea));
    
    if(src.hasBasalMetabolicRate())
      dst.setBasalMetabolicRate(SEScalarPower.unload(src.basalMetabolicRate));
    if(src.hasDiastolicArterialPressureBaseline())
      dst.setDiastolicArterialPressureBaseline(SEScalarPressure.unload(src.diastolicArterialPressureBaseline));  
    if(src.hasHeartRateBaseline())
      dst.setHeartRateBaseline(SEScalarFrequency.unload(src.heartRateBaseline));
    if(src.hasMeanArterialPressureBaseline())
      dst.setMeanArterialPressureBaseline(SEScalarPressure.unload(src.meanArterialPressureBaseline));
    if(src.hasPulsePressureBaseline())
      dst.setPulsePressureBaseline(SEScalarPressure.unload(src.pulsePressureBaseline));
    if(src.hasRespirationRateBaseline())
      dst.setRespirationRateBaseline(SEScalarFrequency.unload(src.respirationRateBaseline));
    if(src.hasSystolicArterialPressureBaseline())
      dst.setSystolicArterialPressureBaseline(SEScalarPressure.unload(src.systolicArterialPressureBaseline));
    if(src.hasTidalVolumeBaseline())
      dst.setTidalVolumeBaseline(SEScalarVolume.unload(src.tidalVolumeBaseline));
    
    if(src.hasHeartRateMaximum())
      dst.setHeartRateMaximum(SEScalarFrequency.unload(src.heartRateMaximum));
    if(src.hasHeartRateMinimum())
      dst.setHeartRateMinimum(SEScalarFrequency.unload(src.heartRateMinimum));
    if(src.hasExpiratoryReserveVolume())
      dst.setExpiratoryReserveVolume(SEScalarVolume.unload(src.expiratoryReserveVolume));  
    if(src.hasFunctionalResidualCapacity())
      dst.setFunctionalResidualCapacity(SEScalarVolume.unload(src.functionalResidualCapacity));
    if(src.hasInspiratoryCapacity())
      dst.setInspiratoryCapacity(SEScalarVolume.unload(src.inspiratoryCapacity));
    if(src.hasInspiratoryReserveVolume())
      dst.setInspiratoryReserveVolume(SEScalarVolume.unload(src.inspiratoryReserveVolume));
    if(src.hasTotalLungCapacity())
      dst.setTotalLungCapacity(SEScalarVolume.unload(src.totalLungCapacity));
    if(src.hasBloodVolumeBaseline())
      dst.setBloodVolumeBaseline(SEScalarVolume.unload(src.bloodVolumeBaseline));
    if(src.hasVitalCapacity())
      dst.setVitalCapacity(SEScalarVolume.unload(src.vitalCapacity));
  }

  public String  getName() { return this.name;}
  public void    setName(String name){this.name=name;}
  public boolean hasName(){return this.name==null?false:true;}

  public eSex    getSex() { return this.sex;}
  public void    setSex(eSex name){this.sex=name;}
  public boolean hasSex(){return this.sex==null?false:true;}

  public SEScalarTime getAge() 
  { 
    if(this.age==null)
      this.age=new SEScalarTime();
    return this.age;
  }
  public boolean      hasAge() {return this.age==null?false:this.age.isValid();}

  public SEScalarMass getWeight() 
  { 
    if(this.weight==null)
      this.weight=new SEScalarMass();
    return this.weight;
  }
  public boolean      hasWeight() {return this.weight==null?false:this.weight.isValid();}

  public SEScalarLength getHeight() 
  { 
    if(this.height==null)
      this.height=new SEScalarLength();
    return this.height;
  }
  public boolean          hasHeight() {return this.height==null?false:this.height.isValid();}

  public boolean hasAlveoliSurfaceArea()
  {
    return alveoliSurfaceArea == null ? false : alveoliSurfaceArea.isValid();
  }
  public SEScalarArea getAlveoliSurfaceArea()
  {
    if (alveoliSurfaceArea == null)
      alveoliSurfaceArea = new SEScalarArea();
    return alveoliSurfaceArea;
  }

  public boolean hasBasalMetabolicRate()
  {
    return basalMetabolicRate == null ? false : basalMetabolicRate.isValid();
  }
  public SEScalarPower getBasalMetabolicRate()
  {
    if (basalMetabolicRate == null)
      basalMetabolicRate = new SEScalarPower();
    return basalMetabolicRate;
  }
  
  public SEScalarMassPerVolume getBodyDensity()
  {
    if(this.bodyDensity==null)
      this.bodyDensity=new SEScalarMassPerVolume();
    return this.bodyDensity;
  }
  public boolean hasBodyDensity() {return this.bodyDensity==null?false:this.bodyDensity.isValid();}

  public SEScalar0To1 getBodyFatFraction()
  {
    if(this.bodyFatFraction==null)
      this.bodyFatFraction=new SEScalar0To1();
    return this.bodyFatFraction;
  }
  public boolean hasBodyFatFraction() {return this.bodyFatFraction==null?false:this.bodyFatFraction.isValid();}

  public SEScalar getBodyMassIndex()
  {
    if(this.bodyMassIndex==null)
      this.bodyMassIndex=new SEScalar();
    return this.bodyMassIndex;
  }
  public boolean hasBodyMassIndex() {return this.bodyMassIndex==null?false:this.bodyMassIndex.isValid();}

  public SEScalarPressure getDiastolicArterialPressureBaseline() 
  { 
    if(this.diastolicArterialPressureBaseline==null)
      this.diastolicArterialPressureBaseline=new SEScalarPressure();
    return this.diastolicArterialPressureBaseline;
  }
  public boolean  hasDiastolicArterialPressureBaseline() {return this.diastolicArterialPressureBaseline==null?false:this.diastolicArterialPressureBaseline.isValid();}

  public SEScalarVolume getExpiratoryReserveVolume() 
  { 
    if(this.expiratoryReserveVolume==null)
      this.expiratoryReserveVolume=new SEScalarVolume();
    return this.expiratoryReserveVolume;
  }
  public boolean  hasExpiratoryReserveVolume() {return this.expiratoryReserveVolume==null?false:this.expiratoryReserveVolume.isValid();}

  public SEScalarVolume getFunctionalResidualCapacity() 
  { 
    if(this.functionalResidualCapacity==null)
      this.functionalResidualCapacity=new SEScalarVolume();
    return this.functionalResidualCapacity;
  }
  public boolean        hasFunctionalResidualCapacity() {return this.functionalResidualCapacity==null?false:this.functionalResidualCapacity.isValid();}

  public SEScalarFrequency getHeartRateBaseline() 
  { 
    if(this.heartRateBaseline==null)
      this.heartRateBaseline=new SEScalarFrequency();
    return this.heartRateBaseline;
  }
  public boolean           hasHeartRateBaseline() {return this.heartRateBaseline==null?false:this.heartRateBaseline.isValid();}


  public SEScalarFrequency getHeartRateMaximum() 
  { 
    if(this.heartRateMaximum==null)
      this.heartRateMaximum=new SEScalarFrequency();
    return this.heartRateMaximum;
  }
  public boolean           hasHeartRateMaximum() {return this.heartRateMaximum==null?false:this.heartRateMaximum.isValid();}


  public SEScalarFrequency getHeartRateMinimum() 
  { 
    if(this.heartRateMinimum==null)
      this.heartRateMinimum=new SEScalarFrequency();
    return this.heartRateMinimum;
  }
  public boolean           hasHeartRateMinimum() {return this.heartRateMinimum==null?false:this.heartRateMinimum.isValid();}

  public SEScalarVolume getInspiratoryCapacity() 
  { 
    if(this.inspiratoryCapacity==null)
      this.inspiratoryCapacity=new SEScalarVolume();
    return this.inspiratoryCapacity;
  }
  public boolean               hasInspiratoryCapacity() {return this.inspiratoryCapacity==null?false:this.inspiratoryCapacity.isValid();}

  public SEScalarVolume getInspiratoryReserveVolume() 
  { 
    if(this.inspiratoryReserveVolume==null)
      this.inspiratoryReserveVolume=new SEScalarVolume();
    return this.inspiratoryReserveVolume;
  }
  public boolean                hasInspiratoryReserveVolume() {return this.inspiratoryReserveVolume==null?false:this.inspiratoryReserveVolume.isValid();}
  
  public SEScalarMass getIdealBodyWeight() 
  { 
    if(this.idealBodyWeight==null)
      this.idealBodyWeight=new SEScalarMass();
    return this.idealBodyWeight;
  }
  public boolean      hasIdealBodyWeight() {return this.idealBodyWeight==null?false:this.idealBodyWeight.isValid();}

  public SEScalarMass getLeanBodyMass()
  {
    if(this.leanBodyMass==null)
      this.leanBodyMass=new SEScalarMass();
    return this.leanBodyMass;
  }
  public boolean                hasLeanBodyMass() {return this.leanBodyMass==null?false:this.leanBodyMass.isValid();}
  
  public SEScalarPressure getMeanArterialPressureBaseline() 
  { 
    if(this.meanArterialPressureBaseline==null)
      this.meanArterialPressureBaseline=new SEScalarPressure();
    return this.meanArterialPressureBaseline;
  }
  public boolean          hasMeanArterialPressureBaseline() {return this.meanArterialPressureBaseline==null?false:this.meanArterialPressureBaseline.isValid();}

  public SEScalarPressure getPulsePressureBaseline() 
  { 
    if(this.pulsePressureBaseline==null)
      this.pulsePressureBaseline=new SEScalarPressure();
    return this.pulsePressureBaseline;
  }
  public boolean          hasPulsePressureBaseline() {return this.pulsePressureBaseline==null?false:this.pulsePressureBaseline.isValid();}

  public SEScalarVolume getResidualVolume() 
  { 
    if(this.residualVolume==null)
      this.residualVolume=new SEScalarVolume();
    return this.residualVolume;
  }
  public boolean        hasResidualVolume() {return this.residualVolume==null?false:this.residualVolume.isValid();}

  public SEScalarFrequency getRespirationRateBaseline() 
  { 
    if(this.respirationRateBaseline==null)
      this.respirationRateBaseline=new SEScalarFrequency();
    return this.respirationRateBaseline;
  }
  public boolean           hasRespirationRateBaseline() {return this.respirationRateBaseline==null?false:this.respirationRateBaseline.isValid();}

  public SEScalar0To1 getRightLungRatio() 
  { 
    if(this.rightLungRatio==null)
      this.rightLungRatio=new SEScalar0To1();
    return this.rightLungRatio;
  }
  public boolean          hasRightLungRatio() {return this.rightLungRatio==null?false:this.rightLungRatio.isValid();}
  
  public SEScalarArea getSkinSurfaceArea() 
  { 
    if(this.skinSurfaceArea==null)
      this.skinSurfaceArea=new SEScalarArea();
    return this.skinSurfaceArea;
  }
  public boolean          hasSkinSurfaceArea() {return this.skinSurfaceArea==null?false:this.skinSurfaceArea.isValid();}

  public SEScalarPressure getSystolicArterialPressureBaseline() 
  { 
    if(this.systolicArterialPressureBaseline==null)
      this.systolicArterialPressureBaseline=new SEScalarPressure();
    return this.systolicArterialPressureBaseline;
  }
  public boolean          hasSystolicArterialPressureBaseline() {return this.systolicArterialPressureBaseline==null?false:this.systolicArterialPressureBaseline.isValid();}  

  public SEScalarVolume getTidalVolumeBaseline() 
  { 
    if(this.tidalVolumeBaseline==null)
      this.tidalVolumeBaseline=new SEScalarVolume();
    return this.tidalVolumeBaseline;
  }
  public boolean        hasTidalVolumeBaseline() {return this.tidalVolumeBaseline==null?false:this.tidalVolumeBaseline.isValid();}

  public SEScalarVolume getTotalLungCapacity() 
  { 
    if(this.totalLungCapacity==null)
      this.totalLungCapacity=new SEScalarVolume();
    return this.totalLungCapacity;
  }
  public boolean        hasTotalLungCapacity() {return this.totalLungCapacity==null?false:this.totalLungCapacity.isValid();}

  public SEScalarVolume getBloodVolumeBaseline() 
  { 
    if(this.bloodVolumeBaseline==null)
      this.bloodVolumeBaseline=new SEScalarVolume();
    return this.bloodVolumeBaseline;
  }
  public boolean        hasBloodVolumeBaseline() {return this.bloodVolumeBaseline==null?false:this.bloodVolumeBaseline.isValid();}  

  public SEScalarVolume getVitalCapacity() 
  { 
    if(this.vitalCapacity==null)
      this.vitalCapacity=new SEScalarVolume();
    return this.vitalCapacity;
  }
  public boolean        hasVitalCapacity() {return this.vitalCapacity==null?false:this.vitalCapacity.isValid();} 

}
