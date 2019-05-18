#pragma once

#include "apophis/Component/Layer.h"

namespace Apophis { namespace Component {

	class Network
	{
	public:
		Network(size_t inputSize) :
			InputSize(inputSize),
			OutputSize(inputSize)
		{

		}

		template<class TransferFunction>
		void AddLayer(int numNodes)
		{
			AddLayer(numNodes, TransferFunction::Default());
		}

		void AddLayer(int numNodes, const TransferFunction::ITransferFunction* transfer)
		{
			m_Layers.emplace_back(std::make_unique<Layer>(OutputSize, numNodes, transfer));
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
		std::vector<std::unique_ptr<Layer>> m_Layers;
	};

	class ITrainableNetwork : public Network
	{
	public:
		ITrainableNetwork(size_t inputSize) : Network(inputSize) {}

		virtual void Train(ConstVectorRef input, ConstVectorRef target) = 0;
	};
}}