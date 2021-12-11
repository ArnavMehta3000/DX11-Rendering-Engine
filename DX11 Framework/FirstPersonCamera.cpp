#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera(CameraInitData cam, Vector3 lookDirection)
	: Camera(cam)
{
	m_LookSens = 0.0005f;
	m_MovementSens = 50.0f;
	m_LookDirection = lookDirection;
	m_MoveDirection = m_LookDirection;
}

FirstPersonCamera::~FirstPersonCamera() {}

void FirstPersonCamera::Update()
{
	if (!enabled) return;

#pragma region Movement
	// W -> +Z
	if (GetAsyncKeyState(0x57))
		TranslatePosition(m_LookDirection * Vector3(m_MovementSens, m_MovementSens, m_MovementSens));
	// S -> -Z
	if (GetAsyncKeyState(0x53))
		TranslatePosition(m_LookDirection * Vector3(-m_MovementSens, -m_MovementSens, -m_MovementSens));
	
	// Q -> +Y
	if (GetAsyncKeyState(0x51))
		TranslatePosition(Vector3(0.0f, m_MovementSens, 0.0f));
	// E -> _Y
	if (GetAsyncKeyState(0x45))
		TranslatePosition(Vector3(0.0f, -m_MovementSens, 0.0f));
#pragma endregion

#pragma region Look
	if (angle > 360.0f)
		angle = 0.0f;
	if (angle < 0.0f)
		angle = 360.0f;

	if (GetAsyncKeyState(VK_RIGHT))
	{
		angle -= m_LookSens;
		m_LookDirection.x = cos(angle);
		m_LookDirection.z = sin(angle);

	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		angle += m_LookSens;
		m_LookDirection.x = cos(angle);
		m_LookDirection.z = sin(angle);
	}
	if (GetAsyncKeyState(VK_UP))
	{
		angle -= m_LookSens;
		m_LookDirection.z = sin(angle);
		m_LookDirection.y = cos(angle);
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		angle += m_LookSens;
		m_LookDirection.z = sin(angle);
		m_LookDirection.y = cos(angle);
	}

#pragma endregion

	// Clamp look direction between -1 and 1
	m_LookDirection = Vector3::Clamp(m_LookDirection, Vector3(-1.0f, -1.0f, -1.0f), Vector3::One());

	SetLens();
}

void FirstPersonCamera::SetLens()
{ 
	XMVECTOR Eye = Vector3::V3ToXMVECTOR(m_Position);
	XMVECTOR Direction = Vector3::V3ToXMVECTOR(m_LookDirection);
	XMVECTOR Up = Vector3::V3ToXMVECTOR(m_Up);

	XMStoreFloat4x4(&m_View, XMMatrixLookToLH(Eye, Direction, Up));
	XMStoreFloat4x4(&m_Proj, XMMatrixPerspectiveFovLH(XM_PIDIV2, m_WindowWidth / (FLOAT)m_WindowHeight, m_NearZ, m_FarZ));
}

void FirstPersonCamera::SetMoveSpeed(float move) { m_MovementSens = move; }

void FirstPersonCamera::SetLookSpeed(float look) { m_LookSens = look; }
