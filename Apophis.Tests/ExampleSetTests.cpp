#include "stdafx.h"
#include "apophis/ExampleSet.h"
#include "apophis/ApophisException.h"
#include "apophis/Random.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Apophis;

constexpr const char* TEST_EXAMPLESET = "{\"input_size\":3,\"output_size\":2,\"examples\":[{\"input\":[1.0,2.0,3.0],\"output\":[4.0,5.0]},{\"input\":[6.0,7.0,8.0],\"output\":[9.0,0.0]}]}";

namespace ApophisTests {

	TEST_CLASS(ExampleSetTests)
	{
	public:

		TEST_METHOD(Example_Construct)
		{
			auto example = Example({ 1.f, 2.f, 3.4f }, { 5.67f });

			Assert::AreEqual(3u, (unsigned int)example.Input.size());
			Assert::AreEqual(1.f, example.Input[0]);
			Assert::AreEqual(2.f, example.Input[1]);
			Assert::AreEqual(3.4f, example.Input[2]);

			Assert::AreEqual(1u, (unsigned int)example.Output.size());
			Assert::AreEqual(5.67f, example.Output[0]);
		}

		static void Populate(ExampleSet& exampleSet)
		{
			exampleSet.AddExample(Example({ 0.f, 1.f, 2.f }, { 3.f, 0.f }));
			exampleSet.AddExample(Example({ 4.f, 5.f, 6.f }, { 7.f, 1.f }));
			exampleSet.AddExample(Example({ 8.f, 9.f, 10.f }, { 11.f, 2.f }));
		}

		TEST_METHOD(ExampleSet_Construct)
		{
			ExampleSet examples(4, 5);

			Assert::AreEqual(4, examples.InputSize);
			Assert::AreEqual(5, examples.OutputSize);
		}

		TEST_METHOD(ExampleSet_AddExample)
		{
			ExampleSet examples(3, 2);
			Populate(examples);

			Assert::AreEqual((size_t)3, examples.size());
		}

		TEST_METHOD(ExampleSet_Iterate)
		{
			ExampleSet examples(3, 2);
			Populate(examples);

			auto it = examples.begin();
			Assert::AreEqual(0.f, it->Input[0]);
			Assert::AreEqual(1.f, it->Input[1]);
			Assert::AreEqual(2.f, it->Input[2]);
			Assert::AreEqual(3.f, it->Output[0]);
			Assert::AreEqual(0.f, it->Output[1]);

			++it;
			Assert::AreEqual(4.f, it->Input[0]);
			Assert::AreEqual(5.f, it->Input[1]);
			Assert::AreEqual(6.f, it->Input[2]);
			Assert::AreEqual(7.f, it->Output[0]);
			Assert::AreEqual(1.f, it->Output[1]);

			++it;
			Assert::AreEqual(8.f, it->Input[0]);
			Assert::AreEqual(9.f, it->Input[1]);
			Assert::AreEqual(10.f, it->Input[2]);
			Assert::AreEqual(11.f, it->Output[0]);
			Assert::AreEqual(2.f, it->Output[1]);

			++it;
			Assert::IsTrue(examples.end() == it);
		}

		TEST_METHOD(ExampleSet_Sample)
		{
			ExampleSet examples(3, 2);
			Populate(examples);
			Seed(5);

			{
				auto example = examples.Sample();
				Assert::AreEqual(8.f, example.Input[0]);
				Assert::AreEqual(9.f, example.Input[1]);
				Assert::AreEqual(10.f, example.Input[2]);
				Assert::AreEqual(11.f, example.Output[0]);
				Assert::AreEqual(2.f, example.Output[1]);
			}

			{
				auto example = examples.Sample();
				Assert::AreEqual(0.f, example.Input[0]);
				Assert::AreEqual(1.f, example.Input[1]);
				Assert::AreEqual(2.f, example.Input[2]);
				Assert::AreEqual(3.f, example.Output[0]);
				Assert::AreEqual(0.f, example.Output[1]);
			}

			{
				auto example = examples.Sample();
				Assert::AreEqual(4.f, example.Input[0]);
				Assert::AreEqual(5.f, example.Input[1]);
				Assert::AreEqual(6.f, example.Input[2]);
				Assert::AreEqual(7.f, example.Output[0]);
				Assert::AreEqual(1.f, example.Output[1]);
			}

			{
				auto example = examples.Sample();
				Assert::AreEqual(0.f, example.Input[0]);
				Assert::AreEqual(1.f, example.Input[1]);
				Assert::AreEqual(2.f, example.Input[2]);
				Assert::AreEqual(3.f, example.Output[0]);
				Assert::AreEqual(0.f, example.Output[1]);
			}
		}

		TEST_METHOD(Export)
		{
			auto examples = ExampleSet(3, 2);
			examples.AddExample({ 1.f, 2.f, 3.f }, { 4.f, 5.f });
			examples.AddExample({ 6.f, 7.f, 8.f }, { 9.f, 0.f });

			auto json = examples.Export();
			Assert::AreEqual(
				TEST_EXAMPLESET,
				json.c_str()
			);
		}

		TEST_METHOD(Import)
		{
			ExampleSet examples;
			examples.Import(TEST_EXAMPLESET);

			Assert::AreEqual(3, examples.InputSize);
			Assert::AreEqual(2, examples.OutputSize);
			Assert::AreEqual(1.f, examples[0].Input[0]);
			Assert::AreEqual(2.f, examples[0].Input[1]);
			Assert::AreEqual(3.f, examples[0].Input[2]);
			Assert::AreEqual(4.f, examples[0].Output[0]);
			Assert::AreEqual(5.f, examples[0].Output[1]);
			Assert::AreEqual(6.f, examples[1].Input[0]);
			Assert::AreEqual(7.f, examples[1].Input[1]);
			Assert::AreEqual(8.f, examples[1].Input[2]);
			Assert::AreEqual(9.f, examples[1].Output[0]);
			Assert::AreEqual(0.f, examples[1].Output[1]);
		}

		TEST_METHOD(Import_NotJSON)
		{
			AssertThrows<ApophisException>([]() {
				ExampleSet examples;
				examples.Import("BAD");
			});
		}

		TEST_METHOD(Import_MissingInputSize)
		{
			AssertThrows<ApophisException>([]() {
				ExampleSet examples;
				examples.Import("{\"output_size\":0,\"examples\":[]}");
			});
		}

		TEST_METHOD(Import_MissingOuputSize)
		{
			AssertThrows<ApophisException>([]() {
				ExampleSet examples;
				examples.Import("{\"input_size\":0,\"examples\":[]}");
			});
		}

		TEST_METHOD(Import_ExamplesNotArray)
		{
			AssertThrows<ApophisException>([]() {
				ExampleSet examples;
				examples.Import("{\"input_size\":0,\"output_size\":0,\"examples\":0}");
			});
		}

		TEST_METHOD(Import_ExampleNotObject)
		{
			AssertThrows<ApophisException>([]() {
				ExampleSet examples;
				examples.Import("{\"input_size\":1,\"output_size\":1,\"examples\":[0]}");
			});
		}

		TEST_METHOD(Import_ExampleNoInput)
		{
			AssertThrows<ApophisException>([]() {
				ExampleSet examples;
				examples.Import("{\"input_size\":1,\"output_size\":1,\"examples\":[{\"output\":[1]}]}");
			});
		}

		TEST_METHOD(Import_ExampleNoOutput)
		{
			AssertThrows<ApophisException>([]() {
				ExampleSet examples;
				examples.Import("{\"input_size\":1,\"output_size\":1,\"examples\":[{\"input\":[1]}]}");
			});
		}

		TEST_METHOD(Import_ExampleInputSizeMismatch)
		{
			AssertThrows<ApophisException>([]() {
				ExampleSet examples;
				examples.Import("{\"input_size\":1,\"output_size\":1,\"examples\":[{\"input\":[1,2], \"output\":[1]}]}");
			});
		}

		TEST_METHOD(Import_ExampleOutputSizeMismatch)
		{
			AssertThrows<ApophisException>([]() {
				ExampleSet examples;
				examples.Import("{\"input_size\":1,\"output_size\":1,\"examples\":[{\"input\":[1], \"output\":[]}]}");
			});
		}

		TEST_METHOD(Import_ExampleInputNotArray)
		{
			AssertThrows<ApophisException>([]() {
				ExampleSet examples;
				examples.Import("{\"input_size\":1,\"output_size\":1,\"examples\":[{\"input\":{}, \"output\":[1]}]}");
			});
		}

		TEST_METHOD(Import_ExampleOutputNotArray)
		{
			AssertThrows<ApophisException>([]() {
				ExampleSet examples;
				examples.Import("{\"input_size\":1,\"output_size\":1,\"examples\":[{\"input\":[1], \"output\":0}]}");
			});
		}
	};
}
