#pragma once

#include <assert.h>
#include "apophis/apophis.h"

namespace Apophis { namespace Component {

	template <class DType, class TransferFunction>
	class Node
	{
	public:
		Node(size_t numInputs, TransferFunction&& transfer) :
			NumInputs(numInputs),
			Activation(0),
			BackPropError(0),
			Transfer(transfer)
		{
			// Add one for the bias
			numInputs++;

			Weights.resize(numInputs);
			for (auto i = 0; i < numInputs; i++)
				Weights[i] = RandomWeight();
			PreviousWeightChanges.resize(numInputs, 0.f);
		}

		real Calculate(ConstVectorRef input)
		{
			assert(input.size() == Weights.size() - 1);

			Activation = Weights[NumInputs];

			for (auto i = 0; i < NumInputs; i++)
				Activation += input[i] * Weights[i];

			return Transfer(Activation);
		}

		const size_t NumInputs;
		real Activation;
		real BackPropError;
		Vector Weights;
		Vector PreviousWeightChanges;
		const TransferFunction Transfer;
	};

}}