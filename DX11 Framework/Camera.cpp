#include "Camera.h"

Camera::Camera(Vector3 position, Vector3 at, Vector3 up, float windowWidth, float windowHeight, float nearZ, float farZ)
{
	m_Position = position;
	m_At = at;
	m_Up = up;
	m_WindowWidth = windowWidth;
	m_WindowHeight = windowHeight;
	m_NearZ = nearZ;
	m_FarZ = farZ;

	SetLens();
}

Camera::~Camera() {}

void Camera::Update()
{
	SetLens();
}

void Camera::SetPosition(float x, float y, float z) { m_Position = Vector3(x, y, z); }

void Camera::SetPosition(const Vector3& pos) { m_Position = pos; }

void Camera::SetLookAt(float x, float y, float z) { m_At = Vector3(x, y, z); }

void Camera::SetLookAt(const Vector3& at) { m_At = at; }

void Camera::SetUp(float x, float y, float z) { m_Up = Vector3(x, y, z); }

void Camera::SetUp(const Vector3& up) { m_Up = up; }

void Camera::SetNearZ(float nearZ) { m_NearZ = nearZ; }

void Camera::SetFarZ(float farZ) { m_FarZ = farZ; }

XMFLOAT4X4 Camera::GetViewProj() const
{
	XMMATRIX view = XMLoadFloat4x4(&m_View);
	XMMATRIX proj = XMLoadFloat4x4(&m_Proj);
	XMFLOAT4X4 output;
	XMStoreFloat4x4(&output, view * proj);

	return output;
}

void Camera::SetLens()
{
	// Create view and projection matrices
	XMVECTOR Eye = XMVectorSet(m_Position.x, m_Position.y, m_Position.z, 0.0f);
	XMVECTOR At = XMVectorSet(m_At.x, m_At.y, m_At.z, 0.0f);
	XMVECTOR Up = XMVectorSet(m_Up.x, m_Up.y, m_Up.z, 0.0f);

	XMStoreFloat4x4(&m_View, XMMatrixLookAtLH(Eye, At, Up));

	XMStoreFloat4x4(&m_Proj, XMMatrixPerspectiveFovLH(XM_PIDIV2, m_WindowWidth / (FLOAT)m_WindowHeight, m_NearZ, m_FarZ));
}
