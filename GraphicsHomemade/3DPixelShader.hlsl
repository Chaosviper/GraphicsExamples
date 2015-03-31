
struct InData{
	float4 pos: SV_POSITION;
	float4 col: COLOR;
};

float4 main(InData inDataFromVertex) : SV_TARGET
{
	return inDataFromVertex.col;
}