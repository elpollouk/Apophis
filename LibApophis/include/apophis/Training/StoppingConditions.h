#pragma once

#include <vector>
#include <memory>
#include "apophis/apophistypes.h"
#include "apophis/Data/Metrics.h"

#define METRIC_COMPARISON_STOPPOINGCONDITION(name, comparison) template<class DType> \
class name : public MerticStoppingCondition<DType> \
{ \
public: \
	name(const char* metric, DType target) : MerticStoppingCondition<DType>(metric, target) {} \
	virtual bool Check(Data::Metrics& metrics) const override \
	{ \
		return metrics.Get<DType>(this->m_Metric) comparison this->m_Target; \
	} \
};

namespace Apophis { namespace Training {
	class IStoppingCondition
	{
	public:
		virtual ~IStoppingCondition() {}

		bool operator()(Data::Metrics& metrics) const { return Check(metrics); }

		virtual bool Check(Data::Metrics& metrics) const = 0;
	};
}}

namespace Apophis { namespace Training { namespace StoppingCondition {

	//----------------------------------------------------------------------------------------------//
	// Stop when any registered condition is met
	//----------------------------------------------------------------------------------------------//
	class AnyStoppingCondition : public IStoppingCondition
	{
	public:
		template<class Condition, class... Types>
		void Add(Types&&... args)
		{
			m_Conditions.emplace_back(std::make_unique<Condition>(std::forward<Types>(args)...));
		}

		virtual bool Check(Data::Metrics& metrics) const override;

	private:
		std::vector<std::unique_ptr<IStoppingCondition>> m_Conditions;
	};

	//----------------------------------------------------------------------------------------------//
	// Metric comparison conditions
	//----------------------------------------------------------------------------------------------//
	template<class DType>
	class MerticStoppingCondition : public IStoppingCondition
	{
	public:
		MerticStoppingCondition(const char* metric, DType target) :
			m_Metric(metric),
			m_Target(target)
		{

		}

	protected:
		std::string m_Metric;
		DType m_Target;
	};

	METRIC_COMPARISON_STOPPOINGCONDITION(MetricLessThan, < );
	METRIC_COMPARISON_STOPPOINGCONDITION(MetricGreaterThan, > );
	METRIC_COMPARISON_STOPPOINGCONDITION(MetricLessThanOrEqual, <= );
	METRIC_COMPARISON_STOPPOINGCONDITION(MetricGreaterThanOrEqual, >= );

	//----------------------------------------------------------------------------------------------//
	// Explicit metric conditions
	//----------------------------------------------------------------------------------------------//
	class LossLessThan : public MetricLessThan<real>
	{
	public:
		LossLessThan(real loss) : MetricLessThan<real>(Data::METRIC_TRAINING_LOSS, loss) {}
	};

	class NumTrainingIterations : public MetricGreaterThanOrEqual<long long>
	{
	public:
		NumTrainingIterations(long long maxSteps) : MetricGreaterThanOrEqual<long long>(Data::METRIC_TRAINING_ITERATIONS, maxSteps) {}
	};

}}}
