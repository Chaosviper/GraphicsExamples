#pragma once
#include "BaseWindow.h"
#include <DirectXMath.h>

class SimpleSquare : public directXHelper::BaseWindow
{
private:
	struct VertexPos{
		DirectX::XMFLOAT3 pos;
	};

	VertexPos g_Vertices[4];

	WORD g_Indicies[6];

	DirectX::XMFLOAT4 constColorForPixel;

	// DIRECT X Resources needed
	ID3D11InputLayout* inputLayout;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11Buffer* constColorForPixelBuffer;

	// SHADER
	ID3D11VertexShader* g_d3dVertexShader;
	ID3D11PixelShader* g_d3dPixelShader;

	int implementedRender();
	int LoadResources();
public:
	SimpleSquare();
	~SimpleSquare();
};

