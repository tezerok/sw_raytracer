#pragma once
#include "Object.h"
#include "Geometry.h"
#include <vector>
#include <optional>

// Basic (non-accelerated) implementation of a scene
// Finding the first ray-object intersection is implemented
//  by performing an intersection check with each object.
struct BasicScene
{
	std::vector<Object> objects;

	std::optional<Hit> findFirstHit(const Ray& ray) const;
};

std::optional<Hit> BasicScene::findFirstHit(const Ray& ray) const
{
	std::optional<Hit> minHit;
	float minT = 0;
	for (const auto& obj : objects)
	{
		auto intersection = std::visit([&ray](const auto& shape) { return findIntersection(ray, shape); }, obj.shape);
		if (intersection) {
			auto& [hitpos, t] = *intersection;
			if (!minHit || t < minT) {
				minT = t;
				minHit = Hit{hitpos, &obj};
			}
		}
	}

	return minHit;
}
