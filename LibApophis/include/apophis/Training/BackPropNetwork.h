#pragma once

#include "apophis/Component/Network.h"
#include "apophis/Component/Layer.h"
#include "apophis/Component/Node.h"
#include "apophis/Training/Trainer.h"

namespace Apophis {	namespace Training {

	class BackPropNode : public Component::Node
	{
	public:
		BackPropNode(size_t numInputs);

		real BackPropError;
		Vector PreviousWeightChanges;
	};

	class BackPropLayer : public Component::Layer
	{
	public:
		BackPropLayer(size_t numInputs, size_t numOutputs, const TransferFunction::ITransferFunction& transfer, Component::Network& network);

		BackPropNode* GetBackPropNode(size_t index);
	};

	class BackPropNetwork : public Component::Network, public ITrainable
	{
	public:
		BackPropNetwork(Utils::IImportReader& reader, real learningRate, real momentum);
		BackPropNetwork(size_t inputSize, real learningRate, real momentum);

		virtual void Train(const Example& example) override;

	protected:
		virtual std::unique_ptr<Component::Layer> CreateLayer(size_t numNodes, const TransferFunction::ITransferFunction& transfer) override;
		virtual std::unique_ptr<Component::Node> CreateNode(size_t numInputs) override;

	private:
		BackPropLayer* GetBackPropLayer(size_t index);

		real m_LearningRate;
		real m_Momentum;
	};

}}