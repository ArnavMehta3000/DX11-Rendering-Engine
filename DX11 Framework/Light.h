#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
using namespace DirectX;


class Light
{
public:
	struct Material
	{
	public:
		Material() { ZeroMemory(this, sizeof(this)); }
		// Ambient light material
		XMFLOAT4 ambient;
		// Diffuse light material
		XMFLOAT4 diffuse;
		// Specular light material
		XMFLOAT4 specular;
	};

	struct DirectionalLight
	{
	public:
		DirectionalLight() { ZeroMemory(this, sizeof(this)); }

		// Ambient light intensty
		XMFLOAT4 Ambient;
		// Diffuse light intensity
		XMFLOAT4 Diffuse;
		// Specular light intensity
		XMFLOAT4 Specular;
		Material Material;

		// Direction the light is facing
		XMFLOAT3 Direction;
		// The specular power
		float SpecularPower;
	};
};
