#pragma once
#ifndef FIRSTPERSONCAMERA_H
#define FIRSTPERSONCAMERA_H
#include "Core.h"
#include "Camera.h"

// WASDQE to move camera, Arrow keys to change look at point
class FirstPersonCamera  : public Camera
{
public:
	FirstPersonCamera(Vector3 position, Vector3 at, Vector3 up, float windowWidth, float windowHeight, float nearZ, float farZ);
	~FirstPersonCamera();

	void Update() override;
	void SetLens() override;

	void SetMoveSpeed(float move = 5.0f);
	void SetLookSpeed(float look = 5.0f);

private:
	float m_MovementSens;
	float m_LookSens;
		
};
#endif // !FIRSTPERSONCAMERA_H
