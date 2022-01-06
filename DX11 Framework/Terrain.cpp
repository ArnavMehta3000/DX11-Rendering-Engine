#include "Terrain.h"

Terrain::Terrain(const char* file, HMapInfo initData, ID3D11Device* device)
	: m_InitData(initData), m_Device(device), m_file(file)
{
	XMMATRIX position = XMMatrixTranslationFromVector(Vector3::V3ToXMVECTOR(Vector3()));
	XMMATRIX scaling = XMMatrixScalingFromVector(Vector3::V3ToXMVECTOR(Vector3(1.0f, 1.0f, 1.0f)));
	XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(Vector3::V3ToXMVECTOR(Vector3()));

	XMStoreFloat4x4(&m_WorldMat, scaling * rotation * position);

	LoadHeightMap(file, initData);
	CreateVertexBuffer();
	CreateIndexBuffer();

	initialized = true;
}

Terrain::~Terrain()
{
	m_HeightMap.clear();
}

void Terrain::LoadHeightMap(const char* file, HMapInfo initData)
{
	m_HeightMap.clear();
	std::vector<unsigned char> input(initData.Width * initData.Height);
	m_HeightMap = std::vector<float>((m_InitData.Width + 1) * (m_InitData.Height + 1));

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

void Terrain::CreateVertexBuffer()
{
	int xSize = m_InitData.Width, zSize = m_InitData.Height;
	UINT vertexCount = xSize * zSize;
	UINT faceCount = (xSize - 1) * (zSize - 1) * 2;

	float halfWidth = 0.5f * m_InitData.Width;
	float halfDepth = 0.5f * m_InitData.Height;

	// Distance beteween vertex points
	float dx = m_InitData.Width / (zSize - 1);
	float dz = m_InitData.Height / (xSize - 1);

	// UV Maps
	float du = 1.0f / (zSize - 1);
	float dv = 1.0f / (xSize - 1);

	std::vector<SimpleVertex> terrainVertices(vertexCount);

	int vertex = 0;
	for (UINT row = 0; row < xSize; row++)
	{
		float z = halfDepth - row * dz;
		for (UINT col = 0; col < zSize; col++)
		{
			float x = -halfWidth + col * dx;
			
			terrainVertices[row * zSize + col] = { XMFLOAT3(x, m_HeightMap[vertex], z),  // Position
												   XMFLOAT3(0.0f, 1.0f, 0.0f),			 // Normals
												   XMFLOAT2(col * du, row * dv) };		 // UV's
			vertex++;
		}
	}



	#pragma region Create Vertex Buffer
	HRESULT hr;
	// Set buffer description
	D3D11_BUFFER_DESC vBd;
	ZeroMemory(&vBd, sizeof(vBd));
	vBd.Usage = D3D11_USAGE_DEFAULT;
	vBd.ByteWidth = sizeof(SimpleVertex) * (xSize* zSize);
	vBd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vBd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vbResource;
	ZeroMemory(&vbResource, sizeof(vbResource));
	vbResource.pSysMem = &terrainVertices[0];
	#pragma endregion

	hr = m_Device->CreateBuffer(&vBd, &vbResource, &m_VertexBuffer);
}

void Terrain::CreateIndexBuffer()
{
	int xSize = m_InitData.Width, zSize = m_InitData.Height;
	UINT faceCount = (xSize - 1) * (zSize - 1) * 2;
	std::vector<UINT> terrainIndices(faceCount * 3);

	UINT tris = 0;
	for (UINT row = 0; row < xSize - 1; ++row)
	{
		for (UINT col = 0; col < zSize - 1; ++col)
		{
			terrainIndices[tris]     = row * zSize + col;
			terrainIndices[tris + 1] = row * zSize + col + 1;
			terrainIndices[tris + 2] = (row + 1) * zSize + col;
			terrainIndices[tris + 3] = (row + 1) * zSize + col;
			terrainIndices[tris + 4] = row * zSize + col + 1;
			terrainIndices[tris + 5] = (row + 1) * zSize + col + 1;
			tris += 6; // next quad
		}
	}




	#pragma region Create Index Buffer
	HRESULT hr;

//  Create buffer description
	D3D11_BUFFER_DESC ibBd;
	ZeroMemory(&ibBd, sizeof(ibBd));

	ibBd.Usage = D3D11_USAGE_DEFAULT;
	ibBd.ByteWidth = sizeof(UINT) * (faceCount * 3);
	ibBd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibBd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA ibResource;
	ZeroMemory(&ibResource, sizeof(ibResource));
	ibResource.pSysMem = &terrainIndices[0];
	#pragma endregion


	hr = m_Device->CreateBuffer(&ibBd, &ibResource, &m_IndexBuffer);
}

TerrainBuffers Terrain::GetBuffers()
{
	if (!initialized) return TerrainBuffers();

	return TerrainBuffers(m_VertexBuffer, m_IndexBuffer, (m_InitData.Width-1) * (m_InitData.Height-1) * 6);
}

void Terrain::UpdateTerrain(HMapInfo newHmInfo)
{
	m_InitData = newHmInfo;

	LoadHeightMap(m_file, m_InitData);
	CreateVertexBuffer();
	CreateIndexBuffer();
}
