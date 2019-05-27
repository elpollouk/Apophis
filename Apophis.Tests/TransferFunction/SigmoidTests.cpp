#include "stdafx.h"
#include "apophis/TransferFunction/Sigmoid.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ApophisTests {
	namespace TransferFunction
	{
		TEST_CLASS(SigmoidTests)
		{
		public:

			TEST_METHOD(Construct)
			{
				auto function = ::Apophis::TransferFunction::Sigmoid();
				Assert::AreEqual("sigmoid", function.GetName());
			}

			TEST_METHOD(Transfer)
			{
				auto function = ::Apophis::TransferFunction::Sigmoid();

				AssertAreClose(0.075858f, function(-2.5f));
				AssertAreClose(0.268941f, function(-1.f));
				AssertAreClose(0.5f, function(0.f));
				AssertAreClose(0.731059f, function(1.f));
				AssertAreClose(0.924142f, function(2.5f));
			}

			TEST_METHOD(Derivative)
			{
				auto function = ::Apophis::TransferFunction::Sigmoid();

				AssertAreClose(0.070104f, function.Derivative(-2.5f));
				AssertAreClose(0.196612f, function.Derivative(-1.f));
				AssertAreClose(0.25f, function.Derivative(0.f));
				AssertAreClose(0.196612f, function.Derivative(1.f));
				AssertAreClose(0.070104f, function.Derivative(2.5f));
			}
		};
	}
}