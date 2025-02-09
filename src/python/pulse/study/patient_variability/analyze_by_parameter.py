# Distributed under the Apache License, Version 2.0.
# See accompanying NOTICE file for details.

from pulse.cdm.bind.Patient_pb2 import PatientData
from pulse.study.patient_variability.analysis_utils import PatientVariabilityResults, Conditional, Field, PropertyError

import plotly.graph_objects as go
import plotly.offline as pyo
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd


fields = [Field.Age_yr,
Field.Height_cm,
Field.BodyMassIndex,
Field.BodyFatFraction,
Field.HeartRateBaseline_bpm,
Field.MeanArterialPressureBaseline_mmHg,
Field.PulsePressureBaseline_mmHg,
Field.RespirationRateBaseline_bpm]

minAge_yr = 18
stdAge_yr = 44
maxAge_yr = 65
minMaleHeight_cm = 163
stdMaleHeight_cm = 180.34
maxMaleHeight_cm = 190
minFemaleHeight_cm = 151
stdFemaleHeight_cm = 162.56
maxFemaleHeight_cm = 175.5
minBMI = 16
stdMaleBMI = 23.71
stdFemaleBMI = 22.31
maxBMI = 29.9
minMaleBFF = 0.02
stdMaleBFF = 0.21
maxMaleBFF = 0.25
minFemaleBFF = 0.1
stdFemaleBFF = 0.28
maxFemaleBFF = 0.32
minHR_bpm = 60
stdHR_bpm = 72
maxHR_bpm = 100
minMAP_mmHg = 70
stdMAP_mmHg = 87
maxMAP_mmHg = 100
minPulsePressure_mmHg = 30
stdPulsePressure_mmHg = 40.5
maxPulsePressure_mmHg = 60
minRR_bpm = 8
stdRR_bpm = 12
maxRR_bpm = 20

mapAllowableErrorFraction = 0.1
stdMapMin_mmHg = stdMAP_mmHg - stdMAP_mmHg * mapAllowableErrorFraction
stdMapMax_mmHg = stdMAP_mmHg + stdMAP_mmHg * mapAllowableErrorFraction
pulsePressureAllowableErrorFraction = 0.1
stdPulsePressureMin_mmHg = stdPulsePressure_mmHg - stdPulsePressure_mmHg * pulsePressureAllowableErrorFraction
stdPulsePressureMax_mmHg = stdPulsePressure_mmHg + stdPulsePressure_mmHg * pulsePressureAllowableErrorFraction

standardValues = {
    (PatientData.eSex.Male, Field.Age_yr): stdAge_yr,
    (PatientData.eSex.Female, Field.Age_yr): stdAge_yr,
    (PatientData.eSex.Male, Field.Height_cm): stdMaleHeight_cm,
    (PatientData.eSex.Female, Field.Height_cm): stdFemaleHeight_cm,
    (PatientData.eSex.Male, Field.BodyMassIndex): stdMaleBMI,
    (PatientData.eSex.Female, Field.BodyMassIndex): stdFemaleBMI,
    (PatientData.eSex.Male, Field.BodyFatFraction): stdMaleBFF,
    (PatientData.eSex.Female, Field.BodyFatFraction): stdFemaleBFF,
    (PatientData.eSex.Male, Field.HeartRateBaseline_bpm): stdHR_bpm,
    (PatientData.eSex.Female, Field.HeartRateBaseline_bpm): stdHR_bpm,
    (PatientData.eSex.Male, Field.MeanArterialPressureBaseline_mmHg): stdMAP_mmHg,
    (PatientData.eSex.Female, Field.MeanArterialPressureBaseline_mmHg): stdMAP_mmHg,
    (PatientData.eSex.Male, Field.PulsePressureBaseline_mmHg): stdPulsePressure_mmHg,
    (PatientData.eSex.Female, Field.PulsePressureBaseline_mmHg): stdPulsePressure_mmHg,
    (PatientData.eSex.Male, Field.RespirationRateBaseline_bpm): stdRR_bpm,
    (PatientData.eSex.Female, Field.RespirationRateBaseline_bpm): stdRR_bpm
    }

lineSep = "-------------------------------------------------------------------"

class PatientVariabilityAnalysis(PatientVariabilityResults):
    systems = []
    passErrors = [10.0, 30.0]  # Errors are in %, not fraction!!!

    def __init__(self, dir: str):
        super().__init__(dir)

    def process(self):
        # Count everything
        no_errors = 0
        failed_setup = 0
        failed_stabilization = 0
        runtime_error = 0
        fatal_runtime_error = 0
        for state in self._results.PatientState:
            if state.Failure == 0:
                no_errors = no_errors + 1
            elif state.Failure == 1:
                failed_setup = failed_setup + 1
            elif state.Failure == 2:
                failed_stabilization = failed_stabilization + 1
            elif state.Failure == 3:
                runtime_error = runtime_error + 1
            elif state.Failure == 4:
                fatal_runtime_error = fatal_runtime_error + 1

        print("Total Runs " + str(len(self._results.PatientState)))
        print("Valid Runs " + str(no_errors))
        print("FailedSetup Runs " + str(failed_setup))
        print("FailedStabilization Runs " + str(failed_stabilization))
        print("RuntimeError Runs " + str(runtime_error))
        print("FatalRuntimeError Runs " + str(fatal_runtime_error))

        results = analysis.everythingQuery()
        numAllPatients = analysis.numPatients(results)
        print("All patients: " + str(numAllPatients))
        del results["ids"]
        analysis.systems = analysis.combineCategories(results)

        analysis.createRadarCharts()
        analysis.createBoxPlots()

    def createBoxPlots(self):
        print(" --------------------- Create box plots ---------------------")
        for passError in analysis.passErrors:
            # Standard male
            results = analysis.standardQuery(PatientData.eSex.Male)
            numStandardMalePatients = analysis.numPatients(results)
            print("Standard male patients: " + str(numStandardMalePatients))
            del results["ids"]
            standardMaleValues = analysis.calculateSystemPassRate(results, passError, analysis.systems)

            # Standard female
            results = analysis.standardQuery(PatientData.eSex.Female)
            numStandardFemalePatients = analysis.numPatients(results)
            print("Standard female patients: " + str(numStandardFemalePatients))
            del results["ids"]
            standardFemaleValues = analysis.calculateSystemPassRate(results, passError, analysis.systems)

            for idx, system in enumerate(analysis.systems):
                plotData = {}
                for field in fields:
                    print("Evaluating " + system + " system with " + str(field))
                    results = analysis.singleParameterQuery(PatientData.eSex.Male, field)
                    del results["ids"]
                    maleValues = analysis.calculateParameterPassRate(results, passError, system)
                    results = analysis.singleParameterQuery(PatientData.eSex.Female, field)
                    del results["ids"]
                    femaleValues = analysis.calculateParameterPassRate(results, passError, system)
                    # Clean up the category names for the plot
                    strField = str(field).replace("Field.", "")
                    strField = strField.split("_", 1)[0]
                    strField = strField.replace("BodyMassIndex", "BMI")
                    strField = strField.replace("BodyFatFraction", "FF")
                    strField = strField.replace("HeartRateBaseline", "HR")
                    strField = strField.replace("MeanArterialPressureBaseline", "MAP")
                    strField = strField.replace("PulsePressureBaseline", "PP")
                    strField = strField.replace("RespirationRateBaseline", "RR")
                    # Blood pressures are special
                    if strField != "MAP" and strField != "PP":
                        strField = strField + " (" + str(len(maleValues + femaleValues)) + ")"
                        plotData.update({strField: maleValues + femaleValues})
                        print("Patients evaluated: " + str(len(maleValues + femaleValues)))

                # Blood pressures are special
                results = analysis.bloodPressureQuery(PatientData.eSex.Male)
                del results["ids"]
                maleValues = analysis.calculateParameterPassRate(results, passError, system)
                results = analysis.bloodPressureQuery(PatientData.eSex.Female)
                del results["ids"]
                femaleValues = analysis.calculateParameterPassRate(results, passError, system)
                strField = "BP"
                strField = strField + " (" + str(len(maleValues + femaleValues)) + ")"
                plotData.update({strField: maleValues + femaleValues})
                print("Patients evaluated: " + str(len(maleValues + femaleValues)))

                # Add NaNs to make data size the same, so pandas doesn't complain
                dataLength = 0
                for key, value in plotData.items():
                    length = len(value)
                    if length > dataLength:
                        dataLength = length
                for key, value in plotData.items():
                    lengthDiff = dataLength - len(value)
                    if lengthDiff > 0:
                        value = np.append(value, np.repeat(np.nan, lengthDiff))
                        plotData.update({key: value})

                df = pd.DataFrame(plotData)
                df.boxplot(grid=True, rot=0)
                left, right = plt.xlim()
                plt.hlines(standardMaleValues[idx], xmin=left, xmax=right, color='blue', linestyles='--')
                plt.hlines(standardFemaleValues[idx], xmin=left, xmax=right, color='r', linestyles='--')
                plt.ylim([0.0, 1.01])
                plt.title(system + " Pass Rate Per Parameter within " + str(passError) + "%")
                plt.tight_layout()
                #plt.show()
                plt.savefig("test_results/patient_variability/Box_Plot_" + str(passError) + "%_" + system + ".png")
                plt.clf()

    def createRadarCharts(self):
        print(" --------------------- Create radar chart ---------------------")
        for passError in analysis.passErrors:
            # Standard male
            results = analysis.standardQuery(PatientData.eSex.Male)
            numStandardMalePatients = analysis.numPatients(results)
            print("Standard male patients: " + str(numStandardMalePatients))
            del results["ids"]
            radarStandardMaleValues = analysis.calculateSystemPassRate(results, passError, analysis.systems)

            #Standard female
            results = analysis.standardQuery(PatientData.eSex.Female)
            numStandardFemalePatients = analysis.numPatients(results)
            print("Standard female patients: " + str(numStandardFemalePatients))
            del results["ids"]
            radarStandardFemaleValues = analysis.calculateSystemPassRate(results, passError, analysis.systems)

            # Nonstandard male query
            results = analysis.nonStandardQuery(PatientData.eSex.Male)
            numNonstandardMalePatients = analysis.numPatients(results)
            print("Nonstandard male patients: " + str(numNonstandardMalePatients))
            del results["ids"]
            radarNonstandardMaleValues = analysis.calculateSystemPassRate(results, passError, analysis.systems)

            # Nonstandard female query
            results = analysis.nonStandardQuery(PatientData.eSex.Female)
            numNonstandardFemalePatients = analysis.numPatients(results)
            print("Nonstandard female patients: " + str(numNonstandardFemalePatients))
            del results["ids"]
            radarNonstandardFemaleValues = analysis.calculateSystemPassRate(results, passError, analysis.systems)

            # Generate radar chart
            radarCategories = [*analysis.systems, analysis.systems[0]]

            radarStandardMaleValues = [*radarStandardMaleValues, radarStandardMaleValues[0]]
            radarStandardFemaleValues = [*radarStandardFemaleValues, radarStandardFemaleValues[0]]
            radarNonstandardMaleValues = [*radarNonstandardMaleValues, radarNonstandardMaleValues[0]]
            radarNonstandardFemaleValues = [*radarNonstandardFemaleValues, radarNonstandardFemaleValues[0]]

            fig = go.Figure(
                data=[
                    go.Scatterpolar(r=radarStandardMaleValues, theta=radarCategories, name='Standard Male ('+str(numStandardMalePatients)+' total)'),
                    go.Scatterpolar(r=radarStandardFemaleValues, theta=radarCategories, name='Standard Female ('+str(numStandardFemalePatients)+' total)'),
                    go.Scatterpolar(r=radarNonstandardMaleValues, theta=radarCategories, name='Cumulative Nonstandard Male ('+str(numNonstandardMalePatients)+' total)'),
                    go.Scatterpolar(r=radarNonstandardFemaleValues, theta=radarCategories, name='Cumulative Nonstandard Female ('+str(numNonstandardFemalePatients)+' total)')
                ],
                layout=go.Layout(
                    title=go.layout.Title(text="Physiology System Pass Rate within " + str(passError) + "%"),
                    polar={'radialaxis': {'visible': True}},
                    showlegend=True
                )
            )
            pyo.offline.plot(fig, filename="test_results/patient_variability/RadarChart_" + str(passError) + "%_" + ".html")

    def numPatients(self, results):
        return len(results["ids"])

    def analyzePropertyError(self, property_error: PropertyError):
        # We can dynamically add members to our property_error object
        property_error.average_error = 0
        for error in property_error.errors:
            property_error.average_error = property_error.average_error + error
        property_error.average_error = property_error.average_error / len(property_error.errors)

    def standardQuery(self, sex:PatientData.eSex):
        #print("Standard " + PatientData.eSex(sex).name + " query")
        query = Conditional()
        query.sex(sex)
        for field in fields:
            query.addCondition(field, '==', standardValues[sex, field])
        results = self.conditionalFilter([query])
        return results

    def nonStandardQuery(self, sex:PatientData.eSex):
        #print("Nonstandard " + sex.name + " query")
        query = Conditional()
        query.sex(sex)
        queryOr = Conditional('OR')
        for field in fields:
            queryOr.addCondition(field, '!=', standardValues[sex, field])
        query.addConditional(queryOr)
        results = self.conditionalFilter([query])
        return results

    def everythingQuery(self):
        print("Everything query")
        query = Conditional()
        results = self.conditionalFilter([query])
        return results

    def singleParameterQuery(self, sex:PatientData.eSex, field:Field):
        #print(sex.name + " " + field.name + " query")
        query = Conditional()
        query.sex(sex)
        for currentField in fields:
            if field != currentField:
                query.addCondition(currentField, '==', standardValues[sex, currentField])
        results = self.conditionalFilter([query])
        return results

    def bloodPressureQuery(self, sex:PatientData.eSex):
        #print(sex.name + " " + field.name + " query")
        query = Conditional()
        query.sex(sex)
        for currentField in fields:
            if Field.MeanArterialPressureBaseline_mmHg != currentField and Field.PulsePressureBaseline_mmHg != currentField:
                query.addCondition(currentField, '==', standardValues[sex, currentField])
        results = self.conditionalFilter([query])
        return results

    def calculateSystemPassRate(self, results, passError, categories):
        passRates = [0] * len(categories)
        totalNumPass = [0] * len(categories)
        totalNumFail = [0] * len(categories)
        for system,properties in results.items():
            print("Examining system "+system)
            numPass = 0
            numFail = 0
            for property,property_error in properties.items():
                for error in property_error.errors:
                    if abs(error) > passError:
                        numFail = numFail + 1
                    else:
                        numPass = numPass + 1
            for idx, category in enumerate(categories):
                if category in system:
                    totalNumPass[idx] = totalNumPass[idx] + numPass
                    totalNumFail[idx] = totalNumFail[idx] + numFail

        for idx, category in enumerate(categories):
            passRate = 0
            numPass = totalNumPass[idx]
            numFail = totalNumFail[idx]
            if numPass + numFail != 0:
                passRate = numPass / (numPass + numFail)
            passRates[idx] = passRate
        return passRates

    def calculateParameterPassRate(self, results, passError, category):
        perPatientNumPass = []
        perPatientNumFail = []
        for system,properties in results.items():
            if not category in system:
                continue
            print("Examining system "+system)
            for property,property_error in properties.items():
                for idx, error in enumerate(property_error.errors):
                    numPass = 0
                    numFail = 0
                    if abs(error) > passError:
                        numFail = 1
                    else:
                        numPass = 1
                    if len(perPatientNumPass) < idx + 1:
                        perPatientNumPass.append(numPass)
                        perPatientNumFail.append(numFail)
                    else:
                        perPatientNumPass[idx] = perPatientNumPass[idx] + numPass
                        perPatientNumFail[idx] = perPatientNumFail[idx] + numFail
        perPatientPassRates = []
        for idx, patient in enumerate(perPatientNumPass):
            passRate = perPatientNumPass[idx] / (perPatientNumPass[idx] + perPatientNumFail[idx])
            perPatientPassRates.append(passRate)
        return perPatientPassRates

    def combineCategories(self, results):
        startCategories = []
        endCategories = []
        for system, properties in results.items():
            startCategories.append(system)
        for startCategory in startCategories:
            newCategory = True
            for idx, endCategory in enumerate(endCategories):
                if startCategory in endCategory:
                    # Replace endCategory with startCategory
                    endCategories[idx] = startCategory
                    newCategory = False
                if endCategory in startCategory:
                    # Don't do anything
                    newCategory = False
                    break
            if newCategory:
                # New, so add
                endCategories.append(startCategory)
        finalCategories = []
        for endCategory in endCategories:
            duplicate = False
            for finalCategory in finalCategories:
                if endCategory == finalCategory:
                    duplicate = True
                    break
            if not duplicate:
                finalCategories.append(endCategory)

        return finalCategories


if __name__ == '__main__':
    analysis = PatientVariabilityAnalysis("./test_results/patient_variability/validation/full/")
    analysis.process()
