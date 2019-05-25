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

		void AddMember(rapidjson::GenericValue<rapidjson::UTF8<>>::StringRefType name, ExportTarget& value)
		{
			Target.AddMember(name, value.Target, Allocator);
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

		rapidjson::Value Target;
		rapidjson::MemoryPoolAllocator<>& Allocator;
	};

}}