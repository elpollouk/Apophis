#pragma once

#include <memory>
#include <vector>
#include <assert.h>
#include "apophis/Component/Node.h"

namespace Apophis { namespace Component {

	class Network;

	class Layer
	{
	public:
		Layer(size_t numInputs, size_t numOutputs, const TransferFunction::ITransferFunction* transfer, Network* network);

		ConstVectorRef Calculate(ConstVectorRef inputs);

		const size_t NumInputs;
		const size_t NumOutputs;
		std::vector<std::unique_ptr<Node>> Nodes;
		Vector Output;

	private:
		Network* m_pNetwork;
	};

}}