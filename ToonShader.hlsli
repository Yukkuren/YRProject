#define MAX_BONES 128

static const float LW = 0.007f;			//ó÷äsê¸ÇÃëæÇ≥
static const float4 LC = { 0,0,0,-1 };	//ó÷äsê¸ÇÃêF

Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);

struct VSInput
{
	float3	Position : POSITION;
	float3	Normal : NORMAL;
	float2	Tex : TEXCOORD;
	float4	bone_weights: WEIGHTS;
	uint4	bone_indices: BONES;
};

struct PSInput
{
	float4 SV_Position : SV_POSITION;
	float4 Position : POSITION;
	float2 Tex : TEXCOORD;
	float4 Color : COLOR;
	float3 wNormal: TEXCOORD1;
	float3 wPos : TEXCOORD2;
};

struct GSInput
{
	float4 Position : POSITION;
	float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
	float4 Color : COLOR;
	float3 wNormal : TEXCOORD1;
	float3 wPos : TEXCOORD2;
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
	float4 EyePos;				//ÉJÉÅÉâç¿ïW
	/*int		inverse;
	int		dummy1;
	int		dummy2;
	int		dummy3;*/
};