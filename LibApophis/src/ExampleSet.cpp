#include "stdafx.h"
#include "apophis/ExampleSet.h"

namespace Apophis {

ExampleSet::ExampleSet(int inputSize, int outputSize) :
	InputSize(inputSize),
	OutputSize(outputSize)
{

}

void ExampleSet::AddExample(Example&& example)
{
	assert(example.Input.size() == InputSize);
	assert(example.Output.size() == OutputSize);

	m_Examples.emplace_back(std::move(example));
	m_Distribution = std::uniform_int_distribution<size_t>(0, m_Examples.size() - 1);
}

const Example& ExampleSet::Sample() const
{
	return m_Examples[m_Distribution(GetRandomGenerator())];
}

}