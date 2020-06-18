//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}
#include"Skinned_mesh.hlsli"


//float4 main( float4 pos : POSITION ) : SV_POSITION
//{
//	return pos;
//}

VS_OUT main(float4 position : POSITION, float4 normal : NORMAL,float2 texcoord:TEXCOORD,
			float4 bone_weights : WEIGHTS, uint4 bone_indices : BONES
	)
{
	VS_OUT vout;


	//UNIT.21
	float3 p = { 0,0,0 };
	float3 n = { 0,0,0 };

	int i2 = 0;
	for (i2 = 0; i2 < 4; i2++)
	{
		p += (bone_weights[i2] * mul(position, bone_transforms[bone_indices[i2]])).xyz;
		n += (bone_weights[i2] * mul(float4(normal.xyz, 0), bone_transforms[bone_indices[i2]])).xyz;
	}
	position = float4(p, 1.0f);
	normal = float4(n, 0.0f);
	//vout.normal = material_color * max(0, dot(L, N));
	//vout.normal = N*L;
	//vout.color.a = material_color.a;
	//vout.color = material_color;


	vout.position = mul(position, world_view_projection);
	vout.normal.w = 0;
	float4 N = normalize(mul(normal, world));
	//float4 L = normalize(-light_direction);
	vout.normal = N;

	//float D = max(0, dot(N, L));

	//float4 influence = { 0.0f, 0.0f, 0.0f, 1.0f };
	//for (int i = 0; i < 4; i++)
	//{
	//	float weight = bone_weights[i];
	//	if (weight > 0.0f) 
	//	{
	//		switch (bone_indices[i]) 
	//		{
	//		case 0: influence.r = weight; break;
	//		case 1: influence.g = weight; break;
	//		case 2: influence.b = weight; break;
	//		}
	//	} 
	//}  
	//vout.color = influence;
	//vout.normal = influence;
	//material_color = influence;

	vout.texcoord = texcoord;
	vout.color = material_color;
	//vout.color.rgb *= D + ambient_color.rgb;

	float3 wP = mul(position, world).xyz;
	//SP6
	float3 E = normalize(EyePos.xyz - wP);
	vout.wEyeDir = E;

	vout.wNormal = N.xyz;
	vout.wPos = wP;

	return vout;
}