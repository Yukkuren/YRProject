#define MAX_BONES 32

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
	float4	color		: COLOR;
	float3	wNormal		: TEXCOORD1;	//ワールド法線
	float3	wEyeDir		: TEXCOORD2;	//ワールド法線
	float3	wPos		: TEXCOORD4;	//ワールド位置
};

struct VS_INPUT
{
	float4	position	: POSITION;
	float4	normal		: NORMAL;
	float2	texcoord	: TEXCOORD;
	float4	bone_weights: WEIGHTS;
	uint4	bone_indices: BONES;
};

cbuffer CONSTANT_BUFFER : register(b0)
{
	row_major float4x4 world_view_projection;
	row_major float4x4 world;
	float4 material_color;
	float4 light_direction;
	float4 light_color;
	float4 ambient_color;
	
	//UNIT.21
	row_major float4x4 bone_transforms[MAX_BONES];
	float4 EyePos;				//カメラ座標
};