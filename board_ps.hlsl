#include"board.hlsli"

float4 main(PSInput pin) : SV_Target
{
	float4 color = DiffuseTexture.Sample(DecalSampler,pin.tex) * pin.color;
	return color;
	//return material_color;
}