#pragma once

#include "apophis/apophistypes.h"

namespace Apophis { namespace Component {

	class Node
	{
	public:
		Node(size_t numInputs);
		virtual ~Node() {}

		size_t GetNumInputs() const { return m_NumInputs; }

		real Calculate(ConstVectorRef input);

		real Activation;
		Vector Weights;

		virtual void Import(Utils::IImportReader& data);
		virtual void Export(Utils::IExportWriter& writer) const;

	protected:
		size_t m_NumInputs;
	};

}}