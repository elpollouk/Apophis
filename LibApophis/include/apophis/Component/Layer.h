#pragma once

#include <memory>
#include <vector>
#include <assert.h>
#include "apophis/Component/Factory.h"

namespace Apophis { namespace Component {

	template<class NodeType>
	class Layer
	{
	public:
		Layer(int size, int numInputs) :
			Size(size),
			NumInputs(numInputs)
		{
			for (auto i = 0; i < size; i++)
				Nodes.emplace_back(CreateNode<NodeType>(numInputs));

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
		std::vector<std::unique_ptr<NodeType>> Nodes;
		Vector Output;
	};

}}