#include "Trajectory.hlsli"

float4 MatPos(float4 POS)
{
    float3 E = EyePos.xyz;
	//float4 POS = input[0].Position + float4(input[0].Normal, 0.0) * LW;
    float4 wPOS = mul(POS, world);
    float3 P = E + at * dot(at, wPOS.xyz - E);
    float len = length(P - E);
    float h = length(wPOS.xyz - P) / len;
    h = max(0.05, h);
	//float h = (abs(len) < 0.001) ? 1.0 : length(wPOS - P) / len;
	//h *= dot(wPOS - P, at) > 0 ? -1 : 1;
    row_major float4x4 Mat =
    {
        1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, h, 0,
		0, 0, 0, 1
    };
    row_major float4x4 newWorld = mul(world, Mat);
    float4 Posi = mul(POS, newWorld);
    Posi = mul(Posi, view);
    Posi = mul(Posi, projection);
    Posi /= Posi.w;

    return Posi;
}


PSInput main(VSInput input)
{
    PSInput output = (PSInput) 0;

    //頂点座標とUV座標を送っているだけ(頂点のみワールド変換)
    float4 P = float4(input.position, 1.0);
    //output.position = mul(P, world_view_projection);
    output.position = MatPos(P);
    output.tex = input.tex;
    output.alpha = input.alpha;
    return output;
}