#include "stdafx.h"
#include "JsonImportReader.h"

using namespace Apophis;
using namespace Apophis::Utils;

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

bool JsonImportReader::HasMember(const char* key)
{
	assert(m_pValue->GetType() == rapidjson::kObjectType);
	return m_pValue->HasMember(key);
}

long long JsonImportReader::GetInt64(const char* key)
{
	assert(m_pValue->GetType() == rapidjson::kObjectType);
	return GetValue()[key].GetInt64();
}

real JsonImportReader::GetReal(const char* key)
{
	assert(m_pValue->GetType() == rapidjson::kObjectType);
	return GetValue()[key].GetFloat();
}

const char* JsonImportReader::GetString(const char* key)
{
	assert(m_pValue->GetType() == rapidjson::kObjectType);
	return GetValue()[key].GetString();
}

std::unique_ptr<IImportReader> JsonImportReader::GetArray(const char* key)
{
	assert(m_pValue->GetType() == rapidjson::kObjectType);
	assert(GetValue()[key].GetType() == rapidjson::kArrayType);
	const auto& value = GetValue()[key];
	return std::make_unique<JsonImportReader>(&value);
}

std::unique_ptr<IImportReader> JsonImportReader::GetObject(const char* key)
{
	assert(m_pValue->GetType() == rapidjson::kObjectType);
	assert(GetValue()[key].GetType() == rapidjson::kObjectType);
	const auto& value = GetValue()[key];
	return std::make_unique<JsonImportReader>(&value);
}

size_t JsonImportReader::Size()
{
	assert(m_pValue->GetType() == rapidjson::kArrayType);
	return m_pValue->Size();
}

long long JsonImportReader::GetInt64(int index)
{
	assert(m_pValue->GetType() == rapidjson::kArrayType);
	return m_pValue->GetArray()[index].GetInt64();
}