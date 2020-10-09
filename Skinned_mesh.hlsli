#define MAX_BONES 128

//�t�H�O�̃p�����[�^
static const float3 fogColor = { 0.8,0.8,0.6 };
static const float fogNear = 20.0;
static const float fogFar = 50.0;

//�������C�g�p�����[�^
static const float3 skyColor = { 0.4,0.6,0.8 };
static const float3 groundColor = { 0.6,0.4,0.2 };

struct VS_OUT
{
	float4	position	: SV_POSITION;
	float4	normal		: NORMAL;
	float2	texcoord	: TEXCOORD;
	float4	color		: COLOR;
	float3	wNormal		: TEXCOORD1;	//���[���h�@��
	float3	wEyeDir		: TEXCOORD2;	//���[���h�@��
	float3	wPos		: TEXCOORD4;	//���[���h�ʒu
};

struct VS_INPUT
{
	float4	position	: POSITION;
	float4	normal		: NORMAL;
	float2	texcoord	: TEXCOORD;
	float4	bone_weights: WEIGHTS;
	uint4	bone_indices: BONES;
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
	float4 light_direction;
	float4 light_color;
	float4 ambient_color;
	
	row_major float4x4 bone_transforms[MAX_BONES];
	float4 EyePos;				//�J�������W
	row_major float4x4 view;		//�r���[�s��
	row_major float4x4 projection;	//�������e�s��
	float3 at;						//�J�������W����J�����̒����_�֌������P�ʃx�N�g��
	float lumi_factor;				//�P�x����(0.0f~1.0f)
	float Offset_X;					//�e�N�X�`��X��������
	float Offset_Y;					//�e�N�X�`��Y��������
	float dummy00;
	float dummy01;
};