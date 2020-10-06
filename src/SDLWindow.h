#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <cassert>

class SDLWindow
{
public:
	explicit SDLWindow(std::string name = "unnamed window", int w = 640, int h = 480, bool fullscreen = false);

	SDLWindow(const SDLWindow&) = delete;
	SDLWindow(SDLWindow&&) noexcept;
	SDLWindow& operator=(SDLWindow) noexcept;
	~SDLWindow();
	friend void swap(SDLWindow&, SDLWindow&) noexcept;

	auto getSDLWindow() const -> const auto& { assert(sdlWindow); return *sdlWindow; }
	auto getSDLRenderer() const -> const auto& { assert(sdlRenderer); return *sdlRenderer; }
	auto getW() const { return w; }
	auto getH() const { return h; }

	auto getSDLWindow() -> auto& { assert(sdlWindow); return *sdlWindow; }
	auto getSDLRenderer() -> auto& { assert(sdlRenderer); return *sdlRenderer; }

	void swapBuffers();

private:
	SDL_Window* sdlWindow; // never null
	SDL_Renderer* sdlRenderer; // never null
	std::string name;
	int w;
	int h;
	bool fullscreen;
};

#include <stdexcept>
struct WindowInitException : std::runtime_error
{
	explicit WindowInitException(const std::string& msg) :
		std::runtime_error(msg)
	{}
};

// .cpp
SDLWindow::SDLWindow(std::string name, int w, int h, bool fullscreen) :
	name{std::move(name)},
	w{w},
	h{h},
	fullscreen{fullscreen}
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS) < 0)
		throw WindowInitException("Can't initialize SDL");

	sdlWindow = SDL_CreateWindow(this->name.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		w, h,
		(fullscreen ? SDL_WINDOW_FULLSCREEN : 0));
	if (sdlWindow == NULL)
		throw WindowInitException("Can't create SDL_Window");

	sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
}

SDLWindow::SDLWindow(SDLWindow&& x) noexcept :
	SDLWindow()
{
	swap(x, *this);
}

SDLWindow& SDLWindow::operator=(SDLWindow x) noexcept
{
	swap(x, *this);
	return *this;
}

SDLWindow::~SDLWindow()
{
	SDL_DestroyRenderer(sdlRenderer);
	SDL_DestroyWindow(sdlWindow);
	SDL_Quit();
}

void swap(SDLWindow& a, SDLWindow& b) noexcept
{
	using std::swap;
	swap(a.sdlWindow, b.sdlWindow);
	swap(a.sdlRenderer, b.sdlRenderer);
	swap(a.name, b.name);
	swap(a.w, b.w);
	swap(a.h, b.h);
	swap(a.fullscreen, b.fullscreen);
}

void SDLWindow::swapBuffers()
{
	SDL_RenderPresent(sdlRenderer);
}
