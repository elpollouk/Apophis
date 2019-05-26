#include "stdafx.h"
#include "apophis/Component/Node.h"
#include "apophis/TransferFunction/ITransferFunction.h"
#include "apophis/Random.h"
#include "Utils/ImportExport.h"

using namespace Apophis;
using namespace Apophis::Component;

Node::Node(size_t numInputs, const TransferFunction::ITransferFunction& transfer) :
	m_NumInputs(numInputs),
	Activation(0),
	m_Transfer(transfer)
{
	// Add one for the bias
	numInputs++;

	Weights.resize(numInputs);
	for (auto i = 0u; i < numInputs; i++)
		Weights[i] = RandomWeight();
}

real Node::Calculate(ConstVectorRef input)
{
	assert(input.size() == Weights.size() - 1);

	Activation = Weights[GetNumInputs()];

	for (auto i = 0u; i < GetNumInputs(); i++)
		Activation += input[i] * Weights[i];

	assert(!isnan(Activation) && !isinf(Activation));
	return m_Transfer(Activation);
}

void Node::Export(Utils::ExportTarget& outputObject) const
{
	auto jWeights = outputObject.Create(rapidjson::kArrayType);
	jWeights.Reserve(Weights.size());
	for (auto weight : Weights)
		jWeights.PushBack(weight);

	outputObject.AddMember("weights", jWeights);
}