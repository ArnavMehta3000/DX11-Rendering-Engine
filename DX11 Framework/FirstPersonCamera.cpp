#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera(CameraInitData cam, Vector3 lookDirection)
	: Camera(cam)
{
	m_LookDirection = lookDirection;
	m_MoveSpeed = 0.01f;
	m_LookSpeed = 0.0005f;
}

FirstPersonCamera::~FirstPersonCamera() {}

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
	if (GetAsyncKeyState(VK_PRIOR))  // Pressed Page Up
		m_Position.y += m_MoveSpeed;

	if (GetAsyncKeyState(VK_NEXT))  // Pressed Page Down
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
