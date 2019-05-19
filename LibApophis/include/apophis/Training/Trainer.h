#pragma once

#include "apophis/apophistypes.h"
#include "apophis/Data/Metrics.h"

namespace Apophis { namespace Training {

	class ITrainable
	{
	public:
		virtual ~ITrainable() {}

		virtual void Train(ConstVectorRef input, ConstVectorRef target) = 0;
	};

	class Trainer
	{
	public:
		Trainer(ITrainable& trainable, Evaluator& evaluator);

		Data::Metrics& GetMetrics() { return m_Metrics; }

		void Run(const ExampleSet& trainingSet, const IStoppingCondition& stoppingCondition);

	private:
		ITrainable& m_Trainable;
		Evaluator& m_Evaluator;
		Data::Metrics m_Metrics;
	};

}}