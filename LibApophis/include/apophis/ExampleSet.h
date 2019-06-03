#pragma once

#include "apophis/apophistypes.h"
#include <random>

namespace Apophis {

	class Example
	{
	public:
		Example() {}
		Example(std::initializer_list<real> input, std::initializer_list<real> output) :
			Input(input),
			Output(output)
		{

		}

		Example(ConstVectorRef input, ConstVectorRef output) :
			Input(input),
			Output(output)
		{

		}

		Vector Input;
		Vector Output;
	};

	class IExampleProvider
	{
	public:
		~IExampleProvider() {}
		virtual const Example& Sample() const = 0;
	};

	class ExampleSet : public IExampleProvider
	{
	public:
		ExampleSet(Utils::IImportReader& data);
		ExampleSet(int inputSize, int outputSize);

		void AddExample(Example&& example);
		void AddExample(std::initializer_list<real> input, std::initializer_list<real> output)
		{
			AddExample(Example(input, output));
		}

		void AddExample(ConstVectorRef input, ConstVectorRef output)
		{
			AddExample(Example(input, output));
		}

		virtual const Example& Sample() const override;
		void Import(Utils::IImportReader& data);
		void Export(Utils::IExportWriter& output) const;

		std::vector<Example>::const_iterator begin() const { return m_Examples.begin(); }
		std::vector<Example>::const_iterator end() const { return m_Examples.end(); }
		size_t size() const { return m_Examples.size(); }
		Example& operator[](size_t index) { return m_Examples[index]; }

		size_t InputSize;
		size_t OutputSize;

	private:
		std::uniform_int_distribution<size_t> m_Distribution;
		std::vector<Example> m_Examples;
	};

}
