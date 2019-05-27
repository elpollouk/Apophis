#include "stdafx.h"
#include "apophis/Component/Network.h"
#include "Utils/ImportExport.h"

using namespace Apophis;
using namespace Apophis::Component;
using namespace Apophis::Utils;

Network::Network(size_t inputSize) :
	m_InputSize(inputSize),
	m_OutputSize(inputSize)
{

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

std::unique_ptr<Node> Network::CreateNode(size_t numInputs, const TransferFunction::ITransferFunction& transfer)
{
	return std::make_unique<Node>(numInputs, transfer);
}

void Network::Export(Utils::ExportTarget& output) const
{
	output.AddMember(FIELD_INPUTSIZE, GetInputSize());
	output.AddMember(FIELD_OUTPUTSIZE, GetOutputSize());
	auto layers = output.Create(rapidjson::kArrayType);
	layers.Reserve(GetLayers().size());
	for (auto& layer : GetLayers())
	{
		auto jLayer = layers.Create(rapidjson::kObjectType);
		layer->Export(jLayer);
		layers.PushBack(jLayer);
	}
	output.AddMember(FIELD_LAYERS, layers);
}

std::string Network::Export() const
{
	rapidjson::MemoryPoolAllocator<> allocator;
	ExportTarget output(rapidjson::kObjectType, allocator);

	Export(output);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	output.Target.Accept(writer);

	return std::string(buffer.GetString(), buffer.GetSize());
}