#include "ParallelToon.hlsli"

//-------------------------------------------- 
// �g�D�[���V�F�[�_�p�g�U���ˊ֐��i�Q�K���j 
//-------------------------------------------- 

float3 ToonDiffuse(float3 N, float3 L, float3 C, float3 K)
{
	float D = dot(N, L);
	if (D < 0.3) D = 0.2;
	else D = 0.6;

	return K * C * D;
}

//--------------------------------------------
// �g�D�[���V�F�[�_�p���ʔ��ˊ֐�(�Q�K��)
//-------------------------------------------- 

float3 ToonSpecular(
	float3 N, float3 L,
	float3 C, float3 E,
	float3 K, float Power)
{
	float3 R = reflect(L, N);
	R = normalize(R);
	float S = dot(R, E);
	S = max(0, S);
	S = pow(S, Power);
	if (S > 0.5) S = 0.8;
	else S = 0.4;
	return S * K * C;
}


float4 main(PSInput input) : SV_TARGET
{
	float4 color = DiffuseTexture.Sample(DecalSampler, input.Tex);
	float3 N = normalize(input.wNormal);
	float3 E = normalize(EyePos.xyz - input.wPos);
	float3 L = normalize(-light_direction.rgb);
	// ����
	float3 A = ambient_color.rgb;

	// �g�U����
	float3 C = light_color.rgb;
	float3 Kd = float3(1.0f, 1.0f, 1.0f);
	float3 D = ToonDiffuse(N, L, C, Kd);

	// ���ʔ���
	float3 Ks = float3(1, 1, 1);
	float3 S = ToonSpecular(N, L, C, E, Ks, 5);

	//�A�E�g���C��
	 if (input.Color.a < 0)
	 {
		 return float4(input.Color.rgb, 1);
	 }

	color.rgb *= input.Color.rgb * float3(A + D * material_color.rgb + S);
	return color;
}