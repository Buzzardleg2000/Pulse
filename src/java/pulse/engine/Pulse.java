/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.engine;

import java.util.*;

import pulse.cdm.bind.Events.eEvent;
import pulse.cdm.compartment.*;
import pulse.cdm.datarequests.*;
import pulse.cdm.engine.SEEventHandler;
import pulse.cdm.engine.SEEventManager;
import pulse.cdm.patient.SEPatient;
import pulse.cdm.patient.nutrition.SENutrition;
import pulse.cdm.properties.*;
import pulse.cdm.properties.CommonUnits.TimeUnit;
import pulse.cdm.substance.SESubstance;
import pulse.cdm.substance.SESubstanceTissuePharmacokinetics;
import pulse.cdm.substance.SESubstanceManager;
import pulse.cdm.system.SESystem;
import pulse.cdm.system.environment.*;
import pulse.cdm.system.equipment.anesthesia_machine.*;
import pulse.cdm.system.equipment.electrocardiogram.*;
import pulse.cdm.system.equipment.inhaler.*;
import pulse.cdm.system.physiology.*;
import pulse.utilities.jniBridge;
import pulse.utilities.Log;
import pulse.utilities.LogListener;
import pulse.utilities.Pair;

public class Pulse
{
  public enum SerializationType 
  { JSON(1), BINARY(0);
    private final int value;
    private SerializationType(int value) 
    {
      this.value = value;
    }
    public int value() 
    {
       return value;
    }}
  
  public SEScalarTime                     timeStep;
  public SEScalarTime                     currentTime;
  
  public SESubstanceManager               substanceManager;
  public SEPatient                        patient;
  // Physiology systems
  public SEBloodChemistrySystem           bloodChemistry;
  public SECardiovascularSystem           cardiovascular;
  public SEEndocrineSystem                endocrine;
  public SEEnergySystem                   energy;
  public SEGastrointestinalSystem         gastrointestinal;
  public SERenalSystem                    renal;
  public SENervousSystem                  nervous;
  public SERespiratorySystem              respiratory;
  public SETissueSystem                   tissue;
  public SEDrugSystem                     drug;
  // Environment 
  public SEEnvironment                    environment;
  // Anesthesia
  public SEAnesthesiaMachine              anesthesiaMachine;
  // ECG
  public SEElectroCardioGram              ecg;
  // Inhaler
  public SEInhaler                        inhaler;
  
  // Compartments
  public SECompartmentManager             compartments;
  
  protected CDMUpdatedCallback            cdmCallback;
  protected LogListener                   listener;
  protected SEEventManager                eventManager = new SEEventManager();
  
  protected long nativeObj;
  protected synchronized native long nativeAllocate();
  protected synchronized native void nativeDelete(long nativeObj);
  protected synchronized native void nativeLogToConsole(long nativeObj, boolean b);
  protected synchronized native void nativeForwardLogMessages(long nativeObj, boolean b);
  protected synchronized native void nativeSetLogFilename(long nativeObj, String logFilename);

  protected native void    nativeForwardEvents(long nativeObj, boolean b);
  
  protected List<Pair<SEDataRequest,SEScalar>> dataRequests = new ArrayList<Pair<SEDataRequest,SEScalar>>();  
  
  static
  {
    jniBridge.initialize();
  }

  public Pulse()
  {
    this.nativeObj=0;
  }

  public void finalize()
  {
    this.cleanUp();
  }
  
  /**
   * Note this does not reset listeners and callback pointers
   */
  public void reset()
  {
    if(timeStep==null)
    {
      timeStep = new SEScalarTime();
      currentTime = new SEScalarTime();
      
      substanceManager = new SESubstanceManager();
      patient = new SEPatient();
      // Physiology systems
      bloodChemistry = new SEBloodChemistrySystem();
      cardiovascular = new SECardiovascularSystem();
      endocrine = new SEEndocrineSystem();
      energy = new SEEnergySystem();
      gastrointestinal = new SEGastrointestinalSystem();
      renal = new SERenalSystem();
      nervous = new SENervousSystem();
      respiratory = new SERespiratorySystem();
      tissue = new SETissueSystem();
      drug = new SEDrugSystem();
      // Environment 
      environment = new SEEnvironment();
      // Anesthesia
      anesthesiaMachine = new SEAnesthesiaMachine();
      // ECG
      ecg = new SEElectroCardioGram();
      // Inhaler
      inhaler = new SEInhaler();
      
      compartments = new SECompartmentManager();
      
      dataRequests = new ArrayList<Pair<SEDataRequest,SEScalar>>();
    }
    currentTime.invalidate();
    substanceManager.loadSubstanceDirectory();
    patient.reset();
    bloodChemistry.reset();
    cardiovascular.reset();
    endocrine.reset();
    energy.reset();
    gastrointestinal.reset();
    renal.reset();
    nervous.reset();
    respiratory.reset();
    tissue.reset();
    drug.reset();
    environment.reset();
    anesthesiaMachine.reset();
    ecg.reset();
    inhaler.reset();
    compartments.clear();
    
    // I am not clearing these, end user manages these
    //cdmCallback=null;
    //listener = null;
    //eventManager = null;
  }

  public void cleanUp()
  {
    if(this.nativeObj!=0)
    {
      nativeDelete(this.nativeObj);
      this.nativeObj=0;
    }
    this.listener = null;
  }

  public void setListener(LogListener listener)
  {
    this.listener = listener;
  }
  
  public SEEventManager getEventManager()
  {
    return this.eventManager;
  }
  
  /**
   * At the end of every AdvanceTime call, C++
   * will push all the data requested from C++ to Java
   * If this is not null, this callback will be called
   * after all data has been set on the Java side.
   * @param cb
   */
  protected void setCDMCallback(CDMUpdatedCallback cb)
  {
    this.cdmCallback = cb;
  }
  /** 
   * C++ will call this method
   * @param type 0 = patient, 1 = anesthesia machine
   * @param event enumeration index
   * @param active if it's on or off
   */
  protected void handleEvent(int type, int event, boolean active, double time_s)
  {
    try
    {
      this.getEventManager().setEvent(eEvent.forNumber(event),active,new SEScalarTime(time_s,TimeUnit.s));
    }
    catch(Exception ex)
    {
      LogError("Processing event "+event, "handleEvent");
    }
  }
  
  public List<Pair<SEDataRequest,SEScalar>> getDataRequestPairs()
  {
    return Collections.unmodifiableList(this.dataRequests);
  }
  
  protected void updateCDM(double time_s, double[] data)
  {
    int i=0;
    currentTime.setValue(time_s,TimeUnit.s);    
    for(Pair<SEDataRequest,SEScalar> e : dataRequests)
    {
      e.getR().setValue(data[i++],e.getL().getUnit());
    }
    if(this.cdmCallback!=null)
      cdmCallback.update(time_s);
  }
  
  /**
   * You can provided a data request with out a unit
   * And the engine will update the request with what
   * ever it calculated the requested value in.
   * The method will be called to inform us what 
   * unit the data given to updateCDM. If the data 
   * request had a unit specified in the first place,
   * this will be the same, but if no unit was specified
   * in the data request, this information is needed
   * when putting the received value into is associated scalar.
   * @param units
   */
  protected void setCDMHeadings(String[] units)
  {
    int i=0;
    for(Pair<SEDataRequest,SEScalar> e : dataRequests)
    {
      // Rip out the units from the headings
      String unit = units[i++];
      int idx = unit.indexOf('(');
      if(idx > -1)
      {
        unit = unit.substring(idx+1, unit.length()-1);
        unit = unit.replace("_", " ");
        e.getL().setUnit(unit);
      }
      else
        e.getL().setUnit("");// Unitless
    }
  }
  
  // Process all the data requests, and map them to CDM scalars
  protected boolean requestData(SEDataRequestManager drMgr)
  {
    dataRequests.clear();
    
    // Gather our systems up into a list to make things easier
    List<SESystem> systems = new ArrayList<SESystem>();
    systems.add(bloodChemistry);
    systems.add(cardiovascular);
    systems.add(endocrine);
    systems.add(energy);
    systems.add(gastrointestinal);
    systems.add(renal);
    systems.add(nervous);
    systems.add(respiratory);
    systems.add(tissue);
    systems.add(drug);
    systems.add(energy);
    
    List<SESystem> equipment = new ArrayList<SESystem>();
    equipment.add(anesthesiaMachine);
    equipment.add(ecg);
    equipment.add(inhaler);
    
    for(SEDataRequest dr : drMgr.getRequestedData())
    {
      switch(dr.getCategory())
      {
        case Physiology:
        {
          SEScalar scalar = null;
          for (SESystem sys : systems)
          {
            scalar = SEScalar.getScalar(sys,dr.getPropertyName());
            if (scalar != null)
            {
              dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,scalar));
              break;
            }
            // Check to see if the request is for a child class
            // I could do this more generically with reflection...
            // I know there should be a dash
  
            if (sys instanceof SEGastrointestinalSystem && dr.getPropertyName().startsWith("StomachContents"))
            {
              String scalarNameWithoutStomach = dr.getPropertyName().substring(dr.getPropertyName().indexOf('-')+1);
              SENutrition nutrition = ((SEGastrointestinalSystem)sys).getStomachContents();
  
              scalar = SEScalar.getScalar(nutrition, scalarNameWithoutStomach);
              if (scalar != null)
              {
                dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,scalar));
                break;
              }
            }
  
            if (sys instanceof SEDrugSystem && dr.getPropertyName().startsWith("PupillaryResponse"))
            {
              String propertyName = dr.getPropertyName().substring(dr.getPropertyName().indexOf('-')+1);
              SEPupillaryResponse pupillaryResponse = ((SEDrugSystem)sys).getPupillaryResponse();
  
              scalar = SEScalar.getScalar(pupillaryResponse, propertyName);
              if (scalar != null)
              {
                dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,scalar));
                break;
              }
            }
  
            if (sys instanceof SENervousSystem && (dr.getPropertyName().startsWith("LeftEyePupillaryResponse") || dr.getPropertyName().startsWith("RightEyePupillaryResponse")))
            {
              String propertyName = dr.getPropertyName().substring(dr.getPropertyName().indexOf('-')+1);
  
  
              SEPupillaryResponse pupillaryResponse=null;
              if(dr.getPropertyName().startsWith("Left"))
                pupillaryResponse = ((SENervousSystem)sys).getLeftEyePupillaryResponse();
              else if(dr.getPropertyName().startsWith("Right"))
                pupillaryResponse = ((SENervousSystem)sys).getRightEyePupillaryResponse();
  
              scalar = SEScalar.getScalar(pupillaryResponse, propertyName);
              if (scalar != null)
              {
                dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,scalar));
                break;
              }
            }
          }
          if(scalar!=null)
            continue;
          Log.error("Unable to find system property " + dr.getPropertyName());
          return false;
        }
        case GasCompartment:
        {
          SEGasCompartment cmpt = compartments.GetGasCompartment(dr.getCompartmentName());        
          if(dr.hasSubstanceName())
          {
            SESubstance sub = this.substanceManager.getSubstance(dr.getSubstanceName());
            dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,SEScalar.getScalar(cmpt.getSubstanceQuantity(sub), dr.getPropertyName())));
            continue;
          }
          else
          {
            dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,SEScalar.getScalar(cmpt, dr.getPropertyName())));
            continue;
          }
        }
        case LiquidCompartment:
        {
          SELiquidCompartment cmpt = compartments.GetLiquidCompartment(dr.getCompartmentName());        
          if(dr.hasSubstanceName())
          {
            SESubstance sub = this.substanceManager.getSubstance(dr.getSubstanceName());
            dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,SEScalar.getScalar(cmpt.getSubstanceQuantity(sub), dr.getPropertyName())));
            continue;
          }
          else
          {
            dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,SEScalar.getScalar(cmpt, dr.getPropertyName())));
            continue;
          }
        }
        case ThermalCompartment:
        {
          SEThermalCompartment cmpt = compartments.GetThermalCompartment(dr.getCompartmentName());        
          dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,SEScalar.getScalar(cmpt, dr.getPropertyName())));
          continue;
        }      
        case TissueCompartment:
        {
          SETissueCompartment cmpt = compartments.GetTissueCompartment(dr.getCompartmentName()); 
          dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,SEScalar.getScalar(cmpt, dr.getPropertyName())));
          continue;
        }
        case Patient:
        {
          String name = dr.getPropertyName();
          if(dr.getPropertyName().startsWith("Patient"))
            name = dr.getPropertyName().substring(7);
          SEScalar scalar = SEScalar.getScalar(this.patient, name);
          if (scalar != null)
          {
            dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,scalar));
            continue;
          }
          Log.error("Unable to find patient property " + dr.getPropertyName());
          return false;
        }
        case Substance:
        {
          SESubstance sub = this.substanceManager.getSubstance(dr.getSubstanceName());
          if(dr.hasCompartmentName())
          {
            if(dr.getPropertyName().equals("PartitionCoefficient"))
            {
              SESubstanceTissuePharmacokinetics subTk = sub.getPK().getTissueKinetics(dr.getCompartmentName());
              dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,subTk.getPartitionCoefficient()));
              continue;
            }
            else
            {
              Log.error("Do not have a property " + dr.getPropertyName() +" associated with compartments on a substance");
              return false;
            }
          }
          else
          {
            SEScalar scalar = SEScalar.getScalar(sub, dr.getPropertyName());
            if (scalar != null)
            {
              dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,scalar));
              continue;
            }
          }
          Log.error("Unable to find substance property " + dr.getPropertyName());
          return false;
        }
        case Environment:
        {
          SEScalar scalar = SEScalar.getScalar(environment, dr.getPropertyName());
          if (scalar != null)
          {
            dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,scalar));
            continue;
          }
          Log.error("Unable to find environment property " + dr.getPropertyName());
          return false;
        }    
        case AnesthesiaMachine:
        {
          SEScalar scalar = SEScalar.getScalar(anesthesiaMachine, dr.getPropertyName());
          if (scalar != null)
          {
            dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,scalar));          
            continue;
          }
          Log.error("Unable to find anesthesia machine property " + dr.getPropertyName());
          return false;
        }   
        case ECG:
        {
          SEScalar scalar = SEScalar.getScalar(ecg, dr.getPropertyName());
          if (scalar != null)
          {
            dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,scalar));          
            continue;
          }
          Log.error("Unable to find ecg property " + dr.getPropertyName());
          return false;
        }   
        case Inhaler:
        {
          SEScalar scalar = SEScalar.getScalar(inhaler, dr.getPropertyName());
          if (scalar != null)
          {
            dataRequests.add(new Pair<SEDataRequest,SEScalar>(dr,scalar));          
            continue;
          }
          Log.error("Unable to find inhaler property " + dr.getPropertyName());
          return false;
        }   
        default:
        {
          Log.error("Unsupported data request type");
          return false;
        }
      }
    }
    
    // Make sure nothing is mapped to null
    for(Pair<SEDataRequest,SEScalar> p : dataRequests)
    {
      if(p.getL()==null || p.getR()==null)
      {
        Log.error("We have an invalid data request pair");
        return false;
      }
    }
    return true;
  }
  
//TODO I could make origin, the Scenario/engine name we are running
 protected void LogDebug(String msg, String origin)
 {
   if(this.listener!=null)
     this.listener.handleDebug(msg, origin);
 }
 protected void LogInfo(String msg, String origin)
 {
   if(this.listener!=null)
     this.listener.handleInfo(msg, origin);
 }
 protected void LogWarning(String msg, String origin)
 {
   if(this.listener!=null)
     this.listener.handleWarn(msg, origin);
 }
 protected void LogError(String msg, String origin)
 {
   if(this.listener!=null)
     this.listener.handleError(msg, origin);
 }
 protected void LogFatal(String msg, String origin)
 {
   if(this.listener!=null)
     this.listener.handleFatal(msg, origin);
 }
}
