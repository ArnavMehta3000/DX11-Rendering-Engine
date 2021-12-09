#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <string>
#include "Core.h"


struct TransformData
{
	Vector3 position, rotation, scale;

	TransformData(XMFLOAT4X4 mat)
	{
		// Decompose the matrix
		XMVECTOR scaling, rotQuat, trans;
		XMMatrixDecompose(&scaling, &rotQuat, &trans, XMLoadFloat4x4(&mat));

		// Set position / rotation / scale
		position = Vector3::XMVECTORToV3(trans);
		rotation = Vector3::XMVECTORToV3(rotQuat);
		scale = Vector3::XMVECTORToV3(scaling);
	}
};

class Transform
{
public:
	Transform(Vector3 pos = Vector3(), Vector3 rot = Vector3(), Vector3 scale = Vector3())
	{
		XMVECTOR p = Vector3::V3ToXMVECTOR(pos);
		XMVECTOR r = Vector3::V3ToXMVECTOR(rot);
		XMVECTOR s = Vector3::V3ToXMVECTOR(scale);

		// Create world matrix
		XMStoreFloat4x4(&worldMat, XMMatrixAffineTransformation(s, p, p, p));
	}

	inline XMFLOAT4X4 GetWorldMatrix() { return worldMat; }
	inline XMMATRIX   GetWorldMatrixXM() { return XMLoadFloat4x4(&worldMat); }

	inline Vector3 GetPosition() const { return TransformData(worldMat).position; }
	inline Vector3 GetRotation() const { return TransformData(worldMat).rotation; }
	inline Vector3 GetScale()    const { return TransformData(worldMat).scale; }
	

	
	inline void TranslatePosition(Vector3 vec)
	{
		Vector3 pos = GetPosition();
		XMMATRIX t = XMMatrixTranslation(pos.x + vec.x, pos.y + vec.y, pos.z + vec.z);
		XMStoreFloat4x4(&worldMat, t);
	}
	inline void TranslateScale(Vector3 vec)
	{
		Vector3 scale = GetScale();
		XMMATRIX s = XMMatrixScaling(scale.x + vec.x, scale.y + vec.y, scale.z + vec.z);
		XMStoreFloat4x4(&worldMat, s);
	}
	inline void TranslateScale(float vec)
	{
		Vector3 scale = GetScale();
		XMMATRIX s = XMMatrixScaling(scale.x + vec, scale.y + vec, scale.z + vec);
		XMStoreFloat4x4(&worldMat, s);
	}
	
	inline void SetPosition(Vector3 pos)
	{
		XMStoreFloat4x4(&worldMat, XMMatrixTranslation(pos.x, pos.y, pos.z));
	}
	inline void SetRotation(Vector3 axis, float angle)
	{
		XMMATRIX r = XMMatrixRotationAxis(Vector3::V3ToXMVECTOR(axis), angle);
		XMStoreFloat4x4(&worldMat, r);
	}
	inline void SetScale(Vector3 scale)
	{
		XMStoreFloat4x4(&worldMat, XMMatrixScaling(scale.x, scale.y, scale.z));
	}
	inline void SetScale(float scale)
	{
		XMStoreFloat4x4(&worldMat, XMMatrixScaling(scale, scale, scale));
	}

	inline Vector3 GetForward()
	{
		Vector3 forward;
		Vector3 rot = GetRotation();

		forward.x = sin(rot.y) * cos(rot.x);
		forward.y = sin(-rot.x);
		forward.z = cos(rot.x) * cos(rot.y);

		return forward;
	}

	inline void Debug(std::string posMsg = "", std::string rotMsg = "", std::string scaleMsg = "")
	{
		GetPosition().Debug(posMsg);
		GetRotation().Debug(rotMsg);
		GetScale().Debug(scaleMsg);
	}

private:
	XMFLOAT4X4 worldMat;

	Vector3 position, rotation, scale;
};


//class Transform
//{
//public:  // Constructors
//	Transform(Vector3 pos = Vector3(), Vector3 rot = Vector3(), Vector3 scale = Vector3())
//	{
//		XMVECTOR p = Vector3::V3ToXMVECTOR(pos);
//		XMVECTOR r = Vector3::V3ToXMVECTOR(rot);
//		XMVECTOR s = Vector3::V3ToXMVECTOR(scale);
//
//		// Create world matrix
//		UpdateMatrix(XMMatrixAffineTransformation(s, p, p, p));
//	}
//	~Transform() {}
//
//public:  // Getter methods
//	inline Vector3    GetPosition() { return m_Position; }
//	inline Vector3    GetRotation() { return m_Rotation; }
//	inline Vector3    GetScale()    { return m_Scale; }
//	inline XMFLOAT4X4 GetWorldMatrix() { return m_WorldMat; }
//	inline XMMATRIX   GetWorldMatrixXM() { return XMLoadFloat4x4(&m_WorldMat); }
//
//public:  // Setter methods
//	inline void SetPosition(Vector3 pos)
//	{
//		UpdateMatrix(XMMatrixTranslation(pos.x, pos.y, pos.z));
//	}
//	inline void SetRotation(Vector3 axis, float angle)
//	{
//		XMMATRIX r = XMMatrixRotationAxis(Vector3::V3ToXMVECTOR(axis), angle);
//		UpdateMatrix(r);
//	}
//	inline void SetScale(Vector3 scale)
//	{
//		UpdateMatrix(XMMatrixScaling(scale.x, scale.y, scale.y));
//	}
//	inline void SetScale(float scale)
//	{
//		UpdateMatrix(XMMatrixScaling(scale, scale, scale));
//	}
//
//private:
//	XMFLOAT4X4 m_WorldMat;
//	Vector3 m_Position, m_Rotation, m_Scale;
//
//
//	void UpdateMatrix(XMMATRIX newMat)
//	{
//		//Update world matrix
//		XMStoreFloat4x4(&m_WorldMat, newMat);
//
//		// Decompose the new matrix
//		XMVECTOR scaling, rotQuat, trans;
//		XMMatrixDecompose(&scaling, &rotQuat, &trans, XMLoadFloat4x4(&m_WorldMat));
//
//		// Update position / rotation / scale
//		m_Position = Vector3::XMVECTORToV3(trans);
//		m_Rotation = Vector3::XMVECTORToV3(rotQuat);
//		m_Scale = Vector3::XMVECTORToV3(scaling);
//	}
//};

#endif // !TRANSFORM_H

