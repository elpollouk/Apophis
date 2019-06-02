#include "stdafx.h"
#include "Bootstrap.h"
#include <iostream>

int main(int argc, const char** argv)
{
	try
	{
		return SampleMain(argc, argv);
	}
	catch (std::exception& ex)
	{
		std::cerr << "UNHANDLED EXCEPTION:\n";
		std::cerr << ex.what() << "\n";
		return 1;
	}
}