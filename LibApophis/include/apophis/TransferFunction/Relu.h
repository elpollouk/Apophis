#pragma once

#include "ITransferFunction.h"

namespace Apophis { namespace TransferFunction {

	class Relu : public ITransferFunction
	{
	public:
		Relu(real leakGradient = 0.01) :
			m_LeakGradient(leakGradient)
		{

		}

		virtual real operator()(real value) const
		{
			return value > 0.f ? value : m_LeakGradient * value;
		}

		virtual real Derivative(real value) const
		{
			return value > 0.f ? 1.f : m_LeakGradient;
		}

	private:
		real m_LeakGradient;
	};

}}