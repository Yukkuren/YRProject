#include "LoadShader.hlsli"
#include "../ShaderToy.hlsli"

void mainImage(out vec4 fragColor, in vec2 fragCoord)
{
    vec2 uv = (fragCoord * 2.0 - iResolution.xy) / iResolution.y;
    uv += (distance(uv, (0.))) * 0.25 * (sin(iTime * 1.73), cos(iTime * 2.67));


    vec3 color_offset = (0.2, 0.1, 0.6);
    vec3 circle_col = (1.0 + 0.5 * sin(iTime + color_offset * 3.14)) * exp(distance(uv, (0.)));

    uv /= exp(fract(iTime * 2.0));

    float dist = distance(uv, (0.0));
    dist = fract(log(dist));

    float out_circle = smoothstep(1.0, 0.9, dist);
    float in_circle = smoothstep(0.95, 0.89, dist);
    vec3 circle = circle_col * (out_circle - in_circle);

    //Ç±ÇÃêîéöÇ≈ê¸ÇÃêFÇ™ïœÇÌÇÈ
    //float4 color = float4(1.0, 0.2, 0.4, 1.0);
    float4 color = float4(red, green, blue, 1.0);
    fragColor = vec4(circle * (color.rgb), 1.0);
}

PSOutput main(PSInput pin) : SV_TARGET
{
    PSOutput Out = (PSOutput) 0;
    float4 fragColor = 0;
    float2 fragCoord = pin.position.xy;

    fragCoord.y = iResolution.y - fragCoord.y;

    mainImage(fragColor, fragCoord);

    Out.Color = fragColor;
    //Out.Luminance = float4(0.0f, 0.0f, 0.0f, 0.0f);

    return Out;
}