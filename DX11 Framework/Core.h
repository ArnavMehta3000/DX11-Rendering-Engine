#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include "Vector3.h"
#include "Matrix.h"
#include "Light.h"
using namespace DirectX;


struct SimpleVertex
{
public:
    XMFLOAT3 Pos;
    XMFLOAT3 Normal;
};

struct ConstantBuffer
{
public:
    XMMATRIX mWorld;
    XMMATRIX mView;
    XMMATRIX mProjection;

    Light::DirectionalLight dirLight;
    
    XMFLOAT3 EyePosW; // Camera position in world space
    XMFLOAT3 LightVecW;
};