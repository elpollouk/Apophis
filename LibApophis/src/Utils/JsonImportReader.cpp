#include "stdafx.h"
#include "JsonImportReader.h"

using namespace Apophis;
using namespace Apophis::Utils;

std::unique_ptr<IImportReader> IImportReader::CreateJsonImportReader(std::string json)
{
	return std::make_unique<JsonImportReader>(json.c_str());
}

JsonImportReader::JsonImportReader(std::string json) :
	m_Owner(true)
{
	auto pDoc = new rapidjson::Document();
	if (pDoc->Parse(json.c_str()).HasParseError())
	{
		delete pDoc;
		throw ApophisException("Failed to parse JSON");
	}
	m_pValue = pDoc;
}

JsonImportReader::JsonImportReader(const rapidjson::Value* value) :
	m_Owner(false),
	m_pValue(value)
{

}

JsonImportReader::~JsonImportReader()
{
	if (m_Owner)
		delete m_pValue;
}

const rapidjson::Value& JsonImportReader::EnsureMember(const char* key)
{
	if (!HasMember(key)) throw ApophisException("JSON object does not have member \"%s\"", key);
	return (*m_pValue)[key];
}

const rapidjson::Value& JsonImportReader::EnsureMember(const char* key, rapidjson::Type type)
{
	auto& v = EnsureMember(key);
	if (v.GetType() != type) throw ApophisException("JSON object member \"%s\" has wrong type", key);
	return v;
}

const rapidjson::Value& JsonImportReader::EnsureIndex(size_t index)
{
	assert(m_pValue->GetType() == rapidjson::kArrayType);
	if (index < 0 || m_pValue->Size() <= index) throw ApophisException("JSON array index %d is out of bounds", index);
	return m_pValue->GetArray()[(rapidjson::SizeType)index];
}

const rapidjson::Value& JsonImportReader::EnsureIndex(size_t index, rapidjson::Type type)
{
	auto& v = EnsureIndex(index);
	if (v.GetType() != type) throw ApophisException("JSON array element %d has wrong type", index);
	return v;
}

bool JsonImportReader::HasMember(const char* key)
{
	assert(m_pValue->GetType() == rapidjson::kObjectType);
	return m_pValue->HasMember(key);
}

long long JsonImportReader::GetInt64(const char* key)
{
	return EnsureMember(key).GetInt64();
}

real JsonImportReader::GetReal(const char* key)
{
	return EnsureMember(key).GetFloat();
}

const char* JsonImportReader::GetString(const char* key)
{
	return EnsureMember(key).GetString();
}

std::unique_ptr<IImportReader> JsonImportReader::GetArray(const char* key)
{
	auto& value = EnsureMember(key, rapidjson::kArrayType);
	return std::make_unique<JsonImportReader>(&value);
}

std::unique_ptr<IImportReader> JsonImportReader::GetObject(const char* key)
{
	auto& value = EnsureMember(key, rapidjson::kObjectType);
	return std::make_unique<JsonImportReader>(&value);
}

size_t JsonImportReader::Size()
{
	assert(m_pValue->GetType() == rapidjson::kArrayType);
	return m_pValue->Size();
}

long long JsonImportReader::GetInt64(size_t index)
{
	return EnsureIndex(index).GetInt64();
}

real JsonImportReader::GetReal(size_t index)
{
	return EnsureIndex(index).GetFloat();
}

const char* JsonImportReader::GetString(size_t index)
{
	return EnsureIndex(index).GetString();
}

std::unique_ptr<IImportReader> JsonImportReader::GetObject(size_t index)
{
	auto& value = EnsureIndex(index, rapidjson::kObjectType);
	return std::make_unique<JsonImportReader>(&value);
}

std::unique_ptr<IImportReader> JsonImportReader::GetArray(size_t index)
{
	auto& value = EnsureIndex(index, rapidjson::kArrayType);
	return std::make_unique<JsonImportReader>(&value);
}
