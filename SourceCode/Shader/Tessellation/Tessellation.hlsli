#define MAX_BONES 128

Texture2D DiffuseTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D HeightTexture : register(t2);

SamplerState DecalSampler : register(s0);
SamplerState WrapSampler : register(s1);


cbuffer CBPerFrame2 : register(b2)
{
    float4 LightColor; //ライトの色
    float4 LightDir; //ライトの方向
    float4 AmbientColor; //環境光
    float4 eyePos; //カメラ座標
    float wave_timer; //波の移動
    float divide; //分割数
    float dummy1;
    float dummy2;
};


struct VSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD;
    float4 bone_weights : WEIGHTS;
    uint4 bone_indices : BONES;
};

//struct VSInput
//{
//    float3 Position : POSITION;
//    float3 Normal : NORMAL;
//    float2 Tex : TEXCOORD;
//    float4 Color : COLOR; //頂点カラー無し
//};

struct HSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD;
    float4 Color : COLOR;
};
struct DSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD;
    float4 Color : COLOR;
};

// 分割情報(システム出力用)
struct HSConstantOutput
{
    float factor[3] : SV_TessFactor;
    float inner_factor : SV_InsideTessFactor;
};

struct PSInput
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR0;
    float3 wPosition : TEXCOORD0; //ワールド座標
    float2 Tex : TEXCOORD1;
    float3 vT : TEXCOORD2;
    float3 vB : TEXCOORD3;
    float3 vN : TEXCOORD4;
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
    //row_major float4x4 world_view_projection;
    //row_major float4x4 world;
    matrix World;
    matrix matWVP;
    float4 material_color;
    float4 light_direction;
    float4 light_color;
    float4 ambient_color;

    row_major float4x4 bone_transforms[MAX_BONES];
    float4 EyePos; //カメラ座標
    row_major float4x4 view; //ビュー行列
    row_major float4x4 projection; //透視投影行列
    float3 at; //カメラ座標からカメラの注視点へ向かう単位ベクトル
    float lumi_factor; //輝度調整(0.0f~1.0f)
    float Offset_X; //テクスチャX方向ずれ
    float Offset_Y; //テクスチャY方向ずれ
    float dummy00;
    float dummy01;
    float4 dummy02;
};