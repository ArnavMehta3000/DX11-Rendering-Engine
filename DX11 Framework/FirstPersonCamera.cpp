#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera(CameraInitData cam, Vector3 lookDirection)
	: Camera(cam)
{
	/*m_LookSens = 0.0005f;
	m_MovementSens = 50.0f;
	m_LookDirection = lookDirection;*/


	m_LookDirection = Vector3(0.0f, 0.0f, 1.0f);
	m_MoveSpeed = 0.01f;
	m_LookSpeed = 0.0005f;
}

FirstPersonCamera::~FirstPersonCamera() {}

//void FirstPersonCamera::Update()
//{
//	if (!enabled) return;
//	#pragma region Movement
//		// W -> +Z
//	if (GetAsyncKeyState(0x57))
//		TranslatePosition(m_LookDirection * Vector3(m_MovementSens, m_MovementSens, m_MovementSens));
//	// S -> -Z
//	if (GetAsyncKeyState(0x53))
//		TranslatePosition(m_LookDirection * Vector3(-m_MovementSens, -m_MovementSens, -m_MovementSens));
//
//	// Q -> +Y
//	if (GetAsyncKeyState(0x51))
//		TranslatePosition(Vector3(0.0f, m_MovementSens, 0.0f));
//	// E -> _Y
//	if (GetAsyncKeyState(0x45))
//		TranslatePosition(Vector3(0.0f, -m_MovementSens, 0.0f));
//	#pragma endregion
//
//	#pragma region Look
//	if (angle > 360.0f)
//		angle = 0.0f;
//	if (angle < 0.0f)
//		angle = 360.0f;
//	if (GetAsyncKeyState(VK_RIGHT))
//	{
//		angle -= m_LookSens;
//		m_LookDirection.x = cos(angle);
//		m_LookDirection.z = sin(angle);
//	}
//	if (GetAsyncKeyState(VK_LEFT))
//	{
//		angle += m_LookSens;
//		m_LookDirection.x = cos(angle);
//		m_LookDirection.z = sin(angle);
//	}
//	if (GetAsyncKeyState(VK_UP))
//	{
//		angle -= m_LookSens;
//		m_LookDirection.z = sin(angle);
//		m_LookDirection.y = cos(angle);
//	}
//	if (GetAsyncKeyState(VK_DOWN))
//	{
//		angle += m_LookSens;
//		m_LookDirection.z = sin(angle);
//		m_LookDirection.y = cos(angle);
//	}
//	#pragma endregion
//
//	m_LookDirection = Vector3::Clamp(m_LookDirection, Vector3(-1.0f, -1.0f, -1.0f), Vector3::One());
//	SetLens();
//}
//
//
//void FirstPersonCamera::SetLens()
//{
//	XMVECTOR Eye = Vector3::V3ToXMVECTOR(m_Position);
//	XMVECTOR Direction = Vector3::V3ToXMVECTOR(m_LookDirection);
//	XMVECTOR Up = Vector3::V3ToXMVECTOR(m_Up);
//	XMStoreFloat4x4(&m_View, XMMatrixLookToLH(Eye, Direction, Up));
//	XMStoreFloat4x4(&m_Proj, XMMatrixPerspectiveFovLH(XM_PIDIV2, m_WindowWidth / (FLOAT)m_WindowHeight, m_NearZ, m_FarZ));
//}








void FirstPersonCamera::Update()
{
	// -----Key codes-----
	// W = 0x57
	// A = 0x41
	// S = 0x53
	// D = 0x44
	// UP = VK_UP
	// DOWN = VK_DOWN
	// LEFT = VK_LEFT
	// RIGHT = VK_RIGHT]
	// Page Up = VK_PRIOR
	// Page Dowwn = VK_NEXT
	//

	constexpr char KEYUP = 0x1;

	Vector3 moveSpeed = Vector3(m_MoveSpeed, m_MoveSpeed, m_MoveSpeed);

	// Flip camera directions
	if (GetAsyncKeyState(VK_TAB) & KEYUP)
	{
		m_LookDirection *= Vector3(-1.0f, -1.0f, -1.0f);
		flipped = !flipped;
	}


	#pragma region Movement
	if (GetAsyncKeyState(0x57))  // Pressed W
		m_Position += (m_LookDirection * moveSpeed);

	if (GetAsyncKeyState(0x53))  // Pressed S
		m_Position -= (m_LookDirection * moveSpeed);

	if (GetAsyncKeyState(0x44))  // Pressed D
		m_Position -= (m_LookDirection.Cross(m_Up) * moveSpeed);

	if (GetAsyncKeyState(0x41))  // Pressed A
		m_Position += (m_LookDirection.Cross(m_Up) * moveSpeed);
	#pragma endregion

	#pragma region Looking
	if (GetAsyncKeyState(VK_UP))  // Pressed up arrow
		m_LookDirection.y += m_LookSpeed;

	if (GetAsyncKeyState(VK_DOWN))  // Pressed down arrow
		m_LookDirection.y -= m_LookSpeed;

	if (GetAsyncKeyState(VK_RIGHT))  // Pressed left arrow
	{
		if (!flipped)
			m_LookDirection.x += m_LookSpeed;
		else
			m_LookDirection.x -= m_LookSpeed;
	}

	if (GetAsyncKeyState(VK_LEFT))  // Pressed right arrow
	{
		if (!flipped)
			m_LookDirection.x -= m_LookSpeed;
		else
			m_LookDirection.x += m_LookSpeed;
	}
	#pragma endregion

	#pragma region Change Height
	if (GetAsyncKeyState(VK_PRIOR))
		m_Position.y += m_MoveSpeed;

	if (GetAsyncKeyState(VK_NEXT))
		m_Position.y -= m_MoveSpeed;
	#pragma endregion


	// Clamp the direction between -1 and 1
	m_LookDirection = Vector3::Clamp(m_LookDirection, Vector3(-1.0f, -1.0f, -1.0f), Vector3(1.0f, 1.0f, 1.0f));
	
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
