#include "Trajectory.hlsli"

PSOutput main(PSInput pin) : SV_Target
{
    PSOutput Out = (PSOutput) 0;
    float4 color = DiffuseTexture.Sample(DecalSampler, pin.tex) * material_color;
    //color = float4(1.0, 1.0, 1.0, 1.0);
    color.a *= pin.alpha;
    Out.Color = color;
    Out.Luminance = float4(0.0, 0.0, 0.0, 0.0);
    return Out;
}