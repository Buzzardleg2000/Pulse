/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.substance;


import com.kitware.pulse.cdm.bind.Substance.SubstanceFractionData;
import com.kitware.pulse.cdm.properties.SEScalar0To1;

public class SESubstanceFraction
{
  protected SEScalar0To1  amount;
  protected String        substance;
  
  public SESubstanceFraction(String s)
  {
    if(s==null)
      throw new RuntimeException("Must provide a valid substance");
    this.substance=s;
  }
  
  public void clear()
  {
    if(this.amount!=null)
      this.amount.invalidate();
  }
  
  public void copy(SESubstanceFraction from)
  {
    this.substance=from.substance;
    if(from.hasAmount())
      this.getAmount().set(from.getAmount());
  }
  
  public static void load(SubstanceFractionData src, SESubstanceFraction dst)
  {
    dst.clear();
    if(src.hasAmount())
      SEScalar0To1.load(src.getAmount(), dst.getAmount());
  }
  public static SubstanceFractionData unload(SESubstanceFraction src)
  {
    SubstanceFractionData.Builder dst = SubstanceFractionData.newBuilder();
    SESubstanceFraction.unload(src,dst);
    return dst.build();
  }
  protected static void unload(SESubstanceFraction src, SubstanceFractionData.Builder dst)
  {
    dst.setName(src.substance);
    if(src.hasAmount())
      dst.setAmount(SEScalar0To1.unload(src.amount)); 
  }
  
  public SEScalar0To1 getAmount() 
  { 
    if(this.amount==null)
      this.amount=new SEScalar0To1();
    return this.amount;
  }
  public boolean hasAmount() {return this.amount==null?false:this.amount.isValid();}
  
  public boolean hasSubstance() { return substance != null; }
  public String getSubstance() 
  { 
    return this.substance;
  }
  
  @Override
  public String toString()
  {
    if(!hasSubstance())
      return "";
    String str = "Amount of "+this.substance+": "+(hasAmount()?getAmount():"None");      
    return str;
  }
}
