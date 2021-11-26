#pragma once
#ifndef FIRSTPERSONCAMERA_H
#define FIRSTPERSONCAMERA_H
#include "Core.h"
#include "Camera.h"


class FirstPersonCamera  : public Camera
{
public:
	FirstPersonCamera(Vector3 position, Vector3 at, Vector3 up, float windowWidth, float windowHeight, float nearZ, float farZ, float xSens, float ySens);
	~FirstPersonCamera();

	void Update() override;


private:
	float m_SensX;
	float m_SensY;
};
#endif // !FIRSTPERSONCAMERA_H
