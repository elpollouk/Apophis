#pragma once

#include "apophis/apophistypes.h"
#include "apophis/Component/Layer.h"

namespace Apophis { namespace Component {

	class Network
	{
	friend class Layer;
	public:
		Network(size_t inputSize);
		Network(Utils::IImportReader& reader);
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

		virtual void Export(Utils::IExportWriter& writer) const;

	protected:
		virtual std::unique_ptr<Layer> CreateLayer(size_t numNodes, const TransferFunction::ITransferFunction& transfer);
		virtual std::unique_ptr<Node> CreateNode(size_t numInputs);

		size_t m_InputSize;
		size_t m_OutputSize;
		std::vector<std::unique_ptr<Layer>> m_Layers;
	};

}}