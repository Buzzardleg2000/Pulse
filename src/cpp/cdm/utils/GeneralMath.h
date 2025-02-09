/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#pragma once
class SESubstance;
class SESubstanceManager;
class SELiquidCompartment;
class SELiquidSubstanceQuantity;

class CDM_DECL GeneralMath
{
public:
  static bool CalculateConcentration(const SEScalarMass& mass, const SEScalarVolume& volume, SEScalarMassPerVolume& concentration, Logger* logger = nullptr);
  static bool CalculateMass(const SEScalarVolume& volume, const SEScalarMassPerVolume& concentration, SEScalarMass& mass, Logger* logger = nullptr);
  static bool CalculateHenrysLawConcentration(const SESubstance& substance, const SEScalarPressure& partialPressure, SEScalarMassPerVolume& concentration, Logger* logger = nullptr);
  static bool CalculatePartialPressureInGas(const SEScalar0To1& volumeFraction, const SEScalarPressure& pressure, SEScalarPressure& partialPressure, Logger* logger = nullptr);
  static bool CalculatePartialPressureInLiquid(const SESubstance& substance, const SEScalarMassPerVolume& concentration, SEScalarPressure& partialPressure, Logger* logger = nullptr);
  static void CalculateOsmolarity(const SEScalarAmountPerVolume& sodiumMolarity, const SEScalarAmountPerVolume& potassiumMolarity, const SEScalarAmountPerVolume& glucoseMolarity, const SEScalarAmountPerVolume& ureaMolarity, SEScalarOsmolarity& fluidOsmolarity);
  static void CalculateOsmolality(const SEScalarAmountPerVolume& sodiumMolarity, const SEScalarAmountPerVolume& potassiumMolarity, const SEScalarAmountPerVolume& glucoseMolarity, const SEScalarAmountPerVolume& ureaMolarity, const SEScalar& specificGravity, SEScalarOsmolality& fluidOsmolality);
  static bool CalculateSpecificGravity(const SEScalarMass& mass, const SEScalarVolume& volume, SEScalar& specificGravity, Logger* logger = nullptr);

  static void   Combinations(std::vector<size_t> maxValues, std::vector<std::vector<size_t>>& permutations);

  static bool LinearInterpolator(std::vector<double>& v, size_t newSize);
  static void LinearInterpolator1(std::vector<double>& v, size_t newSize);
  static double LinearInterpolator(double initial, double target, double percent);
  static double LinearInterpolator(double x1, double x2, double y1, double y2, double xPrime);
  static double PiecewiseLinearInterpolator(const std::vector<std::pair<double, double>>& points, double x);
  static void SplineInterpolater(std::vector<double>& v, size_t newSize);
  static double ParbolicInterpolator(double min, double max, double factor);

  static double Damper(double targetValue, double previousValue, double dampenFraction_perSec, double timeStep_s);

  static double PercentDifference(double expected, double calculated);
  static double PercentTolerance(double expected, double calculated, double epsilon = 1e-20);
  static double ExponentialDecayFunction(double dbase, double dmin, double dmax, double dx);
  static double ExponentialGrowthFunction(double dbase, double dmin, double dmax, double dx);
  static double LogisticFunction(double a, double x50, double k, double x);
  static double RootMeanSquaredError(std::vector<double> observed, std::vector<double> predicted, bool normalize = false, double outlierStandardDeviation = -1.0);
};
