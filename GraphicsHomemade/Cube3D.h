#pragma once
#include "BaseWindow.h"
#include <DirectXMath.h>

class Cube3D: public directXHelper::BaseWindow
{
	struct CoordAndColor{
		DirectX::XMFLOAT3 coord;
		DirectX::XMFLOAT4 color;
	};
	DirectX::XMVECTOR CameraPos;
	float angleCoord;

	CoordAndColor cubeVertex[8];
	WORD indices[36];

	// ** DIRECTX VARIABLES
	ID3D11InputLayout* D11_inputLayout;
	ID3D11Buffer* D11_vertexBuffer;
	ID3D11Buffer* D11_indexBuffer;

	ID3D11Buffer* D11_wordMatrix;
	ID3D11Buffer* D11_viewMatrix;
	ID3D11Buffer* D11_projectionMatrix;
	/*ID3D11Buffer* D11_cameraPos;*/
		
	// SHADER
	ID3D11VertexShader* D11_vertexShader;
	ID3D11PixelShader* D11_pixelShader;

	int implementedRender();
	int LoadResources();

public:
	Cube3D();
	~Cube3D();
};

