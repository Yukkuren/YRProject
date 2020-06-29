#include "board.hlsli"

VS_OUT main(float4 position : POSITION, float4 normal : NORMAL)
{
	VS_OUT vout;

	vout.position = mul(position, world_view_projection);
	vout.normal.w = 0;
	float4 N = normalize(mul(normal, world));
	vout.normal = N;

	return vout;
}