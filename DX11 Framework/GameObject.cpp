#include "GameObject.h"

GameObject::GameObject(GOInitData data)
{
	m_Transform = Transform(data.position, data.rotation, data.scale);
	m_InitData = data;
	initialized = true;
}

GameObject::~GameObject() {}

void GameObject::SetMesh(char* file, ID3D11Device* device, bool invertTexCoords)
{
	if (!initialized) return;

	m_Mesh = OBJLoader::Load(file, device, invertTexCoords);
	hasMesh = true;
}

void GameObject::Update()
{
}

void GameObject::Draw(ConstantBuffer* cb)
{
	if (!hasMesh || !initialized) return;

	m_InitData.deviceContext->IASetVertexBuffers(0, 1, &m_Mesh.VertexBuffer, &m_Mesh.VBStride, &m_Mesh.VBOffset);
	m_InitData.deviceContext->IASetIndexBuffer(m_Mesh.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);


	m_InitData.deviceContext->UpdateSubresource(m_InitData.constantBuffer, 0, nullptr, cb, 0, 0);
	m_InitData.deviceContext->DrawIndexed(m_Mesh.IndexCount, 0, 0);
}
