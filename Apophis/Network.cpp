#include "pch.h"
#include "apophis/apophis.h"
#include "Iris.h"

using namespace Apophis;
using namespace Apophis::Component;
using namespace Apophis::TransferFunction;
using namespace Apophis::Training;
using namespace Apophis::Training::StoppingCondition;

namespace Network
{

const char* Clasify(ConstVectorRef scores)
{
	auto maxIndex = 0u;
	for (auto i = 1u; i < scores.size(); i++)
		if (scores[maxIndex] < scores[i])
			maxIndex = i;

	switch (maxIndex)
	{
	case 0:
		return "I.setosa";

	case 1:
		return "I.versicolor";

	case 2:
		return "I.virginica";

	default:
		return "UNKNOWN";
	}
}

void Run()
{
	const auto TRAINING_ITERATIONS = 10000000;
	const auto TRAINING_ERROR = 0.005f;
	const auto LEARNING_RATE = 0.001f;
	const auto MOMENTUM = 0.5f;

	ExampleSet trainingSet(4, 3);
	ExampleSet testSet(4, 3);
	Iris::PopulateTrainingSet(trainingSet);
	Iris::PopulateTestSet(testSet);

	for (auto i = 0; i < 10; i++)
	{
		printf("\nRun %d:\n", i + 1);

		BackPropNetwork network(trainingSet.InputSize, LEARNING_RATE, MOMENTUM);
		network.AddLayer<Relu>(4);
		network.AddLayer<Relu>(3);

		Evaluator evaluator(network, Loss::SquaredError, testSet);
		AnyStoppingCondition stoppingConditions;
		stoppingConditions.Add<LossLessThan>(TRAINING_ERROR);
		stoppingConditions.Add<NumTrainingIterations>(TRAINING_ITERATIONS);

		Trainer trainer(network, evaluator);
		trainer.Run(trainingSet, stoppingConditions);

		printf("Training Count = %d\n", trainer.GetMetrics().Get<int>(Data::METRIC_TRAINING_ITERATIONS));
		printf("Error = %f\n", trainer.GetMetrics().Get<float>(Data::METRIC_TRAINING_LOSS));

		for (auto i = 0u; i < testSet.size(); i++)
		{
			auto& output = network.Calculate(testSet[i].Input);
			printf("%02u: %s\n", i, Clasify(output));
		}
	}
}

}