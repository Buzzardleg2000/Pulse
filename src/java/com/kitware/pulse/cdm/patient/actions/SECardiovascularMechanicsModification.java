/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

package com.kitware.pulse.cdm.patient.actions;

import com.kitware.pulse.cdm.bind.PatientActions.CardiovascularMechanicsModificationData;
import com.kitware.pulse.cdm.system.physiology.SECardiovascularMechanicsModifiers;

public class SECardiovascularMechanicsModification extends SEPatientAction
  {
    protected SECardiovascularMechanicsModifiers modifiers = null;
    protected String modifiersFile = "";
    protected boolean restabilize = true;

    public SECardiovascularMechanicsModification()
    {
      super();
    }

    public SECardiovascularMechanicsModification(SECardiovascularMechanicsModification other)
    {
      copy(other);
    }

    public void copy(SECardiovascularMechanicsModification other)
    {
      super.copy(other);
      if (other.modifiers != null)
        this.getModifiers().copy(other.modifiers);
      this.modifiersFile = other.modifiersFile;
      this.restabilize = other.restabilize;
    }

    public void clear()
    {
      super.clear();
      if (this.modifiers != null)
        this.modifiers.clear();
      this.modifiersFile = "";
      this.restabilize = true;
    }
    
    public static void load(CardiovascularMechanicsModificationData src, SECardiovascularMechanicsModification dst)
    {
      if (src.hasModifiers())
        SECardiovascularMechanicsModifiers.load(src.getModifiers(), dst.getModifiers());
      else if (src.getModifiersFile().isEmpty())
        dst.setModifiersFile(src.getModifiersFile());
      dst.restabilize = src.getRestabilize();
    }
    public static CardiovascularMechanicsModificationData unload(SECardiovascularMechanicsModification src)
    {
      CardiovascularMechanicsModificationData.Builder dst = CardiovascularMechanicsModificationData.newBuilder();
      unload(src,dst);
      return dst.build();
    }
    protected static void unload(SECardiovascularMechanicsModification src, CardiovascularMechanicsModificationData.Builder dst)
    {
      if(src.hasModifiers())
        dst.setModifiers(SECardiovascularMechanicsModifiers.unload(src.getModifiers()));
      else if (src.hasModifiersFile())
        dst.setModifiersFile(src.getModifiersFile());
      dst.setRestabilize(src.restabilize);
    }

    public boolean isValid()
    {
      return hasModifiers() || hasModifiersFile();
    }
    
    public boolean restabilize() { return this.restabilize; }
    void setRestabilization(boolean b) { this.restabilize = b; }

    public boolean hasModifiers()
    {
      return this.modifiers != null;
    }
    public SECardiovascularMechanicsModifiers getModifiers()
    {
      if (this.modifiers == null)
        this.modifiers = new SECardiovascularMechanicsModifiers();
      return this.modifiers;
    }

    public boolean hasModifiersFile()
    {
      return this.modifiersFile != null && !this.modifiersFile.isEmpty();
    }
    public String getModifiersFile()
    {
      return this.modifiersFile;
    }
    public void setModifiersFile(String s)
    {
      this.modifiersFile = s;
    }

    public String toString()
    {
      String str = "Cardiovascular Mechanics Modification";
      if (this.hasModifiersFile())
        str += "\n\tModifiers File: " + this.modifiersFile;
      else if (hasModifiers())
      {
        str += modifiers.toString();
      }
      str += "\n\tRestablize: " + this.restabilize;
      return str;
    }
  }
