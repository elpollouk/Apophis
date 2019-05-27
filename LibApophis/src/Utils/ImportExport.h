#pragma once

namespace Apophis { namespace Utils {

	// Common field and value constants
	constexpr const char* FIELD_TYPE = "type";
	constexpr const char* FIELD_INPUTSIZE = "input_size";
	constexpr const char* FIELD_OUTPUTSIZE = "output_size";
	constexpr const char* FIELD_TRANSFER = "transfer";
	constexpr const char* FIELD_LAYERS = "layers";
	constexpr const char* FIELD_NODES = "nodes";
	constexpr const char* FIELD_WEIGHTS = "weights";

	constexpr const char* COMPONENTTYPE_LAYER = "layer";
	constexpr const char* COMPONENTTYPE_NODE = "node";

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
		void AddMember(const char* name, T value)
		{
			rapidjson::GenericStringRef<rapidjson::UTF8<>::Ch> jName(name);
			Target.AddMember(jName, value, Allocator);
		}

		void AddMember(const char* name, ExportTarget& value)
		{
			rapidjson::GenericStringRef<rapidjson::UTF8<>::Ch> jName(name);
			Target.AddMember(jName, value.Target, Allocator);
		}

		void AddMember(const char* name, const char* value)
		{
			rapidjson::GenericStringRef<rapidjson::UTF8<>::Ch> jName(name);
			rapidjson::Value v(rapidjson::kStringType);
			v.SetString(value, Allocator);
			Target.AddMember(jName, v, Allocator);
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