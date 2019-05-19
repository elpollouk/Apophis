#include "stdafx.h"
#include "apophis/TransferFunction/Relu.h"
#include "apophis/TransferFunction/Sigmoid.h"

using namespace Apophis::TransferFunction;

#define IMPLEMENT_DEFAULT(function) static function s_Default ## function; \
const ITransferFunction& function::Default() { return s_Default ## function; }

IMPLEMENT_DEFAULT(Relu)
IMPLEMENT_DEFAULT(Sigmoid)
