#define MAX_BONES 128

//--------------------------------------------
//	テクスチャ
//--------------------------------------------
Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);

//--------------------------------------------
//	グローバル変数
//--------------------------------------------
cbuffer CBPerMesh : register(b0)
{
	row_major float4x4 world_view_projection;
	row_major float4x4 world;
	float4 material_color;
	float4 light_direction;
	float4 light_color;
	float4 ambient_color;

	row_major float4x4 bone_transforms[MAX_BONES];
	float4 eyePos;				//カメラ座標
    row_major float4x4 view;
    row_major float4x4 projection;
    float3 at;
    float lumi_factor;
    float Offset_X;
    float Offset_Y;
    float dummy00;
    float dummy01;
    float4 dummy02;
};
cbuffer CBPerFrame : register(b2)
{
	float4  LightColor;		//ライトの色
	float4	LightDir;		//ライトの方向
	float4  AmbientColor;	//環境光
	float4	EyePos;			//カメラ位置
};
//--------------------------------------------
//	データーフォーマット
//--------------------------------------------
struct VSInput
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 Tex      : TEXCOORD;
	//float4 Color    : COLOR;
	float4	bone_weights: WEIGHTS;
	uint4	bone_indices: BONES;
};

struct GSInput
{
	float4 Position : SV_POSITION;
	float2 Tex : TEXCOORD;
	float4 Color : COLOR;
	float3 wPos: TEXCOORD2;
};

struct PSInput
{
	float4 Position : SV_POSITION;
	float2 Tex : TEXCOORD;
	float4 Color : COLOR;
	float3 wNormal: TEXCOORD1;
	float3 wPos : TEXCOORD2;
};

struct PSOutput
{
	float4 Color : SV_TARGET0;
	//float4 wNormal : SV_TARGET1;
	//float4 wPosition : SV_TARGET2;
	//float4 Luminance : SV_TARGET3;
};
