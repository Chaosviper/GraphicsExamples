#include "Lighting.h"
#include <assert.h>
#include <d3dcompiler.h> // required for loading and compiling HLSL shaders

using namespace DirectX;

Lighting::Lighting()
{
	// DIRECTX => ORARIO, LEFT HANDED

	// FRONT FACE
	cubeVertex[0].coord = XMFLOAT3(-1.0f, 1.0f, -1.0f); // 1
	cubeVertex[1].coord = XMFLOAT3(1.0f, 1.0f, -1.0f); // 2
	cubeVertex[2].coord = XMFLOAT3(-1.0f, -1.0f, -1.0f); // 3
	cubeVertex[3].coord = XMFLOAT3(1.0f, -1.0f, -1.0f); // 4
	cubeVertex[0].normals = XMFLOAT3(0.0f, 0.0f, -1.0f);
	cubeVertex[1].normals = XMFLOAT3(0.0f, 0.0f, -1.0f);
	cubeVertex[2].normals = XMFLOAT3(0.0f, 0.0f, -1.0f);
	cubeVertex[3].normals = XMFLOAT3(0.0f, 0.0f, -1.0f);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 3;
	indices[4] = 2;
	indices[5] = 1;

	// TOP FACE
	cubeVertex[4].coord = XMFLOAT3(-1.0f, 1.0f, -1.0f); // 1
	cubeVertex[5].coord = XMFLOAT3(1.0f, 1.0f, -1.0f); // 2
	cubeVertex[6].coord = XMFLOAT3(-1.0f, 1.0f, 1.0f); // 5
	cubeVertex[7].coord = XMFLOAT3(1.0f, 1.0f, 1.0f); // 6
	cubeVertex[4].normals = XMFLOAT3(0.0f, 1.0f, 0.0f);
	cubeVertex[5].normals = XMFLOAT3(0.0f, 1.0f, 0.0f);
	cubeVertex[6].normals = XMFLOAT3(0.0f, 1.0f, 0.0f);
	cubeVertex[7].normals = XMFLOAT3(0.0f, 1.0f, 0.0f);

	indices[6] = 4;
	indices[7] = 6;
	indices[8] = 5;
	indices[9] = 5;
	indices[10] = 6;
	indices[11] = 7;

	// BACK FACE
	cubeVertex[8].coord = XMFLOAT3(-1.0f, 1.0f, 1.0f); // 5
	cubeVertex[9].coord = XMFLOAT3(1.0f, 1.0f, 1.0f); // 6
	cubeVertex[10].coord = XMFLOAT3(1.0f, -1.0f, 1.0f); // 8
	cubeVertex[11].coord = XMFLOAT3(-1.0f, -1.0f, 1.0f); // 7
	cubeVertex[8].normals = XMFLOAT3(0.0f, 0.0f, 1.0f);
	cubeVertex[9].normals = XMFLOAT3(0.0f, 0.0f, 1.0f);
	cubeVertex[10].normals = XMFLOAT3(0.0f, 0.0f, 1.0f);
	cubeVertex[11].normals = XMFLOAT3(0.0f, 0.0f, 1.0f);

	indices[12] = 9;
	indices[13] = 8;
	indices[14] = 10;
	indices[15] = 10;
	indices[16] = 8;
	indices[17] = 11;

	// BOTTOM FACE
	cubeVertex[12].coord = XMFLOAT3(-1.0f, -1.0f, -1.0f); // 3
	cubeVertex[13].coord = XMFLOAT3(1.0f, -1.0f, -1.0f); // 4
	cubeVertex[14].coord = XMFLOAT3(1.0f, -1.0f, 1.0f); // 8
	cubeVertex[15].coord = XMFLOAT3(-1.0f, -1.0f, 1.0f); // 7
	cubeVertex[12].normals = XMFLOAT3(0.0f, -1.0f, 0.0f);
	cubeVertex[13].normals = XMFLOAT3(0.0f, -1.0f, 0.0f);
	cubeVertex[14].normals = XMFLOAT3(0.0f, -1.0f, 0.0f);
	cubeVertex[15].normals = XMFLOAT3(0.0f, -1.0f, 0.0f);

	indices[18] = 12;
	indices[19] = 13;
	indices[20] = 15;
	indices[21] = 13;
	indices[22] = 14;
	indices[23] = 15;

	// RIGHT FACE
	cubeVertex[16].coord = XMFLOAT3(1.0f, 1.0f, -1.0f); // 2
	cubeVertex[17].coord = XMFLOAT3(1.0f, -1.0f, -1.0f); // 4
	cubeVertex[18].coord = XMFLOAT3(1.0f, 1.0f, 1.0f); // 6
	cubeVertex[19].coord = XMFLOAT3(1.0f, -1.0f, 1.0f); // 8
	cubeVertex[16].normals = XMFLOAT3(1.0f, 0.0f, 0.0f);
	cubeVertex[17].normals = XMFLOAT3(1.0f, 0.0f, 0.0f);
	cubeVertex[18].normals = XMFLOAT3(1.0f, 0.0f, 0.0f);
	cubeVertex[19].normals = XMFLOAT3(1.0f, 0.0f, 0.0f);

	indices[24] = 16;
	indices[25] = 18;
	indices[26] = 17;
	indices[27] = 17;
	indices[28] = 18;
	indices[29] = 19;

	// LEFT FACE
	cubeVertex[20].coord = XMFLOAT3(-1.0f, 1.0f, -1.0f); // 1
	cubeVertex[21].coord = XMFLOAT3(-1.0f, -1.0f, -1.0f); // 3
	cubeVertex[22].coord = XMFLOAT3(-1.0f, 1.0f, 1.0f); // 5
	cubeVertex[23].coord = XMFLOAT3(-1.0f, -1.0f, 1.0f); // 7
	cubeVertex[20].normals = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	cubeVertex[21].normals = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	cubeVertex[22].normals = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	cubeVertex[23].normals = XMFLOAT3(-1.0f, 0.0f, 0.0f);
	
	indices[30] = 22;
	indices[31] = 20;
	indices[32] = 23;
	indices[33] = 23;
	indices[34] = 20;
	indices[35] = 21;

	// Variable used for view matrix creation
	CameraPos.m128_f32[0] = 0.0f;
	CameraPos.m128_f32[1] = -1.3f;
	CameraPos.m128_f32[2] = -3.5f;
	CameraPos.m128_f32[3] = 1.0f;

}


int Lighting::LoadResources(){

	assert(g_d3dDevice);

	// ** VERTEX BUFFER
	D3D11_BUFFER_DESC vertexBufferDescription;
	ZeroMemory(&vertexBufferDescription, sizeof(D3D11_BUFFER_DESC));

	vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescription.ByteWidth = sizeof(CoordAndNormals) * _countof(cubeVertex);
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


	//  ***************** Calcolo matrici di trasformazione (world fatta ad ogni render)
	XMVECTOR vectorUP = { 0.0f, 1.0f, 0.0f, 0.0f };
	XMVECTOR focusPoint = { 0.0f, 0.0f, 5.0f, 1.0f };


	transform.worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 5.0f);
	// LH => left hand
	transform.viewMatrix = XMMatrixLookAtLH(CameraPos, focusPoint, vectorUP);
	transform.projectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(40.0f), 1.3f, 1.0f, 100);
	// ****************** END

	// ** BUFFER AGGIUNTIVO PER TRANSFORMATION INFO
	D3D11_BUFFER_DESC constTransfMatrixBufferDesc;
	ZeroMemory(&constTransfMatrixBufferDesc, sizeof(D3D11_BUFFER_DESC));

	constTransfMatrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constTransfMatrixBufferDesc.ByteWidth = sizeof(TransformInfo);
	constTransfMatrixBufferDesc.CPUAccessFlags = 0;
	constTransfMatrixBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA resDataWorldMatrix;
	ZeroMemory(&resDataWorldMatrix, sizeof(D3D11_SUBRESOURCE_DATA));

	resDataWorldMatrix.pSysMem = &transform;

	res = g_d3dDevice->CreateBuffer(&constTransfMatrixBufferDesc, &resDataWorldMatrix, &D11_transformInfo);
	if (FAILED(res)){
		return -1;
	}


	// ** LOAD PRECOMPILED VERTEX SHADER

	ID3DBlob* vertexShaderBlob;
#if _DEBUG
	LPCWSTR compiledVertexShaderObject = L"SimpleDiffuseVShader_d.cso";
#else
	LPCWSTR compiledVertexShaderObject = L"SimpleDiffuseVShader.cso";
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
		{ "POSITION_VAR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(CoordAndNormals, coord), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL_VAR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(CoordAndNormals, normals), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	res = g_d3dDevice->CreateInputLayout(vertexLayoutDescription, _countof(vertexLayoutDescription), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &D11_inputLayout);

	if (FAILED(res)){
		return -1;
	}

	directXHelper::SafeRelease(vertexShaderBlob);

	// ** LOAD PRECOMPILED PIXEL SHADER
	ID3DBlob* pixelShaderBlob;

#if _DEBUG
	LPCWSTR compiledPixelShaderObject2 = L"SimplePixelShader_d.cso";
#else
	LPCWSTR compiledPixelShaderObject2 = L"SimplePixelShader.cso";
#endif

	res = D3DReadFileToBlob(compiledPixelShaderObject2, &pixelShaderBlob);
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

int Lighting::implementedRender(){

	const UINT vertexStride = sizeof(CoordAndNormals);
	const UINT offset = 0;

	// ** Imput Assembler Stage
	g_d3dDeviceContext->IASetInputLayout(D11_inputLayout);
	g_d3dDeviceContext->IASetVertexBuffers(0, 1, &D11_vertexBuffer, &vertexStride, &offset);
	g_d3dDeviceContext->IASetIndexBuffer(D11_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	g_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// ** rotating cube world matrix updating
	cubeAngleCoord += 0.0005f;

	transform.worldMatrix = XMMatrixMultiply(XMMatrixRotationY(cubeAngleCoord), XMMatrixTranslation(0.0f, 0.0f, 5.0f));

	// ** camera world matrix updating
	if (KeyPressed[RIGHT]){
		cameraAngleY = -0.001f;
	}
	else if (KeyPressed[LEFT]){
		cameraAngleY = 0.001f;
	}
	else{
		cameraAngleY = 0.0f;
	}

	if (KeyPressed[UP]){
		// controllo che la Z sia al di qua del cubo, altrimenti il sistema di riferimento e' invertito! y positivo = y negativo! 
		//		nel caso sia al di la, li inverto!
		// Situazione:
		//		Camera |---- (-3.5) ----> (0,0,0) |---- [5] ----> Primo cubo                         <<<ASSE Z!>>>
		cameraAngleX = (CameraPos.m128_f32[2] >= 5) ? -0.001f : 0.001;
	}
	else if (KeyPressed[DOWN]){
		cameraAngleX = (CameraPos.m128_f32[2] >= 5) ? 0.001f : -0.001;
	}
	else{
		cameraAngleX = 0.0f;
	}

	XMMATRIX res = XMMatrixMultiply(XMMatrixRotationX(cameraAngleX), XMMatrixRotationY(cameraAngleY));
	res = XMMatrixMultiply(res, XMMatrixTranslation(0.0f, 0.0f, 5.0f));
	res = XMMatrixMultiply(XMMatrixTranslation(0.0f, 0.0f, -5.0f), res);
	CameraPos = XMVector4Transform(CameraPos, res);

	// ** View matrix updating
	const XMVECTOR vectorUP = { 0.0f, 1.0f, 0.0f, 0.0f };
	const XMVECTOR focusPoint = { 0.0f, 0.0f, 5.0f, 1.0f };

	transform.viewMatrix = XMMatrixLookAtLH(CameraPos, focusPoint, vectorUP);
	
	// Update the resource matrix
	g_d3dDeviceContext->UpdateSubresource(D11_transformInfo, 0, nullptr, &transform, 0, 0);

	// ** Vertex shader stage
	g_d3dDeviceContext->VSSetShader(D11_vertexShader, nullptr, 0);
	// Setto i buffer costante nel VS (lo avrei potuto fare anche in dichiarazione, tanto, nel nostro caso, sono SEMPRE SEMPRE costanti (non solo nella DrawCall)
	g_d3dDeviceContext->VSSetConstantBuffers(0, 1, &D11_transformInfo);
	// --- eventuali buffer costanti da settare ---

	// ** Pixel shader stage
	g_d3dDeviceContext->PSSetShader(D11_pixelShader, nullptr, 0);
	// --- eventuali altri buffer costanti da settare ---

	// ** Draw
	g_d3dDeviceContext->DrawIndexed(_countof(indices), 0, 0);

	// ** Static cube behind the first cube
	//XMMATRIX world2Cube = XMMatrixTranslation(1.0f, 0.0f, 8.0f);
	//g_d3dDeviceContext->UpdateSubresource(D11_wordMatrix, 0, nullptr, &world2Cube, 0, 0);

	//// ** Pixel shader sostitution
	//g_d3dDeviceContext->PSSetShader(D11_pixelShaderMonocolored, nullptr, 0);

	//g_d3dDeviceContext->DrawIndexed(_countof(indices), 0, 0);


	// NOTA: "Merger Stage" (OMSetRenderTarget) e "Swap del back buffer" (present) vengono fatti dalla classe padre!

	return 0;
}

Lighting::~Lighting()
{
}
