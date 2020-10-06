struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

struct PSOutput
{
	float4 Color : SV_TARGET0;
	float4 wNormal : SV_TARGET1;
	float4 wPosition : SV_TARGET2;
};