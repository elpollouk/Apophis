#include "stdafx.h"
#include "apophis/Training/Evaluator.h"
#include "apophis/Component/Network.h"
#include "apophis/ExampleSet.h"

using namespace Apophis;
using namespace Apophis::Training;

Evaluator::Evaluator(Component::Network& network, LossFunction loss, const ExampleSet& examples) :
	m_Network(network),
	m_Loss(loss),
	m_Examples(examples)
{
	assert(loss != nullptr);
	assert(network.GetInputSize() == examples.InputSize);
	assert(network.GetOutputSize() == examples.OutputSize);
}

real Evaluator::Evaluate()
{
	auto error = 0.f;

	for (const Example& example : m_Examples)
		error += m_Loss(example.Output, m_Network.Calculate(example.Input));

	error /= m_Examples.size();

	return error;
}

SampledEvaluator::SampledEvaluator(Component::Network& network, LossFunction loss, const IExampleProvider& examples, size_t numSamples) :
	m_Network(network),
	m_Loss(loss),
	m_Examples(examples),
	m_NumSumples(numSamples)
{
	assert(loss != nullptr);
	assert(network.GetInputSize() == examples.InputSize);
	assert(network.GetOutputSize() == examples.OutputSize);
}

real SampledEvaluator::Evaluate()
{
	auto error = 0.f;

	for (auto i = (size_t)0; i < m_NumSumples; i++)
	{
		auto& example = m_Examples.Sample();
		error += m_Loss(example.Output, m_Network.Calculate(example.Input));
	}

	error /= (real)m_NumSumples;

	return error;
}