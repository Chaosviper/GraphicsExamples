struct InVarFromVertex{
	float4 pos: SV_POSITION;
	float attenuation : COLOR_ATT;
};

float4 main(InVarFromVertex inInfo) : SV_TARGET
{
	float4 finalColor = float4(1.0f*inInfo.attenuation, 1.0f*inInfo.attenuation, 1.0f*inInfo.attenuation, 1.0f*inInfo.attenuation);
	return finalColor;
}