#include "stdafx.h"
#include "apophis/Component/Node.h"
#include "apophis/TransferFunction/ITransferFunction.h"
#include "apophis/Random.h"
#include "apophis/Utils/IExportWriter.h"
#include "Utils/ImportExport.h"

using namespace Apophis;
using namespace Apophis::Component;
using namespace Apophis::Utils;

Node::Node(size_t numInputs) :
	m_NumInputs(numInputs),
	Activation(0)
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
	return Activation;
}

void Node::Import(Utils::IImportReader& data)
{
	auto weights = data.GetArray(FIELD_WEIGHTS);
	if (weights->Size() != Weights.size()) throw ApophisException("Wrong number of weights for node, expected %d, received %d", (int)Weights.size(), (int)weights->Size());

	for (auto i = 0; i < weights->Size(); i++)
		Weights[i] = weights->GetReal(i);
}

void Node::Export(Utils::IExportWriter& writer) const
{
	writer.Set(FIELD_TYPE, COMPONENTTYPE_NODE);

	auto outWeights = writer.SetArray(FIELD_WEIGHTS, Weights.size());
	for (auto weight : Weights)
		outWeights->PushBack(weight);
}