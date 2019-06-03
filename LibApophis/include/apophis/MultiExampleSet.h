#pragma once

#include "apophis/ExampleSet.h"
#include <vector>

namespace Apophis {

	class MultiExampleSet : public IExampleProvider
	{
	public:
		MultiExampleSet(std::initializer_list<const IExampleProvider*> providers);
		virtual const Example& Sample() const override;

	private:
		std::uniform_int_distribution<size_t> m_Distribution;
		std::vector<const IExampleProvider*> m_Providers;
	};
}