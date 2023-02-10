#pragma once

template<typename T>
class IGeometry
{
public:
	virtual ~IGeometry() = default;

	virtual T getIntersection(const Ray<T>& ray) const = 0;
};

template<typename T>
class SphereGeometry : public IGeometry<T>
{
public:
	SphereGeometry(const Point3<T>& origin, const T& radius) : m_sphere{ origin, radius }
	{}

	virtual T getIntersection(const Ray<T>& ray) const override
	{
		return intersect(m_sphere, ray);
	}

private:
	Sphere<T> m_sphere;
};
