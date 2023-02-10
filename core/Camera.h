#pragma once

#include "../math/Vec3.h"

template<typename T>
class Viewport;

template<typename T>
class Camera
{
public:
	Camera(const Point3<T>& origin, const Vec3<T>& direction, const T& horizontalFovAngle) : m_origin(origin), m_direction(direction), m_horizontalFovAngle(horizontalFovAngle)
	{}

	Ray<T> getRay(const Viewport<T>& viewport, int row, int col) const
	{
		T verticalFovAngle = 2. * std::atan(T(viewport.getHeight()) / T(viewport.getWidth()) * std::tan(m_horizontalFovAngle / 2.));

		Vec3<T> look = m_direction.getNormalized();
		Vec3<T> right = (look ^ Vec3<T>{0., 1., 0.}).getNormalized();
		Vec3<T> up = look ^ right;

		T L = std::tan(m_horizontalFovAngle / 2.) * 2. * 1.;
		T h = std::tan(verticalFovAngle / 2.) * 2. * 1.;

		T pL = L / T(viewport.getWidth());
		T ph = h / T(viewport.getHeight());

		T dL = T(col) - T(viewport.getWidth()) / 2. + 0.5;
		T dh = T(row) - T(viewport.getHeight()) / 2. + 0.5;

		Vec3<T> dir = look + pL * dL * right + ph * dh * up;

		return Ray<T>{m_origin, dir.getNormalized()};
	}

private:
	Point3<T> m_origin;
	Vec3<T> m_direction;
	T m_horizontalFovAngle;
};
