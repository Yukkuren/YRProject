#include "toGbuffer.hlsli"

PSOutput main(PSInput input)
{
	PSOutput Out = (PSOutput)0;

	float4 color = DiffuseTexture.Sample(DecalSampler, input.Tex)*input.color;
	float3 N = normalize(input.wNormal);	//ワールド法線
	float3 P = input.wPos;					//ワールド座標
	Out.Color = color;
	//Out.Color = float4(input.Tex.x, input.Tex.y, 1.0f, 1.0f);
	Out.wNormal = float4(N, 1.0f);
	Out.wPosition = float4(P, 1.0f);
	//Out.specular = float4(0, 0, 0, 1);

	return Out;
}