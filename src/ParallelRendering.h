#pragma once
#include "Rendering.h"
#include "SWScreen.h"
#include "ThreadPool.h"
#include <vector>
#include <future>
#include <optional>

template <typename SceneType, typename ScreenType>
void parallelRender(const SceneType& scene, ScreenType& screen, const Camera& camera,
	ThreadPool& threadPool, int regionCount, std::optional<CameraSpan> span = std::nullopt)
{
	std::vector<std::promise<SWScreen>> screenPromises{unsigned(regionCount)}; // note: unfortunately can't be stored in the task itself (std::function must be copyable)
	std::vector<std::future<SWScreen>> screenFutures;
	screenFutures.reserve(regionCount);

	const auto regionW = screen.getW();
	const auto regionH = screen.getH() / regionCount;
	if (!span) {
		float ratio = float(screen.getW())/screen.getH();
		span = {-ratio, ratio, -1, 1};
	}

	// Render to SW screens in parallel, then copy to real (SDL) screen
	for (int region = 0; region < regionCount; ++region)
	{
		// Region to be rendered by a worker thread
		CameraSpan regionSpan = {
			span->left,
			span->right,
			span->bottom + region*(span->top-span->bottom)/regionCount,
			span->bottom + (region+1)*(span->top-span->bottom)/regionCount
		};

		// Render in parallel
		auto& screenPromise = screenPromises[region];
		screenFutures.push_back(screenPromise.get_future());
		threadPool.addTask(
			[&screenPromise, &scene, &camera, regionSpan, regionW, regionH]
			{
				SWScreen swScreen{regionW, regionH};
				render(scene, swScreen, camera, regionSpan);
				screenPromise.set_value(std::move(swScreen));
			}
		);
	}

	// Copy SW screens to output screen
	for (int region = 0; region < regionCount; ++region)
	{
		auto swScreen = screenFutures[region].get();

		for (int y = 0; y < regionH; ++y)
		{
			for (int x = 0; x < regionW; ++x)
			{
				auto pixel = swScreen.getPixel({x, y});
				screen.putPixel({x, region*regionH+y}, pixel);
			}
		}
	}
}
