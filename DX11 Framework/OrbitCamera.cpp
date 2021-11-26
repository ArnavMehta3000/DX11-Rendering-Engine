#include "OrbitCamera.h"

OrbitCamera::OrbitCamera(CameraInitData cam, OrbitMode mode, Vector3 orbitPoint, float orbitSpeed, float radius)
	: Camera(cam)
{
	m_OrbitMode = mode;
	m_OrbitSpeed = orbitSpeed / 1000;
	m_Radius = radius;
	m_At = orbitPoint;
	m_Angle = 0;
}

OrbitCamera::~OrbitCamera() {}

void OrbitCamera::Update()
{
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

	Vector3 vec = Vector3(x, 0, z );
	SetPosition(vec);

		
	SetLens();
}

void OrbitCamera::SetLens()
{
	Camera::SetLens();
}

void OrbitCamera::SetSpeed(float speed) { m_OrbitSpeed = speed; }

void OrbitCamera::SetOrbitMode(OrbitMode mode) { m_OrbitMode = mode; }

void OrbitCamera::SetOrbitPoint(Vector3 point) { m_At = point; }