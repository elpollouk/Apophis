#pragma once

#include "apophis/apophistypes.h"

namespace Apophis { namespace Training {

	class IEvaluator
	{
	public:
		virtual ~IEvaluator() {}

		virtual real Evaluate() = 0;

		real operator()() { return Evaluate(); }
	};

	class Evaluator : public IEvaluator
	{
	public:
		Evaluator(Component::Network& network, LossFunction loss, const ExampleSet& examples);

		virtual real Evaluate() override;

	private:
		Component::Network& m_Network;
		LossFunction m_Loss;
		const ExampleSet& m_Examples;
	};

	class SampledEvaluator : public IEvaluator
	{
	public:
		SampledEvaluator(Component::Network& network, LossFunction loss, const IExampleProvider& examples, size_t numSamples);

		virtual real Evaluate() override;

	private:
		Component::Network& m_Network;
		LossFunction m_Loss;
		const IExampleProvider& m_Examples;
		size_t m_NumSumples;
	};
}}