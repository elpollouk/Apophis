#pragma once

#include "apophis/Component/Network.h"
#include "apophis/ExampleSet.h"

namespace Apophis {	namespace Component {

	class BackPropNetwork : public ITrainableNetwork
	{
	public:
		BackPropNetwork(int inputSize, real learningRate, real momentum) :
			ITrainableNetwork(inputSize),
			m_LearningRate(learningRate),
			m_Momentum(momentum)
		{

		}

		virtual void Train(ConstVectorRef input, ConstVectorRef target) override
		{
			assert(input.size() == GetInputSize());
			assert(target.size() == m_Layers.back()->NumOutputs);

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
				ApplyDeltas(layer.get(), *pInput, m_LearningRate, m_Momentum);
				pInput = &layer->Output;
			}
		}

	private:
		static void CalculateOutputBackPropError(ConstVectorRef targets, Layer* layer)
		{
			assert(targets.size() == layer->Output.size());

			for (auto i = 0u; i < layer->NumOutputs; i++)
			{
				auto& node = layer->Nodes[i];
				node->BackPropError = (targets[i] - layer->Output[i]) * node->GetTransferFunction().Derivative(node->Activation);
			}
		}

		static void CalculateHiddenBackPropError(Layer* targetLayer, const Layer* forwardLayer)
		{
			for (auto i = 0u; i < targetLayer->NumOutputs; i++)
			{
				auto& targetNode = targetLayer->Nodes[i];
				targetNode->BackPropError = 0.f;
				for (auto j = 0u; j < forwardLayer->NumOutputs; j++)
					targetNode->BackPropError += forwardLayer->Nodes[j]->BackPropError * forwardLayer->Nodes[j]->Weights[i];

				targetNode->BackPropError *= targetNode->GetTransferFunction().Derivative(targetLayer->Nodes[i]->Activation);
			}
		}

		static void ApplyDeltas(Layer* targetLayer, ConstVectorRef priorLayerOutput, real learningRate, real momentum)
		{
			for (auto i = 0u; i < targetLayer->NumOutputs; i++)
			{
				auto learningDelta = learningRate * targetLayer->Nodes[i]->BackPropError;
				for (auto j = 0u; j < targetLayer->NumInputs; j++)
				{
					auto weightChange = learningDelta * priorLayerOutput[j];
					weightChange += (momentum * targetLayer->Nodes[i]->PreviousWeightChanges[j]);
					targetLayer->Nodes[i]->Weights[j] += weightChange;
					targetLayer->Nodes[i]->PreviousWeightChanges[j] = weightChange;
				}

				// Update bias
				auto weightChange = learningDelta;
				weightChange += (momentum * targetLayer->Nodes[i]->PreviousWeightChanges[targetLayer->NumInputs]);
				targetLayer->Nodes[i]->Weights[targetLayer->NumInputs] += weightChange;
				targetLayer->Nodes[i]->PreviousWeightChanges[targetLayer->NumInputs] = weightChange;
			}
		}

		real m_LearningRate;
		real m_Momentum;
	};

}}