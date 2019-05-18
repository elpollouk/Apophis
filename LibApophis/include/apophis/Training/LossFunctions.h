#pragma once

#include <assert.h>
#include "apophis/apophis.h"

namespace Apophis { namespace Training { namespace Loss {

	typedef real(*PFNLoss)(ConstVectorRef target, ConstVectorRef actual);

	static real Error(ConstVectorRef target, ConstVectorRef actual)
	{
		assert(target.size() == actual.size());

		auto error = 0.f;

		for (auto i = 0u; i < target.size(); i++)
		{
			auto v = target[i] - actual[i];
			v *= v;
			error += v;
		}
		error *= 0.5;

		return error;
	}

}}}