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

}

real Evaluator::operator()(const ExampleSet& examples)
{
	auto error = 0.f;

	for (const Example& example : examples)
		error += m_Loss(example.Output, m_Network.Calculate(example.Input));

	error /= examples.size();

	return error;
}
