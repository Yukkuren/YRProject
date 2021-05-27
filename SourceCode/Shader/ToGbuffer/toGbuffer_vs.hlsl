#include "toGbuffer.hlsli"

PSInput main(VSInput input)
{
	PSInput Out = (PSInput)0;
	float4 P = float4(input.position, 1.0);
	
	Out.Position = mul(P, world_view_projection);
	
	float3 wPos = mul(P, world).xyz;
	
	float4 wN = normalize(mul(input.normal, world));
	Out.wNormal = wN.xyz;

	Out.wPos = wPos;
	Out.Tex = input.tex;
	Out.color = input.color;

	//Offset値によってテクスチャ座標に変化を与える
	Out.Tex.x = input.tex.x + Offset_X;
	Out.Tex.y = input.tex.y + Offset_Y;

	return Out;
}