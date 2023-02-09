#pragma once

#include "../math/Vec3.h"

#include <vector>
#include <memory>

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
		T verticalFovAngle = 2. * std::atan(T(viewport.getHeight()) / T(viewport.getWidth()) *std::tan(m_horizontalFovAngle / 2.));
				
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

template<typename T>
struct Color
{
	T r;
	T g;
	T b;
};

template<typename T>
class Viewport
{
public:
	Viewport(int width, int height) : m_width(width), m_height(height), m_pixels(width * height)
	{}

	const int& getHeight() const { return m_height; }
	const int& getWidth() const { return m_width; }

	Color<T>& operator()(int col, int row) { return m_pixels[col + row * m_width]; }
	const Color<T>& operator()(int col, int row) const { return m_pixels[col + row * m_width]; }

private:
	int m_width;
	int m_height;
	std::vector<Color<T> > m_pixels;
};

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

template<typename T>
class OpticalProperties
{};

template<typename T>
class Scene;

template<typename T>
class Object
{
	friend class Scene<T>;

private:
	Object(IGeometry<T>* geometry, IColorizer<T>* colorizer, const OpticalProperties<T>& opticalProperties) :
		m_geometry(geometry), m_colorizer(colorizer), m_opticalProperties(opticalProperties)
	{}

	const IGeometry<T>& getGeometry() const { return *m_geometry; }
	const IColorizer<T>& getColorizer() const { return *m_colorizer; }
	const OpticalProperties<T> getOpticalProperties() const { return m_opticalProperties; }

private:
	std::unique_ptr<IGeometry<T> > m_geometry;
	std::unique_ptr<IColorizer<T> > m_colorizer;
	OpticalProperties<T> m_opticalProperties;
};

template<typename T>
Color<T> NO_INTERSECTION_COLOR() { return Color<T>{0.0, 1.0, 1.0}; }

template<typename T>
class Scene
{
public:
	void addObject(IGeometry<T>* geometry, IColorizer<T>* colorizer, const OpticalProperties<T>& opticalProperties)
	{
		Object<T>* obj = new Object<T>(geometry, colorizer, opticalProperties);
		m_objects.push_back(std::unique_ptr<Object<T> >(obj));
	}

	Color<T> renderRay(const Ray<T>& ray)
	{
		T min = std::numeric_limits<T>::infinity();
		Object<T>* minObj = nullptr;
	    for (auto& obj : m_objects)
	    {
			T intersection = obj->getGeometry().getIntersection(ray);
			if ((intersection != NO_INTERSECTION<T>()) && (min > intersection))
			{
				min = obj->getGeometry().getIntersection(ray);
				minObj = obj.get();
			}
	    }

		if (minObj == nullptr)
			return NO_INTERSECTION_COLOR<T>();

		return minObj->getColorizer().getColor();
	}

	void render(const Camera<T>& camera, Viewport<T>& viewport)
	{
		for (int col = 0; col < viewport.getWidth(); ++col)
		{
			for (int row = 0; row < viewport.getHeight(); ++row)
			{
				auto ray = camera.getRay(viewport, row, col);
				viewport(col, row) = renderRay(ray);
			}
		}
	}

private:
	std::vector<std::unique_ptr<Object<T> > > m_objects;
};
