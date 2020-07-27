#include "ToonShader.hlsli"

GSInput main(VSInput input) 
{
	GSInput output = (GSInput)0;

	float3 ps = { 0,0,0 };
	float3 n = { 0,0,0 };

	int i2 = 0;
	for (i2 = 0; i2 < 4; i2++)
	{
		ps += (input.bone_weights[i2] * mul(input.Position, bone_transforms[input.bone_indices[i2]])).xyz;
		n += (input.bone_weights[i2] * mul(float4(input.Normal.xyz, 0), bone_transforms[input.bone_indices[i2]])).xyz;
	}
	input.Position = float4(ps, 1.0f);
	input.Normal = float4(n, 0.0f);


	float4 P = float4(input.Position, 1.0);
	// ワールド法線算出
	float4 N = normalize(mul(input.Normal, world));
	// ワールド変換座標
	float3 worldPos = mul(input.Position, world).xyz;

	output.Position = P;
	output.Color = float4(1, 1, 1, 1);
	output.Normal = input.Normal.xyz;
	output.Tex = input.Tex;
	output.wNormal = N.xyz;
	output.wPos = worldPos;

	return output;
}