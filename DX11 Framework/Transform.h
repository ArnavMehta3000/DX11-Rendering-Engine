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

	XMFLOAT4X4 GetWorldMatrix() { return worldMat; }
	XMMATRIX   GetWorldMatrixXM() { return XMLoadFloat4x4(&worldMat); }

	Vector3    GetPosition() { return TransformData(worldMat).position; }
	Vector3    GetRotation() { return TransformData(worldMat).rotation; }
	Vector3    GetScale()    { return TransformData(worldMat).scale; }
	

	
	void TranslatePosition(Vector3 vec)
	{
		Vector3 pos = GetPosition();
		XMMATRIX t = XMMatrixTranslation(pos.x + vec.x, pos.y + vec.y, pos.z + vec.z);
		XMStoreFloat4x4(&worldMat, t);
	}
	void TranslateScale(Vector3 vec)
	{
		Vector3 scale = GetScale();
		XMMATRIX s = XMMatrixScaling(scale.x + vec.x, scale.y + vec.y, scale.z + vec.z);
		XMStoreFloat4x4(&worldMat, s);
	}
	
	void SetPosition(Vector3 pos)
	{
		XMStoreFloat4x4(&worldMat, XMMatrixTranslation(pos.x, pos.y, pos.z));
	}
	void SetRotation(Vector3 axis, float angle)
	{
		XMMATRIX r = XMMatrixRotationAxis(Vector3::V3ToXMVECTOR(axis), angle);
		XMStoreFloat4x4(&worldMat, r);
	}
	void SetScale(Vector3 scale)
	{
		XMStoreFloat4x4(&worldMat, XMMatrixScaling(scale.x, scale.y, scale.z));
	}


	void Debug(std::string posMsg = "", std::string rotMsg = "", std::string scaleMsg = "")
	{
		GetPosition().Debug(posMsg);
		GetRotation().Debug(rotMsg);
		GetScale().Debug(scaleMsg);
	}

private:
	XMFLOAT4X4 worldMat;
};

#endif // !TRANSFORM_H

