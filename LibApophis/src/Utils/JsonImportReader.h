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
		virtual long long GetInt64(int index) override;

		const rapidjson::Value& GetValue() const { return *m_pValue; }

	private:
		bool m_Owner;
		const rapidjson::Value* m_pValue;
	};

}}