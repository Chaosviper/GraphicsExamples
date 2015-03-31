#include "BaseWindow.h"
#include <assert.h>


namespace directXHelper{

	BaseWindow::BaseWindow() : width(800),
		height(600),
		windowUniqueName("Test"),
		windowDisplayedName("Test1"),
		g_d3dDevice(nullptr),
		g_d3dDeviceContext(nullptr),
		g_d3dSwapChain(nullptr),
		g_d3dRenderTargetView(nullptr),
		mPDepthStencilView(nullptr),
		g_d3dDepthStencilState(nullptr),
		g_WindowHandle(0) {
		clearColor[0] = 0.39f;
		clearColor[1] = 0.58f;
		clearColor[2] = 0.93f;
		clearColor[3] = 1.0f;
	}

	int BaseWindow::InitWindow(HINSTANCE hInstance, int cmdShow){

		// ********************************************* Impostazioni Finestra ************************************************
		WNDCLASSEX wndClass = { 0 };
		wndClass.cbSize = sizeof(WNDCLASSEX); // dimensione in byte della struttura
		wndClass.style = CS_HREDRAW | CS_VREDRAW; // la prima causa refresh se modificata width della scheramta. La seconda per la height
		wndClass.lpfnWndProc = &WndProc; // puntatore ad una procedura che gestira' gli eventi della scheramta. Serve per gestire i messaggi inviati alla schermata
		wndClass.hInstance = hInstance; // instanza del modulo che possiede la classe della schermata
		// "wndClass.hIcon" icona della barra per l'app. Possibile caricarla tramite LoadIcon. NULL => default icon
		wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW); // cursore usato dall'app
		wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // colore di sfondo
		wndClass.lpszMenuName = nullptr; // nome dalla classe menu
		wndClass.lpszClassName = windowUniqueName; // nome che identifica univocamente la classe, usata per creare l'istanza della finestra

		// ************************************************* Registra Finestra ************************************************
		if (!RegisterClassEx(&wndClass)) // Registra la window per mostrarla
		{
			return -1;
		}

		RECT windowRect = { 0, 0, width, height };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE); // Resize della finestra

		// ************************************************** Crea Finestra ***************************************************
		g_WindowHandle = CreateWindowA(windowUniqueName, // deve mechare il nome univoco della finestra
			windowDisplayedName, // nome visualizzato nella title bar
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			nullptr, nullptr, hInstance, nullptr);

		if (!g_WindowHandle)
			return E_FAIL;

		// ************************************************* Mostra Finestra **************************************************
		ShowWindow(g_WindowHandle, cmdShow);

		return S_OK;
	}

	int BaseWindow::InitDirectX(){

		// ******************************************************************************************************
		// ************************************ (1) Informazioni sul render zone ********************************
		// ******************************************************************************************************
		assert(g_WindowHandle != 0);

		RECT clientRect;
		GetClientRect(g_WindowHandle, &clientRect);

		// Compute the exact client dimensions. This will be used
		// to initialize the render targets for our swap chain.
		UINT clientWidth = clientRect.right - clientRect.left;
		UINT clientHeight = clientRect.bottom - clientRect.top;

		// ******************************************************************************************************
		// ************************************ (2) Creazione SwapChain *****************************************
		// ******************************************************************************************************
		
		// ************************************ (2.1)  Descrizione SwapChain ************************************
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Width = clientWidth;
		swapChainDesc.BufferDesc.Height = clientHeight;
		// In this case we specify DXGI_FORMAT_R8G8B8A8_UNORM which creates a 4-component 32-bit unsigned normalized integer format that supports 8 bits per channel including alpha
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // The pixel format of the display. For a list of possible display formats see DXGI_FORMAT
		//  If the window contents are to be displayed full-screen, then this should be the ideal refresh rate at the specified display resolution. If it is a windowed application, then it should be the refresh rate of the desktop.
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = g_WindowHandle;
		// SampleDesc: structure that describes multi-sampling parameters
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Windowed = TRUE;
		
		// ******************************** (2.2) Creazione Device e SwapChain **********************************
		UINT createDeviceFlags = 0;
#if _DEBUG // solo in debug perche' crea overhead inutile in prod
		createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif
		// These are the feature levels that we will accept. Ovvero: versioni di DirectX
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};

		// This will be the feature level that 
		// is used to create our device and swap chain.
		D3D_FEATURE_LEVEL featureLevel;

		HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
			nullptr, createDeviceFlags, featureLevels, _countof(featureLevels),
			D3D11_SDK_VERSION, &swapChainDesc, &g_d3dSwapChain, &g_d3dDevice, &featureLevel,
			&g_d3dDeviceContext);

		if (hr == E_INVALIDARG)
		{
			hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
				nullptr, createDeviceFlags, &featureLevels[1], _countof(featureLevels) - 1,
				D3D11_SDK_VERSION, &swapChainDesc, &g_d3dSwapChain, &g_d3dDevice, &featureLevel,
				&g_d3dDeviceContext);
		}

		if (FAILED(hr))
		{
			return -1;
		}

		// ************************************ (2.3) Retrive Back Buffer ***************************************
		// Next initialize the back buffer of the swap chain and associate it to a 
		// render target view.
		ID3D11Texture2D* backBuffer;
		hr = g_d3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
		if (FAILED(hr))
		{
			return -1;
		}

		// ************************************ (2.4) Link il back buffer al render target **********************
		hr = g_d3dDevice->CreateRenderTargetView(backBuffer, nullptr, &g_d3dRenderTargetView);
		if (FAILED(hr))
		{
			return -1;
		}

		SafeRelease(backBuffer);

		// ******************************************************************************************************
		// ******************************* (3) Creazione Depth Stencil Buffer ***********************************
		// ******************************************************************************************************
		// Create the depth buffer for use with the depth/stencil view.
		
		// ************************************ (3.1) Depth Texture Description *********************************
		D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
		ZeroMemory(&depthStencilBufferDesc, sizeof(D3D11_TEXTURE2D_DESC));

		depthStencilBufferDesc.ArraySize = 1;
		depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilBufferDesc.CPUAccessFlags = 0; // No CPU access required.
		depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilBufferDesc.Width = clientWidth;
		depthStencilBufferDesc.Height = clientHeight;
		depthStencilBufferDesc.MipLevels = 1;
		depthStencilBufferDesc.SampleDesc.Count = 1;
		depthStencilBufferDesc.SampleDesc.Quality = 0;
		depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

		// ************************************ (3.2) Creation of Depth Texture *********************************
		ID3D11Texture2D* pDepthStencil = nullptr;
		hr = g_d3dDevice->CreateTexture2D(&depthStencilBufferDesc, nullptr, &pDepthStencil);
		if (FAILED(hr))
		{
			return -1;
		}

		// *************************** (3.3) Creation of Depth Stencil View Description *************************
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; // if multisample => setting here
		descDSV.Texture2D.MipSlice = 0;
		descDSV.Flags = 0;

		// ******************************** (3.4) Creation of Depth Stencil View ********************************
		hr = g_d3dDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &mPDepthStencilView);
		if (FAILED(hr))
		{
			return -1;
		}

		// **************************** (3.5) EXTRA: setup of Depth Stencil State *******************************
		// Setup depth/stencil state.
		D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc;
		ZeroMemory(&depthStencilStateDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

		depthStencilStateDesc.DepthEnable = TRUE;
		depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
		depthStencilStateDesc.StencilEnable = FALSE;

		hr = g_d3dDevice->CreateDepthStencilState(&depthStencilStateDesc, &g_d3dDepthStencilState);



		// ******************************************************************************************************
		// ************************************ (3) Setting del ViewPort ****************************************
		// ******************************************************************************************************
		/*
		The viewport defines the area in screen space where our final render will go.
		For this application, we will be rendering to the entire client area of the application window
		but we could also define two viewports if we wanted to implement split screen multiplayer or a picture-in-picture effect.

		To configure the viewport, we need to know the width and height of the client area.
		We have already computed the width and height of the client rectangle on lines 243-244 of the InitDirectX function and
		we used the client area to determine the size of the swap chain’s back buffer and the depth-stencil buffer.
		We will set the viewport to the same dimensions
		*/
		// Setta il viewport
		D3D11_VIEWPORT vp;
		vp.Width = static_cast<float>(width);
		vp.Height = static_cast<float>(height);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		g_d3dDeviceContext->RSSetViewports(1, &vp);

		return 0;
	}

	int BaseWindow::InitAll(HINSTANCE hInstance, int cmdShow){
		assert(InitWindow(hInstance, cmdShow) >= 0);
		assert(InitDirectX() >= 0);

		return 0;
	}

	int BaseWindow::Render(){

		Clear(1.0f, 0);

		// Merger stage
		g_d3dDeviceContext->OMSetRenderTargets(1, &g_d3dRenderTargetView, mPDepthStencilView);
		g_d3dDeviceContext->OMSetDepthStencilState(g_d3dDepthStencilState, 1);

		implementedRender();

		g_d3dSwapChain->Present(0, 0);

		return 0;
	}

	int BaseWindow::LoadResources(){
		return 0;
	}

	int BaseWindow::implementedRender(){

		return 0;

	}

	int BaseWindow::Run(){

		assert(LoadResources() >= 0);

		// Main message loop
		MSG msg = { 0 };
		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				Render();
			}
		}

		return (int)msg.wParam;
	}

	// Clear the color and depth buffers.
	void BaseWindow::Clear(FLOAT clearDepth, UINT8 clearStencil)
	{
		g_d3dDeviceContext->ClearRenderTargetView(g_d3dRenderTargetView, clearColor);
		g_d3dDeviceContext->ClearDepthStencilView(mPDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, clearDepth, clearStencil);
	}


	BaseWindow::~BaseWindow()
	{
		SafeRelease(g_d3dDevice);
		SafeRelease(g_d3dDeviceContext);
		SafeRelease(g_d3dSwapChain);
		SafeRelease(g_d3dRenderTargetView);
		SafeRelease(mPDepthStencilView);
		SafeRelease(g_d3dDepthStencilState);
	}






	LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT paintStruct;
		HDC hDC;

		switch (message)
		{
		case WM_PAINT:
		{
			hDC = BeginPaint(hwnd, &paintStruct);
			EndPaint(hwnd, &paintStruct);
		}
		break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}

		return 0;
	}

}