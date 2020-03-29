/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package pulse.cdm.substance;


import java.util.Collection;
import java.util.HashMap;
import java.util.Map;

import pulse.cdm.bind.Substance.SubstanceData;
import pulse.cdm.bind.Substance.SubstancePhysicochemicalsData;
import pulse.cdm.bind.Substance.SubstancePhysicochemicalsData.*;
import pulse.cdm.properties.*;

public class SESubstancePhysicochemicals
{
  protected SEScalar         acidDissociationConstant;
  protected eBindingProtein  bindingProtien;
  protected SEScalar         bloodPlasmaRatio;
  protected SEScalar0To1     fractionUnboundInPlasma;
  protected eIonicState      ionicState;
  protected SEScalar         logP;
  protected SEScalar         oralAbsorptionRateConstant;  
    
  public SESubstancePhysicochemicals()
  {
    
  }
  
  public void reset()
  {
    if(this.acidDissociationConstant!=null)
      this.acidDissociationConstant.invalidate();
    this.bindingProtien = null;
    if(this.bloodPlasmaRatio!=null)
      this.bloodPlasmaRatio.invalidate();
    if(this.fractionUnboundInPlasma!=null)
      this.fractionUnboundInPlasma.invalidate();
    this.ionicState=null;
    if(this.logP!=null)
      this.logP.invalidate();  
    if(this.oralAbsorptionRateConstant!=null)
      this.oralAbsorptionRateConstant.invalidate();    
  }
  
  public boolean isValid()
  {    
    if(!hasAcidDissociationConstant() || 
       !hasBindingProtein() ||
       !hasBloodPlasmaRatio() ||
       !hasFractionUnboundInPlasma() ||
       !hasIonicState() ||
       !hasLogP())       
      return false;
    return true;
  }
  
  public static void load(SubstancePhysicochemicalsData src, SESubstancePhysicochemicals dst)
  {
    dst.reset();
    if(src.hasAcidDissociationConstant())
      SEScalar.load(src.getAcidDissociationConstant(),dst.getAcidDissociationConstant());
    if(src.getBindingProtein()!=eBindingProtein.NullBindingProtein)
      dst.setBindingProtein(src.getBindingProtein());
    if(src.hasBloodPlasmaRatio())
      SEScalar.load(src.getBloodPlasmaRatio(),dst.getBloodPlasmaRatio());
    if(src.hasFractionUnboundInPlasma())
      SEScalar0To1.load(src.getFractionUnboundInPlasma(),dst.getFractionUnboundInPlasma());
    if(src.getIonicState()!=eIonicState.NullIonicState)
      dst.setIonicState(src.getIonicState());
    if(src.hasLogP())
      SEScalar.load(src.getLogP(),dst.getLogP());
    if(src.hasOralAbsorptionRateConstant())
      SEScalar.load(src.getOralAbsorptionRateConstant(),dst.getOralAbsorptionRateConstant());      
  }
  public static SubstancePhysicochemicalsData unload(SESubstancePhysicochemicals src)
  {
    if(!src.isValid())
      return null;
    SubstancePhysicochemicalsData.Builder dst = SubstancePhysicochemicalsData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SESubstancePhysicochemicals src, SubstancePhysicochemicalsData.Builder dst)
  {
    if(src.hasAcidDissociationConstant())
      dst.setAcidDissociationConstant(SEScalar.unload(src.acidDissociationConstant));
    if(src.hasBindingProtein())
      dst.setBindingProtein(src.bindingProtien);
    if(src.hasBloodPlasmaRatio())
      dst.setBloodPlasmaRatio(SEScalar.unload(src.bloodPlasmaRatio));
    if(src.hasFractionUnboundInPlasma())
      dst.setFractionUnboundInPlasma(SEScalar0To1.unload(src.fractionUnboundInPlasma));
    if(src.hasIonicState())
      dst.setIonicState(src.ionicState);
    if(src.hasLogP())
      dst.setLogP(SEScalar.unload(src.logP));
    if(src.hasOralAbsorptionRateConstant())
      dst.setOralAbsorptionRateConstant(SEScalar.unload(src.oralAbsorptionRateConstant));    
  }
  
  public SEScalar getAcidDissociationConstant() 
  { 
    if(this.acidDissociationConstant==null)
      this.acidDissociationConstant=new SEScalar();
    return this.acidDissociationConstant;
  }
  public boolean hasAcidDissociationConstant() {return this.acidDissociationConstant==null?false:this.acidDissociationConstant.isValid();}
  
  public eBindingProtein getBindingProtein() { return this.bindingProtien;}
  public void            setBindingProtein(eBindingProtein protein){this.bindingProtien=protein;}
  public boolean         hasBindingProtein(){return this.bindingProtien==null?false:bindingProtien!=eBindingProtein.NullBindingProtein;}
  
  public SEScalar getBloodPlasmaRatio() 
  { 
    if(this.bloodPlasmaRatio==null)
      this.bloodPlasmaRatio=new SEScalar();
    return this.bloodPlasmaRatio;
  }
  public boolean hasBloodPlasmaRatio() {return this.bloodPlasmaRatio==null?false:this.bloodPlasmaRatio.isValid();}
  
  public SEScalar0To1 getFractionUnboundInPlasma() 
  { 
    if(this.fractionUnboundInPlasma==null)
      this.fractionUnboundInPlasma=new SEScalar0To1();
    return this.fractionUnboundInPlasma;
  }
  public boolean hasFractionUnboundInPlasma() {return this.fractionUnboundInPlasma==null?false:this.fractionUnboundInPlasma.isValid();}
  
  public eIonicState  getIonicState() { return this.ionicState;}
  public void         setIonicState(eIonicState state){this.ionicState=state;}
  public boolean      hasIonicState(){return this.ionicState==null?false:ionicState!=eIonicState.NullIonicState;}
  
  public SEScalar getLogP() 
  { 
    if(this.logP==null)
      this.logP=new SEScalar();
    return this.logP;
  }
  public boolean hasLogP() {return this.logP==null?false:this.logP.isValid();}
  
  
  public SEScalar getOralAbsorptionRateConstant() 
  { 
    if(this.oralAbsorptionRateConstant==null)
      this.oralAbsorptionRateConstant=new SEScalar();
    return this.oralAbsorptionRateConstant;
  }
  public boolean hasOralAbsorptionRateConstant() {return this.oralAbsorptionRateConstant==null?false:this.oralAbsorptionRateConstant.isValid();}
}
