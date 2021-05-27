Texture2D ColorBuffer : register(t0);
SamplerState ColorSampler : register(s0);

struct VSInput
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 Tex      : TEXCOORD;
	float4 Color    : COLOR;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float2 Tex : TEXCOORD;
};

cbuffer CbBlur : register(b0)
{
	int SampleCount : packoffset(c0);
	float4 Offset[16] : packoffset(c1);
};