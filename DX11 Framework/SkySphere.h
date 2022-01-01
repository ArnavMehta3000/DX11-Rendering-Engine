#pragma once
#ifndef SKYBOX_H
#define SKYBOX_H
#include "Core.h"
#include "GameObject.h"


class SkySphere : public GameObject
{
public:
	SkySphere(GOInitData goData, ID3D11Device* device, const wchar_t* textureFile);
	~SkySphere();
};
#endif // !SKYBOX_H