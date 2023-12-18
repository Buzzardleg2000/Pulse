/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

enum class eDecimalFormat_Type { SystemFormatting = 0, DefaultFloat, FixedMantissa, SignificantDigits };
extern CDM_DECL const std::string& eDecimalFormat_Type_Name(eDecimalFormat_Type m);

class CDM_DECL SEDecimalFormat
{
  friend class PBEngine;//friend the serialization class
public:
  SEDecimalFormat(const SEDecimalFormat* dfault = nullptr);
  virtual ~SEDecimalFormat();

  virtual void Clear();
  virtual void Set(const SEDecimalFormat& f);

  void SetPrecision(std::streamsize p);
  std::streamsize GetPrecision();

  void SetNotation(eDecimalFormat_Type n);
  eDecimalFormat_Type GetNotation();

  void SetStream(std::ofstream& s);

protected:
  std::streamsize     m_Precision;
  eDecimalFormat_Type m_Notation;

};