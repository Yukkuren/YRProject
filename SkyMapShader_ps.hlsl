Texture2D diffuse_map : register(t0);
SamplerState diffuse_map_sampler_state : register(s0);

#include"SkyMapShader.hlsli"

PSOutput main(VS_OUT pin) : SV_TARGET
{
	PSOutput Out = (PSOutput)0;
	float4 color = diffuse_map.Sample(diffuse_map_sampler_state,pin.texcoord);

	Out.Color = color;
	Out.Luminance = color * lumi_factor;

	return Out;
}