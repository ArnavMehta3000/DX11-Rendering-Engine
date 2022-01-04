#pragma once
#ifndef FIRSTPERSONCAMERA_H
#define FIRSTPERSONCAMERA_H
#include "Camera.h"

// WASDQE to move camera, Arrow keys to change look at point
class FirstPersonCamera  : public Camera
{
public:
	FirstPersonCamera(CameraInitData cam, Vector3 lookDirection);
	~FirstPersonCamera();

	void Update() override;
	void SetLens() override;

private:
	Vector3 m_LookDirection;

	float m_MoveSpeed;
	float m_LookSpeed;

	bool flipped = false;
};
#endif // !FIRSTPERSONCAMERA_H
