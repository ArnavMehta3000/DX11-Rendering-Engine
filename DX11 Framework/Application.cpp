#include "Application.h"

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

Application::Application()
{
	_hInst = nullptr;
	_hWnd = nullptr;
	_driverType = D3D_DRIVER_TYPE_NULL;
	_featureLevel = D3D_FEATURE_LEVEL_11_0;
	_pd3dDevice = nullptr;
	_pImmediateContext = nullptr;
	_pSwapChain = nullptr;
	_pRenderTargetView = nullptr;
	_pVertexShader = nullptr;
	_pPixelShader = nullptr;
	_pVertexLayout = nullptr;
	_pCubeVertexBuffer = nullptr;
	_pCubeIndexBuffer = nullptr;
	_pConstantBuffer = nullptr;
}

Application::~Application()
{
	Cleanup();
}

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


	InitCamera();
	InitModels();


	XMStoreFloat4x4(&_cubeWorld, XMMatrixIdentity());
	XMStoreFloat4x4(&_pyramidWorld, XMMatrixIdentity());

	return S_OK;
}

void Application::InitCamera()
{
	// Create camera init data
	CameraInitData cameraInitData;
	cameraInitData.position = Vector3(0.0f, 0.0f, -3.0f);
	cameraInitData.at = Vector3(0.0f, 0.0f, 0.0f);
	cameraInitData.up = Vector3(0.0f, 1.0f, 0.0f);
	cameraInitData.windowWidth = _WindowWidth;
	cameraInitData.windowHeight = _WindowHeight;
	cameraInitData.nearZ = 0.1f;
	cameraInitData.farZ = 100.0f;


	staticCam = new Camera(cameraInitData);
	fpsCam = new FirstPersonCamera(cameraInitData);
	orbitCam = new OrbitCamera(cameraInitData, OrbitMode::Counter_Clockwise, cameraInitData.at, 5);

	// Default camerais static camera
	currentCamera.View = staticCam->GetView();
	currentCamera.Projection = staticCam->GetProj();
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
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = _pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_pVertexShader);

	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile(L"DX11 Framework.hlsl", "PS", "ps_4_0", &pPSBlob);

	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = _pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_pPixelShader);
	pPSBlob->Release();

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
	_pImmediateContext->IASetInputLayout(_pVertexLayout);

	return hr;
}

void Application::InitLights()
{
	// Directional light
	directionalLight.Intensity.Ambient           = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	directionalLight.Intensity.Diffuse           = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	directionalLight.Intensity.Specular          = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	directionalLight.Material.ambient            = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	directionalLight.Material.diffuse            = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	directionalLight.Material.specular           = XMFLOAT4(0.2f, 0.2f, 1.0f, 1.0f);
	directionalLight.Direction                   = XMFLOAT3(1.0f, 1.0f, 1.0f);
	directionalLight.SpecularPower               = 0.8f;

	// Directional Light colors
	// Ambient: White
	// Specular: Green
	// Diffuse: Blue


	//Point lights (WIP)
	pointLight.Intensity.Ambient  = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	pointLight.Intensity.Diffuse  = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pointLight.Intensity.Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	pointLight.Material.ambient   = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	pointLight.Material.diffuse   = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	pointLight.Material.specular  = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	pointLight.Position           = XMFLOAT3(0.0f, 1.0f, 0.0f);
	pointLight.Attenuation        = XMFLOAT3(5.0f, 5.0f, 5.0f);
	pointLight.Range              = 10.0f;
	pointLight.SpecularPower	  = 10.2f;
}

void Application::InitTextures()
{
	// Load texture
	HRESULT hr = CreateDDSTextureFromFile(_pd3dDevice, L"Assets/Crate_COLOR.dds", nullptr, &_pTextureRV);

	if (FAILED(hr))
	{
		return;
	}

	// Tell shader to use the texture
	_pImmediateContext->PSSetShaderResources(0, 1, &_pTextureRV);

	// Define and create sampler state
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.Filter         = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU       = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV       = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW       = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD         = 0;
	samplerDesc.MaxLOD         = D3D11_FLOAT32_MAX;

	_pd3dDevice->CreateSamplerState(&samplerDesc, &_pSamplerLinear);

	//// Set sampler state
	//_pImmediateContext->PSSetSamplers(0, 1, &_pSamplerLinear);
}

void Application::InitModels()
{
	objMeshData = OBJLoader::Load("Assets/Models/Blender/donut.obj", _pd3dDevice, false);
	_pMeshIndexBuffer = objMeshData.IndexBuffer;
	_pMeshVertexBuffer = objMeshData.VertexBuffer;
}

HRESULT Application::InitVertexBuffer()
{
	HRESULT hr;

#pragma region Cube
	{
		SimpleVertex cubeVertices[] =
		{
			{ XMFLOAT3(-1.0f,-1.0f,-1.0f), XMFLOAT3(-0.5773f, 0.5773f, 0.5773f),   XMFLOAT2(1.0f, 0.0f) } ,
			{ XMFLOAT3(-1.0f,-1.0f, 1.0f), XMFLOAT3(-0.5773f, 0.5773f, -0.5773f),  XMFLOAT2(0.0f, 0.0f)  },
			{ XMFLOAT3(-1.0f, 1.0f,-1.0f), XMFLOAT3(-0.5773f, -0.5773f, 0.5773f),  XMFLOAT2(1.0f, 1.0f)  },
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
		int offset = 5;
		const unsigned int xSize = 10, zSize = 10;  // HACK: Change the plane size in the index buffer
		SimpleVertex planeVertices[(xSize + 1) * (zSize + 1)];

		// Populate the plane vertex buffer
		for (int i = 0, z = 0; z <= zSize; z++)
		{
			for (int x = 0; x <= xSize; x++)
			{
				planeVertices[i] = { XMFLOAT3(x - offset, 0.0f, z - offset), XMFLOAT3(0.0f, 1.0f, 0.0f) };
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
		const unsigned int xSize = 10, zSize = 10;  // HACK: Change the plane size in the vertex buffer
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
	RECT rc = { 0, 0, 640, 480 };
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
			D3D11_SDK_VERSION, &sd, &_pSwapChain, &_pd3dDevice, &_featureLevel, &_pImmediateContext);
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
	_pImmediateContext->OMSetRenderTargets(1, &_pRenderTargetView, _depthStencilView);
	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)_WindowWidth;
	vp.Height = (FLOAT)_WindowHeight;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	_pImmediateContext->RSSetViewports(1, &vp);

	hr = InitShadersAndInputLayout();
	if (FAILED(hr))
	{
		return hr;
	}

	InitVertexBuffer();

	InitIndexBuffer();

	// Initialize lighting
	InitLights();

	// Initialize textures
	InitTextures();

	// Set primitive topology
	_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = _pd3dDevice->CreateBuffer(&bd, nullptr, &_pConstantBuffer);


	// Define rasterizer state - Wire Frame
	D3D11_RASTERIZER_DESC wfdesc;
	ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));  // Clear memory
	wfdesc.FillMode = D3D11_FILL_WIREFRAME; //D3D10_FILL_SOLID for solid rendering
	wfdesc.CullMode = D3D11_CULL_NONE;  // FRONT: for front face culling (front face not rendered) | default is BACK
	wfdesc.DepthClipEnable = true;  // Enable clipping
	hr = _pd3dDevice->CreateRasterizerState(&wfdesc, &_wireFrame);

	// Define rasterizer state - Solid
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));  // Clear memory
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	hr = _pd3dDevice->CreateRasterizerState(&solidDesc, &_solid);


	if (FAILED(hr))
		return hr;

	return S_OK;
}

void Application::Cleanup()
{
	if (_pImmediateContext)  _pImmediateContext->ClearState();

	if (_wireFrame)			 _wireFrame->Release();
	if (_solid)				 _solid->Release();
	if (_pConstantBuffer)	 _pConstantBuffer->Release();
	if (_pCubeVertexBuffer)		 _pCubeVertexBuffer->Release();
	if (_pCubeIndexBuffer)		 _pCubeIndexBuffer->Release();
	if (_depthStencilView)	 _depthStencilView->Release();
	if (_depthStencilBuffer) _depthStencilBuffer->Release();
	if (_pVertexLayout)		 _pVertexLayout->Release();
	if (_pVertexShader)      _pVertexShader->Release();
	if (_pPixelShader)		 _pPixelShader->Release();
	if (_pRenderTargetView)  _pRenderTargetView->Release();
	if (_pSwapChain)		 _pSwapChain->Release();
	if (_pImmediateContext)  _pImmediateContext->Release();
	if (_pd3dDevice)		 _pd3dDevice->Release();
}



void Application::Update()
{
	// Change draw type
	constexpr char KEYUP = 0x1;
	if (GetAsyncKeyState(VK_RCONTROL) & KEYUP) // Set wire frame
	{
		if (!isWireFrame)  // Is solid, set wire frame
			_pImmediateContext->RSSetState(_wireFrame);
		if (isWireFrame)  // Is wire frame, set solid
			_pImmediateContext->RSSetState(_solid);

		isWireFrame = !isWireFrame;
	}

	// Change current camera
	if (GetAsyncKeyState(0x31) || staticCam->enabled)  // 1: Static camera
	{
		currentCamera.View = staticCam->GetView();
		currentCamera.Projection = staticCam->GetProj();

		staticCam->enabled = true;
		orbitCam->enabled = false;
		fpsCam->enabled = false;
	}
	if (GetAsyncKeyState(0x32) || orbitCam->enabled)  // 2: Orbit camera
	{
		currentCamera.View = orbitCam->GetView();
		currentCamera.Projection = orbitCam->GetProj();

		staticCam->enabled = false;
		orbitCam->enabled = true;
		fpsCam->enabled = false;
	}
	if (GetAsyncKeyState(0x33) || fpsCam->enabled)  // 3: FPS camera
	{
		currentCamera.View = fpsCam->GetView();
		currentCamera.Projection = fpsCam->GetProj();

		staticCam->enabled = false;
		orbitCam->enabled = false;
		fpsCam->enabled = true;
	}



	// Update our time
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

	// Update cameras here
	fpsCam->Update();
	staticCam->Update();
	orbitCam->Update();

	// Donut
	XMStoreFloat4x4(&_meshWorld, XMMatrixRotationX(-t) * XMMatrixTranslation(cos(t * -1) * 6, 3, 3));
	// Pyramid
	XMStoreFloat4x4(&_pyramidWorld, XMMatrixRotationX(t) * XMMatrixTranslation(cos(t * -1) * 6, 0, 3));
	// Cube
	XMStoreFloat4x4(&_cubeWorld, XMMatrixRotationX(t) * XMMatrixTranslation(sin(t) * 6, 0, 3));
	// Plane
	XMStoreFloat4x4(&_planeWorld, XMMatrixTranslation(2.0f, -1.5f, 2.0f));// *XMMatrixScaling(0.4f, 0.4f, 0.4f));
}

void Application::Draw()
{
	// Clear the back buffer
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
	_pImmediateContext->ClearRenderTargetView(_pRenderTargetView, ClearColor);

	// Clear depth stencil 
	_pImmediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	XMMATRIX cubeworld;
	XMMATRIX pyramidWorld;
	XMMATRIX planeWorld;
	XMMATRIX meshWorld;

	// TODO: Change camera here
	XMMATRIX view = XMLoadFloat4x4(&currentCamera.View);
	XMMATRIX transposeView = XMMatrixTranspose(view);
	XMMATRIX projection = XMLoadFloat4x4(&currentCamera.Projection);
	XMFLOAT4X4 eye;
	
	XMStoreFloat4x4(&eye, view);

	// Update variables
	ConstantBuffer cb;
	cb.mView = transposeView;
	cb.mProjection = XMMatrixTranspose(projection);

	// Lights
	cb.dirLight = directionalLight;
	cb.LightVecW = directionalLight.Direction;
	cb.pointLight = pointLight;
	cb.EyePosW = XMFLOAT3(eye._41, eye._42, eye._43);
	// https://stackoverflow.com/questions/39280104/how-to-get-current-camera-position-from-view-matrix

	_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

	// Renders a triangle
	_pImmediateContext->VSSetShader(_pVertexShader, nullptr, 0);
	_pImmediateContext->VSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetConstantBuffers(0, 1, &_pConstantBuffer);
	_pImmediateContext->PSSetShader(_pPixelShader, nullptr, 0);
	_pImmediateContext->PSSetSamplers(0, 1, &_pSamplerLinear);



	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	

	// HACK: Set plane size
	int xSize = 10, zSize = 10;
	// Use plane mesh data
	_pImmediateContext->IASetVertexBuffers(0, 1, &_pPlaneVertexBuffer, &stride, &offset);
	_pImmediateContext->IASetIndexBuffer(_pPlaneIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	// Draw plane
	planeWorld = XMLoadFloat4x4(&_planeWorld);
	cb.mWorld = XMMatrixTranspose(planeWorld);
	_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	_pImmediateContext->DrawIndexed(xSize * zSize * 6, 0, 0);


	// Use pyramid mesh data
	_pImmediateContext->IASetVertexBuffers(0, 1, &_pPyramidVertexBuffer, &stride, &offset);
	_pImmediateContext->IASetIndexBuffer(_pPyramidIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	// Draw pyramid
	pyramidWorld = XMLoadFloat4x4(&_pyramidWorld);
	cb.mWorld = XMMatrixTranspose(pyramidWorld);
	_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	_pImmediateContext->DrawIndexed(3 * 6, 0, 0);


	// Use cube mesh data
	_pImmediateContext->IASetVertexBuffers(0, 1, &_pCubeVertexBuffer, &stride, &offset);
	_pImmediateContext->IASetIndexBuffer(_pCubeIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	// Draw cube
	cubeworld = XMLoadFloat4x4(&_cubeWorld);
	cb.mWorld = XMMatrixTranspose(cubeworld);
	_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	_pImmediateContext->DrawIndexed((3 * 2 * 6), 0, 0);

	
	// Draw mesh
	_pImmediateContext->IASetVertexBuffers(0, 1, &_pMeshVertexBuffer, &objMeshData.VBStride, &objMeshData.VBOffset);
	_pImmediateContext->IASetIndexBuffer(_pMeshIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// Use  mesh data
	meshWorld = XMLoadFloat4x4(&_meshWorld);
	cb.mWorld = XMMatrixTranspose(meshWorld);
	_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);
	_pImmediateContext->DrawIndexed(objMeshData.IndexCount, 0, 0);

	// Present our back buffer to our front buffer
	_pSwapChain->Present(0, 0);
}