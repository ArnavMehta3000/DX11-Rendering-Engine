#pragma once
#ifndef FIRSTPERSONCAMERA_H
#define FIRSTPERSONCAMERA_H
#include "Core.h"
#include "Camera.h"


class FirstPersonCamera  : public Camera
{
public:
	FirstPersonCamera(Vector3 position, Vector3 at, Vector3 up, float windowWidth, float windowHeight, float nearZ, float farZ);
	~FirstPersonCamera();

	void Update() override;
	void SetLens() override;

	void SetMoveSpeed(float xMove, float zMove);
	void SetLookSpeed(float xSens, float ySens);

private:
	float m_LookSensX, m_LookSensY;
	float m_MoveSensX, m_MoveSensY;
		
};
#endif // !FIRSTPERSONCAMERA_H
