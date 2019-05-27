#include "stdafx.h"
#include "JsonExportWriter.h"

using namespace Apophis;
using namespace Apophis::Utils;

#define GSR(s) rapidjson::GenericStringRef<rapidjson::UTF8<>::Ch>(s)

std::unique_ptr<IExportWriter> IExportWriter::CreateJsonExportWriter()
{
	return std::make_unique<JsonExportWriter>();
}

JsonExportWriter::JsonExportWriter() :
	m_OwnsValue(true),
	m_pValue(new rapidjson::Value(rapidjson::kObjectType)),
	m_pAllocator(std::make_shared<rapidjson::MemoryPoolAllocator<>>())
{

}

JsonExportWriter::JsonExportWriter(rapidjson::Value* value, std::shared_ptr<rapidjson::MemoryPoolAllocator<>> allocator) :
	m_OwnsValue(false),
	m_pValue(value),
	m_pAllocator(allocator)
{

}

JsonExportWriter::~JsonExportWriter()
{
	if (m_OwnsValue) delete m_pValue;
}

void JsonExportWriter::Set(const char* key, real value)
{
	assert(m_pValue->GetType() == rapidjson::kObjectType);
	m_pValue->AddMember(GSR(key), value, *m_pAllocator);
}

void JsonExportWriter::Set(const char* key, long long value)
{
	assert(m_pValue->GetType() == rapidjson::kObjectType);
	m_pValue->AddMember(GSR(key), value, *m_pAllocator);
}

void JsonExportWriter::Set(const char* key, const char* value)
{
	assert(m_pValue->GetType() == rapidjson::kObjectType);
	rapidjson::Value s(rapidjson::kStringType);
	s.SetString(value, *m_pAllocator);
	m_pValue->AddMember(GSR(key), s, *m_pAllocator);
}

std::unique_ptr<IExportWriter> JsonExportWriter::SetArray(const char* key, size_t size)
{
	assert(m_pValue->GetType() == rapidjson::kObjectType);
	rapidjson::Value a(rapidjson::kArrayType);
	a.Reserve((rapidjson::SizeType)size, *m_pAllocator);
	auto pValue = &(m_pValue->AddMember(GSR(key), a, *m_pAllocator).GetObject()[key]);
	return std::make_unique<JsonExportWriter>(pValue, m_pAllocator);
}

std::unique_ptr<IExportWriter> JsonExportWriter::SetObject(const char* key)
{
	assert(m_pValue->GetType() == rapidjson::kObjectType);
	rapidjson::Value o(rapidjson::kObjectType);
	auto pValue = &(m_pValue->AddMember(GSR(key), o, *m_pAllocator).GetObject()[key]);
	return std::make_unique<JsonExportWriter>(pValue, m_pAllocator);
}

void JsonExportWriter::PushBack(real value)
{
	assert(m_pValue->GetType() == rapidjson::kArrayType);
	m_pValue->PushBack(value, *m_pAllocator);
}

void JsonExportWriter::PushBack(long long value)
{
	assert(m_pValue->GetType() == rapidjson::kArrayType);
	m_pValue->PushBack(value, *m_pAllocator);
}

void JsonExportWriter::PushBack(const char* value)
{
	assert(m_pValue->GetType() == rapidjson::kArrayType);
	rapidjson::Value s(rapidjson::kStringType);
	s.SetString(value, *m_pAllocator);
	m_pValue->PushBack(s, *m_pAllocator);
}

std::unique_ptr<IExportWriter> JsonExportWriter::PushBackArray(size_t size)
{
	assert(m_pValue->GetType() == rapidjson::kArrayType);
	rapidjson::Value a(rapidjson::kArrayType);
	a.Reserve((rapidjson::SizeType)size, *m_pAllocator);
	auto index = m_pValue->Size();
	auto pValue = &(m_pValue->PushBack(a, *m_pAllocator).GetArray()[index]);
	return std::make_unique<JsonExportWriter>(pValue, m_pAllocator);
}

std::unique_ptr<IExportWriter> JsonExportWriter::PushBackObject()
{
	assert(m_pValue->GetType() == rapidjson::kArrayType);
	rapidjson::Value o(rapidjson::kObjectType);
	auto index = m_pValue->Size();
	auto pValue = &(m_pValue->PushBack(o, *m_pAllocator).GetArray()[index]);
	return std::make_unique<JsonExportWriter>(pValue, m_pAllocator);
}

std::string JsonExportWriter::GetData()
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	m_pValue->Accept(writer);

	return std::string(buffer.GetString(), buffer.GetSize());
}