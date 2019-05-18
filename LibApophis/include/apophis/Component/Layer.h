#pragma once

#include <memory>
#include <vector>
#include <assert.h>
#include "apophis/Component/Node.h"

namespace Apophis { namespace Component {

	class Layer
	{
	public:
		Layer(int size, int numInputs, const TransferFunction::ITransferFunction* transfer) :
			Size(size),
			NumInputs(numInputs)
		{
			for (auto i = 0; i < size; i++)
				Nodes.emplace_back(std::make_unique<Node>(numInputs, transfer));

			Output.resize(size, 0.f);
		}

		ConstVectorRef Calculate(ConstVectorRef inputs)
		{
			for (auto i = 0; i < Size; i++)
				Output[i] = Nodes[i]->Calculate(inputs);

			return Output;
		}

		const int Size;
		const int NumInputs;
		std::vector<std::unique_ptr<Node>> Nodes;
		Vector Output;
	};

}}