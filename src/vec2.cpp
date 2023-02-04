#include <math.h>
#include "vec2.h"

double Vec2::magnitude()
{
	double sum = x * x + y * y;
	return sqrt(sum);
}

Vec2 Vec2::unit()
{
	double magnitude = this->magnitude();
	if(magnitude == 0) return Vec2(0, 0);
	return Vec2(x / magnitude, y / magnitude);
}

double cross(Vec2& a, Vec2& b)
{
	return a.x * b.y - a.y * b.x;
}

bool IVec2::operator==(const IVec2& other) const
{
	return x == other.x && y == other.y;
}

IVec2 ivec2_from_vec2(const Vec2& vec2)
{
	return IVec2((int)vec2.x, (int)vec2.y);
}

Vec2 operator -(const Vec2& a, const Vec2& b)
{
	return Vec2(a.x - b.x, a.y - b.y);
}

Vec2 operator +(const Vec2& a, const Vec2& b)
{
	return Vec2(a.x + b.x, a.y + b.y);
}

double operator *(const Vec2& a, const Vec2& b)
{
	return a.x * b.x + a.y * b.y;
}

Vec2 operator *(const Vec2& a, const double& t)
{
	return Vec2(a.x * t, a.y * t);
}

Vec2 operator *(const double& t, const Vec2& a)
{
	return Vec2(a.x * t, a.y * t);
}

bool Vec2::operator==(const Vec2& other) const
{
	return epsilon_equals(x, other.x) && epsilon_equals(y, other.y);
}
