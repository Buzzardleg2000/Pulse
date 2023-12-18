/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "study/patient_variability/ParameterIteration.h"

namespace pulse::study::patient_variability
{
  template <typename T>
  ParameterIteration<T>::ParameterIteration()
  {
    m_SliceIdx = 0;
  }

  template <typename T>
  std::vector<double> ParameterIteration<T>::SetMinMaxStep(double min, double max, double stepSize)
  {
    std::vector<double> v;
    int idx;
    if (min == max)
      v.push_back(max);
    else
    {
      int n = (int)((max - min) / stepSize);
      for (idx = 0; idx <= n; ++idx)
      {
        v.push_back(min + stepSize * idx);
      }
    }

    return v;
  }

  template <typename T>
  void ParameterIteration<T>::SetValues(const std::vector<T>& values, int sliceIdx)
  {
    m_Values.clear();
    for (T v : values)
      m_Values.push_back(v);
    m_SliceIdx = sliceIdx;
  }

  template <typename T>
  void ParameterIteration<T>::SetValues(std::initializer_list<T> values, int sliceIdx)
  {
    std::vector<T> v(values);
    this->SetValues(v, sliceIdx);
  }

  template class ParameterIteration<size_t>;
  template class ParameterIteration<double>;
}
