#pragma once

#include "apophis/apophistypes.h"

namespace Apophis { namespace Component {

	class Node
	{
	public:
		Node(size_t numInputs, const TransferFunction::ITransferFunction& transfer);
		virtual ~Node() {}

		size_t GetNumInputs() const { return m_NumInputs; }
		const TransferFunction::ITransferFunction& GetTransferFunction() const { return m_Transfer; }

		real Calculate(ConstVectorRef input);

		real Activation;
		Vector Weights;

		virtual void Export(Utils::ExportTarget& outputObject) const;

	protected:
		size_t m_NumInputs;
		const TransferFunction::ITransferFunction& m_Transfer;
	};

}}