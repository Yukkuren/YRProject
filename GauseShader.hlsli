Texture2D ColorBuffer : register(t0);
SamplerState ColorSampler : register(s0);

struct VSOutput
{
	float4 Position : SV_POSITION;
	float2 Tex : TEXCOORD;
};

cbuffer CbBlur
{
	int SampleCount : packoffset(c0);
	float4 Offset[16] : packoffset(c1);
};