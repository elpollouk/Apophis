#pragma once

#include "apophis/Utils/IImportReader.h"

namespace Apophis { namespace Utils {

	class JsonImportReader : public IImportReader
	{
	public:
		JsonImportReader(std::string json);
		JsonImportReader(const rapidjson::Value* value);
		virtual ~JsonImportReader();

		virtual bool HasMember(const char* key) override;
		virtual long long GetInt64(const char* key) override;
		virtual real GetReal(const char* key) override;
		virtual const char* GetString(const char* key) override;
		virtual std::unique_ptr<IImportReader> GetArray(const char* key) override;
		virtual std::unique_ptr<IImportReader> GetObject(const char* key) override;


		virtual size_t Size() override;
		virtual long long GetInt64(size_t index) override;
		virtual real GetReal(size_t index) override;
		virtual const char* GetString(size_t index) override;
		virtual std::unique_ptr<IImportReader> GetObject(size_t index) override;
		virtual std::unique_ptr<IImportReader> GetArray(size_t index) override;

		const rapidjson::Value& GetValue() const { return *m_pValue; }

	private:
		const rapidjson::Value& EnsureMember(const char* key);
		const rapidjson::Value& EnsureMember(const char* key, rapidjson::Type type);
		const rapidjson::Value& EnsureIndex(size_t index);
		const rapidjson::Value& EnsureIndex(size_t index, rapidjson::Type type);

		bool m_Owner;
		const rapidjson::Value* m_pValue;
	};

}}