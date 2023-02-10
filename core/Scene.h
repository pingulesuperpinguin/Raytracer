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
class ILight;

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
	void addLight(ILight<T>* light);
	Color<T> renderRay(const Ray<T>& ray);
	void render(const Camera<T>& camera, Viewport<T>& viewport);

private:
	std::vector<std::unique_ptr<Object<T> > > m_objects;
	std::vector<std::unique_ptr<ILight<T> > > m_lights;
};

#include "Geometry.h"
#include "Colorizer.h"
#include "OpticalProperties.h"
#include "Viewport.h"
#include "Camera.h"
#include "Light.h"


template<typename T>
void Scene<T>::addObject(IGeometry<T>* geometry, IColorizer<T>* colorizer, OpticalProperties<T>* opticalProperties)
{
	Object<T>* obj = new Object<T>(geometry, colorizer, opticalProperties);
	m_objects.push_back(std::unique_ptr<Object<T> >(obj));
}

template<typename T>
void Scene<T>::addLight(ILight<T>* light)
{
	m_lights.emplace_back(light);
}

template<typename T>
Color<T> Scene<T>::renderRay(const Ray<T>& ray)
{
	Contact<T> minContact = { std::numeric_limits<T>::infinity(), {0., 0., 0.}, {0., 0., 0.} };
	Object<T>* minObj = nullptr;
	for (auto& obj : m_objects)
	{
		Contact<T> intersection = obj->getGeometry().getIntersection(ray);
		if ((intersection.distance != NO_INTERSECTION<T>()) && (minContact.distance > intersection.distance))
		{
			minContact = intersection;
			minObj = obj.get();
		}
	}

	if (minObj == nullptr)
		return NO_INTERSECTION_COLOR<T>();

	auto& op = minObj->getOpticalProperties();

	Color<T> col = minObj->getColorizer().getColor();
	Color<T> finalColor = op.ambient * col;

	for (auto& light : m_lights)
	{
		Color<T> lightColor = op.diffusion * light->getLightAtContact(minContact);
		finalColor = finalColor + Color<T>{ lightColor.r* col.r, lightColor.g* col.g, lightColor.b* col.b};
	}

	return finalColor;
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
