#include "stdafx.h"
#include "apophis/Training/Trainer.h"
#include "apophis/Component/Network.h"
#include "apophis/ExampleSet.h"
#include "apophis/Training/StoppingConditions.h"
#include "apophis/Training/Evaluator.h"
#include "apophis/Utils/Timer.h"

using namespace Apophis;
using namespace Apophis::Training;

static void NullProgress(const Data::Metrics& metrics) {}

Trainer::Trainer(ITrainable& trainable, IEvaluator& evaluator) :
	m_Trainable(trainable),
	m_Evaluator(evaluator)
{

}

void Trainer::Run(const IExampleProvider& trainingSet, const IStoppingCondition& stoppingCondition, std::function<void(const Data::Metrics&)> onProgress)
{
	if (onProgress == nullptr) onProgress = NullProgress;

	Utils::Timer timer;
	auto trainingCount = 0ll;
	auto loss = m_Evaluator();

	m_Metrics.Set(Data::METRIC_TRAINING_LOSS, loss);
	onProgress(m_Metrics);

	do
	{
		for (auto i = 0; i < 10000; i++)
		{
			auto& example = trainingSet.Sample();
			m_Trainable.Train(example);
			trainingCount++;
		}

		loss = m_Evaluator();

		m_Metrics.Set(Data::METRIC_TRAINING_LOSS, loss);
		m_Metrics.Set(Data::METRIC_TRAINING_ITERATIONS, trainingCount);
		m_Metrics.Set(Data::METRIC_TRAINING_TIME, timer.Total());

		if (timer.Read() >= 10.)
		{
			onProgress(m_Metrics);
			timer.Reset();
		}

	} while (!stoppingCondition(m_Metrics));

	onProgress(m_Metrics);
}
