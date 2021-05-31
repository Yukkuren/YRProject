Texture2D DiffuseTexture : register(t0); //�e�N�X�`��
SamplerState DecalSampler : register(s0); //�T���v��


struct VSInput
{
    float3 position : POSITION;
    float2 tex : TEXCOORD;
    float alpha : ALPHA;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
    float alpha : ALPHA;
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
    float4 EyePos; //�J�������W
    row_major float4x4 view; //�r���[�s��
    row_major float4x4 projection; //�������e�s��
    float3 at; //�J�������W����J�����̒����_�֌������P�ʃx�N�g��
    float dummy;
    float4 dummy2;
}