#pragma once

#include <memory>
#include "apophis/TransferFunction/Relu.h"
#include "apophis/TransferFunction/Sigmoid.h"
#include "Node.h"

namespace Apophis {
	template<class NodeType, class... Types>
	std::unique_ptr<NodeType> CreateNode(size_t numInputs, Types&&... args)
	{
		return std::make_unique<NodeType>(numInputs, std::forward<Types>(args)...);
	}

	typedef Component::Node<real, TransferFunction::Relu<real>> ReluNode;
	template<>
	std::unique_ptr<ReluNode> CreateNode<ReluNode>(size_t numInputs)
	{
		return std::make_unique<ReluNode>(numInputs, ::Apophis::TransferFunction::Relu<real>(0.01f));
	}

	typedef Component::Node<real, TransferFunction::Sigmoid<real>> SigmoidNode;
	template<>
	std::unique_ptr<SigmoidNode> CreateNode<SigmoidNode>(size_t numInputs)
	{
		return std::make_unique<SigmoidNode>(numInputs, ::Apophis::TransferFunction::Sigmoid<real>());
	}
}