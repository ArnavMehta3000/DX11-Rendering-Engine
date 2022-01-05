#include "Camera.h"

Camera::Camera(CameraInitData cam)
{
	m_Position = cam.position;
	m_At = cam.at;
	m_Up = cam.up;

	m_WindowWidth = cam.windowWidth;
	m_WindowHeight = cam.windowHeight;
	
	m_NearZ = cam.nearZ;
	m_FarZ = cam.farZ;

	m_CamForward = Vector3(0.0f, 0.0f, 1.0f);
	m_CamRight = Vector3(1.0f, 0.0f, 0.0f);

	SetLens();
	bool enabled = true;
}

Camera::~Camera() {}

void Camera::Update()
{
	if (!enabled) return;

	SetLens();
}

void Camera::SetPosition(float x, float y, float z) { m_Position = Vector3(x, y, z); }

void Camera::SetPosition(const Vector3& pos) { m_Position = pos; }

void Camera::SetLookAt(float x, float y, float z) { m_At = Vector3(x, y, z); }

void Camera::SetLookAt(const Vector3& at) { m_At = at; }

void Camera::SetUp(float x, float y, float z) { m_Up = Vector3(x, y, z); }

void Camera::SetUp(const Vector3& up) { m_Up = up; }

void Camera::TranslatePosition(Vector3 delta)
{
	SetPosition(m_Position + (delta / 10000));
}

void Camera::TranslateLookAt(Vector3 delta)
{
	SetLookAt(m_At + (delta / 10000));
}

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
	XMVECTOR Eye = XMVectorSet(m_Position.x, m_Position.y, m_Position.z, 0.0f);
	XMVECTOR At = XMVectorSet(m_At.x, m_At.y, m_At.z, 0.0f);
	XMVECTOR Up = XMVectorSet(m_Up.x, m_Up.y, m_Up.z, 0.0f);

	// Build a view matrix using left hand coord system
	XMStoreFloat4x4(&m_View, XMMatrixLookAtLH(Eye, At, Up));

	XMStoreFloat4x4(&m_Proj, XMMatrixPerspectiveFovLH(XM_PIDIV2, m_WindowWidth / (FLOAT)m_WindowHeight, m_NearZ, m_FarZ));
}
