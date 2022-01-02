#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

#include "Core.h"
#include "Grid.h"

struct TerrainVertex
{
	TerrainVertex()
	{
		Position = Vector3();
		Color = Vector3(1.0f, 1.0f, 1.0f);
	}
	TerrainVertex(Vector3 pos, Vector3 col)
	{
		Position = pos;
		Color = col;
	}
	Vector3 Position;
	Vector3 Color;
	//Vector3 Normal;
};

class Terrain
{
public:
	Terrain(GridData gData, ID3D11Device* device);
	~Terrain();

	XMFLOAT4X4 GetWorldMatrix();

	void Update();
	void Draw();
public:
	HRESULT CreateBuffers();


	GridData      m_GridData;
	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;
	ID3D11Device* m_Device;
	XMFLOAT4X4    m_WorldMat;
};
#endif // !TERRAIN_H


