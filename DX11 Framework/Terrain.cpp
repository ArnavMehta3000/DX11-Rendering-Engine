#include "Terrain.h"

Terrain::Terrain(ID3D11Device* device)
{
	m_Device = device;
	m_GridCreated = false;

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
}

Terrain::~Terrain()
{
}

void Terrain::LoadHeightMap(const char* fileName, int sizeX, int sizeZ)
{
	// Per vertex height
	std::vector<unsigned char> in(sizeX * sizeZ);

	std::ifstream inFile;
	inFile.open(fileName);

	if (inFile.good())
	{
		// Read data
		inFile.read((char*)&in[0], (std::streamsize(in.size())));
	}
}

TerrainMeshData Terrain::GenerateGrid(float sizeX, float sizeZ, UINT resX, UINT resZ)
{
	UINT vertexCount = resX * resZ;
	UINT faceCount = (resX - 1) * (resZ - 1) * 2;
	TerrainMeshData meshData(vertexCount, faceCount * 3);

	// -------------------------------------------- Create the vertices

	float halfWidth = 0.5f * resX;
	float halfDepth = 0.5f * resZ;

	float dx = sizeX / (resX - 1);
	float dz = sizeZ / (resZ - 1);

	float du = 1.0f / (resZ - 1);
	float dv = 1.0f / (resX - 1);

	for (UINT i = 0; i < resX; ++i)
	{
		float z = halfDepth - i * dz;
		for (UINT j = 0; j < resZ; ++j)
		{
			float x = -halfWidth + j * dx;

			meshData.Vertices[i * resZ + j].Position = Vector3(x, 0.0f, z);

			// Ignore for now, used for lighting.
			meshData.Vertices[i * resZ + j].Normal = Vector3(0.0f, 1.0f, 0.0f);

			// Ignore for now, used for texturing.
			meshData.Vertices[i * resZ + j].TexC.x = j * du;
			meshData.Vertices[i * resZ + j].TexC.y = i * dv;
		}
	}


	// -------------------------------------------- Create the indices

	meshData.Indices.resize(faceCount * 3); // 3 indices per face
	// Iterate over each quad and compute indices.
	UINT k = 0;
	for (UINT i = 0; i < resX - 1; ++i)
	{
		for (UINT j = 0; j < resZ - 1; ++j)
		{
			meshData.Indices[k] = i * resZ + j;
			meshData.Indices[k + 1] = i * resZ + j + 1;
			meshData.Indices[k + 2] = (i + 1) * resZ + j;
			meshData.Indices[k + 3] = (i + 1) * resZ + j;
			meshData.Indices[k + 4] = i * resZ + j + 1;
			meshData.Indices[k + 5] = (i + 1) * resZ + j + 1;
			k += 6; // next quad
		}
	}


	// Create the vertex buffer
	std::vector<SimpleVertex> vertices(meshData.Vertices.size());
	for (int i = 0; i < meshData.Vertices.size(); i++)
	{
		Vector3 pos = meshData.Vertices[i].Position;
		Vector3 norm = meshData.Vertices[i].Normal;
		Vector3 tex = meshData.Vertices[i].TexC;
		XMFLOAT3 p = XMFLOAT3(pos.x, pos.y, pos.z);
		XMFLOAT3 n = XMFLOAT3(norm.x, norm.y, norm.z);
		XMFLOAT2 t = XMFLOAT2(tex.x, tex.y);

		// Get height from height map and apply to y pos (p)

		vertices[i].Pos = p;
		vertices[i].Normal = n;
		vertices[i].TexC = t;
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(SimpleVertex) * meshData.Vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	m_Device->CreateBuffer(&vbd, &vinitData, &m_TerrainVertexBuffer);


	// Create the index buffer

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.ByteWidth = sizeof(UINT) * meshData.Indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &meshData.Indices[0];
	m_Device->CreateBuffer(&ibd, &iinitData, &m_TerrainIndexBuffer);

	m_TerrainGrid = meshData;
	m_GridCreated = true;

	return meshData;
}