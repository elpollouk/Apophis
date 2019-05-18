#pragma once

#include "apophis/Training/Evaluator.h"
#include "apophis/Training/StoppingConditions.h"
#include "apophis/Data/Metrics.h"

namespace Apophis { namespace Training {

	class Trainer
	{
	public:
		Trainer(Component::ITrainableNetwork* pNetwork, Evaluator* pEvaluator) :
			m_pNetwork(pNetwork),
			m_pEvaluator(pEvaluator)
		{

		}

		Data::Metrics& GetMetrics() { return m_Metrics; }

		void Run(ExampleSet& trainingSet, const StoppingCondition::IStoppingCondition& stoppingCondition)
		{
			auto trainingCount = 0;
			do
			{
				for (auto i = 0; i < 100; i++)
				{
					auto& example = trainingSet.Sample();
					m_pNetwork->Train(example.Input, example.Output);
					trainingCount++;
				}

				auto error = (*m_pEvaluator)(trainingSet);

				m_Metrics.Set(Data::METRIC_TRAINING_ERROR, error);
				m_Metrics.Set(Data::METRIC_TRAINING_ITERATIONS, trainingCount);
			} while (!stoppingCondition(m_Metrics));
		}

	private:
		Component::ITrainableNetwork* m_pNetwork;
		Evaluator* m_pEvaluator;
		Data::Metrics m_Metrics;
	};

}}