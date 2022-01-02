#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

#include "Core.h"
#include <vector>
#include <fstream>


struct HMapInfo
{
	HMapInfo() : Width(0), Height(0), Scale(0.0f), CellSpacing(0.0f) {}
	HMapInfo(unsigned int width, unsigned int height, float scale, float cellSpacing)
		: Width(width), Height(height), Scale(scale), CellSpacing(cellSpacing) {}

	unsigned int Width;
	unsigned int Height;
	float Scale;
	float CellSpacing;
};

struct TerrainBuffers
{
	TerrainBuffers() {}

	void SetCount(int v, int i)
	{
		VertexCount = v;
		IndexCount = i;
	}

	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	int VertexCount = 0, IndexCount = 0;
};

struct TerrainVertex
{
public:
	TerrainVertex() : Pos(XMFLOAT3()), TexC(XMFLOAT2()) {}
	TerrainVertex(XMFLOAT3 pos, XMFLOAT2 uv)
		: Pos(pos), TexC(uv) {}

	XMFLOAT3 Pos;
	XMFLOAT2 TexC;
	//XMFLOAT3 Normal;

	bool operator<(const TerrainVertex other) const
	{
		return memcmp((void*)this, (void*)&other, sizeof(TerrainVertex)) > 0;
	};
};

class TerrainData
{
public:
	TerrainData(int terrainWidth, int terrainHeight,ID3D11Device* device);
	~TerrainData();

	void AddTriangle(int t1, int t2, int t3);
	TerrainBuffers CreateBuffers();

	std::vector<XMFLOAT3> m_Vertices;
	std::vector<XMFLOAT2> m_UV;
	std::vector<TerrainVertex> m_TerrainVertices;
	std::vector<WORD> m_Triangles;
	TerrainBuffers m_Buffers;
private:
	int m_TriangleIndex;
	ID3D11Device* m_Device;
};

class Terrain
{
public:
	Terrain(const char* file, HMapInfo initData, ID3D11Device* device);
	~Terrain();

	void LoadHeightMap(const char* file, HMapInfo initData);
	void GenerateMesh(std::vector<float> heightMap);
	XMFLOAT4X4 GetWorldMat() { return m_WorldMat; }
	TerrainBuffers GetBuffers() { return m_Buffers; }
private:
	std::vector<float> m_HeightMap;
	HMapInfo           m_InitData;
	ID3D11Device*      m_Device;
	TerrainBuffers     m_Buffers;
	XMFLOAT4X4         m_WorldMat;
};
#endif // !TERRAIN_H