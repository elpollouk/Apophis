#include "stdafx.h"
#include "apophis/Data/Metrics.h"
#include "apophis/Training/StoppingConditions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Apophis::Data;
using namespace Apophis::Training;

namespace ApophisTests { namespace Training
{

	TEST_CLASS(StoppingConditionTests)
	{
	public:
		TEST_METHOD(MetricLessThan)
		{
			Metrics metrics;
			metrics.Set("A", 20.f);

			auto condition = StoppingCondition::MetricLessThan<float>("A", 10.f);
			Assert::IsFalse(condition(metrics));

			metrics.Set("A", 10.f);
			Assert::IsFalse(condition(metrics));

			metrics.Set("A", 9.9999f);
			Assert::IsTrue(condition(metrics));
		}

		TEST_METHOD(MetricGreaterThanOrEqual)
		{
			Metrics metrics;
			auto condition = StoppingCondition::MetricGreaterThanOrEqual<double>("B", 10.);
			Assert::IsFalse(condition(metrics));

			metrics.Set("B", 10.);
			Assert::IsTrue(condition(metrics));

			metrics.Set("B", 10.00001);
			Assert::IsTrue(condition(metrics));
		}

		TEST_METHOD(NumTrainingIterations)
		{
			Metrics metrics;
			metrics.Set(METRIC_TRAINING_ITERATIONS, 99999ll);

			auto condition = StoppingCondition::NumTrainingIterations(100000);
			Assert::IsFalse(condition(metrics));

			metrics.Add(METRIC_TRAINING_ITERATIONS, 1ll);
			Assert::IsTrue(condition(metrics));
		}

		TEST_METHOD(AnyStoppingCondition)
		{
			Metrics metrics;
			metrics.Set("A", 7);
			metrics.Set("B", 11ll);

			auto condition = StoppingCondition::AnyStoppingCondition();
			condition.Add<StoppingCondition::MetricLessThan<int>>("A", 5);
			condition.Add<StoppingCondition::MetricGreaterThanOrEqual<long long>>("B", 13ll);
			Assert::IsFalse(condition(metrics));

			metrics.Set("A", 3);
			Assert::IsTrue(condition(metrics));

			metrics.Set("A", 30);
			Assert::IsFalse(condition(metrics));

			metrics.Set("B", 17);
			Assert::IsTrue(condition(metrics));
		}
	};

}}