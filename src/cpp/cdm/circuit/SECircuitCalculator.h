/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
#include "cdm/utils/SmartEnum.h"

#define CIRCUIT_CALCULATOR_TEMPLATE typename CircuitType, typename NodeType, typename PathType, \
                                    typename CapacitanceUnit, typename FluxUnit, typename InductanceUnit, \
                                    typename PotentialUnit, typename QuantityUnit, typename ResistanceUnit
#define CIRCUIT_CALCULATOR_TYPES CircuitType,NodeType,PathType,CapacitanceUnit,FluxUnit,InductanceUnit,PotentialUnit,QuantityUnit,ResistanceUnit

class eigen; // Encapsulate eigen in pimpl pattern

// These are the Eigen Solvers we can use for solving our circuits
struct CDM_DECL EigenCircuitSolver
{
  enum Type { Direct, PartialPivLu, FullPivLu, JacobiSvd, HouseholderQr, Ldlt, Llt, SparseLU, SparseQR, BiCGSTAB, ConjugateGradient };
  SMART_ENUM(EigenCircuitSolver, Type, 11);
  static char const* Value(size_t idx);
};

template<CIRCUIT_CALCULATOR_TEMPLATE>
class CDM_DECL SECircuitCalculator : public Loggable
{
public:
  SECircuitCalculator(const CapacitanceUnit& c, const FluxUnit& f, const InductanceUnit& i, const PotentialUnit& p, const QuantityUnit& q, const ResistanceUnit& r, Logger* logger);
  virtual ~SECircuitCalculator();
  
  virtual void SetEigenSolver(EigenCircuitSolver s){ m_solver = s; };
  virtual void SetEigenSolver(EigenCircuitSolver::Type t) { m_solver.set(t); };

  // Analysis of the Circuit dynamics
  virtual void Process(CircuitType& circuit, double timeStep_s);
  virtual void PostProcess(CircuitType& circuit);

protected:
  /**/virtual void ParseIn();
  /**/virtual bool Solve();
  /**/virtual void ParseOut();
  /**/virtual void CalculateFluxes();
  /**/virtual void CalculateQuantities();

  virtual void ParseInPotentialSources();

  virtual void Verbose(std::string location);

  // Valve Support
  virtual bool CheckAndModifyValves(bool solverPassed);
  virtual bool IsCurrentValveStateUnique();
  // Eigen Support
  virtual void PopulateAMatrix(NodeType& nKCL, PathType& p, double dMultiplier, bool hasPotentialSource = false);  
  // These are all transient and cleared/set at the start of the process call

  std::stringstream m_ss;

  double                         m_dT_s; 
  double                         m_currentTime_s;
  double                         m_refPotential;
  CircuitType*                   m_circuit;
  EigenCircuitSolver             m_solver;
  std::set<uint64_t>             m_valveStates;
  std::map<PathType*, size_t>    m_potentialSources;
  std::map<NodeType*, size_t>    m_blackBoxPotentialSources;

  const CapacitanceUnit &m_CapacitanceUnit;
  const FluxUnit        &m_FluxUnit;
  const InductanceUnit  &m_InductanceUnit;
  const PotentialUnit   &m_PotentialUnit;
  const QuantityUnit    &m_QuantityUnit;
  const ResistanceUnit  &m_ResistanceUnit;

private:
  eigen* _eigen;
};
