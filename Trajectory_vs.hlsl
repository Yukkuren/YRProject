#include "Trajectory.hlsli"

PSInput main(VSInput input)
{
    PSInput output = (PSInput) 0;

    //頂点座標とUV座標を送っているだけ(頂点のみワールド変換)
    float4 P = float4(input.position, 1.0);
    output.position = mul(P, world_view_projection);
    output.tex = input.tex;

    return output;
}