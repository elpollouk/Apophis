#include "pch.h"
#include "apophis/TransferFunction/Relu.h"
#include "apophis/TransferFunction/Sigmoid.h"
#include "apophis/Training/BackPropNetwork.h"
#include "apophis/Training/Trainer.h"
#include "apophis/Training/LossFunctions.h"
#include "apophis/Training/Evaluator.h"
#include "apophis/Training/StoppingConditions.h"
#include "apophis/ExampleSet.h"
#include "Network.h"

using namespace Apophis;
using namespace Apophis::Component;
using namespace Apophis::TransferFunction;
using namespace Apophis::Training;

namespace Network
{

Example CreateExample(real i0, real i1, real o0)
{
	real input[] = { i0, i1 };
	real output[] = { o0 };
	return Example(input, 2, output, 1);
}

void Run()
{
	const auto TRAINING_ITERATIONS = 1000000;
	const auto TRAINING_ERROR = 0.00001f;
	const auto LEARNING_RATE = 0.05f;
	const auto MOMENTUM = 0.5f;

	ExampleSet trainingSet(2, 1);
	trainingSet.AddExample(CreateExample(0.f, 0.f, 0.f));
	trainingSet.AddExample(CreateExample(1.f, 0.f, 1.f));
	trainingSet.AddExample(CreateExample(0.f, 1.f, 1.f));
	trainingSet.AddExample(CreateExample(1.f, 1.f, 0.f));

	for (auto i = 0; i < 10; i++)
	{
		printf("\nRun %d:\n", i + 1);

		BackPropNetwork network(trainingSet.InputSize, LEARNING_RATE, MOMENTUM);
		network.AddLayer<Relu>(3);
		network.AddLayer<Relu>(1);

		Evaluator evaluator(network, Loss::SquaredError);
		StoppingCondition::AnyStoppingCondition stoppingConditions;
		stoppingConditions.Add<StoppingCondition::MetricLessThan<real>>(Data::METRIC_TRAINING_ERROR, TRAINING_ERROR);
		stoppingConditions.Add<StoppingCondition::NumTrainingIterations>(TRAINING_ITERATIONS);

		Trainer trainer(network, evaluator);
		trainer.Run(trainingSet, stoppingConditions);

		printf("Training Count = %d\n", trainer.GetMetrics().Get<int>(Data::METRIC_TRAINING_ITERATIONS));
		for (const auto& example : trainingSet)
		{
			auto output = network.Calculate(example.Input);
			printf("%f, %f = %f, Error = %f\n", example.Input[0], example.Input[1], output[0], Training::Loss::SquaredError(example.Output, output));
		}
	}
}

}