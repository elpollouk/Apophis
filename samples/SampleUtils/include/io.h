#pragma once

namespace SampleUtils { namespace IO {

	std::unique_ptr<Apophis::Utils::IImportReader> LoadJson(const char* filename);

}}
