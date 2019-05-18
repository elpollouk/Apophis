#include "stdafx.h"
#include "apophis/Component/Layer.h"
#include "apophis/Component/Network.h"

using namespace Apophis;
using namespace Apophis::Component;

Layer::Layer(size_t numInputs, size_t numOutputs, const TransferFunction::ITransferFunction* transfer, Network* network) :
	NumOutputs(numOutputs),
	NumInputs(numInputs),
	m_pNetwork(network)
{
	for (auto i = 0u; i < numOutputs; i++)
		Nodes.emplace_back(m_pNetwork->CreateNode(numInputs, transfer));

	Output.resize(numOutputs, 0.f);
}

ConstVectorRef Layer::Calculate(ConstVectorRef inputs)
{
	for (auto i = 0u; i < NumOutputs; i++)
		Output[i] = Nodes[i]->Calculate(inputs);

	return Output;
}