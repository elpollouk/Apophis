#include "stdafx.h"
#include <iostream>
#include "include/ui.h"

using namespace Apophis;
using namespace Apophis::Data;

std::string TimeToString(double totalSeconds)
{
	auto hours = (int)(totalSeconds / 3600);
	totalSeconds -= (hours * 3600);
	auto minutes = (int)(totalSeconds / 60);
	totalSeconds -= (minutes * 60);
	auto seconds = (int)totalSeconds;
	totalSeconds -= seconds;
	auto ms = (int)(1000 * totalSeconds);

	char buffer[64];
	sprintf_s(buffer, "%02d:%02d:%02d.%03d", hours, minutes, seconds, ms);
	return std::string(buffer);
}

void SampleUtils::UI::OnProgress(Metrics& metrics)
{
	auto totalTime = metrics.Get<double>(METRIC_TRAINING_TIME);
	auto totalIterations = metrics.Get<long long>(METRIC_TRAINING_ITERATIONS);
	auto rate = totalTime == 0. ? 0 : (int)((double)totalIterations / totalTime);
	std::cout << TimeToString(totalTime)
		      << ": Iterations = "
		      << totalIterations
		      << " - Error = "
		      << metrics.Get<real>(METRIC_TRAINING_LOSS)
		      << " (iters/sec = "
		      << rate
		      << ")\n";
}
