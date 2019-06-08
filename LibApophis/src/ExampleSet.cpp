#include "stdafx.h"
#include "apophis/ExampleSet.h"
#include "apophis/ApophisException.h"
#include "apophis/Utils/IExportWriter.h"
#include "apophis/Utils/IImportReader.h"
#include "Utils/ImportExport.h"
#include "Random.h"

using namespace Apophis::Utils;

namespace Apophis {

ExampleSet::ExampleSet(Utils::IImportReader& data)
{
	Import(data);
}

ExampleSet::ExampleSet(int inputSize, int outputSize) :
	m_InputSize(inputSize),
	m_OutputSize(outputSize)
{

}

void ExampleSet::AddExample(Example&& example)
{
	assert(example.Input.size() == GetInputSize());
	assert(example.Output.size() == GetOutputSize());

	m_Examples.emplace_back(std::move(example));
	m_Distribution = std::uniform_int_distribution<size_t>(0, m_Examples.size() - 1);
}

const Example& ExampleSet::Sample() const
{
	return m_Examples[m_Distribution(GetRandomGenerator())];
}

void ImportVector(VectorRef outputVector, IImportReader& dataVector, size_t expectedSize)
{
	if (dataVector.Size() != expectedSize) throw ApophisException("Vector has wrong number of elements, expected %d but contained %d", (int)expectedSize, (int)dataVector.Size());

	outputVector.resize(dataVector.Size());
	for (auto i = 0; i < (int)dataVector.Size(); i++)
		outputVector[i] = dataVector.GetReal(i);
}

void ExampleSet::Import(IImportReader& data)
{
	m_Examples.clear();

	m_InputSize = data.GetInt32(FIELD_INPUTSIZE);
	m_OutputSize = data.GetInt32(FIELD_OUTPUTSIZE);

	auto examples = data.GetArray(FIELD_EXAMPLES);
	for (auto i = 0; i < (int)examples->Size(); i++)
	{
		Example example;
		auto jExample = examples->GetObject(i);
		ImportVector(example.Input, *jExample->GetArray(FIELD_INPUT), m_InputSize);
		ImportVector(example.Output, *jExample->GetArray(FIELD_OUTPUT), m_OutputSize);
		AddExample(std::move(example));
	}

}

void ExportVector(IExportWriter& outputArray, ConstVectorRef vector)
{
	for (auto value : vector)
		outputArray.PushBack(value);
}

void ExportExample(IExportWriter& outputArray, const Example& example)
{
	auto outputExample = outputArray.PushBackObject();

	auto input = outputExample->SetArray(FIELD_INPUT, example.Input.size());
	ExportVector(*input, example.Input);

	auto output = outputExample->SetArray(FIELD_OUTPUT, example.Output.size());
	ExportVector(*output, example.Output);
}

void ExampleSet::Export(IExportWriter& output) const
{
	output.Set(FIELD_TYPE, COMPONENTTYPE_EXAMPLESET);
	output.Set(FIELD_INPUTSIZE, m_InputSize);
	output.Set(FIELD_OUTPUTSIZE, m_OutputSize);

	auto examples = output.SetArray(FIELD_EXAMPLES, m_Examples.size());
	for (const auto& example : m_Examples)
		ExportExample(*examples, example);
}

}