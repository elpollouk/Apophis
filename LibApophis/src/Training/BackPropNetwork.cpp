#include "stdafx.h"
#include "apophis/Training/BackPropNetwork.h"
#include "apophis/TransferFunction/ITransferFunction.h"

using namespace Apophis;
using namespace Apophis::Component;
using namespace Apophis::Training;

static void CalculateOutputBackPropError(ConstVectorRef targets, BackPropLayer* layer)
{
	assert(targets.size() == layer->GetOutput().size());

	for (auto i = 0u; i < layer->GetNumOutputs(); i++)
	{
		auto node = layer->GetBackPropNode(i);
		node->BackPropError = (targets[i] - layer->GetOutput(i)) * node->GetTransferFunction().Derivative(node->Activation);
	}
}

static void CalculateHiddenBackPropError(BackPropLayer* targetLayer, BackPropLayer* forwardLayer)
{
	assert(targetLayer->GetNumOutputs() == forwardLayer->GetNumInputs());

	for (auto i = 0u; i < targetLayer->GetNumOutputs(); i++)
	{
		auto targetNode = targetLayer->GetBackPropNode(i);
		targetNode->BackPropError = 0.f;
		for (auto j = 0u; j < forwardLayer->GetNumOutputs(); j++)
			targetNode->BackPropError += forwardLayer->GetBackPropNode(j)->BackPropError * forwardLayer->Nodes[j]->Weights[i];

		targetNode->BackPropError *= targetNode->GetTransferFunction().Derivative(targetLayer->Nodes[i]->Activation);
	}
}

static void UpdateWeights(BackPropLayer* targetLayer, ConstVectorRef priorLayerOutput, real learningRate, real momentum)
{
	for (auto i = 0u; i < targetLayer->GetNumOutputs(); i++)
	{
		auto node = targetLayer->GetBackPropNode(i);
		auto learningDelta = learningRate * node->BackPropError;
		for (auto j = 0u; j < targetLayer->GetNumInputs(); j++)
		{
			auto weightChange = learningDelta * priorLayerOutput[j];
			weightChange += (momentum * node->PreviousWeightChanges[j]);
			node->Weights[j] += weightChange;
			node->PreviousWeightChanges[j] = weightChange;
		}

		// Update bias
		auto weightChange = learningDelta;
		weightChange += (momentum * node->PreviousWeightChanges[targetLayer->GetNumInputs()]);
		node->Weights[targetLayer->GetNumInputs()] += weightChange;
		node->PreviousWeightChanges[targetLayer->GetNumInputs()] = weightChange;
	}
}


BackPropNetwork::BackPropNetwork(size_t inputSize, real learningRate, real momentum) :
	Network(inputSize),
	m_LearningRate(learningRate),
	m_Momentum(momentum)
{

}

void BackPropNetwork::Train(ConstVectorRef input, ConstVectorRef target)
{
	assert(input.size() == GetInputSize());
	assert(target.size() == m_Layers.back()->GetNumOutputs());

	auto actual = Calculate(input);

	auto i = m_Layers.size() - 1;
	auto pForwardLayer = GetBackPropLayer(i--);
	CalculateOutputBackPropError(target, pForwardLayer);
	do
	{
		auto pTargetLayer = GetBackPropLayer(i);
		CalculateHiddenBackPropError(pTargetLayer, pForwardLayer);
		pForwardLayer = pTargetLayer;
	} while (i--);

	auto pInput = &input;
	for (auto i = 0u; i < m_Layers.size(); i++)
	{
		auto pLayer = GetBackPropLayer(i);
		UpdateWeights(pLayer, *pInput, m_LearningRate, m_Momentum);
		pInput = &pLayer->GetOutput();
	}
}

BackPropLayer* BackPropNetwork::GetBackPropLayer(size_t index)
{
	auto pLayer = m_Layers[index].get();
	assert(dynamic_cast<BackPropLayer*>(pLayer) != nullptr);
	return static_cast<BackPropLayer*>(pLayer);
}

std::unique_ptr<Layer> BackPropNetwork::CreateLayer(size_t numNodes, const TransferFunction::ITransferFunction& transfer)
{
	return std::make_unique<BackPropLayer>(GetOutputSize(), numNodes, transfer, *this);
}

std::unique_ptr<Node> BackPropNetwork::CreateNode(size_t numInputs, const TransferFunction::ITransferFunction& transfer)
{
	return std::make_unique<BackPropNode>(numInputs, transfer);
}

BackPropNode::BackPropNode(size_t numInputs, const TransferFunction::ITransferFunction& transfer) :
	Component::Node(numInputs, transfer),
	BackPropError(0)
{
	PreviousWeightChanges.resize(Weights.size(), 0.f);
}

BackPropLayer::BackPropLayer(size_t numInputs, size_t numOutputs, const TransferFunction::ITransferFunction& transfer, Network& network) :
	Component::Layer(numInputs, numOutputs, transfer, network)
{

}

BackPropNode* BackPropLayer::GetBackPropNode(size_t index)
{
	auto pNode = Nodes[index].get();
	assert(dynamic_cast<BackPropNode*>(pNode) != nullptr);
	return static_cast<BackPropNode*>(pNode);
}
