#include "pch.h"
#include "apophis/apophis.h"
#include "apophis/ExampleSet.h"
#include "apophis/TransferFunction/Relu.h"
#include "apophis/TransferFunction/Sigmoid.h"
#include "apophis/Component/Node.h"
#include "apophis/Component/Layer.h"
#include "Network.h"

using namespace Apophis;
using namespace Apophis::Component;
using namespace Apophis::TransferFunction;

namespace Network
{

real Error(ConstVectorRef target, ConstVectorRef actual)
{
	assert(target.size() == actual.size());

	auto error = 0.f;

	for (auto i = 0u; i < target.size(); i++)
	{
		auto v = target[i] - actual[i];
		v *= v;
		error += 0.5f * v;
	}

	return error;
}

void CalculateOutputBackPropError(ConstVectorRef targets, Layer* layer)
{
	assert(targets.size() == layer->Output.size());

	for (auto i = 0; i < layer->Size; i++)
	{
		auto node = layer->Nodes[i].get();
		node->BackPropError = (targets[i] - layer->Output[i]) * node->Transfer->Derivative(node->Activation);
	}
}

void CalculateHiddenBackPropError(Layer* targetLayer, Layer* forwardLayer)
{
	for (auto i = 0; i < targetLayer->Size; i++)
	{
		auto targetNode = targetLayer->Nodes[i].get();
		targetNode->BackPropError = 0.f;
		for (auto k = 0; k < forwardLayer->Size; k++)
		{
			targetNode->BackPropError += forwardLayer->Nodes[k]->BackPropError * forwardLayer->Nodes[k]->Weights[i];
		}
		targetNode->BackPropError *= targetNode->Transfer->Derivative(targetLayer->Nodes[i]->Activation);
	}
}

void ApplyDeltas(Layer* targetLayer, ConstVectorRef priorLayerOutput, real learningRate, real momentum)
{
	for (auto i = 0; i < targetLayer->Size; i++)
	{
		auto learningDelta = learningRate * targetLayer->Nodes[i]->BackPropError;
		for (auto j = 0; j < targetLayer->NumInputs; j++)
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

class Network
{
public:
	Network(int inputSize) :
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
		m_Layers.emplace_back(std::make_unique<Layer>(numNodes, OutputSize, transfer));
		OutputSize = numNodes;
	}

	ConstVectorRef Calculate(ConstVectorRef input)
	{
		assert(input.size() == InputSize);

		auto pInput = &input;
		for (auto& layer : m_Layers)
			pInput = &layer->Calculate(*pInput);

		assert((*pInput)[0] != -NAN);
		return *pInput;
	}

	void Train(ConstVectorRef input, ConstVectorRef target, real learningRate, real momentum)
	{
		assert(input.size() == InputSize);
		assert(target.size() == m_Layers.back()->Size);

		auto actual = Calculate(input);

		auto i = m_Layers.size() - 1;
		auto pForwardLayer = m_Layers[i--].get();
		CalculateOutputBackPropError(target, pForwardLayer);
		do
		{
			auto pTargetLayer = m_Layers[i].get();
			CalculateHiddenBackPropError(pTargetLayer, pForwardLayer);
			pForwardLayer = pTargetLayer;
		}
		while (i--);

		auto pInput = &input;
		for (auto& layer : m_Layers)
		{
			ApplyDeltas(layer.get(), *pInput, learningRate, momentum);
			pInput = &layer->Output;
		}
	}

	real Evaluate(const ExampleSet& examples)
	{
		auto error = 0.f;

		for (const Example& example : examples)
			error += Error(example.Output, Calculate(example.Input));

		error /= examples.size();

		return error;
	}

	int InputSize;
	int OutputSize;

private:
	std::vector<std::unique_ptr<Layer>> m_Layers;
};

Example CreateExample(real i0, real i1, real o0)
{
	real input[] = { i0, i1 };
	real output[] = { o0 };
	return Example(input, 2, output, 1);
}

void Run()
{
	const auto TRAINING_ITERATIONS = 1000000u;
	const auto LEARNING_RATE = 0.05f;
	const auto MOMENTUM = 0.5f;

	ExampleSet trainingSet(2, 1);
	trainingSet.AddExample(CreateExample(0.f, 0.f, 0.f));
	trainingSet.AddExample(CreateExample(1.f, 0.f, 1.f));
	trainingSet.AddExample(CreateExample(0.f, 1.f, 1.f));
	trainingSet.AddExample(CreateExample(1.f, 1.f, 0.f));

	for (auto i = 0; i < 10; i++)
	{
		printf("\nRun %d:\n", i + 1);

		Network network(trainingSet.InputSize);
		network.AddLayer<Relu>(3);
		network.AddLayer<Relu>(1);

		auto trainingCount = 0u;
		do
		{
			for (auto i = 0; i < 100; i++)
			{
				auto& example = trainingSet.Sample();
				network.Train(example.Input, example.Output, LEARNING_RATE, MOMENTUM);
				trainingCount++;
			}
		}
		while (network.Evaluate(trainingSet) > 0.00001f && trainingCount < TRAINING_ITERATIONS);

		printf("Training Count = %d\n", trainingCount);
		for (const auto& example : trainingSet)
		{
			auto output = network.Calculate(example.Input);
			printf("%f, %f = %f, Error = %f\n", example.Input[0], example.Input[1], output[0], Error(example.Output, output));
		}
	}
}

}