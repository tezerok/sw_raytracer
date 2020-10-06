#pragma once
#include "Vec.h"
#include <optional>
#include <cassert>

class Camera
{
public:
	Camera(vec3f pos, vec3f dir, float focalLength) :
		pos(pos), focalLength(focalLength)
	{
		setDir(dir);
	}

	auto getDir() const { return dir; }
	void setDir(vec3f newDir) { assert(lengthSqr(newDir) != 0); dir = normalized(newDir); }

	vec3f pos;
	float focalLength;

private:
	vec3f dir; // normalized
};
std::array<vec3f, 3> getAxes(const Camera& camera);

struct CameraSpan
{
	float left;
	float right;
	float bottom;
	float top;
};

template <typename SceneType, int maxDepth>
RgbColor traceRay(const Ray& ray, const SceneType& scene, int branchFactor, int depth)
{
	if (depth > maxDepth)
		return {0, 0, 0}; // background color

	// Primary ray
	const auto hit = scene.findFirstHit(ray);
	if (hit)
	{
		const auto& obj = *hit->obj;
		const vec3f hitpos = hit->pos;
		const Material material = getMaterial(obj, hitpos);
		const vec3f normal = getNormal(obj, hitpos);
		const float intensity = -(ray.dir * normal);
		RgbColor color = intensity*material.color*0.8f;

		// Secondary rays
		if (depth + 1 <= maxDepth)
		{
			for (int i = 0; i < branchFactor; ++i)
			{
				const vec3f jitterDir = {(rand()%1001-500)/500.f, (rand()%1001-500)/500.f, (rand()%1001-500)/500.f};
				const vec3f reflectedDir = ray.dir - (ray.dir * normal)*2*normal;
				const vec3f newDir = normalized(material.roughness*jitterDir + reflectedDir);
				const Ray secondaryRay = {hitpos + newDir*0.01, newDir};
				const RgbColor secondaryColor = traceRay<SceneType, maxDepth>(secondaryRay, scene, branchFactor, depth+1);

				color = color + material.reflectivity * secondaryColor / branchFactor;
			}
		}

		return clamp(color, {0, 0, 0}, {1, 1, 1});
	}
	else
		return {0, 0, 0};
}

template <typename SceneType, typename ScreenType>
void render(const SceneType& scene, ScreenType& screen, const Camera& camera, std::optional<CameraSpan> span = std::nullopt)
{
	const auto screenW = screen.getW();
	const auto screenH = screen.getH();
	if (!span) {
		float ratio = float(screenW)/screenH;
		span = {-ratio, ratio, -1, 1};
	}
	const float xScale = span->right - span->left;
	const float yScale = span->top - span->bottom;

	const vec3f origin = camera.pos;
	const auto axes = getAxes(camera);
	const vec3f xAxis = axes[0];
	const vec3f yAxis = axes[1];
	const vec3f zAxis = axes[2];

	// Per-pixel ray tracing
	for (int y = 0; y < screenH; ++y)
	{
		for (int x = 0; x < screenW; ++x)
		{
			const vec3f dir = normalized(
				xAxis*(span->left + xScale*x/screenW) +
				yAxis*(span->bottom + yScale*y/screenH) +
				-zAxis*camera.focalLength
			);
			const Ray ray = {origin, dir};

			screen.putPixel(vec2i{x, y}, traceRay<SceneType, 4>(ray, scene, 3, 0));
		}
	}
}

std::array<vec3f, 3> getAxes(const Camera& camera)
{
	std::array<vec3f, 3> axes;

	// Looking up
	if (camera.getDir().y > 0.999) {
		axes[0] = vec3f{1, 0, 0};
		axes[1] = vec3f{0, 0, -1};
		axes[2] = vec3f{0, 1, 0};
	}
	// Looking down
	else if (camera.getDir().y < -0.999) {
		axes[0] = vec3f{1, 0, 0};
		axes[1] = vec3f{0, 0, 1};
		axes[2] = vec3f{0, -1, 0};
	}
	// Any other direction
	else {
		axes[0] = camera.getDir() ^ vec3f{0, 1, 0};
		axes[1] = axes[0] ^ camera.getDir();
		axes[2] = -camera.getDir();
	}

	return axes;
}
