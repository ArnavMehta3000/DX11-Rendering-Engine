#include "Engine.h"
Engine* Engine::m_instance = nullptr;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}


Engine::Engine(HINSTANCE hInstance, int nCmdShow)
{
	Init(hInstance, nCmdShow);
}

Engine::~Engine()
{
	Clean();
}

void Engine::Clean()
{
	if (m_pImmediateContext)  m_pImmediateContext->ClearState();

	if (m_wireFrame)		  m_wireFrame->Release();
	if (m_solid)			  m_solid->Release();
	if (m_pConstantBuffer)	  m_pConstantBuffer->Release();	
	if (m_pRenderTargetView)  m_pRenderTargetView->Release();
	if (m_pSwapChain)		  m_pSwapChain->Release();
	if (m_pImmediateContext)  m_pImmediateContext->Release();
	if (m_pd3dDevice)		  m_pd3dDevice->Release();

}

void Engine::Quit()
{
	m_isRunning = false;
}

Engine* Engine::GetInstance()
{
	if (m_instance == nullptr)
		throw "Engine not initialized!";

    return m_instance;
}

Engine* Engine::GetInstance(HINSTANCE hInstance, int nCmdShow)
{
	if (m_instance == nullptr)
		m_instance = new Engine(hInstance, nCmdShow);

	return m_instance;
}

HRESULT Engine::Init(HINSTANCE hInstance, int nCmdShow)
{
	m_hInst = nullptr;
	m_hWnd = nullptr;
	m_driverType = D3D_DRIVER_TYPE_NULL;
	m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	m_pd3dDevice = nullptr;
	m_pImmediateContext = nullptr;
	m_pSwapChain = nullptr;
	m_pRenderTargetView = nullptr;
	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
	m_pVertexLayout = nullptr;

	if (FAILED(InitWindow(hInstance, nCmdShow))) return E_FAIL;

	RECT rc;
	GetClientRect(m_hWnd, &rc);
	m_WindowWidth = rc.right - rc.left;
	m_WindowHeight = rc.bottom - rc.top;

	if (FAILED(InitDevice())) { Clean(); return E_FAIL; }

	// TODO: Setup WVP matrices
	m_isRunning = true;

	return S_OK;
}

HRESULT Engine::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize        = sizeof(WNDCLASSEX);
	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = WndProc;
	wcex.cbClsExtra    = 0;
	wcex.cbWndExtra    = 0;
	wcex.hInstance     = hInstance;
	wcex.hIcon         = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
	wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName  = nullptr;
	wcex.lpszClassName = L"Game Engine Class";
	wcex.hIconSm       = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	m_hInst = hInstance;
	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindow(L"Game Engine Class", L"Game Engine", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if (!m_hWnd)
		return E_FAIL;

	ShowWindow(m_hWnd, nCmdShow);

	return S_OK;
}

HRESULT Engine::InitDevice()
{
	HRESULT hr = S_OK;

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};

	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	#pragma region Create Swap Chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = m_WindowWidth;
	sd.BufferDesc.Height = m_WindowHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(nullptr, m_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	#pragma endregion

	if (FAILED(hr))
		return hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	if (FAILED(hr))
		return hr;

	hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	pBackBuffer->Release();

	if (FAILED(hr))
		return hr;

	// TODO: Depth and stencil buffers here

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)m_WindowWidth;
	vp.Height = (FLOAT)m_WindowHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &vp);

	// TODO: Init shaders and input layout
	// TODO: Init vertex and index buffer

	// Set primitive topology
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// TODO: Constant buffer in scene or engine

	// Create the constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = m_pd3dDevice->CreateBuffer(&bd, nullptr, &m_pConstantBuffer);

	// Define rasterizer state - Wire Frame
	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));  // Clear memory
	wfdesc.FillMode = D3D11_FILL_WIREFRAME; //D3D10_FILL_SOLID for solid rendering
	wfdesc.CullMode = D3D11_CULL_NONE;  // FRONT: for front face culling (front face not rendered) | default is BACK
	wfdesc.DepthClipEnable = true;  // Enable clipping
	hr = m_pd3dDevice->CreateRasterizerState(&wfdesc, &m_wireFrame);

	// Define rasterizer state - Solid
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));  // Clear memory
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	hr = m_pd3dDevice->CreateRasterizerState(&solidDesc, &m_solid);


	if (FAILED(hr))
		return hr;

	return S_OK;
}



void Engine::RegisterScenes()
{
}

void Engine::Update()
{
}

void Engine::Render()
{
}