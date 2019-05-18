#pragma once

#include "apophis/Component/Layer.h"

namespace Apophis { namespace Component {

	class Network
	{
	friend class Layer;
	public:
		Network(size_t inputSize) :
			InputSize(inputSize),
			OutputSize(inputSize)
		{

		}

		template<class TransferFunction>
		void AddLayer(size_t numNodes)
		{
			AddLayer(numNodes, TransferFunction::Default());
		}

		void AddLayer(size_t numNodes, const TransferFunction::ITransferFunction* transfer)
		{
			m_Layers.emplace_back(CreateLayer(numNodes, transfer));
			OutputSize = numNodes;
		}

		ConstVectorRef Calculate(ConstVectorRef input)
		{
			assert(input.size() == InputSize);

			auto pInput = &input;
			for (auto& layer : m_Layers)
				pInput = &layer->Calculate(*pInput);

			return *pInput;
		}

		const size_t InputSize;
		size_t OutputSize;

	protected:
		virtual std::unique_ptr<Layer> CreateLayer(size_t numNodes, const TransferFunction::ITransferFunction* transfer)
		{
			return std::make_unique<Layer>(OutputSize, numNodes, transfer, this);
		}

		virtual std::unique_ptr<Node> CreateNode(size_t numInputs, const TransferFunction::ITransferFunction* transfer)
		{
			return std::make_unique<Node>(numInputs, transfer);
		}

		std::vector<std::unique_ptr<Layer>> m_Layers;
	};

	class ITrainableNetwork : public Network
	{
	public:
		ITrainableNetwork(size_t inputSize) : Network(inputSize) {}

		virtual void Train(ConstVectorRef input, ConstVectorRef target) = 0;
	};
}}