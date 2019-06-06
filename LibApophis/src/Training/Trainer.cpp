#include "stdafx.h"
#include "apophis/Training/Trainer.h"
#include "apophis/Component/Network.h"
#include "apophis/ExampleSet.h"
#include "apophis/Training/StoppingConditions.h"
#include "apophis/Training/Evaluator.h"
#include "apophis/Utils/Timer.h"

using namespace Apophis;
using namespace Apophis::Training;

Trainer::Trainer(ITrainable& trainable, IEvaluator& evaluator) :
	m_Trainable(trainable),
	m_Evaluator(evaluator)
{

}

void Trainer::Run(const IExampleProvider& trainingSet, const IStoppingCondition& stoppingCondition)
{
	Utils::Timer timer;
	auto trainingCount = 0ll;
	do
	{
		for (auto i = 0; i < 10000; i++)
		{
			auto& example = trainingSet.Sample();
			m_Trainable.Train(example);
			trainingCount++;
		}

		auto loss = m_Evaluator();

		m_Metrics.Set(Data::METRIC_TRAINING_LOSS, loss);
		m_Metrics.Set(Data::METRIC_TRAINING_ITERATIONS, trainingCount);

		if (timer.Read() >= 10)
		{
			timer.Reset();
			printf("%lli - Error = %f\n", trainingCount, loss);
		}
	} while (!stoppingCondition(m_Metrics));
}
