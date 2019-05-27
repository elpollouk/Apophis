#pragma once
#include <exception>
#include <string>
#include <cstdarg>

namespace Apophis {

	class ApophisException : public std::exception
	{
	public:
		ApophisException(const char* message, ...);
		ApophisException(const char* message, std::va_list args);
		ApophisException(const ApophisException& rhs) noexcept;
		ApophisException(ApophisException&& rhs) noexcept;
		ApophisException& operator=(const ApophisException& rhs) noexcept;

		virtual char const* what() const override { return m_WhatMessage.c_str(); }

	private:
		std::string m_WhatMessage;
	};

}
