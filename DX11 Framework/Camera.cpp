#include "Camera.h"

//Camera::Camera(Vector3 position, Vector3 at, Vector3 up, float windowWidth, float windowHeight, float nearDepth, float farDepth)
//{
//	// Init view matrix
//	XMVECTOR Eye = Vector3::V3ToXMVECTOR(position);
//	XMVECTOR At = Vector3::V3ToXMVECTOR(at);
//	XMVECTOR Up = Vector3::V3ToXMVECTOR(up);
//
//	m_windowWidth = windowWidth;
//	m_windowHeight= windowHeight;
//	m_farDepth = farDepth;
//	m_nearDepth = nearDepth;
//}
//
//void Camera::Update()
//{
//	//XMStoreFloat4x4(&m_view, XMMatrixLookAtLH(Eye, At, Up));
//	//XMStoreFloat4x4(&m_projection, XMMatrixPerspectiveFovLH(XM_PIDIV2, m_windowWidth / (FLOAT)m_windowHeight, 0.01f, 100.0f));
//}

Camera::Camera(Vector3 position, Vector3 at, Vector3 up, float windowWidth, float windowHeight, float nearZ, float farZ)
{
	m_Position = position;
	m_At = at;
	m_Up = up;
	m_WindowWidth = windowWidth;
	m_WindowHeight = windowHeight;
	m_NearZ = nearZ;
	m_FarZ = farZ;
}

Camera::~Camera()
{
}

void Camera::Update()
{
	SetLens();
}

void Camera::SetPosition(float x, float y, float z)
{
	m_Position = Vector3(x, y, z);
}

void Camera::SetPosition(const Vector3& pos)
{
	m_Position = pos;
}

void Camera::SetLookAt(float x, float y, float z)
{
	m_At = Vector3(x, y, z);
}

void Camera::SetLookAt(const Vector3& at)
{
	m_At = at;
}

void Camera::SetNearZ(float nearZ)
{
	m_NearZ = nearZ;
}

void Camera::SetFarZ(float farZ)
{
	m_FarZ = farZ;
}

void Camera::SetLens()
{
	// Create view and projection matrices
	XMVECTOR Eye = XMVectorSet(m_Position.x, m_Position.y, m_Position.z, 0.0f);
	XMVECTOR At = XMVectorSet(m_At.x, m_At.y, m_At.z, 0.0f);
	XMVECTOR Up = XMVectorSet(m_Up.x, m_Up.y, m_Up.z, 0.0f);

	XMStoreFloat4x4(&m_View, XMMatrixLookAtLH(Eye, At, Up));

	XMStoreFloat4x4(&m_Proj, XMMatrixPerspectiveFovLH(XM_PIDIV2, m_WindowWidth / (FLOAT)m_WindowHeight, 0.01f, 100.0f));
}
