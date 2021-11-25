#include "FirstPersonCamera.h"

FirstPersonCamera::FirstPersonCamera(Vector3 position, Vector3 at, Vector3 up, float windowWidth, float windowHeight, float nearZ, float farZ)
	: Camera(position, at, up, windowWidth, windowHeight, nearZ, farZ)
{
}

FirstPersonCamera::~FirstPersonCamera() {}

void FirstPersonCamera::Update()
{
	OutputDebugString(L"Helleo");
	Camera::Update();
}
