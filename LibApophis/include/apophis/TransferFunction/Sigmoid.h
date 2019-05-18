#pragma once

#include <math.h>

namespace Apophis {
	namespace TransferFunction {

		template <class DType>
		class Sigmoid
		{
		public:
			DType operator()(DType value) const
			{
				return 1 / (1 + expf(-value));
			}

			DType Derivative(DType value) const
			{
				value = operator()(value);
				return value * (1 - value);
			}
		};

		template<>
		class Sigmoid<double>
		{
		public:
			double operator()(double value) const
			{
				return 1 / (1 + exp(-value));
			}

			double Derivative(double value) const
			{
				value = operator()(value);
				return value * (1 - value);
			}
		};
	}
}
