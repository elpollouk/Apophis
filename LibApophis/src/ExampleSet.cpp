#include "stdafx.h"
#include "apophis/ExampleSet.h"
#include "Random.h"

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

bool ImportVector(VectorRef outputVector, const rapidjson::Value& jsonVector)
{
	if (jsonVector.GetType() != rapidjson::kArrayType) return false;

	outputVector.resize(jsonVector.Size());
	for (auto i = 0; i < jsonVector.Size(); i++)
		outputVector[i] = (real)jsonVector[i].GetDouble();

	return true;
}

bool ExampleSet::Import(const std::string& data)
{
	rapidjson::Document json;
	if (json.Parse(data.c_str()).HasParseError()) return false;

	if (!json.HasMember("input_size")) return false;
	InputSize = json["input_size"].GetInt();
	if (!json.HasMember("output_size")) return false;
	OutputSize = json["output_size"].GetInt();

	if (!json.HasMember("examples")) return false;
	auto& examples = json["examples"];
	if (examples.GetType() != rapidjson::kArrayType) return false;
	for (auto i = 0; i < examples.Size(); i++)
	{
		Example example;
		if (!ImportVector(example.Input, examples[i]["input"])) return false;
		if (!ImportVector(example.Output, examples[i]["output"])) return false;
		AddExample(std::move(example));
	}

	return true;
}

void ExportVector(rapidjson::Value& outputArray, ConstVectorRef vector, rapidjson::MemoryPoolAllocator<>& allocator)
{
	outputArray.Reserve(vector.size(), allocator);
	for (auto value : vector)
		outputArray.PushBack(value, allocator);
}

void ExportExample(rapidjson::Value& outputArray, const Example& example, rapidjson::MemoryPoolAllocator<>& allocator)
{
	rapidjson::Value jsonExample(rapidjson::kObjectType);

	rapidjson::Value input(rapidjson::kArrayType);
	ExportVector(input, example.Input, allocator);
	jsonExample.AddMember("input", input, allocator);

	rapidjson::Value output(rapidjson::kArrayType);
	ExportVector(output, example.Output, allocator);
	jsonExample.AddMember("output", output, allocator);

	outputArray.PushBack(jsonExample, allocator);
}

std::string ExampleSet::Export()
{
	rapidjson::Document json;
	auto& allocator = json.GetAllocator();
	json.SetObject();

	json.AddMember("input_size", InputSize, allocator);
	json.AddMember("output_size", OutputSize, allocator);

	rapidjson::Value examples(rapidjson::kArrayType);
	for (const auto& example : m_Examples)
		ExportExample(examples, example, allocator);

	json.AddMember("examples", examples, allocator);


	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	json.Accept(writer);

	return std::string(buffer.GetString(), buffer.GetSize());
}

}