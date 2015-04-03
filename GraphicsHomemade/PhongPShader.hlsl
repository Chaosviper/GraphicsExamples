
struct InVarFromVertex{
	float4 pos: SV_POSITION;
	float4 lightVector: L_W;
	float4 normalVector : N_W;
	float4 viewVector: V_W;
};

float4 main(InVarFromVertex inInfo) : SV_TARGET
{
	inInfo.normalVector = normalize(inInfo.normalVector);
	inInfo.viewVector = normalize(inInfo.viewVector);
	inInfo.lightVector = normalize(inInfo.lightVector);

	// ** REFLECTIVE TERM
	// Calculate halfway vector
	float4 halfwayVector = inInfo.lightVector + inInfo.viewVector;
	halfwayVector = normalize(halfwayVector);

	// Calculate info for attenutation (Blinn-Phong)
	float cosHN = dot(halfwayVector, inInfo.normalVector);
	cosHN = max(0, cosHN);

	float smoothness = 1280.0f;
	float ks = 0.8f;

	cosHN = pow(cosHN, smoothness);
	float attenuationRef = cosHN*ks;

	// ** DIFFUSIVE TERM
	float cosLN = dot(inInfo.lightVector, inInfo.normalVector);
	cosLN = max(0, cosLN);

	float kd = 0.8;
	float attenuationDiff = kd*cosLN;

	// ** AMBIENTAL TERM
	float ka = 0.15;
	float attenuationAmb = ka; // TODO: aggiungere intensita' luminosa!

	// ** DIFF + REFL
	float attenuation = attenuationRef + attenuationDiff + attenuationAmb;

	float4 finalColor = float4(1.0f*attenuation, 1.0f*attenuation, 1.0f*attenuation, 1.0f);
		return finalColor;
}