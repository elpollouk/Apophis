#pragma once

#include "apophis/apophistypes.h"

namespace Apophis { namespace Training { namespace Loss {

	real SquaredError(ConstVectorRef target, ConstVectorRef actual);

}}}