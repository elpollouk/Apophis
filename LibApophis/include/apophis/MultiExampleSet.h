#pragma once

#include "apophis/ExampleSet.h"
#include <vector>

namespace Apophis {

	class MultiExampleSet : public IExampleProvider
	{
	public:
		MultiExampleSet(std::initializer_list<const IExampleProvider*> providers);

		virtual size_t GetInputSize() const override { return m_InputSize; }
		virtual size_t GetOutputSize() const override { return m_OutputSize; }
		virtual const Example& Sample() const override;

	private:
		size_t m_InputSize;
		size_t  m_OutputSize;
		std::uniform_int_distribution<size_t> m_Distribution;
		std::vector<const IExampleProvider*> m_Providers;
	};
}