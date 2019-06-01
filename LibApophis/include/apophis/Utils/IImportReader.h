#pragma once

#include "apophis/apophistypes.h"
#include <memory>

namespace Apophis { namespace Utils {

	class IImportReader
	{
	public:
		static std::unique_ptr<IImportReader> CreateJsonImportReader(std::string json);

	public:
		virtual ~IImportReader() {}

		virtual bool HasMember(const char* key) = 0;
		virtual long long GetInt64(const char* key) = 0;
		virtual real GetReal(const char* key) = 0;
		virtual const char* GetString(const char* key) = 0;
		virtual std::unique_ptr<IImportReader> GetArray(const char* key) = 0;
		virtual std::unique_ptr<IImportReader> GetObject(const char* key) = 0;

		virtual size_t Size() = 0;
		virtual long long GetInt64(int index) = 0;
		virtual real GetReal(int index) = 0;
		virtual const char* GetString(int index) = 0;
		virtual std::unique_ptr<IImportReader> GetObject(int index) = 0;
		virtual std::unique_ptr<IImportReader> GetArray(int index) = 0;

		// Helpers for common types
		int GetInt32(const char* key) { return (int)GetInt64(key); }
		int GetInt32(int index) { return (int)GetInt64(index); }
		size_t GetSize_t(const char* key) { return (size_t)GetInt64(key); }
	};
}}