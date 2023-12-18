/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/blackbox/electrical/SEElectricalBlackBox.h"

SEElectricalBlackBox::SEElectricalBlackBox(const std::string& name, Logger* logger) : SEBlackBox<ELECTRICAL_BLACK_BOX>(name,logger)
{

}
SEElectricalBlackBox::~SEElectricalBlackBox()
{
  Clear();
}

template class SEBlackBox<ELECTRICAL_BLACK_BOX>;
