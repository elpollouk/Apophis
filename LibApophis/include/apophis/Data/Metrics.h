#pragma once

#include <string>
#include <map>

namespace Apophis { namespace Data {

	constexpr const char* METRIC_TRAINING_ITERATIONS = "training/num_iterations";
	constexpr const char* METRIC_TRAINING_LOSS = "training/loss";
	constexpr const char* METRIC_TRAINING_TIME = "training/time";

	class Metrics
	{
	private:
		union Value
		{
			double dValue;
			long long iValue;
		};

	public:
		template<class DType>
		void Set(const std::string key, DType value) { static_assert(false, "Unsupported metric data type"); }
		template<class DType>
		DType Add(const std::string key, DType value) { static_assert(false, "Unsupported metric data type"); }
		template<class DType>
		DType Get(const std::string key) const { static_assert(false, "Unsupported metric data type"); }

		//---------------------------------------------------------------------------------------------//
		// Double
		//---------------------------------------------------------------------------------------------//
		template<>
		void Set<double>(const std::string key, double value)
		{
			Get(key).dValue = value;
		}

		template<>
		double Add<double>(const std::string key, double value)
		{
			return Get(key).dValue += value;
		}

		template<>
		double Get<double>(const std::string key) const
		{
			return Get(key).dValue;
		}

		//---------------------------------------------------------------------------------------------//
		// Float
		//---------------------------------------------------------------------------------------------//
		template<>
		void Set<float>(const std::string key, float value)
		{
			Get(key).dValue = value;
		}

		template<>
		float Add<float>(const std::string key, float value)
		{
			return (float)(Get(key).dValue += value);
		}

		template<>
		float Get<float>(const std::string key) const
		{
			return (float)Get(key).dValue;
		}

		//---------------------------------------------------------------------------------------------//
		// Int64
		//---------------------------------------------------------------------------------------------//
		template<>
		void Set<long long>(const std::string key, long long value)
		{
			Get(key).iValue = value;
		}

		template<>
		long long Add<long long>(const std::string key, long long value)
		{
			return Get(key).iValue += value;
		}

		template<>
		long long Get<long long>(const std::string key) const
		{
			return Get(key).iValue;
		}

		//---------------------------------------------------------------------------------------------//
		// Int32
		//---------------------------------------------------------------------------------------------//
		template<>
		void Set<int>(const std::string key, int value)
		{
			Get(key).iValue = value;
		}

		template<>
		int Add<int>(const std::string key, int value)
		{
			return (int)(Get(key).iValue += value);
		}

		template<>
		int Get<int>(const std::string key) const
		{
			return (int)Get(key).iValue;
		}

	private:
		Value& Get(const std::string& key)
		{
			auto it = m_Values.find(key);
			if (it == m_Values.end())
			{
				m_Values.emplace(std::make_pair(key, Value({})));
				return m_Values[key];
			}
			return it->second;
		}

		const Value Get(const std::string& key) const
		{
			auto it = m_Values.find(key);
			if (it == m_Values.end())
				return Value({});

			return it->second;
		}

		std::map<const std::string, Value> m_Values;
	};

}}
