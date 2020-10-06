#pragma once
#include "Vec.h"
#include "Color.h"
#include <vector>
#include <cassert>

// Software implementation of the Screen concept
// Simply writes pixels into a memory buffer.
class SWScreen
{
public:
	explicit SWScreen(int w, int h) :
		w(w),
		h(h),
		pixels(w*h)
	{}

	auto getW() const { return w; }
	auto getH() const { return h; }
	auto getPixel(vec2i pos) const {
		assert(pos.x >= 0 && pos.x < w);
		assert(pos.y >= 0 && pos.y < h);
		return pixels[pos.y*w+pos.x];
	}

	void clear();
	void putPixel(vec2i pos, RgbColor col);

private:
	int w, h;
	std::vector<RgbColor> pixels;
};

void SWScreen::clear()
{
	std::fill(begin(pixels), end(pixels), RgbColor{});
}

void SWScreen::putPixel(vec2i pos, RgbColor col)
{
	pixels[pos.y*w+pos.x] = col;
}
