#pragma once
#ifndef VECTOR3_H
#define VECTOR3_H

#include <iostream>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <DirectXMath.h>
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

	// Static helper functions
	static DirectX::XMFLOAT3 V3ToXMFLOAT3(const Vector3& vec);
	static Vector3 XMFLOAT3ToV3(const DirectX::XMFLOAT3& other);

	static DirectX::XMVECTOR V3ToXMVECTOR(const Vector3& vec);
	static Vector3 XMVECTORToV3(const DirectX::XMVECTOR& other);
};
#endif