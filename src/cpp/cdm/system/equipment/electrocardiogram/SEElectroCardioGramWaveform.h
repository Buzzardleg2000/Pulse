/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once

// Keep enums in sync with appropriate schema/cdm/ElectroCardioGram.proto file !!
enum class eElectroCardioGram_WaveformType { Sinus = 0, 
                                             VentricularFibrillation,
                                             VentricularTachycardia };
extern CDM_DECL const std::string& eElectroCardioGram_WaveformType_Name(eElectroCardioGram_WaveformType m);

// Keep enums in sync with appropriate schema/cdm/CompartmentEnums.proto file !!
enum class eElectroCardioGram_WaveformLead { NullLead = 0,
                                             Lead1, 
                                             Lead2,
                                             Lead3,
                                             Lead4,
                                             Lead5,
                                             Lead6,
                                             Lead7,
                                             Lead8,
                                             Lead9,
                                             Lead10,
                                             Lead11,
                                             Lead12 };
extern CDM_DECL const std::string& eElectroCardioGram_WaveformLead_Name(eElectroCardioGram_WaveformLead m);


class CDM_DECL SEElectroCardioGramWaveform : public Loggable
{
  friend class PBElectroCardioGram;//friend the serialization class
public:

  SEElectroCardioGramWaveform(Logger* logger);
  virtual ~SEElectroCardioGramWaveform();

  virtual void Clear();// Deletes all members

  virtual bool HasLeadNumber() const;
  virtual eElectroCardioGram_WaveformLead  GetLeadNumber() const;
  virtual void SetLeadNumber(eElectroCardioGram_WaveformLead n);
  virtual void InvalidateLeadNumber();

  virtual eElectroCardioGram_WaveformType GetType() const;
  virtual void SetType(eElectroCardioGram_WaveformType t);

  virtual bool HasOriginalData() const;
  virtual SEArrayElectricPotential& GetOriginalData();
  virtual const SEArrayElectricPotential* GetOriginalData() const;

  virtual bool HasActiveCycle() const;
  virtual SEArrayElectricPotential& GetActiveCycle();
  virtual const SEArrayElectricPotential* GetActiveCycle() const;
  virtual void GenerateActiveCycle(const SEScalarFrequency& hr, const SEScalarTime& step, double amplitudeModifier);

  virtual size_t GetActiveIndex() { return m_ActiveIndex; }
  virtual void SetActiveIndex(size_t idx) { m_ActiveIndex = idx; }

  virtual void GetCycleValue(SEScalarElectricPotential& v, bool advance);

protected:

  eElectroCardioGram_WaveformLead          m_LeadNumber;
  eElectroCardioGram_WaveformType          m_Type;
  SEArrayElectricPotential*                m_OriginalData;
  SEArrayElectricPotential*                m_ActiveCycle;
  size_t                                   m_ActiveIndex;
  bool                                     m_Recycling;
};
