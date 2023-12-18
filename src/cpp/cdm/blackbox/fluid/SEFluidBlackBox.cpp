/* Distributed under the Apache License, Version 2.0.
   See accompanying NOTICE file for details.*/

#include "cdm/CommonDefs.h"
#include "cdm/blackbox/fluid/SEFluidBlackBox.h"

SEFluidBlackBox::SEFluidBlackBox(const std::string& name, Logger* logger) : SEBlackBox<FLUID_BLACK_BOX>(name,logger)
{

}

SEFluidBlackBox::~SEFluidBlackBox()
{
  Clear();
}

template class SEBlackBox<FLUID_BLACK_BOX>;
