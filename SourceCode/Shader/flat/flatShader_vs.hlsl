#include "flatShader.hlsli"

GSInput main(VSInput input)
{
	GSInput output = (GSInput)0;

	float3 ps = { 0,0,0 };
	float3 n = { 0,0,0 };

	int i2 = 0;
	for (i2 = 0; i2 < 4; i2++)
	{
		ps += (input.bone_weights[i2] * mul(float4(input.Position, 1.0f), bone_transforms[input.bone_indices[i2]])).xyz;
		n += (input.bone_weights[i2] * mul(float4(input.Normal.xyz, 0), bone_transforms[input.bone_indices[i2]])).xyz;
	}
	input.Position = float4(ps, 1.0f);
	input.Normal = float4(n, 0.0f);

	float4 P = float4(input.Position, 1.0);

	// ƒ[ƒ‹ƒh•ÏŠ·
	float4 worldPos = mul(P,world);
	output.Position = P;
	output.Color = float4(1, 1, 1, 1);
	output.Tex = input.Tex;
	output.wPos = worldPos.xyz;

	return output;
}
