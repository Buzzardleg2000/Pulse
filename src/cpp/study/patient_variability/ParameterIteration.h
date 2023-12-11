/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

namespace pulse::study::patient_variability
{
  template <typename T>
  class ParameterIteration
  {
  public:
    ParameterIteration();
    virtual ~ParameterIteration() = default;

    bool Empty() const { return m_Values.size() == 0; }
    void SetSliceIndex(size_t idx) { m_SliceIdx = idx; }
    void SetValues(const std::vector<T>& values, int sliceIdx = 0);
    void SetValues(std::initializer_list<T> values, int sliceIdx = 0);

    static std::vector<double> SetMinMaxStep(double min, double max, double step);

    T GetSlice() const { return m_Values[m_SliceIdx]; }
    bool HasValues() const { return m_Values.size() > 0; }
    std::vector<T>& GetValues() { return m_Values; }
    const std::vector<T>& GetValues() const { return m_Values; }

  protected:
    size_t          m_SliceIdx;
    std::vector<T>  m_Values;
  };
}
