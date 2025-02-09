%Patient Methodology {#PatientMethodology}
==========================

@anchor patient-overview
Overview
========

Abstract
--------

The engine allows for patient variability via a set of parameters used to define aspects of the simulated human. These parameters are used throughout the system models to manipulate the physiological responses and homeostatic state. The code base contains several defined patients created for various testing and analysis purposes.

@anchor patient-intro
Introduction
------------

The engine was architected for the development and integration of multiscale models for an individualized whole-body predictive patient physiology model. The engine incorporates existing and novel models of organs and physiological systems into a whole-body model of a single generic individual within a reference population. Therefore, specific patient definitions are needed to create a virtual representation of the specific virtual human being simulated. The patient definition leverages the Commmon Data Model (CDM) to allow physiological parameters to be disseminated throughout the engine.
@anchor patient-design
System Design
=============

Background and Scope
--------------------

### Requirements

The patient needs to contain all user-definable parameters specific to an individual. Any parameters that are not explicitly set need to be calculated based on standard, healthy values.

### Approach

Each patient parameter falls into one of three categories:
- <b>Required:</b> The only parameter that is required to be set on the patient at runtime is Sex. All other values not explicitly set will be determined using default values and/or calculated outputs.
- <b>Optional:</b> These parameters can be optionally explicitly set by the user. Many optional values have bounds associated with them to ensure a healthy initial patient.
- <b>Unallowed:</b> These parameters are calculated or determined through simulation based on other settings. They can not be modified by the user.

Table 1 provides the values and equations used to determine optional values not explicitly set by the user and those unallowed to be externally modified.
@anchor patient-features
Features and Capabilities
-------------------------

### Definitions

<center>
*Table 1. All patient parameters, how they are determined, and upper and lower limits.*
</center>

|	Parameter	|	Modification Category	|	Initial Value (If not Explicitly Set)	|	Lower Bound	|	Upper Bound	|	Notes	|
|	---	|	---	|	---	|	---	|	---	|	---	|
|	Sex	|	Required	|	\f[M = Male,F = Female\f]	|	 -	|	 -	|	This is the only parameter required to be explicitly set.	|
|	Age	|	Optional	|	\f[A[yr] = 44\f]	|	18 yr	|	65 yr	|	No pediatric or geriatric modeling.	|
|	Ideal Weight	|	Optional	|	\f[Iw[kg] = \left\{ {\begin{array}{*{20}{c}} {{\rm{50}}{\rm{.0  +  2}}{\rm{.3}}\left( {{\rm{H[in]  -  60}}} \right),\;M}\\ {{\rm{45}}{\rm{.5  +  2}}{\rm{.3}}\left( {{\rm{H[in]  -  60}}} \right),\;F} \end{array}} \right.\f]	|	 -	|	 -	|	@cite green2017green	|
|	Weight	|	Optional	|	\f[W[kg] = 21.75H{[m]^2}\f]	|	 -	|	 -	|		|
|	Height	|	Optional	|	\f[{\rm{H[cm] = }}\left\{ {\begin{array}{*{20}{c}} {{\rm{177}},\;M}\\ {{\rm{163}},\;F} \end{array}} \right.\f]	|	M = 163 cm, F = 151 cm	|	M = 190 cm, F = 175.5 cm	|	Min = 3rd percentile, Max = 97th percentile, Standard = 50th percentile. @cite Centers2016clinical	|
|	Body Mass Index	|	Optional	|	\f[BMI[{\rm{kg/}}{m^2}] = W[kg]/{\rm{H[m}}{{\rm{]}}^2}\f]	|	16 kg/m<sup>2</sup>	|	30 kg/m<sup>2</sup>	|	No severly Underweight or Obese. @cite World2006bmi	|
|	Body Fat Fraction	|	Optional	|	\f[{\rm{Ff[cm] = }}\left\{ {\begin{array}{*{20}{c}} {{\rm{0}}{\rm{.21}},\;M}\\ {0.28,\;F} \end{array}} \right.\f]	|	M = 0.02, F = 0.10	|	M = 0.25, F = 0.32	|	No obese and not less than essential fat. @cite muth2009what	|
|	Blood Volume Baseline	|	Optional	|	\f[Bv[mL] = 65.6W{[kg]^{1.02}}\f]	|	Bv * 0.85	|	Bv * 1.15	|	Above Stage 1 Hypovolemia. @cite Morgan2006Clinical	|
|	Basal Metabolic Rate	|	Optional	|	\f[{\rm{Bmr[kcal/day] = }}\left\{ {\begin{array}{*{20}{c}} {{\rm{88}}{\rm{.632  +  3}}{\rm{.397W[kg]  +  4}}{\rm{.799H[cm]  -  5}}{\rm{.677A[yr]}},\;M}\\ {{\rm{447}}{\rm{.593  +  9}}{\rm{.247W[kg]  +  3}}{\rm{.098H[cm]  -  4}}{\rm{.330A[yr]}},\;F} \end{array}} \right.\f]	|	 -	|	 -	|	@cite roza1984metabolic	|
|	Heart Rate Baseline	|	Optional	|	\f[Hr[bpm] = 72\f]	|	50 bpm	|	110 bpm	|	Bradycardia < 60 bpm & Tachycardia > 100 bpm.	|
|	Systolic Arterial Pressure Baseline	|	Optional	|	\f[Sys[mmHg] = 114\f]	|	90 mmHg	|	120 mmHg	|	No hypotension or hypertension. 	|
|	Diastolic Arterial Pressure Baseline	|	Optional	|	\f[Dia[mmHg] = 73.5\f]	|	60 mmHg	|	80 mmHg	|	No hypotension or hypertension. Includes pressure fraction check: Dia > 0.75 Sys.	|
|	Pulse Pressure Pressure Baseline	|	Optional	|	\f[Pp[mmHg] = Sys[mmHg] - Dia[mmHg]\f]	|	0.25 * Sys	|	 -	|		|
|	Mean Arterial Pressure	|	Optional	|	\f[Map = \frac{1}{3}Sys + \frac{2}{3}Dia\f]	|	 -	|	 -	|	@cite guyton2006medical	|
|	Respiration Rate Baseline	|	Optional	|	\f[Rr[bpm] = 12\f]	|	8 bpm	|	20 bpm	|		|
|	Alveoli Surface Area	|	Optional	|	\f[Asa[{m^2}] = \frac{{{\rm{Tlc[L]}}}}{{6.17}} \times {\rm{70}}\f]	|	 -	|	 -	|	Calculated using standard Tlc (6.17 L) @cite ganong1995review and standard Asa (70 m2) @cite roberts2000gaseous.	|
|	Right Lung Ratio	|	Optional	|	\f[Rlr = 0.525\f]	|	0.5	|	0.6	|		|
|	Skin Surface Area	|	Optional	|	\f[Ssa[{m^2}] = 0.20247W{[kg]^{0.45}}H{[m]^{0.725}}\f]	|	 -	|	 -	|	@cite du1989formula	|
|	Heart Rate Maximum	|	Optional	|	\f[H{r_{\max }}[bpm] = 208 - 0.7A[yr]\f]	|	 -	|	 -	|	@cite tanaka2001age	|
|	Heart Rate Minimum	|	Optional	|	\f[H{r_{\min }}[bpm] = 0.001\f]	|	 -	|	 -	|		|
|	Functional Residual Capacity	|	Optional	|	\f[Frc[mL] = 30Iw[kg]\f]	|	 -	|	 -	|	@cite ganong1995review	|
|	Total Lung Capacity	|	Optional	|	\f[Tlc[mL] = 80Iw[kg]\f]	|	 -	|	 -	|	@cite ganong1995review	|
|	Residual Volume	|	Optional	|	\f[Rv[mL] = 16Iw[kg]\f]	|	 -	|	 -	|	@cite ganong1995review	|
|	Lean Body Mass	|	Unallowed	|	\f[Lbm = W(1 - Ff)\f]	|	 -	|	 -	|		|
|	Body Density	|	Unallowed	|	\f[Bd = {{\left( {\frac{{4.95}}{{Ff + 4.5}} + \frac{{4.57}}{{Ff + 4.142}}} \right)} \mathord{\left/  {\vphantom {{\left( {\frac{{4.95}}{{Ff + 4.5}} + \frac{{4.57}}{{Ff + 4.142}}} \right)} 2}} \right.  \kern-\nulldelimiterspace} 2}\f]	|	 -	|	 -	|	Average of Siri @cite siri1961body and Brozek @cite brovzek1963densitometric equations.	|
|	Tidal Volume Baseline	|	Unallowed	|	\f[Tv[mL] = 37Iw[kg] - Frc[mL]\f]	|	 -	|	 -	|	@cite ganong1995review	|
|	Expiratory Reserve Volume	|	Unallowed	|	\f[Erv = Frc - Rv\f]	|	 -	|	 -	|	@cite ganong1995review	|
|	Inspiratory Capacity	|	Unallowed	|	\f[Ic = Tlc - Frc\f]	|	 -	|	 -	|	@cite ganong1995review	|
|	Inspriatory Reserve Volume	|	Unallowed	|	\f[Irv = Ic - Tv\f]	|	 -	|	 -	|	@cite ganong1995review	|
|	Vital Capacity	|	Unallowed	|	\f[Vc - Tlc - Rv\f]	|	 -	|	 -	|	@cite ganong1995review	|

### Stabilization

There are two complete sets of patient parameters stored in the data model:

1. <i>Initial Patient</i>: These are the healthy patient parameters before any conditions or action are applied. All patient parameters are set at the beginning of the resting stabilization period (see @ref SystemMethodology). These values are used to modify the inner workings of systems. The complex interactions require a simulated period to allow everything to balance into a stable, homeostatic, healthy starting point. These parameters should not be changed during a simulation.

2. <i>Current Patient</i>: After each stabilization period (i.e., resting, conditions, and feedback), several parameters are reset due to their reliance on combined effects. These parameters can further be changed during a simulation.

While every effort has been made to allow any combination of patient parameters within bounds, there is no guarantee that all combinations will be able to reach a stable starting homeostatic point. 

### Dependencies

Patient parameters are not dependent on any systems, but many systems are dependent on specific patient values. Some parameters are used to modify baseline fluid/thermal circuit values or direct calculation of outputs, and others manipulate how feedback mechanisms work. For details see the following system methodology sections regarding dependencies and patient variability:

@secreflist
  @refitem cardiovascular-variability "Cardiovascular Dependencies"
  @refitem drugs-variability "Drugs Dependencies"
  @refitem energy-variability "Energy Dependencies"
  @refitem nervous-variability "Nervous Dependencies"
  @refitem renal-dependencies "Renal Dependencies"
  @refitem respiratory-variability "Respiratory Dependencies"
@endsecreflist

@anchor patient-results
Results and Conclusions
=======================

Verification and Validation
---------------------------

<center><br>
*Table 2. There are several patients that we created for base validation, system-specific patient effects validation, and extreme case will stabilization.*
</center>

|	<b>Name</b>	|	Standard Male	|	Standard Female	|	Default Male	|	Default Female	|	Overweight	|	Underweight	|	Tachycardic	|	Bradycardic	|	Extreme Female	|	Extreme Male	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	<b>Sex</b>	|	Male	|	Female	|	Male	|	Female	|	Male	|	Male	|	Male	|	Male	|	Female	|	Male	|
|	<b>Age</b>	|	44 yr	|	44 yr	|	-	|	-	|	44 yr	|	44 yr	|	44 yr	|	44 yr	|	18 yr	|	65 yr	|
|	<b>Weight</b>	|	170 lb	|	130 lb	|	-	|	-	|	215 lb	|	115 lb	|	170 lb	|	170 lb	|	-	|	-	|
|	<b>Height</b>	|	71 in	|	64 in	|	-	|	-	|	71 in	|	71 in	|	71 in	|	71 in	|	4.5 ft	|	7 ft	|
|	<b>Body Fat Fraction</b>	|	0.21	|	0.28	|	-	|	-	|	0.25	|	0.15	|	0.18	|	0.18	|	0.32	|	0.02	|
|	<b>Heart Rate Baseline</b>	|	72 bpm	|	72 bpm	|	-	|	-	|	72 bpm	|	72 bpm	|	110 bpm	|	50 bpm	|	60 bpm	|	100 bpm	|
|	<b>Systolic Arterial Pressure Baseline</b>	|	114 mmHg	|	114 mmHg	|	-	|	-	|	114 mmHg	|	114 mmHg	|	114 mmHg	|	114 mmHg	|	90 mmHg	|	120 mmHg	|
|	<b>Diastolic Arterial Pressure Baseline</b>	|	73.5 mmHg	|	73.5 mmHg	|	-	|	-	|	73.5 mmHg	|	73.5 mmHg	|	73.5 mmHg	|	73.5 mmHg	|	60 mmHg	|	80 mmHg	|
|	<b>Respiration Rate Baseline</b>	|	12 bpm	|	12 bpm	|	-	|	-	|	12 bpm	|	12 bpm	|	20 bpm	|	8 bpm	|	8 bpm	|	20 bpm	|
|	<b>Right Lung Ratio</b>	|	-	|	-	|	-	|	-	|	-	|	-	|	-	|	-	|	0.5	|	0.6	|

<center><br>
*Table 3. There are several patients we created for combined effects validation and showcase scenarios.*
</center>

|	<b>Name</b>	|	Cynthia	|	Gus	|	Joel	|	Nathan	|	Rick	|	Hassan	|	Soldier	|	Jeff	|	Carol	|	Jane	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	<b>Sex</b>	|	Female	|	Male	|	Male	|	Male	|	Male	|	Male	|	Male	|	Male	|	Female	|	Female	|
|	<b>Age</b>	|	30 yr	|	32 yr	|	44 yr	|	44 yr	|	23 yr	|	28 yr	|	22 yr	|	25 yr	|	40 yr	|	18 yr	|
|	<b>Weight</b>	|	130 lb	|	190 lb	|	170 lb	|	170 lb	|	161 lb	|	185 lb	|	170 lb	|	180 lb	|	160 lb	|	120 lb	|
|	<b>Height</b>	|	64 in	|	70 in	|	71 in	|	71 in	|	68 in	|	72 in	|	71 in	|	72 in	|	66 in	|	64 in	|
|	<b>Body Fat Fraction</b>	|	0.18	|	0.18	|	0.18	|	0.18	|	0.18	|	0.18	|	0.18	|	0.12	|	0.25	|	0.18	|
|	<b>Heart Rate Baseline</b>	|	72 bpm	|	93 bpm	|	110 bpm	|	72 bpm	|	100 bpm	|	110 bpm	|	84 bpm	|	72 bpm	|	72 bpm	|	72 bpm	|
|	<b>Systolic Arterial Pressure Baseline</b>	|	114 mmHg	|	90 mmHg	|	120 mmHg	|	114 mmHg	|	120 mmHg	|	90 mmHg	|	114 mmHg	|	114 mmHg	|	114 mmHg	|	114 mmHg	|
|	<b>Diastolic Arterial Pressure Baseline</b>	|	73.5 mmHg	|	60 mmHg	|	80 mmHg	|	73.5 mmHg	|	80 mmHg	|	60 mmHg	|	73.5 mmHg	|	73.5 mmHg	|	73.5 mmHg	|	73.5 mmHg	|
|	<b>Respiration Rate Baseline</b>	|	18 bpm	|	14 bpm	|	15 bpm	|	16 bpm	|	16 bpm	|	18 bpm	|	16 bpm	|	16 bpm	|	18 bpm	|	18 bpm	|

All of the patients that are included with the code base have been validated to ensure that they reach a homeostatic point that hits all of the defined criteria. The following tables show the state of the patients after all stabilization phases (see @ref SystemMethodology for details) and compare the resulting patient values to those initially set/computed. Values that are explicitly set are expected to have very tight tolerances, while those that are initially calculated may drift due to the complex interaction of all patient parameters and system settings. The most sensitive of these parameters to external effects is tidal volume.

Values that are colored green are within 10% of the original set or calculated/estimated (expected) value, yellow are within 30%, and red are greater than 30% error. Values that are not within 10% are not necessarily a failure.  As previously described, many of these are estimated before the stabilization simulation begins, and are really determined and overwritten after all other factors reach homeostasis. Those values that are explicitly set by the user/patient file should have much tighter tolerances.

<center>
*Table 4. StandardMale patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/StandardMaleValidationTable.md

<center>
*Table 5. StandardFemale patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/StandardFemaleValidationTable.md

<center>
*Table 6. DefaultMale patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/DefaultMaleValidationTable.md

<center>
*Table 7. DefaultFemale patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/DefaultFemaleValidationTable.md

<center>
*Table 8. Overweight patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/OverweightValidationTable.md

<center>
*Table 9. Underweight patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/UnderweightValidationTable.md

<center>
*Table 10. Tachycardic patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/TachycardicValidationTable.md

<center>
*Table 11. Bradycardic patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/BradycardicValidationTable.md

<center>
*Table 12. Cynthia patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/CynthiaValidationTable.md

<center>
*Table 13. Gus patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/GusValidationTable.md

<center>
*Table 14. Joel patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/JoelValidationTable.md

<center>
*Table 15. Nathan patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/NathanValidationTable.md

<center>
*Table 16. Hassan patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/HassanValidationTable.md

<center>
*Table 17. Soldier patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/SoldierValidationTable.md

<center>
*Table 18. Jeff patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/JeffValidationTable.md

<center>
*Table 19. Carol patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/CarolValidationTable.md

<center>
*Table 20. Jane patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/JaneValidationTable.md

<center>
*Table 21. ExtremeFemale patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/ExtremeFemaleValidationTable.md

<center>
*Table 22. ExtremeMale patient values with expected and engine output.*
</center><br>

@insert ./test_results/tables/ExtremeMaleValidationTable.md

Conclusions
-----------

The methodology used to define and set patient parameters is successful in allowing for the simulation of unique humans.
@anchor patient-future
Future Work
===========

Coming Soon
-----------

There are no planned near-term additions.

Recommended Improvements
------------------------

The physiology engine is providing consistent and accurate physiology simulation, and its ability to simulate physiology for a generic representative of a population will improve as development continues. It is not within the scope of the current project for the engine to predictively simulate the physiology of a specific (or average) individual within a population, but the development of technologies that make the engine usable and extensible to the community also facilitate multiscale model development and can be leveraged for predictive simulation and thus clinical use.

One way to extend the engine for clinical use is to use data from sources such as electronic health records (EHRs), lab reports, fitness evaluations, or wearable sensors to create a personalized physiological profile. A tool could then translate the personal physiological profile into parameters that can inform the physiology models within the systems' framework.
@anchor patient-appendices
Appendices
==========

Acronyms
--------

CDM - Common Data Model

Data Model Implementation
-------------------------

@ref PatientTable "Patient"