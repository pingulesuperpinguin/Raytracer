#pragma once

template<typename T>
struct Contact
{
	T distance;
	Point3<T> point;
	Vec3<T> normal;
};

template<typename T>
constexpr Contact<T> NO_CONTACT() { return { -1., {0., 0., 0.}, {0., 0., 0.} }; }

template<typename T>
class IGeometry
{
public:
	virtual ~IGeometry() = default;

	virtual Contact<T> getIntersection(const Ray<T>& ray) const = 0;
};

template<typename T>
class SphereGeometry : public IGeometry<T>
{
public:
	SphereGeometry(const Point3<T>& origin, const T& radius) : m_sphere{ origin, radius }
	{}

	virtual Contact<T> getIntersection(const Ray<T>& ray) const override
	{
		T inter = intersect(m_sphere, ray);
		if (inter == NO_INTERSECTION<T>())
			return NO_CONTACT<T>();

		auto point = ray.getPointAtAbscice(inter);

		return { inter, point, (point - m_sphere.center).getNormalized() };
	}

private:
	Sphere<T> m_sphere;
};
