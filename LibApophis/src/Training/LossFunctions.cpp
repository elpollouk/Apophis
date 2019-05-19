#include "stdafx.h"
#include "apophis/Training/LossFunctions.h"

using namespace Apophis;

real Apophis::Training::Loss::SquaredError(ConstVectorRef target, ConstVectorRef actual)
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
