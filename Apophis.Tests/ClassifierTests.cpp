#include "stdafx.h"
#include "apophis/Component/Network.h"
#include "apophis/Classifier.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Apophis;

namespace ApophisTests {
	
	class MockNetwork : public Component::Network
	{
	public:
		MockNetwork() : Component::Network(0) {}

		virtual ConstVectorRef Calculate(ConstVectorRef input) override
		{
			LastInput = input;
			return Output;
		}

		Vector Output;
		Vector LastInput;
	};
	TEST_CLASS(ClassifierTests)
	{
	public:
		TEST_METHOD(Integer_OneAboveThreshold)
		{
			MockNetwork network;
			network.Output = { 0.1f, 0.8f, -0.1f };
			auto classify = Classifier<int>(network, { 5, 7, 11 }, 0.5f, -1);
			auto classification = classify({ 13.f, 17.f });

			Assert::AreEqual(7, classification, L"Incorrect classification");
			Assert::AreEqual(2, (int)network.LastInput.size(), L"Wrong sized input passed to network");
			Assert::AreEqual(13.f, network.LastInput[0], L"First network input was wrong");
			Assert::AreEqual(17.f, network.LastInput[1], L"First network input was wrong");
		}

		TEST_METHOD(StdString_TwoAboveThreshold)
		{
			MockNetwork network;
			network.Output = { 0.1f, 0.8f, 0.9f };
			auto classify = Classifier<std::string>(network, { "A", "B", "C" }, 0.5f, "D");
			auto classification = classify({ 13.f, 17.f });

			Assert::AreEqual("C", classification.c_str(), L"Incorrect classification");
		}

		TEST_METHOD(String_NoneAboveThreshold)
		{
			MockNetwork network;
			network.Output = { 0.1f, 0.8f, 0.9f };
			auto classify = Classifier<const char*>(network, { "A", "B", "C" }, 0.99f, "D");
			auto classification = classify({ 13.f, 17.f });

			Assert::AreEqual("D", classification, L"Incorrect classification");
		}
	};
}
