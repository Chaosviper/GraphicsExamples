
struct DataIn{
	float3 position: POSITION_VAR;
	float4 color: COLOR_VAR;
};

struct DataOut{
	float4 position: SV_POSITION;
	float4 color: COLOR;
};

cbuffer worldMatrix: register(b0){
	float4x4 world;
}

cbuffer viewMatrix : register(b1){
	float4x4 view;
}

cbuffer projMatrix : register(b2){
	float4x4 proj;
}

DataOut main(DataIn data)
{
	DataOut forPixelShader;
	// Color:
	forPixelShader.color = data.color;
	// Position (in projection space)
	float4 var = mul(world, float4(data.position, 1.0f));
	var = mul(view, var);
	var = mul(proj, var);

	forPixelShader.position = var;

	return forPixelShader;
}