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
	constexpr const char* FIELD_EXAMPLES = "examples";
	constexpr const char* FIELD_INPUT = "input";
	constexpr const char* FIELD_OUTPUT = "output";

	constexpr const char* COMPONENTTYPE_NETWORK = "network";
	constexpr const char* COMPONENTTYPE_LAYER = "layer";
	constexpr const char* COMPONENTTYPE_NODE = "node";

}}