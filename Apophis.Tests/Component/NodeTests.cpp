#include "stdafx.h"
#include "apophis/Component/Node.h"
#include "apophis/TransferFunction/Relu.h"
#include "apophis/TransferFunction/Sigmoid.h"
#include "Utils/JsonExportWriter.h"
#include "apophis/Utils/IImportReader.h"
#include "apophis/ApophisException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Apophis;
using namespace Apophis::Component;

namespace ApophisTests { namespace Component
{
	TEST_CLASS(NodeTests)
	{
	public:
		TEST_METHOD(Construct)
		{
			auto node = Node(4);

			Assert::AreEqual(4, (int)node.GetNumInputs());
			Assert::AreEqual(0.f, node.Activation);
			Assert::AreEqual(5, (int)node.Weights.size());
		}

		TEST_METHOD(Calculate)
		{
			auto node = Node(2);
			node.Weights[0] = 3.f;
			node.Weights[1] = 5.f;
			node.Weights[2] = 7.f;

			Vector input = { 11.f, 13.f };

			Assert::AreEqual(105.f, node.Calculate(input));
			Assert::AreEqual(105.f, node.Activation);
		}

		TEST_METHOD(Export)
		{
			auto outputObject = Utils::JsonExportWriter();

			auto node = Node(2);
			node.Weights[0] = 3.f;
			node.Weights[1] = 5.f;
			node.Weights[2] = 7.f;

			node.Export(outputObject);

			Assert::AreEqual("node", outputObject.GetValue()["type"].GetString());
			Assert::IsTrue(outputObject.GetValue().HasMember("weights"));
			auto& weights = outputObject.GetValue()["weights"].GetArray();
			Assert::AreEqual(3, (int)weights.Size());
			Assert::AreEqual(3.f, weights[0].GetFloat());
			Assert::AreEqual(5.f, weights[1].GetFloat());
			Assert::AreEqual(7.f, weights[2].GetFloat());
		}

		TEST_METHOD(Import)
		{
			auto reader = Utils::IImportReader::CreateJsonImportReader("{\"weights\":[7,11,13,17]}");
			auto node = Node(3);

			node.Import(*reader);

			Assert::AreEqual(7.f, node.Weights[0]);
			Assert::AreEqual(11.f, node.Weights[1]);
			Assert::AreEqual(13.f, node.Weights[2]);
			Assert::AreEqual(17.f, node.Weights[3]);
		}

		TEST_METHOD(Import_WrongNumberOfWeights)
		{
			AssertThrows<ApophisException>([]() {
				auto reader = Utils::IImportReader::CreateJsonImportReader("{\"weights\":[7,11,13,17]}");
				auto node = Node(4);
				node.Import(*reader);
			});

		}
	};
}}
