#include "GameObject.h"

GameObject::GameObject(GOInitData data)
{
	m_Position = data.position;
	m_Rotation = data.rotation;
	m_Scale = data.scale;

	UpdateWorldMatrix();

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

void GameObject::LoadTexture(ID3D11Device* device, const wchar_t* file)
{
	m_Texture = new Texture(device);
	hasTexture = m_Texture->LoadTexture(file);
}

void GameObject::Update()
{
	UpdateWorldMatrix();
}

void GameObject::Draw(ConstantBuffer* cb)
{
	if (!hasMesh || !initialized) return;

	m_InitData.deviceContext->IASetVertexBuffers(0, 1, &m_Mesh.VertexBuffer, &m_Mesh.VBStride, &m_Mesh.VBOffset);
	m_InitData.deviceContext->IASetIndexBuffer(m_Mesh.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	m_InitData.deviceContext->UpdateSubresource(m_InitData.constantBuffer, 0, nullptr, cb, 0, 0);
	m_InitData.deviceContext->DrawIndexed(m_Mesh.IndexCount, 0, 0);
}

void GameObject::DrawTextured(ConstantBuffer* cb, ID3D11DeviceContext* ctx, int slot)
{
	if (hasTexture)
	{
		auto tex = m_Texture->GetTextureRV();
		ctx->PSSetShaderResources(slot, 1, &tex);
	}

	Draw(cb);
}

XMFLOAT4X4 GameObject::GetWorldMatrix() { return m_WorldMatrix; }

Vector3 GameObject::GetPosition()
{
	XMVECTOR scaling, rotQuat, trans;
	XMMatrixDecompose(&scaling, &rotQuat, &trans, XMLoadFloat4x4(&m_WorldMatrix));

	m_Position = Vector3::XMVECTORToV3(trans);
	return m_Position;
}

Vector3 GameObject::GetRotation()
{
	XMVECTOR scaling, rotQuat, trans;
	XMMatrixDecompose(&scaling, &rotQuat, &trans, XMLoadFloat4x4(&m_WorldMatrix));

	m_Rotation = Vector3::XMVECTORToV3(rotQuat);
	return m_Rotation;
}

Vector3 GameObject::GetScale()
{
	XMVECTOR scaling, rotQuat, trans;
	XMMatrixDecompose(&scaling, &rotQuat, &trans, XMLoadFloat4x4(&m_WorldMatrix));

	m_Scale = Vector3::XMVECTORToV3(scaling);
	return m_Scale;
}

void GameObject::SetPosition(Vector3 pos) {	m_Position = pos; }

void GameObject::SetRotation(Vector3 rot) { m_Rotation = rot; }

void GameObject::SetScale(Vector3 scale) { m_Scale = scale; }

void GameObject::UpdateWorldMatrix()
{
	// Create transformation matrices
	XMMATRIX position = XMMatrixTranslationFromVector(Vector3::V3ToXMVECTOR(m_Position));
	XMMATRIX scaling  = XMMatrixScalingFromVector(Vector3::V3ToXMVECTOR(m_Scale));
	XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(Vector3::V3ToXMVECTOR(m_Rotation));

	XMStoreFloat4x4(&m_WorldMatrix, scaling * rotation * position);
}
