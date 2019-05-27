#include "stdafx.h"
#include "apophis/Component/Layer.h"
#include "apophis/Component/Network.h"
#include "apophis/Utils/IExportWriter.h"
#include "Utils/ImportExport.h"

using namespace Apophis;
using namespace Apophis::Component;
using namespace Apophis::Utils;

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

void Layer::Export(Utils::IExportWriter& writer)
{
	writer.Set(FIELD_TYPE, COMPONENTTYPE_LAYER);
	writer.Set(FIELD_INPUTSIZE, GetNumInputs());
	writer.Set(FIELD_OUTPUTSIZE, GetNumOutputs());
	writer.Set(FIELD_TRANSFER, Nodes[0]->GetTransferFunction().GetName());

	auto nodes = writer.SetArray(FIELD_NODES, Nodes.size());
	for (const auto& node : Nodes)
	{
		auto outNode = nodes->PushBackObject();
		node->Export(*outNode);
	}
}