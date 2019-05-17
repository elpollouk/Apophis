#pragma once

#include "ITransferFunction.h"

namespace Apophis { namespace TransferFunction {

	template <class DType>
	class Relu : public ITransferFunction<DType>
	{
	public:
		Relu(DType leakGradient = {}) :
			m_LeakGradient(leakGradient)
		{

		}

		virtual DType Transfer(DType value)
		{
			return value > 0 ? value : m_LeakGradient * value;
		}

		virtual DType TransferDerivative(DType value)
		{
			return value > 0 ? 1 : m_LeakGradient;
		}

	private:
		DType m_LeakGradient;
	};

}}