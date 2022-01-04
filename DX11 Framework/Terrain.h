#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

#include "Core.h"
#include <vector>
#include <fstream>


struct HMapInfo
{
	HMapInfo() : Width(0), Height(0), Scale(0.0f) {}
	HMapInfo(unsigned int width, unsigned int height, float scale)
		: Width(width), Height(height), Scale(scale) {}

	unsigned int Width;
	unsigned int Height;
	float Scale;
};

struct TerrainBuffers
{
	TerrainBuffers()
		: VertexBuffer(nullptr), IndexBuffer(nullptr), IndexCount(0) {}

	TerrainBuffers(ID3D11Buffer* vb, ID3D11Buffer* ib, int indexCount)
		: VertexBuffer(vb), IndexBuffer(ib), IndexCount(indexCount) {}

	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	int IndexCount;
};

class Terrain
{
public:
	Terrain(const char* file, HMapInfo initData, ID3D11Device* device);
	~Terrain();
	XMFLOAT4X4 GetWorldMat() { return m_WorldMat; }
	void LoadHeightMap(const char* file, HMapInfo initData);
	TerrainBuffers GetBuffers();

public:
	void CreateVertexBuffer();
	void CreateIndexBuffer();

	std::vector<float> m_HeightMap;
	HMapInfo           m_InitData;
	ID3D11Device*      m_Device;
	XMFLOAT4X4         m_WorldMat;
	ID3D11Buffer*      m_VertexBuffer;
	ID3D11Buffer*      m_IndexBuffer;
	int                m_IndexCount;

	bool initialized = false;
};
#endif // !TERRAIN_H