Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);


//���_�t�H�[�}�b�g
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
	float4 Position  : SV_POSITION;// ���_���W(�ˉe���W�n)
	float2 Tex  : TEXCOORD;          // �e�N�X�`�����W
	float4 Color : COLOR0;
};

//�萔�o�b�t�@
cbuffer CBPerFrame : register(b0)
{
	row_major float4x4 View;   // �r���[�ϊ��s��
	row_major float4x4 Projection;  // �����ϊ��s��
	row_major float4x4 SR;			//�X�P�[����]�s��
};