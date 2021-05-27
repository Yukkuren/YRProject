#define MAX_BONES 128

//--------------------------------------------
//	�e�N�X�`��
//--------------------------------------------
Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);
Texture2D FurTexture : register(t1);
SamplerState FurSampler : register(s1);


//--------------------------------------------
//	�O���[�o���ϐ�
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
	float4 eyepos;				//�J�������W
	float4x4 view;
	float4x4 projection;
	float3 at;
	float lumi_factor;
	float offset_X;
	float offset_Y;
	float dummy00;
	float dummy01;
};
cbuffer CBPerFrame : register(b2)
{
	float4  LightColor;		//���C�g�̐F
	float4	LightDir;		//���C�g�̕���
	float4  AmbientColor;	//����
	float4	EyePos;			//�J�����ʒu
	float	Density;		//�т̖��x
	float	Distance;		//�т̒���
	float	dummy1;
	float	dummy2;
};
static const uint LAYER = 16;			//�t�@�[�̒���

//--------------------------------------------
//	�f�[�^�[�t�H�[�}�b�g
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
	float4 Position : POSITION;
	float3 Normal   : NORMAL;
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
	float3 Normal   : NORMAL;
};

struct PSOutput
{
	float4 Color : SV_TARGET0;
	//float4 wNormal : SV_TARGET1;
	//float4 wPosition : SV_TARGET2;
	float4 Luminance : SV_TARGET3;
};
