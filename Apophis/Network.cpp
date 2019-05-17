#include "pch.h"
#include "Network.h"

#define RELU

typedef float real;
typedef std::vector<real> Vector;
typedef const Vector& VectorRef;

static std::default_random_engine generator(1234);
static std::uniform_real_distribution<real> distribution(0.f, 1.f);
static auto RandomWeight = std::bind(distribution, generator);

class Example
{
public:
	Example(const real* input, int inputSize, const real* output, int outputSize) :
		Input(input, input + inputSize),
		Output(output, output + outputSize)
	{

	}

	const Vector Input;
	const Vector Output;
};

class ExampleSet
{
public:
	ExampleSet(int inputSize, int outputSize) :
		InputSize(inputSize),
		OutputSize(outputSize)
	{

	}

	void AddExample(Example&& example)
	{
		assert(example.Input.size() == InputSize);
		assert(example.Output.size() == OutputSize);

		m_Examples.emplace_back(example);
		m_Distribution = std::uniform_int_distribution<size_t>(0, m_Examples.size() - 1);
	}

	const Example& Sample()
	{
		return m_Examples[m_Distribution(generator)];
	}

	std::vector<Example>::const_iterator begin() const
	{
		return m_Examples.begin();
	}

	std::vector<Example>::const_iterator end() const
	{
		return m_Examples.end();
	}

	size_t size() const
	{
		return m_Examples.size();
	}

	const int InputSize;
	const int OutputSize;

private:
	std::uniform_int_distribution<size_t> m_Distribution;
	std::vector<Example> m_Examples;
};

namespace Network
{
#if defined(RELU)
real Transfer(real activation)
{
	return activation > 0 ? activation : 0.01f * activation;
}

real TransferDerivitive(real activation)
{
	return activation > 0 ? 1.f : 0.01f;
}
#else
real Transfer(real activation)
{
	return 1 / (1 + expf(-activation));
}

real TransferDerivitive(real activation)
{
	activation = Transfer(activation);
	return activation * (1 - activation);
}
#endif

class Node
{
public:
	Node(int numInputs) :
		NumInputs(numInputs),
		BackPropError(0.f),
		Activation(0.f)
	{
		// Add one for the bias
		numInputs++;

		Weights.resize(numInputs);
		for (auto i = 0; i < numInputs; i++)
			Weights[i] = RandomWeight();
		PreviousWeightChanges.resize(numInputs, 0.f);
	}


	real Calculate(VectorRef input)
	{
		assert(input.size() == Weights.size() - 1);

		Activation = Weights[NumInputs];

		for (auto i = 0; i < NumInputs; i++)
			Activation += input[i] * Weights[i];

		return Transfer(Activation);
	}

	const int NumInputs;
	real Activation;
	real BackPropError;
	Vector Weights;
	Vector PreviousWeightChanges;
};

class Layer
{
public:
	Layer(int size, int numInputs) :
		Size(size),
		NumInputs(numInputs)
	{
		for (auto i = 0; i < size; i++)
			Nodes.emplace_back(std::make_unique<Node>(numInputs));

		Outputs.resize(size, 0.f);
	}

	VectorRef Calculate(VectorRef inputs)
	{
		for (auto i = 0; i < Size; i++)
			Outputs[i] = Nodes[i]->Calculate(inputs);

		return Outputs;
	}

	const int Size;
	const int NumInputs;
	std::vector<std::unique_ptr<Node>> Nodes;
	Vector Outputs;
};

real Error(VectorRef target, VectorRef actual)
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

void CalculateOutputBackPropError(VectorRef targets, Layer* layer)
{
	assert(targets.size() == layer->Outputs.size());

	for (auto i = 0; i < layer->Size; i++)
		layer->Nodes[i]->BackPropError = (targets[i] - layer->Outputs[i]) * TransferDerivitive(layer->Nodes[i]->Activation);
}

void CalculateHiddenBackPropError(Layer* targetLayer, Layer* forwardLayer)
{
	for (auto i = 0; i < targetLayer->Size; i++)
	{
		targetLayer->Nodes[i]->BackPropError = 0.f;
		for (auto k = 0; k < forwardLayer->Size; k++)
		{
			targetLayer->Nodes[i]->BackPropError += forwardLayer->Nodes[k]->BackPropError * forwardLayer->Nodes[k]->Weights[i];
		}
		targetLayer->Nodes[i]->BackPropError *= TransferDerivitive(targetLayer->Nodes[i]->Activation);
	}
}

void ApplyDeltas(Layer* targetLayer, VectorRef priorLayerOutput, real learningRate, real momentum)
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

	void AddLayer(int numNodes)
	{
		m_Layers.emplace_back(std::make_unique<Layer>(numNodes, OutputSize));
		OutputSize = numNodes;
	}

	VectorRef Calculate(VectorRef input)
	{
		assert(input.size() == InputSize);

		auto pInput = &input;
		for (auto& layer : m_Layers)
			pInput = &layer->Calculate(*pInput);

		assert((*pInput)[0] != -NAN);
		return *pInput;
	}

	void Train(VectorRef input, VectorRef target, real learningRate, real momentum)
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
			pInput = &layer->Outputs;
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
		network.AddLayer(2);
		network.AddLayer(1);

		auto trainingCount = 0;
		do
		{
			for (auto i = 0; i < 100; i++)
			{
				auto& example = trainingSet.Sample();
				network.Train(example.Input, example.Output, LEARNING_RATE, MOMENTUM);
				trainingCount++;
			}
		}
		while (network.Evaluate(trainingSet) > 0.00001f && trainingCount < 100000);

		printf("Training Count = %d\n", trainingCount);
		for (const auto& example : trainingSet)
		{
			auto output = network.Calculate(example.Input);
			printf("%f, %f = %f, Error = %f\n", example.Input[0], example.Input[1], output[0], Error(example.Output, output));
		}
	}
}

}