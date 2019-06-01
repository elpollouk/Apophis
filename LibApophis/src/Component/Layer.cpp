#include "stdafx.h"
#include "apophis/Component/Layer.h"
#include "apophis/Component/Network.h"
#include "apophis/Utils/IExportWriter.h"
#include "apophis/ApophisException.h"
#include "Utils/ImportExport.h"

using namespace Apophis;
using namespace Apophis::Component;
using namespace Apophis::Utils;

Layer::Layer(size_t numInputs, size_t numOutputs, const TransferFunction::ITransferFunction& transfer, Network& network) :
	m_NumOutputs(numOutputs),
	m_NumInputs(numInputs),
	m_pTransfer(&transfer)
{
	for (auto i = 0u; i < numOutputs; i++)
		Nodes.emplace_back(network.CreateNode(numInputs));

	m_Output.resize(numOutputs, 0.f);
}

ConstVectorRef Layer::Calculate(ConstVectorRef inputs)
{
	assert(inputs.size() == GetNumInputs());

	for (auto i = 0u; i < GetNumOutputs(); i++)
	{
		auto activation = Nodes[i]->Calculate(inputs);
		m_Output[i] = GetTransferFunction()(activation);
	}

	return m_Output;
}

void Layer::Import(Utils::IImportReader& data)
{
	if (data.GetSize_t(FIELD_INPUTSIZE) != GetNumInputs()) throw ApophisException("Wrong input size for layer expecified. Expected %d, received %d", (int)GetNumInputs(), data.GetInt32(FIELD_INPUTSIZE));
	if (data.GetSize_t(FIELD_OUTPUTSIZE) != GetNumOutputs()) throw ApophisException("Wrong output size for layer expecified. Expected %d, received %d", (int)GetNumOutputs(), data.GetInt32(FIELD_OUTPUTSIZE));
	m_pTransfer = &TransferFunction::ITransferFunction::Get(data.GetString(FIELD_TRANSFER));

	auto nodes = data.GetArray(FIELD_NODES);
	if (nodes->Size() != Nodes.size()) throw ApophisException("Wrong number of nodes specified. Expeceted %d, received %d", (int)Nodes.size(), (int)nodes->Size());
	for (auto i = 0; i < (int)Nodes.size(); i++)
		Nodes[i]->Import(*nodes->GetObject(i));
}

void Layer::Export(Utils::IExportWriter& writer) const
{
	writer.Set(FIELD_TYPE, COMPONENTTYPE_LAYER);
	writer.Set(FIELD_INPUTSIZE, GetNumInputs());
	writer.Set(FIELD_OUTPUTSIZE, GetNumOutputs());
	writer.Set(FIELD_TRANSFER, m_pTransfer->GetName());

	auto nodes = writer.SetArray(FIELD_NODES, Nodes.size());
	for (const auto& node : Nodes)
	{
		auto outNode = nodes->PushBackObject();
		node->Export(*outNode);
	}
}