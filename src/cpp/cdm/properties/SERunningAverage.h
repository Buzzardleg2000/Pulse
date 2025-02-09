/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/properties/SEProperty.h"

class CDM_DECL SERunningAverage : public SEProperty
{
  friend class PBProperty;//friend the serialization class
public:
  
  SERunningAverage();
  virtual ~SERunningAverage();

  void Invalidate() override;
  bool IsValid() const override;

  /** Add value to Sample list and return the new average */
  double Sample(double d);
  /** Return the average value */
  double Value();

  unsigned int NumSamples() { return m_NumSamples; }
  
protected:

  double          m_Sum;
  unsigned int    m_NumSamples;
};
