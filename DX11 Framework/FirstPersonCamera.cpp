#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera(CameraInitData cam, Vector3 lookDirection)
	: Camera(cam)
{
	m_LookSens = 0.0005f;
	m_MovementSens = 10.0f;
	m_LookDirection = lookDirection;
}

FirstPersonCamera::~FirstPersonCamera() {}

void FirstPersonCamera::Update()
{
	if (!enabled) return;

#pragma region Movement
	// W -> +Z
	if (GetAsyncKeyState(0x57))
		TranslatePosition(Vector3(0.0f, 0.0f, m_MovementSens));
	// S -> -Z
	if (GetAsyncKeyState(0x53))
		TranslatePosition(Vector3(0.0f, 0.0f, -m_MovementSens));
	// D -> +X
	if (GetAsyncKeyState(0x44))
		TranslatePosition(Vector3(m_MovementSens, 0.0f, 0.0f));
	// A -> -X
	if (GetAsyncKeyState(0x41))
		TranslatePosition(Vector3(-m_MovementSens, 0.0f, 0.0f));
	// Q -> +Y
	if (GetAsyncKeyState(0x51))
		TranslatePosition(Vector3(0.0f, m_MovementSens, 0.0f));
	// E -> _Y
	if (GetAsyncKeyState(0x45))
		TranslatePosition(Vector3(0.0f, -m_MovementSens, 0.0f));
#pragma endregion

#pragma region Look
	// Up Arrow -> Increase Y
	//if (GetAsyncKeyState(VK_UP))
	//	m_LookDirection.y += m_LookSens;
	//// Down Arrow -> Decrease Y
	//if (GetAsyncKeyState(VK_DOWN))
	//	m_LookDirection.y -= m_LookSens;
	// Right Arrow -> Increase X
	if (GetAsyncKeyState(VK_RIGHT))
	{
		
		if (m_LookDirection.x >= 0.0f && m_LookDirection.z <= 0.0f)
		{
			// Increase X
			m_LookDirection.x += m_LookSens;

			// Reached X limit decrease Z
			if (m_LookDirection.x >= 1.0f)
				m_LookDirection.z -= m_LookSens;
		}
		//m_LookDirection.x += m_LookSens;

		//// Reached right limit go back
		//if (m_LookDirection.x >= 1.0f)
		//	m_LookDirection.z -= m_LookSens;
		//
		//// Reached back limit
		//if (m_LookDirection.z < -1.0f)
		//	m_LookDirection.z += m_LookSens;
	}

	//// Left Arrow -> Decrease X
	//if (GetAsyncKeyState(VK_LEFT))
	//{
	//	m_LookDirection.x -= m_LookSens;

	//}
#pragma endregion

	// Clamp look direction
	m_LookDirection = Vector3::Clamp(m_LookDirection, Vector3(-1.0f, -1.0f, -1.0f), Vector3::One());
	SetLens();
}

void FirstPersonCamera::SetLens()
{ 
	Vector3 look = m_LookDirection;
	XMVECTOR Eye = Vector3::V3ToXMVECTOR(m_Position);
	XMVECTOR Direction = Vector3::V3ToXMVECTOR(look);
	XMVECTOR Up = Vector3::V3ToXMVECTOR(m_Up);
	look.Debug();
	XMStoreFloat4x4(&m_View, XMMatrixLookToLH(Eye, Direction, Up));
	XMStoreFloat4x4(&m_Proj, XMMatrixPerspectiveFovLH(XM_PIDIV2, m_WindowWidth / (FLOAT)m_WindowHeight, m_NearZ, m_FarZ));
}

void FirstPersonCamera::SetMoveSpeed(float move) { m_MovementSens = move; }

void FirstPersonCamera::SetLookSpeed(float look) { m_LookSens = look; }
