#pragma once
#include "apophis/apophistypes.h"
#include <memory>

namespace Apophis { namespace Utils {

	class IExportWriter
	{
	public:
		static std::unique_ptr<IExportWriter> CreateJsonExportWriter();

	public:
		virtual ~IExportWriter() {}

		virtual void Set(const char* key, real value) = 0;
		virtual void Set(const char* key, long long value) = 0;
		virtual void Set(const char* key, const char* value) = 0;
		virtual std::unique_ptr<IExportWriter> SetArray(const char* key, size_t size) = 0;
		virtual std::unique_ptr<IExportWriter> SetObject(const char* key) = 0;

		virtual void PushBack(real value) = 0;
		virtual void PushBack(long long value) = 0;
		virtual void PushBack(const char* value) = 0;
		virtual std::unique_ptr<IExportWriter> PushBackArray(size_t size) = 0;
		virtual std::unique_ptr<IExportWriter> PushBackObject() = 0;

		virtual std::string GetData() = 0;
	};

}}