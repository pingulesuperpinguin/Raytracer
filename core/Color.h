#pragma once

template<typename T>
struct Color
{
	T r;
	T g;
	T b;
};

template<typename T>
Color<T> operator*(const T& s, const Color<T>& col)
{
	return { s * col.r, s * col.g, s * col.b };
}

template<typename T>
Color<T> operator+(const Color<T>& col1, const Color<T>& col2)
{
	return { col1.r + col2.r, col1.g + col2.g, col1.b + col2.b };
}

template<typename T>
Color<T> NO_INTERSECTION_COLOR() { return Color<T>{0.0, 1.0, 1.0}; }

