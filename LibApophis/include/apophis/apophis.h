#pragma once
#include <vector>

namespace Apophis {
	typedef float real;
	typedef std::vector<real> Vector;
	typedef Vector& VectorRef;
	typedef const Vector& ConstVectorRef;


	void Seed(unsigned int seed);
	real RandomReal();
	real RandomWeight();
}
