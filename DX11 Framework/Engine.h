#pragma once
#ifndef ENGINE_H
#define ENGINE_H
#include"Core.h"



class Engine
{
public:
	static Engine* GetInstance();
	static Engine* GetInstance(HINSTANCE hInstance, int nCmdShow);
	static bool IsRunning() { return m_instance->m_isRunning; }
	HRESULT Init(HINSTANCE hInstance, int nCmdShow);

	void RegisterScenes();
	void Update();
	void Render();

	void Clean();
	void Quit();

	inline ID3D11Device*		GetDevice() { return m_pd3dDevice; }
	inline ID3D11DeviceContext* GetDeviceContext() { return m_pImmediateContext; }

private:  // Windows variables
	UINT                      m_WindowHeight;
	UINT                      m_WindowWidth;

	HINSTANCE                 m_hInst;
	HWND                      m_hWnd;
	D3D_DRIVER_TYPE           m_driverType;
	D3D_FEATURE_LEVEL         m_featureLevel;
	ID3D11Device*			  m_pd3dDevice;
	ID3D11DeviceContext*	  m_pImmediateContext;
	IDXGISwapChain*           m_pSwapChain;
	ID3D11RenderTargetView*   m_pRenderTargetView;
	ID3D11VertexShader*       m_pVertexShader;
	ID3D11PixelShader*        m_pPixelShader;
	ID3D11InputLayout*        m_pVertexLayout;
	ID3D11Buffer*			  m_pConstantBuffer;
	ID3D11RasterizerState*    m_wireFrame;
	ID3D11RasterizerState*    m_solid;

private:  // Setup methods
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	HRESULT InitDevice();
	//HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
	//HRESULT InitShadersAndInputLayout();
	//HRESULT InitVertexBuffer();
	//HRESULT InitIndexBuffer();

private:
	static Engine* m_instance;
	bool m_isRunning = false;
	bool m_isWireframe = false;

	Engine(HINSTANCE hInstance, int nCmdShow);
	~Engine();

};
#endif // !ENGINE_H