/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/patient/assessments/SEPatientAssessment.h"
class SEUrinalysisMicroscopic;

// Keep enums in sync with appropriate schema/cdm/PatienAssessmentEnums.proto file !!
enum class eUrinalysis_PresenceIndicator {
  NullPresence = 0,
  Positive,
  Negative
};
extern CDM_DECL const std::string& eUrinalysis_PresenceIndicator_Name(eUrinalysis_PresenceIndicator m);

// Keep enums in sync with appropriate schema/cdm/PatienAssessmentEnums.proto file !!
enum class eUrinalysis_ClarityIndicator {
  NullClarity = 0,
  Translucent,
  SlightlyCloudy,
  Cloudy,
  Turbid
};
extern CDM_DECL const std::string& eUrinalysis_ClarityIndicator_Name(eUrinalysis_ClarityIndicator m);

// Keep enums in sync with appropriate schema/cdm/PatienAssessmentEnums.proto file !!
enum class eUrinalysis_UrineColor {
  NullColor = 0,
  PaleYellow,
  Yellow,
  DarkYellow
};
extern CDM_DECL const std::string& eUrinalysis_UrineColor_Name(eUrinalysis_UrineColor m);


class CDM_DECL SEUrinalysis : public SEPatientAssessment
{
  friend class PBPatientAssessment;//friend the serialization class
public:

  SEUrinalysis(Logger* logger);
  virtual ~SEUrinalysis();

  void Clear() override;

  bool SerializeToString(std::string& output, eSerializationFormat m) const override;
  bool SerializeToFile(const std::string& filename) const override;

  virtual bool HasColor() const;
  virtual eUrinalysis_UrineColor GetColor() const;
  virtual void SetColor(eUrinalysis_UrineColor color);
  virtual void InvalidateColor();

  virtual bool HasAppearance() const;
  virtual eUrinalysis_ClarityIndicator GetAppearance() const;
  virtual void SetAppearance(eUrinalysis_ClarityIndicator c);
  virtual void InvalidateAppearance();

  virtual bool HasGlucose() const;
  virtual eUrinalysis_PresenceIndicator GetGlucose() const;
  virtual void SetGlucose(eUrinalysis_PresenceIndicator p);
  virtual void InvalidateGlucose();

  virtual bool HasKetone() const;
  virtual eUrinalysis_PresenceIndicator GetKetone() const;
  virtual void SetKetone(eUrinalysis_PresenceIndicator p);
  virtual void InvalidateKetone();
  
  virtual bool HasBilirubin() const;
  virtual SEScalar& GetBilirubin();
  virtual double GetBilirubin() const;

  virtual bool HasSpecificGravity() const;
  virtual SEScalar& GetSpecificGravity();
  virtual double GetSpecificGravity() const;

  virtual bool HasBlood() const;
  virtual eUrinalysis_PresenceIndicator GetBlood() const;
  virtual void SetBlood(eUrinalysis_PresenceIndicator p);
  virtual void InvalidateBlood();

  virtual bool HasPH() const;
  virtual SEScalar& GetPH();
  virtual double GetPH() const;

  virtual bool HasProtein() const;
  virtual eUrinalysis_PresenceIndicator GetProtein() const;
  virtual void SetProtein(eUrinalysis_PresenceIndicator p);
  virtual void InvalidateProtein();

  virtual bool HasUrobilinogen() const;
  virtual SEScalarMassPerVolume& GetUrobilinogen();
  virtual double GetUrobilinogen(const MassPerVolumeUnit& unit) const;

  virtual bool HasNitrite() const;
  virtual eUrinalysis_PresenceIndicator GetNitrite() const;
  virtual void SetNitrite(eUrinalysis_PresenceIndicator p);
  virtual void InvalidateNitrite();

  virtual bool HasLeukocyteEsterase() const;
  virtual eUrinalysis_PresenceIndicator GetLeukocyteEsterase() const;
  virtual void SetLeukocyteEsterase(eUrinalysis_PresenceIndicator p);
  virtual void InvalidateLeukocyteEsterase();

  virtual bool HasMicroscopic() const;
  virtual SEUrinalysisMicroscopic& GetMicroscopic();
  virtual const SEUrinalysisMicroscopic* GetMicroscopic() const;
  virtual void RemoveMicroscopic();
  
protected:

  eUrinalysis_UrineColor        m_Color;
  eUrinalysis_ClarityIndicator  m_Appearance;
  eUrinalysis_PresenceIndicator m_Glucose;
  eUrinalysis_PresenceIndicator m_Ketone;
  SEScalar*                     m_Bilirubin;
  SEScalar*                     m_SpecificGravity;
  eUrinalysis_PresenceIndicator m_Blood;
  SEScalar*                     m_pH;
  eUrinalysis_PresenceIndicator m_Protein;
  SEScalarMassPerVolume*        m_Urobilinogen;
  eUrinalysis_PresenceIndicator m_Nitrite;
  eUrinalysis_PresenceIndicator m_LeukocyteEsterase;

  SEUrinalysisMicroscopic*      m_Microscopic;
};  