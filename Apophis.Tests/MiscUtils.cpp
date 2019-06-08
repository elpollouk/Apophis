#include "stdafx.h"
#include "apophis/Utils/IImportReader.h"

using namespace Apophis;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

std::unique_ptr<Utils::IImportReader> LoadJson(const char * path)
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
