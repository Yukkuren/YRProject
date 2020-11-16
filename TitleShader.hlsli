
static const float4 sph1 = float4(0.0, 0.0, 0.0, 1.0);

Texture2D	iChannel0 : register(t0);
Texture2D	iChannel1 : register(t1);
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
};