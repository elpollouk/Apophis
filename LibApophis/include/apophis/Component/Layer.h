#pragma once

#include <memory>
#include <vector>
#include <assert.h>
#include "apophis/Component/Node.h"

namespace Apophis { namespace Component {

	class Layer
	{
	public:
		Layer(size_t numInputs, size_t numOutputs, const TransferFunction::ITransferFunction* transfer) :
			NumOutputs(numOutputs),
			NumInputs(numInputs)
		{
			for (auto i = 0u; i < numOutputs; i++)
				Nodes.emplace_back(std::make_unique<Node>(numInputs, transfer));

			Output.resize(numOutputs, 0.f);
		}

		ConstVectorRef Calculate(ConstVectorRef inputs)
		{
			for (auto i = 0u; i < NumOutputs; i++)
				Output[i] = Nodes[i]->Calculate(inputs);

			return Output;
		}

		const size_t NumInputs;
		const size_t NumOutputs;
		std::vector<std::unique_ptr<Node>> Nodes;
		Vector Output;
	};

}}