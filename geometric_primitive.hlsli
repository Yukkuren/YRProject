struct VS_OUT
{
	float4 position : SV_POSITION;
};

cbuffer CONSTANT_BUFFER : register(b0)
{
	row_major float4x4 world_view_projection;
	row_major float4x4 world;
	float4 material_color;
	float4 EyePos;					//�J�������W
	row_major float4x4 view;		//�r���[�s��
	row_major float4x4 projection;	//�������e�s��
	float3 at;						//�J�������W����J�����̒����_�֌������P�ʃx�N�g��
	float dummy;
}