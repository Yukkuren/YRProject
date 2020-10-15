struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float4 color    : COLOR;
};

struct PSOutput
{
	float4 Color : SV_TARGET0;
	//float4 wNormal : SV_TARGET1;
	//float4 wPosition : SV_TARGET2;
	float4 Luminance : SV_TARGET3;
};
