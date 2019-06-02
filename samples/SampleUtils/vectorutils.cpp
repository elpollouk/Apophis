#include "stdafx.h"
#include "vectorutils.h"

namespace SampleUtils { namespace Vector {

Apophis::Vector OneHot(size_t size, unsigned int value)
{
	auto v = Apophis::Vector(size);
	for (auto i = 0u; i < size; i++)
		v[i] = i == value ? 1.f : 0.f;
	return v;
}

}}