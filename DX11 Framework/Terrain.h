#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <fstream>
#include "Core.h"


struct TerrainVertex
{
	TerrainVertex() {}
	TerrainVertex(const Vector3& pos, const Vector3& norm, const Vector3& uv)
		: Position(pos), Normal(norm), TexC(uv) {}


	Vector3 Position, Normal, TexC;
};

struct TerrainMeshData
{
	TerrainMeshData(){}
	TerrainMeshData(int vertexCount, int indexCount)
	{
		SetVertexCount(vertexCount);
		SetIndexCount(indexCount);
	}
	void SetVertexCount(int vertexCount)
	{
		std::vector<TerrainVertex> vert(vertexCount);
		Vertices = vert;
	}
	void SetIndexCount(int indexCount)
	{
		std::vector<UINT> ind(indexCount);
		Indices = ind;
	}
	std::vector<TerrainVertex> Vertices;
	std::vector<UINT> Indices;
};


class Terrain
{
public:
	Terrain(ID3D11Device* device);
	~Terrain();

	void LoadHeightMap(const char* fileName, int sizeX, int sizeZ);
	TerrainMeshData GenerateGrid(float sizeX, float sizeZ, UINT resX, UINT resZ);

	ID3D11Buffer* GetTerrainVBuffer() { return m_TerrainVertexBuffer; }
	ID3D11Buffer* GetTerrainIBuffer() { return m_TerrainIndexBuffer; }
	
public:
	TerrainMeshData	m_TerrainGrid;
	XMFLOAT4X4 m_WorldMatrix;
	ID3D11Device* m_Device;
	ID3D11Buffer* m_TerrainVertexBuffer;
	ID3D11Buffer* m_TerrainIndexBuffer;

	bool m_GridCreated;
};
#endif // !TERRAIN_H


