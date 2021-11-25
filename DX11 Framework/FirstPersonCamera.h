#pragma once
#include "Camera.h"

class Camera;

class FirstPersonCamera : public Camera
{
public:
	FirstPersonCamera(Vector3 position, Vector3 at, Vector3 up, float windowWidth, float windowHeight, float nearZ, float farZ);
	~FirstPersonCamera() {}

	void Update() override;
};

