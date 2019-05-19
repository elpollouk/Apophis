#include "stdafx.h"
#include "apophis/Training/Evaluator.h"
#include "apophis/Component/Network.h"
#include "apophis/ExampleSet.h"

using namespace Apophis;
using namespace Apophis::Training;

Evaluator::Evaluator(Component::Network& network, LossFunction loss) :
	m_Network(network),
	m_Loss(loss)
{
	assert(loss != nullptr);
}

real Evaluator::operator()(const ExampleSet& examples)
{
	assert(examples.InputSize == m_Network.GetInputSize());
	assert(examples.OutputSize == m_Network.GetOutputSize());

	auto error = 0.f;

	for (const Example& example : examples)
		error += m_Loss(example.Output, m_Network.Calculate(example.Input));

	error /= examples.size();

	return error;
}
