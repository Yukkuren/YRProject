
//--------------------------------------------
//	�e�N�X�`��
//--------------------------------------------


Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);

//--------------------------------------------
//	�O���[�o���ϐ�
//--------------------------------------------

cbuffer CBPerMesh : register( b0 )
{
    matrix  World;
	matrix	matWVP;
};

//--------------------------------------------
//	�f�[�^�[�t�H�[�}�b�g
//--------------------------------------------

struct VSInput
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 Tex      : TEXCOORD;
	float4 Color    : COLOR;
};

struct PSInput
{
	float4 Position : SV_POSITION; 
	float2 Tex : TEXCOORD;
	float4 Color : COLOR;
};

//--------------------------------------------
//	���_�V�F�[�_�[
//--------------------------------------------

PSInput VSMain( VSInput input )
{
    PSInput output = (PSInput)0;

	float4 P = float4(input.Position, 1.0);
	// ���[���h��Ԃɕϊ�.
	float4 worldPos = mul(World, P);


	// �ˉe��Ԃɕϊ�.
	float4 projPos = mul(matWVP, P);

	// �o�͒l�ݒ�.
	output.Position = projPos;
	output.Color = input.Color;
	output.Color = float4(1,1,1,1);
	output.Tex = input.Tex;

    return output;
}

//--------------------------------------------
//	�s�N�Z���V�F�[�_�[
//--------------------------------------------

float4 PSMain( PSInput input ) : SV_TARGET0
{
	return DiffuseTexture.Sample(DecalSampler, input.Tex) * input.Color;
}
