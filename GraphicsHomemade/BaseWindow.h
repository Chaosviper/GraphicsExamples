#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <string>

namespace directXHelper{

	class BaseWindow
	{
		// ** Window params
		LONG width, height;
		LPCSTR windowUniqueName;
		LPCSTR windowDisplayedName;

		HWND g_WindowHandle; //ID dell'istanza della finestra

		// ** Other variables
		FLOAT clearColor[4];



		// ** PRIVATE FUNCTIONS
		int InitWindow(HINSTANCE hInstance, int cmdShow);
		int InitDirectX();

		int Render();

		void Clear(FLOAT clearDepth, UINT8 clearStencil);

	protected:
		// ** The 4 essential directX element
		ID3D11Device* g_d3dDevice; // A ID3D11Device instance is used for allocating GPU resources such as buffers, textures, shaders, and state objects (to name a few).
		ID3D11DeviceContext* g_d3dDeviceContext; // The ID3D11DeviceContext is used to configure the rendering pipeline and draw geometry
		IDXGISwapChain* g_d3dSwapChain; // The IDXGISwapChain stores the buffers that are used for rendering data. This interface is also used to determine how the buffers are swapped when the rendered image should be presented to the screen
		ID3D11RenderTargetView* g_d3dRenderTargetView; // Render target view for the back buffer of the swap chain.
		ID3D11DepthStencilView* mPDepthStencilView; // Depth buffer view

		// ** Extra directX element
		ID3D11DepthStencilState* g_d3dDepthStencilState;// Define the functionality of the depth/stencil stages.


		virtual int implementedRender();
		virtual int LoadResources();
	public:
		BaseWindow();

		int InitAll(HINSTANCE hInstance, int cmdShow);
		int Run();

		virtual ~BaseWindow();
	};

	// Safely release a COM object.
	template<typename T>
	inline void SafeRelease(T& ptr)
	{
		if (ptr != NULL)
		{
			ptr->Release();
			ptr = NULL;
		}
	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

}