#include "stdafx.h"
#include "apophis/apophis.h"

static std::default_random_engine s_generator;
static std::uniform_real_distribution<Apophis::real> s_zero_to_one(0.f, 1.f);

namespace Apophis {

	void Seed(unsigned int seed)
	{
		s_generator.seed(seed);
	}
	
	std::default_random_engine& GetRandomGenerator()
	{
		return s_generator;
	}

	real RandomReal()
	{
		return s_zero_to_one(s_generator);
	}

}