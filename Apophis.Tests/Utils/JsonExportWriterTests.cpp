#include "stdafx.h"
#include "Utils/JsonExportWriter.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Apophis::Utils;

namespace ApophisTests { namespace Utils {

	TEST_CLASS(JsonExportWriterTests)
	{
	public:
		TEST_METHOD(Set_Real)
		{
			JsonExportWriter writer;
			writer.Set("foo", 123.45f);
			Assert::AreEqual(123.45f, writer.GetValue()["foo"].GetFloat());
		}

		TEST_METHOD(Set_Integer)
		{
			JsonExportWriter writer;
			writer.Set("bar", 321ll);
			Assert::AreEqual(321, writer.GetValue()["bar"].GetInt());
		}

		TEST_METHOD(Set_String)
		{
			JsonExportWriter writer;
			writer.Set("test", "abc");
			Assert::AreEqual("abc", writer.GetValue()["test"].GetString());
		}

		TEST_METHOD(Set_Array)
		{
			JsonExportWriter writer;
			writer.SetArray("a", 4);
			Assert::AreEqual((int)rapidjson::kArrayType, (int)writer.GetValue()["a"].GetType());
		}

		TEST_METHOD(Set_Object)
		{
			JsonExportWriter writer;
			writer.SetObject("o");
			Assert::AreEqual((int)rapidjson::kObjectType, (int)writer.GetValue()["o"].GetType());
		}

		TEST_METHOD(PushBack_Real)
		{
			JsonExportWriter writer;
			auto a = writer.SetArray("foo", 4);
			a->PushBack(4.f);
			a->PushBack(3.f);
			a->PushBack(2.f);
			a->PushBack(1.f);
			Assert::AreEqual(4, (int)writer.GetValue()["foo"].GetArray().Size());
			Assert::AreEqual(4.f, writer.GetValue()["foo"].GetArray()[0].GetFloat());
			Assert::AreEqual(3.f, writer.GetValue()["foo"].GetArray()[1].GetFloat());
			Assert::AreEqual(2.f, writer.GetValue()["foo"].GetArray()[2].GetFloat());
			Assert::AreEqual(1.f, writer.GetValue()["foo"].GetArray()[3].GetFloat());
		}

		TEST_METHOD(PushBack_Integer)
		{
			JsonExportWriter writer;
			auto a = writer.SetArray("foo", 2);
			a->PushBack(-3ll);
			a->PushBack(7ll);
			Assert::AreEqual(2, (int)writer.GetValue()["foo"].GetArray().Size());
			Assert::AreEqual(-3, writer.GetValue()["foo"].GetArray()[0].GetInt());
			Assert::AreEqual(7, writer.GetValue()["foo"].GetArray()[1].GetInt());
		}

		TEST_METHOD(PushBack_String)
		{
			JsonExportWriter writer;
			auto a = writer.SetArray("foo", 3);
			a->PushBack("a");
			a->PushBack("b");
			a->PushBack("c");
			Assert::AreEqual(3, (int)writer.GetValue()["foo"].GetArray().Size());
			Assert::AreEqual("a", writer.GetValue()["foo"].GetArray()[0].GetString());
			Assert::AreEqual("b", writer.GetValue()["foo"].GetArray()[1].GetString());
			Assert::AreEqual("c", writer.GetValue()["foo"].GetArray()[2].GetString());
		}

		TEST_METHOD(PushBack_Array)
		{
			JsonExportWriter writer;
			auto a = writer.SetArray("foo", 2);
			a->PushBackArray(3);
			Assert::AreEqual(1, (int)writer.GetValue()["foo"].GetArray().Size());
			Assert::AreEqual((int)rapidjson::kArrayType, (int)writer.GetValue()["foo"].GetArray()[0].GetType());
		}

		TEST_METHOD(PushBack_Object)
		{
			JsonExportWriter writer;
			auto a = writer.SetArray("foo", 2);
			a->PushBackObject();
			Assert::AreEqual(1, (int)writer.GetValue()["foo"].GetArray().Size());
			Assert::AreEqual((int)rapidjson::kObjectType, (int)writer.GetValue()["foo"].GetArray()[0].GetType());
		}

		TEST_METHOD(GetData)
		{
			JsonExportWriter writer;
			auto o = writer.SetObject("foo");
			o->Set("a", 12.f);
			o->Set("b", "three");
			o->Set("c", 4ll);
			auto a = writer.SetArray("d", 1);
			o = a->PushBackObject();
			o->Set("e", "test");

			Assert::AreEqual("{\"foo\":{\"a\":12.0,\"b\":\"three\",\"c\":4},\"d\":[{\"e\":\"test\"}]}", writer.GetData().c_str());
		}
	};

}}
