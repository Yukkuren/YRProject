#include "toGbuffer.hlsli"

PSInput main(VSInput input)
{
	PSInput Out = (PSInput)0;

	float4 P = float4(input.position, 1.0);
	float3 wPos = mul(P, world);
	Out.Position = mul(P, world_view_projection);
	Out.wPos = wPos;

	float3 wN = mul(input.normal, (float3x3)world);
	Out.wNormal = normalize(wN);
	
	Out.Tex = input.tex;
	Out.color = input.color;

	return Out;
}