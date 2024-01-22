/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/assessments/SEPatientAssessment.h"

// Keep enums in sync with appropriate schema/cdm/PatienAssessmentEnums.proto file !!
enum class eUrinalysis_MicroscopicObservationType {
  NullObservationType = 0,
  LowPowerField,
  HighPowerField
};
extern CDM_DECL const std::string& eUrinalysis_MicroscopicObservationType_Name(eUrinalysis_MicroscopicObservationType m);

// Keep enums in sync with appropriate schema/cdm/PatienAssessmentEnums.proto file !!
enum class eUrinalysis_MicroscopicObservationAmount {
  NullObservationAmount = 0,
  Few,
  Moderate,
  Many
};
extern CDM_DECL const std::string& eUrinalysis_MicroscopicObservationAmount_Name(eUrinalysis_MicroscopicObservationAmount m);


class CDM_DECL SEUrinalysisMicroscopic : public Loggable
{
  friend class PBPatientAssessment;//friend the serialization class
  friend class SEUrinalysis;
protected:
  SEUrinalysisMicroscopic(Logger* logger);
public:
  virtual ~SEUrinalysisMicroscopic();

  virtual void Clear();

  virtual bool HasObservationType() const;
  virtual eUrinalysis_MicroscopicObservationType GetObservationType() const;
  virtual void SetObservationType(eUrinalysis_MicroscopicObservationType p);
  virtual void InvalidateObservationType();

  virtual bool HasRedBloodCells() const;
  virtual SEScalarAmount& GetRedBloodCells();
  virtual double GetRedBloodCells(const AmountUnit& unit) const;

  virtual bool HasWhiteBloodCells() const;
  virtual SEScalarAmount& GetWhiteBloodCells();
  virtual double GetWhiteBloodCells(const AmountUnit& unit) const;

  virtual bool HasEpithelialCells() const;
  virtual eUrinalysis_MicroscopicObservationAmount GetEpithelialCells() const;
  virtual void SetEpithelialCells(eUrinalysis_MicroscopicObservationAmount p);
  virtual void InvalidateEpithelialCells();

  virtual bool HasCasts() const;
  virtual SEScalarAmount& GetCasts();
  virtual double GetCasts(const AmountUnit& unit) const;

  virtual bool HasCrystals() const;
  virtual eUrinalysis_MicroscopicObservationAmount GetCrystals() const;
  virtual void SetCrystals(eUrinalysis_MicroscopicObservationAmount p);
  virtual void InvalidateCrystals();

  virtual bool HasBacteria() const;
  virtual eUrinalysis_MicroscopicObservationAmount GetBacteria() const;
  virtual void SetBacteria(eUrinalysis_MicroscopicObservationAmount p);
  virtual void InvalidateBacteria();

  virtual bool HasTrichomonads() const;
  virtual eUrinalysis_MicroscopicObservationAmount GetTrichomonads() const;
  virtual void SetTrichomonads(eUrinalysis_MicroscopicObservationAmount p);
  virtual void InvalidateTrichomonads();

  virtual bool HasYeast() const;
  virtual eUrinalysis_MicroscopicObservationAmount GetYeast() const;
  virtual void SetYeast(eUrinalysis_MicroscopicObservationAmount p);
  virtual void InvalidateYeast();

protected:

  eUrinalysis_MicroscopicObservationType   m_ObservationType;
  SEScalarAmount*                          m_RedBloodCells;
  SEScalarAmount*                          m_WhiteBloodCells;
  eUrinalysis_MicroscopicObservationAmount m_EpithelialCells;
  SEScalarAmount*                          m_Casts;
  eUrinalysis_MicroscopicObservationAmount m_Crystals;
  eUrinalysis_MicroscopicObservationAmount m_Bacteria;
  eUrinalysis_MicroscopicObservationAmount m_Trichomonads;
  eUrinalysis_MicroscopicObservationAmount m_Yeast;
};  