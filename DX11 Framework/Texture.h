#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H
#include "Core.h"
class Texture
{
public:
	Texture(ID3D11Device* device);
	~Texture();

	bool LoadTexture(const wchar_t* filename);
	ID3D11ShaderResourceView* GetTextureRV() { return m_TextureRV; }
private:
	ID3D11ShaderResourceView* m_TextureRV = nullptr;
	ID3D11Device* m_d3dDevice;
};
#endif // !TEXTURE_H
