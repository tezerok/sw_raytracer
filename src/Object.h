#pragma once
#include "Geometry.h"
#include <functional>

// Visual attributes of a shape
struct Material
{
	RgbColor color = {0.8, 0.8, 0.8};
	float reflectivity = 0.2f;
	float roughness = 0.f;
};

// Shape with a material
struct Object
{
	Shape shape;

	// Returns the material of a point (absolute coords) on the object's surface
	std::function<Material(const Object&, vec3f)> getMaterial;
};

// Ray-object intersection
struct Hit
{
	vec3f pos;
	const Object* obj;
};

Material getMaterial(const Object& obj, vec3f p)
{
	return obj.getMaterial(obj, p);
}

vec3f getNormal(const Object& obj, vec3f p)
{
	return std::visit([p](const auto& shape) { return getNormal(shape, p); }, obj.shape);
}
