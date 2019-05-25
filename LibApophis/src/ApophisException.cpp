#include "stdafx.h"
#include "apophis/ApophisException.h"
#include <cstdio>

using namespace Apophis;

static void FormatIntoString(std::string& target, const char* message, std::va_list args)
{
	auto messageLength = std::vsnprintf(nullptr, 0, message, args) + 1;
	target.resize(messageLength);
	std::vsnprintf(&target[0], messageLength, message, args);
}

ApophisException::ApophisException(const char* message, ...)
{
	std::va_list args;
	va_start(args, message);
	FormatIntoString(m_WhatMessage, message, args);
	va_end(args);
}

ApophisException::ApophisException(const char* message, std::va_list args)
{
	FormatIntoString(m_WhatMessage, message, args);
}

ApophisException::ApophisException(const ApophisException& rhs) noexcept :
	m_WhatMessage(rhs.m_WhatMessage)
{

}

ApophisException::ApophisException(ApophisException&& rhs) noexcept :
	m_WhatMessage(std::move(rhs.m_WhatMessage))
{

}

ApophisException& ApophisException::operator=(const ApophisException & rhs) noexcept
{
	m_WhatMessage = rhs.m_WhatMessage;
	return *this;
}