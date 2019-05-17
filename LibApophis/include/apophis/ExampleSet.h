#pragma once

#include "Apophis.h"
#include <random>

namespace Apophis {

	class Example
	{
	public:
		Example(const real* input, int inputSize, const real* output, int outputSize) :
			Input(input, input + inputSize),
			Output(output, output + outputSize)
		{

		}

		const Vector Input;
		const Vector Output;
	};

	class ExampleSet
	{
	public:
		ExampleSet(int inputSize, int outputSize);

		void AddExample(Example&& example);

		const Example& Sample();

		std::vector<Example>::const_iterator begin() const
		{
			return m_Examples.begin();
		}

		std::vector<Example>::const_iterator end() const
		{
			return m_Examples.end();
		}

		size_t size() const
		{
			return m_Examples.size();
		}

		const int InputSize;
		const int OutputSize;

	private:
		std::uniform_int_distribution<size_t> m_Distribution;
		std::vector<Example> m_Examples;
	};

}
