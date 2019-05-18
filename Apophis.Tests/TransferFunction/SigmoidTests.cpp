#include "stdafx.h"
#include "apophis/TransferFunction/Sigmoid.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ApophisTests {
	namespace TransferFunction
	{
		TEST_CLASS(SigmoidTests)
		{
		public:

			TEST_METHOD(Transfer_float)
			{
				auto function = ::Apophis::TransferFunction::Sigmoid<float>();

				AssertAreClose(0.075858f, function(-2.5f));
				AssertAreClose(0.268941f, function(-1.f));
				AssertAreClose(0.5f, function(0.f));
				AssertAreClose(0.731059f, function(1.f));
				AssertAreClose(0.924142f, function(2.5f));
			}

			TEST_METHOD(Derivative_float)
			{
				auto function = ::Apophis::TransferFunction::Sigmoid<float>();

				AssertAreClose(0.070104f, function.Derivative(-2.5f));
				AssertAreClose(0.196612f, function.Derivative(-1.f));
				AssertAreClose(0.25f, function.Derivative(0.f));
				AssertAreClose(0.196612f, function.Derivative(1.f));
				AssertAreClose(0.070104f, function.Derivative(2.5f));
			}

			TEST_METHOD(Transfer_double)
			{
				auto function = ::Apophis::TransferFunction::Sigmoid<double>();

				AssertAreClose(0.075858, function(-2.5));
				AssertAreClose(0.268941, function(-1.));
				AssertAreClose(0.5, function(0.));
				AssertAreClose(0.731059, function(1.));
				AssertAreClose(0.924142, function(2.5));
			}

			TEST_METHOD(Derivative_double)
			{
				auto function = ::Apophis::TransferFunction::Sigmoid<double>();

				AssertAreClose(0.070104, function.Derivative(-2.5));
				AssertAreClose(0.196612, function.Derivative(-1.));
				AssertAreClose(0.25, function.Derivative(0.));
				AssertAreClose(0.196612, function.Derivative(1.));
				AssertAreClose(0.070104, function.Derivative(2.5));
			}
		};
	}
}