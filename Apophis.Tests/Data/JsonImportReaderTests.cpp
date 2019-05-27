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

		TEST_METHOD(GetInt64_MissingMember)
		{
			AssertThrows<ApophisException>([]() {
				auto reader = JsonImportReader("{}");
				reader.GetInt64("foo");
			});
		}

		TEST_METHOD(GetReal)
		{
			auto reader = JsonImportReader("{\"foo\":56.0}");
			Assert::AreEqual(56.f, reader.GetReal("foo"));
		}

		TEST_METHOD(GetReal_MissingMember)
		{
			AssertThrows<ApophisException>([]() {
				auto reader = JsonImportReader("{}");
				reader.GetReal("foo");
			});
		}

		TEST_METHOD(GetString)
		{
			auto reader = JsonImportReader("{\"foo\":\"bar\"}");
			Assert::AreEqual("bar", reader.GetString("foo"));
		}

		TEST_METHOD(GetString_MissingMember)
		{
			AssertThrows<ApophisException>([]() {
				auto reader = JsonImportReader("{}");
				reader.GetString("foo");
			});
		}

		TEST_METHOD(GetArray)
		{
			auto reader = JsonImportReader("{\"foo\":[5,7.0,\"11\"]}");
			auto arr = reader.GetArray("foo");
			Assert::IsNotNull(arr.get());
			Assert::AreEqual(3, (int)arr->Size());
			Assert::AreEqual(5, arr->GetInt32(0));
			Assert::AreEqual(7.f, arr->GetReal(1));
			Assert::AreEqual("11", arr->GetString(2));
		}

		TEST_METHOD(GetArray_MissingMember)
		{
			AssertThrows<ApophisException>([]() {
				auto reader = JsonImportReader("{}");
				reader.GetArray("foo");
			});
		}

		TEST_METHOD(GetArray_WrongType)
		{
			AssertThrows<ApophisException>([]() {
				auto reader = JsonImportReader("{\"foo\":5}");
				reader.GetArray("foo");
			});
		}

		TEST_METHOD(GetArray_InvalidIndex_Over)
		{
			AssertThrows<ApophisException>([]() {
				auto reader = JsonImportReader("{\"foo\":[2]}");
				auto arr = reader.GetArray("foo");
				arr->GetInt64(2);
			});
		}

		TEST_METHOD(GetArray_InvalidIndex_Under)
		{
			AssertThrows<ApophisException>([]() {
				auto reader = JsonImportReader("{\"foo\":[2]}");
				auto arr = reader.GetArray("foo");
				arr->GetInt64(-1);
			});
		}

		TEST_METHOD(GetObject)
		{
			auto reader = JsonImportReader("{\"foo\":{\"bar\":\"baz\"}}");
			auto obj = reader.GetObject("foo");
			Assert::IsNotNull(obj.get());
			Assert::IsTrue(obj->HasMember("bar"));
			Assert::AreEqual("baz", obj->GetString("bar"));
		}

		TEST_METHOD(GetObject_MissingMember)
		{
			AssertThrows<ApophisException>([]() {
				auto reader = JsonImportReader("{}");
				reader.GetObject("foo");
			});
		}

		TEST_METHOD(GetObject_WrongType)
		{
			AssertThrows<ApophisException>([]() {
				auto reader = JsonImportReader("{\"foo\":[]}");
				reader.GetObject("foo");
			});
		}

		TEST_METHOD(GetObjectFromArray)
		{
			auto reader = JsonImportReader("{\"foo\":[{\"bar\":\"baz\"}]}");
			auto obj = reader.GetArray("foo")->GetObject(0);
			Assert::AreEqual("baz", obj->GetString("bar"));
		}

		TEST_METHOD(GetObjectFromArray_WrongType)
		{
			AssertThrows<ApophisException>([]() {
				auto reader = JsonImportReader("{\"foo\":[0]}");
				auto arr = reader.GetArray("foo");
				arr->GetObject(0);
			});
		}

		TEST_METHOD(GetArrayFromArray)
		{
			auto reader = JsonImportReader("{\"foo\":[[\"bar\"]]}");
			auto arr = reader.GetArray("foo")->GetArray(0);
			Assert::AreEqual("bar", arr->GetString(0));
		}

		TEST_METHOD(GetArrayFromArray_WrongType)
		{
			AssertThrows<ApophisException>([]() {
				auto reader = JsonImportReader("{\"foo\":[0]}");
				auto arr = reader.GetArray("foo");
				arr->GetArray(0);
			});
		}
	};

}}