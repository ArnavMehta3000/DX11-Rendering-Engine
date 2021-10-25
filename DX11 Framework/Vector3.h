#pragma once
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>
#include <Windows.h>

class Vector3
{
public:
	float x, y, z;

	Vector3(float p_x = 0.0f, float p_y = 0.0f, float p_z = 0.0f);
	Vector3(const Vector3& other);

	inline Vector3 operator+(const Vector3& other) { return Vector3(x + other.x, y + other.y, z + other.z); }
	inline Vector3 operator-(const Vector3& other) { return Vector3(x - other.x, y - other.y, z - other.z); }
	inline Vector3 operator*(const Vector3& other) { return Vector3(x * other.x, y * other.y, z * other.z); }
	inline Vector3 operator/(float value) { if (value == 0) throw "Division by 0 exception!"; else return Vector3(x / value, y / value, z / value); }
	inline Vector3 operator=(const Vector3& other) { return Vector3(x = other.x, y = other.y, z = other.z); }
	inline Vector3 operator+=(const Vector3& other) { x += other.x; y += other.y; z += other.z; return *this; }
	inline Vector3 operator-=(const Vector3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
	inline Vector3 operator*=(const Vector3& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
	inline Vector3 operator/=(float value) { if (value == 0) throw "Division by 0 exception!"; else { x /= value; y /= value; z /= value; return *this; } }

	float Dot(const Vector3& other);
	Vector3 Cross(const Vector3& other);
	Vector3 Normalize();

	float Square();
	float Distance(const Vector3& other);
	float SqrDistance(const Vector3& other);
	float Magnitude();

	void Debug(std::string msg = "");
};