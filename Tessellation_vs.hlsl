#include "Tessellation.hlsli"

HSInput main(VSInput input)
{
    HSInput output = (HSInput) 0;

    float3 ps = { 0, 0, 0 };
    float3 n = { 0, 0, 0 };

    int i2 = 0;
    for (i2 = 0; i2 < 4; i2++)
    {
        ps += (input.bone_weights[i2] * mul(float4(input.Position, 1.0f), bone_transforms[input.bone_indices[i2]])).xyz;
        n += (input.bone_weights[i2] * mul(float4(input.Normal.xyz, 0), bone_transforms[input.bone_indices[i2]])).xyz;
    }
    input.Position = ps;
    input.Normal = n;
    //input.Position = mul(float4(ps, 1.0), world_view_projection).xyz;
    //input.Normal = normalize(mul(float4(n, 0.0), world)).xyz;
    output.Color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    output.Tex = input.Tex;

    //output.Tex.x = input.Tex.x + Offset_X;
    //output.Tex.y = input.Tex.y + Offset_Y;

    return output;
}