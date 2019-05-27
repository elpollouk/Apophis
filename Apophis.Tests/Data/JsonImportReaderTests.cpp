#include "stdafx.h"
#include "Utils/JsonImportReader.h"
#include "apophis/ApophisException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Apophis;
using namespace Apophis::Utils;

namespace ApophisTests { namespace Utils {

	TEST_CLASS(JsonImportReaderTests)
	{
	public:
		TEST_METHOD(ParseFailure)
		{
			AssertThrows<ApophisException>([]() {
				JsonImportReader("BAD");
			});
		}

		TEST_METHOD(HasMember)
		{
			auto reader = JsonImportReader("{\"test\":\"foo\"}");
			Assert::IsTrue(reader.HasMember("test"));
			Assert::IsFalse(reader.HasMember("bar"));
		}

		TEST_METHOD(GetInt32)
		{
			auto reader = JsonImportReader("{\"foo\":123}");
			Assert::AreEqual(123, reader.GetInt32("foo"));
		}

		TEST_METHOD(GetReal)
		{
			auto reader = JsonImportReader("{\"foo\":56.0}");
			Assert::AreEqual(56.f, reader.GetReal("foo"));
		}

		TEST_METHOD(GetString)
		{
			auto reader = JsonImportReader("{\"foo\":\"bar\"}");
			Assert::AreEqual("bar", reader.GetString("foo"));
		}

		TEST_METHOD(GetArray)
		{
			auto reader = JsonImportReader("{\"foo\":[5,7,11]}");
			auto arr = reader.GetArray("foo");
			Assert::IsNotNull(arr.get());
			Assert::AreEqual(3, (int)arr->Size());
			Assert::AreEqual(5, arr->GetInt32(0));
			Assert::AreEqual(7, arr->GetInt32(1));
			Assert::AreEqual(11, arr->GetInt32(2));
		}

		TEST_METHOD(GetObject)
		{
			auto reader = JsonImportReader("{\"foo\":{\"bar\":\"baz\"}}");
			auto obj = reader.GetObject("foo");
			Assert::IsNotNull(obj.get());
			Assert::IsTrue(obj->HasMember("bar"));
			Assert::AreEqual("baz", obj->GetString("bar"));
		}
	};

}}