Texture2D	iChannel0 : register(t0);
//Texture2D	iChannel1 : register(t1);
SamplerState DecalSampler : register(s0);

struct VSInput
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 Tex      : TEXCOORD;
	float4 Color    : COLOR;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float2 fragCoord : TEXCOORD;
};

struct PSOutput
{
	float4 Color : SV_TARGET0;
	//float4 wNormal : SV_TARGET1;
	//float4 wPosition : SV_TARGET2;
	float4 Luminance : SV_TARGET3;
};

cbuffer CONSTANT_BUFFER : register(b0)
{
	float3		iResolution;
	float		iTime;
	float		brightness;
	float		ray_brightness;
	float		gamma;
	float		spot_brightness;
	float		ray_density;
	float		curvature;
	float		red;
	float		green;
	float		blue;
	float4		material_color;
	float		dummy1;
	float		dummy2;
	float		dummy3;
};