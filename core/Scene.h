#pragma once

#include "Color.h"

#include "../math/Vec3.h"

#include <vector>
#include <memory>

template<typename T>
class Scene;

template<typename T>
class IGeometry;

template<typename T>
class IColorizer;

template<typename T>
class OpticalProperties;

template<typename T>
class Scene;

template<typename T>
class Camera;

template<typename T>
class Viewport;

template<typename T>
class Object
{
	friend class Scene<T>;

private:
	Object(IGeometry<T>* geometry, IColorizer<T>* colorizer, OpticalProperties<T>* opticalProperties) :
		m_geometry(geometry), m_colorizer(colorizer), m_opticalProperties(opticalProperties)
	{}

	const IGeometry<T>& getGeometry() const { return *m_geometry; }
	const IColorizer<T>& getColorizer() const { return *m_colorizer; }
	const OpticalProperties<T>& getOpticalProperties() const { return *m_opticalProperties; }

private:
	std::unique_ptr<IGeometry<T> > m_geometry;
	std::unique_ptr<IColorizer<T> > m_colorizer;
	std::unique_ptr<OpticalProperties<T> > m_opticalProperties;
};

template<typename T>
class Scene
{
public:
	void addObject(IGeometry<T>* geometry, IColorizer<T>* colorizer, OpticalProperties<T>* opticalProperties);
	Color<T> renderRay(const Ray<T>& ray);
	void render(const Camera<T>& camera, Viewport<T>& viewport);

private:
	std::vector<std::unique_ptr<Object<T> > > m_objects;
};

#include "Geometry.h"
#include "Colorizer.h"
#include "OpticalProperties.h"
#include "Viewport.h"
#include "Camera.h"


template<typename T>
void Scene<T>::addObject(IGeometry<T>* geometry, IColorizer<T>* colorizer, OpticalProperties<T>* opticalProperties)
{
	Object<T>* obj = new Object<T>(geometry, colorizer, opticalProperties);
	m_objects.push_back(std::unique_ptr<Object<T> >(obj));
}

template<typename T>
Color<T> Scene<T>::renderRay(const Ray<T>& ray)
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

template<typename T>
void Scene<T>::render(const Camera<T>& camera, Viewport<T>& viewport)
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
