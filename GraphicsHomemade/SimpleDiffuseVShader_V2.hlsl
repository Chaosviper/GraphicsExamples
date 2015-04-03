struct InVar{
	float3 pos: POSITION_VAR;
	float3 norm: NORMAL_VAR;
};

cbuffer Transform: register(b0){
	float4x4 world;
	float4x4 view;
	float4x4 projection;
}

cbuffer Lights: register(b1){
	float4 lightPos;
}

struct OutVar{
	float4 forPixelPos: SV_POSITION;
	float4 forPixelLightVec: V_LIGHT;
	float4 forPixelNorm: V_NORM;
};

OutVar main(InVar inArg)
{
	OutVar toPixelShader;
	// COORD
	float4 posOmogenee = float4(inArg.pos, 1.0f); //point

	float4 worldCoordOmogenee = mul(world, posOmogenee);
	toPixelShader.forPixelPos = mul(view, worldCoordOmogenee);
	toPixelShader.forPixelPos = mul(projection, toPixelShader.forPixelPos);
	// NORMAL
	float4 normOmogenee = float4(inArg.norm, 0.0f); //vector
	float4 worldNormOmogenee = mul(world, normOmogenee);

	// Calculate light vector (from surface point to lightPoint)
	toPixelShader.forPixelLightVec = lightPos - worldCoordOmogenee; // <-- Will be interpolated in triangle traversal!

	toPixelShader.forPixelNorm = worldNormOmogenee; // <-- Will be interpolated in triangle traversal!

	return toPixelShader;
}