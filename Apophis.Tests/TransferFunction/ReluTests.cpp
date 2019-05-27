#include "stdafx.h"
#include "apophis/TransferFunction/Relu.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ApophisTests { namespace TransferFunction
{
	TEST_CLASS(ReluTests)
	{
	public:

		TEST_METHOD(Construct)
		{
			auto function = ::Apophis::TransferFunction::Relu();
			Assert::AreEqual("relu", function.GetName());
		}

		TEST_METHOD(Transfer_Positive_ZeroLeak)
		{
			auto function = ::Apophis::TransferFunction::Relu(0.f);

			Assert::AreEqual(1.f, function(1.f));
			Assert::AreEqual(2.f, function(2.f));
			Assert::AreEqual(3.4f, function(3.4f));
		}

		TEST_METHOD(Transfer_Negative_ZeroLeak)
		{
			auto function = ::Apophis::TransferFunction::Relu(0.f);

			Assert::AreEqual(0.f, function(-1.f));
			Assert::AreEqual(0.f, function(-2.f));
			Assert::AreEqual(0.f, function(-3.4f));
		}

		TEST_METHOD(Transfer_Positive_Leaky)
		{
			auto function = ::Apophis::TransferFunction::Relu();

			Assert::AreEqual(1.f, function(1.f));
			Assert::AreEqual(2.f, function(2.f));
			Assert::AreEqual(3.4f, function(3.4f));
		}

		TEST_METHOD(Transfer_Negative_Leaky_001)
		{
			auto function = ::Apophis::TransferFunction::Relu();

			Assert::AreEqual(-0.01f, function(-1.f));
			Assert::AreEqual(-0.02f, function(-2.f));
			Assert::AreEqual(-0.034f, function(-3.4f));
		}

		TEST_METHOD(Transfer_Negative_Leaky_01)
		{
			auto function = ::Apophis::TransferFunction::Relu(0.1f);

			Assert::AreEqual(-0.1f, function(-1.f));
			Assert::AreEqual(-0.2f, function(-2.f));
			Assert::AreEqual(-0.34f, function(-3.4f));
		}

		TEST_METHOD(Derivative_NonLeaky)
		{
			auto function = ::Apophis::TransferFunction::Relu(0.0f);

			Assert::AreEqual(0.f, function.Derivative(-3.f));
			Assert::AreEqual(0.f, function.Derivative(-1.5f));
			Assert::AreEqual(1.f, function.Derivative(2.5f));
			Assert::AreEqual(1.f, function.Derivative(4.f));
		}

		TEST_METHOD(Derivative_Leaky)
		{
			auto function = ::Apophis::TransferFunction::Relu();

			Assert::AreEqual(0.01f, function.Derivative(-3.f));
			Assert::AreEqual(0.01f, function.Derivative(-1.5f));
			Assert::AreEqual(1.f, function.Derivative(2.5f));
			Assert::AreEqual(1.f, function.Derivative(4.f));
		}
	};
}}