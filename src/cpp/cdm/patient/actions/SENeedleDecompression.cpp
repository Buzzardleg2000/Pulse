/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "stdafx.h"
#include "patient/actions/SENeedleDecompression.h"
#include "io/protobuf/PBPatientActions.h"

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

void SENeedleDecompression::Copy(const SENeedleDecompression& src)
{
  PBPatientAction::Copy(src, *this);
}

bool SENeedleDecompression::IsValid() const
{
  return SEPatientAction::IsValid() && HasSide();
}

bool SENeedleDecompression::IsActive() const
{
  return IsValid() && m_State == eSwitch::On;
}

void SENeedleDecompression::SetActive(bool b)
{
  m_State = b ? eSwitch::On : eSwitch::Off;
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

void SENeedleDecompression::ToString(std::ostream &str) const
{
  str << "Patient Action : Needle Decompression"; 
  if(HasComment())
    str<<"\n\tComment: "<<m_Comment;
  str << "\n\tState: " << eSwitch_Name(m_State);
  str  << "\n\tSide: "; HasSide()? str << eSide_Name(GetSide()) : str << "No Side Set";
  str << std::flush;
}