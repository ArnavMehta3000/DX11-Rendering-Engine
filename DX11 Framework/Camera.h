#pragma once
#include "Core.h"


class Camera
{
private:
	Vector3    m_eye;
	Vector3    m_at;
	Vector3    m_up;
	float      m_windowWidth;
	float      m_windowHeight;
	float      m_nearDepth;
	float      m_farDepth;

	XMFLOAT4X4 m_view;
	XMFLOAT4X4 m_projection;

public:
	/// <summary>
	/// Create a camera
	/// </summary>
	/// <param name="position">Position of camera in world coordinates</param>
	/// <param name="at">Target position the camera is looking 'At'</param>
	/// <param name="up">Define the 'Up-Axis'</param>
	/// <param name="nearDepth">Near clip plane of the camera</param>
	/// <param name="farDepth">Far clip plane of the camera</param>
	Camera(Vector3 position, Vector3 at, Vector3 up, float windowWidth, float windowHeight, float nearDepth, float farDepth);
	~Camera() {}

	void Update();
	
	inline Vector3 GetEye()                 { return m_eye; }
	inline Vector3 GetAt()                  { return m_at; }
	inline Vector3 GetUp()                  { return m_up; }
	inline void SetEye(Vector3 value)       { return m_eye; }
	inline void SetAt(Vector3 value)        { return m_at; }
	inline void SetUp(Vector3 value)        { return m_up; }
	inline float GetWindowWidth()           { return m_windowWidth; }
	inline float GetWindowHeight()          { return m_windowHeight; }
	inline float GetNearDepth()             { return m_nearDepth; }
	inline float GetFarDepth()              { return m_farDepth; }
	inline void SetNearDepth(float value)   { m_nearDepth = value; }
	inline void SetFarDepth(float value)    { m_farDepth = value; }
	inline XMFLOAT4X4 GetViewMatrix()       { return m_view; }
	inline XMFLOAT4X4 GetProjectionMatrix() { return m_projection; }

	void Reshape(float windowWidth, float windowHeight, float nearDepth, float farDepth);
};

