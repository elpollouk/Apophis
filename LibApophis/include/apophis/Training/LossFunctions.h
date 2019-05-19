#pragma once

#include "apophis/apophis.h"

namespace Apophis { namespace Training { namespace Loss {

	typedef real(*PFNLoss)(ConstVectorRef target, ConstVectorRef actual);

	real SquaredError(ConstVectorRef target, ConstVectorRef actual);

}}}