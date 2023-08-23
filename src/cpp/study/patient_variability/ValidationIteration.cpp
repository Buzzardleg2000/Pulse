/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "ValidationIteration.h"

#include "cdm/CommonDefs.h"
#include "cdm/engine/SEAdvanceTime.h"
#include "cdm/engine/SESerializeState.h"
#include "cdm/patient/actions/SEAirwayObstruction.h"
#include "cdm/patient/actions/SEChestOcclusiveDressing.h"
#include "cdm/patient/actions/SEHemorrhage.h"
#include "cdm/patient/actions/SENeedleDecompression.h"
#include "cdm/patient/actions/SESubstanceCompoundInfusion.h"
#include "cdm/patient/actions/SETensionPneumothorax.h"
#include "cdm/properties/SEScalar0To1.h"
#include "cdm/properties/SEScalarTime.h"
#include "cdm/utils/GeneralMath.h"

namespace pulse::study::patient_variability
{
  ValidationIteration::ValidationIteration(Logger& logger) : ScenarioIteration(logger)
  {

  }
  ValidationIteration::~ValidationIteration()
  {

  }

  void ValidationIteration::GenerateSlicedActionSets(std::pair<std::string, std::string> patientFolderAndStateFilename, const std::string destDir)
  {
    GenerateScenarios(patientFolderAndStateFilename, destDir);
  }

  void ValidationIteration::GenerateCombinationActionSets(std::pair<std::string, std::string> patientFolderAndStateFilename, const std::string destDir)
  {
    GenerateScenarios(patientFolderAndStateFilename, destDir);
  }

  void ValidationIteration::GenerateScenarios(std::pair<std::string, std::string> patientFolderAndStateFilename, const std::string destDir)
  {
    
  }
}
