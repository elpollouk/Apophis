#pragma once

#include "apophis/apophistypes.h"

namespace Apophis { namespace Training {

	class Evaluator
	{
	public:
		Evaluator(Component::Network& network, LossFunction loss, const ExampleSet& examples);

		real operator()();

	private:
		Component::Network& m_Network;
		LossFunction m_Loss;
		const ExampleSet& m_Examples;
	};

}}