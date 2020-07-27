#include"geometric_primitive.hlsli"


//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

VS_OUT main(float3 position : POSITION)
{
	VS_OUT vout;
	float4 P = float4(position, 1.0);
	vout.position = mul(P, world_view_projection);

	/*vout.color = material_color * max(0, dot(L, N));
	vout.color.a = material_color.a;*/
	return vout;
}