#include "stdafx.h"
#include "apophis/Component/Layer.h"
#include "apophis/Component/Network.h"

using namespace Apophis;
using namespace Apophis::Component;

Layer::Layer(size_t numInputs, size_t numOutputs, const TransferFunction::ITransferFunction& transfer, Network& network) :
	m_NumOutputs(numOutputs),
	m_NumInputs(numInputs)
{
	for (auto i = 0u; i < numOutputs; i++)
		Nodes.emplace_back(network.CreateNode(numInputs, transfer));

	m_Output.resize(numOutputs, 0.f);
}

ConstVectorRef Layer::Calculate(ConstVectorRef inputs)
{
	assert(inputs.size() == GetNumInputs());

	for (auto i = 0u; i < GetNumOutputs(); i++)
		m_Output[i] = Nodes[i]->Calculate(inputs);

	return m_Output;
}