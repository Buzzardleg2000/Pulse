/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SENeedleDecompression.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SENeedleDecompression::SENeedleDecompression(Logger* logger) : SEPatientAction(logger)
{
  m_State=eSwitch::Off;
  m_Side= eSide::NullSide;
}

SENeedleDecompression::~SENeedleDecompression()
{
  m_State = eSwitch::Off;
  m_Side = eSide::NullSide;
}

void SENeedleDecompression::Clear()
{
  SEPatientAction::Clear();
  m_State = eSwitch::Off;
  m_Side= eSide::NullSide;
}

void SENeedleDecompression::Copy(const SENeedleDecompression& src, bool /*preserveState*/)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SENeedleDecompression::IsValid() const
{
  return SEPatientAction::IsValid() && HasSide();
}

bool SENeedleDecompression::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return m_State == eSwitch::On;
}
void SENeedleDecompression::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SENeedleDecompression::GetScalar(const std::string& /*name*/)
{
  return nullptr;
}

eSwitch SENeedleDecompression::GetState() const
{
  return m_State;
}
void SENeedleDecompression::SetState(eSwitch state)
{
  m_State = (state == eSwitch::NullSwitch) ? eSwitch::Off : state;
}

eSide SENeedleDecompression::GetSide() const
{
  return m_Side;
}
void SENeedleDecompression::SetSide(eSide Side)
{
  m_Side = Side;
}
bool SENeedleDecompression::HasSide() const
{
  return m_Side== eSide::NullSide ?false:true;
}
void SENeedleDecompression::InvalidateSide()
{
  m_Side = eSide::NullSide;
}
