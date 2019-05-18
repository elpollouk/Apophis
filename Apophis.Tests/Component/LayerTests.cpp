#include "stdafx.h"
#include "apophis/Component/Layer.h"
#include "apophis/TransferFunction/Relu.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Apophis;
using namespace Apophis::Component;
using namespace Apophis::TransferFunction;

namespace ApophisTests { namespace Component
{
	TEST_CLASS(LayerTests)
	{
	public:
		TEST_METHOD(Layer_Construct)
		{
			auto layer = Layer(3, 2, Relu::Default());

			Assert::AreEqual(2, (int)layer.NumOutputs);
			Assert::AreEqual(2, (int)layer.Nodes.size());
			Assert::AreEqual(2, (int)layer.Output.size());

			Assert::AreEqual(3, (int)layer.NumInputs);
			Assert::AreEqual(4, (int)layer.Nodes[0]->Weights.size());
			Assert::AreEqual(4, (int)layer.Nodes[1]->Weights.size());
		}

		TEST_METHOD(Layer_Calculate)
		{
			auto layer = Layer(3, 2, Relu::Default());

			auto value = 1.f;
			for (auto& node : layer.Nodes)
			{
				for (auto& weight : node->Weights)
				{
					weight = value;
					value += 1;
				}
			}

			Vector input = { 0.3f, 0.5f, 0.7f };
			auto& output = layer.Calculate(input);

			Assert::AreEqual(2, (int)output.size());
			Assert::AreEqual((const void*)&layer.Output, (const void*)&output);
			AssertAreClose(7.4f, output[0]);
			AssertAreClose(17.4f, output[1]);
		}
	};
}}
