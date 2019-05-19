#pragma once

#include <vector>
#include <memory>
#include "apophis/Data/Metrics.h"

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

	template<class DType>
	class MetricLessThan : public MerticStoppingCondition<DType>
	{
	public:
		MetricLessThan(const char* metric, DType target) : MerticStoppingCondition<DType>(metric, target) {}

		virtual bool Check(Data::Metrics& metrics) const override
		{
			return metrics.Get<DType>(this->m_Metric) < this->m_Target;
		}
	};

	template<class DType>
	class MetricGreaterThanOrEqual : public MerticStoppingCondition<DType>
	{
	public:
		MetricGreaterThanOrEqual(const char* metric, DType target) : MerticStoppingCondition<DType>(metric, target) {}

		virtual bool Check(Data::Metrics& metrics) const override
		{
			return metrics.Get<DType>(this->m_Metric) >= this->m_Target;
		}
	};

	class NumTrainingIterations : public MetricGreaterThanOrEqual<long long>
	{
	public:
		NumTrainingIterations(long long maxSteps) : MetricGreaterThanOrEqual<long long>(Data::METRIC_TRAINING_ITERATIONS, maxSteps) {}
	};

}}}
