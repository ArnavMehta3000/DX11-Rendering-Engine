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

class Application
{
private:
	HINSTANCE                 _hInst;
	HWND                      _hWnd;
	D3D_DRIVER_TYPE			  _driverType;
	D3D_FEATURE_LEVEL		  _featureLevel;
	ID3D11Device*             _pd3dDevice;
	ID3D11DeviceContext*      _pImmediateContext;
	IDXGISwapChain*           _pSwapChain;
	ID3D11RenderTargetView*   _pRenderTargetView;
	// Camera stuff
	Camera*            m_StaticCam;
	FirstPersonCamera* m_FpsCam;
	OrbitCamera*       m_OrbitCamera;
	CameraMats         m_CurrentCamera;

	// Game objects
	GameObject* m_HerculesPlane;
	GameObject* m_GroundPlane;


	ID3D11VertexShader*       _pVertexShader;
	ID3D11PixelShader*        _pPixelShader;
	ID3D11InputLayout*        _pVertexLayout;

	ID3D11Buffer*             _pConstantBuffer;

	ID3D11DepthStencilView*   _depthStencilView;
	ID3D11Texture2D*          _depthStencilBuffer = nullptr;

	ID3D11RasterizerState*    _wireFrame;
	ID3D11RasterizerState*    _solid;
	ID3D11BlendState*		  _transparency;

	ID3D11Texture2D*          _myTexture;
	ID3D11ShaderResourceView* _pTextureRV = nullptr;
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

	bool isWireFrame = false;

public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void Update();
	void Draw();
};
#endif // !APPLICATION_H
