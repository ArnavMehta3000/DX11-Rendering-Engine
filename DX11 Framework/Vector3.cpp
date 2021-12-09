#include "Vector3.h"

Vector3::Vector3(float p_x, float p_y, float p_z) : x(p_x), y(p_y), z(p_z) {}
Vector3::Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}

float Vector3::Dot(const Vector3& other)
{
	return (x * other.x + y * other.y + z * other.z);
}

Vector3 Vector3::Cross(const Vector3& other)
{
	return Vector3((y * other.z - z * other.y), (z * other.x - x * other.z), (x * other.y - y * other.x));
}

Vector3 Vector3::Normalize()
{
	return Vector3(x / Magnitude(), y / Magnitude(), z / Magnitude());;
}

float Vector3::Square()
{
	return ((x * x) + (y * y) + (z * z));
}

float Vector3::Distance(const Vector3& other)
{
	float X = other.x - x;
	float Y = other.y - y;
	return (sqrt((X * X) + (Y * Y)));
}

float Vector3::SqrDistance(const Vector3& other)
{
	float dist = Distance(other);
	return (dist * dist);
}

float Vector3::Magnitude()
{
	return (sqrt(Square()));
}

void Vector3::Debug(std::string msg)
{
	std::ostringstream os;

	if (msg == "")
		os << "Vector3(" << x << "," << y << "," << z << ")" << std::endl;
	else
		os << "[" << msg <<"]Vector3(" << x << "," << y << "," << z << ")" << std::endl;

	OutputDebugStringA(os.str().c_str());
}


DirectX::XMFLOAT3 Vector3::V3ToXMFLOAT3(const Vector3& vec)
{
	return DirectX::XMFLOAT3(vec.x, vec.y, vec.z);
}

Vector3 Vector3::XMFLOAT3ToV3(const DirectX::XMFLOAT3& other)
{
	return Vector3(other.x, other.y, other.z);
}


DirectX::XMVECTOR Vector3::V3ToXMVECTOR(const Vector3& vec)
{
	return DirectX::XMVectorSet(vec.x, vec.y, vec.z, 0.0f);
}

Vector3 Vector3::XMVECTORToV3(const DirectX::XMVECTOR& other)
{
	DirectX::XMFLOAT3 output;
	DirectX::XMStoreFloat3(&output, other);
	return XMFLOAT3ToV3(output);
}
