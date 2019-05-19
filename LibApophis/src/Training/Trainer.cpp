#include "stdafx.h"
#include "apophis/Training/Trainer.h"
#include "apophis/Component/Network.h"
#include "apophis/ExampleSet.h"
#include "apophis/Training/StoppingConditions.h"
#include "apophis/Training/Evaluator.h"

using namespace Apophis;
using namespace Apophis::Training;

Trainer::Trainer(ITrainable& trainable, Evaluator& evaluator) :
	m_Trainable(trainable),
	m_Evaluator(evaluator)
{

}

void Trainer::Run(const ExampleSet& trainingSet, const IStoppingCondition& stoppingCondition)
{
	auto trainingCount = 0;
	do
	{
		for (auto i = 0; i < 100; i++)
		{
			auto& example = trainingSet.Sample();
			m_Trainable.Train(example);
			trainingCount++;
		}

		auto loss = m_Evaluator();

		m_Metrics.Set(Data::METRIC_TRAINING_LOSS, loss);
		m_Metrics.Set(Data::METRIC_TRAINING_ITERATIONS, trainingCount);
	} while (!stoppingCondition(m_Metrics));
}
