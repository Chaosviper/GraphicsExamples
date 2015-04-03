#pragma once
#include "BaseWindow.h"
#include <DirectXMath.h>

#define BLINNPHONG

class Lighting : public directXHelper::BaseWindow
{
	struct CoordAndNormals{
		DirectX::XMFLOAT3 coord;
		DirectX::XMFLOAT3 normals;
	};

	struct TransformInfo{
		DirectX::XMMATRIX worldMatrix;
		DirectX::XMMATRIX viewMatrix;
		DirectX::XMMATRIX projectionMatrix;
	};

	DirectX::XMVECTOR CameraPos;
	float cubeAngleCoord;
	float cameraAngleX;
	float cameraAngleY;

	TransformInfo transform;


	CoordAndNormals cubeVertex[24]; // Ne ho bisogno 24 perche' devo fare vertex seam per le normali!!!
	WORD indices[36];

	// ** DIRECTX VARIABLES
	ID3D11InputLayout* D11_inputLayout;
	ID3D11Buffer* D11_vertexBuffer;
	ID3D11Buffer* D11_indexBuffer;

	ID3D11Buffer* D11_transformInfo;
	ID3D11Buffer* D11_lightPos;
#ifdef BLINNPHONG
	ID3D11Buffer* D11_cameraPos;
#endif
	/*ID3D11Buffer* D11_cameraPos;*/
		
	// SHADER
	ID3D11VertexShader* D11_vertexShader;
	ID3D11PixelShader* D11_pixelShader;

	int implementedRender();
	int LoadResources();

public:
	Lighting();
	~Lighting();
};

