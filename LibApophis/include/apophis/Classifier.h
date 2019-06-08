#pragma once

#include "apophis/Component/Network.h"

namespace Apophis {

	template <class TClassification>
	class Classifier
	{
	public:
		Classifier(Component::Network& network, std::initializer_list<TClassification> classifications, real threshold, TClassification none) :
			m_Network(network),
			m_Classifictions(classifications),
			m_Threshold(threshold),
			m_None(none)
		{
			
		}

		TClassification Classify(ConstVectorRef input)
		{
			auto& result = m_Network.Calculate(input);
			size_t argmax = result.argmax();

			return result[argmax] < m_Threshold ? m_None : m_Classifictions[argmax];
		}

		TClassification operator()(ConstVectorRef input) { return Classify(input); }

	private:
		Component::Network& m_Network;
		std::vector<TClassification> m_Classifictions;
		real m_Threshold;
		TClassification m_None;
	};
}