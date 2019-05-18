#pragma once

#include "apophis/apophis.h"

#define DEFAULT_TRANSFER public: static const ITransferFunction* Default();

namespace Apophis { namespace TransferFunction {

	class ITransferFunction
	{
	public:
		virtual ~ITransferFunction() {}

		virtual real operator()(real value) const = 0;
		virtual real Derivative(real value) const = 0;
	};

}}