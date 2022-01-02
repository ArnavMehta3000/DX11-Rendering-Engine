#include "Terrain.h"

Terrain::Terrain(GridData gData, ID3D11Device* device)
{
	m_GridData = gData;
	m_Device = device;
	XMStoreFloat4x4(&m_WorldMat, XMMatrixIdentity());

	CreateBuffers();
}

Terrain::~Terrain()
{
}

XMFLOAT4X4 Terrain::GetWorldMatrix() { return m_WorldMat; }

HRESULT Terrain::CreateBuffers()
{
	HRESULT hr;

	//
	// Create vertex buffer
	//

	// Set buffer description
	D3D11_BUFFER_DESC vertBd;
	ZeroMemory(&vertBd, sizeof(vertBd));
	vertBd.Usage = D3D11_USAGE_DEFAULT;
	vertBd.ByteWidth = sizeof(TerrainVertex) * m_GridData.VertexCount;
	vertBd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertBd.CPUAccessFlags = 0;
	
	D3D11_SUBRESOURCE_DATA terrainVBInitData;
	ZeroMemory(&terrainVBInitData, sizeof(terrainVBInitData));
	terrainVBInitData.pSysMem = &m_GridData.Vertices[0];
	hr = m_Device->CreateBuffer(&vertBd, &terrainVBInitData, &m_VertexBuffer);


	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create terrain Vertex Buffer");
		return hr;
	}

	//
	// Create index buffer
	//

	// Set buffer description
	D3D11_BUFFER_DESC indBd;
	ZeroMemory(&indBd, sizeof(indBd));

	indBd.Usage = D3D11_USAGE_DEFAULT;
	indBd.ByteWidth = sizeof(WORD) * m_GridData.IndexCount;
	indBd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indBd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA terrainIBInitData;
	ZeroMemory(&terrainIBInitData, sizeof(terrainIBInitData));
	terrainIBInitData.pSysMem = &m_GridData.Indices[0];
	hr = m_Device->CreateBuffer(&indBd, &terrainIBInitData, &m_IndexBuffer);

	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to create terrain Index Buffer");
		return hr;
	}

	

	return hr;
}
