#include "SDLWindow.h"
#include "SDLScreen.h"
#include "BasicScene.h"
#include "ParallelRendering.h"
#include "ThreadPool.h"
#include <vector>
#include <chrono>
#include <ratio>
#include <iostream>

class ExampleScene {
public:
	ExampleScene();
	void update(); // simple animations

	const BasicScene& getScene() const { return basicScene; }

private:
	BasicScene basicScene;
	float time = 0.f; // used in animation
};

void waitForEvents();
void pollEvents();

// Measurement utility
template <typename RatioType, typename CallableType, typename ...Args>
void timedCall(std::string_view msg, CallableType&& callable, Args&&... args)
{
	auto beginTimepoint = std::chrono::high_resolution_clock::now();
	callable(args...);
	auto endTimepoint = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration<double, RatioType>{endTimepoint - beginTimepoint};
	std::cout << msg << duration.count() << "\n";
}

int main()
{
	SDLWindow sdlWindow{"SW raytracer", 1280, 780};
	SDLScreen sdlScreen{&sdlWindow};

	Camera camera{{0, 4, 0}, {0, -0.55, -1}, 1};
	ExampleScene scene;

	ThreadPool threadPool;

	bool isStatic = true;
	// Static image
	if (isStatic)
	{
		//parallelRender(scene, sdlScreen, camera, threadPool, 8);
		timedCall<std::ratio<1>>("parallel render [seconds]: ",
			&parallelRender<decltype(scene.getScene()), decltype(sdlScreen)>,
			scene.getScene(), sdlScreen, camera, threadPool, 8, std::nullopt);
		sdlWindow.swapBuffers();
		waitForEvents();
	}
	// Animated scene
	else
	{
		while (true)
		{
			pollEvents();
			sdlScreen.clear();

			parallelRender(scene.getScene(), sdlScreen, camera, threadPool, 8);
			scene.update();

			sdlWindow.swapBuffers();
		}
	}
}

void pollEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			exit(0);
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			exit(0);
	}
}

void waitForEvents()
{
	SDL_Event event;
	while (SDL_WaitEvent(&event))
	{
		if (event.type == SDL_QUIT)
			exit(0);
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			exit(0);
	}
}

ExampleScene::ExampleScene() :
	basicScene{{
	Object{
		Sphere{-5, 2.0, -10, 2.5},
		[] (const Object& obj, vec3f p) {
			return Material{{0.8,0.8,0.8}, 0.8, 0};
		}
	},
	Object{
		Sphere{5, 2.0, -10, 2.5},
		[] (const Object& obj, vec3f p) {
			const Sphere& sphere = std::get<Sphere>(obj.shape);
			//float xt = (p.x-sphere.pos.x) / sphere.radius;
			float yt = (p.y-sphere.pos.y) / sphere.radius;
			RgbColor color = {
				0.5f+0.5f*sin(yt*3.14159f*20),
				0.2f,
				0.2f
			};
			return Material{color, 0.8};
		}
	},
	Object{
		Sphere{0, 0.8, -7, 1.5},
		[] (const Object& obj, vec3f p) {
			return Material{{0.8,0.0,0.0}, 0.8, 0};
		}
	},
	Object{
		Sphere{2, 0.5, -5, 0.8},
		[] (const Object& obj, vec3f p) {
			return Material{{0.8,0.8,0.0}, 0.8};
		}
	},
	Object{
		Sphere{-2, 0.5, -5, 0.8},
		[] (const Object& obj, vec3f p) {
			return Material{{0.0,0.8,0.0}, 0.8};
		}
	},
	// Floor triangle 1
	Object{
		Triangle{{
				{-16, -0.7,  16},
				{ 16, -0.7,  16},
				{-16, -0.7, -16}
		}},
		[] (const Object& obj, vec3f p) {
			return Material{{0.6f, 0.6f, 0.6f}, 0.8f};
		}
	},
	// Floor triangle 2
	Object{
		Triangle{{
				{-16, -0.7, -16},
				{ 16, -0.7,  16},
				{ 16, -0.7, -16}
		}},
		[] (const Object& obj, vec3f p) {
			return Material{{0.6f, 0.6f, 0.6f}, 0.8f};
		}
	}
}}
{}

void ExampleScene::update()
{
	std::get<Sphere>(basicScene.objects[0].shape).pos.y += cos(time)*0.2f;
	std::get<Sphere>(basicScene.objects[1].shape).pos.y += cos(time+4.8f)*0.1f;
	std::get<Sphere>(basicScene.objects[2].shape).pos.y += cos(time+4.8f)*0.1f;
	time += 0.1f;
}
