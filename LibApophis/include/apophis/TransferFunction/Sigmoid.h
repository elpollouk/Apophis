#pragma once

#include <math.h>
#include "ITransferFunction.h"

namespace Apophis {	namespace TransferFunction {

	class Sigmoid : public ITransferFunction
	{
		DEFAULT_TRANSFER

	public:
		virtual real operator()(real value) const
		{
			return 1.f / (1.f + expf(-value));
		}

		virtual real Derivative(real value) const
		{
			value = operator()(value);
			return value * (1.f - value);
		}
	};

}}
