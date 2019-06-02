#include "pch.h"
#include <iostream>
#include "Bootstrap.h"

using namespace SampleUtils;

int SampleMain(int argc, const char** argv)
{
	std::cout << "XOX Sample\n";
	auto reader = IO::LoadJson("../../Data/Xor/network.json");
	return 0;
}
