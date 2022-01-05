#pragma once
#ifndef APPLICATION_H
#define APPLICATION_H

#include "Core.h"
#include "OBJLoader.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#include "FirstPersonCamera.h"
#include "OrbitCamera.h"
#include "GameObject.h"
#include "SkySphere.h"
#include "Terrain.h"

class Application
{
private:
	HINSTANCE                 _hInst;
	HWND                      _hWnd;
	D3D_DRIVER_TYPE			  _driverType;
	D3D_FEATURE_LEVEL		  _featureLevel;
	ID3D11Device*             _pd3dDevice;
	ID3D11DeviceContext*      m_ImmediateContext;
	IDXGISwapChain*           _pSwapChain;
	ID3D11RenderTargetView*   _pRenderTargetView;
	
	// Camera stuff
	Camera*                   m_FrontCam;
	Camera*                   m_TopDownCam;
	FirstPersonCamera*        m_FpsCam;
	OrbitCamera*              m_OrbitCam;
	CameraMats                m_CurrentCamera;

	// Game objects
	SkySphere*                m_SkySphere;
	GameObject*               m_HerculesPlane;
	Terrain*                  m_Terrain;

	// Hard coded mesh data
	ID3D11Buffer*             _pCubeVertexBuffer;
	ID3D11Buffer*             _pPyramidVertexBuffer;
	ID3D11Buffer*             _pPlaneVertexBuffer;
				              
	ID3D11Buffer*             _pCubeIndexBuffer;
	ID3D11Buffer*             _pPyramidIndexBuffer;
	ID3D11Buffer*             _pPlaneIndexBuffer;

	XMFLOAT4X4                _cubeWorld;
	XMFLOAT4X4                _pyramidWorld;
	XMFLOAT4X4                _planeWorld;


	ID3D11VertexShader*       _pVertexShader;
	ID3D11VertexShader*		  _pPlaneVertexShader;
	ID3D11PixelShader*        _pPixelShader;
	ID3D11PixelShader*        _pSkyPixelShader;
	ID3D11InputLayout*        _pVertexLayout;

	ID3D11Buffer*             m_ConstantBuffer;

	ID3D11DepthStencilView*   _depthStencilView;
	ID3D11Texture2D*          _depthStencilBuffer = nullptr;

	ID3D11RasterizerState*    _wireFrame;
	ID3D11RasterizerState*    _solidCullBack;
	ID3D11RasterizerState*    _solidCullFront;
	ID3D11BlendState*		  _transparency;

	ID3D11ShaderResourceView* m_HerculesTexRV = nullptr;
	ID3D11ShaderResourceView* m_CubeTexRV = nullptr;
	ID3D11SamplerState*       _pSamplerLinear = nullptr;

	// Lights
	Light::DirectionalLight directionalLight;
	Light::PointLight       pointLight;

private:
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	void Cleanup();
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	HRESULT InitShadersAndInputLayout();
	void InitLights();
	void InitCamera();
	void InitTextures();
	void InitModels();
	HRESULT InitVertexBuffer();
	HRESULT InitIndexBuffer();

	UINT _WindowHeight;
	UINT _WindowWidth;

	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float blendfactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	bool isWireFrame = false;
	bool isTransparent = false;

	bool debug = true;
	bool frontCamOpt = true, topDownCamOpt = true , orbitCamOpt = true, fpsCamOpt = true;

	bool showScene1 = true;  // Plane scene
	bool showScene2 = false; // Hard coded meshes
	bool showScene3 = false; // Terrain scene

	// Plane vertex shader controls
	float time = 0.0f, rate = 10.0f, amp = 0.6f, freq = 1.0f, threshold = -5.0f;

public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void Update();
	void Draw();
};
#endif // !APPLICATION_H
