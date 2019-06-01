#include "stdafx.h"
#include "apophis/Component/Layer.h"
#include "apophis/Component/Network.h"
#include "apophis/TransferFunction/Relu.h"
#include "apophis/TransferFunction/Sigmoid.h"
#include "Utils/JsonExportWriter.h"
#include "apophis/Utils/IImportReader.h"
#include "apophis/ApophisException.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Apophis;
using namespace Apophis::Component;
using namespace Apophis::TransferFunction;

namespace ApophisTests { namespace Component
{
	TEST_CLASS(LayerTests)
	{
	public:

		static void InitWeights(Layer& layer)
		{
			auto value = 1.f;
			for (auto& node : layer.Nodes)
			{
				for (auto& weight : node->Weights)
				{
					weight = value;
					value += 1;
				}
			}
		}

		TEST_METHOD(Construct)
		{
			Network network(0);
			auto layer = Layer(3, 2, Relu::Default(), network);

			Assert::AreEqual(2, (int)layer.GetNumOutputs());
			Assert::AreEqual(2, (int)layer.Nodes.size());
			Assert::AreEqual(2, (int)layer.GetOutput().size());

			Assert::AreEqual(3, (int)layer.GetNumInputs());
			Assert::AreEqual(4, (int)layer.Nodes[0]->Weights.size());
			Assert::AreEqual(4, (int)layer.Nodes[1]->Weights.size());
		}

		TEST_METHOD(Calculate)
		{
			Network network(0);
			auto layer = Layer(3, 2, Relu::Default(), network);
			InitWeights(layer);

			Vector input = { 0.3f, 0.5f, 0.7f };
			auto& output = layer.Calculate(input);

			Assert::AreEqual(2, (int)output.size());
			Assert::AreEqual((const void*)&layer.GetOutput(), (const void*)&output);
			AssertAreClose(7.4f, output[0]);
			AssertAreClose(17.4f, output[1]);
		}

		TEST_METHOD(Export)
		{
			auto outputObject = Utils::JsonExportWriter();

			Network network(0);
			auto layer = Layer(3, 2, Relu::Default(), network);
			InitWeights(layer);

			layer.Export(outputObject);

			Assert::AreEqual("layer", outputObject.GetValue()["type"].GetString());
			Assert::AreEqual(3, outputObject.GetValue()["input_size"].GetInt());
			Assert::AreEqual(2, outputObject.GetValue()["output_size"].GetInt());
			Assert::AreEqual(Relu::Name, outputObject.GetValue()["transfer"].GetString());

			Assert::IsTrue(outputObject.GetValue().HasMember("nodes"));
			auto& nodes = outputObject.GetValue()["nodes"].GetArray();
			Assert::AreEqual(2, (int)nodes.Size());

			auto& weights0 = nodes[0].GetObject()["weights"].GetArray();
			Assert::AreEqual(4, (int)weights0.Size());
			Assert::AreEqual(1.f, weights0[0].GetFloat());
			Assert::AreEqual(2.f, weights0[1].GetFloat());
			Assert::AreEqual(3.f, weights0[2].GetFloat());
			Assert::AreEqual(4.f, weights0[3].GetFloat());

			auto& weights1 = nodes[1].GetObject()["weights"].GetArray();
			Assert::AreEqual(4, (int)weights1.Size());
			Assert::AreEqual(5.f, weights1[0].GetFloat());
			Assert::AreEqual(6.f, weights1[1].GetFloat());
			Assert::AreEqual(7.f, weights1[2].GetFloat());
			Assert::AreEqual(8.f, weights1[3].GetFloat());
		}

		TEST_METHOD(Import)
		{
			auto json = "{\"input_size\":2,\"output_size\":2,\"transfer\":\"relu\",\"nodes\":[{\"weights\":[1,2,3]},{\"weights\":[4,5,6]}]}";
			auto reader = Utils::IImportReader::CreateJsonImportReader(json);
			auto network = Network(2);
			auto layer = Layer(2, 2, TransferFunction::Sigmoid::Default(), network);

			layer.Import(*reader);

			Assert::AreEqual("relu", layer.GetTransferFunction().GetName());
			Assert::AreEqual(1.f, layer.Nodes[0]->Weights[0]);
			Assert::AreEqual(2.f, layer.Nodes[0]->Weights[1]);
			Assert::AreEqual(3.f, layer.Nodes[0]->Weights[2]);
			Assert::AreEqual(4.f, layer.Nodes[1]->Weights[0]);
			Assert::AreEqual(5.f, layer.Nodes[1]->Weights[1]);
			Assert::AreEqual(6.f, layer.Nodes[1]->Weights[2]);
		}

		TEST_METHOD(Import_WrongInputSize)
		{
			AssertThrows<ApophisException>([]() {
				auto json = "{\"input_size\":3,\"output_size\":2,\"transfer\":\"relu\",\"nodes\":[{\"weights\":[1,2,3]},{\"weights\":[4,5,6]}]}";
				auto reader = Utils::IImportReader::CreateJsonImportReader(json);
				auto network = Network(2);
				auto layer = Layer(2, 2, TransferFunction::Sigmoid::Default(), network);

				layer.Import(*reader);
			});
		}

		TEST_METHOD(Import_WrongOutputSize)
		{
			AssertThrows<ApophisException>([]() {
				auto json = "{\"input_size\":2,\"output_size\":1,\"transfer\":\"relu\",\"nodes\":[{\"weights\":[1,2,3]},{\"weights\":[4,5,6]}]}";
				auto reader = Utils::IImportReader::CreateJsonImportReader(json);
				auto network = Network(2);
				auto layer = Layer(2, 2, TransferFunction::Sigmoid::Default(), network);

				layer.Import(*reader);
			});
		}

		TEST_METHOD(Import_WrongNumberOfNodes)
		{
			AssertThrows<ApophisException>([]() {
				auto json = "{\"input_size\":2,\"output_size\":2,\"transfer\":\"relu\",\"nodes\":[{\"weights\":[1,2,3]}]}";
				auto reader = Utils::IImportReader::CreateJsonImportReader(json);
				auto network = Network(2);
				auto layer = Layer(2, 2, TransferFunction::Sigmoid::Default(), network);

				layer.Import(*reader);
			});
		}
	};
}}
