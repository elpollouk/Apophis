#pragma once

#include "apophis/apophistypes.h"

namespace Apophis { namespace Training {

	class Evaluator
	{
	public:
		Evaluator(Component::Network& network, LossFunction loss);

		real operator()(const ExampleSet& examples);

	private:
		Component::Network& m_Network;
		LossFunction m_Loss;
	};

}}