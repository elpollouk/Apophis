#pragma once

#include "apophis/Training/LossFunctions.h"
#include "apophis/ExampleSet.h"
#include "apophis/Component/Network.h"

namespace Apophis { namespace Training {

	class Evaluator
	{
	public:
		Evaluator(Component::Network* pNetwork, Loss::PFNLoss pLoss) :
			m_pNetwork(pNetwork),
			m_pLoss(pLoss)
		{

		}

		real operator()(const ExampleSet& examples)
		{
			auto error = 0.f;

			for (const Example& example : examples)
				error += m_pLoss(example.Output, m_pNetwork->Calculate(example.Input));

			error /= examples.size();

			return error;
		}

	private:
		Component::Network* m_pNetwork;
		Loss::PFNLoss m_pLoss;
	};

}}