// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Headers for CppUnitTest
#include "CppUnitTest.h"

#include <vector>
#include <functional>
#include <stdio.h>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>


template<typename T> static void AssertAreClose(const T& expected, const T& actual, const T& range = 0.00001f, const wchar_t* message = NULL, const Microsoft::VisualStudio::CppUnitTestFramework::__LineInfo* pLineInfo = NULL)
{
	if (actual < (expected - range) || actual >(expected + range))
	{
		wchar_t buffer[4096];
		auto sExpected = Microsoft::VisualStudio::CppUnitTestFramework::ToString(expected);
		auto sActual = Microsoft::VisualStudio::CppUnitTestFramework::ToString(actual);
		if (message)
			swprintf_s(buffer, L"%s. Expected:<%s> Actual:<%s>", message, sExpected.c_str(), sActual.c_str());
		else
			swprintf_s(buffer, L"Assert failed. Expected:<%s> Actual:<%s>", sExpected.c_str(), sActual.c_str());

		Assert::Fail(buffer, pLineInfo);
	}
}

template<typename T> static T AssertThrows(std::function<void()> test)
{
	try
	{
		test();
		Microsoft::VisualStudio::CppUnitTestFramework::Assert::Fail(L"No exception thrown");
		return T("No exception thrown");
	}
	catch (T& ex)
	{
		return ex;
	}
	catch (...)
	{
		Microsoft::VisualStudio::CppUnitTestFramework::Assert::Fail(L"Wrong exception type thrown");
		return T("Wrong exception type thrown");
	}
}