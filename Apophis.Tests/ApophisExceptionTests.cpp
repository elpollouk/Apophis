#include "stdafx.h"
#include "apophis/ApophisException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Apophis;

namespace ApophisTests {

	TEST_CLASS(ApophisExceptionTests)
	{
	public:

		TEST_METHOD(ConstructAndThrow)
		{
			try
			{
				throw ApophisException("Testing %d, %s, %.2f", 1, "two", 3.33333);
			}
			catch (std::exception& ex)
			{
				Assert::AreEqual("Testing 1, two, 3.33", ex.what());
			}
		}

		TEST_METHOD(CopyConstruct)
		{
			auto e1 = ApophisException("Test");
			auto e2 = e1;

			Assert::AreEqual("Test", e1.what());
			Assert::AreEqual(e1.what(), e2.what());
		}

		TEST_METHOD(Move)
		{
			auto e1 = ApophisException("Foo");
			auto e2 = std::move(e1);

			Assert::AreEqual("", e1.what());
			Assert::AreEqual("Foo", e2.what());
		}

		TEST_METHOD(Assign)
		{
			auto e1 = ApophisException("%cFoo%c", '<', '>');
			auto e2 = ApophisException("Bar");
			Assert::AreEqual("<Foo>", e1.what());
			Assert::AreEqual("Bar", e2.what());

			e2 = e1;
			Assert::AreEqual("<Foo>", e1.what());
			Assert::AreEqual("<Foo>", e2.what());
		}
	};

}
