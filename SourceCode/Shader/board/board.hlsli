Texture2D DiffuseTexture : register( t0 );//テクスチャ
SamplerState DecalSampler : register( s0 );//サンプラ

struct VSInput
{
	float3 position :POSITION;
	float4 normal : NORMAL;
	float2 tex : TEXCOORD;
	float4 color : COLOR;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
	float4 color : COLOR;
};

cbuffer CONSTANT_BUFFER : register(b0)
{
	row_major float4x4 world_view_projection;
	row_major float4x4 world;
	float4 material_color;
}

cbuffer CBPerFrame3 : register(b3)
{
	float Offset_X;		//テクスチャX方向ずれ
	float Offset_Y;		//テクスチャY方向ずれ
	float dummy00;
	float dummy01;
}