#pragma once

#include "apophis/apophistypes.h"
#include "apophis/Component/Node.h"

namespace Apophis { namespace Component {

	class Network;

	class Layer
	{
	public:
		Layer(size_t numInputs, size_t numOutputs, const TransferFunction::ITransferFunction& transfer, Network& network);
		virtual ~Layer() {}

		size_t GetNumInputs() const { return m_NumInputs; }
		size_t GetNumOutputs() const { return m_NumOutputs; }
		ConstVectorRef GetOutput() const { return m_Output; }
		real GetOutput(size_t index) const { return m_Output[index]; }

		ConstVectorRef Calculate(ConstVectorRef inputs);

		std::vector<std::unique_ptr<Node>> Nodes;

	private:
		size_t m_NumInputs;
		size_t m_NumOutputs;
		Vector m_Output;
	};

}}