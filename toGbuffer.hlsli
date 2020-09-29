Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);

struct VSInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
	float4 color : COLOR;
};

struct PSInput
{
	float4 Position : SV_POSITION;
	float4 color : COLOR0;
	float3 wNormal : TEXCOORD1;
	float3 wPos : TEXCOORD2;
	float2 Tex : TEXCOORD3;
	
};

struct PSOutput
{
	float4 Color : SV_TARGET0;
	float4 wNormal : SV_TARGET1;
	float4 wPosition : SV_TARGET2;
	//float4 specular : SV_TARGET3;
};


cbuffer CONSTANT_BUFFER : register(b0)
{
	row_major float4x4 world_view_projection;
	row_major float4x4 world;
	float4	material_color;
};

cbuffer CBPerFrame2 : register(b2)
{
	float4	LightColor;		//ライトの色
	float4	LightDir;		//ライトの方向
	float4	AmbientColor;	//環境光
	float4  EyePos;			//カメラ座標
};

cbuffer CBPerFrame3 : register(b3)
{
	float Offset_X;		//テクスチャX方向ずれ
	float Offset_Y;		//テクスチャY方向ずれ
	float dummy00;
	float dummy01;
}
