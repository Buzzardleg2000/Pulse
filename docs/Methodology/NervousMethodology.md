%Nervous Methodology {#NervousMethodology}
==========================

@anchor nervous-overview
Overview
========
Abstract
--------
The %Nervous System is a preliminary system that will represent the central %nervous system (CNS) and peripheral %nervous system (PNS). Currently, the system includes a baroreceptor feedback model that is used to regulate arterial pressure. This is accomplished by tracking the normalized changes in important cardiovascular circuit parameters, such as heart rate, vascular resistance, and vascular compliance. The normalized changes are communicated to the %Cardiovascular System, where they are adjusted in an attempt to return arterial pressure to its normal value. The model was validated with an acute hemorrhage scenario. The baroreceptor feedback shows good agreement with the expected trends. Additionally, the %Nervous System provides for the tracking of intracranial pressure and cerebral blood flow for examination in traumatic brain injury events and pupillary size and reactivity for brain injuries and drug effects. In the future, other feedback mechanisms will be added for both homeostasis or crisis states.

@anchor nervous-introduction
Introduction
------------
The %Nervous System in the human body is comprised of the central nervous system (CNS) and the the peripheral %nervous system (PNS). The CNS contains the brain and the spinal cord, while the PNS is subdivided into the somatic, autonomic, and enteric nervous systems. The somatic system controls voluntary movement. The autonomic nervous system is further subdivided into the parasympathetic and sympathetic nervous system. The sympathetic nervous system is activated in times of stress and crisis, commonly referred to as the "flight or fight" behaviors. The parasympathetic nervous system controls homeostasis functions during the relaxed state. The enteric nervous system controls the gastrointestinal system. Many of these behaviors are tightly connected to the %Endocrine System. In many cases, the %Nervous System receptors trigger hormone releases that cause systemic changes, such as dilation and constriction of vessels, heart contractility changes, and respiration changes @cite guyton2006medical. 

%The engine will focus on a select few mechanisms within the %Nervous System to provide feedback for both homeostasis and crisis behavior in the body. The list is shown below:
 - Baroreceptors
 - Chemoreceptors
 - Local autoregulation
 - Temperature Effects
 - Generic parasympathetic and sympathetic effects
 
 Additionally, the engine will model the following features related to brain function: 
 - Simple model of traumatic brain injury (TBI)
 - Intracranial pressure
 - Pupillary response to drugs and TBI

@anchor nervous-system-design
System Design
=============
Background and Scope
--------------------
#### Baroreceptor Feedback
The baroreceptor mechanism provides rapid negative feedback control of arterial pressure. A drop in arterial pressure is sensed by the baroreceptors and leads to an increase in heart rate, heart elastance, and a decrease in systemic vascular resistance. These changes operate with the goal maintaining arterial pressure at its healthy resting level. The baroreceptor mechanism may be divided into three parts: afferent, CNS, and efferent. The afferent part contains the receptors, which detect changes in the MAP. After that, the CNS portions the response into either sympathetic or parasympathetic. Lastly, the efferent part is used to provide feedback to the vasculature and organs within the %Cardiovascular System @cite ottesen2004applied. The model chosen only models the CNS and efferent portion of the baroreceptors. The fidelity of the model does require the modeling of the actual stretch receptors in the aorta or carotid arteries to provide the necessary feedback. Instead the changes in MAP are used to calculate changes in parasympathetic and sympathetic responses, which trigger the changes in the %Cardiovascular System.

#### TBI
Traumatic brain injuries are relatively common, affecting millions annually. They are usually defined as a blunt or penetrating injury to the head that disrupts normal brain function. Furthermore, they are classified as either focal (e.g. cerebral contusions, lacerations, and hematomas) or diffuse (e.g. concussions and diffuse axonal injuries) @cite adoni2007pupillary. The scope of the TBI model is somewhat limited by the low fidelity of the modeled brain. The brain is represented in the current %Cardiovascular circuit with only two resistors and a compliance, all within one compartment. Because the circuit is modeled in this way, TBI is considered as an acute, non-localized, non-penetrating injury from a circuit perspective. Thus, the TBI model can account for intracranial pressure and cerebral blood flow on the basis of the whole brain, but not to specific areas of the brain. However, the engine does provide for three injury inputs: diffuse, right focal, and left focal. These will be discussed in the @ref nervous-actions "Actions" section. Similarly to the @ref RenalMethodology "Renal System", the brain circuit could be expanded to accommodate a higher-fidelity brain model.

#### Pupillary Response
The pupil is the small hole in the iris that allows light to pass through the lens, allowing vision. The pupil's diameter is controlled by the autonomic nervous system in order to affect the amount of light entering the eye. This diameter control is carried out by two muscles: the pupilloconstrictor, which is parasympathetically controlled (via the neurotransmitter acetylcholine), and the pupillodilator, which is sympathetically controlled (via the neurotransmitter norepinephrine) @cite adoni2007pupillary. The synaptic pathways for these two pupil effects differ, and thus, any deviation from normal pupil behavior can shed light on any interference, whether that be a brain injury causing increased pressure on a nerve or a drug effect interfering with synaptic transmission. Because pupil examination is informative yet minimally invasive, it is an excellent tool for helping to diagnose neurological problems. The PERRLA assessment is commonly used to classify pupillary behavior: Pupils Equal, Round, Reactive to Light, and Accomodating. Pupillary response is modeled by tracking any modification to pupil size and light reactivity for each eye. Both TBI @ref nervous-actions "actions" and @ref drugs-pharmacodynamics "drugs" can apply modifiers to pupil size and reactivity.

Data Flow
---------
An overview of the data flow in the %Nervous system is shown in Figure 1.

### Initialization and Stabilization
The engine initialization and stabilization is described in detail in the [stabilization section](@ref system-stabilization) of the @ref SystemMethodology report. The mean arterial pressure set-point is updated after the %Cardiovascular system reaches a homeostatic state.

### Preprocess
#### Baroreceptor Feedback
The baroreceptor feedback is calculated here. It calculates the difference between the current MAP and the recorded set point to determine modifiers for the important %cardiovascular circuit parameters (heart rate, heart elastance, systemic resistance, and venous compliance).

### Process
#### Check Brain Status
Intracranial pressure is checked here to determine whether or not to throw the Intracranial Hypertension or Intracranial Hypotension events.

#### SetPupilEffects
Pupil modifiers are retrieved from the %Drugs System and combined with any modifiers from TBI before being applied to the eyes.

### Post Process
There is no system specific functionality for the %Nervous System in Post Process.

@anchor nervous-assessments
### Assessments
Assessments are data collected and packaged to resemble a report or analysis that might be ordered by a physician. No assessments are associated with the %Nervous System.

@anchor nervous-features
Features, Capabilities, and Dependencies
----------------------------------------
### Baroreceptors
The baroreceptor model implemented is adapted from the models described by Ottesen et al @cite ottesen2004applied. The model is used to drive the current MAP towards the resting set-point of the patient. This is accomplished by calculating the sympathetic and parasympathetic response. The fraction of the baroreceptor response that corresponds to the sympathetic effects is determined from Equation 1. The parsympathetic fraction is shown in Equation 2.

\f[ \eta_{s} (P_{a}) = \left[ 1+ \left( \frac{P_{a}}{P_{a,setpoint}} \right)^{ \nu} \right]^{-1} \f]
<center>
*Equation 1.*
</center><br>

\f[ \eta_{p} (P_{a}) = \left[ 1+ \left( \frac{P_{a}}{P_{a,setpoint}} \right)^{- \nu} \right]^{-1} \f]
<center>
*Equation 2.*
</center><br>

Where &nu; is a parameter that represents the response slope of the baroreceptors, <b>p</b><sub>a</sub> is the current MAP, and <b>p</b><sub>a,setpoint</sub> is the MAP set-point. An example of the sympathetic and parasympathetic responses as a function of MAP are shown in Figure 1. These were calculated with an assumed MAP set-point of 87 mmHg. The model in @cite ottesen2004applied uses an &nu value of 1, which worked well in an isolated system as shown in Figure 1. However, when integrated into the whole-body physiology model, this was unable to account for the accumulated response of the baroreceptors. For example, as the MAP increases, the sympathetic response increases, however, the effects of the sympathetic response drop the MAP. At the next time step, the sympathetic response will drop. In reality the response is required to maintain this effect, but the constant loop of feedback obscures the needed sympathetic response. To combat this, we increased the value of &nu to 4.

@htmlonly
<center>
<a href="./plots/Nervous/Response_Fractions.jpg"><img src="./plots/Nervous/Response_Fractions.jpg" width="550"></a><br>
</center>
@endhtmlonly
<center>
<i>Figure 1. The sympathetic and parasympatheric response fractions are displayed as a function of mean arterial pressure (MAP). Both fractional forms show asymptotic behavior as divergence from the MAP set-point occurs. The response fractions are additive, always summing to a value of 1.0. At homeostasis (MAP equal to the set-point), the fractions are both equal to 0.5.</i>
</center><br>

As described in the [cardiovascular](@ref cardiovascular-initialize) methodology report, the %cardiovascular system is initialized according to patient definitions and the stabilized to a homeostatic state. The set-point is the resultant mean arterial pressure following the engine stabilization period. The set-point is adjusted dynamically with certain actions and insults, as shown in Equation 3.

\f[ P^{n+1}_{a,setpoint} = P^{n}_{a,setpoint} + \Delta P_{a,drugs} + \Delta P_{a,exercise} \f]
<center>
*Equation 3.*
</center><br>

Where &Delta;<b>p</b><sub>a,drugs</sub> and &Delta;<b>p</b><sub>a,exercise</sub> are changes in MAP due to drugs and exercise, respectively. 

The sympathetic and parasympathetic fractional responses are used to determine the response of the following %cardiovascular parameters:
- heart rate
- heart elastance
- systemic vascular resistance
- systemic vascular compliance
This is accomplished by tracking the time-dependent values of each parameter relative to their value at the MAP set-point. The time-dependent behavior of these parameters is presented via a set of ordinary differential equations, as shown in Equations 4-7.

\f[ \frac{dx_{HR}}{dt} = \left(- \frac{1}{ \tau_{HR}} \right) \left( -x_{HR} + \alpha_{HR} \eta_{s}(P_{a}) + \beta_{HR} \eta_{p}(P_{a}) + \gamma_{HR} \right) \f]
<center>
*Equation 4.* 
</center><br>

\f[ \frac{dx_{E}}{dt} = \left(- \frac{1}{ \tau_{E}} \right) \left( -x_{E} + \alpha_{E} \eta_{s}(P_{a}) + \gamma_{E} \right) \f]
<center>
*Equation 5.*
</center><br>

\f[ \frac{dx_{R}}{dt} = \left(- \frac{1}{ \tau_{R}} \right) \left( -x_{R} + \alpha_{R} \eta_{s}(P_{a}) + \gamma_{R} \right) \f]
<center>
*Equation 6.*
</center><br>

\f[ \frac{dx_{C}}{dt} = \left(- \frac{1}{ \tau_{C}} \right) \left( -x_{C} + \alpha_{C} \eta_{p}(P_{a}) + \gamma_{C} \right) \f]
<center>
*Equation 7.*
</center><br>

Where x<sub>HR</sub>, x<sub>E</sub>, x<sub>R</sub> and x<sub>C</sub> are the relative values of heart rate, heart elastance, vascular resistance and vascular compliance, respectively. &tau;<sub>HR</sub>, &tau;<sub>E</sub>, &tau;<sub>R</sub> and &tau;<sub>C</sub> are the time constants for heart rate, heart elastance, vascular resistance and vascular compliance, respectively. The remaining &alpha;, &beta; and &gamma; parameters are a set of tuning variables used to achieve the correct responses in the %Cardiovascular System during arterial pressure shifts. Note that the heart rate feedback is a function of both the sympathetic response and parasympathetic response, whereas the elastance feedback and vascular tone feedback depend on the sympathetic or parasympathetic responses individually. Figure 2 shows the normalized response curves.

@htmlonly
<center>
<table border="0">
<tr>
    <td><a href="./plots/Nervous/Normalized_HeartElastance.jpg"><img src="./plots/Nervous/Normalized_HeartElastance.jpg" width="550"></a></td>
    <td><a href="./plots/Nervous/Normalized_HeartRate.jpg"><img src="./plots/Nervous/Normalized_HeartRate.jpg" width="550"></a></td>
</tr>
<tr>
    <td><a href="./plots/Nervous/Normalized_SystemicCompliance.jpg"><img src="./plots/Nervous/Normalized_SystemicCompliance.jpg" width="550"></a></td>
    <td><a href="./plots/Nervous/Normalized_SystemicResistance.jpg"><img src="./plots/Nervous/Normalized_SystemicResistance.jpg" width="550"></a></td>
</tr>
</table><br>
</center>
@endhtmlonly
<center>
<i>Figure 2. The plot array demonstrates the normalized organ responses to sympathetic or parasympathetic activity, plotted against the normalized mean arterial pressure.</i>
</center><br>

This model works well for smaller increases in MAP. However, the baroreceptors reach a saturation level and can no longer continue to apply the same level of change to the achieve a MAP response. This can be seen in the case of hemorrhage. Lower levels of hemorrhage result in the ability to fully maintain MAP with the above changes to parameters to the %Cardiovascular System. However, as the blood pressure continues to drop, the baroreceptors become less effective. To account for this, the model includes a saturation parameter. When the sympathetic response reaches 0.78, an event is triggered for baroreceptor saturation. After the baroreceptors reach saturation, Equations 4-7 are scaled to by a baroreceptor effectiveness parameter to reduce the response. When the MAP reaches a level of approximately 45 mmHg, the body responds with a "last-ditch" response. This is a strong burst of baroreceptor activity to attempt to sustain cardiovasclar function @cite guyton2006medical. The baroreceptor effectiveness parameter is increased for a MAP between 40 and 45 mmHg to represent this response. Below 40mmHg, the baroreceptor effectiveness rapidly falls contributing to cardiovascular collapse. The best example of this response is the baroreceptor role in hemorrhage, particularly the role in the cascade to through hemorrhagic (hypovolemic) shock @cite guyton2006medical @cite Batchinsky2007sympathetic.

It is also important to consider the second order effects of the baroreceptor response. The literature shows that the baroreceptors become less effective not only as the MAP deviates further from its baseline, but also as the response extends through time @cite Sheriff2006editorial @cite Drummond1996acute, @cite Dampney2017resetting. This sustained response is not feasible for long periods of time and barorector resetting occurs. A Baroreceptor Active event is triggered when the pressure diviates from the setpoint by plus or minus 5%. After 7 minutes of continuous baroreceptor activation, the MAP setpoint is modified in the direction of the diviation by 35% of the deviation. This is key for modeling longer scenarios where compensatory mechanisms begin to fail. 

### Chemoreceptors
The chemoreceptors are chemosensitive cells that are sensitive to reduced oxygen and excess carbon dioxide. Excitation of the chemoreceptors stimulates the sympathetic nervous system. The chemoreceptors contribute significantly to the control of respiratory function, and they are included in the [respiratory control model](@ref respiratory-chemoreceptors) developed. As sympathetic activators, the chemoreceptors also increase the heart rate and contractility. The complete mechanisms of chemoreceptor feedback are complicated and beyond the current scope of the engine, so a phenomenological model was developed to elicit an appropriate response to hypoxia and hypercapnia. Only the heart rate effects of chemoreceptor stimulation are modeled in the current version of the engine, but contractility modification will be included in a future release.  Figure 3 shows the chemoreceptor effect on heart rate. In the figure, the abscissa values represent a fractional deviation of gas concentration from baseline, and the ordinate values show the resultant change in heart rate as a fraction of the baseline heart rate. The final heart rate modification due to chemoreceptors is the sum of the oxygen and carbon dioxide effects. For example, severe hypoxia and severe hypercapnia will result in a three-fold increase in heart rate (baseline + 2 * baseline).

@htmlonly
<center>
<a href="./plots/Nervous/ChemoreceptorsModifiers.jpg"><img src="./plots/Nervous/ChemoreceptorsModifiers.jpg" width="600"></a><br>
</center>
@endhtmlonly
<center>
<i>Figure 3. The chemoreceptor model is a phenomenological model which elicits a tuned response to hypoxia and/or hypercapnia. A reverse effect is also present, but at a much lesser magnitude.</i>
</center><br>

### TBI
Three important metrics are used to evaluate patients with traumatic brain injuries: intracranial pressure (ICP), cerebral blood flow (CBF), and cerebral perfusion pressure (CPP). Patients with brain injuries often exhibit increased intracranial pressure, decreased cerebral blood flow, and a cerebral perfusion pressure outside of a normal range @cite steiner2006monitoring. Cerebral perfusion pressure is defined as

\f[ CPP = MAP - ICP \f]
<center>
*Equation 8.*
</center><br>

 Where MAP is the mean arterial pressure. In order to model these behaviors, the Brain Injury action will modify the resistors of the brain circuit, which is shown in Figure 4 below. The brain circuit is a section of the @ref cardiovascular-features "cardiovascular circuit".

@htmlonly
<center>
<a href="./Images/Nervous/BrainCircuit.png"><img src="./Images/Nervous/BrainCircuit.png" width="550"></a><br>
</center>
@endhtmlonly
<center>
<i>Figure 4. The brain is represented by two resistors and a compliance. The upstream resistor, R1, is connected to the aorta, and the downstream resistor, R2, is connected to the vena cava.</i>
</center><br>

By increasing R1 and R2, the ICP can be increased while CBF decreases. The resistors are tuned based on the severity (on a scale from 0 to 1) of TBI such that ICP is above 25 mmHg and CBF is near 8 mL per 100 grams of brain tissue per minute for the most severe injury.

### Pupillary Response
Pupil size and reactivity can vary patient-to-patient based on many factors, including age, mental and emotional state, and ambient light conditions @cite winn1994factors. Furthermore, pupillary assessments like PERRLA are often qualitative rather than quantitative. Because of this, the engine models pupillary modifiers rather than discrete pupil sizes and size changes over time. In this way, the pupillary effects can be assessed qualitatively without the need for baseline patient values for pupil size and reactivity.

There are two ways to affect the pupils in engine: drug pharmacodynamic effects and TBI. For a discussion of pharmacodynamic effects on pupillary response, see @ref drugs-pharmacodynamics "Drugs Methodology". The other method of influencing pupillary response is TBI. Increasing intracranial pressures constrict ocular nerves, causing pupillary disruptions @cite kingsly2012severe. Because of this relationship between ICP and pupil effects, ICP is the metric on which pupillary modifiers are based. For the pupil size modifier, Equation 9 was developed so that pupil size (m<sub>s</sub>) begins to see effects when ICP increases above around 20 mmHg, the pressure at which recoverable brain damage is often observed @cite steiner2006monitoring, hitting its maximum slope at 22.5 mmHg, and leveling off as ICP approaches 25 mmHg. For pupil light reactivity (m<sub>r</sub>, the curve seen in Equation 10 was developed so that it remains at 0 until ICP approaches 20 mmHg, then drops off sharply towards -1 as ICP approaches 25 mmHg. For both of these modifiers, a 1 represents "maximal" effect, -1 represents "minimal" effect, and 0 represents no effect. So a pupil size modifier of 1 would mean the pupil size is at its maximum possible diameter.

\f[m_s=1/(1+(e^{-2.3\left(ICP-22.5\right)})\f] 
<center>
*Equation 9.*
</center><br>

\f[m_{r} =-.001*10^{.3(ICP-15)} \f] 
<center>
*Equation 10.*
</center><br>

For diffuse injuries, these modifiers are applied to both eyes. For focal injuries, the modifiers are applied to the eye on the same side as the injury, as this reflects the most common observed behavior @cite kingsly2012severe.

@anchor nervous-variability
### Patient Variability
The baroreceptor reflex gains and time constants are independent of patient configuration. However, set-points are computed after stabilization, and the baroreceptor reflex drives towards a configuration-specific homeostasis. Because TBI uses multiplicative factors to modify the brain section of the cardiovascular circuit, the patient variability of TBI is the same as that seen in the %Cardiovascular System. There is no patient variability inherent in the pupillary response model, as it uses modifiers instead of baseline values. A detailed discussion of patient configuration and variability is available in the @ref PatientMethodology report.
@anchor nervous-dependencies
### Dependencies
The baroreceptors interact directly with the %cardiovascular system by modifying %cardiovascular circuit and heart driver parameters. These include heart rate, left and right heart elastance, systemic vascular resistance and systemic vascular compliance. The %cardiovascular parameters are modified by first calculating the normalized change in the parameter. This normalized change is passed into the common data model (CDM) where the %cardiovascular system may extract the change and then apply it locally as a fractional adjustment. The resultant hemodynamic changes feedback directly to the MAP, and the updated MAP is used by the %Nervous system in the next time slice to compute new normalized baroreceptor effects. TBI also directly modifies the cardiovascular circuit, specifically both the upstream and downstream brain resistors. Finally, the %Drugs system can contribute additional pupillary effects, that combine with possible TBI pupil effects via simple summation.

@anchor nervous-assumptions
Assumptions and Limitations
---------------------------
The limitation of the baroreceptor model implemented is not tracking adjustments in unstressed volume. Currently the engine does not correctly represent the physiologic unstressed volume under resting conditions. Therefore, these changes to the unstressed volume cannot be reflected in the engine. This may be addressed in future releases of the engine.

The TBI model assumes an acute injury to the brain; chronic effects are not considered, and neither are consciousness assessments. The brain is considered to be one compartment and is not subdivided into discrete functional areas. This means that there is no circuit distinction between a diffuse TBI and a Left Focal TBI. The only difference between the TBI actions is the expected pupil effect. TBI metric validation assumes a supine adult. The current TBI model does not consider specific effects of CO<sub>2</sub> saturation, O<sub>2</sub> saturation, or blood pH on CBF.

@anchor nervous-actions
Actions
-------

### Insults

#### Brain Injury
The Brain Injury action refers to an acute injury to the brain. Whereas in a real patient, a TBI could have manifold effects, both acute and chronic, the model only considers the effects of TBI on brain vascular resistance and pupillary response. The effects of the Brain Injury action depend on the severity value from 0 to 1 and a Type (Diffuse, Left Focal, or Right Focal) assigned to the injury. A severity of 0 will result in a multiplicative factor of 1 being applied to both the upstream and downstream resistors in the brain, which equates to no deviation from the normal, uninjured state. For an injury severity of 1, the upstream resistor is assigned a multiplicative factor of 4.775 and the downstream resistor is assigned a multiplicative factor of 30.409. Severity values between 0 and 1 are converted to multiplicative factors linearly. Increased flow resistance results in increased ICP and decreased CBF. A Diffuse-type injury affects both eyes equally, whereas a Left Focal injury affects only the left eye, and a Right Focal injury affects only the right eye.

@anchor nervous-events
Events
------

### Intracranial Hypertension
Intracranial Hypertension is triggered when the intracranial pressure exceeds 25 mmHg. The event is reversed when the pressure returns below 24 mmHg.

### Intracranial Hypertension
Intracranial Hypotension is triggered when the intracranial pressure drops below 7 mmHg. The event is reversed when the pressure returns above 7.5 mmHg.

@anchor nervous-results
Results and Conclusions
=======================

Validation - Resting Physiologic State
--------------------------------------
No resting state physiology validation was completed, because the baroreceptors do not provide feedback during the initial resting and chronic stabilization phases. For more information on the stabilization phases, see @ref SystemMethodology.

Validation - Actions and Conditions
--------------------

Actions and conditions with responses specific to the Nervous System were validated. A summary of this validation is shown in Table 2. More details on each individual scenario's validation can be found below.

<center>
*Table 2. Cumulative validation results for Nervous specific conditions and actions scenarios.*
</center>

|	Key	|
|	---	|
|<span class="success">	Good agreement: correct trends or <10% deviation from expected	</span>|
|<span class="warning"> 	Some deviation: correct trend and/or <30% deviation from expected	</span>|
|<span class="danger">	Poor agreement: incorrect trends or >30% deviation from expected	</span>|

|	Scenario 	|	Description	|	Good	|	Decent	|	Bad	|
|	---	|	---	|	---	|	---	|	---	|
|	Baroreceptors	|	Patient has 10% blood loss in 30s. View response in heart rate, contractility, systemic vascular resistance, and venous compliance.	|<span class="success">	3	</span>|<span class="warning">	0	</span>|<span class="danger">	0	</span>|
|	Brain Injury (TBI)	|	Patient has three brain injuries of increasing severity with recovery in between. View response in intracranial pressure, cerebral blood flow, and cerebral perfusion pressure.	|<span class="success">	23	</span>|<span class="warning">	2	</span>|<span class="danger">	0	</span>|
|		|	Total	|<span class="success">	26	</span>|<span class="warning">	2	</span>|<span class="danger">	0	</span>|


### Baroreceptor Reflex
The baroreceptor reflex is validated through simulation of an acute hemorrhage scenario. This scenario begins with the healthy male patient. An hemorrhage is initiated and proceeds through hemorrhagic shock to patient death. The cardiac output and the MAP are shown in Figure 5 for both the Pulse model and the experimental data in @cite guyton2006medical. This scenario and validation shows the baroreceptor activation and the effectiveness changes throughout the range of MAP changes important in hemorrhage. More details can be found in the @CardiovascularMethodology.

@htmlonly
<center>
<table>
<tr>
<td><a href="./plots/Cardiovascular/MeanArterialPressureShock.jpg"><img src="./plots/Cardiovascular/MeanArterialPressureShock.jpg" width="550"></a>
</td>
<td><a href="./plots/Cardiovascular/CardiacOutputShock.jpg"><img src="./plots/Cardiovascular/CardiacOutputShock.jpg" width="550"></a>
</td>
</tr>
</table>
<br>
</center>
@endhtmlonly
<center>
<i>Figure 5. Normalized mean arterial pressure and cardiac output as blood loss increases for the Pulse model (left) and the validation data @cite guyton2006medical (right).</i>
</center><br>
<i>Table 3. The baroreceptor reflex validation data for the hemorrhage scenario shows good agreement with expected results.</i>
</center>

|	Action	|	Notes	|	Sampled Scenario Time (s)	|	Heart Rate (beats/min)	|	Cardiac Output (mL/min)	|	Systemic Vascular Resistance (mmHg s/mL)	|
|	---	|	---	|	---	|	---	|	---	|	---	|
|	Hemorrhage	|	10% blood loss in 30 s	|	200	|<span class="success">	Increase ~30% @cite Hosomi1979effect @cite Ottesen2004applied	</span>|<span class="success">	Decrease ~15-20% @cite Hosomi1979effect @cite Ottesen2004applied	</span>|<span class="success">	Increase 10-15% @cite Hosomi1979effect @cite Ottesen2004applied	</span>|


### Brain Injury
The Brain Injury action is validated through repeated application and removal of increasing severities of TBI. The scenario begins with a healthy male patient. After a short time, a mild brain injury (Severity = 0.2, Type = Diffuse) is applied, and the patient is allowed to stabilize before the injury state is removed (only one TBI action can be in effect at a time, so adding a Diffuse Severity 0 TBI removes all TBI effects). This process is repeated for a more severe injury (Severity = 0.75, Type = Left Focal) and severe (Severity = 1, Type = Right Focal) brain injury. We expect to see increases in ICP, with the most severe injury resulting in an ICP greater than 25 mmHg, and decreases in CBF, with the most severe case approaching 8 mL per 100 grams of brain per minute, which, for the validated patient, equates to 108 mL per minute @cite bergeronSME @cite steiner2006monitoring. The scenario shows good agreement for these values. We expect CPP to either increase above its maximum normal value or decrease below its minimum normal value, but, though we see a drop, it isn't quite as pronounced as expected @cite steiner2006monitoring. We can also see that for the low severity injury, ICP doesn't quite reach the threshold to strongly affect the pupils. For the Left Focal injury, only the left pupil is affected, and for the Right Focal injury, only the right pupil is affected.

@htmlonly
<center>
<table>
<tr>
<td><a href="./plots/Nervous/TBI_ICP.jpg"><img src="./plots/Nervous/TBI_ICP.jpg" width="550"></a>
</td>
<td><a href="./plots/Nervous/TBI_CBF.jpg"><img src="./plots/Nervous/TBI_CBF.jpg" width="550"></a>
</td>
</tr>
<tr>
<td><a href="./plots/Nervous/TBI_MAP.jpg"><img src="./plots/Nervous/TBI_MAP.jpg" width="550"></a>
</td>
<td><a href="./plots/Nervous/TBI_CPP.jpg"><img src="./plots/Nervous/TBI_CPP.jpg" width="550"></a>
</td>
</tr>
</table><br>
<a href="./plots/Nervous/TBILegend.jpg"><img src="./plots/Nervous/TBILegend.jpg" width="800"></a><br>
</center>
@endhtmlonly
<center>
<i>Figure 6. Traumatic brain injury response at three different severity levels.</i>
</center><br>

@htmlonly
<center>
<a href="./plots/Nervous/TBI_PupilICP.jpg"><img src="./plots/Nervous/TBI_PupilICP.jpg" width="1200"></a><br>
<a href="./plots/Nervous/TBI_LeftSize.jpg"><img src="./plots/Nervous/TBI_LeftSize.jpg" width="1200"></a><br>
<a href="./plots/Nervous/TBI_LeftReactivity.jpg"><img src="./plots/Nervous/TBI_LeftReactivity.jpg" width="1200"></a><br>
<a href="./plots/Nervous/TBI_RightSize.jpg"><img src="./plots/Nervous/TBI_RightSize.jpg" width="1200"></a><br>
<a href="./plots/Nervous/TBI_RightReactivity.jpg"><img src="./plots/Nervous/TBI_RightReactivity.jpg" width="1200"></a><br>
<a href="./plots/Nervous/PupilLegend.jpg"><img src="./plots/Nervous/PupilLegend.jpg" width="1200"></a><br>
</center>
@endhtmlonly
<center>
<i>Figure 7. Pupillary response to the same TBI scenario as shown in Figure 4 where increasing severities are applied first as Diffuse, then as Left Focal, then as Right Focal.</i>
<br>
<i>Table 4. The validation data for the TBI scenario shows good agreement with expected results.</i>
</center>

|	Action	|	Notes	|	Action Occurrence Time (s)	|	Sampled Scenario Time (s)	|	Intracranial Pressure (mmHg)	|	Cerebral Blood Flow (mL/min)	|	Cerebral Perfusion Pressure (mmHg)	|	Heart Rate (1/min)	|	Respiration Rate (1/min)	|
|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|	---	|
|	Brain Injury	|	Severity 0.2, CPP=MAP-ICP, StandardMale brain mass=1450g	|	20	|	600	|<span class="success">	10% Increase @cite bergeronSME	</span>|<span class="success">	Decrease @cite steiner2006monitoring	</span>|<span class="success">	Decrease @cite balestreri2006impact	</span>|<span class="success">	0-10% Decrease @cite bergeronSME	</span>|<span class="success">	0-10% Decrease @cite bergeronSME	</span>|
|	Brain Injury	|	Severity 0	|	620	|	900	|<span class="success">	7-15 mmHg @cite steiner2006monitoring	</span>|<span class="success">	50-65 mL/100g/min @cite guyton2006medical	</span>|<span class="success">	60-98 mmHg	</span>|<span class="success">	72 @cite guyton2006medical	</span>|<span class="success">	[12.0, 20.0], [13.0, 19.0] @cite silverthorn2013human @cite mantoni2007reduced	</span>|
|	Brain Injury	|	Severity 0.75	|	920	|	1700	|<span class="success">	Increase @cite steiner2006monitoring	</span>|<span class="success">	Decrease @cite steiner2006monitoring	</span>|<span class="success">	Decrease @cite balestreri2006impact	</span>|<span class="success">	0-15% Decrease @cite bergeronSME	</span>|<span class="success">	0-20% Decrease @cite bergeronSME	</span>|
|	Brain Injury	|	Severity 0	|	1720	|	2000	|<span class="success">	7-15 mmHg @cite steiner2006monitoring	</span>|<span class="success">	50-65 mL/100g/min @cite guyton2006medical	</span>|<span class="success">	60-98 mmHg	</span>|<span class="success">	72 @cite guyton2006medical	</span>|<span class="success">	[12.0, 20.0], [13.0, 19.0] @cite silverthorn2013human @cite mantoni2007reduced	</span>|
|	Brain Injury	|	Severity 1	|	2020	|	3220	|<span class="success">	>25 mmHg @cite steiner2006monitoring	</span>|<span class="success">	<8 mL/100g/min @cite steiner2006monitoring	</span>|<span class="success">	Decrease @cite balestreri2006impact	</span>|<span class="warning">	Decrease @cite bergeronSME	</span>|<span class="warning">	Decrease @cite bergeronSME	</span>|

@anchor nervous-conclusions
Conclusions
-----------
The %Nervous System is currently in a preliminary state that contains only a baroreceptor feedback model, basic TBI, pupillary response, and a chemoreceptor model. The baroreceptor feedback is used to control rapid changes in arterial pressure by adjusting heart rate, heart elastance, and vascular resistance and compliance. The baroreceptor model has been validated by comparing the engine outputs to experimental data for hemorrhage for both first order and second order responses. The unstressed volume changes are not accounted for in the model and may improve the performance of the model.  The TBI model shows good agreement for the some basic TBI metrics for acute brain injuries. The introduction of an accurate ICP calculation based on cerebrospinal fluid properties will improve the model. Further discretization of the model will improve the localization of the TBI injury and effects. It is also important to include the damping of respiratory regulation as part of improving the TBI model. We also plan to improve the chemoreceptor model to include damping due to sedation and systemic vascular resistance and heart contractility changes. Pupillary response behaves as expected and arms the engine with yet another tool for matching output with clinical data. 

@anchor nervous-future
Future Work
===========
Coming Soon
-----------
- Chemoreceptor modification of heart contractility and systemic vascular resistance
- Local autoregulation
- Cerebrospinal fluid model
- Respiratory regulation damping with TBI and sedation

Recommended Improvements
------------------------
- Including unstressed volume in the tone model
- Patient variability incorporated into the baroreceptor reflex model constants
- Cerebrospinal fluid model to calculate intracranial pressure
- Baroreceptor changes during exercise and sleep
- Positional variability
- Higher-fidelity brain model with localized injuries
- Interventions to treat TBI like drainage of cerebrospinal fluid, hypothermic treatment, hyperventilation, and mannitol therapy
- Consciousness model, including reduced level of consciousness with carbon monoxide and other intoxications

Appendices
==========
Acronyms
--------
CBF - Cerebral Blood Flow

CNS - Central %Nervous System

CPP - Cerebral Perfusion Pressure

ICP - Intracranial Pressure

PERRLA - Pupils Equal, Round, Reactive to Light, and Accommodating

PNS - Peripheral %Nervous System

MAP	- Mean Arterial Pressure

mmHg - Millimeters mercury

Data Model Implementation
-------------------------
@ref NervousSystemTable "Nervous"
