#include "stdafx.h"
#include "apophis/Component/Network.h"
#include "apophis/TransferFunction/Relu.h"
#include "apophis/TransferFunction/Sigmoid.h"
#include "apophis/Utils/IImportReader.h"
#include "Utils/JsonExportWriter.h"
#include "Utils/ImportExport.h"
#include "apophis/ExampleSet.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Apophis;
using namespace Apophis::Component;
using namespace Apophis::TransferFunction;
using namespace Apophis::Utils;

namespace ApophisTests { namespace Component
{
	TEST_CLASS(NetworkTests)
	{
	public:

		static void InitWeights(Network& network)
		{
			auto value = 1.f;
			for (auto& layer : network.GetLayers())
			{
				for (auto& node : layer->Nodes)
				{
					for (auto& weight : node->Weights)
					{
						weight = value;
						value += 1;
					}
				}
			}
		}

		TEST_METHOD(Build)
		{
			Network network(3);
			network.AddLayer<Relu>(2);
			network.AddLayer<Sigmoid>(1);

			Assert::AreEqual(3, (int)network.GetInputSize());
			Assert::AreEqual(1, (int)network.GetOutputSize());
			Assert::AreEqual(2, (int)network.GetLayers().size());
		}

		float GetWeight(const rapidjson::GenericValue<rapidjson::UTF8<>>::ConstArray& layers, unsigned int layer, unsigned int node, unsigned int weight)
		{
			auto _layer = layers[layer].GetObject();
			auto _nodes = _layer[FIELD_NODES].GetArray();
			auto _node = _nodes[node].GetObject();
			auto _weights = _node[FIELD_WEIGHTS].GetArray();
			return _weights[weight].GetFloat();
		}

		TEST_METHOD(Export)
		{
			auto outputObject = Utils::JsonExportWriter();

			Network network(3);
			network.AddLayer<Relu>(2);
			network.AddLayer<Sigmoid>(1);
			InitWeights(network);

			network.Export(outputObject);

			Assert::AreEqual("network", outputObject.GetValue()["type"].GetString());
			Assert::AreEqual(3, outputObject.GetValue()["input_size"].GetInt());
			Assert::AreEqual(1, outputObject.GetValue()["output_size"].GetInt());
			Assert::IsTrue(outputObject.GetValue().HasMember("layers"));
			auto& layers = outputObject.GetValue()["layers"].GetArray();
			Assert::AreEqual(2, (int)layers.Size());

			Assert::AreEqual(Relu::Name, layers[0].GetObject()[FIELD_TRANSFER].GetString());
			Assert::AreEqual(2, (int)layers[0].GetObject()[FIELD_NODES].GetArray().Size());
			
			Assert::AreEqual(4, (int)layers[0].GetObject()[FIELD_NODES].GetArray()[0].GetObject()[FIELD_WEIGHTS].GetArray().Size());
			Assert::AreEqual(1.f, GetWeight(layers, 0, 0, 0));
			Assert::AreEqual(2.f, GetWeight(layers, 0, 0, 1));
			Assert::AreEqual(3.f, GetWeight(layers, 0, 0, 2));
			Assert::AreEqual(4.f, GetWeight(layers, 0, 0, 3));

			Assert::AreEqual(4, (int)layers[0].GetObject()[FIELD_NODES].GetArray()[1].GetObject()[FIELD_WEIGHTS].GetArray().Size());
			Assert::AreEqual(5.f, GetWeight(layers, 0, 1, 0));
			Assert::AreEqual(6.f, GetWeight(layers, 0, 1, 1));
			Assert::AreEqual(7.f, GetWeight(layers, 0, 1, 2));
			Assert::AreEqual(8.f, GetWeight(layers, 0, 1, 3));

			Assert::AreEqual(Sigmoid::Name, layers[1].GetObject()[FIELD_TRANSFER].GetString());
			Assert::AreEqual(1, (int)layers[1].GetObject()[FIELD_NODES].GetArray().Size());
			Assert::AreEqual(3, (int)layers[1].GetObject()[FIELD_NODES].GetArray()[0].GetObject()[FIELD_WEIGHTS].GetArray().Size());
			Assert::AreEqual( 9.f, GetWeight(layers, 1, 0, 0));
			Assert::AreEqual(10.f, GetWeight(layers, 1, 0, 1));
			Assert::AreEqual(11.f, GetWeight(layers, 1, 0, 2));
		}

		TEST_METHOD(Import)
		{
			auto outputObject = Utils::JsonExportWriter();

			Network network(3);
			network.AddLayer<Relu>(2);
			network.AddLayer<Sigmoid>(1);
			InitWeights(network);

			network.Export(outputObject);

			auto json = outputObject.GetData();
			auto reader = IImportReader::CreateJsonImportReader(json);

			Network importedNetwork(*reader);

			Assert::AreEqual(3, (int)importedNetwork.GetInputSize());
			Assert::AreEqual(1, (int)importedNetwork.GetOutputSize());
			Assert::AreEqual(2, (int)importedNetwork.GetLayers().size());
			Assert::AreEqual(3, (int)importedNetwork.GetLayers()[0]->GetNumInputs());
			Assert::AreEqual(2, (int)importedNetwork.GetLayers()[0]->GetNumOutputs());
			Assert::AreEqual(2, (int)importedNetwork.GetLayers()[0]->Nodes.size());
			Assert::AreEqual(2, (int)importedNetwork.GetLayers()[1]->GetNumInputs());
			Assert::AreEqual(1, (int)importedNetwork.GetLayers()[1]->GetNumOutputs());
			Assert::AreEqual(1, (int)importedNetwork.GetLayers()[1]->Nodes.size());

			Assert::AreEqual(1.f, importedNetwork.GetLayers()[0]->Nodes[0]->Weights[0]);
			Assert::AreEqual(2.f, importedNetwork.GetLayers()[0]->Nodes[0]->Weights[1]);
			Assert::AreEqual(3.f, importedNetwork.GetLayers()[0]->Nodes[0]->Weights[2]);
			Assert::AreEqual(4.f, importedNetwork.GetLayers()[0]->Nodes[0]->Weights[3]);

			Assert::AreEqual(5.f, importedNetwork.GetLayers()[0]->Nodes[1]->Weights[0]);
			Assert::AreEqual(6.f, importedNetwork.GetLayers()[0]->Nodes[1]->Weights[1]);
			Assert::AreEqual(7.f, importedNetwork.GetLayers()[0]->Nodes[1]->Weights[2]);
			Assert::AreEqual(8.f, importedNetwork.GetLayers()[0]->Nodes[1]->Weights[3]);

			Assert::AreEqual(9.f, importedNetwork.GetLayers()[1]->Nodes[0]->Weights[0]);
			Assert::AreEqual(10.f, importedNetwork.GetLayers()[1]->Nodes[0]->Weights[1]);
			Assert::AreEqual(11.f, importedNetwork.GetLayers()[1]->Nodes[0]->Weights[2]);
		}

		static std::unique_ptr<Utils::IImportReader> Load(const char * path)
		{
			FILE* f = nullptr;
			auto err = fopen_s(&f, path, "rb");
			Assert::AreEqual(0, err, L"Failed to open input file");

			fseek(f, 0, SEEK_END);
			auto size = ftell(f);
			fseek(f, 0, SEEK_SET);

			std::string buffer((size_t)size, (char)0);
			fread_s(&buffer[0], size, 1, size, f);
			fclose(f);

			return Utils::IImportReader::CreateJsonImportReader(buffer);
		}

		static int ArgMax(ConstVectorRef vector)
		{
			auto maxIndex = 0;
			for (auto i = 1; i < vector.size(); i++)
				if (vector[maxIndex] < vector[i])
					maxIndex = i;

			return maxIndex;
		}

		TEST_METHOD(IrisClassifier)
		{
			Network network(*Load("../../../Data/Iris/network.json"));
			ExampleSet data(*Load("../../../Data/Iris/test.json"));

			Assert::AreEqual(4, (int)network.GetInputSize(), L"Incorrect network input size");
			Assert::AreEqual(3, (int)network.GetOutputSize(), L"Incorrect network output size");
			Assert::AreEqual(15, (int)data.size(), L"Incorrect number of examples in test set");

			Assert::AreEqual(0, ArgMax(network.Calculate(data[0].Input)), L"Example 0 classified incorrectly");
			Assert::AreEqual(0, ArgMax(network.Calculate(data[1].Input)), L"Example 1 classified incorrectly");
			Assert::AreEqual(0, ArgMax(network.Calculate(data[2].Input)), L"Example 2 classified incorrectly");
			Assert::AreEqual(0, ArgMax(network.Calculate(data[3].Input)), L"Example 3 classified incorrectly");
			Assert::AreEqual(0, ArgMax(network.Calculate(data[4].Input)), L"Example 4 classified incorrectly");
			Assert::AreEqual(1, ArgMax(network.Calculate(data[5].Input)), L"Example 5 classified incorrectly");
			Assert::AreEqual(1, ArgMax(network.Calculate(data[6].Input)), L"Example 6 classified incorrectly");
			Assert::AreEqual(1, ArgMax(network.Calculate(data[7].Input)), L"Example 7 classified incorrectly");
			Assert::AreEqual(1, ArgMax(network.Calculate(data[8].Input)), L"Example 8 classified incorrectly");
			Assert::AreEqual(1, ArgMax(network.Calculate(data[9].Input)), L"Example 9 classified incorrectly");
			Assert::AreEqual(2, ArgMax(network.Calculate(data[10].Input)), L"Example 10 classified incorrectly");
			Assert::AreEqual(2, ArgMax(network.Calculate(data[11].Input)), L"Example 11 classified incorrectly");
			Assert::AreEqual(2, ArgMax(network.Calculate(data[12].Input)), L"Example 12 classified incorrectly");
			Assert::AreEqual(2, ArgMax(network.Calculate(data[13].Input)), L"Example 13 classified incorrectly");
			Assert::AreEqual(2, ArgMax(network.Calculate(data[14].Input)), L"Example 14 classified incorrectly");
		}
	};
}}
