/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

namespace pulse::study::patient_variability
{
  enum class eGenStyle
  {
    Combo = 0,
    Slice
  };

  class ParameterIteration
  {
  public:
    ParameterIteration();
    virtual ~ParameterIteration() = default;

    bool Empty() const { return m_Values.size() == 0; }
    void SetSliceIndex(size_t idx) { m_SliceIdx = idx; }
    void SetMinMaxStep(double min, double max, double step);
    void SetValues(std::initializer_list<double> values, int sliceIdx = 0);

    double GetSlice() const { return m_Values[m_SliceIdx]; }
    bool HasValues() const { return m_Values.size() > 0; }
    std::vector<double>& GetValues() { return m_Values; }
    const std::vector<double>& GetValues() const { return m_Values; }

  protected:
    size_t              m_SliceIdx;
    std::vector<double> m_Values;
  };
}
