#include "Texture.h"

Texture::Texture(ID3D11Device* device)
{
	m_d3dDevice = device;
	m_LoadedTexture = false;
}

Texture::~Texture() {}

bool Texture::LoadTexture(const wchar_t* filename)
{
	HRESULT hr = CreateDDSTextureFromFile(m_d3dDevice, filename, nullptr, &m_TextureRV);
	
	if (FAILED(hr))
	{
		OutputDebugString(L"Failed to load texture");
		return false;
	}

	m_LoadedTexture = true;
	return true;
}