#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H
#include "Core.h"
#include "GameObject.h"


class SkySphere : public GameObject
{
public:
	SkySphere(const char* initFile, ID3D11DeviceContext* deviceContext, ID3D11Buffer* constantBuffer, ID3D11Device* device);
	SkySphere(GOInitData goData, ID3D11Device* device, const wchar_t* textureFile);
	~SkySphere();
};
#endif // !SKYBOX_H