#pragma once

#include "Color.h"

#include <tuple>
#include <vector>

template<typename T>
class Viewport
{
public:
	Viewport(int width, int height) : m_width(width), m_height(height), m_pixels(width* height)
	{}

	int getHeight() const { return m_height; }
	int getWidth() const { return m_width; }

	Color<T>& operator()(int col, int row) { return m_pixels[col + row * m_width]; }
	const Color<T>& operator()(int col, int row) const { return m_pixels[col + row * m_width]; }

private:
	int m_width;
	int m_height;
	std::vector<Color<T> > m_pixels;
};

template<typename T>
std::tuple<T, T> getMinMaxOnViewport(const Viewport<T>& viewport)
{
    T min = std::numeric_limits<T>::infinity();
    T max = -std::numeric_limits<T>::infinity();
    for (int row = 0; row < viewport.getHeight(); ++row)
    {
        for (int col = 0; col < viewport.getWidth(); ++col)
        {
            const Color<T>& c = viewport(col, row);
            min = std::min(min, std::min(c.r, std::min(c.g, c.b)));
            max = std::max(max, std::max(c.r, std::min(c.g, c.b)));
        }
    }
    return { min, max };
}
