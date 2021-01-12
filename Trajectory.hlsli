Texture2D DiffuseTexture : register(t0); //テクスチャ
SamplerState DecalSampler : register(s0); //サンプラ


struct VSInput
{
    float3 position : POSITION;
    float2 tex : TEXCOORD;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
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
    row_major float4x4 world_view_projection;
    row_major float4x4 world;
    float4 material_color;
    float4 EyePos; //カメラ座標
    row_major float4x4 view; //ビュー行列
    row_major float4x4 projection; //透視投影行列
    float3 at; //カメラ座標からカメラの注視点へ向かう単位ベクトル
    float dummy;
}