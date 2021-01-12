#include "Trajectory.hlsli"

PSOutput main(PSInput pin) : SV_Target
{
    PSOutput Out = (PSOutput) 0;
    float4 color = DiffuseTexture.Sample(DecalSampler, pin.tex) * material_color;
    Out.Color = color;
    Out.Luminance = float4(0.0, 0.0, 0.0, 0.0);
    return Out;
}