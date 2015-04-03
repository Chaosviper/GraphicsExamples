struct InVar{
	float3 pos: POSITION_VAR; // Object coord
	float3 norm: NORMAL_VAR; // Object coord
};

cbuffer Transform: register(b0){
	float4x4 world; // World coord of the obj
	float4x4 view;
	float4x4 projection;
}

cbuffer Lights: register(b1){
	float4 lightPos; // World coord
}

cbuffer CameraPos : register(b2){
	float4 cameraPos; // World coord
}

struct OutVar{
	float4 forPixelPos: SV_POSITION;
	float4 lightW: L_W;
	float4 normalW: N_W;
	float4 viewW: V_W;
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
	worldNormOmogenee = normalize(worldNormOmogenee);

	// Calculate light vector (from surface point to lightPoint)
	float4 lightVec = lightPos - worldCoordOmogenee;
	lightVec = normalize(lightVec);
	// Calculate view vector (from surface point to eye point)
	float4 viewVec = cameraPos - worldCoordOmogenee;
	viewVec = normalize(viewVec);

	toPixelShader.normalW = worldNormOmogenee;
	toPixelShader.viewW = viewVec;
	toPixelShader.lightW = lightVec;


	return toPixelShader;
}