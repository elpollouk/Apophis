#pragma once

#include "apophis/apophistypes.h"
#include "apophis/ApophisException.h"
#include "apophis/ExampleSet.h"
#include "apophis/MultiExampleSet.h"
#include "apophis/Random.h"
#include "apophis/Classifier.h"
#include "apophis/Data/Metrics.h"
#include "apophis/Utils/IExportWriter.h"
#include "apophis/Utils/IImportReader.h"
#include "apophis/Utils/Timer.h"

#include "apophis/Component/Node.h"
#include "apophis/Component/Layer.h"
#include "apophis/Component/Network.h"

#include "apophis/TransferFunction/ITransferFunction.h"
#include "apophis/TransferFunction/Relu.h"
#include "apophis/TransferFunction/Sigmoid.h"

#include "apophis/Training/LossFunctions.h"
#include "apophis/Training/StoppingConditions.h"
#include "apophis/Training/Evaluator.h"
#include "apophis/Training/Trainer.h"

#include "apophis/Training/BackPropNetwork.h"
