#include "stdafx.h"
#include "apophis/Training/StoppingConditions.h"

using namespace Apophis;
using namespace Apophis::Training::StoppingCondition;

bool AnyStoppingCondition::Check(Data::Metrics& metrics) const
{
	for (auto& condition : m_Conditions)
		if ((*condition)(metrics))
			return true;

	return false;
}
