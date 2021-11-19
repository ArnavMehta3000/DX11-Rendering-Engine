#pragma once
#include "Core.h"


class Camera
{
private:
	XMFLOAT3 m_eye;
	XMFLOAT3 m_at;
	XMFLOAT3 m_up;
	FLOAT    m_windowWidth;
	FLOAT    m_windowHeight;
	FLOAT    m_nearDepth;
	FLOAT    m_farDepth;

	XMFLOAT4X4 m_view;
	XMFLOAT4X4 m_projection;

public:
	Camera(XMFLOAT3 position, XMFLOAT3 at, XMFLOAT3 up, FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
	~Camera();

	void Update();
	
	// Getters and setters for private variables

	// Getters for the view/projection/combined matrices

	void Reshape(FLOAT windowWidth, FLOAT windowHeight, FLOAT nearDepth, FLOAT farDepth);
};

