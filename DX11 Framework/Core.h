#pragma once
#ifndef CORE_H
#define CORE_H

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <DirectXMath.h>
#include <directxcolors.h>
#include <DirectXCollision.h>
#include "resource.h"
#include "DDSTextureLoader.h"
#include "Vector3.h"
#include "Matrix.h"
#include "Light.h"
using namespace DirectX;

struct SimpleVertex
{
public:
    XMFLOAT3 Pos;
    XMFLOAT3 Normal;
    XMFLOAT2 TexC;

    bool operator<(const SimpleVertex other) const
    {
        return memcmp((void*)this, (void*)&other, sizeof(SimpleVertex)) > 0;
    };
};

struct ConstantBuffer
{
public:
    XMMATRIX mWorld;
    XMMATRIX mView;
    XMMATRIX mProjection;

    Light::DirectionalLight dirLight;
    Light::PointLight pointLight;
    
    XMFLOAT3 EyePosW; // Camera position in world space
    float Time;

    XMFLOAT3 LightVecW;
    float Rate;

    // Plane VS
    float Amplitude;
    float Frequency;
    float Threshold;
    float padding;

    // Terrain PS
    float WaterLevel;
    float GrassLevel;
    float StoneLevel;
    float SnowLevel;
};
#endif // !CORE_H
