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

			size_t maxIndex = 0;
			for (auto i = (size_t)1; i < result.size(); i++)
				if (result[maxIndex] < result[i])
					maxIndex = i;

			return result[maxIndex] < m_Threshold ? m_None : m_Classifictions[maxIndex];
		}

		TClassification operator()(ConstVectorRef input) { return Classify(input); }

	private:
		Component::Network& m_Network;
		std::vector<TClassification> m_Classifictions;
		real m_Threshold;
		TClassification m_None;
	};
}