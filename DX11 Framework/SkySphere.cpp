#include "SkySphere.h"

SkySphere::SkySphere(GOInitData goData, ID3D11Device* device, const wchar_t* textureFile) : GameObject(goData)
{
	SetMesh("Assets/Models/Blender/sphere.obj", device, false);
	LoadTexture(device, textureFile);
}

SkySphere::~SkySphere() {}