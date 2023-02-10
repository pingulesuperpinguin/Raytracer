#pragma once

template<typename T>
struct Color
{
	T r;
	T g;
	T b;
};

template<typename T>
Color<T> NO_INTERSECTION_COLOR() { return Color<T>{0.0, 1.0, 1.0}; }

