#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Core.h"
#include "Transform.h"
#include "OBJLoader.h"

struct GOInitData
{
	Vector3 position, rotation, scale;
	ID3D11DeviceContext* deviceContext;
	ID3D11Buffer* constantBuffer;
};

class GameObject
{
public:
	GameObject(GOInitData data);
	~GameObject();

	inline Transform GetTransform() { return m_Transform; }
	void SetMesh(char* file, ID3D11Device* device, bool invertTexCoords = false);
	void Update();
	void Draw(ConstantBuffer* cb);

private:
	Transform  m_Transform;
	GOInitData m_InitData;
	MeshData   m_Mesh;

	bool initialized = false;
	bool hasMesh = false;

};
#endif // !GAMEOBJECT_H