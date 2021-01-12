#include "Trajectory.hlsli"

PSInput main(VSInput input)
{
    PSInput output = (PSInput) 0;

    //���_���W��UV���W�𑗂��Ă��邾��(���_�̂݃��[���h�ϊ�)
    float4 P = float4(input.position, 1.0);
    output.position = mul(P, world_view_projection);
    output.tex = input.tex;

    return output;
}