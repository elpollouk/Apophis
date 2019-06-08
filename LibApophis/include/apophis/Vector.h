#pragma once

#include "apophis/apophistypes.h"
#include "apophis/ApophisException.h"

namespace Apophis {

	typedef float real;

	class Vector
	{
	public:
		Vector() noexcept;
		Vector(std::initializer_list<real> data) noexcept;
		Vector(size_t size, const real* data) noexcept;
		Vector(size_t size) noexcept;

		Vector(const Vector& rhs) noexcept;
		Vector(Vector&& rhs) noexcept;

		~Vector() noexcept;

		Vector& operator=(const Vector& rhs) noexcept;

		size_t size() const noexcept { return m_Size; }
		real& operator[](size_t index)
		{
#if defined(_DEBUG)
			if (index < 0 || m_Size <= index) throw ApophisException("Index %d out of Vector range %d", (int)index, (int)m_Size);
#endif
			return m_Data[index];
		}

		real operator[](size_t index) const
		{
#if defined(_DEBUG)
			if (index < 0 || m_Size <= index) throw ApophisException("Index %d out of Vector range %d", (int)index, (int)m_Size);
#endif
			return m_Data[index];
		}

		real* begin() noexcept { return m_Data; }
		real* end() noexcept { return m_Data + m_Size; }
		const real* begin() const noexcept { return m_Data; }
		const real* end() const noexcept { return m_Data + m_Size; }

		void resize(size_t newSize) noexcept;
		void resize(size_t newSize, real newValues) noexcept;

	private:
		size_t m_Size;
		real* m_Data;
	};

	typedef Vector& VectorRef;
	typedef const Vector& ConstVectorRef;

}
