#pragma once
#include <chrono>

namespace Apophis { namespace Utils {

	class Timer
	{
	public:
		Timer()
		{
			Reset();
		}

		void Reset()
		{
			m_Start = std::chrono::steady_clock::now();
		}

		float Read()
		{
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_Start);
			return (float)duration.count() / 1000.f;
		}

	private:
		std::chrono::time_point<std::chrono::steady_clock> m_Start;
	};

}}