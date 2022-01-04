#include "Terrain.h"

Terrain::Terrain(const char* file, HMapInfo initData, ID3D11Device* device)
	: m_InitData(initData), m_Device(device)
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
	const unsigned int xSize = m_InitData.Width, zSize = m_InitData.Height;
	std::vector<SimpleVertex> terrainVertices((xSize + 1) * (zSize + 1));

	float offsetX = (xSize + 1) / 2.0f;
	float offsetZ = (zSize + 1) / 2.0f;

	
	for (int i = 0, z = 0; z < zSize; z++)
		for (int x = 0; x < xSize; x++)
		{
			terrainVertices[i] = { XMFLOAT3(x - offsetX, m_HeightMap[i], z - offsetZ),
								   XMFLOAT3(0.0f, 1.0f, 0.0f),
								   XMFLOAT2(x - offsetX / xSize, z - offsetZ / zSize) };
			i++;
		}

	#pragma region Create Vertex Buffer
	HRESULT hr;
// Set buffer description
	D3D11_BUFFER_DESC vBd;
	ZeroMemory(&vBd, sizeof(vBd));
	vBd.Usage = D3D11_USAGE_DEFAULT;
	vBd.ByteWidth = sizeof(SimpleVertex) * (xSize + 1) * (zSize + 1);
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
	const unsigned int xSize = m_InitData.Width, zSize = m_InitData.Height;
	int indexCount = (m_InitData.Width) * (m_InitData.Height) * 6;
	std::vector<UINT> terrainIndices(indexCount);

	int vert = 0, tris = 0;
	for (int z = 0; z < zSize; z++)
	{
		for (int x = 0; x < xSize; x++)
		{
			terrainIndices[tris + 0] = vert + 0;
			terrainIndices[tris + 1] = vert + xSize + 1;
			terrainIndices[tris + 2] = vert + 1;
			terrainIndices[tris + 3] = vert + 1;
			terrainIndices[tris + 4] = vert + xSize + 1;
			terrainIndices[tris + 5] = vert + xSize + 2;

			vert++;
			tris += 6;
		}
		vert++;
	}

	#pragma region Create Index Buffer
	HRESULT hr;

//  Create buffer description
	D3D11_BUFFER_DESC ibBd;
	ZeroMemory(&ibBd, sizeof(ibBd));

	ibBd.Usage = D3D11_USAGE_DEFAULT;
	ibBd.ByteWidth = sizeof(WORD) * (m_InitData.Width) * (m_InitData.Height) * 6;
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

	return TerrainBuffers(m_VertexBuffer, m_IndexBuffer, (m_InitData.Width) * (m_InitData.Height) * 6);
}