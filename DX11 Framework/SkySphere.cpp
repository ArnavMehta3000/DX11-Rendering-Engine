#include "SkySphere.h"

SkySphere::SkySphere(const char* initFile, ID3D11DeviceContext* deviceContext, ID3D11Buffer* constantBuffer, ID3D11Device* device)
	: GameObject(initFile, deviceContext, constantBuffer)
{
	SetMesh("Assets/Models/Blender/sphere.obj", device, false);

	// Load texture using json file
	using json = nlohmann::json;
	json jFile;
	std::ifstream input(initFile);
	input >> jFile;

	const std::string tex = jFile["texture"].get<std::string>();
	std::wstring textureFile = std::wstring(tex.begin(), tex.end());

	LoadTexture(device, textureFile.c_str());
}

SkySphere::SkySphere(GOInitData goData, ID3D11Device* device, const wchar_t* textureFile)
	: GameObject(goData)
{
	SetMesh("Assets/Models/Blender/sphere.obj", device, false);
	LoadTexture(device, textureFile);
}

SkySphere::~SkySphere() {}