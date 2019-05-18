#include "stdafx.h"
#include "apophis/Data/Metrics.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Apophis::Data;

namespace ApophisTests { namespace Data
{

	TEST_CLASS(MetricsTests)
	{
	public:
		//---------------------------------------------------------------------------------------//
		// Double
		//---------------------------------------------------------------------------------------//
		TEST_METHOD(GetDouble_NonExisting)
		{
			Metrics metrics;

			Assert::AreEqual(0., metrics.Get<double>("A"));
			Assert::AreEqual(0., metrics.Get<double>("B"));
		}

		TEST_METHOD(Set_double_NonExisting)
		{
			Metrics metrics;
			metrics.Set("A", 1.);
			metrics.Set("B", 2.);

			Assert::AreEqual(1., metrics.Get<double>("A"));
			Assert::AreEqual(2., metrics.Get<double>("B"));
		}

		TEST_METHOD(Add_double_NonExisting)
		{
			Metrics metrics;
			Assert::AreEqual(1., metrics.Add("A", 1.));
			Assert::AreEqual(2., metrics.Add("B", 2.));

			Assert::AreEqual(1., metrics.Get<double>("A"));
			Assert::AreEqual(2., metrics.Get<double>("B"));
		}

		TEST_METHOD(Set_double_Existing)
		{
			Metrics metrics;
			metrics.Set("A", 1.);
			metrics.Set("B", 2.);
			metrics.Set("A", 3.);

			Assert::AreEqual(3., metrics.Get<double>("A"));
			Assert::AreEqual(2., metrics.Get<double>("B"));
		}

		TEST_METHOD(Add_double_Existing)
		{
			Metrics metrics;
			metrics.Set("A", 1.);
			metrics.Set("B", 2.);
			Assert::AreEqual(-1., metrics.Add("B", -3.));

			Assert::AreEqual(1., metrics.Get<double>("A"));
			Assert::AreEqual(-1., metrics.Get<double>("B"));
		}


		//---------------------------------------------------------------------------------------//
		// Float
		//---------------------------------------------------------------------------------------//
		TEST_METHOD(GetFloat_NonExisting)
		{
			Metrics metrics;

			Assert::AreEqual(0.f, metrics.Get<float>("A"));
			Assert::AreEqual(0.f, metrics.Get<float>("B"));
		}

		TEST_METHOD(Set_float_NonExisting)
		{
			Metrics metrics;
			metrics.Set("A", 1.f);
			metrics.Set("B", 2.f);

			Assert::AreEqual(1.f, metrics.Get<float>("A"));
			Assert::AreEqual(2.f, metrics.Get<float>("B"));
		}

		TEST_METHOD(Add_float_NonExisting)
		{
			Metrics metrics;
			Assert::AreEqual(1.f, metrics.Add("A", 1.f));
			Assert::AreEqual(2.f, metrics.Add("B", 2.f));

			Assert::AreEqual(1.f, metrics.Get<float>("A"));
			Assert::AreEqual(2.f, metrics.Get<float>("B"));
		}

		TEST_METHOD(Set_float_Existing)
		{
			Metrics metrics;
			metrics.Set("A", 1.f);
			metrics.Set("B", 2.f);
			metrics.Set("A", 3.f);

			Assert::AreEqual(3.f, metrics.Get<float>("A"));
			Assert::AreEqual(2.f, metrics.Get<float>("B"));
		}

		TEST_METHOD(Add_float_Existing)
		{
			Metrics metrics;
			metrics.Set("A", 1.f);
			metrics.Set("B", 2.f);
			Assert::AreEqual(-1.f, metrics.Add("B", -3.f));

			Assert::AreEqual(1.f, metrics.Get<float>("A"));
			Assert::AreEqual(-1.f, metrics.Get<float>("B"));
		}


		//---------------------------------------------------------------------------------------//
		// Int64
		//---------------------------------------------------------------------------------------//
		TEST_METHOD(GetInt64_NonExisting)
		{
			Metrics metrics;

			Assert::AreEqual(0, (int)metrics.Get<long long>("A"));
			Assert::AreEqual(0, (int)metrics.Get<long long>("B"));
		}

		TEST_METHOD(Set_int64_NonExisting)
		{
			Metrics metrics;
			metrics.Set("A", 3ll);
			metrics.Set("B", 4ll);

			Assert::AreEqual(3, (int)metrics.Get<long long>("A"));
			Assert::AreEqual(4, (int)metrics.Get<long long>("B"));
		}

		TEST_METHOD(Add_int64_NonExisting)
		{
			Metrics metrics;
			Assert::AreEqual(3, (int)metrics.Add("A", 3ll));
			Assert::AreEqual(4, (int)metrics.Add("B", 4ll));

			Assert::AreEqual(3, (int)metrics.Get<long long>("A"));
			Assert::AreEqual(4, (int)metrics.Get<long long>("B"));
		}

		TEST_METHOD(Set_int64_Existing)
		{
			Metrics metrics;
			metrics.Set("A", 4ll);
			metrics.Set("B", 5ll);
			metrics.Set("A", 6ll);

			Assert::AreEqual(6, (int)metrics.Get<long long>("A"));
			Assert::AreEqual(5, (int)metrics.Get<long long>("B"));
		}

		TEST_METHOD(Add_int64_Existing)
		{
			Metrics metrics;
			metrics.Set("A", 3ll);
			metrics.Set("B", 4ll);
			Assert::AreEqual(13, (int)metrics.Add("A", 10ll));

			Assert::AreEqual(13, (int)metrics.Get<long long>("A"));
			Assert::AreEqual(4, (int)metrics.Get<long long>("B"));
		}


		//---------------------------------------------------------------------------------------//
		// Int32
		//---------------------------------------------------------------------------------------//
		TEST_METHOD(GetInt32_NonExisting)
		{
			Metrics metrics;

			Assert::AreEqual(0, metrics.Get<int>("A"));
			Assert::AreEqual(0, metrics.Get<int>("B"));
		}

		TEST_METHOD(Set_int32_NonExisting)
		{
			Metrics metrics;
			metrics.Set("A", 3);
			metrics.Set("B", 4);

			Assert::AreEqual(3, metrics.Get<int>("A"));
			Assert::AreEqual(4, metrics.Get<int>("B"));
		}

		TEST_METHOD(Add_int32_NonExisting)
		{
			Metrics metrics;
			Assert::AreEqual(3, metrics.Add("A", 3));
			Assert::AreEqual(4, metrics.Add("B", 4));

			Assert::AreEqual(3, metrics.Get<int>("A"));
			Assert::AreEqual(4, metrics.Get<int>("B"));
		}

		TEST_METHOD(Set_int32_Existing)
		{
			Metrics metrics;
			metrics.Set("A", 4);
			metrics.Set("B", 5);
			metrics.Set("A", 6);

			Assert::AreEqual(6, metrics.Get<int>("A"));
			Assert::AreEqual(5, metrics.Get<int>("B"));
		}

		TEST_METHOD(Add_int32_Existing)
		{
			Metrics metrics;
			metrics.Set("A", 3);
			metrics.Set("B", 4);
			Assert::AreEqual(13, metrics.Add("A", 10));

			Assert::AreEqual(13, metrics.Get<int>("A"));
			Assert::AreEqual(4, metrics.Get<int>("B"));
		}

	};

}}