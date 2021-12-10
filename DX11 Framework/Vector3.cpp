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

Vector3 Vector3::Clamp(Vector3 vec, Vector3 min, Vector3 max)
{
	// X axis
	if (vec.x >= max.x) vec.x = max.x;
	if (vec.x < min.x)  vec.x = min.x;

	// Y axis
	if (vec.y >= max.y) vec.y = max.y;
	if (vec.y < min.y)  vec.y = min.y;

	// Z axis
	if (vec.z >= max.z) vec.z = max.z;
	if (vec.z < min.z)  vec.z = min.z;

	return vec;
}

Vector3 Vector3::Clamp01(Vector3 vec)
{
	return Clamp(vec, Vector3::Zero(), Vector3::One());
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

std::string Vector3::Debug(std::string msg)
{
	std::ostringstream os;

	if (msg == "")
		os << "Vector3(" << x << "," << y << "," << z << ")" << std::endl;
	else
		os << "[" << msg <<"]Vector3(" << x << "," << y << "," << z << ")" << std::endl;

	std::string output = os.str();
	OutputDebugStringA(output.c_str());
	
	return output;
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
