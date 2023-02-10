#pragma once

#include "Color.h"

template<typename T>
class IColorizer
{
public:
	virtual Color<T> getColor() const = 0;
	virtual ~IColorizer() = default;
};

template<typename T>
class FlatColorizer : public IColorizer<T>
{
public:
	FlatColorizer(const Color<T>& color) : m_color(color)
	{}

	virtual Color<T> getColor() const override { return m_color; }

private:
	Color<T> m_color;
};
