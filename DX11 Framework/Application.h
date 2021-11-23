#pragma once

#include "Core.h"
#include "OBJLoader.h"


class Application
{
private:
	HINSTANCE                 _hInst;
	HWND                      _hWnd;
	D3D_DRIVER_TYPE           _driverType;
	D3D_FEATURE_LEVEL         _featureLevel;
	ID3D11Device*			  _pd3dDevice;
	ID3D11DeviceContext*	  _pImmediateContext;
	IDXGISwapChain*			  _pSwapChain;
	ID3D11RenderTargetView*   _pRenderTargetView;
	Camera*                   m_camera;

	ID3D11VertexShader*		  _pVertexShader;
	ID3D11PixelShader*		  _pPixelShader;
	ID3D11InputLayout*		  _pVertexLayout;

	ID3D11Buffer*			  _pCubeVertexBuffer;
	ID3D11Buffer*			  _pPyramidVertexBuffer;
	ID3D11Buffer*			  _pPlaneVertexBuffer;
	ID3D11Buffer*			  _pMeshVertexBuffer;
	
	ID3D11Buffer*			  _pCubeIndexBuffer;
	ID3D11Buffer*			  _pPyramidIndexBuffer;
	ID3D11Buffer*			  _pPlaneIndexBuffer;
	ID3D11Buffer*			  _pMeshIndexBuffer;
	
	ID3D11Buffer*			  _pConstantBuffer;
	
	XMFLOAT4X4                _cubeWorld;
	XMFLOAT4X4                _pyramidWorld;
	XMFLOAT4X4                _planeWorld;
	XMFLOAT4X4                _meshWorld;
		
	ID3D11DepthStencilView*   _depthStencilView;
	ID3D11Texture2D*		  _depthStencilBuffer = nullptr;
	
	ID3D11RasterizerState*	  _wireFrame;
	ID3D11RasterizerState*	  _solid;
	
	ID3D11Texture2D*		  _myTexture;
	ID3D11ShaderResourceView* _pTextureRV = nullptr;
	ID3D11SamplerState*		  _pSamplerLinear = nullptr;
	
	MeshData				  objMeshData;

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