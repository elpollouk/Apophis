#include "stdafx.h"
#include "apophis/Component/Network.h"
#include "apophis/Utils/IExportWriter.h"
#include "apophis/Utils/IImportReader.h"
#include "apophis/ApophisException.h"
#include "Utils/ImportExport.h"

using namespace Apophis;
using namespace Apophis::Component;
using namespace Apophis::Utils;

Network::Network(size_t inputSize) :
	m_InputSize(inputSize),
	m_OutputSize(inputSize)
{

}

Network::Network(Utils::IImportReader& reader)
{
	m_InputSize = reader.GetSize_t(FIELD_INPUTSIZE);
	m_OutputSize = m_InputSize;

	auto layers = reader.GetArray(FIELD_LAYERS);
	for (size_t i = 0; i < layers->Size(); i++)
	{
		auto layer = layers->GetObject(i);
		auto layerSize = layer->GetSize_t(FIELD_OUTPUTSIZE);
		AddLayer(layerSize, TransferFunction::Relu::Default());
		m_Layers[i]->Import(*layer);
	}

	if (m_OutputSize != reader.GetSize_t(FIELD_OUTPUTSIZE)) throw ApophisException("Incorrect output size for network, expected %d, last layer size %d", reader.GetInt32(FIELD_OUTPUTSIZE), (int)m_OutputSize);
}

void Network::AddLayer(size_t numNodes, const TransferFunction::ITransferFunction& transfer)
{
	m_Layers.emplace_back(CreateLayer(numNodes, transfer));
	m_OutputSize = numNodes;
}

ConstVectorRef Network::Calculate(ConstVectorRef input)
{
	assert(input.size() == GetInputSize());

	auto pInput = &input;
	for (auto& layer : m_Layers)
		pInput = &layer->Calculate(*pInput);

	assert(pInput->size() == GetOutputSize());
	return *pInput;
}

std::unique_ptr<Layer> Network::CreateLayer(size_t numNodes, const TransferFunction::ITransferFunction& transfer)
{
	return std::make_unique<Layer>(GetOutputSize(), numNodes, transfer, *this);
}

std::unique_ptr<Node> Network::CreateNode(size_t numInputs)
{
	return std::make_unique<Node>(numInputs);
}

void Network::Export(Utils::IExportWriter& writer) const
{
	writer.Set(FIELD_TYPE, COMPONENTTYPE_NETWORK);
	writer.Set(FIELD_INPUTSIZE, GetInputSize());
	writer.Set(FIELD_OUTPUTSIZE, GetOutputSize());

	auto layers = writer.SetArray(FIELD_LAYERS, GetLayers().size());
	for (auto& layer : GetLayers())
	{
		auto outLayer = layers->PushBackObject();
		layer->Export(*outLayer);
	}
}
