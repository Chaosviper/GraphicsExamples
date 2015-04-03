
struct InVarFromVertex{
	float4 pos: SV_POSITION;
	float4 lightVector: L_W;
	float4 normalVector : N_W;
	float4 halfwayVector: HW_W;
};

float4 main(InVarFromVertex inInfo) : SV_TARGET
{
	inInfo.normalVector = normalize(inInfo.normalVector);
	inInfo.halfwayVector = normalize(inInfo.halfwayVector);
	// Calculate info for attenutation (Blinn-Phong)
	float cosHN = dot(inInfo.halfwayVector, inInfo.normalVector);
	cosHN = max(0, cosHN);

	float smoothness = 1.0f;
	float ks = 0.8f;

	cosHN = pow(cosHN, smoothness);
	float attenuation = cosHN*ks;

	inInfo.lightVector = normalize(inInfo.lightVector);
	float cosLN = dot(inInfo.lightVector, inInfo.normalVector);
	cosLN = max(0, cosLN);
	attenuation = attenuation*cosLN;

	float4 finalColor = float4(1.0f*attenuation, 1.0f*attenuation, 1.0f*attenuation, 1.0f*attenuation);
	return finalColor;
}