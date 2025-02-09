/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package com.kitware.pulse.cdm.patient.assessments;

import com.google.protobuf.InvalidProtocolBufferException;
import com.google.protobuf.util.JsonFormat;

import com.kitware.pulse.cdm.bind.PatientAssessments.CompleteBloodCountData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;
import com.kitware.pulse.cdm.properties.SEScalarAmountPerVolume;
import com.kitware.pulse.cdm.properties.SEScalarMassPerAmount;
import com.kitware.pulse.cdm.properties.SEScalarMassPerVolume;
import com.kitware.pulse.cdm.properties.SEScalarVolume;
import com.kitware.pulse.utilities.FileUtils;

public class SECompleteBloodCount extends SEPatientAssessment
{
  protected SEScalar0To1            hematocrit;
  protected SEScalarMassPerVolume   hemoglobin;
  protected SEScalarAmountPerVolume plateletCount;
  protected SEScalarMassPerAmount   meanCorpuscularHemoglobin;
  protected SEScalarMassPerVolume   meanCorpuscularHemoglobinConcentration;
  protected SEScalarVolume          meanCorpuscularVolume;
  protected SEScalarAmountPerVolume redBloodCellCount;
  protected SEScalarAmountPerVolume whiteBloodCellCount;
  
  @Override
  public void clear()
  {
    super.clear();
    this.hematocrit = null;
    this.hemoglobin = null;
    this.plateletCount = null;
    this.meanCorpuscularHemoglobin = null;
    this.meanCorpuscularHemoglobinConcentration = null;
    this.meanCorpuscularVolume = null;
    this.redBloodCellCount = null;
    this.whiteBloodCellCount = null;
  }
  
  public void readFile(String fileName) throws InvalidProtocolBufferException
  {
    CompleteBloodCountData.Builder builder = CompleteBloodCountData.newBuilder();
    JsonFormat.parser().merge(FileUtils.readFile(fileName), builder);
    SECompleteBloodCount.load(builder.build(), this);
  }
  public void writeFile(String fileName) throws InvalidProtocolBufferException
  {
    FileUtils.writeFile(fileName, JsonFormat.printer().print(SECompleteBloodCount.unload(this)));
  }
  
  public static void load(CompleteBloodCountData src, SECompleteBloodCount dst)
  {
    SEPatientAssessment.load(src.getPatientAssessment(), dst);
    if(src.hasHematocrit())
      SEScalar0To1.load(src.getHematocrit(),dst.getHematocrit());
    if(src.hasHemoglobin())
      SEScalarMassPerVolume.load(src.getHemoglobin(),dst.getHemoglobin());
    if(src.hasPlateletCount())
      SEScalarAmountPerVolume.load(src.getPlateletCount(),dst.getPlateletCount());
    if(src.hasMeanCorpuscularHemoglobin())
      SEScalarMassPerAmount.load(src.getMeanCorpuscularHemoglobin(),dst.getMeanCorpuscularHemoglobin());
    if(src.hasMeanCorpuscularHemoglobinConcentration())
      SEScalarMassPerVolume.load(src.getMeanCorpuscularHemoglobinConcentration(),dst.getMeanCorpuscularHemoglobinConcentration());
    if(src.hasMeanCorpuscularVolume())
      SEScalarVolume.load(src.getMeanCorpuscularVolume(),dst.getMeanCorpuscularVolume());
    if(src.hasRedBloodCellCount())
      SEScalarAmountPerVolume.load(src.getRedBloodCellCount(),dst.getRedBloodCellCount());
    if(src.hasWhiteBloodCellCount())
      SEScalarAmountPerVolume.load(src.getWhiteBloodCellCount(),dst.getWhiteBloodCellCount());
  }
  
  public static CompleteBloodCountData unload(SECompleteBloodCount src)
  {
    CompleteBloodCountData.Builder dst = CompleteBloodCountData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  
  protected static void unload(SECompleteBloodCount src, CompleteBloodCountData.Builder dst)
  {
    SEPatientAssessment.unload(src, dst.getPatientAssessmentBuilder());
    if (src.hasHematocrit())
      dst.setHematocrit(SEScalar0To1.unload(src.getHematocrit()));
    if (src.hasHemoglobin())
      dst.setHemoglobin(SEScalarMassPerVolume.unload(src.getHemoglobin()));
    if (src.hasPlateletCount())
      dst.setPlateletCount(SEScalarAmountPerVolume.unload(src.getPlateletCount()));
    if (src.hasMeanCorpuscularHemoglobin())
      dst.setMeanCorpuscularHemoglobin(SEScalarMassPerAmount.unload(src.getMeanCorpuscularHemoglobin()));
    if (src.hasMeanCorpuscularHemoglobinConcentration())
      dst.setMeanCorpuscularHemoglobinConcentration(SEScalarMassPerVolume.unload(src.getMeanCorpuscularHemoglobinConcentration()));
    if (src.hasMeanCorpuscularVolume())
      dst.setMeanCorpuscularVolume(SEScalarVolume.unload(src.getMeanCorpuscularVolume()));
    if (src.hasRedBloodCellCount())
      dst.setRedBloodCellCount(SEScalarAmountPerVolume.unload(src.getRedBloodCellCount()));
    if (src.hasWhiteBloodCellCount())
      dst.setWhiteBloodCellCount(SEScalarAmountPerVolume.unload(src.getWhiteBloodCellCount()));
  }
  
  public boolean hasHematocrit()
  {
    return hematocrit == null ? false : hematocrit.isValid();
  }
  public SEScalar0To1 getHematocrit()
  {
    if (hematocrit == null)
      hematocrit = new SEScalar0To1();
    return hematocrit;
  }
  
  public boolean hasHemoglobin()
  {
    return hemoglobin == null ? false : hemoglobin.isValid();
  }
  public SEScalarMassPerVolume getHemoglobin()
  {
    if (hemoglobin == null)
      hemoglobin = new SEScalarMassPerVolume();
    return hemoglobin;
  }
  
  public boolean hasPlateletCount()
  {
    return plateletCount == null ? false : plateletCount.isValid();
  }
  public SEScalarAmountPerVolume getPlateletCount()
  {
    if (plateletCount == null)
      plateletCount = new SEScalarAmountPerVolume();
    return plateletCount;
  }
  
  public boolean hasMeanCorpuscularHemoglobin()
  {
    return meanCorpuscularHemoglobin == null ? false : meanCorpuscularHemoglobin.isValid();
  }
  public SEScalarMassPerAmount getMeanCorpuscularHemoglobin()
  {
    if (meanCorpuscularHemoglobin == null)
      meanCorpuscularHemoglobin = new SEScalarMassPerAmount();
    return meanCorpuscularHemoglobin;
  }
  
  public boolean hasMeanCorpuscularHemoglobinConcentration()
  {
    return meanCorpuscularHemoglobinConcentration == null ? false : meanCorpuscularHemoglobinConcentration.isValid();
  }
  public SEScalarMassPerVolume getMeanCorpuscularHemoglobinConcentration()
  {
    if (meanCorpuscularHemoglobinConcentration == null)
      meanCorpuscularHemoglobinConcentration = new SEScalarMassPerVolume();
    return meanCorpuscularHemoglobinConcentration;
  }
  
  public boolean hasMeanCorpuscularVolume()
  {
    return meanCorpuscularVolume == null ? false : meanCorpuscularVolume.isValid();
  }
  public SEScalarVolume getMeanCorpuscularVolume()
  {
    if (meanCorpuscularVolume == null)
      meanCorpuscularVolume = new SEScalarVolume();
    return meanCorpuscularVolume;
  }
  
  public boolean hasRedBloodCellCount()
  {
    return redBloodCellCount == null ? false : redBloodCellCount.isValid();
  }
  public SEScalarAmountPerVolume getRedBloodCellCount()
  {
    if (redBloodCellCount == null)
      redBloodCellCount = new SEScalarAmountPerVolume();
    return redBloodCellCount;
  }
  
  public boolean hasWhiteBloodCellCount()
  {
    return whiteBloodCellCount == null ? false : whiteBloodCellCount.isValid();
  }
  public SEScalarAmountPerVolume getWhiteBloodCellCount()
  {
    if (whiteBloodCellCount == null)
      whiteBloodCellCount = new SEScalarAmountPerVolume();
    return whiteBloodCellCount;
  }

}
