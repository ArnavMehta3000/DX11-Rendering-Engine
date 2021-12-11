#include "PlaneScene.h"

PlaneScene::PlaneScene(std::string name) : Scene(name) {}

PlaneScene::~PlaneScene() {}

void PlaneScene::Init(SceneInitData data)
{
	m_Device = data.device;
	m_DeviceContext = data.deviceCtx;
	m_SwapChain = data.swapChain;
	m_ContantBuffer = data.constantBuffer;

	// Init orbit camera
	CameraInitData cameraInitData;
	cameraInitData.position = Vector3(0.0f, 5.0f, -5.0f);
	cameraInitData.at = Vector3(0.0f, 0.0f, 0.0f);
	cameraInitData.up = Vector3(0.0f, 1.0f, 0.0f);
	cameraInitData.windowWidth = 640;
	cameraInitData.windowHeight = 480;
	cameraInitData.nearZ = 0.1f;
	cameraInitData.farZ = 100.0f;

	m_OrbitCamera = new OrbitCamera(cameraInitData, OrbitMode::Counter_Clockwise, cameraInitData.at, 5);
	m_CurrentCamera.View = m_OrbitCamera->GetView();
	m_CurrentCamera.Projection = m_OrbitCamera->GetProj();

	// Init gameobjects
	GOInitData hercules;
	hercules.constantBuffer = m_ContantBuffer;
	hercules.deviceContext = m_DeviceContext;
	hercules.position = Vector3(0.0f, 0.0f, 0.0f);
	hercules.rotation = Vector3();
	hercules.scale = Vector3::One();

	m_HerculesPlane = new GameObject(hercules);
	m_HerculesPlane->SetMesh("Assets/Models/Airplane/Hercules.obj", m_Device, false);

	m_Initialised = true;
}

void PlaneScene::Enter()
{
	OutputDebugString(L"Entered Plane Scene\n");
}

void PlaneScene::Update()
{
	if (!m_Initialised) return;

	m_HerculesPlane->Update();
}

void PlaneScene::Render(ConstantBuffer* cb)
{
	if (!m_Initialised) return;
	cb->mWorld = XMMatrixTranspose(XMLoadFloat4x4(&m_HerculesPlane->GetWorldMatrix()));;
	m_HerculesPlane->Draw(cb);
}

void PlaneScene::Exit()
{
}

CameraMats PlaneScene::GetActiveCamera()
{
	return CameraMats();
}
