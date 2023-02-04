#pragma once
#include <cstddef>
#include <functional>
#include "utils.h"

struct IVec2 {
	int x = 0;
	int y = 0;

	IVec2() { x = 0; y = 0; }
	IVec2(int x, int y) : x(x), y(y) {}

	bool operator==(const IVec2& other) const;
};

struct Vec2 {
	double x = 0;
	double y = 0;

	Vec2() { x = 0; y = 0; }
	Vec2(double x, double y) : x(x), y(y) {}

	double magnitude();
	Vec2 unit();

	bool operator==(const Vec2& other) const;
};

double cross(Vec2& a, Vec2& b);
Vec2 operator -(const Vec2& a, const Vec2& b);
Vec2 operator +(const Vec2& a, const Vec2& b);
double operator *(const Vec2& a, const Vec2& b);
Vec2 operator *(const Vec2& a, const double& t);
Vec2 operator *(const double& t, const Vec2& a);
IVec2 ivec2_from_vec2(const Vec2& vec2);

struct IVec2Hasher
{
	std::size_t operator()(const IVec2& ivec2) const {
		//return std::hash<int>()(ivec2.x) ^ std::hash<int>()(ivec2.y);
		uintmax_t hash = std::hash<int>{}(ivec2.x);
        hash <<= sizeof(uintmax_t) * 4;
        hash ^= std::hash<int>{}(ivec2.y);
        return std::hash<uintmax_t>{}(hash);
	}
};
