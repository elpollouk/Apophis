#pragma once
#include <vector>
#include "apophis/Vector.h"

namespace Apophis {

	namespace Component {
		class Layer;
		class Network;
		class Node;
	}
	namespace Data {
		class Metrics;
	}
	namespace Training {
		typedef real(*LossFunction)(ConstVectorRef target, ConstVectorRef actual);
		class Evaluator;
		class IEvaluator;
		class IStoppingCondition;
		class ITrainable;
		class SampledEvaluator;
		class Trainer;
	}
	namespace Utils {
		class IExportWriter;
		class IImportReader;
		class Timer;
	}
	namespace TransferFunction {
		class ITransferFunction;
	}
	class ApophisException;
	class IExampleProvider;
	class ExampleSet;
	class MultiExampleSet;
}
