#include "stdafx.h"
#include "io.h"

using namespace Apophis;
using namespace Apophis::Utils;

namespace SampleUtils { namespace IO {

std::unique_ptr<IImportReader> LoadJson(const char* filename)
{
	FILE* f = nullptr;
	auto err = fopen_s(&f, filename, "rb");
	if (err != 0) throw ApophisException("Failed to open %s, err = %d", filename, (int)err);

	fseek(f, 0, SEEK_END);
	auto size = ftell(f);
	fseek(f, 0, SEEK_SET);

	std::string buffer((size_t)size, (char)0);
	fread_s(&buffer[0], size, 1, size, f);
	fclose(f);

	return IImportReader::CreateJsonImportReader(buffer);
}

void SaveJson(Apophis::Utils::IExportWriter& writer, const char* filename)
{
	FILE* f = nullptr;
	auto err = fopen_s(&f, filename, "wb");
	if (err != 0) throw ApophisException("Failed to open %s, err = %d", filename, (int)err);

	auto json = writer.GetData();
	fwrite(json.c_str(), 1, json.size(), f);
	fclose(f);
}

void SaveExamples(const Apophis::ExampleSet& examples, const char* filename)
{
	auto writer = Utils::IExportWriter::CreateJsonExportWriter();
	examples.Export(*writer);
	SaveJson(*writer, filename);
}

void SaveNetwork(const Apophis::Component::Network& network, const char* filename)
{
	auto writer = Utils::IExportWriter::CreateJsonExportWriter();
	network.Export(*writer);
	SaveJson(*writer, filename);
}

}}
