#include "../ShaderToy.hlsli"
#include "SilexarsShader.hlsli"

#define t iTime
#define r iResolution.xy

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec3 c;
    float l, z = t;
    for (int i = 0; i < 3; i++)
    {
        vec2 uv, p = fragCoord.xy / r;
        uv = p;
        p -= .5;
        p.x *= r.x / r.y;
        z += .07;
        l = length(p);
        uv += p / l * (sin(z) + 1.) * abs(sin(l * 9. - z * 2.));
        c[i] = .01 / length(abs(mod(uv, 1.) - .5));
    }
    fragColor = vec4(c / l, t);
}


PSOutput main(PSInput pin) : SV_TARGET
{
    PSOutput Out = (PSOutput) 0;
    float4 fragColor = 0;
    float2 fragCoord = pin.position.xy;

    fragCoord.y = iResolution.y - fragCoord.y;

    mainImage(fragColor, fragCoord);

    Out.Color = fragColor;
    Out.Luminance = float4(0.0f, 0.0f, 0.0f, 0.0f);

    return Out;
}