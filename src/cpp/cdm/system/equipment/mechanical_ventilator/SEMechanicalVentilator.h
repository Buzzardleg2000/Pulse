/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "system/equipment/SEEquipment.h"
class SEEventHandler;
class SEMechanicalVentilatorConfiguration;
class SESubstance;
class SESubstanceFraction;
class SESubstanceConcentration;

// Keep enums in sync with appropriate schema/cdm/MechanicalVentilator.proto file !!
enum class eMechanicalVentilator_Connection { NullConnection = 0, Off, Mask, Tube };
extern const std::string& eMechanicalVentilator_Connection_Name(eMechanicalVentilator_Connection m);

// Keep enums in sync with appropriate schema/cdm/MechanicalVentilator.proto file !!
enum class eMechanicalVentilator_Control { NullControl = 0, PC_CMV };
extern const std::string& eMechanicalVentilator_Control_Name(eMechanicalVentilator_Control m);

// Keep enums in sync with appropriate schema/cdm/MechanicalVentilator.proto file !!
enum class eMechanicalVentilator_DriverWaveform { NullDriverWaveform = 0, Square };
extern const std::string& eMechanicalVentilator_DriverWaveform_Name(eMechanicalVentilator_DriverWaveform m);

class CDM_DECL SEMechanicalVentilator : public SEEquipment
{
  friend class PBMechanicalVentilator;//friend the serialization class
protected:
  friend SEMechanicalVentilatorConfiguration;
public:

  SEMechanicalVentilator(SESubstanceManager& substances);
  virtual ~SEMechanicalVentilator();

  virtual void Clear();

  bool SerializeToString(std::string& output, SerializationFormat m) const;
  bool SerializeToFile(const std::string& filename, SerializationFormat m) const;
  bool SerializeFromString(const std::string& src, SerializationFormat m);
  bool SerializeFromFile(const std::string& filename, SerializationFormat m);

protected:

  /** @name StateChange
  *   @brief - This method is called when ever there is a state change
  *            Specically a new file has been loaded, configuration action, or the system reset
  *            Engine specific methodology can then update their logic.
  */
  virtual void StateChange(){};
  virtual void Merge(const SEMechanicalVentilator& from);
  virtual void ProcessConfiguration(SEMechanicalVentilatorConfiguration& config);

public:

  virtual const SEScalar* GetScalar(const std::string& name);

  virtual eMechanicalVentilator_Connection GetConnection() const;
  virtual void SetConnection(eMechanicalVentilator_Connection c);

  virtual bool HasEndoTrachealTubeResistance() const;
  virtual SEScalarPressureTimePerVolume& GetEndoTrachealTubeResistance();
  virtual double GetEndoTrachealTubeResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasPositiveEndExpiredPressure() const;
  virtual SEScalarPressure& GetPositiveEndExpiredPressure();
  virtual double GetPositiveEndExpiredPressure(const PressureUnit& unit) const;

  virtual bool HasFunctionalResidualCapacity() const;
  virtual SEScalarPressure& GetFunctionalResidualCapacity();
  virtual double GetFunctionalResidualCapacity(const PressureUnit& unit) const;

  virtual bool HasExpirationCycleFlow() const;
  virtual SEScalarVolumePerTime& GetExpirationCycleFlow();
  virtual double GetExpirationCycleFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasExpirationCyclePressure() const;
  virtual SEScalarPressure& GetExpirationCyclePressure();
  virtual double GetExpirationCyclePressure(const PressureUnit& unit) const;

  virtual bool HasExpirationCycleTime() const;
  virtual SEScalarTime& GetExpirationCycleTime();
  virtual double GetExpirationCycleTime(const TimeUnit& unit) const;

  virtual bool HasExpirationTubeResistance() const;
  virtual SEScalarPressureTimePerVolume& GetExpirationTubeResistance();
  virtual double GetExpirationTubeResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasExpirationValveResistance() const;
  virtual SEScalarPressureTimePerVolume& GetExpirationValveResistance();
  virtual double GetExpirationValveResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual eMechanicalVentilator_DriverWaveform GetExpirationWaveform() const;
  virtual void SetExpirationWaveform(eMechanicalVentilator_DriverWaveform w);

  virtual bool HasInspirationLimitFlow() const;
  virtual SEScalarVolumePerTime& GetInspirationLimitFlow();
  virtual double GetInspirationLimitFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasInspirationLimitPressure() const;
  virtual SEScalarPressure& GetInspirationLimitPressure();
  virtual double GetInspirationLimitPressure(const PressureUnit& unit) const;

  virtual bool HasInspirationPauseTime() const;
  virtual SEScalarTime& GetInspirationPauseTime();
  virtual double GetInspirationPauseTime(const TimeUnit& unit) const;

  virtual bool HasPeakInspiratoryPressure() const;
  virtual SEScalarPressure& GetPeakInspiratoryPressure();
  virtual double GetPeakInspiratoryPressure(const PressureUnit& unit) const;

  virtual bool HasEndTidalCarbonDioxidePressure() const;
  virtual SEScalarPressure& GetEndTidalCarbonDioxidePressure();
  virtual double GetEndTidalCarbonDioxidePressure(const PressureUnit& unit) const;

  virtual bool HasInspirationLimitVolume() const;
  virtual SEScalarVolume& GetInspirationLimitVolume();
  virtual double GetInspirationLimitVolume(const VolumeUnit& unit) const;

  virtual bool HasInspirationTriggerFlow() const;
  virtual SEScalarVolumePerTime& GetInspirationTriggerFlow();
  virtual double GetInspirationTriggerFlow(const VolumePerTimeUnit& unit) const;

  virtual bool HasInspirationTriggerPressure() const;
  virtual SEScalarPressure& GetInspirationTriggerPressure();
  virtual double GetInspirationTriggerPressure(const PressureUnit& unit) const;

  virtual bool HasInspirationTriggerTime() const;
  virtual SEScalarTime& GetInspirationTriggerTime();
  virtual double GetInspirationTriggerTime(const TimeUnit& unit) const;

  virtual bool HasInspirationTubeResistance() const;
  virtual SEScalarPressureTimePerVolume& GetInspirationTubeResistance();
  virtual double GetInspirationTubeResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual bool HasInspirationValveResistance() const;
  virtual SEScalarPressureTimePerVolume& GetInspirationValveResistance();
  virtual double GetInspirationValveResistance(const PressureTimePerVolumeUnit& unit) const;

  virtual eMechanicalVentilator_DriverWaveform GetInspirationWaveform() const;
  virtual void SetInspirationWaveform(eMechanicalVentilator_DriverWaveform w);

  bool HasFractionInspiredGas() const;
  bool HasFractionInspiredGas(const SESubstance& substance) const;
  const std::vector<SESubstanceFraction*>& GetFractionInspiredGases();
  const std::vector<const SESubstanceFraction*>& GetFractionInspiredGases() const;
  SESubstanceFraction& GetFractionInspiredGas(SESubstance& substance);
  const SESubstanceFraction* GetFractionInspiredGas(const SESubstance& substance) const;
  void RemoveFractionInspiredGas(const SESubstance& substance);
  void RemoveFractionInspiredGases();

  bool HasConcentrationInspiredAerosol() const;
  bool HasConcentrationInspiredAerosol(const SESubstance& substance) const;
  const std::vector<SESubstanceConcentration*>& GetConcentrationInspiredAerosols();
  const std::vector<const SESubstanceConcentration*>& GetConcentrationInspiredAerosols() const;
  SESubstanceConcentration& GetConcentrationInspiredAerosol(SESubstance& substance);
  const SESubstanceConcentration* GetConcentrationInspiredAerosol(const SESubstance& substance) const;
  void RemoveConcentrationInspiredAerosol(const SESubstance& substance);
  void RemoveConcentrationInspiredAerosols();

protected:
  
  eMechanicalVentilator_Connection             m_Connection;
  SEScalarPressureTimePerVolume*               m_EndoTrachealTubeResistance;

  // Expiratory Baseline Properties (Only set 1)
  SEScalarPressure*                            m_PositiveEndExpiredPressure;
  SEScalarPressure*                            m_FunctionalResidualCapacity;

  // Expriatory Cycle Properties (Only Set 1)
  SEScalarVolumePerTime*                       m_ExpirationCycleFlow;
  SEScalarPressure*                            m_ExpirationCyclePressure;
  SEScalarTime*                                m_ExpirationCycleTime;

  SEScalarPressureTimePerVolume*               m_ExpirationTubeResistance;
  SEScalarPressureTimePerVolume*               m_ExpirationValveResistance;
  eMechanicalVentilator_DriverWaveform         m_ExpirationWaveform;

  // Inspiratory Limit Properties (Only set 1)
  SEScalarVolumePerTime*                       m_InspirationLimitFlow;
  SEScalarPressure*                            m_InspirationLimitPressure;
  SEScalarVolume*                              m_InspirationLimitVolume;

  SEScalarTime*                                m_InspirationPauseTime;

  // Inspiratory Target Properties (Only set 1)
  SEScalarPressure*                            m_PeakInspiratoryPressure;
  SEScalarPressure*                            m_EndTidalCarbonDioxidePressure;

  // Inspiratory Trigger Properties (Only set 1)
  SEScalarVolumePerTime*                       m_InspirationTriggerFlow;
  SEScalarPressure*                            m_InspirationTriggerPressure;
  SEScalarTime*                                m_InspirationTriggerTime;

  SEScalarPressureTimePerVolume*               m_InspirationTubeResistance;
  SEScalarPressureTimePerVolume*               m_InspirationValveResistance;
  eMechanicalVentilator_DriverWaveform         m_InspirationWaveform;

  std::vector<SESubstanceFraction*>            m_FractionInspiredGases;
  std::vector<const SESubstanceFraction*>      m_cFractionInspiredGases;

  std::vector<SESubstanceConcentration*>       m_ConcentrationInspiredAerosols;
  std::vector<const SESubstanceConcentration*> m_cConcentrationInspiredAerosols;

  SESubstanceManager&                          m_Substances;
};
