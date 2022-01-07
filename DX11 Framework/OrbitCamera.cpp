#include "OrbitCamera.h"

OrbitCamera::OrbitCamera(CameraInitData cam, OrbitMode mode, Vector3 orbitPoint, float orbitSpeed, float radius)
	: Camera(cam)
{
	m_OrbitMode = mode;
	m_OrbitSpeed = orbitSpeed / 1000;
	m_Radius = radius;
	m_At = orbitPoint;
	m_Angle = 0;

	m_MoveSpeed = 0.5f;
}

OrbitCamera::~OrbitCamera() {}

void OrbitCamera::Update()
{
	if (!enabled) return;

	// Update angle
	switch (m_OrbitMode)
	{
	case OrbitMode::Counter_Clockwise:
		m_Angle -= m_OrbitSpeed;
		if (m_Angle < 0)
			m_Angle = 359;
		
		break;

	case OrbitMode::None:
		break;

	case OrbitMode::Clockwise:
		m_Angle += m_OrbitSpeed;
		if (m_Angle > 359)
			m_Angle = 0;
		break;
	};

	float angleInRad = m_Angle * (XM_PI / 180);

	
	float x = m_At.x + (m_Radius * sin(angleInRad));
	float z = m_At.z + (m_Radius * cos(angleInRad));
	
	if (GetAsyncKeyState(0x51))  // Q Key
		m_Position.y += m_MoveSpeed;
	if (GetAsyncKeyState(0x45))  // E Key
		m_Position.y -= m_MoveSpeed;	

	Vector3 pos = Vector3(x, m_Position.y, z);
	SetPosition(pos);
	

	SetLens();
}

void OrbitCamera::SetLens()
{
	Camera::SetLens();
}

void OrbitCamera::SetMoveSpeed(float mSpeed) { m_MoveSpeed = mSpeed; }

void OrbitCamera::SetSpeed(float speed) { m_OrbitSpeed = speed; }

void OrbitCamera::SetRadius(float rad)
{
	m_Radius = rad;
}

void OrbitCamera::SetOrbitMode(OrbitMode mode) { m_OrbitMode = mode; }