#pragma once

namespace SampleUtils { namespace IO {

	std::unique_ptr<Apophis::Utils::IImportReader> LoadJson(const char* filename);
	void SaveJson(Apophis::Utils::IExportWriter& writer, const char* filename);
	void SaveExamples(const Apophis::ExampleSet& examples, const char* filename);
	void SaveNetwork(const Apophis::Component::Network& network, const char* filename);

}}
