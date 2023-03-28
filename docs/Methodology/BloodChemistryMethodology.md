Blood Chemistry Methodology {#BloodChemistryMethodology}
==========================

@anchor bloodchemistry-overview
Overview
========
Abstract
--------
The purpose of the Blood Chemistry System is primarily to hold the system-level blood substance data. It also uses information about the blood to compute plasma volume and whole-body mass and concentration information. In the future, all blood chemistry computations will take place in the Blood Chemistry system, including the acid-base balance computations of blood gas binding and species distribution that are currently performed by the Saturation class. For that reason, the saturation and acid-base balance models are described in the [approach](@ref bloodchemistry-approach) section of this document.

Introduction
------------
The Blood Chemistry System holds the system-level blood substance data that is computed on the compartment level by other systems. The Blood Chemistry system provides a link for systems and users to acquire blood substance data. In other words, if a user or another system needs to know the concentration of a substance in the blood or plasma, that information is obtained from the Blood Chemistry system data. The Blood Chemistry system also processes information about the blood to compute plasma volume, and it computes the Arterial Blood Gas, Complete Blood Count, and Metabolic Panel [assessments](@ref bloodchemistry-assessments).

Although they currently reside in a different class, the blood gas distribution models in the Saturation class are conceptually a part of the Blood Chemistry system. Therefore, models from the Saturation class are described here.

@anchor bloodchemistry-system-design
System Design
=============
Background and Scope
--------------------
The analysis of the blood constituents is important for assessing physiological function. %Respiratory function and acid-base status are assessed by analyzing the distribution of gas species in the blood. Because respiratory, renal, and cardiovascular system functions integrate to maintain acid-base homeostasis, abnormal function in any of those systems can cause an acid-base disturbance. For example, respiratory conditions which lead to an increase in the amount of dissolved CO2 in the blood, such as bronchitis, are associated with respiratory acidosis. Metabolic acidosis often accompanies renal pathologies. In order to investigate the origin of an acid-base disturbance, a provider may order an arterial blood gas test, metabolic panel, or a complete blood count. These three assessments, provide a detailed account of the composition of a person's blood, and this information is used, along with a physiological model of acid-base balance, to determine the causes of acid-base disturbances. 

There are three models commonly used for acid-base analysis. One model, often called the Boston model, is easy to understand and remember, but it is based entirely on the Henderson-Hasselbalch equation, and bicarbonate equilibrium alone is demonstrably insufficient for predicting carbon dioxide species distribution in human blood @cite wooten2010standard. To compensate, the Boston model includes six bicarbonate-based rules that are used to assess acid-base status at the bedside. Another model extends the Boston approach by introducing a new parameter called "base excess" to quantify the metabolic component of an acid-base disturbance @cite siggaard1977van. The problem with base-excess is that it is not CO2-invariant in vivo due to the ionic shifts between the intravascular and interstitial compartments. However, base excess is CO2-invariant when it is computed from measured plasma pH and PCO2, but at a constant hemoglobin concentration of 50 g/L. A third model, developed by Peter Stewart and thus called the Stewart approach, describes acid-base balance mechanistically through the electrochemical equilibrium dynamics @cite kellum2009stewart.

The Stewart approach has been the subject of criticism. Some researchers have used dilutional acidosis as a model to argue that Stewart's approach does not actually provide any mechanistic insights because "neither the principle of electroneutrality, nor a change in [SID], nor increased H2O dissociation plays a mechanistic role" @cite doberer2009critique. However, others have noted that the Stewart method "is physicochemically well grounded" and that a dilution of the [SID] is "inevitably a dilution of the buffer base" @cite lang2010comment. Although the Boston model and the Copenhagen model are used more often for clinical, or bedside, analysis of acid-base status, the mechanistic detail of the Stewart model facilitates the computational modeling of acid-base balance and disturbances. We use the Stewart model as the basis for the acid-base balance model.

@anchor bloodchemistry-requirements
### Requirements
- Acid-base status 
- %Respiratory and metabolic acid-base disturbances

@anchor bloodchemistry-approach
### Approach
The Blood Chemistry system is the link between compartment-level and system-level blood constituent data. It populates system-level concentration and other data from compartment level data. 

Additionally, the blood chemistry computations required to compute acid-base balance and blood gas distribution are performed by the Blood Chemistry system (note that these equations are in the separate Saturation class in the current engine). The Blood Chemistry system does not adjust mass or concentration of substances independently, but it does adjust concentration of species of a substance. For example, the Blood Chemistry system does not adjust the mass or concentration of total carbon dioxide in the blood, but it does compute the concentrations of the constituent species of total carbon dioxide (CO2): dissolved CO2, bound CO2, and bicarbonate. The acid-base and hemoglobin saturation models are used to compute how much of the total amount of carbon dioxide is in the form of bicarbonate, how much is dissolved, and how much is bound to hemoglobin. Similarly, the Blood Chemistry system does not change the total amount of oxygen in a compartment, but it does compute how much oxygen is dissolved and how much is bound to hemoglobin.

The acid-base and blood gas distribution model is based on the Stewart model for acid-base balance and the hemoglobin binding model described by Dash and Bassingthwaighte @cite dash2010erratum. As described by Stewart, the model begins with electroneutrality. The charge of all strong ions in solution must be zero, as demonstrated by Equation 1.

\f[\ \ \left[Na^+\right]+\left[K^+\right]+\left[I^{n+}\right]+\left[Cl^-\right]+\left[La^-\right]+\left[Ket^-\right]+\left[I^{n-}\right]+{\mathrm{[HCO}}^-_{\mathrm{3}}]-\left[A^-\right]\ =0 \f]
<center>
<i>Equation 1.</i>
</center><br>
Where *I<sup>n,+/-</sup>* represents the n anions and cations not included in the engine. All symbols are defined in [Table 6](@ref bloodchemistry-symbols). Note that some ions, such as carbonate, are in such minute concentrations that their contribution to electrical neutrality is negligible. These ions are neglected and thus excluded from Equation 1.

It is convenient to define a concentration called strong ion difference [SID], described by Equation 2, which reduces the electroneutrality to Equation 3.

\f[[SID]=\left[Na^+\right]+\left[K^+\right]+\left[I^{n+}\right]-\left(\left[Cl^-\right]+\left[La^-\right]+\left[Ket^-\right]+\left[I^{n-}\right]\right) \f]
<center>
<i>Equation 2.</i>
</center><br>

\f[[SID]-{\mathrm{[HCO}}^-_{\mathrm{3}}]-[A^-]=0 \f]
<center>
<i>Equation 3.</i>
</center><br>

Equation 4 is an empirical formula for the blood protein anion contribution (*A<sup>-</sup>*) developed by Figge et al. @cite figge1992serum.

\f[\left[A^-\right]=\left[Alb\right]\left(0.123pH-0.631\right)+[Pi](0.309pH-0.469) \f]
<center>
<i>Equation 4.</i>
</center><br>

The Henderson-Hasselbalch equation and equations representing Henry's law for oxygen and carbon dioxide are shown as Equations 5, 6, and 7.

\f[pH=6.1+{\mathrm{log} \left(\frac{\left[HCO^-_3\right]}{\alpha P_{CO_2}}\right)\ } \f]
<center>
<i>Equation 5.</i>
</center><br>

\f[\left[O_2\right]={\alpha }_{o_2} P_{O_2} \f]
<center>
<i>Equation 6.</i>
</center><br>

\f[S_{CO_2}=f\left(pH,P_{O_2},P_{CO_2},T,DPG\right) \f]
<center>
<i>Equation 7.</i>
</center><br>

The above equations are combined into a single expression with three unknowns: bicarbonate concentration, the partial pressure of oxygen, and the partial pressure of carbon dioxide.

\f[[SID]-{\mathrm{[HCO}}^-_{\mathrm{3}}]-\left[Alb\right]\left\{0.123 \left(6.1+{\mathrm{log} \left(\frac{\left[HCO^-_3\right]}{\alpha_{O_{2}} P_{CO_2}}\right)\ }\right)-0.631\right\}+[Pi]\left\{0.309 \left(6.1+{\mathrm{log} \left(\frac{\left[HCO^-_3\right]}{\alpha_{CO_{2}} P_{CO_2}}\right)\ }\right)-0.469\right\}=0\f]
<center>
<i>Equation 8.</i>
</center><br>

The conservation of mass requires that the total amount of oxygen and carbon dioxide remain constant. Therefore, the sum of dissolved oxygen and hemoglobin-bound oxygen must remain constant. Likewise, the sum of dissolved carbon dioxide, hemoglobin-bound carbon dioxide, and bicarbonate must remain constant. The amount of oxygen or carbon dioxide bound to hemoglobin at a given partial pressure of the gas is defined by the model described in @cite dash2010erratum. The binding model and the conservation equations fully define the system, represented by Equation 8 above and Equations 9, 10, 11, and 12 below.

\f[[{\mathrm{T}}_{{\mathrm{O}}_{\mathrm{2}}}]=\left(\left[O_2\right]+4 S_{O_2}\left[Hgb\right]\right) \f]
<center>
<i>Equation 9.</i>
</center><br>

\f[[{\mathrm{T}}_{{\mathrm{CO}}_{\mathrm{2}}}]=\left(\left[CO_2\right]+\left[HCO^-_3\right]+4{S}_{CO_2}\left[Hgb\right]\right) \f]
<center>
<i>Equation 10.</i>
</center><br>

\f[S_{O_2}=f\left(pH,P_{O_2},P_{CO_2},T,DPG\right) \f]
<center>
<i>Equation 11.</i>
</center><br>

\f[S_{CO_2}=f\left(pH,P_{O_2},P_{CO_2},T,DPG\right) \f]
<center>
<i>Equation 12.</i>
</center><br>

The blood gas distribution model is used to compute the acid-base status and gas saturation in every cardiovascular compartment at every time slice. This model is used only for intravascular fluid, which is a lumped model of the intra and extracellular fluid spaces within the blood vessels. In the engine, the extravascular fluid does not contain hemoglobin or any substrate for gases to bind.

@anchor bloodchemistry-data-flow
Data Flow
---------
### Initialize
Compartment-level substance data is initialized by the substances controller class, and whole-body blood substance data is initialized from the compartment data by the Blood Chemistry system. The constant values of blood density, blood specific heat, plasma fraction of neutral lipid and phospholipid, white blood cell count, phosphate, and strong ion difference concentrations are also initialized by the Blood Chemistry system. Note that the strong ion difference will not be constant following a planned future improvement. The blood chemistry system does not require stabilization. Blood properties are initialized and initial system data is set in the initialization method. System data is populated throughout the engine stabilization process. 

### Preprocess
There is no system specific function for Preprocess in Blood Chemistry.

### Process
During Process, the blood concentrations, blood gases, and other blood properties that are calculated or changed by other systems are set on the blood chemistry system data objects.

### Post Process
There is no system specific function for Post Process in Blood Chemistry.

### Assessments
Assessments are data collected and packaged to resemble a report or analysis that might be ordered by a physician. There are three assessments in the Blood Chemistry system: an Arterial Blood Gas Test (ABG), a Metabolic panel, and a Complete Blood Count (CBC). The metabolic panel is modeled after the Chem-14 blood test. Currently, eight of the fourteen Chem-14 components are included in the metabolic panel. 

@anchor bloodchemistry-features
Features and Capabilities
-------------------------
The Blood Chemistry system is a support system primarily for storing and relaying data. Thus, the fidelity of the system is a direct representation of the fidelity of the other system models. The concentration of a substance in the blood is computed, typically in a specific compartment or multiple compartments, during the simulation of some process within another system. For example, the blood concentration of sodium is adjusted by the [renal system](@ref RenalMethodology) during filtration and clearance and by the [tissue system](@ref TissueMethodology) in the compartment diffusion models. The whole-body, system-level blood concentration of sodium is set to the concentration in the vena cava compartment by the Blood Chemistry system.

Some calculations are preformed by the Blood Chemistry system. The red blood cell count, hematocrit, and the plasma volume are calculated here. The red blood cell volume is calculated based on the hemoglobin content of the body (a variable) and the average mass of hemoglobin per red blood cell (currently a constant parameter). The average volume of a red blood cell is then used to calculate the total volume of the red blood cells. The hematocrit is then calculated as the ratio of red blood cell volume to total blood volume. The plasma volume is calculated as the total blood volume less the red blood cell volume.

@anchor bloodchemistry-variability
### Patient Variability
Substances are initialized in the body by concentration rather than by mass, so morphological variability in patients will have no effect on initial concentrations. However, patient variability does affect physiology, and homeostatic states will vary with patient parameters. For that reason, post-stabilization variance in blood chemistry, including substance concentration, is possible between two different patients. A detailed discussion of patient variability is available in the @ref PatientMethodology report.

### Outputs
The Blood Chemistry system provides (for circulating substances):
- Blood concentration
- Total mass in blood, tissue, and body
- Pulmonary shunt fraction (fraction of blood bypassing pulmonary oxygenation)

@anchor bloodchemistry-assumptions
Assumptions and Limitations
---------------------------
Limitations on the Blood Chemistry system are imposed by the supported systems and by the active substances in the simulation. For example, ALT, AST, and ALP are not supported substances, and therefore cannot be included in the metabolic panel assessment. 

Additionally, blood cells are not modeled. We attempt to account for the contribution of cellular elements to the blood volume, as described above, but white cells and platelets are not included. 

@anchor bloodchemistry-events
Events
------
### Hypercapnia
Hypercapnia occurs when the partial pressure of carbon dioxide in the blood rises to greater than 60 mmHg @cite guyton2006medical, as is most commonly seen in respiratory conditions, such as severe airway obstruction. This condition manifests as a decrease in blood pH and an increase in cardiac output, blood pressure, and heart rate.

The engine triggers the hypercapnia event when the partial pressure of carbon dioxide in the aorta rises above 60 mmHg. This is a reversible condition and is considered resolved when the partial pressure falls below 57 mmHg. This 3 mmHg window provides a buffer to account for normal fluctuations in the model.

### Hypoxia
Hypoxia occurs when the partial pressure of oxygen in the blood falls below 65 mmHg @cite Pierson2000Pathophysiology. This can be due to a number of conditions that range from heart failure to poor respiratory function. This condition manifests as a decrease in oxygen saturation, increased heart rate, increased respiratory rate, decreased tidal volume, and pulmonary hypertension.

The engine triggers the hypoxia event when the partial pressure of oxygen in the aorta falls below 65 mmHg. This is a reversible condition and is considered resolved when the partial pressure rises above 68 mmHg. This 3 mmHg window provides a buffer to account for normal fluctuations in the model.

### Hyperoxemia
Hyperoxemia occurs when the partial pressure of oxygen in the blood rises above 120 mmHg, usually due to excessive supplemental oxygen delivery. Hyperoxemia can induce cerebral vasoconstriction, neuronal cell death, and seizures. In addition, hyperoxemia reduces the cardiac index and heart rate while increasing peripheral vascular resistance @cite gershengorn2014hyperoxemia.

The engine triggers the moderate hyperoxemia event when the partial pressure of oxygen in the aorta rises above 120 mmHg. This is a reversible condition and is considered resolved when the partial pressure falls below 117 mmHg. This 3 mmHg window provides a buffer to account for normal fluctuations in the model.

If the partial pressure of oxygen in the blood rises to greater than 200 mmHg, the severe hyperoxemia event is triggered to flag the oxygen toxicity.

### Hypocapnia
Hypocapnia occurs when the partial pressure of carbon dioxide in the bloos falls below 30 mmHg and is most commonly associated with rapid breathing or hyperventilation. 

The engine triggers the moderate hypocapnia event when the partial pressure of carbon dioxide in the aorta falls below 30 mmHg. This is a reversible condition and is considered resolved when the partial pressure rises above 33 mmHg. This 3 mmHg window provides a buffer to account for normal fluctuations in the model.

FI the partial pressure of carbon dioxide in the blood falls below 15 mmHg, the severe hypocapnia event is triggered.

### Brain Oxygen Deficit
The brain is unable to complete any significant anaerobic metabolism. Therefore, without oxygen in the brain, unconsciousness results within five to ten seconds, and permanent damage can occur within five to ten minutes @cite guyton2006medical. Additionally, irreversible damage can occur if the oxygen tension in the brain is too low for a prolonged period of time @cite dhawan2011neurointensive. 

There are two events in the engine related to an oxygen deficit in the brain: Brain Oxygen Deficit and Critical Brain Oxygen Deficit. The engine triggers the brain oxygen deficit event when the partial pressure of oxygen in the brain drops below 19 mmHg. If the partial pressure of oxygen in the brain drops below 10 mmHg, the patient enters a critical brain oxygen deficit state. If the critical deficit event is active, it is removed when the oxygen partial pressure increases above 12 mmHg, and only the less-critical deficit event will be active. Both deficit events are removed when the oxygen partial pressure returns above 25 mmHg. The threshold values are chosen based on empirical data reviewed in summary in @cite dhawan2011neurointensive, and from data presented in @cite purins2012brain and @cite doppenberg1998determination.

### Myocardium Oxygen Deficit
Blood flow through the coronary arteries provides required oxygen to the heart muscle. If the oxygen supply to the heart is limited through respiratory distress or some other condition, the demand for oxygen will cause dilation of the coronary arteries. By increasing the flow of blood to the heart muscle, the oxygen supply increases @cite guyton2006medical. This effect is not currently modeled in the engine, but the myocardium oxygen level is observed and an event is triggered when it becomes too low. In the future, the resistance to flow in the coronary arteries will be increased to represent blood vessel dilation, and the asystole rhythm change will be tied to the arterial partial pressure of oxygen.

The engine triggers the myocardium oxygen deficit event when the partial pressure of oxygen is less than 5 mmHg. 

### Acid-Base Disturbance Events
Acid-base disturbances can be respiratory or metabolic in origin. When an acid-base disturbance is caused by a change in the total amount of carbon dioxide in the blood, it is considered to be a respiratory disturbance because that system regulates carbon dioxide. If the origin is a change in the strong ion difference, then the disturbance is said to be metabolic in origin. Because the respiratory system responds to a metabolic disturbance and the renal and other systems respond to a respiratory disturbance with compensatory mechanisms, it is difficult to observe a purely metabolic or respiratory disturbance. For that reason, disturbances are sometimes further classified as acute, compensated, and mixed @cite hall2011guyton. There are four acid-base disturbance events observed in the engine.

#### Metabolic Acidosis
The metabolic acidosis event is triggered when the blood pH drops below the lower bound of the normal range, which is 7.35 for arterial blood (note that the blood pH is sampled from the aorta compartment) @cite Leeuwen2015laboratory, **and** the bicarbonate concentration is less than 22.0 mM. This reversible event is removed when the blood pH increases above 7.38. The small buffer of 0.03 is to allow for numerical fluctuations during transitions.

#### Metabolic Alkalosis
The metabolic alkalosis event is triggered when the blood pH increases above the upper bound of the normal range, which is 7.45 for arterial blood (note that the blood pH is sampled from the aorta compartment) @cite Leeuwen2015laboratory, **and** the bicarbonate concentration is greater than 26.0 mM. This reversible event is removed when the blood pH decreases below 7.42. The small buffer of 0.04 is to allow for numerical fluctuations during transitions.

#### Respiratory Acidosis
The respiratory acidosis event is triggered when the blood pH drops below the lower bound of the normal range, which is 7.35 for arterial blood (note that the blood pH is sampled from the aorta compartment) @cite Leeuwen2015laboratory, **and** the partial pressure of carbon dioxide in the arteries is greater than 47.0 mmHg. This reversible event is removed when the blood pH increases above 7.38. The small buffer of 0.03 is to allow for numerical fluctuations during transitions.

#### Respiratory Alkalosis
The respiratory alkalosis event is triggered when the blood pH increases above the upper bound of the normal range, which is 7.45 for arterial blood (note that the blood pH is sampled from the aorta compartment) @cite Leeuwen2015laboratory, **and** the partial pressure of carbon dioxide in the arteries less than 44.0 mmHg. This reversible event is removed when the blood pH decreases below 7.42. The small buffer of 0.04 is to allow for numerical fluctuations during transitions. 

<a href="./Images/BloodChemistry/BloodChemistryAcidBaseEvents.png"><img src="./Images/BloodChemistry/BloodChemistryAcidBaseEvents.png"></a>
<center>
*Figure 2. The acidosis and alkalosis events are broken down into either metabolic or respiratory induced. Metabolic induced acidosis/alkalosis is driven from a shift in bicarbonate
while respiratory induced acidosis/alkalosis is due to a chance in arterial carbon dioxide. The shift between either acidosis or alkalosis occurs at a blood pH of 7.4.*
</center><br>

@anchor bloodchemistry-assessments
Assessments
-----------

### Arterial Blood Gas Test

An arterial blood gas test is a measure of the oxygen and carbon dioxide in the blood and the blood pH. These factors are used to identify poor lung or kidney function. Many conditions can be associated with an imbalance of blood oxygen, carbon dioxide, and pH, such as COPD and asthma. A complete list of the components of this assessment and the results can be found in the [Validation](@ref bloodchemistry-validation-assessments) section.

### Complete Blood Count

A CBC is a blood test that is used to assess a patient&rsquo;s overall health. It measures several components of the blood including red blood cells, white blood cells, hemoglobin concentration, and hematocrit. When measured, these values can reveal a variety of disorders, including anemia and infection. A complete list of the components of this assessment and the results can be found in the [Validation](@ref bloodchemistry-validation-assessments) section.

### Metabolic Panel

A comprehensive metabolic panel, Chem 14, is a screening assessment used by clinicians to evaluate the overall health of a patient. This assessment can be used to screen for diabetes, liver disease, and kidney disease. This test measures a variety of substance concentrations in the blood, including sodium, protein, calcium, glucose, creatinine, and blood urea nitrogen (BUN). A complete list of the components of this assessment and the results can be found in the [Validation](@ref bloodchemistry-validation-assessments) section.

@anchor bloodchemistry-results
Results and Conclusions
=======================

Validation - Resting Physiologic State
--------------------------------------
Published values from the literature were compared to the engine output to perform a quantitative validation of the blood chemistry resting physiology. Table 1 shows the validation results.  The validation is specified with a color coding system, with green indicating a less than 10% error, yellow indicating a less than 30% error, and red indicating a greater than 30% error when comparing the engine output to the published values. All references are noted in the table.

<center>
*Table 1. Results of the resting physiology validation of the Blood Chemistry System.*
</center>

<b>Standard Male</b>

@insert ./test_results/tables/BloodChemistry-StandardMaleValidationTable.md

<b>Standard Female</b>

@insert ./test_results/tables/BloodChemistry-StandardFemaleValidationTable.md

Overall, the engine meets validation, with all but two of the validation parameters having less than a 10% error when compared to published values.  Only one parameter has an error greater than than 30%. 

Validation - Conditions and Actions
-----------------------
## Hemorrhage
The Blood Chemistry system does not have any conditions or actions directly embedded, but the outputs of Blood Chemistry are dependent on the actions and conditions of other systems. An example is the Hemorrhage insult, an action of the [Cardiovascular](@ref CardiovascularMethodology) System. Figure 2 shows the effects of hemorrhage and fluid resuscitation with saline on the total blood volume and the total hemoglobin for the patient.

<center>
<a href="./plots/BloodChemistry/Volume.jpg"><img src="./plots/BloodChemistry/Volume.jpg" width="1100"></a>
<a href="./plots/BloodChemistry/Hemoglobin.jpg"><img src="./plots/BloodChemistry/Hemoglobin.jpg" width="1100"></a>
<a href="./plots/BloodChemistry/BloodChemistryLegend.jpg"><img src="./plots/BloodChemistry/BloodChemistryLegend.jpg" width="500"></a>
</center>
<center>
*Figure 3. The total blood volume decreases with the hemorrhage, then increases as saline is infused. The total hemoglobin also decreases with the hemorrhage but remains reduced after the hemorrhage is stopped.*
</center><br>

@anchor bloodchemistry-validation-assessments
Validation - Assessments
------------------------
There are three asessments in the Blood Chemistry system. Each assessment provides all of the values associated with a specific "blood test" as a single output. The three assessments currently supported are an arterial blood gas test, a metabolic panel (Chem-14), and a complete blood count. These assessments were validated with published data, as shown in Table 2, Table 3, and Table 4, respectively. As with the resting physiology, the references for all values are provided and the results are color coded. 

<center>
*Table 2. Results of the arterial blood gas test as compiled during the healthy resting physiology.*
</center>

<b>Standard Male</b>

@insert ./test_results/tables/ArterialBloodGasTest-StandardMaleValidationTable.md

<b>Standard Female</b>

@insert ./test_results/tables/ArterialBloodGasTest-StandardFemaleValidationTable.md

<center>
*Table 3. Results of the metabolic panel as compiled during the healthy resting physiology.*
</center>

<b>Standard Male</b>

@insert ./test_results/tables/CompleteMetabolicPanel-StandardMaleValidationTable.md

<b>Standard Female</b>

@insert ./test_results/tables/CompleteMetabolicPanel-StandardFemaleValidationTable.md

<center>
*Table 4. Results of the complete blood count as compiled during the healthy resting physiology.*
</center>

<b>Standard Male</b>

@insert ./test_results/tables/CompleteBloodCount-StandardMaleValidationTable.md

<b>Standard Female</b>

@insert ./test_results/tables/CompleteBloodCount-StandardFemaleValidationTable.md

All of the measures in the two Blood Chemistry assessments meet validation. 

@anchor bloodchemistry-fourCompartment
Four Compartment Test
------------------------
As mentioned above, the Blood Chemistry system serves the primary purpose of storing and relaying information between the other systems. This heavy reliance on other systems can make it difficult to test the funcitonality of blood gas balance. To this end, the scalability of the engine was leveraged to create a simpler system comprised of only four compartments: Pulmonary, to represent the capillaries in the lungs where oxygen and carbon dioxide exchange occurs; Arteries, representing all of the oxygen-rich vasculature running to the tissues; Capillaries, representing the location of oxygen removal from the vasculature; and Veins, representing the section of the vasulature containing oxygen-poor blood. A diagram of this simplified system can be seen in Figure 4 below.

<a href="./Images/BloodChemistry/BloodChemistryFourCompartment.png"><img src="./Images/BloodChemistry/BloodChemistryFourCompartment.png" width="600"></a>
<center>
*Figure 4. The simplified Four Compartment test uses only Pulmonary, Arteries, Capillaries, and Veins to hone in on the functionality in the Blood Chemistry system.*
</center><br>

Pressures, volumes, and substances were initialized to good values (see Table 5 below), and then the simplified Four Compartment system was run as the engine would run the normal, complete model using the Preprocess, Process, Postprocess paradigm. In the Preprocess step, oxygen is removed and carbon dioxide is added in the Capillaries to simulate metabolism while oxygen is added and carbon dioxide is removed in the Pulmonary compartment to represent respiration. If a tissue compartment was present, diffusion could also occur in this stage. In the Process step, circuit calculation and substance transport are done. Then, the Postprocess step moves the "Next" values to "Current". For more information about this paradigm, see @ref CircuitMethodology.

<center>
*Table 5. Initial values for the Four Compartment test. Variables with an asterisk indicate that the values were pulled from a simulation run to a stable point.*
| Variable | Initial Value |
| :---------------- | :---------- |
| Veins Pressure | 4 mmHg @cite Leeuwen2015laboratory |
| Pulmonary Pressure | 10 mmHg @cite Edwards2009pocket |
| Arteries Pressure | 90 mmHg @cite Leeuwen2015laboratory |
| Capillaries Pressure | 30 mmHg |
| Blood Flow | 5600 mL/min @cite guyton2006medical |
| Total Blood Volume | 5 L @cite guyton2006medical |
| Arteries Volume | 23% of Total Blood Volume @cite valentin2002icrp @cite hudsmuth2005heartvolume @cite Edwards2009pocket |
| Veins Volume | 60% of Total Blood Volume @cite valentin2002icrp @cite hudsmuth2005heartvolume @cite Edwards2009pocket @cite effros1967vascular |
| Pulmonary Volume | 12% of Total Blood Volume @cite valentin2002icrp @cite hudsmuth2005heartvolume @cite Edwards2009pocket |
| Capillaries Volume | 5% of Total Blood Volume @cite valentin2002icrp @cite hudsmuth2005heartvolume @cite Edwards2009pocket |
| Albumin Concentration | 45 g/L @cite valtin1995renal |
| Hematocrit | .45 @cite guyton2006medical @cite valtin1995renal |
| Body Temperature | 37 C @cite herman2007physics |
| [SID] | 40.5 mmol/L @cite kellum2009stewart |
| Phosphate | 1.1 mmol/L @cite Leeuwen2015laboratory |
| *Pulmonary and Arteries CO2 Saturation | .0282123  |
| *Pulmonary and Arteries O2 Saturation | .974759  |
| *Pulmonary and Arteries CO2 Concentration | 1.30049 mmol/L  |
| *Pulmonary and Arteries O2 Concentration | .129065 mmol/L  |
| *Pulmonary and Arteries HCO3 Concentration | 25.9377 mmol/L  |
| *Pulmonary and Arteries pH | 7.39982  |
| *Capillaries and Veins CO2 Saturation | .162382  |
| *Capillaries and Veins O2 Saturation | .789701  |
| *Capillaries and Veins CO2 Concentration | 1.40557 mmol/L  |
| *Capillaries and Veins O2 Concentration | .0560892 mmol/L  |
| *Capillaries and Veins HCO3 Concentration | 26.1182 mmol/L  |
| *Capillaries and Veins pH | 7.36909 @cite valentin2002icrp |
| Oxygen Exchange Rate | 5954.2 ug/S @cite guyton2006medical |
| Carbon Dioxide Exchange Rate | 6140.4 ug/S @cite guyton2006medical |
</center>

Because the Four Compartment test is initialized to good values, and because of the design of the engine and Blood Chemistry system, output values should be within physiological ranges. Indeed, the outputs shown below reflect the proper function of the Blood Chemistry system.
<center>
*Table 6. Stable results of the Four Compartment test.*
Variable			|	Four Compartment Test Ending Value	|	Valid Value	|
------------------------	------------------------	------------------------	|	------------------------	|	------------------------	|
Arteries HCO3			|	0.0158445	|<span class="success">	[0.134, 0.159] g/dL @cite valtin1995renal	</span>|
Arteries CO2			|	41.9369	|<span class="success">	40 mmHg @cite guyton2006medical	</span>|
Arteries O2			|	108.012	|<span class="success">	95 mmHg @cite guyton2006medical	</span>|
Arteries pH			|	7.39498	|<span class="success">	7.2 @cite valentin2002icrp [7.35, 7.4] @cite guyton2006medical	</span>|
Capillaries HCO3			|	0.0159359	|<span class="success">	[0.134, 0.159] @cite valtin1995renal	</span>|
Capillaries pH			|	7.36949	|<span class="success">	7.2 @cite valentin2002icrp [7.35, 7.4] @cite guyton2006medical	</span>|
Pulmonary HCO3			|	0.0158445	|<span class="success">	[0.134, 0.159] @cite valtin1995renal	</span>|
Pulmonary CO2			|	41.9369	|<span class="success">	40 mmHg @cite guyton2006medical	</span>|
Pulmonary O2			|	108.012	|<span class="success">	104 mmHg @cite guyton2006medical	</span>|
Pulmonary pH			|	7.39498	|<span class="success">	7.2 @cite valentin2002icrp [7.35, 7.4] @cite guyton2006medical	</span>|
Veins HCO3			|	0.0159359	|<span class="success">	[0.134, 0.159] @cite valtin1995renal	</span>|
Veins CO2			|	44.7292	|<span class="success">	45 mmHg @cite guyton2006medical	</span>|
Veins O2			|	39.2399	|<span class="success">	40 mmHg @cite guyton2006medical	</span>|
Veins pH			|	7.36949	|<span class="success">	7.2 @cite valentin2002icrp [7.35, 7.4] @cite guyton2006medical	</span>|
Total Hemoglobin			|	765.0004356	|<span class="success">	[661.5, 835.2] g @cite guyton2006medical @cite onofrio1995sim	</span>|
</center>

@anchor bloodchemistry-conclusions
Conclusions
-----------
The current values output from the Blood Chemistry System provide accurate results for a clinical overview of patient health. Blood gases and hemoglobin (multiple types) are circulated using the transport methodology discussed in the @ref CircuitMethodology and the @ref TissueMethodology. The Blood Chemistry System is a powerful tool for validation of multiple systems within the engine. The system is able to illustrate the dependencies each system has on another, i.e., if the %Respiratory System is unable to provide oxygen, a shortage of oxygen in the blood stream results, reducing both the arterial oxygen partial pressure and the oxygen saturation as displayed by the Blood Chemistry system. Future work will focus on adding additional key substances to provide a more complete assessment of a patient's overall health.

@anchor bloodchemistry-future
Future Work
===========

Coming Soon
-----------
Planned improvements to other systems will have an indirect effect on Blood Chemistry. For example, a planned improvement to the vascular tone model in the [Cardiovascular](@ref CardiovascularMethodology) system will reduce the resistance of the blood vessels supplying the myocardium, simulating dilation. This improvement to homeostatic feedback will make the heart muscle more robust to an oxygen deficit, enabling a physiologically accurate threshold for the myocardium hypoxia event. Additionally, major work has already been completed toward a more accurate and complete handling of all substances within the engine. This work will allow for a direct computation of the ion concentrations and the strong ion difference, enabling mechanistic simulation of acid-base disturbances and compensation.

Recommended Improvements
------------------------
The exclusion of blood cellular elements prohibits the implementation of a variety of mechanistic models of physiologic processes, including coagulation, infectious disease, and hematopoiesis to name a few. 
- Inclusion of RBC, platelets, lymphocytes, monocytes, neutrophils, eosinophils, and basophils as blood constituents.
- Adaptive total hemoglobin (for example, increasing hemoglobin with exposure to low-oxygen pressure environments)
- Myoglobin substance models
	- Including interactions with oxygen and carbon monoxide

@anchor bloodchemistry-appendices
Appendices
==========

@anchor bloodchemistry-symbols
Acronyms and Symbols
--------
<center>
*Table 7. List of acronyms and symbols.*
| Symbol or Acronym | Description |
| :---------------- | :---------- |
| *A<sup>-</sup>* | The sum of all weak anions that are the conjugate base of all non-volatile weak acids |
| *Alb* | Albumin |
| ALP | Alkaline Phosphatase |
| ALT | Alanine Aminotransferase |
| AST | Aspartate Aminotransferase |
| *Ca<sup>2+</sup>* | Calcium (strong cation) |
| CBC | Complete Blood Count |
| *Cl<sup>-</sup>* | Chloride (strong anion) |
| *CO<sub>2</sub>* | Carbon dioxide gas |
| *HCO<sub>3</sub><sup>-</sup>* | Bicarbonate  |
| *Hct* | Hematocrit  |
| *Hgb* | Hemoglobin  |
| *I<sup>n+</sup>* | Represents a lumped set of unnamed cations (e.g. Mg*^{2+}* + Cu*^{2+}* + Fe*^{3+}* + ... ) |
| *I^<sup>n-</sup>* | Represents a lumped set of unnamed anions (e.g. sulfate + beta-hydroxybutyrate + ... )  |
| *K<sup>+</sup>* | Potassium (strong cation)  |
| *Ket<sup>-</sup>* | Ketone body (i.e. acetoacetate, beta-hydroxybutyrate, and acetone)  |
| *La<sup>-</sup>* | Lactate (strong anion)  |
| mmHg | Millimeters Mercury |
| *Na<sup>+</sup>* | Sodium (string cation)  |
| *O<sub>2</sub>* | Oxygen gas  |
| *OH<sup>-</sup>* | Hydroxide ion  |
| *P<sub>i</sub>* | Phosphate  |
| *P<sub>CO<sub>2</sub></sub>* | Partial pressure of carbon dioxide  |
| *P<sub>O<sub>2</sub></sub>* | Partial pressure of oxygen  |
| *pH* | Negative log of the hydrogen ion concentration  |
| *S<sub>CO<sub>2</sub></sub>* | Fraction of hemoglobin binding sites with a bound carbon dioxide molecule  |
| *S<sub>O<sub>2</sub></sub>* | Fraction of hemoglobin binding sites with a bound oxygen molecule  |
| *SID* | Strong Ion Difference,  defined as [SID] = [strong cations] - [strong anions]  |
| *T* | Temperature  |
| *T<sub>CO<sub>2</sub></sub>* | Total carbon dioxide  |
| *T<sub>O<sub>2</sub></sub>* | Total oxygen  |
| <i>&alpha;<sub>CO<sub>2</sub></sub></i> | The plasma *CO_2* solubility coefficient  |
| <i>&alpha;<sub>O<sub>2</sub></sub></i> | The plasma *O_2* solubility coefficient  |
</center>

Data Model Implementation
-------------------------
@ref BloodChemistrySystemTable "BloodChemistry"
