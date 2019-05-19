#pragma once
#include <vector>

namespace Apophis {

	typedef float real;
	typedef std::vector<real> Vector;
	typedef Vector& VectorRef;
	typedef const Vector& ConstVectorRef;

	namespace Component {
		class Layer;
		class Network;
		class Node;
	}
	namespace Training {
		typedef real(*LossFunction)(ConstVectorRef target, ConstVectorRef actual);
		class Evaluator;
		class IStoppingCondition;
		class ITrainable;
		class Trainer;
	}
	namespace TransferFunction {
		class ITransferFunction;
	}
	class ExampleSet;
}
