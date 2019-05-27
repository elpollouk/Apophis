#pragma once

#include "apophis/apophistypes.h"
#include "apophis/Component/Layer.h"

namespace Apophis { namespace Component {

	class Network
	{
	friend class Layer;
	public:
		Network(size_t inputSize);
		virtual ~Network() {}

		size_t GetInputSize() const { return m_InputSize; }
		size_t GetOutputSize() const { return m_OutputSize; }
		const std::vector<std::unique_ptr<Layer>>& GetLayers() const { return m_Layers; }

		template<class TransferFunction>
		void AddLayer(size_t numNodes)
		{
			AddLayer(numNodes, TransferFunction::Default());
		}

		void AddLayer(size_t numNodes, const TransferFunction::ITransferFunction& transfer);
		ConstVectorRef Calculate(ConstVectorRef input);

		void Export(Utils::ExportTarget& output) const;
		std::string Export() const;

	protected:
		virtual std::unique_ptr<Layer> CreateLayer(size_t numNodes, const TransferFunction::ITransferFunction& transfer);
		virtual std::unique_ptr<Node> CreateNode(size_t numInputs, const TransferFunction::ITransferFunction& transfer);

		size_t m_InputSize;
		size_t m_OutputSize;
		std::vector<std::unique_ptr<Layer>> m_Layers;
	};

}}