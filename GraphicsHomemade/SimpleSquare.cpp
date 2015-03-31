#include "SimpleSquare.h"
#include <assert.h>
#include <d3dcompiler.h> // required for loading and compiling HLSL shaders


SimpleSquare::SimpleSquare()
{
	g_Indicies[0] = 0;
	g_Indicies[1] = 1;
	g_Indicies[2] = 2;
	g_Indicies[3] = 0;
	g_Indicies[4] = 2;
	g_Indicies[5] = 3;

	g_Vertices[0] = { DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f) };
	g_Vertices[1] = { DirectX::XMFLOAT3(-0.5f, 0.5f, 0.5f) };
	g_Vertices[2] = { DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f) };
	g_Vertices[3] = { DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f) };

	constColorForPixel = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
}

int SimpleSquare::implementedRender(){

	const UINT vertexStride = sizeof(DirectX::XMFLOAT3);
	const UINT offset = 0;

	// ** Imput Assembler Stage
	g_d3dDeviceContext->IASetInputLayout(inputLayout);
	g_d3dDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexStride, &offset);
	g_d3dDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	g_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ** Vertex shader stage
	g_d3dDeviceContext->VSSetShader(g_d3dVertexShader, nullptr, 0);
	// --- eventuali buffer costanti da settare ---

	// ** Pixel shader stage
	g_d3dDeviceContext->PSSetShader(g_d3dPixelShader, nullptr, 0);
	// Setta il buffer costante nel PS (lo avrei potuto fare anche in dichiarazione, tanto e' SEMPRE SEMPRE costante (non solo nella DrawCall)
	g_d3dDeviceContext->PSSetConstantBuffers(0, 1, &constColorForPixelBuffer);
	// --- eventuali altri buffer costanti da settare ---

	// ** Draw
	g_d3dDeviceContext->DrawIndexed(_countof(g_Indicies), 0, 0);

	// NOTA: "Merger Stage" (OMSetRenderTarget) e "Swap del back buffer" (present) vengono fatti dalla classe padre!
	return 0;
}

int SimpleSquare::LoadResources(){
	assert(g_d3dDevice);

	// ** VERTEX BUFFER
	D3D11_BUFFER_DESC vertexBufferDescription;
	ZeroMemory(&vertexBufferDescription, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescription.ByteWidth = sizeof(VertexPos) * _countof(g_Vertices);
	vertexBufferDescription.CPUAccessFlags = 0;
	vertexBufferDescription.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resDataVertex;
	ZeroMemory(&resDataVertex, sizeof(D3D11_SUBRESOURCE_DATA));

	resDataVertex.pSysMem = g_Vertices;

	HRESULT res = g_d3dDevice->CreateBuffer(&vertexBufferDescription, &resDataVertex, &vertexBuffer);
	if (FAILED(res)){
		return -1;
	}

	// ** INDEX BUFFER
	D3D11_BUFFER_DESC indexBufferDescription;
	ZeroMemory(&indexBufferDescription, sizeof(D3D11_BUFFER_DESC));

	indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDescription.ByteWidth = sizeof(WORD) * _countof(g_Indicies);
	indexBufferDescription.CPUAccessFlags = 0;
	indexBufferDescription.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resDataIndicies;
	ZeroMemory(&resDataIndicies, sizeof(D3D11_SUBRESOURCE_DATA));

	resDataIndicies.pSysMem = g_Indicies;

	res = g_d3dDevice->CreateBuffer(&indexBufferDescription, &resDataIndicies, &indexBuffer);
	if (FAILED(res)){
		return -1;
	}

	// ** BUFFER AGGIUNTIVO PER COLORE FIXED DEI PIXEL
	D3D11_BUFFER_DESC constPixelBufferDesc;
	ZeroMemory(&constPixelBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constPixelBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constPixelBufferDesc.ByteWidth = sizeof(DirectX::XMFLOAT4);
	constPixelBufferDesc.CPUAccessFlags = 0;
	constPixelBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	res = g_d3dDevice->CreateBuffer(&constPixelBufferDesc, nullptr, &constColorForPixelBuffer);
	if (FAILED(res)){
		return -1;
	}

	g_d3dDeviceContext->UpdateSubresource(constColorForPixelBuffer, 0, nullptr, &constColorForPixel, 0, 0);

	// ** LOAD PRECOMPILED VERTEX SHADER

	ID3DBlob* vertexShaderBlob;
#if _DEBUG
	LPCWSTR compiledVertexShaderObject = L"myVertexShader_d.cso";
#else
	LPCWSTR compiledVertexShaderObject = L"myVertexShader.cso";
#endif

	res = D3DReadFileToBlob(compiledVertexShaderObject, &vertexShaderBlob);
	if (FAILED(res))
	{
		return -1;
	}

	res = g_d3dDevice->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &g_d3dVertexShader);
	if (FAILED(res))
	{
		return -1;
	}

	// *** CREATE INPUT LAYOUT FOR VERTEX SHADER
	D3D11_INPUT_ELEMENT_DESC vertexLayoutDescription[] = {
		// first argument: The HLSL semantic associated with this element in a shader input-signature.
		{ "POSITION_VAR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexPos, pos), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	res = g_d3dDevice->CreateInputLayout(vertexLayoutDescription, _countof(vertexLayoutDescription), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);

	if (FAILED(res)){
		return -1;
	}

	directXHelper::SafeRelease(vertexShaderBlob);

	// ** LOAD PRECOMPILED PIXEL SHADER
	ID3DBlob* pixelShaderBlob;
#if _DEBUG
	LPCWSTR compiledPixelShaderObject = L"myPixelShader_d.cso";
#else
	LPCWSTR compiledPixelShaderObject = L"myPixelShader.cso";
#endif

	res = D3DReadFileToBlob(compiledPixelShaderObject, &pixelShaderBlob);
	if (FAILED(res))
	{
		return -1;
	}

	res = g_d3dDevice->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &g_d3dPixelShader);
	if (FAILED(res))
	{
		return -1;
	}

	directXHelper::SafeRelease(pixelShaderBlob);

	return 0;
}

SimpleSquare::~SimpleSquare()
{
}
