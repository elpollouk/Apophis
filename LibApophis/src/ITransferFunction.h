#pragma once

namespace Apophis {
	template <class DType>
	class ITransferFunction
	{
	public:
		virtual ~ITransferFunction() {}

		virtual DType Transfer(DType value) = 0;
		virtual DType TransferDerivative(DType value) = 0;

		DType operator()(DType value) { return Transfer(value); }
	};
}