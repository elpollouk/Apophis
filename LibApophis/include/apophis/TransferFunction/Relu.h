#pragma once

namespace Apophis { namespace TransferFunction {

	template <class DType>
	class Relu
	{
	public:
		Relu(DType leakGradient = {}) :
			m_LeakGradient(leakGradient)
		{

		}

		DType operator()(DType value) const
		{
			return value > 0 ? value : m_LeakGradient * value;
		}

		DType Derivative(DType value) const
		{
			return value > 0 ? 1 : m_LeakGradient;
		}

	private:
		DType m_LeakGradient;
	};

}}