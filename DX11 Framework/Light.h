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

	// Structure for different light intensities
	struct Intensity
	{
		Intensity() { ZeroMemory(this, sizeof(this)); }

		XMFLOAT4 Ambient;  // Ambient light intensity
		XMFLOAT4 Diffuse;  // Diffuse light intensity
		XMFLOAT4 Specular; // Specular light intensity
	};

	// Structure for different light materials
	struct Material
	{
	public:
		Material() { ZeroMemory(this, sizeof(this)); }

		XMFLOAT4 ambient;  // Ambient light material
		XMFLOAT4 diffuse;  // Diffuse light material
		XMFLOAT4 specular; // Specular light material
	};

	// Structure for directional lights
	struct DirectionalLight
	{
	public:
		DirectionalLight() { ZeroMemory(this, sizeof(this)); }

		Intensity Intensity;
		Material Material; 

		XMFLOAT3 Direction;
		float SpecularPower;
	};

	// Structure for point lights
	struct PointLight
	{
		Intensity Intensity;
		Material Material;

		XMFLOAT3 Position;
		float Range;  // Max reach distance of point light

		XMFLOAT3 Attenuation;  // Fading of light
		float SpecularPower;
	};
};
