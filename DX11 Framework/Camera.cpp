#include "Camera.h"

Camera::Camera(Vector3 position, Vector3 at, Vector3 up, float windowWidth, float windowHeight, float nearDepth, float farDepth)
{
	// Init view matrix
	XMVECTOR Eye = Vector3::V3ToXMVECTOR(position);
	XMVECTOR At = Vector3::V3ToXMVECTOR(at);
	XMVECTOR Up = Vector3::V3ToXMVECTOR(up);

	m_windowWidth = windowWidth;
	m_windowHeight= windowHeight;
	m_farDepth = farDepth;
	m_nearDepth = nearDepth;
}

void Camera::Update()
{
	XMStoreFloat4x4(&m_view, XMMatrixLookAtLH(Eye, At, Up));
	XMStoreFloat4x4(&m_projection, XMMatrixPerspectiveFovLH(XM_PIDIV2, m_windowWidth / (FLOAT)m_windowHeight, 0.01f, 100.0f));
}
