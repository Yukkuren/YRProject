
#include "Font.hlsli"

Texture2D diffuseMap : register(t0);
SamplerState diffuseMapSamplerState : register(s0);

PSOutput main(VS_OUT pin) : SV_TARGET
{
	PSOutput Out = (PSOutput)0;
	Out.Color = diffuseMap.Sample(diffuseMapSamplerState, pin.texcoord) * pin.color;
	//Out.Luminance = float4(0.0f, 0.0f, 0.0f, 0.0f);
	return Out;
}

