﻿/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

namespace Pulse.CDM
{
  public class SECardiovascularMechanicsModification : SEPatientAction
  {
    protected SECardiovascularMechanicsModifiers modifiers = null;
    protected string modifiersFile = "";
    protected bool restabilize = true;

    public SECardiovascularMechanicsModification()
    {

    }

    public SECardiovascularMechanicsModification(SECardiovascularMechanicsModification other)
    {
      Copy(other);
    }

    public void Copy(SECardiovascularMechanicsModification other)
    {
      base.Copy(other);
      if (other.modifiers != null)
        this.GetModifiers().Copy(other.modifiers);
      this.modifiersFile = other.modifiersFile;
      this.restabilize = other.restabilize;
    }

    public override void Clear()
    {
      base.Clear();
      if (this.modifiers != null)
        this.modifiers.Clear();
      this.modifiersFile = "";
      restabilize = true;
    }

    public override bool IsValid()
    {
      return HasModifiers() || HasModifiersFile();
    }

    public bool Restabilize() { return restabilize; }
    public void SetRestabilization(bool b) { restabilize = b; }

    public bool HasModifiers()
    {
      return this.modifiers != null;
    }
    public SECardiovascularMechanicsModifiers GetModifiers()
    {
      if (this.modifiers == null)
        this.modifiers = new SECardiovascularMechanicsModifiers();
      return this.modifiers;
    }

    public bool HasModifiersFile()
    {
      return !string.IsNullOrEmpty(this.modifiersFile);
    }
    public string GetModifiersFile()
    {
      return this.modifiersFile;
    }
    public void SetModificationFile(string s)
    {
      this.modifiersFile = s;
    }

    public override string ToString()
    {
      string str = "Cardiovascular Mechanics Modification";
      if (this.HasModifiersFile())
        str += "\n\tModifiers File: " + this.modifiersFile;
      else if (HasModifiers())
      {
        str += modifiers.ToString();
      }
      str += "\n\tRestabilize: " + this.restabilize;
      return str;
    }
  }
}
