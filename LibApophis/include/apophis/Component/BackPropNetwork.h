#pragma once

#include "apophis/Component/Network.h"

namespace Apophis {	namespace Component {

	class BackPropNetwork : public ITrainableNetwork
	{
	public:
		BackPropNetwork(int inputSize, real learningRate, real momentum);

		virtual void Train(ConstVectorRef input, ConstVectorRef target) override;

	private:
		real m_LearningRate;
		real m_Momentum;
	};

}}