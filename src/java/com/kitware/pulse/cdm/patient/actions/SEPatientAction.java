/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.actions.SEAction;
import com.kitware.pulse.cdm.bind.PatientActions.AnyPatientActionData;
import com.kitware.pulse.cdm.bind.PatientActions.PatientActionData;
import com.kitware.pulse.utilities.Log;

public abstract class SEPatientAction extends SEAction
{

  private static final long serialVersionUID = -28145404473115862L;

  public SEPatientAction()
  {
    
  }
  
  public void copy(SEPatientAction other)
  {
    super.copy(other);
  }
  
  @Override
  public void reset()
  {
    super.reset();
  }
  
  public static void load(PatientActionData src, SEPatientAction dst) 
  {
    SEAction.load(src.getAction(), dst);
  }
  protected static void unload(SEPatientAction src, PatientActionData.Builder dst)
  {
    SEAction.unload(src, dst.getActionBuilder());
  }
  
  public static SEPatientAction ANY2CDM(AnyPatientActionData c)
  {
    switch(c.getActionCase())
    {
    case ASSESSMENT:
    {
      SEPatientAssessmentRequest dst = new SEPatientAssessmentRequest();
      SEPatientAssessmentRequest.load(c.getAssessment(), dst);
      return dst;
    }
    case ACUTERESPIRATORYDISTRESSSYNDROMEEXACERBATION:
    {
      SEAcuteRespiratoryDistressSyndromeExacerbation dst = new SEAcuteRespiratoryDistressSyndromeExacerbation();
      SEAcuteRespiratoryDistressSyndromeExacerbation.load(c.getAcuteRespiratoryDistressSyndromeExacerbation(), dst);
      return dst;
    }
    case ACUTESTRESS:
    {
      SEAcuteStress dst = new SEAcuteStress();
      SEAcuteStress.load(c.getAcuteStress(), dst);
      return dst;
    }
    case AIRWAYOBSTRUCTION:
    {
      SEAirwayObstruction dst = new SEAirwayObstruction();
      SEAirwayObstruction.load(c.getAirwayObstruction(), dst);
      return dst;
    }
    case ASTHMAATTACK:
    {
      SEAsthmaAttack dst = new SEAsthmaAttack();
      SEAsthmaAttack.load(c.getAsthmaAttack(), dst);
      return dst;
    }
    case BRAININJURY:
    {
      SEBrainInjury dst = new SEBrainInjury();
      SEBrainInjury.load(c.getBrainInjury(), dst);
      return dst;
    }
    case BRONCHOCONSTRICTION:
    {
      SEBronchoconstriction dst = new SEBronchoconstriction();
      SEBronchoconstriction.load(c.getBronchoconstriction(), dst);
      return dst;
    }
    case CARDIACARREST:
    {
      SECardiacArrest dst = new SECardiacArrest();
      SECardiacArrest.load(c.getCardiacArrest(), dst);
      return dst;
    }
    case CHESTCOMPRESSIONFORCE:
    {
      SEChestCompressionForce dst = new SEChestCompressionForce();
      SEChestCompressionForce.load(c.getChestCompressionForce(), dst);
      return dst;
    }
    case CHESTCOMPRESSIONFORCESCALE:
    {
      SEChestCompressionForceScale dst = new SEChestCompressionForceScale();
      SEChestCompressionForceScale.load(c.getChestCompressionForceScale(), dst);
      return dst;
    }
    case CHESTOCCLUSIVEDRESSING:
    {
      SEChestOcclusiveDressing dst = new SEChestOcclusiveDressing();
      SEChestOcclusiveDressing.load(c.getChestOcclusiveDressing(), dst);
      return dst;
    }
    case CHRONICOBSTRUCTIVEPULMONARYDISEASEEXACERBATION:
    {
      SEChronicObstructivePulmonaryDiseaseExacerbation dst = new SEChronicObstructivePulmonaryDiseaseExacerbation();
      SEChronicObstructivePulmonaryDiseaseExacerbation.load(c.getChronicObstructivePulmonaryDiseaseExacerbation(), dst);
      return dst;
    }
    case CONSCIOUSRESPIRATION:
    {
      SEConsciousRespiration dst = new SEConsciousRespiration();
      SEConsciousRespiration.load(c.getConsciousRespiration(), dst);
      return dst;
    }
    case CONSUMENUTRIENTS:
    {
      SEConsumeNutrients dst = new SEConsumeNutrients();
      SEConsumeNutrients.load(c.getConsumeNutrients(), dst);;
      return dst;
    }
    case DYSPNEA:
    {
      SEDyspnea dst = new SEDyspnea();
      SEDyspnea.load(c.getDyspnea(), dst);
      return dst;
    }
    case EXERCISE:
    {
      SEExercise dst = new SEExercise();
      SEExercise.load(c.getExercise(), dst);
      return dst;
    }
    case HEMORRHAGE:
    {
      SEHemorrhage dst = new SEHemorrhage();
      SEHemorrhage.load(c.getHemorrhage(), dst);
      return dst;
    }
    case IMPAIREDALVEOLAREXCHANGEEXACERBATION:
    {
      SEImpairedAlveolarExchangeExacerbation dst = new SEImpairedAlveolarExchangeExacerbation();
      SEImpairedAlveolarExchangeExacerbation.load(c.getImpairedAlveolarExchangeExacerbation(), dst);
      return dst;
    }
    case INTUBATION:
    {
      SEIntubation dst = new SEIntubation();
      SEIntubation.load(c.getIntubation(), dst);
      return dst;
    }
    case LOBARPNEUMONIAEXACERBATION:
    {
      SELobarPneumoniaExacerbation dst = new SELobarPneumoniaExacerbation();
      SELobarPneumoniaExacerbation.load(c.getLobarPneumoniaExacerbation(), dst);
      return dst;
    }
    case MECHANICALVENTILATION:
    {
      SEMechanicalVentilation dst = new SEMechanicalVentilation();
      SEMechanicalVentilation.load(c.getMechanicalVentilation(), dst);
      return dst;
    }
    case NEEDLEDECOMPRESSION:
    {
      SENeedleDecompression dst = new SENeedleDecompression();
      SENeedleDecompression.load(c.getNeedleDecompression(), dst);
      return dst;
    }
    case PERICARDIALEFFUSION:
    {
      SEPericardialEffusion dst = new SEPericardialEffusion();
      SEPericardialEffusion.load(c.getPericardialEffusion(), dst);
      return dst;
    }
    case PULMONARYSHUNTEXACERBATION:
    {
      SEPulmonaryShuntExacerbation dst = new SEPulmonaryShuntExacerbation();
      SEPulmonaryShuntExacerbation.load(c.getPulmonaryShuntExacerbation(), dst);
      return dst;
      
    }
    case RESPIRATORYFATIGUE:
    {
      SERespiratoryFatigue dst = new SERespiratoryFatigue();
      SERespiratoryFatigue.load(c.getRespiratoryFatigue(), dst);
      return dst;
    }
    case SUBSTANCEBOLUS:
    {
      SESubstanceBolus dst = new SESubstanceBolus();
      SESubstanceBolus.load(c.getSubstanceBolus(), dst);
      return dst;
    }
    case SUBSTANCECOMPOUNDINFUSION:
    {
      SESubstanceCompoundInfusion dst = new SESubstanceCompoundInfusion();
      SESubstanceCompoundInfusion.load(c.getSubstanceCompoundInfusion(), dst);
      return dst;
    }
    case SUBSTANCEINFUSION:
    {
      SESubstanceInfusion dst = new SESubstanceInfusion();
      SESubstanceInfusion.load(c.getSubstanceInfusion(), dst);
      return dst;
    }
    case SUPPLEMENTALOXYGEN:
    {
      SESupplementalOxygen dst = new SESupplementalOxygen();
      SESupplementalOxygen.load(c.getSupplementalOxygen(), dst);
      return dst;
    }
    case TENSIONPNEUMOTHORAX:
    {
      SETensionPneumothorax dst = new SETensionPneumothorax();
      SETensionPneumothorax.load(c.getTensionPneumothorax(), dst);
      return dst;
    }
    case URINATE:
    {
      SEUrinate dst = new SEUrinate();
      SEUrinate.load(c.getUrinate(), dst);
      return dst;
    }
    case ACTION_NOT_SET:
      Log.warn("AnyPatientActionData was empty...was that intended?");
      return null;
    }
    Log.error("Unsupported Patient Action type "+c.getActionCase());
    return null;
  }
  public static AnyPatientActionData CDM2ANY(SEPatientAction c)
  {
    AnyPatientActionData.Builder dst = AnyPatientActionData.newBuilder();
    if(c instanceof SEPatientAssessmentRequest)
    {
      dst.setAssessment(SEPatientAssessmentRequest.unload((SEPatientAssessmentRequest)c));
      return dst.build();
    }
    if(c instanceof SEAcuteRespiratoryDistressSyndromeExacerbation)
    {
      dst.setAcuteRespiratoryDistressSyndromeExacerbation(SEAcuteRespiratoryDistressSyndromeExacerbation.unload((SEAcuteRespiratoryDistressSyndromeExacerbation)c));
      return dst.build();
    }
    if(c instanceof SEAcuteStress)
    {
      dst.setAcuteStress(SEAcuteStress.unload((SEAcuteStress)c));
      return dst.build();
    }
    if(c instanceof SEAirwayObstruction)
    {
      dst.setAirwayObstruction(SEAirwayObstruction.unload((SEAirwayObstruction)c));
      return dst.build();
    }
    if(c instanceof SEAsthmaAttack)
    {
      dst.setAsthmaAttack(SEAsthmaAttack.unload((SEAsthmaAttack)c));
      return dst.build();
    }
    if(c instanceof SEBrainInjury)
    {
      dst.setBrainInjury(SEBrainInjury.unload((SEBrainInjury)c));
      return dst.build();
    }
    if(c instanceof SEBronchoconstriction)
    {
      dst.setBronchoconstriction(SEBronchoconstriction.unload((SEBronchoconstriction)c));
      return dst.build();
    }
    if(c instanceof SECardiacArrest)
    {
      dst.setCardiacArrest(SECardiacArrest.unload((SECardiacArrest)c));
      return dst.build();
    }
    if(c instanceof SEChestCompressionForce)
    {
      dst.setChestCompressionForce(SEChestCompressionForce.unload((SEChestCompressionForce)c));
      return dst.build();
    }
    if(c instanceof SEChestCompressionForceScale)
    {
      dst.setChestCompressionForceScale(SEChestCompressionForceScale.unload((SEChestCompressionForceScale)c));
      return dst.build();
    }
    if(c instanceof SEChestOcclusiveDressing)
    {
      dst.setChestOcclusiveDressing(SEChestOcclusiveDressing.unload((SEChestOcclusiveDressing)c));
      return dst.build();
    }
    if(c instanceof SEChronicObstructivePulmonaryDiseaseExacerbation)
    {
      dst.setChronicObstructivePulmonaryDiseaseExacerbation(SEChronicObstructivePulmonaryDiseaseExacerbation.unload((SEChronicObstructivePulmonaryDiseaseExacerbation)c));
      return dst.build();
    }
    if(c instanceof SEConsciousRespiration)
    {
      dst.setConsciousRespiration(SEConsciousRespiration.unload((SEConsciousRespiration)c));
      return dst.build();
    }
    if(c instanceof SEConsumeNutrients)
    {
      dst.setConsumeNutrients(SEConsumeNutrients.unload((SEConsumeNutrients)c));
      return dst.build();
    }
    if(c instanceof SEDyspnea)
    {
      dst.setDyspnea(SEDyspnea.unload((SEDyspnea)c));
      return dst.build();
    }
    if(c instanceof SEExercise)
    {
      dst.setExercise(SEExercise.unload((SEExercise)c));
      return dst.build();
    }
    if(c instanceof SEHemorrhage)
    {
      dst.setHemorrhage(SEHemorrhage.unload((SEHemorrhage)c));
      return dst.build();
    }
    if(c instanceof SEImpairedAlveolarExchangeExacerbation)
    {
      dst.setImpairedAlveolarExchangeExacerbation(SEImpairedAlveolarExchangeExacerbation.unload((SEImpairedAlveolarExchangeExacerbation)c));
      return dst.build();
    }
    if(c instanceof SEIntubation)
    {
      dst.setIntubation(SEIntubation.unload((SEIntubation)c));
      return dst.build();
    }
    if(c instanceof SELobarPneumoniaExacerbation)
    {
      dst.setLobarPneumoniaExacerbation(SELobarPneumoniaExacerbation.unload((SELobarPneumoniaExacerbation)c));
      return dst.build();
    }
    if(c instanceof SEMechanicalVentilation)
    {
      dst.setMechanicalVentilation(SEMechanicalVentilation.unload((SEMechanicalVentilation)c));
      return dst.build();
    }
    if(c instanceof SENeedleDecompression)
    {
      dst.setNeedleDecompression(SENeedleDecompression.unload((SENeedleDecompression)c));
      return dst.build();
    }
    if(c instanceof SEPericardialEffusion)
    {
      dst.setPericardialEffusion(SEPericardialEffusion.unload((SEPericardialEffusion)c));
      return dst.build();
    }
    if(c instanceof SEPulmonaryShuntExacerbation)
    {
      dst.setPulmonaryShuntExacerbation(SEPulmonaryShuntExacerbation.unload((SEPulmonaryShuntExacerbation)c));
      return dst.build();
    }
    if(c instanceof SERespiratoryFatigue)
    {
      dst.setRespiratoryFatigue(SERespiratoryFatigue.unload((SERespiratoryFatigue)c));
      return dst.build();
    }
    if(c instanceof SESubstanceBolus)
    {
      dst.setSubstanceBolus(SESubstanceBolus.unload((SESubstanceBolus)c));
      return dst.build();
    }
    if(c instanceof SESubstanceCompoundInfusion)
    {
      dst.setSubstanceCompoundInfusion(SESubstanceCompoundInfusion.unload((SESubstanceCompoundInfusion)c));
      return dst.build();
    }
    if(c instanceof SESubstanceInfusion)
    {
      dst.setSubstanceInfusion(SESubstanceInfusion.unload((SESubstanceInfusion)c));
      return dst.build();
    }
    if(c instanceof SESupplementalOxygen)
    {
      dst.setSupplementalOxygen(SESupplementalOxygen.unload((SESupplementalOxygen)c));
      return dst.build();
    }
    if(c instanceof SETensionPneumothorax)
    {
      dst.setTensionPneumothorax(SETensionPneumothorax.unload((SETensionPneumothorax)c));
      return dst.build();
    }
    if(c instanceof SEUrinate)
    {
      dst.setUrinate(SEUrinate.unload((SEUrinate)c));
      return dst.build();
    }
    Log.error("Unsupported Patient Action type "+c);
    return dst.build();
  }
  
  @Override
  public abstract String toString();
}
