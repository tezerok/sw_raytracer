#pragma once
#include "Vec.h"
#include <optional>
#include <variant>
#include <utility>

// ---------------------
// - Renderable shapes -
// ---------------------

struct Sphere
{
	vec3f pos;
	float radius;
};

struct Triangle
{
	vec3f verts[3];
};

struct Mesh
{};

using Shape = std::variant<Sphere, Triangle, Mesh>;

vec3f getNormal(const Sphere& sphere, vec3f p)
{
	if (p == sphere.pos)
		return {};
	return normalized(p - sphere.pos);
}

vec3f getNormal(const Triangle& tri)
{
	return normalized((tri.verts[1]-tri.verts[0]) ^ (tri.verts[2]-tri.verts[1]));
}

vec3f getNormal(const Triangle& tri, vec3f p)
{
	return getNormal(tri);
}

vec3f getNormal(const Mesh& mesh, vec3f p)
{
	return {};
}

// -------------------------------
// - General geometrical objects -
// -------------------------------

struct Ray
{
	vec3f origin;
	vec3f dir;
};

std::optional<std::pair<vec3f, float>> findIntersection(const Ray& ray, const Sphere& sphere)
{
	const auto& o = ray.origin;
	const auto& d = ray.dir;
	const auto& s = sphere.pos;
	const auto& r = sphere.radius;
	const auto dd = d*d;
	const auto oms = (o-s);
	const auto omsSqr = oms*oms;
	const auto doms = d*oms;

	float D = 4.f*doms*doms - 4.f*dd*(omsSqr-r*r);
	if (D < 0.f)
		return std::nullopt;

	float sqrtD = sqrt(D);
	float t1 = (-2.f*d*oms + sqrtD) / (2.f*dd);
	float t2 = (-2.f*d*oms - sqrtD) / (2.f*dd);

	float tmin = std::min(t1, t2);
	if (tmin >= 0.f)
	{
		const auto hitpoint = o + tmin*d;
		return std::make_pair(hitpoint, tmin);
	}

	return std::nullopt;
}

std::optional<std::pair<vec3f, float>> findIntersection(const Ray& ray, const Triangle& tri)
{
	auto n = getNormal(tri);
	auto connector = tri.verts[0] - ray.origin;
	auto speed = -(n * ray.dir);
	auto dist = -(connector * n);
	auto t = dist / speed;
	if (t < 0.f)
		return std::nullopt;

	// Find point on plane, check whether it's in triangles's boundaries
	vec3f p = ray.origin + t*ray.dir;
	vec3f es[] = {
		tri.verts[1] - tri.verts[0],
		tri.verts[2] - tri.verts[1],
		tri.verts[0] - tri.verts[2]
	};
	vec3f ns[] = {
		es[0] ^ n,
		es[1] ^ n,
		es[2] ^ n
	};
	vec3f ps[] = {
		p - tri.verts[0],
		p - tri.verts[1],
		p - tri.verts[2]
	};

	if (ps[0] * ns[0] <= 0.f &&
		ps[1] * ns[1] <= 0.f &&
		ps[2] * ns[2] <= 0.f)
	{
		return std::make_pair(p, t);
	}

	return std::nullopt;
}

std::optional<std::pair<vec3f, float>> findIntersection(const Ray& ray, const Mesh& mesh)
{
	return std::nullopt;
}
