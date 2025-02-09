/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/system/equipment/SEEquipment.h"
#include "cdm/system/equipment/electrocardiogram/SEElectroCardioGramWaveform.h"

class CDM_DECL SEElectroCardioGram : public SEEquipment
{
  friend class PBElectroCardioGram;//friend the serialization class
protected:

public:

  SEElectroCardioGram(Logger* logger);
  virtual ~SEElectroCardioGram();

  void Clear() override;

  void Copy(const SEElectroCardioGram& src);

  const SEScalar* GetScalar(const std::string& name) override;

  bool SerializeToString(std::string& output, eSerializationFormat m) const;
  bool SerializeToFile(const std::string& filename) const;
  bool SerializeFromString(const std::string& src, eSerializationFormat m);
  bool SerializeFromFile(const std::string& filename);

  virtual bool HasLead1ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead1ElectricPotential();
  virtual double GetLead1ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead2ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead2ElectricPotential();
  virtual double GetLead2ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead3ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead3ElectricPotential();
  virtual double GetLead3ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead4ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead4ElectricPotential();
  virtual double GetLead4ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead5ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead5ElectricPotential();
  virtual double GetLead5ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead6ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead6ElectricPotential();
  virtual double GetLead6ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead7ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead7ElectricPotential();
  virtual double GetLead7ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead8ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead8ElectricPotential();
  virtual double GetLead8ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead9ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead9ElectricPotential();
  virtual double GetLead9ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead10ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead10ElectricPotential();
  virtual double GetLead10ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead11ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead11ElectricPotential();
  virtual double GetLead11ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasLead12ElectricPotential() const;
  virtual SEScalarElectricPotential& GetLead12ElectricPotential();
  virtual double GetLead12ElectricPotential(const ElectricPotentialUnit& unit) const;

  virtual bool HasWaveforms();
  virtual bool HasWaveform(eElectroCardioGram_WaveformLead, eElectroCardioGram_WaveformType);
  virtual SEElectroCardioGramWaveform& GetWaveform(eElectroCardioGram_WaveformLead, eElectroCardioGram_WaveformType);
  virtual const SEElectroCardioGramWaveform* GetWaveform(eElectroCardioGram_WaveformLead, eElectroCardioGram_WaveformType) const;

  virtual eElectroCardioGram_WaveformType GetActiveType() { return m_ActiveType; }
  virtual void ClearCycles();
  virtual void PullCycleValues();
  virtual void StartNewCycle(eElectroCardioGram_WaveformType t, const SEScalarFrequency& hr, const SEScalarTime& dt, double amplitudeModifier);

protected:
  virtual std::vector<SEElectroCardioGramWaveform*>& GetWaveforms();

  SEScalarElectricPotential* m_Lead1ElectricPotential;
  SEScalarElectricPotential* m_Lead2ElectricPotential;
  SEScalarElectricPotential* m_Lead3ElectricPotential;
  SEScalarElectricPotential* m_Lead4ElectricPotential;
  SEScalarElectricPotential* m_Lead5ElectricPotential;
  SEScalarElectricPotential* m_Lead6ElectricPotential;
  SEScalarElectricPotential* m_Lead7ElectricPotential;
  SEScalarElectricPotential* m_Lead8ElectricPotential;
  SEScalarElectricPotential* m_Lead9ElectricPotential;
  SEScalarElectricPotential* m_Lead10ElectricPotential;
  SEScalarElectricPotential* m_Lead11ElectricPotential;
  SEScalarElectricPotential* m_Lead12ElectricPotential;

  eElectroCardioGram_WaveformType           m_ActiveType;
  std::vector<SEElectroCardioGramWaveform*> m_Waveforms;
};