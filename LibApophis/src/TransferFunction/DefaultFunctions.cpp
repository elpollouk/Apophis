#include "stdafx.h"
#include <string.h>
#include "apophis/ApophisException.h"
#include "apophis/TransferFunction/Relu.h"
#include "apophis/TransferFunction/Sigmoid.h"

using namespace Apophis::TransferFunction;

#define IMPLEMENT_DEFAULT(function) static function s_Default ## function; \
const ITransferFunction& function::Default() { return s_Default ## function; }

IMPLEMENT_DEFAULT(Relu)
IMPLEMENT_DEFAULT(Sigmoid)

const ITransferFunction& ITransferFunction::Get(const char* name)
{
	if (strcmp(Relu::Name, name) == 0)
	{
		return Relu::Default();
	}
	else if (strcmp(Sigmoid::Name, name) == 0)
	{
		return Sigmoid::Default();
	}

	assert(false);
	throw ApophisException("Unknown transfer type \"%s\"", name);
}