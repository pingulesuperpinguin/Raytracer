#pragma once

#include "../math/Vec3.h"

#include "Color.h"

template<typename T>
class ILight
{
public:
	virtual ~ILight() = default;

	virtual Color<T> getLightAtContact(const Contact<T>& contact) const = 0;
};

template<typename T>
class PointLight : public ILight<T>
{
public:
	PointLight(const Point3<T>& origin, const Color<T>& color, T intensity) : m_origin(origin), m_color(color), m_intensity(intensity)
	{}

	virtual Color<T> getLightAtContact(const Contact<T>& contact) const override
	{
		T dist = std::sqrt((contact.point - m_origin) * (contact.point - m_origin));
		Vec3<T> lightVec = (contact.point - m_origin).getNormalized();
		return (m_intensity / dist / dist) * (contact.normal * lightVec) * m_color;
	}

private:
	Point3<T> m_origin;
	Color<T> m_color;
	T m_intensity;
};

