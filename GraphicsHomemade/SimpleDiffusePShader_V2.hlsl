struct InVarFromVertex{
	float4 pos: SV_POSITION;
	float4 worldLightVec: V_LIGHT;
	float4 worldNormVec: V_NORM;
};

float4 main(InVarFromVertex inInfo) : SV_TARGET
{
	// Calculate light vector (from surface point to lightPoint)
	inInfo.worldLightVec = normalize(inInfo.worldLightVec);
	inInfo.worldNormVec = normalize(inInfo.worldNormVec);

	float cosAlpha = dot(inInfo.worldLightVec, inInfo.worldNormVec);
	cosAlpha = max(0, cosAlpha);
	float kd = 0.8f;

	float attenuation = cosAlpha*kd;

	float4 finalColor = float4(1.0f*attenuation, 1.0f*attenuation, 1.0f*attenuation, 1.0f*attenuation);
	return finalColor;
}