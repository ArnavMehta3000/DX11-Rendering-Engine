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

	void SetMoveSpeed(float move = 5.0f);
	void SetLookSpeed(float look = 5.0f);

private:
	float m_MovementSens;
	float m_LookSens;
	Vector3 m_LookDirection;
	
	bool flip = false;
	float angle = 0;
};
#endif // !FIRSTPERSONCAMERA_H
