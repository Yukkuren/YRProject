Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);


//頂点フォーマット
struct VSInput
{
	float3 Position : POSITION;
	float2 Size     : TEXCOORD0;
	float2 UV_pos   : UV_POS;
	float2 UV_size  : UV_SIZE;
	float4 Color    : COLOR;
};

struct GSInput
{
	float3 Position : POSITION;
	float2 Size     : TEXCOORD;
	float2 UV_pos   : UV_POS;
	float2 UV_size  : UV_SIZE;
	float4 Color    : COLOR;
};

struct PSInput
{
	float4 Position  : SV_POSITION;// 頂点座標(射影座標系)
	float2 Tex  : TEXCOORD;          // テクスチャ座標
	float4 Color : COLOR0;
};

//定数バッファ
cbuffer CBPerFrame : register(b0)
{
	row_major float4x4 View;   // ビュー変換行列
	row_major float4x4 Projection;  // 透視変換行列
	row_major float4x4 SR;			//スケール回転行列
};