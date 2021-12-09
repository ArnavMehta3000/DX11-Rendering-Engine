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

XMFLOAT4X4 GameObject::GetWorldMatrix()
{
	return m_WorldMatrix;
}

XMMATRIX GameObject::GetWorldMatrixXM()
{
	return XMLoadFloat4x4(&m_WorldMatrix);
}

Vector3 GameObject::GetPosition()
{
	XMVECTOR scaling, rotQuat, trans;
	XMMatrixDecompose(&scaling, &rotQuat, &trans, XMLoadFloat4x4(&m_WorldMatrix));

	return Vector3::XMVECTORToV3(trans);
}

Vector3 GameObject::GetRotation()
{
	XMVECTOR scaling, rotQuat, trans;
	XMMatrixDecompose(&scaling, &rotQuat, &trans, XMLoadFloat4x4(&m_WorldMatrix));

	return Vector3::XMVECTORToV3(rotQuat);
}

Vector3 GameObject::GetScale()
{
	XMVECTOR scaling, rotQuat, trans;
	XMMatrixDecompose(&scaling, &rotQuat, &trans, XMLoadFloat4x4(&m_WorldMatrix));

	return Vector3::XMVECTORToV3(scaling);
}

void GameObject::UpdateWorldMatrix()
{
	XMMATRIX position = XMMatrixTranslationFromVector(Vector3::V3ToXMVECTOR(m_Position));
	XMMATRIX scaling  = XMMatrixScalingFromVector(Vector3::V3ToXMVECTOR(m_Scale));
	XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(Vector3::V3ToXMVECTOR(m_Rotation));

	XMStoreFloat4x4(&m_WorldMatrix, scaling * position * rotation);
}
