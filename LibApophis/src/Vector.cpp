#include "stdafx.h"
#include "apophis/Vector.h"

using namespace Apophis;

Vector::Vector() noexcept :
	m_Size(0),
	m_Data(nullptr)
{

}

Vector::Vector(std::initializer_list<real> data) noexcept :
	m_Size(data.size()),
	m_Data(new real[m_Size])
{
	size_t i = 0;
	for (auto v : data)
		m_Data[i++] = v;
}

Vector::Vector(size_t size, const real* data) noexcept :
	m_Size(size),
	m_Data(new real[m_Size])
{
	for (size_t i = 0; i < m_Size; i++)
		m_Data[i] = data[i];
}

Vector::Vector(size_t size) noexcept :
	m_Size(size),
	m_Data(new real[m_Size])
{
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
	if (m_Data) delete[] m_Data;
}

Vector& Vector::operator=(const Vector& rhs) noexcept
{
	if (m_Size != rhs.size())
	{
		delete[] m_Data;
		m_Size = rhs.size();
		m_Data = new real[m_Size];
	}

	for (size_t i = 0; i < m_Size; i++)
		m_Data[i] = rhs[i];

	return *this;
}

void Vector::resize(size_t newSize) noexcept
{
	auto newData = new real[newSize];

	auto copySize = newSize < m_Size ? newSize : m_Size;
	for (size_t i = 0; i < copySize; i++)
		newData[i] = m_Data[i];

	delete[] m_Data;
	m_Size = newSize;
	m_Data = newData;
}

void Vector::resize(size_t newSize, real newValue) noexcept
{
	auto originalSize = m_Size;
	resize(newSize);

	if (originalSize < newSize)
		for (size_t i = originalSize; i < newSize; i++)
			m_Data[i] = newValue;
}
