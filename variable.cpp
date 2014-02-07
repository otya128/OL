#include "stdafx.h"
#include "variable.h"
#include "Object.h"
namespace lang{
variable::variable(variable* s)
{
    this->parentVariable = s;
}


variable::variable(void)
{
}
variable::~variable(void)
{
}
}
