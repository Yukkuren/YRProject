#include"geometric_primitive.hlsli"


//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

VS_OUT main(float3 position : POSITION)
{
	VS_OUT vout;
	float4 POS = float4(position, 1.0);
	POS = mul(POS, world);

	float3 E = EyePos.xyz;

	float3 P = E + at * dot(at, POS - E);
	float h = length(POS - P) / length(P - E);
	//h *= dot(input[i].wPos - P, at) > 0 ? -1 : 1;
	row_major float4x4 Mat = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 1
	};
	row_major float4x4 newWorld = mul(world,Mat);
	float4 Posi = mul(float4(position,1.0), newWorld);
	Posi = mul(Posi, view);
	Posi = mul(Posi, projection);
	Posi /= Posi.w;
	
	vout.position = Posi;



	/*vout.color = material_color * max(0, dot(L, N));
	vout.color.a = material_color.a;*/
	return vout;
}