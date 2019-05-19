#include "stdafx.h"
#include "apophis/Component/Network.h"

using namespace Apophis;
using namespace Apophis::Component;

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
