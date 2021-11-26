#pragma once
#ifndef ORBITCAMERA_H
#define ORBITCAMERA_H

#include "Camera.h"

enum class OrbitMode
{
	Counter_Clockwise = -1,
	None = 0,
	Clockwise = 1
};

class OrbitCamera : public Camera
{
public:
	OrbitCamera(CameraInitData cam, OrbitMode mode, Vector3 orbitPoint = Vector3(), float orbitSpeed = 1.0f, float radius = 5.0f);
	~OrbitCamera();

	void Update() override;
	void SetLens() override;

	void SetSpeed(float speed);
	void SetOrbitMode(OrbitMode mode);

	void SetOrbitPoint(Vector3 point);

private:
	OrbitMode m_OrbitMode;

	float m_Radius;
	float m_OrbitSpeed;
	float m_Angle;
};
#endif // !ORBITCAMERA_H
