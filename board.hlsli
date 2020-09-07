Texture2D DiffuseTexture : register( t0 );//�e�N�X�`��
SamplerState DecalSampler : register( s0 );//�T���v��

struct VSInput
{
	float3 position :POSITION;
	float4 normal : NORMAL;
	float2 tex : TEXCOORD;
	float4 color : COLOR;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
	float4 color : COLOR;
};

cbuffer CONSTANT_BUFFER : register(b0)
{
	row_major float4x4 world_view_projection;
	row_major float4x4 world;
	float4 material_color;
}

cbuffer CBPerFrame3 : register(b3)
{
	float Offset_X;		//�e�N�X�`��X��������
	float Offset_Y;		//�e�N�X�`��Y��������
	float dummy00;
	float dummy01;
}