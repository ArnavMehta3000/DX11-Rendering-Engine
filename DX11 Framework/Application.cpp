#include "Application.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

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

Application::Application()
{
	_hInst = nullptr;
	_hWnd = nullptr;
	_driverType = D3D_DRIVER_TYPE_NULL;
	_featureLevel = D3D_FEATURE_LEVEL_11_0;
	_pd3dDevice = nullptr;
	m_ImmediateContext = nullptr;
	_pSwapChain = nullptr;
	_pRenderTargetView = nullptr;
	_pVertexShader = nullptr;
	_pPixelShader = nullptr;
	_pVertexLayout = nullptr;
	m_ConstantBuffer = nullptr;
}

Application::~Application()
{
	Cleanup();
}

void Application::Cleanup()
{
	if (m_ImmediateContext)  m_ImmediateContext->ClearState();

	if (_wireFrame)
		_wireFrame->Release();

	if (_solidCullBack)
		_solidCullBack->Release();

	if (_solidCullFront)
		_solidCullFront->Release();

	if (_transparency)
		_transparency->Release();

	if (m_ConstantBuffer)
		m_ConstantBuffer->Release();

	if (_depthStencilView)
		_depthStencilView->Release();

	if (_depthStencilBuffer)
		_depthStencilBuffer->Release();

	if (_pVertexLayout)
		_pVertexLayout->Release();

	if (_pPlaneVertexShader)
		_pPlaneVertexShader->Release();

	if (_pVertexShader)
		_pVertexShader->Release();

	if (_pPixelShader)
		_pPixelShader->Release();

	if (_pSkyPixelShader)
		_pSkyPixelShader->Release();
	
	if (_pTerrainPixelShader)
		_pTerrainPixelShader->Release();

	if (_pRenderTargetView)
		_pRenderTargetView->Release();

	if (_pSwapChain)
		_pSwapChain->Release();

	if (m_ImmediateContext)
		m_ImmediateContext->Release();

	if (_pd3dDevice)
		_pd3dDevice->Release();
}

#pragma region Base Init Chain
HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
	if (FAILED(InitWindow(hInstance, nCmdShow)))
	{
		return E_FAIL;
	}

	RECT rc;
	GetClientRect(_hWnd, &rc);
	_WindowWidth = rc.right - rc.left;
	_WindowHeight = rc.bottom - rc.top;

	if (FAILED(InitDevice()))
	{
		Cleanup();

		return E_FAIL;
	}

	// Initialize other things
	InitVertexBuffer();
	InitIndexBuffer();

	InitLights();
	InitTextures();
	InitCamera();
	InitModels();

	XMStoreFloat4x4(&_cubeWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&_pyramidWorld, XMMatrixIdentity());

	// Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(_hWnd);
	ImGui_ImplDX11_Init(_pd3dDevice, m_ImmediateContext);
	ImGui::StyleColorsDark();

	return S_OK;
}

HRESULT Application::InitShadersAndInputLayout()
{
	HRESULT hr;

	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	hr = CompileShaderFromFile(L"DX11 Framework.hlsl", "VS", "vs_4_0", &pVSBlob);

	if (FAILED(hr))
	{
		MessageBox(nullptr,
				   L"Default vertex shader failed to compile.", L"Vertex Shader Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = _pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_pVertexShader);

	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Create vertex shader for plane
	ID3DBlob* pPlaneVSBlob = nullptr;
	hr = CompileShaderFromFile(L"DX11 Framework.hlsl", "VS_PLANE", "vs_4_0", &pPlaneVSBlob);

	if (FAILED(hr))
	{
		MessageBox(nullptr,
				   L"Plane vertex shader failed to compile.", L"Vertex Shader Error", MB_OK);
		return hr;
	}

	hr = _pd3dDevice->CreateVertexShader(pPlaneVSBlob->GetBufferPointer(), pPlaneVSBlob->GetBufferSize(), nullptr, &_pPlaneVertexShader);

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile(L"DX11 Framework.hlsl", "PS", "ps_4_0", &pPSBlob);

	if (FAILED(hr))
	{
		MessageBox(nullptr,
				   L"Defaut pixel sahder failed to compile.", L"Pixel Shader Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_pPixelShader);
	pPSBlob->Release();

	if (FAILED(hr))
		return hr;

	// Compile the sky pixel shader
	ID3DBlob* pSkyPSBlob = nullptr;
	hr = CompileShaderFromFile(L"DX11 Framework.hlsl", "PS_SKY", "ps_4_0", &pSkyPSBlob);
	if (FAILED(hr))
	{
		pSkyPSBlob->Release();
		MessageBox(nullptr,
				   L"Sky Pixel Shader failed to compile.", L"Pixel Shader Error", MB_OK);
		return hr;
	}

	//Create the sky pixel shader
	hr = _pd3dDevice->CreatePixelShader(pSkyPSBlob->GetBufferPointer(), pSkyPSBlob->GetBufferSize(), nullptr, &_pSkyPixelShader);
	if (FAILED(hr))
		return hr;


	// COmpile the terrain pixel shader
	ID3DBlob* pTerrainBlob = nullptr;
	hr = CompileShaderFromFile(L"DX11 Framework.hlsl", "PS_TERRAIN", "ps_4_0", &pTerrainBlob);
	if (FAILED(hr))
	{
		pSkyPSBlob->Release();
		MessageBox(nullptr,
				   L"Terrain Pixel Shader failed to compile.", L"Pixel Shader Error", MB_OK);
		return hr;
	}

	//Create the terrain pixel shader
	hr = _pd3dDevice->CreatePixelShader(pTerrainBlob->GetBufferPointer(), pTerrainBlob->GetBufferSize(), nullptr, &_pTerrainPixelShader);
	if (FAILED(hr))
		return hr;


	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = _pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(),
										pVSBlob->GetBufferSize(), &_pVertexLayout);
	pVSBlob->Release();

	if (FAILED(hr))
		return hr;

	// Set the input layout
	m_ImmediateContext->IASetInputLayout(_pVertexLayout);

	return hr;
}

HRESULT Application::InitVertexBuffer()
{
	HRESULT hr;

	#pragma region Cube
	{
		SimpleVertex cubeVertices[] =
		{
			{ XMFLOAT3(-1.0f,-1.0f,-1.0f), XMFLOAT3(-0.5773f, 0.5773f, 0.5773f),   XMFLOAT2(1.0f, 0.0f) } ,
			{ XMFLOAT3(-1.0f,-1.0f, 1.0f), XMFLOAT3(-0.5773f, 0.5773f, -0.5773f),  XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(-1.0f, 1.0f,-1.0f), XMFLOAT3(-0.5773f, -0.5773f, 0.5773f),  XMFLOAT2(1.0f, 1.0f) },
			{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-0.5773f, -0.5773f, -0.5773f), XMFLOAT2(0.0f, 1.0f) },
			{ XMFLOAT3(1.0f,-1.0f,-1.0f),  XMFLOAT3(0.5773f, -0.5773f, -0.5773f),  XMFLOAT2(0.0f, 0.0f) },
			{ XMFLOAT3(1.0f,-1.0f, 1.0f),  XMFLOAT3(0.5773f, 0.5773f, 0.5773f),    XMFLOAT2(1.0f, 0.0f) },
			{ XMFLOAT3(1.0f, 1.0f,-1.0f),  XMFLOAT3(0.5773f, 0.5773f, -0.5773f),   XMFLOAT2(0.0f, 1.0f) },
			{ XMFLOAT3(1.0f, 1.0f, 1.0f),  XMFLOAT3(0.5773f, -0.5773f, 0.5773f),   XMFLOAT2(1.0f, 1.0f) }
		};

		D3D11_BUFFER_DESC cubeBd;
		ZeroMemory(&cubeBd, sizeof(cubeBd));
		cubeBd.Usage = D3D11_USAGE_DEFAULT;
		cubeBd.ByteWidth = sizeof(SimpleVertex) * 8;
		cubeBd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		cubeBd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA CubeInitData;
		ZeroMemory(&CubeInitData, sizeof(CubeInitData));
		CubeInitData.pSysMem = cubeVertices;

		hr = _pd3dDevice->CreateBuffer(&cubeBd, &CubeInitData, &_pCubeVertexBuffer);
	}
	#pragma endregion

	#pragma region Pyramid
	{
		SimpleVertex pyramidVertices[] =
		{
			// Base
			{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT3(-0.6626f * -1, -0.3490f * -1, -0.6626f * -1) },
			{ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT3(0.6626f * -1, -0.3490f * -1, 0.6626f * -1) },
			{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-0.6626f * -1, -0.3490f * -1, 0.6626f * -1) },
			{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0.6626f * -1, -0.3490f * -1, -0.6626f * -1) },
			// Tip
			{ XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f) },
		};

		D3D11_BUFFER_DESC pyramidBd;
		ZeroMemory(&pyramidBd, sizeof(pyramidBd));
		pyramidBd.Usage = D3D11_USAGE_DEFAULT;
		pyramidBd.ByteWidth = sizeof(SimpleVertex) * 5;
		pyramidBd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		pyramidBd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA PyramidInitData;
		ZeroMemory(&PyramidInitData, sizeof(PyramidInitData));
		PyramidInitData.pSysMem = pyramidVertices;

		hr = _pd3dDevice->CreateBuffer(&pyramidBd, &PyramidInitData, &_pPyramidVertexBuffer);
	}
	#pragma endregion

	#pragma region Plane
	{
		// Define plane size
		const unsigned int xSize = 50, zSize = 50;  // HACK: Change the plane size in the index buffer
		float offsetX = xSize / 2.0f, offsetZ = zSize / 2.0f;
		SimpleVertex planeVertices[(xSize + 1) * (zSize + 1)];

		// Populate the plane vertex buffer
		for (int i = 0, z = 0; z <= zSize; z++)
		{
			for (int x = 0; x <= xSize; x++)
			{
				planeVertices[i] = { XMFLOAT3(x - offsetX, 0.0f, z - offsetZ), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(x - offsetX / xSize, z - offsetZ / zSize)};
				i++;
			}
		}

		// Set buffer description
		D3D11_BUFFER_DESC planeBd;
		ZeroMemory(&planeBd, sizeof(planeBd));
		planeBd.Usage = D3D11_USAGE_DEFAULT;
		planeBd.ByteWidth = sizeof(SimpleVertex) * (xSize + 1) * (zSize + 1);
		planeBd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		planeBd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA PlaneInitData;
		ZeroMemory(&PlaneInitData, sizeof(PlaneInitData));
		PlaneInitData.pSysMem = planeVertices;

		hr = _pd3dDevice->CreateBuffer(&planeBd, &PlaneInitData, &_pPlaneVertexBuffer);
	}
	#pragma endregion

	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT Application::InitIndexBuffer()
{
	HRESULT hr;

	#pragma region Cube
	{
		// Index buffer for cube
		WORD cubeIndices[] =
		{
			// Face 1
			0,1,2, // -x
			1,3,2,

			// Face 2
			4,6,5, // +x
			5,6,7,

			// Face 3
			0,5,1, // -y
			0,4,5,

			// Face 4
			2,7,6, // +y
			2,3,7,

			// Face 5
			0,6,4, // -z
			0,2,6,

			// Face 6
			1,7,3, // +z
			1,5,7,
		};

		// Buffer description for cube
		D3D11_BUFFER_DESC cubeBd;
		ZeroMemory(&cubeBd, sizeof(cubeBd));

		cubeBd.Usage = D3D11_USAGE_DEFAULT;
		cubeBd.ByteWidth = sizeof(WORD) * 3 * 2 * 6;  // 3 verts per triangle | 2 triangles per face | 6 faces
		cubeBd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		cubeBd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA CubeInitData;
		ZeroMemory(&CubeInitData, sizeof(CubeInitData));
		CubeInitData.pSysMem = cubeIndices;
		hr = _pd3dDevice->CreateBuffer(&cubeBd, &CubeInitData, &_pCubeIndexBuffer);
	}
	#pragma endregion

	#pragma region Pyramid
	{
		WORD pyramidIndices[] =
		{
			0, 2, 1,
			1, 2, 3,
			0, 1, 4,
			1, 3, 4,
			3, 2, 4,
			2, 0, 4
		};

		D3D11_BUFFER_DESC pyramidBd;
		ZeroMemory(&pyramidBd, sizeof(pyramidBd));

		pyramidBd.Usage = D3D11_USAGE_DEFAULT;
		pyramidBd.ByteWidth = sizeof(WORD) * 3 * 6;
		pyramidBd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		pyramidBd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA PyramidInitData;
		ZeroMemory(&PyramidInitData, sizeof(PyramidInitData));
		PyramidInitData.pSysMem = pyramidIndices;
		hr = _pd3dDevice->CreateBuffer(&pyramidBd, &PyramidInitData, &_pPyramidIndexBuffer);
	}
	#pragma endregion

	#pragma region Plane
	{
		const unsigned int xSize = 50, zSize = 50;  // HACK: Change the plane size in the vertex buffer
		WORD planeIndices[xSize * zSize * 6];  // Plane size * 3 verts * 2 tris per quad

		int vert = 0, tris = 0;
		for (int z = 0; z < zSize; z++)
		{
			for (int x = 0; x < xSize; x++)
			{
				planeIndices[tris + 0] = vert + 0;
				planeIndices[tris + 1] = vert + xSize + 1;
				planeIndices[tris + 2] = vert + 1;
				planeIndices[tris + 3] = vert + 1;
				planeIndices[tris + 4] = vert + xSize + 1;
				planeIndices[tris + 5] = vert + xSize + 2;

				vert++;
				tris += 6;
			}
			vert++;
		}

		//  Create buffer description
		D3D11_BUFFER_DESC planeBd;
		ZeroMemory(&planeBd, sizeof(planeBd));

		planeBd.Usage = D3D11_USAGE_DEFAULT;
		planeBd.ByteWidth = sizeof(WORD) * xSize * zSize * 6;
		planeBd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		planeBd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA PlaneInitData;
		ZeroMemory(&PlaneInitData, sizeof(PlaneInitData));
		PlaneInitData.pSysMem = planeIndices;
		hr = _pd3dDevice->CreateBuffer(&planeBd, &PlaneInitData, &_pPlaneIndexBuffer);
	}
	XMStoreFloat4x4(&_planeWorld, XMMatrixTranslation(0.0f, -1.5, 0.0));
	#pragma endregion

	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT Application::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_TUTORIAL1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"TutorialWindowClass";
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_TUTORIAL1);
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	_hInst = hInstance;
	RECT rc = { 0, 0, 1280, 960 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	_hWnd = CreateWindow(L"TutorialWindowClass", L"DX11 Framework", WS_OVERLAPPEDWINDOW,
						 CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
						 nullptr);
	if (!_hWnd)
		return E_FAIL;

	ShowWindow(_hWnd, nCmdShow);

	return S_OK;
}

HRESULT Application::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if defined(DEBUG) || defined(_DEBUG)
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows
		// the shaders to be optimized and to run exactly the way they will run in
		// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	#endif

	ID3DBlob* pErrorBlob;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
							dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

	if (FAILED(hr))
	{
		if (pErrorBlob != nullptr)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

		if (pErrorBlob) pErrorBlob->Release();

		return hr;
	}

	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

HRESULT Application::InitDevice()
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

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = _WindowWidth;
	sd.BufferDesc.Height = _WindowHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = _hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(nullptr, _driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
										   D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pd3dDevice, &_featureLevel, &m_ImmediateContext);
		if (SUCCEEDED(hr))
			break;
	}

	if (FAILED(hr))
		return hr;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	if (FAILED(hr))
		return hr;

	hr = _pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &_pRenderTargetView);
	pBackBuffer->Release();

	if (FAILED(hr))
		return hr;

	// Define depth/stencil buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = _WindowWidth;
	depthStencilDesc.Height = _WindowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	//// Create depth.stencil buffer
	hr = _pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &_depthStencilBuffer);
	if (_depthStencilBuffer != 0)
		hr = _pd3dDevice->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthStencilView);

	// Add it to the output merger
	m_ImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _depthStencilView);
	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)_WindowWidth;
	vp.Height = (FLOAT)_WindowHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_ImmediateContext->RSSetViewports(1, &vp);

	hr = InitShadersAndInputLayout();
	if (FAILED(hr))
	{
		return hr;
	}

	// Set primitive topology
	m_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = _pd3dDevice->CreateBuffer(&bd, nullptr, &m_ConstantBuffer);

	// Define rasterizer state - Wire Frame
	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));  // Clear memory
	wfdesc.FillMode = D3D11_FILL_WIREFRAME; //D3D10_FILL_SOLID for solid rendering
	wfdesc.CullMode = D3D11_CULL_NONE;  // FRONT: for front face culling (front face not rendered) | default is BACK
	wfdesc.DepthClipEnable = true;  // Enable clipping
	hr = _pd3dDevice->CreateRasterizerState(&wfdesc, &_wireFrame);

	// Define rasterizer state - Solid Cull Back
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));  // Clear memory
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.DepthClipEnable = true;
	hr = _pd3dDevice->CreateRasterizerState(&solidDesc, &_solidCullBack);

	solidDesc.CullMode = D3D11_CULL_FRONT;
	hr = _pd3dDevice->CreateRasterizerState(&solidDesc, &_solidCullFront);

	// Define blend state
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend = D3D11_BLEND_BLEND_FACTOR;
	rtbd.BlendOp = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;

	_pd3dDevice->CreateBlendState(&blendDesc, &_transparency);

	if (FAILED(hr))
		return hr;

	return S_OK;
}
#pragma endregion


#pragma region My Init Chain
void Application::InitCamera()
{
	// Create camera init data
	CameraInitData cameraInitData;
	cameraInitData.position = Vector3(0.0f, 2.0f, -17.0f);
	cameraInitData.at = Vector3(0.0f, 0.0f, 0.0f);
	cameraInitData.up = Vector3(0.0f, 1.0f, 0.0f);
	cameraInitData.windowWidth = _WindowWidth;
	cameraInitData.windowHeight = _WindowHeight;
	cameraInitData.nearZ = 0.01f;
	cameraInitData.farZ = 5000.0f;

	m_FrontCam = new Camera(cameraInitData);

	cameraInitData.position = Vector3(0.0f, 17.0f, 1.0f);
	m_TopDownCam = new Camera(cameraInitData);

	m_FpsCam = new FirstPersonCamera(cameraInitData, Vector3(0.0f, 0.0f, 1.0f));

	cameraInitData.position = Vector3(0.0f, 20.0f, 1.0f);
	m_OrbitCam = new OrbitCamera(cameraInitData, OrbitMode::Counter_Clockwise, cameraInitData.at, 5);

	// Default camera is static camera
	m_CurrentCamera.View = m_FrontCam->GetView();
	m_CurrentCamera.Projection = m_FrontCam->GetProj();
}

void Application::InitLights()
{
	// Directional light
	directionalLight.Intensity.Ambient  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLight.Intensity.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLight.Intensity.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLight.Material.ambient   = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	directionalLight.Material.diffuse   = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLight.Material.specular  = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	directionalLight.Direction          = XMFLOAT3(1.0f, 1.0f, 1.0f);
	directionalLight.SpecularPower      = 0.8f;

	//Point lights
	pointLight.Intensity.Ambient        = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pointLight.Intensity.Diffuse        = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pointLight.Intensity.Specular       = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pointLight.Material.ambient         = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	pointLight.Material.diffuse         = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pointLight.Material.specular        = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pointLight.Position                 = XMFLOAT3(0.0f, 3.0f, 0.0f);
	pointLight.Attenuation              = XMFLOAT3(5.0f, 5.0f, 5.0f);
	pointLight.Range                    = 10.0f;
	pointLight.SpecularPower            = 0.8f;
}

void Application::InitTextures()
{
	// Load texture
	HRESULT hr;
	hr = CreateDDSTextureFromFile(_pd3dDevice, L"Assets/Crate_COLOR.dds", nullptr, &m_CubeTexRV);

	if (FAILED(hr))
	{
		return;
	}

	// Define and create sampler state
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	_pd3dDevice->CreateSamplerState(&samplerDesc, &_pSamplerLinear);
}

void Application::InitModels()
{
	// Init plane
	m_HerculesPlane = new GameObject("HerculesInitData.json", m_ImmediateContext, m_ConstantBuffer);
	m_HerculesPlane->SetMesh("Assets/Models/Airplane/Hercules.obj", _pd3dDevice, false);
	m_HerculesPlane->LoadTexture(_pd3dDevice, L"Assets/Models/Airplane/Hercules_COLOR.dds");

	// Init skybox
	m_SkySphere = new SkySphere("SkySphereInitData.json", m_ImmediateContext, m_ConstantBuffer, _pd3dDevice);

	HMapInfo hm(513, 513, 100.0f);
	m_Terrain = new Terrain("Assets/Terrain/HM 513.raw", hm, _pd3dDevice);
}
#pragma endregion


void Application::Update()
{
	#pragma region Timer
	static float t = 0.0f;
	if (_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();

		if (dwTimeStart == 0)
			dwTimeStart = dwTimeCur;

		t = (dwTimeCur - dwTimeStart) / 1000.0f;
	}
	#pragma endregion

	float speed = 0.0001f;
	if (time < 360)
		time += speed;
	else
		time = 0.0f;

	// Update gameobject
	m_SkySphere->Update();
	m_HerculesPlane->Update();

	// Update hard coded mesh scene
	if (showScene2)
	{
		XMStoreFloat4x4(&_cubeWorld, XMMatrixTranslation(sin(t * 2) * 6, 0.0f, 0.0f));
		XMStoreFloat4x4(&_pyramidWorld, XMMatrixTranslation(0.0f, 0.0f, cos(t * 2) * 6));
	}


	#pragma region Update Cameras
	m_FpsCam->Update();
	m_FrontCam->Update();
	m_TopDownCam->Update();
	m_OrbitCam->Update();

	if (m_FpsCam->enabled)
	{
		m_CurrentCamera.View = m_FpsCam->GetView();
		m_CurrentCamera.Projection = m_FpsCam->GetProj();
	}

	if (m_FrontCam->enabled)
	{
		m_CurrentCamera.View = m_FrontCam->GetView();
		m_CurrentCamera.Projection = m_FrontCam->GetProj();
	}

	if (m_TopDownCam->enabled)
	{
		m_CurrentCamera.View = m_TopDownCam->GetView();
		m_CurrentCamera.Projection = m_TopDownCam->GetProj();
	}

	if (m_OrbitCam->enabled)
	{
		m_CurrentCamera.View = m_OrbitCam->GetView();
		m_CurrentCamera.Projection = m_OrbitCam->GetProj();
	}
	#pragma endregion
}

void Application::Draw()
{
	#pragma region Render Setup
	// Clear the back buffer
	m_ImmediateContext->ClearRenderTargetView(_pRenderTargetView, ClearColor);

	// Clear depth stencil
	m_ImmediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	XMMATRIX view = XMLoadFloat4x4(&m_CurrentCamera.View);
	XMMATRIX transposeView = XMMatrixTranspose(view);
	XMMATRIX projection = XMLoadFloat4x4(&m_CurrentCamera.Projection);
	XMFLOAT4X4 eye;

	XMMATRIX meshWorld;

	XMStoreFloat4x4(&eye, view);

	// Update variables
	ConstantBuffer cb;
	cb.mView = transposeView;
	cb.mProjection = XMMatrixTranspose(projection);

	// Plane vb
	cb.Time = time;
	cb.Rate = rate;
	cb.Amplitude = amp;
	cb.Frequency = freq;
	cb.Threshold = threshold;

	// Terrain ps
	cb.WaterLevel = waterLevel;
	cb.GrassLevel = grassLevel;
	cb.StoneLevel = stoneLevel;
	cb.SnowLevel  = snowLevel;

	// Lights
	cb.dirLight = directionalLight;
	cb.pointLight = pointLight;

	cb.LightVecW = directionalLight.Direction;
	cb.pointLight = pointLight;
	cb.EyePosW = XMFLOAT3(eye._41, eye._42, eye._43);
	// Reference https://stackoverflow.com/questions/39280104/how-to-get-current-camera-position-from-view-matrix

	m_ImmediateContext->UpdateSubresource(m_ConstantBuffer, 0, nullptr, &cb, 0, 0);

	// Renders a triangle
	m_ImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
	m_ImmediateContext->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	m_ImmediateContext->PSSetConstantBuffers(0, 1, &m_ConstantBuffer);
	m_ImmediateContext->PSSetSamplers(0, 1, &_pSamplerLinear);
	#pragma endregion

	#pragma region Draw GameObjects
	// Global transparency
	if (isTransparent)
		m_ImmediateContext->OMSetBlendState(_transparency, blendfactor, 0xffffffff);
	else
		m_ImmediateContext->OMSetBlendState(0, 0, 0xffffffff);

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;

	// Draw  skybox only when
	if (!isWireFrame && !isTransparent)
	{
		m_ImmediateContext->RSSetState(_solidCullFront);
		m_ImmediateContext->PSSetShader(_pSkyPixelShader, nullptr, 0);

		cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&m_SkySphere->GetWorldMatrix()));
		m_SkySphere->DrawTextured(&cb, 0);

		m_ImmediateContext->PSSetShader(_pPixelShader, nullptr, 0);
		m_ImmediateContext->RSSetState(_solidCullBack);
	}

	if (showScene1)  // Plane scene
	{
		cb.mWorld = XMMatrixTranspose(XMLoadFloat4x4(&m_HerculesPlane->GetWorldMatrix()));
		m_HerculesPlane->DrawTextured(&cb, 1);
	}

	if (showScene2)  // Hard coded meshes scene
	{
		m_ImmediateContext->PSSetShaderResources(1, 1, &m_CubeTexRV);

		// Pyramid
		m_ImmediateContext->IASetVertexBuffers(0, 1, &_pPyramidVertexBuffer, &stride, &offset);
		m_ImmediateContext->IASetIndexBuffer(_pPyramidIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		meshWorld = XMLoadFloat4x4(&_pyramidWorld);
		cb.mWorld = XMMatrixTranspose(meshWorld);
		m_ImmediateContext->UpdateSubresource(m_ConstantBuffer, 0, nullptr, &cb, 0, 0);
		m_ImmediateContext->DrawIndexed(3 * 6, 0, 0);


		// Cube
		m_ImmediateContext->IASetVertexBuffers(0, 1, &_pCubeVertexBuffer, &stride, &offset);
		m_ImmediateContext->IASetIndexBuffer(_pCubeIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		meshWorld = XMLoadFloat4x4(&_cubeWorld);
		cb.mWorld = XMMatrixTranspose(meshWorld);
		m_ImmediateContext->UpdateSubresource(m_ConstantBuffer, 0, nullptr, &cb, 0, 0);
		m_ImmediateContext->DrawIndexed((3 * 2 * 6), 0, 0);


		// Plane
		m_ImmediateContext->VSSetShader(_pPlaneVertexShader, nullptr, 0);  // Set shader

		int xSize = 50, zSize = 50;
		m_ImmediateContext->IASetVertexBuffers(0, 1, &_pPlaneVertexBuffer, &stride, &offset);
		m_ImmediateContext->IASetIndexBuffer(_pPlaneIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		meshWorld = XMLoadFloat4x4(&_planeWorld);
		cb.mWorld = XMMatrixTranspose(meshWorld);
		m_ImmediateContext->UpdateSubresource(m_ConstantBuffer, 0, nullptr, &cb, 0, 0);
		m_ImmediateContext->DrawIndexed(xSize * zSize * 6, 0, 0);

		m_ImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);  // Reset Shader
	}

	if (showScene3)  // Terrain scene
	{
		ID3D11Buffer* vb = m_Terrain->GetBuffers().VertexBuffer;
		ID3D11Buffer* ib = m_Terrain->GetBuffers().IndexBuffer;

		m_ImmediateContext->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
		m_ImmediateContext->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);

		m_ImmediateContext->PSSetShader(_pTerrainPixelShader, nullptr, 0);
		meshWorld = XMLoadFloat4x4(&m_Terrain->GetWorldMat());
		cb.mWorld = XMMatrixTranspose(meshWorld);

		m_ImmediateContext->UpdateSubresource(m_ConstantBuffer, 0, nullptr, &cb, 0, 0);
		m_ImmediateContext->DrawIndexed(m_Terrain->GetBuffers().IndexCount, 0, 0);

		m_ImmediateContext->PSSetShader(_pPixelShader, nullptr, 0);
	}
	#pragma endregion

	#pragma region ImGui Stuff
	// ImGui display
	if (debug)
	{
		using namespace ImGui;
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		NewFrame();
		{
			Begin("Debug Menu", 0, ImGuiWindowFlags_AlwaysAutoResize);

			BeginTabBar("Settings");

			#pragma region World Settings
			if (BeginTabItem("World"))
			{
				ColorEdit3("Background Color", ClearColor);
				if (Checkbox("Wireframe", &isWireFrame))
				{
					if (isWireFrame)  // Is solid, set wire frame
						m_ImmediateContext->RSSetState(_wireFrame);
					if (!isWireFrame)  // Is wire frame, set solid
						m_ImmediateContext->RSSetState(_solidCullBack);
				}

				NewLine();

				if (CollapsingHeader("Transparency", ImGuiTreeNodeFlags_DefaultOpen))
				{
					Checkbox("Is Transparent?", &isTransparent);
					if (isTransparent)
						ColorEdit4("Transparency Color", blendfactor);
				}

				EndTabItem();
			}
			#pragma endregion

			#pragma region Lights
			if (BeginTabItem("Lights"))
			{
				#pragma region Directional Light
				if (TreeNode("Directional Light"))
				{
					SliderFloat3("Direction", &directionalLight.Direction.x, -1.0f, 1.0f);

					ColorEdit3("Diffuse##I", &directionalLight.Intensity.Diffuse.x);
					ColorEdit3("Specular##I", &directionalLight.Intensity.Specular.x);

					SliderFloat("Specular Power", &directionalLight.SpecularPower, 0.001f, 2.0f);

					TreePop();
				}
				#pragma endregion

				NewLine();

				#pragma region Point Light
				if (TreeNode("Point Light"))
				{
					DragFloat3("Position##point", &pointLight.Position.x, 0.5f, -1000.0f, 1000.0f);
					ColorEdit3("Color##IP", &pointLight.Intensity.Specular.x);
					SliderFloat("Diffusion", &pointLight.SpecularPower, 0.001f, 2.0f);
					SliderFloat("Range", &pointLight.Range, 0.001f, 1000.0f);

					TreePop();
				}
				#pragma endregion

				EndTabItem();
			}
			#pragma endregion

			#pragma region Cameras
			if (BeginTabItem("Cameras"))
			{
				Text("Set Camera");

				if (Button("Front View Camera") || m_FrontCam->enabled)
				{
					m_FrontCam->enabled = true;
					m_OrbitCam->enabled = false;
					m_FpsCam->enabled = false;
					m_TopDownCam->enabled = false;

					SameLine(); Checkbox("Show Options##Static", &frontCamOpt);
					if (frontCamOpt)
					{
						Vector3 pos = m_FrontCam->GetPosition();
						Vector3 look = m_FrontCam->GetLookAt();

						DragFloat3("Position##Static", &pos.x); SameLine();
						if (Button("Reset##SPos"))
							m_FrontCam->SetPosition(Vector3(0.0f, 2.0f, -17.0f));
						else
							m_FrontCam->SetPosition(pos);

						DragFloat3("Look At##Static", &look.x); SameLine();
						if (Button("Reset##SLook"))
							m_FrontCam->SetLookAt(Vector3::Zero());
						else
							m_FrontCam->SetLookAt(look);
					}
				}

				if (Button("Top View Camera") || m_TopDownCam->enabled)
				{
					m_FrontCam->enabled = false;
					m_OrbitCam->enabled = false;
					m_FpsCam->enabled = false;
					m_TopDownCam->enabled = true;

					SameLine(); Checkbox("Show Options##Static", &topDownCamOpt);
					if (topDownCamOpt)
					{
						Vector3 pos = m_TopDownCam->GetPosition();
						Vector3 look = m_TopDownCam->GetLookAt();

						DragFloat3("Position##Static", &pos.x); SameLine();
						if (Button("Reset##SPos"))
							m_TopDownCam->SetPosition(Vector3(0.0f, 17.0f, 1.0f));
						else
							m_TopDownCam->SetPosition(pos);

						DragFloat3("Look At##Static", &look.x); SameLine();
						if (Button("Reset##SLook"))
							m_TopDownCam->SetLookAt(Vector3::Zero());
						else
							m_TopDownCam->SetLookAt(look);
					}
				}

				if (Button("Orbit Camera") || m_OrbitCam->enabled)
				{
					m_FrontCam->enabled = false;
					m_OrbitCam->enabled = true;
					m_FpsCam->enabled = false;
					m_TopDownCam->enabled = false;

					SameLine(); Checkbox("Show Options##Orbit", &orbitCamOpt);
					if (orbitCamOpt)
					{
						Text("Q/E to change camera height");

						float radius = m_OrbitCam->GetRadius();
						float speed = m_OrbitCam->GetSpeed();
						float mSpeed = m_OrbitCam->GetMoveSpeed();

						if (DragFloat("Move Speed", &mSpeed, 0.001f, 0.005f, 1.0f))
							m_OrbitCam->SetMoveSpeed(mSpeed);

						if (DragFloat("Radius##Orbit", &radius, 0.5f, 1.0f, 250.0f))
							m_OrbitCam->SetRadius(radius);

						if (SliderFloat("Speed##Orbit", &speed, -0.1f, 0.1f))
							m_OrbitCam->SetSpeed(speed);
					}
				}

				if (Button("FPS Camera") || m_FpsCam->enabled)
				{
					m_FrontCam->enabled = false;
					m_OrbitCam->enabled = false;
					m_FpsCam->enabled = true;
					m_TopDownCam->enabled = false;

					SameLine();
					Checkbox("Show Options##FPS", &fpsCamOpt);
					if (fpsCamOpt)
						Text("W/S to move\nArrow keys to rotate\nPg Up/Down to move camera up or down\nTAB to flip camera");
				}

				EndTabItem();
			}
			#pragma endregion

			#pragma region Scenes
			if (BeginTabItem("Scenes"))
			{
				if (Button("Scene 1: Mesh Loading"))
				{
					showScene1 = true;
					showScene2 = false;
					showScene3 = false;
				}

				if (Button("Scene 2: Hard Coded Meshes"))
				{
					showScene1 = false;
					showScene2 = true;
					showScene3 = false;
				}

				if (showScene2)
				{
					NewLine();

					DragFloat("Rate", &rate, 0.1f, 0.0f, 20.0f);
					DragFloat("Amplitude", &amp, 0.1f, 0.0f, 20.0f);
					DragFloat("Frequency", &freq, 0.1f, 0.0f, 50.0f);
					DragFloat("Threshold", &threshold, 0.1f, -10.0f, 10.0f);
				}

				if (Button("Scene 3: Terrain"))
				{
					showScene1 = false;
					showScene2 = false;
					showScene3 = true;
				}

				if (showScene3)
				{
					NewLine();

					DragFloat("Water Level", &waterLevel, 1.0f, 0.0f,		terrainHeight, "%.3f", ImGuiSliderFlags_NoInput);
					DragFloat("Grass Level", &grassLevel, 1.0f, waterLevel, terrainHeight, "%.3f", ImGuiSliderFlags_NoInput);
					DragFloat("Stone Level", &stoneLevel, 1.0f, grassLevel, terrainHeight, "%.3f", ImGuiSliderFlags_NoInput);
					DragFloat("Snow Level",  &snowLevel,  1.0f, stoneLevel, terrainHeight, "%.3f", ImGuiSliderFlags_NoInput);

					NewLine();
					DragFloat("Height", &terrainHeight, 1.0f, 0.0f, 500.0f);
					
					if (Button("Generate"))
						m_Terrain->UpdateTerrain(HMapInfo(513, 513, terrainHeight));
				}
				EndTabItem();
				#pragma endregion
			}

			EndTabBar();
		}

		// Create test window
		End();
		Render();
		ImGui_ImplDX11_RenderDrawData(GetDrawData());
	}
	#pragma endregion

	// Present our back buffer to our front buffer
	_pSwapChain->Present(0, 0);
}
