/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.cdm.system.equipment.mechanical_ventilator;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;

import pulse.cdm.bind.MechanicalVentilator.MechanicalVentilatorData;
import pulse.cdm.bind.MechanicalVentilator.MechanicalVentilatorData.eConnection;
import pulse.cdm.bind.MechanicalVentilator.MechanicalVentilatorData.eDriverWaveform;
import pulse.cdm.bind.Substance.SubstanceConcentrationData;
import pulse.cdm.bind.Substance.SubstanceData.eState;
import pulse.cdm.bind.Substance.SubstanceFractionData;
import pulse.cdm.properties.*;
import pulse.cdm.substance.SESubstanceConcentration;
import pulse.cdm.substance.SESubstanceFraction;
import pulse.cdm.system.equipment.SEEquipment;
import pulse.utilities.Log;

public class SEMechanicalVentilator extends SEEquipment
{
  protected eConnection                       connection;
  protected SEScalarPressureTimePerVolume     endotrachealTubeResistance;
  
  // Expiratory Baseline Properties (Only set 1)
  protected SEScalarPressure                  positiveEndExpiredPressure;
  protected SEScalarPressure                  functionalResidualCapacity;
  
  // Expriatory Cycle Properties (Only Set 1)
  protected SEScalarVolumePerTime             expirationCycleFlow;
  protected SEScalarPressure                  expirationCyclePressure;
  protected SEScalarTime                      expirationCycleTime;
  
  protected SEScalarPressureTimePerVolume     expirationTubeResistance;
  protected SEScalarPressureTimePerVolume     expirationValveResistance;
  protected eDriverWaveform                   expirationWaveform;

  // Inspiratory Limit Properties (Only set 1)
  protected SEScalarVolumePerTime             inspirationLimitFlow;
  protected SEScalarPressure                  inspirationLimitPressure;
  protected SEScalarVolume                    inspirationLimitVolume;
  

  protected SEScalarTime                      inspirationPauseTime;
  
  // Inspiratory Target Properties (Only set 1)
  protected SEScalarPressure                  peakInspiratoryPressure;
  protected SEScalarPressure                  endTidalCarbonDioxidePressure;
  
  // Inspiratory Trigger Properties (Only set 1)
  protected SEScalarVolumePerTime             inspirationTriggerFlow;
  protected SEScalarPressure                  inspirationTriggerPressure;
  protected SEScalarTime                      inspirationTriggerTime;

  protected SEScalarPressureTimePerVolume     inspirationTubeResistance;
  protected SEScalarPressureTimePerVolume     inspirationValveResistance;
  protected eDriverWaveform                   inspirationWaveform;
  
  protected List<SESubstanceFraction>         fractionInspiredGases;
  protected List<SESubstanceConcentration>    concentrationInspiredAerosol;


  public SEMechanicalVentilator()
  {
    connection = null;
    endotrachealTubeResistance = null;

    positiveEndExpiredPressure = null;
    functionalResidualCapacity = null;

    expirationCycleFlow = null;
    expirationCyclePressure = null;
    expirationCycleTime = null;

    expirationTubeResistance = null;
    expirationValveResistance = null;
    expirationWaveform = null;

    inspirationLimitFlow = null;
    inspirationLimitPressure = null;
    inspirationLimitVolume = null;

    inspirationPauseTime = null;

    peakInspiratoryPressure = null;
    endTidalCarbonDioxidePressure = null;

    inspirationTriggerFlow = null;
    inspirationTriggerPressure = null;
    inspirationTriggerTime = null;

    inspirationTubeResistance = null;
    inspirationValveResistance = null;
    inspirationWaveform = null;

    this.fractionInspiredGases=new ArrayList<SESubstanceFraction>();
    this.concentrationInspiredAerosol=new ArrayList<SESubstanceConcentration>();
  }

  @Override
  public void reset()
  {
    connection = null;
    if (endotrachealTubeResistance != null)
      endotrachealTubeResistance.invalidate();

    if (positiveEndExpiredPressure != null)
      positiveEndExpiredPressure.invalidate();
    if (functionalResidualCapacity != null)
      functionalResidualCapacity.invalidate();

    if (expirationCycleFlow != null)
      expirationCycleFlow.invalidate();
    if (expirationCyclePressure != null)
      expirationCyclePressure.invalidate();
    if (expirationCycleTime != null)
      expirationCycleTime.invalidate();

    if (expirationTubeResistance != null)
      expirationTubeResistance.invalidate();
    if (expirationValveResistance != null)
      expirationValveResistance.invalidate();
    expirationWaveform = null;

    if (inspirationLimitFlow != null)
      inspirationLimitFlow.invalidate();
    if (inspirationLimitPressure != null)
      inspirationLimitPressure.invalidate();
    if (inspirationLimitVolume != null)
      inspirationLimitVolume.invalidate();

    if (inspirationPauseTime != null)
      inspirationPauseTime.invalidate();

    if (peakInspiratoryPressure != null)
      peakInspiratoryPressure.invalidate();
    if (endTidalCarbonDioxidePressure != null)
      endTidalCarbonDioxidePressure.invalidate();

    if (inspirationTriggerFlow != null)
      inspirationTriggerFlow.invalidate();
    if (inspirationTriggerPressure != null)
      inspirationTriggerPressure.invalidate();
    if (inspirationTriggerTime != null)
      inspirationTriggerTime.invalidate();

    if (inspirationTubeResistance != null)
      inspirationTubeResistance.invalidate();
    if (inspirationValveResistance != null)
      inspirationValveResistance.invalidate();
    inspirationWaveform = null;
    
    this.fractionInspiredGases.clear();
    this.concentrationInspiredAerosol.clear();
  }

  public void copy(SEMechanicalVentilator from)
  {
    reset();
    if(from.connection!=null && from.connection != eConnection.NullConnection)
    	this.connection=from.connection;
    if(from.hasEndotrachealTubeResistance())
      this.getEndotrachealTubeResistance().set(from.getEndotrachealTubeResistance());
    
    if(from.hasPositiveEndExpiredPressure())
      this.getPositiveEndExpiredPressure().set(from.getPositiveEndExpiredPressure());
    if(from.hasFunctionalResidualCapacity())
      this.getFunctionalResidualCapacity().set(from.getFunctionalResidualCapacity());
    
    if(from.hasExpirationCycleFlow())
      this.getExpirationCycleFlow().set(from.getExpirationCycleFlow());
    if(from.hasExpirationCyclePressure())
      this.getExpirationCyclePressure().set(from.getExpirationCyclePressure());
    if(from.hasExpirationCycleTime())
      this.getExpirationCycleTime().set(from.getExpirationCycleTime());
    
    if(from.hasExpirationTubeResistance())
      this.getExpirationTubeResistance().set(from.getExpirationTubeResistance());
    if(from.hasExpirationValveResistance())
      this.getExpirationValveResistance().set(from.getExpirationValveResistance());
    if(from.expirationWaveform!=null && from.expirationWaveform != eDriverWaveform.NullDriverWaveform)
      this.expirationWaveform=from.expirationWaveform;
    
    if(from.hasInspirationLimitFlow())
      this.getInspirationLimitFlow().set(from.getInspirationLimitFlow());
    if(from.hasInspirationLimitPressure())
      this.getInspirationLimitPressure().set(from.getInspirationLimitPressure());
    if(from.hasInspirationLimitVolume())
      this.getInspirationLimitVolume().set(from.getInspirationLimitVolume());
    
    if(from.hasInspirationPauseTime())
      this.getInspirationPauseTime().set(from.getInspirationPauseTime());
    
    if(from.hasPeakInspiratoryPressure())
      this.getPeakInspiratoryPressure().set(from.getPeakInspiratoryPressure());
    if(from.hasEndTidalCarbonDioxidePressure())
      this.getEndTidalCarbonDioxidePressure().set(from.getEndTidalCarbonDioxidePressure());
    
    if(from.hasInspirationTriggerFlow())
      this.getInspirationTriggerFlow().set(from.getInspirationTriggerFlow());
    if(from.hasInspirationTriggerPressure())
      this.getInspirationTriggerPressure().set(from.getInspirationTriggerPressure());
    if(from.hasInspirationTriggerTime())
      this.getInspirationTriggerTime().set(from.getInspirationTriggerTime());
    
    if(from.hasInspirationTubeResistance())
      this.getInspirationTubeResistance().set(from.getInspirationTubeResistance());
    if(from.hasInspirationValveResistance())
      this.getInspirationValveResistance().set(from.getInspirationValveResistance());
    if(from.inspirationWaveform!=null && from.inspirationWaveform != eDriverWaveform.NullDriverWaveform)
      this.inspirationWaveform=from.inspirationWaveform;
    
    if(from.fractionInspiredGases!=null)
    {
      SESubstanceFraction mine;
      for(SESubstanceFraction sf : from.fractionInspiredGases)
      {
        mine=this.createFractionInspiredGas(sf.getSubstance());
        if(sf.hasAmount())
          mine.getAmount().set(sf.getAmount());
      }
    }
    if(from.concentrationInspiredAerosol!=null)
    {
      SESubstanceConcentration mine;
      for(SESubstanceConcentration sc : from.concentrationInspiredAerosol)
      {
        mine=this.createConcentrationInspiredAerosol(sc.getSubstance());
        if(sc.hasConcentration())
          mine.getConcentration().set(sc.getConcentration());
      }
    }
  }

  public static void load(MechanicalVentilatorData src, SEMechanicalVentilator dst)
  {
    dst.reset();
    if (src.getConnection()!=eConnection.UNRECOGNIZED)
      dst.setConnection(src.getConnection());
    
    if (src.hasPositiveEndExpiredPressure())
      SEScalarPressure.load(src.getPositiveEndExpiredPressure(), dst.getPositiveEndExpiredPressure());
    else if (src.hasFunctionalResidualCapacity())
      SEScalarPressure.load(src.getFunctionalResidualCapacity(), dst.getFunctionalResidualCapacity());
    
    if (src.hasExpirationCycleFlow())
      SEScalarVolumePerTime.load(src.getExpirationCycleFlow(), dst.getExpirationCycleFlow());
    else if (src.hasExpirationCyclePressure())
      SEScalarPressure.load(src.getExpirationCyclePressure(), dst.getExpirationCyclePressure());
    else if (src.hasExpirationCycleTime())
      SEScalarTime.load(src.getExpirationCycleTime(), dst.getExpirationCycleTime());
    
    if (src.hasExpirationTubeResistance())
      SEScalarPressureTimePerVolume.load(src.getExpirationTubeResistance(), dst.getExpirationTubeResistance());
    if (src.hasExpirationValveResistance())
      SEScalarPressureTimePerVolume.load(src.getExpirationValveResistance(), dst.getExpirationValveResistance());
    if (src.getExpirationWaveform()!=eDriverWaveform.UNRECOGNIZED)
      dst.setExpirationWaveform(src.getExpirationWaveform());
    
    if (src.hasInspirationLimitFlow())
      SEScalarVolumePerTime.load(src.getInspirationLimitFlow(), dst.getInspirationLimitFlow());
    else if (src.hasInspirationLimitPressure())
      SEScalarPressure.load(src.getInspirationLimitPressure(), dst.getInspirationLimitPressure());
    else if (src.hasInspirationLimitVolume())
      SEScalarVolume.load(src.getInspirationLimitVolume(), dst.getInspirationLimitVolume());
    
    if (src.hasInspirationPauseTime())
      SEScalarTime.load(src.getInspirationPauseTime(), dst.getInspirationPauseTime());
    
    if (src.hasPeakInspiratoryPressure())
      SEScalarPressure.load(src.getPeakInspiratoryPressure(), dst.getPeakInspiratoryPressure());
    else if (src.hasEndTidalCarbonDioxidePressure())
      SEScalarPressure.load(src.getEndTidalCarbonDioxidePressure(), dst.getEndTidalCarbonDioxidePressure());
    
    if (src.hasInspirationTriggerFlow())
      SEScalarVolumePerTime.load(src.getInspirationTriggerFlow(), dst.getInspirationTriggerFlow());
    else if (src.hasInspirationTriggerPressure())
      SEScalarPressure.load(src.getInspirationTriggerPressure(), dst.getInspirationTriggerPressure());
    else if (src.hasInspirationTriggerTime())
      SEScalarTime.load(src.getInspirationTriggerTime(), dst.getInspirationTriggerTime());
    
    if (src.hasInspirationTubeResistance())
      SEScalarPressureTimePerVolume.load(src.getInspirationTubeResistance(), dst.getInspirationTubeResistance());
    if (src.hasInspirationValveResistance())
      SEScalarPressureTimePerVolume.load(src.getInspirationValveResistance(), dst.getInspirationValveResistance());
    if (src.getInspirationWaveform()!=eDriverWaveform.UNRECOGNIZED)
      dst.setInspirationWaveform(src.getInspirationWaveform());
    
    if(src.getFractionInspiredGasList()!=null)
    {
      for(SubstanceFractionData subData : src.getFractionInspiredGasList())
      {
        SEScalar0To1.load(subData.getAmount(),dst.createFractionInspiredGas(subData.getName()).getAmount());
      }
    }
    
    if(src.getConcentrationInspiredAerosolList()!=null)
    {
      for(SubstanceConcentrationData subData : src.getConcentrationInspiredAerosolList())
      {
        SEScalarMassPerVolume.load(subData.getConcentration(),dst.createConcentrationInspiredAerosol(subData.getName()).getConcentration());
      }
    }
  }
  public static MechanicalVentilatorData unload(SEMechanicalVentilator src)
  {
    MechanicalVentilatorData.Builder dst = MechanicalVentilatorData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SEMechanicalVentilator src, MechanicalVentilatorData.Builder dst)
  {
    if (src.hasConnection())
      dst.setConnection(src.connection);
    
    if(src.hasPositiveEndExpiredPressure())
      dst.setPositiveEndExpiredPressure(SEScalarPressure.unload(src.getPositiveEndExpiredPressure()));
    else if (src.hasFunctionalResidualCapacity())
      dst.setFunctionalResidualCapacity(SEScalarPressure.unload(src.getFunctionalResidualCapacity()));
    
    if(src.hasExpirationCycleFlow())
      dst.setExpirationCycleFlow(SEScalarVolumePerTime.unload(src.getExpirationCycleFlow()));
    else if (src.hasExpirationCyclePressure())
      dst.setExpirationCyclePressure(SEScalarPressure.unload(src.getExpirationCyclePressure()));
    else if (src.hasExpirationCycleTime())
      dst.setExpirationCycleTime(SEScalarTime.unload(src.getExpirationCycleTime()));
    
    if(src.hasExpirationTubeResistance())
      dst.setExpirationTubeResistance(SEScalarPressureTimePerVolume.unload(src.getExpirationTubeResistance()));
    if(src.hasExpirationValveResistance())
      dst.setExpirationValveResistance(SEScalarPressureTimePerVolume.unload(src.getExpirationValveResistance()));
    if (src.hasExpirationWaveform())
      dst.setExpirationWaveform(src.expirationWaveform);
    
    if(src.hasInspirationLimitFlow())
      dst.setInspirationLimitFlow(SEScalarVolumePerTime.unload(src.getInspirationLimitFlow()));
    else if (src.hasInspirationLimitPressure())
      dst.setInspirationLimitPressure(SEScalarPressure.unload(src.getInspirationLimitPressure()));
    else if (src.hasInspirationLimitVolume())
      dst.setInspirationLimitVolume(SEScalarVolume.unload(src.getInspirationLimitVolume()));
    
    if(src.hasInspirationPauseTime())
      dst.setInspirationPauseTime(SEScalarTime.unload(src.getInspirationPauseTime()));
    
    if(src.hasPeakInspiratoryPressure())
      dst.setPeakInspiratoryPressure(SEScalarPressure.unload(src.getPeakInspiratoryPressure()));
    else if (src.hasEndTidalCarbonDioxidePressure())
      dst.setEndTidalCarbonDioxidePressure(SEScalarPressure.unload(src.getEndTidalCarbonDioxidePressure()));
    
    if(src.hasInspirationTriggerFlow())
      dst.setInspirationTriggerFlow(SEScalarVolumePerTime.unload(src.getInspirationTriggerFlow()));
    else if (src.hasInspirationTriggerPressure())
      dst.setInspirationTriggerPressure(SEScalarPressure.unload(src.getInspirationTriggerPressure()));
    else if (src.hasInspirationTriggerTime())
      dst.setInspirationTriggerTime(SEScalarTime.unload(src.getInspirationTriggerTime()));
    
    if(src.hasInspirationTubeResistance())
      dst.setInspirationTubeResistance(SEScalarPressureTimePerVolume.unload(src.getInspirationTubeResistance()));
    if(src.hasInspirationValveResistance())
      dst.setInspirationValveResistance(SEScalarPressureTimePerVolume.unload(src.getInspirationValveResistance()));
    if (src.hasInspirationWaveform())
      dst.setInspirationWaveform(src.inspirationWaveform);
    
    for(SESubstanceFraction ambSub : src.fractionInspiredGases)
      dst.addFractionInspiredGas(SESubstanceFraction.unload(ambSub));
    
    for(SESubstanceConcentration ambSub : src.concentrationInspiredAerosol)
      dst.addConcentrationInspiredAerosol(SESubstanceConcentration.unload(ambSub));
  }
  
  public eConnection getConnection()
  {
    return connection;
  }
  public void setConnection(eConnection c)
  {
    connection = (c == eConnection.UNRECOGNIZED) ? null : c;
  }
  public boolean hasConnection()
  {
    return connection != null;
  }
  
  public SEScalarPressureTimePerVolume getEndotrachealTubeResistance()
  {
    if (endotrachealTubeResistance == null)
      endotrachealTubeResistance = new SEScalarPressureTimePerVolume();
    return endotrachealTubeResistance;
  }
  public boolean hasEndotrachealTubeResistance()
  {
    return endotrachealTubeResistance == null ? false : endotrachealTubeResistance.isValid();
  }
  
  public SEScalarPressure getPositiveEndExpiredPressure()
  {
    if (positiveEndExpiredPressure == null)
      positiveEndExpiredPressure = new SEScalarPressure();
    return positiveEndExpiredPressure;
  }
  public boolean hasPositiveEndExpiredPressure()
  {
    return positiveEndExpiredPressure == null ? false : positiveEndExpiredPressure.isValid();
  }
  
  public SEScalarPressure getFunctionalResidualCapacity()
  {
    if (functionalResidualCapacity == null)
      functionalResidualCapacity = new SEScalarPressure();
    return functionalResidualCapacity;
  }public boolean hasFunctionalResidualCapacity()
  {
    return functionalResidualCapacity == null ? false : functionalResidualCapacity.isValid();
  }
  
  public SEScalarVolumePerTime getExpirationCycleFlow()
  {
    if (expirationCycleFlow == null)
      expirationCycleFlow = new SEScalarVolumePerTime();
    return expirationCycleFlow;
  }
  public boolean hasExpirationCycleFlow()
  {
    return expirationCycleFlow == null ? false : expirationCycleFlow.isValid();
  }
  
  public SEScalarPressure getExpirationCyclePressure()
  {
    if (expirationCyclePressure == null)
      expirationCyclePressure = new SEScalarPressure();
    return expirationCyclePressure;
  }
  public boolean hasExpirationCyclePressure()
  {
    return expirationCyclePressure == null ? false : expirationCyclePressure.isValid();
  }
  
  public SEScalarTime getExpirationCycleTime()
  {
    if (expirationCycleTime == null)
      expirationCycleTime = new SEScalarTime();
    return expirationCycleTime;
  }
  public boolean hasExpirationCycleTime()
  {
    return expirationCycleTime == null ? false : expirationCycleTime.isValid();
  }
  
  public SEScalarPressureTimePerVolume getExpirationTubeResistance()
  {
    if (expirationTubeResistance == null)
      expirationTubeResistance = new SEScalarPressureTimePerVolume();
    return expirationTubeResistance;
  }
  public boolean hasExpirationTubeResistance()
  {
    return expirationTubeResistance == null ? false : expirationTubeResistance.isValid();
  }
  
  public SEScalarPressureTimePerVolume getExpirationValveResistance()
  {
    if (expirationValveResistance == null)
      expirationValveResistance = new SEScalarPressureTimePerVolume();
    return expirationValveResistance;
  }
  public boolean hasExpirationValveResistance()
  {
    return expirationValveResistance == null ? false : expirationValveResistance.isValid();
  }
  
  public eDriverWaveform getExpirationWaveform()
  {
    return expirationWaveform;
  }
  public void setExpirationWaveform(eDriverWaveform w)
  {
    expirationWaveform = (w == eDriverWaveform.UNRECOGNIZED) ? null : w;
  }
  public boolean hasExpirationWaveform()
  {
    return expirationWaveform != null;
  }
  
  public SEScalarVolumePerTime getInspirationLimitFlow()
  {
    if (inspirationLimitFlow == null)
      inspirationLimitFlow = new SEScalarVolumePerTime();
    return inspirationLimitFlow;
  }
  public boolean hasInspirationLimitFlow()
  {
    return inspirationLimitFlow == null ? false : inspirationLimitFlow.isValid();
  }
  
  public SEScalarPressure getInspirationLimitPressure()
  {
    if (inspirationLimitPressure == null)
      inspirationLimitPressure = new SEScalarPressure();
    return inspirationLimitPressure;
  }
  public boolean hasInspirationLimitPressure()
  {
    return inspirationLimitPressure == null ? false : inspirationLimitPressure.isValid();
  }
  
  public SEScalarVolume getInspirationLimitVolume()
  {
    if (inspirationLimitVolume == null)
      inspirationLimitVolume = new SEScalarVolume();
    return inspirationLimitVolume;
  }
  public boolean hasInspirationLimitVolume()
  {
    return inspirationLimitVolume == null ? false : inspirationLimitVolume.isValid();
  }
  
  public SEScalarTime getInspirationPauseTime()
  {
    if (inspirationPauseTime == null)
      inspirationPauseTime = new SEScalarTime();
    return inspirationPauseTime;
  }
  public boolean hasInspirationPauseTime()
  {
    return inspirationPauseTime == null ? false : inspirationPauseTime.isValid();
  }

  public SEScalarPressure getPeakInspiratoryPressure()
  {
    if (peakInspiratoryPressure == null)
      peakInspiratoryPressure = new SEScalarPressure();
    return peakInspiratoryPressure;
  }
  public boolean hasPeakInspiratoryPressure()
  {
    return peakInspiratoryPressure == null ? false : peakInspiratoryPressure.isValid();
  }
  
  public SEScalarPressure getEndTidalCarbonDioxidePressure()
  {
    if (endTidalCarbonDioxidePressure == null)
      endTidalCarbonDioxidePressure = new SEScalarPressure();
    return endTidalCarbonDioxidePressure;
  }
  public boolean hasEndTidalCarbonDioxidePressure()
  {
    return endTidalCarbonDioxidePressure == null ? false : endTidalCarbonDioxidePressure.isValid();
  }
  
  public SEScalarVolumePerTime getInspirationTriggerFlow()
  {
    if (inspirationTriggerFlow == null)
      inspirationTriggerFlow = new SEScalarVolumePerTime();
    return inspirationTriggerFlow;
  }
  public boolean hasInspirationTriggerFlow()
  {
    return inspirationTriggerFlow == null ? false : inspirationTriggerFlow.isValid();
  }
  
  public SEScalarPressure getInspirationTriggerPressure()
  {
    if (inspirationTriggerPressure == null)
      inspirationTriggerPressure = new SEScalarPressure();
    return inspirationTriggerPressure;
  }
  public boolean hasInspirationTriggerPressure()
  {
    return inspirationTriggerPressure == null ? false : inspirationTriggerPressure.isValid();
  }
  
  public SEScalarTime getInspirationTriggerTime()
  {
    if (inspirationTriggerTime == null)
      inspirationTriggerTime = new SEScalarTime();
    return inspirationTriggerTime;
  }
  public boolean hasInspirationTriggerTime()
  {
    return inspirationTriggerTime == null ? false : inspirationTriggerTime.isValid();
  }
  
  public SEScalarPressureTimePerVolume getInspirationTubeResistance()
  {
    if (inspirationTubeResistance == null)
      inspirationTubeResistance = new SEScalarPressureTimePerVolume();
    return inspirationTubeResistance;
  }
  public boolean hasInspirationTubeResistance()
  {
    return inspirationTubeResistance == null ? false : inspirationTubeResistance.isValid();
  }
  
  public SEScalarPressureTimePerVolume getInspirationValveResistance()
  {
    if (inspirationValveResistance == null)
      inspirationValveResistance = new SEScalarPressureTimePerVolume();
    return inspirationValveResistance;
  }
  public boolean hasInspirationValveResistance()
  {
    return inspirationValveResistance == null ? false : inspirationValveResistance.isValid();
  }
  
  public eDriverWaveform getInspirationWaveform()
  {
    return inspirationWaveform;
  }
  public void setInspirationWaveform(eDriverWaveform w)
  {
    inspirationWaveform = (w == eDriverWaveform.UNRECOGNIZED) ? null : w;
  }
  public boolean hasInspirationWaveform()
  {
    return inspirationWaveform != null;
  }
  
  //////////////////////////////
  // Fraction Of Inspired Gas //
  //////////////////////////////
  
  public SESubstanceFraction createFractionInspiredGas(String substance)
  {
    return getFractionInspiredGas(substance);
  }
  public SESubstanceFraction getFractionInspiredGas(String substance)
  {
    for(SESubstanceFraction sf : this.fractionInspiredGases)
    {
      if(sf.getSubstance().equals(substance))
      {        
        return sf;
      }
    }    
    SESubstanceFraction sf = new SESubstanceFraction(substance);
    this.fractionInspiredGases.add(sf);
    return sf;
  }
  public boolean hasFractionInspiredGas()
  {
    return !this.fractionInspiredGases.isEmpty();
  }
  public boolean hasFractionInspiredGas(String substance)
  {
    for(SESubstanceFraction sf : this.fractionInspiredGases)
    {
      if(sf.getSubstance().equals(substance))
      {        
        return true;
      }
    }
    return false;
  }
  public List<SESubstanceFraction> getFractionInspiredGas()
  {
    return this.fractionInspiredGases;
  }
  public void removeFractionInspiredGas(String substance)
  {
    for(SESubstanceFraction sf : this.fractionInspiredGases)
    {
      if(sf.getSubstance().equals(substance))
      {
        this.fractionInspiredGases.remove(sf);
        return;
      }
    }  
  }
  

  public SESubstanceConcentration createConcentrationInspiredAerosol(String substance)
  {
    return getConcentrationInspiredAerosol(substance);
  }
  public SESubstanceConcentration getConcentrationInspiredAerosol(String substance)
  {
    for(SESubstanceConcentration sc : this.concentrationInspiredAerosol)
    {
      if(sc.getSubstance().equals(substance))
      {
        return sc;
      }
    }    
    SESubstanceConcentration sc = new SESubstanceConcentration(substance);
    this.concentrationInspiredAerosol.add(sc);
    return sc;
  }
  public boolean hasConcentrationInspiredAerosol()
  {
    return !this.concentrationInspiredAerosol.isEmpty();
  }
  public boolean hasConcentrationInspiredAerosol(String substance)
  {
    for(SESubstanceConcentration sc : this.concentrationInspiredAerosol)
    {
      if(sc.getSubstance().equals(substance))
      {
        return true;
      }
    }
    return false;
  }
  public List<SESubstanceConcentration> getConcentrationInspiredAerosol()
  {
    return Collections.unmodifiableList(this.concentrationInspiredAerosol);
  }
  public void removeConcentrationInspiredAerosol(String substance)
  {
    for(SESubstanceConcentration sc : this.concentrationInspiredAerosol)
    {
      if(sc.getSubstance().equals(substance))
      {
        this.concentrationInspiredAerosol.remove(sc);
        return;
      }
    }  
  }
  
  public void trim()
  {
    SESubstanceFraction sf;
    Iterator<SESubstanceFraction> ifrac = this.fractionInspiredGases.iterator();
    while(ifrac.hasNext())
    {
      sf=ifrac.next();
      if(sf.getAmount().getValue()==0)
        ifrac.remove();
    }    
  }

  @Override
  public String toString()
  {
    String str = "Mechanical Ventilator"
        + "\n\tConnection: " + (hasConnection()?getConnection():"NotProvided")
        
        + "\n\tEndotrachealTubeResistance: " + (hasEndotrachealTubeResistance()?getEndotrachealTubeResistance():"NotProvided")
        
        + "\n\tPositiveEndExpiredPressure: " + (hasPositiveEndExpiredPressure()?getPositiveEndExpiredPressure():"NotProvided")
        + "\n\tFunctionalResidualCapacity: " + (hasFunctionalResidualCapacity()?getFunctionalResidualCapacity():"NotProvided")
        
        + "\n\tExpirationCycleFlow: " + (hasExpirationCycleFlow()?getExpirationCycleFlow():"NotProvided")
        + "\n\tExpirationCyclePressure: " + (hasExpirationCyclePressure()?getExpirationCyclePressure():"NotProvided")
        + "\n\tExpirationCycleTime: " + (hasExpirationCycleTime()?getExpirationCycleTime():"NotProvided")
        
        + "\n\tExpirationTubeResistance: " + (hasExpirationTubeResistance()?getExpirationTubeResistance():"NotProvided")
        + "\n\tExpirationValveResistance: " + (hasExpirationValveResistance()?getExpirationValveResistance():"NotProvided")
        + "\n\tExpirationWaveform: " + (hasExpirationWaveform()?getExpirationWaveform():"NotProvided")
        
        + "\n\tInspirationLimitFlow: " + (hasInspirationLimitFlow()?getInspirationLimitFlow():"NotProvided")
        + "\n\tInspirationLimitPressure: " + (hasInspirationLimitPressure()?getInspirationLimitPressure():"NotProvided")
        + "\n\tInspirationLimitVolume: " + (hasInspirationLimitVolume()?getInspirationLimitVolume():"NotProvided")
        
        + "\n\tInspirationPauseTime: " + (hasInspirationPauseTime()?getInspirationPauseTime():"NotProvided")
        
        + "\n\tPeakInspiratoryPressure: " + (hasPeakInspiratoryPressure()?getPeakInspiratoryPressure():"NotProvided")
        + "\n\tEndTidalCarbonDioxidePressure: " + (hasEndTidalCarbonDioxidePressure()?getEndTidalCarbonDioxidePressure():"NotProvided")
        
        + "\n\tInspirationTriggerFlow: " + (hasInspirationTriggerFlow()?getInspirationTriggerFlow():"NotProvided")
        + "\n\tInspirationTriggerPressure: " + (hasInspirationTriggerPressure()?getInspirationTriggerPressure():"NotProvided")
        + "\n\tInspirationTriggerTime: " + (hasInspirationTriggerTime()?getInspirationTriggerTime():"NotProvided")
        
        + "\n\tInspirationTubeResistance: " + (hasInspirationTubeResistance()?getInspirationTubeResistance():"NotProvided")
        + "\n\tInspirationValveResistance: " + (hasInspirationValveResistance()?getInspirationValveResistance():"NotProvided")
        + "\n\tInspirationWaveform: " + (hasInspirationWaveform()?getInspirationWaveform():"NotProvided");
        
    for(SESubstanceFraction sf : this.fractionInspiredGases)
      str += "\n\t"+sf.toString();
    for(SESubstanceConcentration sc : this.concentrationInspiredAerosol)
    str += "\n\t"+sc.toString();
    return str;
  }
}