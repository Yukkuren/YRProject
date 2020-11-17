#include "TitleShader.hlsli"
//#define TYPE 4
//
//#if TYPE == 1
//#define brightness 1.
//#define ray_brightness 11.
//#define gamma 5.
//#define spot_brightness 4.
//#define ray_density 1.5
//#define curvature .1
//#define red   7.
//#define green 1.3
//#define blue  1.
////1 -> ridged, 2 -> sinfbm, 3 -> pure fbm
//#define noisetype 2
//#define sin_freq 50. //for type 2
//#elif TYPE == 2
//#define brightness 1.5
//#define ray_brightness 10.
//#define gamma 8.
//#define spot_brightness 15.
//#define ray_density 3.5
//#define curvature 15.
//#define red   4.
//#define green 1.
//#define blue  .1
//#define noisetype 1
//#define sin_freq 13.
//#elif TYPE == 3
//#define brightness 1.5
//#define ray_brightness 20.
//#define gamma 4.
//#define spot_brightness .95
//#define ray_density 3.14
//#define curvature 17.
//#define red   2.9
//#define green .7
//#define blue  3.5
//#define noisetype 2
//#define sin_freq 15.

//#elif TYPE == 4
//#define brightness 3.
//#define ray_brightness 5.
//#define gamma 6.
//#define spot_brightness 1.5
//#define ray_density 6.
//#define curvature 90.
//#define red   5.8
//#define green 1.
//#define blue  4.5
//#define noisetype 1
//#define sin_freq 6.
//#define YO_DAWG

//#elif TYPE == 5
//#define brightness 2.
//#define ray_brightness 5.
//#define gamma 5.
//#define spot_brightness 1.7
//#define ray_density 30.
//#define curvature 1.
//#define red   1.
//#define green 4.0
//#define blue  4.9
//#define noisetype 2
//#define sin_freq 5. //for type 2
//#endif



float noise(float2 x)
{
	//return texture(iChannel0, x * .01).x;
	return iChannel0.Sample(DecalSampler, (x * 0.01)).x;
}

float fbm(float2 p)
{
	float z = 2.0;
	float rz = 0.0;
	p *= 0.25;
	for (float i = 1.; i < 6.0; i++)
	{
		rz += abs((noise(p) - 0.5) * 2.0) / z;
		//rz += (sin(noise(p) * sin_freq) * 0.5 + 0.5) / z;
		//rz += noise(p) / z;
		z = z * 2.0;
		p *= 2.0;
		p = mul(p, m2);
	}
	return rz;
}

PSOutput main(PSInput input) : SV_TARGET0
{
	PSOutput Out = (PSOutput)0;
	//float4 color = (float4)0;
	//color = iChannel0.Sample(DecalSampler, input.fragCoord);
	////Out.Color = float4(input.fragCoord.xy,1.0, 1.0);
	//Out.Color = color;
	//Out.Luminance = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//return Out;

	float t = -iTime * 0.03;
	float2 frag = float2(input.fragCoord.x * iResolution.x, input.fragCoord.y * iResolution.y);
	float2 uv = frag.xy / iResolution.xy - 0.5;
	uv.x *= iResolution.x / iResolution.y;
	uv *= curvature * 0.05 + 0.0001;

	float r = sqrt(dot(uv, uv));
	float x = dot(normalize(uv), float2(0.5, 0.0)) + t;
	float y = dot(normalize(uv), float2(0.0, 0.5)) + t;

	x = fbm(float2(y * ray_density * 0.5, r + x * ray_density * 0.2));
	y = fbm(float2(r + y * ray_density * 0.1, x * ray_density * 0.5));

	float val;
	val = fbm(float2(r + y * ray_density, r + x * ray_density - y));
	val = smoothstep(gamma * 0.02 - 0.1, ray_brightness + (gamma * 0.02 - 0.1) + 0.001, val);
	val = sqrt(val);

	float3 col = val / float3(red, green, blue);
	col = clamp(1.0 - col, 0.0, 1.0);
	col = lerp(col, float3(1.0,1.0,1.0), spot_brightness - r / 0.1 / curvature * 200.0 / brightness);
	col = clamp(col, 0.0, 1.0);
	col = pow(col, float3(1.7,1.7,1.7));

	Out.Color = float4(col, 1.0);
	Out.Luminance = float4(0.0f, 0.0f, 0.0f, 0.0f);

	return Out;
}