#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <fstream>
#include "Core.h"
#include "JSON\json.hpp"
#include "OBJLoader.h"
#include "Texture.h"

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
	GameObject(const char* initFile, ID3D11DeviceContext* deviceContext, ID3D11Buffer* constantBuffer);
	~GameObject();

	void SetMesh(char* file, ID3D11Device* device, bool invertTexCoords = false);
	void LoadTexture(ID3D11Device* device, const wchar_t* file);
	virtual void Update();
	virtual void Draw(ConstantBuffer* cb);
	virtual void DrawTextured(ConstantBuffer* cb, int slot = 0);

	XMFLOAT4X4 GetWorldMatrix();

	Vector3 GetPosition();
	Vector3 GetRotation();
	Vector3 GetScale();

	void SetPosition(Vector3 pos);
	void SetRotation(Vector3 rot);
	void SetScale(Vector3 scale);

private:
	Vector3    m_Position, m_Rotation, m_Scale;
	XMFLOAT4X4 m_WorldMatrix;
	GOInitData m_InitData;
	MeshData   m_Mesh;
	Texture* m_Texture;

	bool initialized = false;
	bool hasMesh = false;
	bool hasTexture = false;

protected:
	void UpdateWorldMatrix();
};
#endif // !GAMEOBJECT_H