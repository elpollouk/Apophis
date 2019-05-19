#include "stdafx.h"
#include "apophis/Component/BackPropNetwork.h"

using namespace Apophis;
using namespace Apophis::Component;
//using namespace Apophis::Training;

static void CalculateOutputBackPropError(ConstVectorRef targets, Layer* layer)
{
	assert(targets.size() == layer->GetOutput().size());

	for (auto i = 0u; i < layer->GetNumOutputs(); i++)
	{
		auto& node = layer->Nodes[i];
		node->BackPropError = (targets[i] - layer->GetOutput(i)) * node->GetTransferFunction().Derivative(node->Activation);
	}
}

static void CalculateHiddenBackPropError(Layer* targetLayer, const Layer* forwardLayer)
{
	for (auto i = 0u; i < targetLayer->GetNumOutputs(); i++)
	{
		auto& targetNode = targetLayer->Nodes[i];
		targetNode->BackPropError = 0.f;
		for (auto j = 0u; j < forwardLayer->GetNumOutputs(); j++)
			targetNode->BackPropError += forwardLayer->Nodes[j]->BackPropError * forwardLayer->Nodes[j]->Weights[i];

		targetNode->BackPropError *= targetNode->GetTransferFunction().Derivative(targetLayer->Nodes[i]->Activation);
	}
}

static void UpdateWeights(Layer* targetLayer, ConstVectorRef priorLayerOutput, real learningRate, real momentum)
{
	for (auto i = 0u; i < targetLayer->GetNumOutputs(); i++)
	{
		auto learningDelta = learningRate * targetLayer->Nodes[i]->BackPropError;
		for (auto j = 0u; j < targetLayer->GetNumInputs(); j++)
		{
			auto weightChange = learningDelta * priorLayerOutput[j];
			weightChange += (momentum * targetLayer->Nodes[i]->PreviousWeightChanges[j]);
			targetLayer->Nodes[i]->Weights[j] += weightChange;
			targetLayer->Nodes[i]->PreviousWeightChanges[j] = weightChange;
		}

		// Update bias
		auto weightChange = learningDelta;
		weightChange += (momentum * targetLayer->Nodes[i]->PreviousWeightChanges[targetLayer->GetNumInputs()]);
		targetLayer->Nodes[i]->Weights[targetLayer->GetNumInputs()] += weightChange;
		targetLayer->Nodes[i]->PreviousWeightChanges[targetLayer->GetNumInputs()] = weightChange;
	}
}


BackPropNetwork::BackPropNetwork(int inputSize, real learningRate, real momentum) :
	ITrainableNetwork(inputSize),
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
	auto pForwardLayer = m_Layers[i--].get();
	CalculateOutputBackPropError(target, pForwardLayer);
	do
	{
		auto pTargetLayer = m_Layers[i].get();
		CalculateHiddenBackPropError(pTargetLayer, pForwardLayer);
		pForwardLayer = pTargetLayer;
	} while (i--);

	auto pInput = &input;
	for (auto& layer : m_Layers)
	{
		UpdateWeights(layer.get(), *pInput, m_LearningRate, m_Momentum);
		pInput = &layer->GetOutput();
	}
}
