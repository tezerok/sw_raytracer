#pragma once
#include <algorithm> // clamp

template<typename T>
struct Vec2
{
	T x = {};
	T y = {};
};

using vec2f = Vec2<float>;
using vec2i = Vec2<int>;

template<typename T> bool operator==(const Vec2<T>& a, const Vec2<T>& b) { return a.x == b.x && a.y == b.y; }
template<typename T> float operator*(const Vec2<T>& a, const Vec2<T>& b) { return a.x*b.x + a.y*b.y; }
template<typename T> Vec2<T> operator*(float t, const Vec2<T>& v) { return Vec2<T>{t*v.x, t*v.y}; }
template<typename T> Vec2<T> operator*(const Vec2<T>& v, float t) { return Vec2<T>{t*v.x, t*v.y}; }
template<typename T> Vec2<T> operator/(const Vec2<T>& v, float t) { return Vec2<T>{v.x/t, v.y/t}; }
template<typename T> Vec2<T> operator+(const Vec2<T>& a, const Vec2<T>& b) { return Vec2<T>{a.x+b.x, a.y+b.y}; }
template<typename T> Vec2<T> operator-(const Vec2<T>& a, const Vec2<T>& b) { return Vec2<T>{a.x-b.x, a.y-b.y}; }
template<typename T> Vec2<T> operator-(const Vec2<T>& v) { return Vec2<T>{-v.x, -v.y}; }
template<typename T> float length(const Vec2<T>& v) { return sqrt(v.x*v.x+v.y*v.y); }
template<typename T> float lengthSqr(const Vec2<T>& v) { return v.x*v.x+v.y*v.y; }
template<typename T> Vec2<T> normalized(const Vec2<T>& v) { assert(length(v) > 0); return v / length(v); }
template<typename T> Vec2<T> clamp(const Vec2<T>& v, const Vec2<T>& low, const Vec2<T>& high) {
	return Vec2<T>{std::clamp(v.x, low.x, high.x), std::clamp(v.y, low.y, high.y)};
}

template<typename T>
struct Vec3
{
	T x = {};
	T y = {};
	T z = {};
};

using vec3f = Vec3<float>;
using vec3i = Vec3<int>;

template<typename T> bool operator==(const Vec3<T>& a, const Vec3<T>& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
template<typename T> float operator*(const Vec3<T>& a, const Vec3<T>& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
template<typename T> Vec3<T> operator*(float t, const Vec3<T>& v) { return Vec3<T>{t*v.x, t*v.y, t*v.z}; }
template<typename T> Vec3<T> operator*(const Vec3<T>& v, float t) { return Vec3<T>{t*v.x, t*v.y, t*v.z}; }
template<typename T> Vec3<T> operator/(const Vec3<T>& v, float t) { return Vec3<T>{v.x/t, v.y/t, v.z/t}; }
template<typename T> Vec3<T> operator+(const Vec3<T>& a, const Vec3<T>& b) { return Vec3<T>{a.x+b.x, a.y+b.y, a.z+b.z}; }
template<typename T> Vec3<T> operator-(const Vec3<T>& a, const Vec3<T>& b) { return Vec3<T>{a.x-b.x, a.y-b.y, a.z-b.z}; }
template<typename T> Vec3<T> operator-(const Vec3<T>& v) { return Vec3<T>{-v.x, -v.y, -v.z}; }
template<typename T> Vec3<T> operator^(const Vec3<T>& a, const Vec3<T>& b) { return Vec3<T>{a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x}; }
template<typename T> float length(const Vec3<T>& v) { return sqrt(v.x*v.x+v.y*v.y+v.z*v.z); }
template<typename T> float lengthSqr(const Vec3<T>& v) { return v.x*v.x+v.y*v.y+v.z*v.z; }
template<typename T> Vec3<T> normalized(const Vec3<T>& v) { assert(length(v) > 0); return v / length(v); }
template<typename T> Vec3<T> clamp(const Vec3<T>& v, const Vec3<T>& low, const Vec3<T>& high) {
	return Vec3<T>{std::clamp(v.x, low.x, high.x), std::clamp(v.y, low.y, high.y), std::clamp(v.z, low.z, high.z)};
}
