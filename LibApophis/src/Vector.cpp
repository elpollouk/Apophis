#include "stdafx.h"
#include "apophis/Vector.h"
#include <memory.h>

using namespace Apophis;

static real* AllocArray(size_t size)
{
	return (real*)malloc(sizeof(real) * size);
}

Vector::Vector() noexcept :
	m_Size(0),
	m_Data(nullptr)
{

}

Vector::Vector(std::initializer_list<real> data) noexcept :
	m_Size(data.size()),
	m_Data(AllocArray(m_Size))
{
	size_t i = 0;
	for (auto v : data)
		m_Data[i++] = v;
}

Vector::Vector(size_t size, const real* data) noexcept :
	m_Size(size),
	m_Data(AllocArray(m_Size))
{
	for (size_t i = 0; i < m_Size; i++)
		m_Data[i] = data[i];
}

Vector::Vector(size_t size) noexcept :
	m_Size(size),
	m_Data(AllocArray(m_Size))
{
}

Vector::Vector(size_t size, real initialValue) noexcept : Vector(size)
{
	for (size_t i = 0; i < m_Size; i++)
		m_Data[i] = initialValue;
}

Vector::Vector(const Vector& rhs) noexcept : Vector(rhs.m_Size, rhs.m_Data)
{

}

Vector::Vector(Vector&& rhs) noexcept :
	m_Size(std::exchange(rhs.m_Size, 0)),
	m_Data(std::exchange(rhs.m_Data, nullptr))
{

}

Vector::~Vector()
{
	if (m_Data) free(m_Data);
}

Vector& Vector::operator=(const Vector& rhs) noexcept
{
	if (m_Size != rhs.size())
	{
		m_Size = rhs.size();
		m_Data = (real*)realloc(m_Data, sizeof(real) * m_Size);
	}

	for (size_t i = 0; i < m_Size; i++)
		m_Data[i] = rhs[i];

	return *this;
}

Vector& Vector::operator=(Vector&& rhs) noexcept
{
	if (m_Data) free(m_Data);
	m_Size = std::exchange(rhs.m_Size, 0);
	m_Data = std::exchange(rhs.m_Data, nullptr);

	return *this;
}

void Vector::resize(size_t newSize) noexcept
{
	m_Data = (real*)realloc(m_Data, sizeof(real) * newSize);
	m_Size = newSize;
}

void Vector::resize(size_t newSize, real newValue) noexcept
{
	auto originalSize = m_Size;
	resize(newSize);

	if (originalSize < newSize)
		for (size_t i = originalSize; i < newSize; i++)
			m_Data[i] = newValue;
}

real Vector::max() const
{
	return m_Data[argmax()];
}

size_t Vector::argmax() const
{
	if (size() <= 0) throw ApophisException("Vector has no elements");

	size_t max = 0;
	for (size_t i = 1; i < size(); i++)
		if (m_Data[max] < m_Data[i])
			max = i;
	return max;
}

Vector Vector::OneHot(size_t size, size_t hotElement)
{
	Vector v(size, 0.f);
	if (0 <= hotElement && hotElement < size)
		v[hotElement] = 1.f;

	return v;
}