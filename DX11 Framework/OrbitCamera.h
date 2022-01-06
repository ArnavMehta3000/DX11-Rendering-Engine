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

	void SetMoveSpeed(float mSpeed);
	void SetSpeed(float speed);
	void SetRadius(float rad);
	void SetOrbitMode(OrbitMode mode);

	inline float GetMoveSpeed() { return m_MoveSpeed; }
	inline float GetSpeed() { return m_OrbitSpeed; }
	inline float GetRadius() { return m_Radius; }

private:
	OrbitMode m_OrbitMode;

	float m_MoveSpeed;
	float m_Radius;
	float m_OrbitSpeed;
	float m_Angle;
};
#endif // !ORBITCAMERA_H
