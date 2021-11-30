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
	


	SetLens();
}

void FirstPersonCamera::SetLens()
{
	Camera::SetLens();
}

void FirstPersonCamera::SetMoveSpeed(float move) { m_MovementSens = move; }

void FirstPersonCamera::SetLookSpeed(float look) { m_LookSens = look; }
