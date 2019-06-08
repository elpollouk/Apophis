#include "stdafx.h"
#include "apophis/Vector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Apophis;

namespace ApophisTests {

	TEST_CLASS(VectorTests)
	{
		TEST_METHOD(Construct_Default)
		{
			auto v = Vector();

			Assert::AreEqual(0, (int)v.size(), L"Constructed Vector has wrong size");
		}

		TEST_METHOD(Construct_InitializerList)
		{
			auto v = Vector({ 7.f, 13.f, 17.f });

			Assert::AreEqual(3, (int)v.size(), L"Constructed Vector has wrong size");
			Assert::AreEqual(7.f, v[0], L"Element 0 has wrong value");
			Assert::AreEqual(13.f, v[1], L"Element 1 has wrong value");
			Assert::AreEqual(17.f, v[2], L"Element 2 has wrong value");
		}
		
		TEST_METHOD(Construct_RawArray)
		{
			const real a[] = { 5.f, 3.f, 1.f, 0.5f };
			auto v = Vector(4, a);

			Assert::AreEqual(4, (int)v.size(), L"Constructed Vector has wrong size");
			Assert::AreEqual(5.f, v[0], L"Element 0 has wrong value");
			Assert::AreEqual(3.f, v[1], L"Element 1 has wrong value");
			Assert::AreEqual(1.f, v[2], L"Element 2 has wrong value");
			Assert::AreEqual(0.5f, v[3], L"Element 3 has wrong value");
		}

		TEST_METHOD(Construct_Copy)
		{
			auto v1 = Vector({ 2.f, 4.f });
			auto v2 = v1;

			Assert::AreEqual(2, (int)v2.size(), L"Constructed Vector has wrong size");
			Assert::AreEqual(2.f, v2[0], L"Element 0 has wrong value");
			Assert::AreEqual(4.f, v2[1], L"Element 1 has wrong value");
		}

		TEST_METHOD(Construct_Move)
		{
			auto v1 = Vector({ 2.f, 4.f });
			auto v2 = std::move(v1);

			Assert::AreEqual(2, (int)v2.size(), L"Constructed Vector has wrong size");
			Assert::AreEqual(2.f, v2[0], L"Element 0 has wrong value");
			Assert::AreEqual(4.f, v2[1], L"Element 1 has wrong value");
			Assert::AreEqual(0, (int)v1.size(), L"Moved Vector has wrong size");
		}

		TEST_METHOD(Construct_Size)
		{
			auto v = Vector(2);

			Assert::AreEqual(2, (int)v.size(), L"Constructed Vector has wrong size");
		}

		TEST_METHOD(AssignToElement)
		{
			auto v = Vector(3);
			v[0] = 17.f;
			v[1] = 19.f;
			v[2] = 23.f;

			Assert::AreEqual(17.f, v[0], L"Element 0 has wrong value");
			Assert::AreEqual(19.f, v[1], L"Element 1 has wrong value");
			Assert::AreEqual(23.f, v[2], L"Element 2 has wrong value");
		}

		TEST_METHOD(Assign)
		{
			auto v1 = Vector({ 1.f, 2.f, 3.f });
			auto v2 = Vector(0);
			v2 = v1;

			Assert::AreEqual(3, (int)v2.size(), L"Constructed Vector has wrong size");
			Assert::AreEqual(1.f, v2[0], L"Element 0 has wrong value");
			Assert::AreEqual(2.f, v2[1], L"Element 1 has wrong value");
			Assert::AreEqual(3.f, v2[2], L"Element 2 has wrong value");
		}

		TEST_METHOD(ConstAccess)
		{
			auto v1 = Vector({ 2.f, 4.f });
			const auto& v2 = v1;

			Assert::AreEqual(2.f, v2[0], L"Element 0 has wrong value");
			Assert::AreEqual(4.f, v2[1], L"Element 1 has wrong value");
		}

		TEST_METHOD(Iterate)
		{
			auto vec = Vector({ 1.f, 2.f, 3.f });
			real t = 0.f;
			for (auto v : vec)
				t += v;

			Assert::AreEqual(6.f, t, L"Value sum was wrong");
		}

		TEST_METHOD(Iterate_Const)
		{
			auto vec1 = Vector({ 4.f, 5.f, 6.f });
			const auto& vec2 = vec1;
			real t = 0.f;
			for (auto v : vec2)
				t += v;

			Assert::AreEqual(15.f, t, L"Value sum was wrong");
		}

		TEST_METHOD(resize_Grow)
		{
			auto v = Vector({ 1.f, 2.f });
			v.resize(3);

			Assert::AreEqual(3, (int)v.size(), L"Vector has wrong size");

			v[2] = 3.f;

			Assert::AreEqual(1.f, v[0], L"Element 0 has wrong value");
			Assert::AreEqual(2.f, v[1], L"Element 1 has wrong value");
			Assert::AreEqual(3.f, v[2], L"Element 2 has wrong value");
		}

		TEST_METHOD(resize_Shrink)
		{
			auto v = Vector({ 1.f, 2.f });
			v.resize(1);

			Assert::AreEqual(1, (int)v.size(), L"Vector has wrong size");

			Assert::AreEqual(1.f, v[0], L"Element 0 has wrong value");
		}

		TEST_METHOD(resize_WithValue_Grow)
		{
			auto v = Vector({ 1.f, 2.f });
			v.resize(3, 4.f);

			Assert::AreEqual(3, (int)v.size(), L"Vector has wrong size");
			Assert::AreEqual(1.f, v[0], L"Element 0 has wrong value");
			Assert::AreEqual(2.f, v[1], L"Element 1 has wrong value");
			Assert::AreEqual(4.f, v[2], L"Element 2 has wrong value");
		}

		TEST_METHOD(resize_WithValue_Shrink)
		{
			auto v = Vector({ 1.f, 2.f });
			v.resize(1, 4.f);

			Assert::AreEqual(1, (int)v.size(), L"Vector has wrong size");
			Assert::AreEqual(1.f, v[0], L"Element 0 has wrong value");
		}

		TEST_METHOD(resize_WithValue_DefaultConstruct)
		{
			Vector v;
			v.resize(3, 4.f);

			Assert::AreEqual(3, (int)v.size(), L"Vector has wrong size");
			Assert::AreEqual(4.f, v[0], L"Element 0 has wrong value");
			Assert::AreEqual(4.f, v[1], L"Element 1 has wrong value");
			Assert::AreEqual(4.f, v[2], L"Element 2 has wrong value");
		}

		TEST_METHOD(max)
		{
			Vector v({ 5.f, 10.f, 15.f, 7.f });
			Assert::AreEqual(15.f, v.max(), L"Incorrect max value returned");
		}

		TEST_METHOD(argmax)
		{
			Vector v({ 5.f, 11.f, 7.f, 7.f });
			Assert::AreEqual(1, (int)v.argmax(), L"Incorrect max index returned");
		}

		TEST_METHOD(argmax_multimax)
		{
			Vector v({ 5.f, 1.f, 7.f, 7.f });
			Assert::AreEqual(2, (int)v.argmax(), L"Incorrect max index returned");
		}

		TEST_METHOD(OneHot)
		{
			auto v = Vector::OneHot(4, 2);

			Assert::AreEqual(4, (int)v.size(), L"Vector was constructed with wrong size");
			Assert::AreEqual(0.f, v[0], L"Element 0 has wrong value");
			Assert::AreEqual(0.f, v[1], L"Element 1 has wrong value");
			Assert::AreEqual(1.f, v[2], L"Element 2 has wrong value");
			Assert::AreEqual(0.f, v[3], L"Element 3 has wrong value");
		}

		TEST_METHOD(OneHot_NegativeValue)
		{
			auto v = Vector::OneHot(4, (size_t)-1);

			Assert::AreEqual(4, (int)v.size(), L"Vector was constructed with wrong size");
			Assert::AreEqual(0.f, v[0], L"Element 0 has wrong value");
			Assert::AreEqual(0.f, v[1], L"Element 1 has wrong value");
			Assert::AreEqual(0.f, v[2], L"Element 2 has wrong value");
			Assert::AreEqual(0.f, v[3], L"Element 3 has wrong value");
		}
	};

}
