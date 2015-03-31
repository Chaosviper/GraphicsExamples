/* 
We don’t need the position variable which was bound to the SV_Position system-value semantic
because that variable was only required by the rasterizer stage.
*/

cbuffer colorBuffer: register(b0){
	float4 color;
}

float4 mainPixelShader() : SV_TARGET
{
	return color;
}