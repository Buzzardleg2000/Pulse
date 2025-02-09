/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/patient/actions/SEChestOcclusiveDressing.h"
#include "cdm/io/protobuf/PBPatientActions.h"

SEChestOcclusiveDressing::SEChestOcclusiveDressing(Logger* logger) : SEPatientAction(logger)
{
  m_State = eSwitch::Off;
  m_Side= eSide::NullSide;
}

SEChestOcclusiveDressing::~SEChestOcclusiveDressing()
{
  m_State = eSwitch::Off;
  m_Side = eSide::NullSide;
}

void SEChestOcclusiveDressing::Clear()
{
  SEPatientAction::Clear();
  m_State= eSwitch::Off;
  m_Side= eSide::NullSide;
}

void SEChestOcclusiveDressing::Copy(const SEChestOcclusiveDressing& src, bool /*preserveState*/)
{
  //if(preserveState) // Cache any state before copy,
  PBPatientAction::Copy(src, *this);
  //if(preserveState) // Put back any state
}

bool SEChestOcclusiveDressing::IsValid() const
{
  return SEPatientAction::IsValid() && HasSide();
}

bool SEChestOcclusiveDressing::IsActive() const
{
  if (!SEPatientAction::IsActive())
    return false;
  return m_State == eSwitch::On;
}
void SEChestOcclusiveDressing::Deactivate()
{
  SEPatientAction::Deactivate();
  Clear();//No stateful properties
}

const SEScalar* SEChestOcclusiveDressing::GetScalar(const std::string& /*name*/)
{
  return nullptr;
}

eSide SEChestOcclusiveDressing::GetSide() const
{
  return m_Side;
}
void SEChestOcclusiveDressing::SetSide(eSide Side)
{
  m_Side = Side;
}
bool SEChestOcclusiveDressing::HasSide() const
{
  return m_Side== eSide::NullSide ?false:true;
}
void SEChestOcclusiveDressing::InvalidateSide()
{
  m_Side = eSide::NullSide;
}
