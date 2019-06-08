#pragma once
#include <chrono>

namespace Apophis { namespace Utils {

	class Timer
	{
	public:
		Timer() :
			m_Start(std::chrono::steady_clock::now()),
			m_IterationStart(m_Start)
		{

		}

		void Reset()
		{
			m_IterationStart = std::chrono::steady_clock::now();
		}

		double Read()
		{
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_IterationStart);
			return (double)duration.count() / 1000.;
		}

		double Total()
		{
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_Start);
			return (double)duration.count() / 1000.;
		}

	private:
		std::chrono::time_point<std::chrono::steady_clock> m_Start;
		std::chrono::time_point<std::chrono::steady_clock> m_IterationStart;
	};

}}