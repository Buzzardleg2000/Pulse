/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/
package pulse.cdm.substance.quantity;

import pulse.cdm.bind.SubstanceQuantity.LiquidSubstanceQuantityData;
import pulse.cdm.properties.CommonUnits.AmountPerVolumeUnit;
import pulse.cdm.properties.CommonUnits.MassPerAmountUnit;
import pulse.cdm.properties.CommonUnits.MassPerVolumeUnit;
import pulse.cdm.properties.SEScalar0To1;
import pulse.cdm.properties.SEScalarAmountPerVolume;
import pulse.cdm.properties.SEScalarMass;
import pulse.cdm.properties.SEScalarMassPerVolume;
import pulse.cdm.properties.SEScalarPressure;
import pulse.cdm.substance.SESubstance;

public class SELiquidSubstanceQuantity extends SESubstanceQuantity
{
  protected SEScalarMassPerVolume   concentration;
  protected SEScalarMass            mass;
  protected SEScalarMass            massCleared;
  protected SEScalarMass            massDeposited;
  protected SEScalarMass            massExcreted;
  protected SEScalarAmountPerVolume molarity;
  protected SEScalarPressure        partialPressure;
  protected SEScalar0To1            saturation;

  public SELiquidSubstanceQuantity(SESubstance s)
  {
    super(s);
    concentration = null;
    mass = null;
    massCleared = null;
    massDeposited = null;
    massExcreted = null;
    molarity = null;
    partialPressure = null;
    saturation = null;
  }

  @Override
  public void reset()
  {
    super.reset();
    if (concentration != null)
      concentration.invalidate();
    if (mass != null)
      mass.invalidate();
    if(massCleared!=null)
      massCleared.invalidate();
    if(massDeposited!=null)
      massDeposited.invalidate();    
    if(massExcreted!=null)
      massExcreted.invalidate();    
    if (molarity != null)
      molarity.invalidate();
    if (partialPressure != null)
      partialPressure.invalidate();
    if (saturation != null)
      saturation.invalidate();
  }

  public static void load(LiquidSubstanceQuantityData src, SELiquidSubstanceQuantity dst)
  {
    SESubstanceQuantity.load(src.getSubstanceQuantity(),dst);
    if (src.hasConcentration()) 
      SEScalarMassPerVolume.load(src.getConcentration(),dst.getConcentration()); 
    if (src.hasMass()) 
      SEScalarMass.load(src.getMass(),dst.getMass()); 
    if (src.hasMassCleared())
      SEScalarMass.load(src.getMassCleared(),dst.getMassCleared());
    if (src.hasMassDeposited())
      SEScalarMass.load(src.getMassDeposited(),dst.getMassDeposited());
    if (src.hasMassExcreted())
      SEScalarMass.load(src.getMassExcreted(),dst.getMassExcreted());        
    if (src.hasMolarity())
    	SEScalarAmountPerVolume.load(src.getMolarity(),dst.getMolarity());        
    if (src.hasPartialPressure()) 
      SEScalarPressure.load(src.getPartialPressure(),dst.getPartialPressure()); 
    if (src.hasSaturation()) 
      SEScalar0To1.load(src.getSaturation(),dst.getSaturation());
  }
  public static LiquidSubstanceQuantityData unload(SELiquidSubstanceQuantity src)
  {
    LiquidSubstanceQuantityData.Builder dst = LiquidSubstanceQuantityData.newBuilder();
    unload(src,dst);
    return dst.build();
  }
  protected static void unload(SELiquidSubstanceQuantity src, LiquidSubstanceQuantityData.Builder dst)
  {
    SESubstanceQuantity.unload(src,dst.getSubstanceQuantityBuilder());
    if (src.hasConcentration())
      dst.setConcentration(SEScalarMassPerVolume.unload(src.concentration));
    if (src.hasMass())
      dst.setMass(SEScalarMass.unload(src.mass));
    if (src.hasMassCleared())
      dst.setMassCleared(SEScalarMass.unload(src.massCleared));
    if (src.hasMassDeposited())
      dst.setMassDeposited(SEScalarMass.unload(src.massDeposited));
    if (src.hasMassExcreted())
      dst.setMassExcreted(SEScalarMass.unload(src.massExcreted));    
    if (src.hasMolarity())
      dst.setMolarity(SEScalarAmountPerVolume.unload(src.molarity));    
    if (src.hasPartialPressure())
      dst.setPartialPressure(SEScalarPressure.unload(src.partialPressure));
    if (src.hasSaturation())
      dst.setSaturation(SEScalar0To1.unload(src.saturation));   
  }  

  public boolean hasConcentration()
  {
    return concentration == null ? false : concentration.isValid();
  }
  public SEScalarMassPerVolume getConcentration()
  {
    if (concentration == null)
      concentration = new SEScalarMassPerVolume();
    return concentration;
  }

  public boolean hasMass()
  {
    return mass == null ? false : mass.isValid();
  }
  public SEScalarMass getMass()
  {
    if (mass == null)
      mass = new SEScalarMass();
    return mass;
  }
  
  public SEScalarMass getMassCleared() 
  { 
    if(this.massCleared==null)
      this.massCleared=new SEScalarMass();
    return this.massCleared;
  }
  public boolean      hasMassCleared() {return this.massCleared==null?false:this.massCleared.isValid();}
  
  public SEScalarMass getMassDeposited() 
  { 
    if(this.massDeposited==null)
      this.massDeposited=new SEScalarMass();
    return this.massDeposited;
  }
  public boolean      hasMassDeposited() {return this.massDeposited==null?false:this.massDeposited.isValid();}
  
  public SEScalarMass getMassExcreted() 
  { 
    if(this.massExcreted==null)
      this.massExcreted=new SEScalarMass();
    return this.massExcreted;
  }
  public boolean      hasMassExcreted() {return this.massExcreted==null?false:this.massExcreted.isValid();}
   
  public boolean hasMolarity()
  {
    return molarity == null ? false : molarity.isValid();
  }
  public SEScalarAmountPerVolume getMolarity()
  {
    if (molarity == null)
      molarity = new SEScalarAmountPerVolume();
    molarity.setValue(getConcentration().getValue(MassPerVolumeUnit.ug_Per_mL)/substance.getMolarMass().getValue(MassPerAmountUnit.ug_Per_mol),AmountPerVolumeUnit.mol_Per_mL);
    return molarity;
  }

  public boolean hasPartialPressure()
  {
    return partialPressure == null ? false : partialPressure.isValid();
  }
  public SEScalarPressure getPartialPressure()
  {
    if (partialPressure == null)
      partialPressure = new SEScalarPressure();
    return partialPressure;
  }

  public boolean hasSaturation()
  {
    return saturation == null ? false : saturation.isValid();
  }
  public SEScalar0To1 getSaturation()
  {
    if (saturation == null)
      saturation = new SEScalar0To1();
    return saturation;
  }
}
