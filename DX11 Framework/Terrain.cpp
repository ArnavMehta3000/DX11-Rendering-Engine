#include "Terrain.h"

Terrain::Terrain(const char* file, HMapInfo initData, ID3D11Device* device)
	: m_InitData(initData), m_Device(device)
{
	XMMATRIX position = XMMatrixTranslationFromVector(Vector3::V3ToXMVECTOR(Vector3()));
	XMMATRIX scaling = XMMatrixScalingFromVector(Vector3::V3ToXMVECTOR(Vector3(1.0f, 1.0f, 1.0f)));
	XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(Vector3::V3ToXMVECTOR(Vector3()));

	XMStoreFloat4x4(&m_WorldMat, scaling * rotation * position);

	XMStoreFloat4x4(&m_WorldMat, XMMatrixIdentity());
	LoadHeightMap(file, initData);
	GenerateMesh(m_HeightMap);
}

Terrain::~Terrain()
{
	m_HeightMap.clear();
}

void Terrain::LoadHeightMap(const char* file, HMapInfo initData)
{
	std::vector<unsigned char> input(initData.Width * initData.Height);
	m_HeightMap = std::vector<float>(initData.Width * initData.Height);

	std::ifstream inFile;
	inFile.open(file, std::ios_base::binary);

	if (inFile)
	{
		inFile.read((char*)&input[0], (std::streamsize)input.size());
		inFile.close();
	}

	for (UINT i = 0; i < initData.Width * initData.Height; i++)
		m_HeightMap[i] = (input[i] / 255.0f) * initData.Scale;
}

void Terrain::GenerateMesh(std::vector<float> heightMap)
{
	int width = m_InitData.Width;
	int height = m_InitData.Height;
	float halfX = -0.5f * (width - 1);
	float halfZ = 0.5f * (height - 1);

	TerrainData terrainData(width, height, m_Device);
	int vertexIndex = 0;
	for (unsigned int z = 0; z < height; z++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			terrainData.m_Vertices[vertexIndex] = XMFLOAT3(halfX + x, m_HeightMap[width * x + z], halfZ - z);
			terrainData.m_UV[vertexIndex] = XMFLOAT2(x / (float)width, z / (float)height);

			if (x < width - 1 && z < height - 1)
			{
				terrainData.AddTriangle(vertexIndex, vertexIndex + width + 1, vertexIndex + width);
				terrainData.AddTriangle(vertexIndex + width + 1, vertexIndex, vertexIndex + 1);
			}

			vertexIndex++;
		}
	}

	m_Buffers = terrainData.CreateBuffers();
}



TerrainData::TerrainData(int terrainWidth, int terrainHeight, ID3D11Device* device)
{
	m_Vertices = std::vector<XMFLOAT3>(terrainHeight * terrainWidth);
	m_Triangles = std::vector<WORD>((terrainWidth - 1) * (terrainHeight - 1) * 6);
	m_UV = std::vector<XMFLOAT2>(terrainWidth * terrainHeight);
	m_TerrainVertices = std::vector<TerrainVertex>(terrainHeight * terrainWidth);
	m_Device = device;
	m_TriangleIndex = 0;
}

TerrainData::~TerrainData()
{
	m_Vertices.clear();
	m_Triangles.clear();
}

void TerrainData::AddTriangle(int t1, int t2, int t3)
{
	m_Triangles[m_TriangleIndex] = t1;
	m_Triangles[m_TriangleIndex + 1] = t2;
	m_Triangles[m_TriangleIndex + 2] = t3;
	m_TriangleIndex += 3;
}

TerrainBuffers TerrainData::CreateBuffers()
{
	m_Buffers = TerrainBuffers();
	HRESULT hr;
	
	// Create the vertex
	for (int i = 0; i < m_TerrainVertices.size(); i++)
		m_TerrainVertices[i] = TerrainVertex(m_Vertices[i], m_UV[i]);

	// Create void pointer to vertex data
	void* vertices = 0;
	//memcpy(vertices, &m_TerrainVertices[0], sizeof(TerrainVertex) * m_TerrainVertices.size());

	//
	//Create the vertex buffer
	//

	// VB Desc
	D3D11_BUFFER_DESC vbDesc;
	ZeroMemory(&vbDesc, sizeof(vbDesc));
	vbDesc.Usage = D3D11_USAGE_DEFAULT;
	vbDesc.ByteWidth = sizeof(TerrainVertex) * m_TerrainVertices.size();
	vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vbResource;
	ZeroMemory(&vbResource, sizeof(vbResource));
	vbResource.pSysMem = &m_TerrainVertices[0];

	hr = m_Device->CreateBuffer(&vbDesc, &vbResource, &m_Buffers.VertexBuffer);
	if (FAILED(hr))
		OutputDebugString(L"Failed to create terrain VB");



	//
	// Create the index buffer
	//

	D3D11_BUFFER_DESC ibDesc;
	ZeroMemory(&ibDesc, sizeof(ibDesc));

	ibDesc.Usage = D3D11_USAGE_DEFAULT;
	ibDesc.ByteWidth = sizeof(WORD) * m_Triangles.size();
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA ibResource;
	ZeroMemory(&ibResource, sizeof(ibResource));
	ibResource.pSysMem = &m_Triangles[0];

	hr = m_Device->CreateBuffer(&ibDesc, &ibResource, &m_Buffers.IndexBuffer);
	if (FAILED(hr))
		OutputDebugString(L"Failed to create terrain IB");


	m_Buffers.SetCount(m_TerrainVertices.size(), m_Triangles.size());

	return m_Buffers;
}
