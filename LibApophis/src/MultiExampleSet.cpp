#include "stdafx.h"
#include "apophis/MultiExampleSet.h"
#include "Random.h"

using namespace Apophis;

MultiExampleSet::MultiExampleSet(std::initializer_list<const IExampleProvider*> providers) :
	m_InputSize((*providers.begin())->GetInputSize()),
	m_OutputSize((*providers.begin())->GetOutputSize()),
	m_Providers(providers),
	m_Distribution(std::uniform_int_distribution<size_t>(0, providers.size() - 1))
{
	for (const auto provider : m_Providers)
	{
		assert(provider->GetInputSize() == m_InputSize);
		assert(provider->GetOutputSize() == m_OutputSize);
	}
}

const Example& MultiExampleSet::Sample() const
{
	auto index = m_Distribution(GetRandomGenerator());
	return m_Providers[index]->Sample();
}
