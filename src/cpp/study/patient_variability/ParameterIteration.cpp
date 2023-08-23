/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"

#include "ParameterIteration.h"

namespace pulse::study::patient_variability
{
  ParameterIteration::ParameterIteration()
  {
    m_SliceIdx = 0;
  }

  void ParameterIteration::SetMinMaxStep(double min, double max, double stepSize)
  {
    m_Values.clear();
    int idx;
    if (min == max)
      m_Values.push_back(max);
    else
    {
      int n = (int)((max - min) / stepSize);
      for (idx = 0; idx <= n; ++idx)
      {
        m_Values.push_back(min + stepSize * idx);
      }
      //if (includeUpper && m_Values[n] != m_UpperLimit)
      //  m_Values.push_back(m_UpperLimit);
    }
  }


  void ParameterIteration::SetValues(std::initializer_list<double> values, int sliceIdx)
  {
    m_Values.clear();
    for (double v : values)
      m_Values.push_back(v);
    m_SliceIdx = sliceIdx;
  }
}
