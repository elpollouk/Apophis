#pragma once

#include "ITransferFunction.h"

namespace Apophis { namespace TransferFunction {

	class Relu : public ITransferFunction
	{
		DEFAULT_TRANSFER

	public:
		static constexpr const char* Name = "relu";

		Relu(real leakGradient = 0.01) :
			ITransferFunction(Name),
			m_LeakGradient(leakGradient)
		{

		}

		virtual real operator()(real value) const override
		{
			return value > 0.f ? value : m_LeakGradient * value;
		}

		virtual real Derivative(real value) const override
		{
			return value > 0.f ? 1.f : m_LeakGradient;
		}

	private:
		real m_LeakGradient;
	};

}}