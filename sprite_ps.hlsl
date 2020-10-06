Texture2D diffuse_map : register(t0);
SamplerState diffuse_map_sampler_state : register( s0 );
#include"sprite.hlsli"

PSOutput main(VS_OUT pin) : SV_TARGET
{
	PSOutput Out = (PSOutput)0;

	Out.Color = diffuse_map.Sample(diffuse_map_sampler_state, pin.texcoord)
	* pin.color;

	Out.wPosition = pin.pos;

	Out.wNormal = float4(0.0, 0.0, 1.0, 1.0);

	return Out;
}