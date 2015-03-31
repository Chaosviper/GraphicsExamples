
struct appDataIn{
	// sematic: POSITION_VAR => linkata da codice al momento della definizione
	//	dell'input layout
	float3 position: POSITION_VAR;
};

struct vertexShaderOutput{
	// SV_POSITION => system-value semantic as this is required by the rasterizer stage
	float4 position : SV_POSITION; // REQUIRED SEMANTIC "SV_POSITION"
};

vertexShaderOutput mainVertexShader(appDataIn actualVertex)
{
	vertexShaderOutput actualOutput;

	actualOutput.position = float4(actualVertex.position.xyz, 1.0f); // float4 perche' sono coord omogenee
	
	return actualOutput;
}