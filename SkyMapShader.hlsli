#define MAX_BONES 128

//フォグのパラメータ
static const float3 fogColor = { 0.8,0.8,0.6 };
static const float fogNear = 20.0;
static const float fogFar = 50.0;

//半球ライトパラメータ
static const float3 skyColor = { 0.4,0.6,0.8 };
static const float3 groundColor = { 0.6,0.4,0.2 };

struct VS_OUT
{
	float4	position	: SV_POSITION;
	float4	normal		: NORMAL;
	float2	texcoord	: TEXCOORD;
};

struct VS_INPUT
{
	float4	position	: POSITION;
	float4	normal		: NORMAL;
	float2	texcoord	: TEXCOORD;
};

struct PSOutput
{
	float4 Color : SV_TARGET0;
	//float4 wNormal : SV_TARGET1;
	//float4 wPosition : SV_TARGET2;
	//float4 Luminance : SV_TARGET3;
};

cbuffer CONSTANT_BUFFER : register(b0)
{
	row_major float4x4 world_view_projection;
	row_major float4x4 world;
	float4 material_color;
	float4 light_direction;
	float4 light_color;
	float4 ambient_color;

	row_major float4x4 bone_transforms[MAX_BONES];
	float4 EyePos;				//カメラ座標
	row_major float4x4 view;		//ビュー行列
	row_major float4x4 projection;	//透視投影行列
	float3 at;						//カメラ座標からカメラの注視点へ向かう単位ベクトル
	float dummy;
};