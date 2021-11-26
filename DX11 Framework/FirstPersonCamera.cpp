#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera(Vector3 position, Vector3 at, Vector3 up, float windowWidth, float windowHeight, float nearZ, float farZ)
	: Camera(position, at, up, windowWidth, windowHeight, nearZ, farZ)
{
	m_LookSensX = 1.0f;
	m_LookSensY = 1.0f;

	m_MoveSensX = 5.0f;
	m_MoveSensY = 5.0f;
}

FirstPersonCamera::~FirstPersonCamera() {}

void FirstPersonCamera::Update()
{
	SetLens();
}

void FirstPersonCamera::SetLens()
{
	
}

void FirstPersonCamera::SetMoveSpeed(float xMove, float zMove) { m_MoveSensX = xMove; m_MoveSensY = zMove; }

void FirstPersonCamera::SetLookSpeed(float xSens, float ySens) { m_LookSensX = xSens; m_LookSensY = ySens; }
