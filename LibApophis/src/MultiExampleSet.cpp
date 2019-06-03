#include "stdafx.h"
#include "apophis/MultiExampleSet.h"
#include "Random.h"

using namespace Apophis;

MultiExampleSet::MultiExampleSet(std::initializer_list<const IExampleProvider*> providers) :
	m_Providers(providers),
	m_Distribution(std::uniform_int_distribution<size_t>(0, providers.size() - 1))
{

}

const Example& MultiExampleSet::Sample() const
{
	auto index = m_Distribution(GetRandomGenerator());
	return m_Providers[index]->Sample();
}
