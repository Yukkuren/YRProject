#include "Tessellation.hlsli"


//--------------------------------------------
//	�n�[�t�����o�[�g
//--------------------------------------------
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// C:���ˌ�(�F�E����)
// K:���˗�

float3 HalfLambert(float3 N, float3 L, float3 C, float3 K)
{
    float D = dot(N, -L);
    D = D * 0.5 + 0.5;
	//D = D * D;	// ���������Ɍ�������H
    return C * D * K;
}


//--------------------------------------------
//	���ʔ��ˊ֐��i�t�H���j
//--------------------------------------------
// N:�@��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// C:���ˌ�(�F�E����)
// E:���_�����x�N�g��(���K���ς�)
// K:���˗�(0�`1.0)
// Power:�n�C���C�g�̋���(�P���x)

//�t�H���V�F�[�f�B���O
float3 PhongSpecular(float3 N, float3 L, float3 C, float3 E,
	float3 K, float Power)
{
    float3 R = reflect(L, N);
    R = normalize(R);
    float3 S = dot(R, E);
    S = max(0, S);
    S = pow(S, Power);
    S = S * K * C;
    return S;
}


//--------------------------------------------
// �������C�g
//--------------------------------------------
// N:�@��
// E:���_�����x�N�g��(���K���ς�)
// L:���˃x�N�g��(���K���ς�)
// LightColor :���C�g�F
float3 RimLight(
	float3 N, float3 E, float3 L, float3 LightColor)
{
    float rim = 1.0 - clamp(dot(N, E), 0.0, 1.0);
	//	float ratio = ratio = max(dot(L, E), 0);
    float ratio = dot(N, -L);
    ratio = ratio * 0.5 + 0.5;


    return LightColor * rim * ratio * 10.0;
}



//PSOutput main(PSInput input) : SV_Target
//{
//    PSOutput Out = (PSOutput) 0;

//    //Out.Luminance = float4(1.0, 1.0, 1.0, 1.0);
//    //return Out;
//    float4 color = DiffuseTexture.Sample(DecalSampler, input.Tex);
//    float3 E = normalize(EyePos.xyz - input.wPosition);
//    float3 L = normalize(LightDir.xyz);
//    float3 C = LightColor.rgb;
//	//����
//    float3 A = AmbientColor.rgb;

//	// �@���擾
//    float3 N = NormalTexture.Sample(DecalSampler, input.Tex).xyz;
//    N = N * 2.0f - 1.0f;

//	// �ڋ�Ԏ�
//    float3 vx = normalize(input.vT);
//    float3 vy = normalize(input.vB);
//    float3 vz = normalize(input.vN);

//	// �@�����[���h�ϊ�
//    float3x3 mat = { vx, vy, vz };

//	// ���[���h�ϊ�
//    N = normalize(mul(N, mat)).xyz;

//	// �g�U����
//    float3 Kd = { 1, 1, 1 };
//    float3 D = HalfLambert(N, L, C, Kd);

//	// ���ʔ���
//    float3 Ks = { 1, 1, 1 };
//    float3 S = PhongSpecular(N, L, C, E, Ks, 50);
//	// �������C�g
//    float3 R = RimLight(N, E, L, C);

//    color *= input.Color;
//    color.rgb *= float4(A + D + S + R, 1);

//    Out.Color = color;
//    Out.Color = float4(1.0, 1.0, 1.0, 1.0);
//    Out.Luminance = float4(0.0, 0.0, 0.0, 0.0);
//    return Out;
//}

float4 main(PSInput input) : SV_Target
{
    float4 color = DiffuseTexture.Sample(DecalSampler, input.Tex);
    float3 E = normalize(EyePos.xyz - input.wPosition);
    float3 L = normalize(LightDir.xyz);
    float3 C = LightColor.rgb;
	//����
    float3 A = AmbientColor.rgb;

	// �@���擾
    float3 N = NormalTexture.Sample(DecalSampler, input.Tex).xyz;
    N = N * 2.0f - 1.0f;

	// �ڋ�Ԏ�
    float3 vx = normalize(input.vT);
    float3 vy = normalize(input.vB);
    float3 vz = normalize(input.vN);

	// �@�����[���h�ϊ�
    float3x3 mat = { vx, vy, vz };

	// ���[���h�ϊ�
    N = normalize(mul(N, mat)).xyz;

	// �g�U����
    float3 Kd = { 1, 1, 1 };
    float3 D = HalfLambert(N, L, C, Kd);

	// ���ʔ���
    float3 Ks = { 1, 1, 1 };
    float3 S = PhongSpecular(N, L, C, E, Ks, 50);
	// �������C�g
    float3 R = RimLight(N, E, L, C);

    color *= input.Color;
    color.rgb *= float4(A + D + S + R, 1);

    return color;
}