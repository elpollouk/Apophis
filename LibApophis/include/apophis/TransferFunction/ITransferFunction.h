#pragma once

#include "apophis/apophis.h"

namespace Apophis { namespace TransferFunction {

	class ITransferFunction
	{
	public:
		virtual ~ITransferFunction() {}

		virtual real operator()(real value) const = 0;
		virtual real Derivative(real value) const = 0;
	};

}}