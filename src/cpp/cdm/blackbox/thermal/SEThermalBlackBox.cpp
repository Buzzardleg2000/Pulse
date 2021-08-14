/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDataModel.h"
#include "cdm/blackbox/thermal/SEThermalBlackBox.h"

SEThermalBlackBox::SEThermalBlackBox(const std::string& name, Logger* logger) : SEBlackBox<THERMAL_BLACK_BOX>(name,logger)
{

}

SEThermalBlackBox::~SEThermalBlackBox()
{
  Clear();
}
