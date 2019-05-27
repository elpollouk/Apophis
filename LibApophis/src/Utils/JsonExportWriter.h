#pragma once

#include "apophis/Utils/IExportWriter.h"

namespace Apophis { namespace Utils {

	class JsonExportWriterImpl;

	class JsonExportWriter : public IExportWriter
	{
	public:
		JsonExportWriter();
		JsonExportWriter(rapidjson::Value* value, std::shared_ptr<rapidjson::MemoryPoolAllocator<>> allocator);
		virtual ~JsonExportWriter();

		virtual void Set(const char* key, real value) override;
		virtual void Set(const char* key, long long value) override;
		virtual void Set(const char* key, const char* value) override;
		virtual std::unique_ptr<IExportWriter> SetArray(const char* key, size_t size) override;
		virtual std::unique_ptr<IExportWriter> SetObject(const char* key) override;

		virtual void PushBack(real value) override;
		virtual void PushBack(long long value) override;
		virtual void PushBack(const char* value) override;
		virtual std::unique_ptr<IExportWriter> PushBackArray(size_t size) override;
		virtual std::unique_ptr<IExportWriter> PushBackObject() override;

		virtual std::string GetData() override;

		const rapidjson::Value& GetValue() const { return *m_pValue; }

	private:
		bool m_OwnsValue;
		rapidjson::Value* m_pValue;
		std::shared_ptr<rapidjson::MemoryPoolAllocator<>> m_pAllocator;
	};

}}
