#include "stdafx.h"
#include "apophis/Component/Node.h"
#include "apophis/TransferFunction/Relu.h"
#include "apophis/TransferFunction/Sigmoid.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Apophis;
using namespace Apophis::Component;

namespace ApophisTests { namespace Component
{
	TEST_CLASS(NodeTests)
	{
	public:
		const TransferFunction::Relu m_Relu;
		const TransferFunction::Sigmoid m_Sigmoid;

		TEST_METHOD(ReluNode_Construct)
		{
			auto node = Node(4, &m_Relu);

			Assert::AreEqual(4, (int)node.NumInputs);
			Assert::AreEqual(0.f, node.Activation);
			Assert::AreEqual(0.f, node.BackPropError);
			Assert::AreEqual(5, (int)node.Weights.size());
			Assert::AreEqual(5, (int)node.PreviousWeightChanges.size());
		}

		TEST_METHOD(ReluNode_Calculate_PositiveWeights)
		{
			auto node = Node(2, &m_Relu);
			node.Weights[0] = 3.f;
			node.Weights[1] = 5.f;
			node.Weights[2] = 7.f;

			Vector input = { 11.f, 13.f };

			Assert::AreEqual(105.f, node.Calculate(input));
		}

		TEST_METHOD(ReluNode_Calculate_NegativeWeights)
		{
			auto node = Node(2, &m_Relu);
			node.Weights[0] = -3.f;
			node.Weights[1] = -5.f;
			node.Weights[2] = 6.f;

			Vector input = { 1.f, 1.f };

			Assert::AreEqual(-0.02f, node.Calculate(input));
		}

		TEST_METHOD(SigmoidNode_Calculate_PositiveWeights)
		{
			auto node = Node(3, &m_Sigmoid);
			node.Weights[0] = .1f;
			node.Weights[1] = .2f;
			node.Weights[2] = .3f;
			node.Weights[3] = .4f;

			Vector input = { 1.f, 2.f, 3.f };

			AssertAreClose(0.858149f, node.Calculate(input));
		}

		TEST_METHOD(SigmoidNode_Calculate_NegativeWeights)
		{
			auto node = Node(3, &m_Sigmoid);
			node.Weights[0] = .1f;
			node.Weights[1] = -.2f;
			node.Weights[2] = -.3f;
			node.Weights[3] = -.4f;

			Vector input = { 1.f, 2.f, 3.f };

			AssertAreClose(0.167982f, node.Calculate(input));
		}
	};
}}
