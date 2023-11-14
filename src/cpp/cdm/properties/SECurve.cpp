/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/properties/SECurve.h"
#include "cdm/io/protobuf/PBProperties.h"

SECurve::SECurve()
{
  
}

SECurve::~SECurve()
{
  Invalidate();
}

void SECurve::Invalidate()
{
  DELETE_VECTOR(m_Segments);
}

bool SECurve::IsValid() const
{
  if (!HasSegment())
    return false;
  for (SESegment* s : m_Segments)
    if (!s->IsValid())
      return false;

  ///\TODO Make sure no segment gaps and anything else
  return true;
}

void SECurve::Copy(const SECurve& src)
{
  PBProperty::Copy(src, *this);
}

bool SECurve::HasSegment() const
{
  return !m_Segments.empty();
}
SESegmentConstant& SECurve::AddConstantSegment()
{
  SESegmentConstant* s = new SESegmentConstant();
  m_Segments.push_back(s);
  return *s;
}
SESegmentLinear& SECurve::AddLinearSegment()
{
  SESegmentLinear* s = new SESegmentLinear();
  m_Segments.push_back(s);
  return *s;
}
SESegmentParabolic& SECurve::AddParabolicSegment()
{
  SESegmentParabolic* s = new SESegmentParabolic();
  m_Segments.push_back(s);
  return *s;
}
SESegmentSigmoidal& SECurve::AddSigmoidalSegment()
{
  SESegmentSigmoidal* s = new SESegmentSigmoidal();
  m_Segments.push_back(s);
  return *s;
}
const std::vector<SESegment*>& SECurve::GetSegments()
{
  return m_Segments;
}
const std::vector<const SESegment*> SECurve::GetSegments() const
{
  return std::vector<const SESegment*>(m_Segments.begin(), m_Segments.end());
}

std::string SECurve::ToString() const
{
  std::string str = "Curve";
  for(SESegment* s : m_Segments)
    str += s->ToString();
  return str;
}
void SECurve::ToString(std::ostream& str) const
{
  str << ToString();
}
