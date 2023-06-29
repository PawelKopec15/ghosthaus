#pragma once

#include <cmath>
#include <iostream>

template <typename T>
class Vector2
{
public:
	T x;
	T y;

	Vector2(const T& _x, const T& _y) : x(_x), y(_y) {}

	static Vector2 ZERO() { return Vector2(0, 0); }
	static Vector2 UP() { return Vector2(0, -1); }
	static Vector2 DOWN() { return Vector2(0, 1); }
	static Vector2 LEFT() { return Vector2(-1, 0); }
	static Vector2 RIGHT() { return Vector2(1, 0); }

	Vector2 operator+(const Vector2& other) { return Vector2(x + other.x, y + other.y); }
	Vector2 operator-(const Vector2& other) { return Vector2(x - other.x, y - other.y); }

	Vector2 operator/(const T val) { return Vector2(x / val, y / val); }
	Vector2 operator*(const T val) { return Vector2(x * val, y * val); }

	double lengthDbl() const { return std::sqrt(std::pow((double)x, 2) + std::pow((double)y, 2)); }
	int lengthInt() const { return (int)std::round(lengthDbl()); }

	std::string getPrintString() const { return std::to_string(x) + " " + std::to_string(y); }

	T dot(const Vector2& other) { return (x * other.x) + (y * other.y); }
};