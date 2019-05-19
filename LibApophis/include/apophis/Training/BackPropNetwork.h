#pragma once

#include "apophis/Component/Network.h"

namespace Apophis {	namespace Training {

	class BackPropNetwork : public Component::ITrainableNetwork
	{
	public:
		BackPropNetwork(size_t inputSize, real learningRate, real momentum);

		virtual void Train(ConstVectorRef input, ConstVectorRef target) override;

	private:
		real m_LearningRate;
		real m_Momentum;
	};

}}