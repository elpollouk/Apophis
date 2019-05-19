#pragma once

#include "apophis/Training/LossFunctions.h"

namespace Apophis { 
	class ExampleSet;
	namespace Component {
		class Network;
	}
}

namespace Apophis { namespace Training {

	class Evaluator
	{
	public:
		Evaluator(Component::Network& network, Loss::PFNLoss loss);

		real operator()(const ExampleSet& examples);

	private:
		Component::Network& m_Network;
		Loss::PFNLoss m_Loss;
	};

}}