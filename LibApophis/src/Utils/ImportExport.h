#pragma once

namespace Apophis { namespace Utils {

	// A helper to wrap around RapidJSON types in a way that's safe for public header file forward declarions
	class ExportTarget
	{
	public:
		ExportTarget(rapidjson::Type type, rapidjson::MemoryPoolAllocator<>& allocator) :
			Target(type),
			Allocator(allocator)
		{

		}

		ExportTarget Create(rapidjson::Type type)
		{
			return ExportTarget(type, Allocator);
		}

		template <typename T>
		void AddMember(rapidjson::GenericValue<rapidjson::UTF8<>>::StringRefType name, T value)
		{
			Target.AddMember(name, value, Allocator);
		}

		void AddMember(rapidjson::GenericValue<rapidjson::UTF8<>>::StringRefType name, ExportTarget& value)
		{
			Target.AddMember(name, value.Target, Allocator);
		}

		void AddMember(rapidjson::GenericValue<rapidjson::UTF8<>>::StringRefType name, const char* value)
		{
			rapidjson::Value v(rapidjson::kStringType);
			v.SetString(value, Allocator);
			Target.AddMember(name, v, Allocator);
		}

		void Reserve(size_t size)
		{
			Target.Reserve((rapidjson::SizeType)size, Allocator);
		}

		template <typename T>
		void PushBack(T value)
		{
			Target.PushBack(value, Allocator);
		}

		void PushBack(rapidjson::Value& value)
		{
			Target.PushBack(value, Allocator);
		}

		void PushBack(ExportTarget& value)
		{
			Target.PushBack(value.Target, Allocator);
		}

		rapidjson::Value Target;
		rapidjson::MemoryPoolAllocator<>& Allocator;
	};

}}