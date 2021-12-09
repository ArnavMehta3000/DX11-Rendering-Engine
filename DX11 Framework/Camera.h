#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Core.h"

struct CameraInitData
{
	Vector3 position = Vector3();
	Vector3 at = Vector3();
	Vector3 up = Vector3();
	float windowWidth = 0;
	float windowHeight = 0;
	float nearZ = 0;
	float farZ = 0;
};

struct CameraMats
{
	XMFLOAT4X4 View;
	XMFLOAT4X4 Projection;

	XMFLOAT4X4 GetCombined()
	{
		XMMATRIX view = XMLoadFloat4x4(&View);
		XMMATRIX proj = XMLoadFloat4x4(&Projection);
		XMFLOAT4X4 output;
		XMStoreFloat4x4(&output, view * proj);

		return output;
	}
};

class Camera
{
public:
	Camera(CameraInitData cam);
	~Camera();

	virtual void Update();

	// Get/Set world camera position
	inline Vector3  GetPosition() const { return m_Position; }
	void SetPosition(float x, float y, float z);
	void SetPosition(const Vector3& pos);
	void SetLookAt(float x, float y, float z);
	void SetLookAt(const Vector3& at);
	void SetUp(float x, float y, float z);
	void SetUp(const Vector3& up);
	void TranslatePosition(Vector3 delta);
	void TranslateLookAt(Vector3 delta);

	// Get camera basis vectors
	inline Vector3 GetAt() const { return m_At; }
	inline Vector3 GetUp()const { return m_Up; }

	// Get/Set frustum properties
	inline float GetNearZ()const { return m_NearZ; }
	inline float GetFarZ()const { return m_FarZ; }
	void SetNearZ(float nearZ);
	void SetFarZ(float farZ);

	// Define camera space via LookAt parameters.

	// Get View/Proj matrices.
	inline XMFLOAT4X4 GetView()const { return m_View; }
	inline XMFLOAT4X4 GetProj()const { return m_Proj; }
	XMFLOAT4X4 GetViewProj()const;


	bool enabled = false;


protected:
	// Camera coordinate relative to world space.
	Vector3 m_Position; //EYE
	Vector3 m_At;
	Vector3 m_Up;

	Vector3 m_CamForward;
	Vector3 m_CamRight;

	float m_NearZ;
	float m_FarZ;
	float m_WindowWidth, m_WindowHeight;

	XMFLOAT4X4 m_View;
	XMFLOAT4X4 m_Proj;

	virtual void SetLens();
};
#endif // !CAMERA_H
