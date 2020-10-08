#include "furShader.hlsli"

//--------------------------------------------
// �g�U���ˊ֐��i�����o�[�g�V�F�[�f�B���O�j 
//--------------------------------------------
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// C:���ˌ�(�F�E����)
// K:���˗�(0�`1.0)
float3 Diffuse(float3 N, float3 L, float3 C, float3 K)
{
	float D = dot(N, L);
	D = max(0, D);		//���̒l��0�ɂ���
	return K * C * D;
}

//--------------------------------------------
// ���ʔ��ˊ֐�(�u�����E�t�H��)
//--------------------------------------------
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// C:���ˌ�(�F�E����)
// E:���_�����x�N�g��(���K���ς�)
// K:���˗�(0�`1.0)
// Power:�n�C���C�g�̋���(�P���x)
float3 BlinnPhongSpecular(float3 N, float3 L, float3 C, float3 E,
	float3 K, float3 power)
{
	//�n�[�t�x�N�g��
	float3 H = normalize(L + E);

	float3 S = dot(H, N);
	S = max(0, S);
	S = pow(S, power);
	S = S * K * C;
	return S;
}
//--------------------

PSOutput main(PSInput input) : SV_TARGET
{
	PSOutput Out = (PSOutput)0;

	float4 color = DiffuseTexture.Sample(DecalSampler, input.Tex);
	//float4 color = FurTexture.Sample(DecalSampler, input.Tex);
	float3 N = normalize(input.wNormal);
	float3 E = normalize(EyePos.xyz - input.wPos);
	float3 L = normalize(-light_direction.rgb);

	// ����
	float3 A = AmbientColor.rgb;

	// �g�U����
	float3 C = light_color.rgb;
	float3 Kd = float3(1.0f, 1.0f, 1.0f);
	float3 D = Diffuse(N, L, C, Kd);

	// ���ʔ���
	float3 Ks = float3(1.0f, 1.0f, 1.0f);
	//SP5
	float Shinness = 5.0f;
	//SP6
	float3 S = BlinnPhongSpecular(N, -L, C, E, Ks, Shinness);

	Out.Color = color * input.Color * float4(A + D + S, 1.0f);
	//Out.Color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	Out.Luminance = float4(1.0f, 1.0f, 1.0f, 1.0f);

	return Out;
}
