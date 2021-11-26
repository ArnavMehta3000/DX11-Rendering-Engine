#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera(Vector3 position, Vector3 at, Vector3 up, float windowWidth, float windowHeight, float nearZ, float farZ, float xSens, float ySens)
	: Camera(position, at, up, windowWidth, windowHeight, nearZ, farZ)
{
	m_SensX = xSens;
	m_SensY = ySens;
}

FirstPersonCamera::~FirstPersonCamera() {}

void FirstPersonCamera::Update()
{
	Camera::Update();
}
