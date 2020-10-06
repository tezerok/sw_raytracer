#pragma once
#include "SDLWindow.h"
#include "Vec.h"
#include "Color.h"

// SDL implementation of the Screen concept
// Used as a render target for a graphical window.
class SDLScreen
{
public:
	explicit SDLScreen(SDLWindow* sdlWindow) :
		sdlWindow(sdlWindow)
	{}

	auto getW() const { return sdlWindow->getW(); }
	auto getH() const { return sdlWindow->getH(); }

	void clear();
	void putPixel(vec2i pos, RgbColor col);

private:
	SDLWindow* sdlWindow;
};

void SDLScreen::clear()
{
	auto* renderer = &sdlWindow->getSDLRenderer();
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
}

void SDLScreen::putPixel(vec2i pos, RgbColor col)
{
	auto* renderer = &sdlWindow->getSDLRenderer();
	SDL_SetRenderDrawColor(renderer, 255*col.x, 255*col.y, 255*col.z, 255);
	SDL_RenderDrawPoint(renderer, pos.x, getH() - pos.y);
}
