//-------------------------------------------------------------------------------------------------
// Textures and Samplers.
//-------------------------------------------------------------------------------------------------
Texture2D       ColorBuffer0  : register(t0);
Texture2D       ColorBuffer1  : register(t1);
Texture2D       ColorBuffer2  : register(t2);
Texture2D       ColorBuffer3  : register(t3);
Texture2D       ColorBuffer4  : register(t4);
Texture2D       ColorBuffer5  : register(t5);
Texture2D       ColorBuffer6  : register(t6);
SamplerState    ColorSampler : register(s0);

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
	float2 TexCoord : TEXCOORD;
};