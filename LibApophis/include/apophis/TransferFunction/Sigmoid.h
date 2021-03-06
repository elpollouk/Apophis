#pragma once

#include <math.h>
#include "ITransferFunction.h"

namespace Apophis {	namespace TransferFunction {

	class Sigmoid : public ITransferFunction
	{
		DEFAULT_TRANSFER

	public:
		static constexpr const char* Name = "sigmoid";

		Sigmoid() : ITransferFunction(Name) {}

		virtual real operator()(real value) const override
		{
			return 1.f / (1.f + expf(-value));
		}

		virtual real Derivative(real value) const override
		{
			value = operator()(value);
			return value * (1.f - value);
		}
	};

}}
