#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera(CameraInitData cam)
	: Camera(cam)
{
	m_LookSens = 5.0f;
	m_MovementSens = 5.0f;
}

FirstPersonCamera::~FirstPersonCamera() {}

void FirstPersonCamera::Update()
{
	#pragma region Movement
	// W -> +Z
	if (GetAsyncKeyState(0x57))
	{
		TranslatePosition(Vector3(0.0f, 0.0f, m_MovementSens));
		TranslateLookAt(Vector3(0.0f, 0.0f, m_LookSens));
	}
	// S -> -Z
	if (GetAsyncKeyState(0x53))
	{
		TranslatePosition(Vector3(0.0f, 0.0f, -m_MovementSens));
		TranslateLookAt(Vector3(0.0f, 0.0f, -m_LookSens));
	}

	// D -> +X
	if (GetAsyncKeyState(0x44))
	{
		TranslatePosition(Vector3(m_MovementSens, 0.0f, 0.0f));
		TranslateLookAt(Vector3(m_LookSens, 0.0f, 0.0f));
	}

	// A -> -X
	if (GetAsyncKeyState(0x41))
	{
		TranslatePosition(Vector3(-m_MovementSens, 0.0f, 0.0f));
		TranslateLookAt(Vector3(-m_LookSens, 0.0f, 0.0f));
	}

	// Q -> +Y
	if (GetAsyncKeyState(0x51))
	{
		TranslatePosition(Vector3(0.0f, m_MovementSens, 0.0f));
		TranslateLookAt(Vector3(0.0f, m_LookSens, 0.0f));
	}

	// E -> _Y
	if (GetAsyncKeyState(0x45))
	{
		TranslatePosition(Vector3(0.0f, -m_MovementSens, 0.0f));
		TranslateLookAt(Vector3(0.0f, -m_LookSens, 0.0f));
	}
	#pragma endregion


	#pragma region Look
	// W -> +Z
	if (GetAsyncKeyState(VK_UP))
	{
		TranslateLookAt(Vector3(0.0f, 0.0f, m_LookSens));
	}
	// S -> -Z
	if (GetAsyncKeyState(VK_DOWN))
	{
		TranslateLookAt(Vector3(0.0f, 0.0f, -m_LookSens));
	}

	// D -> +X
	if (GetAsyncKeyState(VK_RIGHT))
	{
		TranslateLookAt(Vector3(m_LookSens, 0.0f, 0.0f));
	}

	// A -> -X
	if (GetAsyncKeyState(VK_LEFT))
	{
		TranslateLookAt(Vector3(-m_LookSens, 0.0f, 0.0f));
	}

	// Q -> +Y
	if (GetAsyncKeyState(VK_PRIOR))
	{
		TranslateLookAt(Vector3(0.0f, m_LookSens, 0.0f));
	}

	// E -> _Y
	if (GetAsyncKeyState(VK_NEXT))
	{
		TranslateLookAt(Vector3(0.0f, -m_LookSens, 0.0f));
	}
	#pragma endregion


	SetLens();
}

void FirstPersonCamera::SetLens()
{
	Camera::SetLens();
}

void FirstPersonCamera::SetMoveSpeed(float move) { m_MovementSens = move; }

void FirstPersonCamera::SetLookSpeed(float look) { m_LookSens = look; }
