#pragma once

#include "apophis/TransferFunction/ITransferFunction.h"

namespace Apophis { namespace Component {

	class Node
	{
	public:
		Node(size_t numInputs, const TransferFunction::ITransferFunction& transfer);

		size_t GetNumInputs() const { return m_NumInputs; }
		const TransferFunction::ITransferFunction& GetTransferFunction() const { return m_Transfer; }

		real Calculate(ConstVectorRef input);

		real Activation;
		real BackPropError;
		Vector Weights;
		Vector PreviousWeightChanges;

	protected:
		size_t m_NumInputs;
		const TransferFunction::ITransferFunction& m_Transfer;
	};

}}