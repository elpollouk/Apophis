#pragma once

#include "apophis/Data/Metrics.h"

namespace Apophis {
	class ExampleSet;
	namespace Component {
		class ITrainableNetwork;
	}
	namespace Training {
		class Evaluator;
		namespace StoppingCondition {
			class IStoppingCondition;
		}
	}
}

namespace Apophis { namespace Training {

	class Trainer
	{
	public:
		Trainer(Component::ITrainableNetwork& network, Evaluator& evaluator);

		Data::Metrics& GetMetrics() { return m_Metrics; }

		void Run(const ExampleSet& trainingSet, const StoppingCondition::IStoppingCondition& stoppingCondition);

	private:
		Component::ITrainableNetwork& m_Network;
		Evaluator& m_Evaluator;
		Data::Metrics m_Metrics;
	};

}}