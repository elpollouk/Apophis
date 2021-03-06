#include "stdafx.h"
#include "apophis/apophis.h"
#include "apophis/Utils/IImportReader.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Apophis;
using namespace Apophis::TransferFunction;
using namespace Apophis::Training;
using namespace Apophis::Training::StoppingCondition;

namespace ApophisTests { namespace Training { namespace Algorithms {

	TEST_CLASS(BackPropTests)
	{
	public:
		TEST_METHOD(Xor)
		{
			const auto TRAINING_ITERATIONS = 5000;
			const auto TRAINING_ERROR = 0.00001f;
			const auto LEARNING_RATE = 0.05f;
			const auto MOMENTUM = 0.5f;

			ExampleSet trainingSet(2, 1);
			trainingSet.AddExample({ 0.f, 0.f }, { 0.f });
			trainingSet.AddExample({ 1.f, 0.f }, { 1.f });
			trainingSet.AddExample({ 0.f, 1.f }, { 1.f });
			trainingSet.AddExample({ 1.f, 1.f }, { 0.f });

			BackPropNetwork network(trainingSet.GetInputSize(), LEARNING_RATE, MOMENTUM);
			network.AddLayer<Relu>(3);
			network.AddLayer<Relu>(1);

			Evaluator evaluator(network, Loss::SquaredError, trainingSet);
			StoppingCondition::AnyStoppingCondition stoppingConditions;
			stoppingConditions.Add<LossLessThan>(TRAINING_ERROR);
			stoppingConditions.Add<NumTrainingIterations>(TRAINING_ITERATIONS);

			Trainer trainer(network, evaluator);
			trainer.Run(trainingSet, stoppingConditions);

			Assert::IsTrue(network.Calculate(trainingSet[0].Input)[0] <= 0.01f);
			Assert::IsTrue(network.Calculate(trainingSet[1].Input)[0] >= 0.99f);
			Assert::IsTrue(network.Calculate(trainingSet[2].Input)[0] >= 0.99f);
			Assert::IsTrue(network.Calculate(trainingSet[3].Input)[0] <= 0.01f);
		}

		TEST_METHOD(Import_Xor)
		{
			BackPropNetwork network(*LoadJson("../../../Data/Xor/network.json"), 0.f, 0.f);

			Assert::IsTrue(network.Calculate({ 0.f, 0.f })[0] <= 0.01f);
			Assert::IsTrue(network.Calculate({ 1.f, 0.f })[0] >= 0.99f);
			Assert::IsTrue(network.Calculate({ 0.f, 1.f })[0] >= 0.99f);
			Assert::IsTrue(network.Calculate({ 1.f, 1.f })[0] <= 0.01f);
		}
	};

}}}