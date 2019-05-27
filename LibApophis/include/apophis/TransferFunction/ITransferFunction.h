#pragma once

#include "apophis/apophistypes.h"

#define DEFAULT_TRANSFER public: static const ITransferFunction& Default();

namespace Apophis { namespace TransferFunction {

	class ITransferFunction
	{
	public:
		ITransferFunction(const char* name) : m_Name(name) {}
		virtual ~ITransferFunction() {}

		const char* GetName() const { return m_Name; }

		virtual real operator()(real value) const = 0;
		virtual real Derivative(real value) const = 0;

		static const ITransferFunction& Get(const char* name);

	private:
		const char* m_Name;
	};

}}