#pragma once

#include "apophis/Component/Network.h"
#include "apophis/Component/Node.h"

namespace Apophis {	namespace Training {

	class BackPropNode : public Component::Node
	{
	public:
		BackPropNode(size_t numInputs, const TransferFunction::ITransferFunction& transfer);

		real BackPropError;
		Vector PreviousWeightChanges;
	};

	class BackPropLayer : public Component::Layer
	{
	public:
		BackPropLayer(size_t numInputs, size_t numOutputs, const TransferFunction::ITransferFunction& transfer, Component::Network& network);

		BackPropNode* GetBackPropNode(size_t index);
	};

	class BackPropNetwork : public Component::ITrainableNetwork
	{
	public:
		BackPropNetwork(size_t inputSize, real learningRate, real momentum);

		virtual void Train(ConstVectorRef input, ConstVectorRef target) override;

	protected:
		virtual std::unique_ptr<Component::Layer> CreateLayer(size_t numNodes, const TransferFunction::ITransferFunction& transfer) override;
		virtual std::unique_ptr<Component::Node> CreateNode(size_t numInputs, const TransferFunction::ITransferFunction& transfer) override;

	private:
		BackPropLayer* GetBackPropLayer(size_t index);

		real m_LearningRate;
		real m_Momentum;
	};

}}