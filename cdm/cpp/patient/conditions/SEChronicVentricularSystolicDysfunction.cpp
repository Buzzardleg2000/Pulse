/**************************************************************************************
Copyright 2015 Applied Research Associates, Inc.
Licensed under the Apache License, Version 2.0 (the "License"); you may not use
this file except in compliance with the License. You may obtain a copy of the License
at:
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
**************************************************************************************/

#include "stdafx.h"
#include "patient/conditions/SEChronicVentricularSystolicDysfunction.h"

SEChronicVentricularSystolicDysfunction::SEChronicVentricularSystolicDysfunction() : SEChronicHeartFailure()
{
}

SEChronicVentricularSystolicDysfunction::~SEChronicVentricularSystolicDysfunction()
{
  Clear();
}

void SEChronicVentricularSystolicDysfunction::Clear()
{
  SEChronicHeartFailure::Clear();
}

bool SEChronicVentricularSystolicDysfunction::IsValid() const
{
  return SEChronicHeartFailure::IsValid();
}

void SEChronicVentricularSystolicDysfunction::Load(const cdm::ChronicVentricularSystolicDysfunctionData& src, SEChronicVentricularSystolicDysfunction& dst)
{
  SEChronicVentricularSystolicDysfunction::Serialize(src, dst);
}
void SEChronicVentricularSystolicDysfunction::Serialize(const cdm::ChronicVentricularSystolicDysfunctionData& src, SEChronicVentricularSystolicDysfunction& dst)
{
  dst.Clear();
}

cdm::ChronicVentricularSystolicDysfunctionData* SEChronicVentricularSystolicDysfunction::Unload(const SEChronicVentricularSystolicDysfunction& src)
{
  cdm::ChronicVentricularSystolicDysfunctionData* dst = new cdm::ChronicVentricularSystolicDysfunctionData();
  SEChronicVentricularSystolicDysfunction::Serialize(src, *dst);
  return dst;
}
void SEChronicVentricularSystolicDysfunction::Serialize(const SEChronicVentricularSystolicDysfunction& src, cdm::ChronicVentricularSystolicDysfunctionData& dst)
{

}

void SEChronicVentricularSystolicDysfunction::ToString(std::ostream &str) const
{
  str << "Patient Condition : Ventricular Systolic Dysfunction Heart Failure"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << std::flush;
}