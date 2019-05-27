#include "stdafx.h"
#include "apophis/Component/Network.h"
#include "apophis/TransferFunction/Relu.h"
#include "apophis/TransferFunction/Sigmoid.h"
#include "Utils/ImportExport.h"

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

		float GetWeight(const rapidjson::GenericValue<rapidjson::UTF8<>>::Array& layers, unsigned int layer, unsigned int node, unsigned int weight)
		{
			auto _layer = layers[layer].GetObject();
			auto _nodes = _layer[FIELD_NODES].GetArray();
			auto _node = _nodes[node].GetObject();
			auto _weights = _node[FIELD_WEIGHTS].GetArray();
			return _weights[weight].GetFloat();
		}

		TEST_METHOD(Export)
		{
			auto& allocator = rapidjson::MemoryPoolAllocator<>();
			auto outputObject = Utils::ExportTarget(rapidjson::kObjectType, allocator);

			Network network(3);
			network.AddLayer<Relu>(2);
			network.AddLayer<Sigmoid>(1);
			InitWeights(network);

			network.Export(outputObject);

			Assert::AreEqual("network", outputObject.Target["type"].GetString());
			Assert::AreEqual(3, outputObject.Target["input_size"].GetInt());
			Assert::AreEqual(1, outputObject.Target["output_size"].GetInt());
			Assert::IsTrue(outputObject.Target.HasMember("layers"));
			auto& layers = outputObject.Target["layers"].GetArray();
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
	};
}}
