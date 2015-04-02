struct InVar{
	float3 pos: POSITION_VAR;
	float3 norm: NORMAL_VAR;
};

cbuffer Transform: register(b0){
	float4x4 world;
	float4x4 view;
	float4x4 projection;
}

float4 main( InVar inArg ) : SV_POSITION
{
	float4 toReturn = float4(inArg.pos, 1.0f);
	toReturn = mul(world, toReturn);
	toReturn = mul(view, toReturn);
	toReturn = mul(projection, toReturn);

	return toReturn;
}