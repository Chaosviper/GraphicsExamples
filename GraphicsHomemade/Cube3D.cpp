#include "Cube3D.h"
#include <assert.h>
#include <d3dcompiler.h> // required for loading and compiling HLSL shaders

using namespace DirectX;

Cube3D::Cube3D()
{
	cubeVertex[0].coord = XMFLOAT3(-1.0f, 1.0f, -1.0f);
	cubeVertex[0].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	cubeVertex[1].coord = XMFLOAT3(1.0f, 1.0f, -1.0f);
	cubeVertex[1].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	cubeVertex[2].coord = XMFLOAT3(1.0f, 1.0f, 1.0f);
	cubeVertex[2].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	cubeVertex[3].coord = XMFLOAT3(-1.0f, 1.0f, 1.0f);
	cubeVertex[3].color = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	cubeVertex[4].coord = XMFLOAT3(-1.0f, -1.0f, -1.0f);
	cubeVertex[4].color = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	cubeVertex[5].coord = XMFLOAT3(1.0f, -1.0f, -1.0f);
	cubeVertex[5].color = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	cubeVertex[6].coord = XMFLOAT3(1.0f, -1.0f, 1.0f);
	cubeVertex[6].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cubeVertex[7].coord = XMFLOAT3(-1.0f, -1.0f, 1.0f);
	cubeVertex[7].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	indices[0] = 3;
	indices[1] = 1;
	indices[2] = 0;
	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;
	indices[6] = 0;
	indices[7] = 5;
	indices[8] = 4;
	indices[9] = 1;
	indices[10] = 5;
	indices[11] = 0;
	indices[12] = 3;
	indices[13] = 4;
	indices[14] = 7;
	indices[15] = 0;
	indices[16] = 4;
	indices[17] = 3;
	indices[18] = 1;
	indices[19] = 6;
	indices[20] = 5;
	indices[21] = 2;
	indices[22] = 6;
	indices[23] = 1;
	indices[24] = 2;
	indices[25] = 7;
	indices[26] = 6;
	indices[27] = 3;
	indices[28] = 7;
	indices[29] = 2;
	indices[30] = 6;
	indices[31] = 4;
	indices[32] = 5;
	indices[33] = 7;
	indices[34] = 4;
	indices[35] = 6;

	// Variable used for view matrix creation
	CameraPos.m128_f32[0] = 0.0f;
	CameraPos.m128_f32[1] = -1.3f;
	CameraPos.m128_f32[2] = -3.5f;
	CameraPos.m128_f32[3] = 1.0f;

}


int Cube3D::LoadResources(){

	assert(g_d3dDevice);

	// ** VERTEX BUFFER
	D3D11_BUFFER_DESC vertexBufferDescription;
	ZeroMemory(&vertexBufferDescription, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescription.ByteWidth = sizeof(CoordAndColor) * _countof(cubeVertex);
	vertexBufferDescription.CPUAccessFlags = 0;
	vertexBufferDescription.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resDataVertex;
	ZeroMemory(&resDataVertex, sizeof(D3D11_SUBRESOURCE_DATA));

	resDataVertex.pSysMem = cubeVertex;

	HRESULT res = g_d3dDevice->CreateBuffer(&vertexBufferDescription, &resDataVertex, &D11_vertexBuffer);
	if (FAILED(res)){
		return -1;
	}

	// ** INDEX BUFFER
	D3D11_BUFFER_DESC indexBufferDescription;
	ZeroMemory(&indexBufferDescription, sizeof(D3D11_BUFFER_DESC));

	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescription.ByteWidth = sizeof(WORD) * _countof(indices);
	indexBufferDescription.CPUAccessFlags = 0;
	indexBufferDescription.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resDataIndicies;
	ZeroMemory(&resDataIndicies, sizeof(D3D11_SUBRESOURCE_DATA));

	resDataIndicies.pSysMem = indices;

	res = g_d3dDevice->CreateBuffer(&indexBufferDescription, &resDataIndicies, &D11_indexBuffer);
	if (FAILED(res)){
		return -1;
	}


	//  ***************** Calcolo matrici di trasformazione
	XMVECTOR vectorUP = { 0.0f, 1.0f, 0.0f, 0.0f };
	XMVECTOR focusPoint = { 0.0f, 0.0f, 1.0f, 1.0f };


	XMMATRIX worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 5.0f);
	// LH => left hand
	XMMATRIX viewMatrix = XMMatrixLookAtLH(CameraPos, focusPoint, vectorUP);
	XMMATRIX porjectionMatrix = XMMatrixPerspectiveFovLH(11.35f, 1.3f, 1.0f, 10);
	// ****************** END

	// ** BUFFER AGGIUNTIVO PER WORLD MATRIX
	D3D11_BUFFER_DESC constTransfMatrixBufferDesc;
	ZeroMemory(&constTransfMatrixBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constTransfMatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constTransfMatrixBufferDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	constTransfMatrixBufferDesc.CPUAccessFlags = 0;
	constTransfMatrixBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resDataWorldMatrix;
	ZeroMemory(&resDataWorldMatrix, sizeof(D3D11_SUBRESOURCE_DATA));

	resDataWorldMatrix.pSysMem = &worldMatrix;

	res = g_d3dDevice->CreateBuffer(&constTransfMatrixBufferDesc, &resDataWorldMatrix, &D11_wordMatrix);
	if (FAILED(res)){
		return -1;
	}

	// ** BUFFER AGGIUNTIVO PER VIEW MATRIX
	D3D11_SUBRESOURCE_DATA resDataViewMatrix;
	ZeroMemory(&resDataViewMatrix, sizeof(D3D11_SUBRESOURCE_DATA));

	resDataViewMatrix.pSysMem = &viewMatrix;

	res = g_d3dDevice->CreateBuffer(&constTransfMatrixBufferDesc, &resDataViewMatrix, &D11_viewMatrix);
	if (FAILED(res)){
		return -1;
	}

	// ** BUFFER AGGIUNTIVO PER PROJECTION MATRIX
	D3D11_SUBRESOURCE_DATA resDataProjMatrix;
	ZeroMemory(&resDataProjMatrix, sizeof(D3D11_SUBRESOURCE_DATA));

	resDataProjMatrix.pSysMem = &porjectionMatrix;

	res = g_d3dDevice->CreateBuffer(&constTransfMatrixBufferDesc, &resDataProjMatrix, &D11_projectionMatrix);
	if (FAILED(res)){
		return -1;
	}

	// ** LOAD PRECOMPILED VERTEX SHADER

	ID3DBlob* vertexShaderBlob;
#if _DEBUG
	LPCWSTR compiledVertexShaderObject = L"3DVertexShader_d.cso";
#else
	LPCWSTR compiledVertexShaderObject = L"3DVertexShader.cso";
#endif

	res = D3DReadFileToBlob(compiledVertexShaderObject, &vertexShaderBlob);
	if (FAILED(res))
	{
		return -1;
	}

	res = g_d3dDevice->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &D11_vertexShader);
	if (FAILED(res))
	{
		return -1;
	}

	// *** CREATE INPUT LAYOUT FOR VERTEX SHADER
	D3D11_INPUT_ELEMENT_DESC vertexLayoutDescription[] = {
		// first argument: The HLSL semantic associated with this element in a shader input-signature.
		{ "POSITION_VAR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(CoordAndColor, coord), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR_VAR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, offsetof(CoordAndColor, color), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	res = g_d3dDevice->CreateInputLayout(vertexLayoutDescription, _countof(vertexLayoutDescription), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &D11_inputLayout);

	if (FAILED(res)){
		return -1;
	}

	directXHelper::SafeRelease(vertexShaderBlob);

	// ** LOAD PRECOMPILED PIXEL SHADER
	ID3DBlob* pixelShaderBlob;
#if _DEBUG
	LPCWSTR compiledPixelShaderObject = L"3DPixelShader_d.cso";
#else
	LPCWSTR compiledPixelShaderObject = L"3DPixelShader.cso";
#endif

	res = D3DReadFileToBlob(compiledPixelShaderObject, &pixelShaderBlob);
	if (FAILED(res))
	{
		return -1;
	}

	res = g_d3dDevice->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &D11_pixelShader);
	if (FAILED(res))
	{
		return -1;
	}

	directXHelper::SafeRelease(pixelShaderBlob);

	return 0;
}

int Cube3D::implementedRender(){

	const UINT vertexStride = sizeof(CoordAndColor);
	const UINT offset = 0;

	// ** Imput Assembler Stage
	g_d3dDeviceContext->IASetInputLayout(D11_inputLayout);
	g_d3dDeviceContext->IASetVertexBuffers(0, 1, &D11_vertexBuffer, &vertexStride, &offset);
	g_d3dDeviceContext->IASetIndexBuffer(D11_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	g_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ** Vertex shader stage
	g_d3dDeviceContext->VSSetShader(D11_vertexShader, nullptr, 0);
	// Setto i buffer costante nel VS (lo avrei potuto fare anche in dichiarazione, tanto, nel nostro caso, sono SEMPRE SEMPRE costanti (non solo nella DrawCall)
	g_d3dDeviceContext->VSSetConstantBuffers(0, 1, &D11_wordMatrix);
	g_d3dDeviceContext->VSSetConstantBuffers(1, 1, &D11_viewMatrix);
	g_d3dDeviceContext->VSSetConstantBuffers(2, 1, &D11_projectionMatrix);
	// --- eventuali buffer costanti da settare ---

	// ** Pixel shader stage
	g_d3dDeviceContext->PSSetShader(D11_pixelShader, nullptr, 0);
	// --- eventuali altri buffer costanti da settare ---

	// ** Draw
	g_d3dDeviceContext->DrawIndexed(_countof(indices), 0, 0);

	// NOTA: "Merger Stage" (OMSetRenderTarget) e "Swap del back buffer" (present) vengono fatti dalla classe padre!

	return 0;
}

Cube3D::~Cube3D()
{
}
