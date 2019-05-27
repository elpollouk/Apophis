#include "stdafx.h"
#include "apophis/ExampleSet.h"
#include "apophis/ApophisException.h"
#include "apophis/Utils/IExportWriter.h"
#include "Random.h"

using namespace Apophis::Utils;

namespace Apophis {

ExampleSet::ExampleSet() : ExampleSet(0, 0)
{
}

ExampleSet::ExampleSet(int inputSize, int outputSize) :
	InputSize(inputSize),
	OutputSize(outputSize)
{

}

void ExampleSet::AddExample(Example&& example)
{
	assert(example.Input.size() == InputSize);
	assert(example.Output.size() == OutputSize);

	m_Examples.emplace_back(std::move(example));
	m_Distribution = std::uniform_int_distribution<size_t>(0, m_Examples.size() - 1);
}

const Example& ExampleSet::Sample() const
{
	return m_Examples[m_Distribution(GetRandomGenerator())];
}

void ImportVector(VectorRef outputVector, const rapidjson::Value& jsonVector, size_t expectedSize)
{
	if (jsonVector.GetType() != rapidjson::kArrayType) throw ApophisException("Vector specified as non-array type");
	if (jsonVector.GetArray().Size() != expectedSize) throw ApophisException("Vector has wrong number of elements, expected %d but contained %d", (int)expectedSize, (int)jsonVector.GetArray().Size());

	outputVector.resize(jsonVector.Size());
	for (auto i = 0u; i < jsonVector.Size(); i++)
		outputVector[i] = (real)jsonVector[i].GetDouble();
}

void ExampleSet::Import(const std::string& data)
{
	m_Examples.clear();

	rapidjson::Document json;
	if (json.Parse(data.c_str()).HasParseError()) throw ApophisException("Failed to parse JSON");

	if (!json.HasMember("input_size")) throw ApophisException("\"input_size\" not specified");
	InputSize = json["input_size"].GetInt();
	if (!json.HasMember("output_size")) throw ApophisException("\"output_size\" not specified");
	OutputSize = json["output_size"].GetInt();

	if (!json.HasMember("examples")) throw ApophisException("\"examples\" not specified");
	auto& examples = json["examples"];
	if (examples.GetType() != rapidjson::kArrayType) throw ApophisException("\"examples\" is not an array type");
	for (auto i = 0u; i < examples.Size(); i++)
	{
		Example example;
		const auto& jExample = examples[i];
		if (jExample.GetType() != rapidjson::kObjectType) throw ApophisException("Non-object type specified in \"examples\" array");
		if (!jExample.HasMember("input")) throw ApophisException("\"input\" not specified for example");
		if (!jExample.HasMember("output")) throw ApophisException("\"output\" not specified for example");
		ImportVector(example.Input, jExample["input"], InputSize);
		ImportVector(example.Output, jExample["output"], OutputSize);
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

	auto input = outputExample->SetArray("input", example.Input.size());
	ExportVector(*input, example.Input);

	auto output = outputExample->SetArray("output", example.Output.size());
	ExportVector(*output, example.Output);
}

void ExampleSet::Export(IExportWriter& output)
{
	output.Set("input_size", InputSize);
	output.Set("output_size", OutputSize);

	auto examples = output.SetArray("examples", m_Examples.size());
	for (const auto& example : m_Examples)
		ExportExample(*examples, example);
}

}