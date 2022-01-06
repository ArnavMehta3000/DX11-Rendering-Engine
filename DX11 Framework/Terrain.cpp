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
	/*const unsigned int xSize = m_InitData.Width, zSize = m_InitData.Height;
	std::vector<SimpleVertex> terrainVertices((xSize) * (zSize));

	float offsetX = (xSize) / 2.0f;
	float offsetZ = (zSize) / 2.0f;

	
	for (int i = 0, z = 0; z < zSize; z++)
		for (int x = 0; x < xSize; x++)
		{
			terrainVertices[i] = { XMFLOAT3(x - offsetX, m_HeightMap[i], z - offsetZ),
								   XMFLOAT3(0.0f, 1.0f, 0.0f),
								   XMFLOAT2(x - offsetX / xSize, z - offsetZ / zSize) };
			i++;
		}*/


	int m = m_InitData.Width, n = m_InitData.Height;
	UINT vertexCount = m * n;
	UINT faceCount = (m - 1) * (n - 1) * 2;

	float halfWidth = 0.5f * m_InitData.Width;
	float halfDepth = 0.5f * m_InitData.Height;
	float dx = m_InitData.Width / (n - 1);
	float dz = m_InitData.Height / (m - 1);
	float du = 1.0f / (n - 1);
	float dv = 1.0f / (m - 1);

	std::vector<SimpleVertex> terrainVertices(vertexCount);

	int t = 0;
	for (UINT i = 0; i < m; i++)
	{
		float z = halfDepth - i * dz;
		for (UINT j = 0; j < n; j++)
		{
			float x = -halfWidth + j * dx;
			
			terrainVertices[i * n + j] = { XMFLOAT3(x, m_HeightMap[t], z),
											XMFLOAT3(0.0f, 1.0f, 0.0f),
											XMFLOAT2(j * du, i * dv) };
			t++;
		}
		//t++;
	}



	#pragma region Create Vertex Buffer
	HRESULT hr;
// Set buffer description
	D3D11_BUFFER_DESC vBd;
	ZeroMemory(&vBd, sizeof(vBd));
	vBd.Usage = D3D11_USAGE_DEFAULT;
	vBd.ByteWidth = sizeof(SimpleVertex) * (m* n);
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
	int m = m_InitData.Width, n = m_InitData.Height;
	UINT vertexCount = m * n;
	UINT faceCount = (m - 1) * (n - 1) * 2;
	std::vector<UINT> terrainIndices(faceCount * 3);

	UINT tris = 0;
	for (UINT i = 0; i < m - 1; ++i)
	{
		for (UINT j = 0; j < n - 1; ++j)
		{
			terrainIndices[tris] = i * n + j;
			terrainIndices[tris + 1] = i * n + j + 1;
			terrainIndices[tris + 2] = (i + 1) * n + j;
			terrainIndices[tris + 3] = (i + 1) * n + j;
			terrainIndices[tris + 4] = i * n + j + 1;
			terrainIndices[tris + 5] = (i + 1) * n + j + 1;
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