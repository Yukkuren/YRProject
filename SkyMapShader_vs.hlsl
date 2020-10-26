//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}
#include"SkyMapShader.hlsli"


//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

VS_OUT main(VS_INPUT input)
{
	VS_OUT vout = (VS_OUT)(0);

	vout.position = mul(input.position, world_view_projection);
	vout.normal.w = 0;
	float4 N = normalize(mul(input.normal, world));
	vout.normal = N;

	vout.texcoord.x = input.texcoord.x + Offset_X;
	vout.texcoord.y = input.texcoord.y + Offset_Y;

	return vout;
}