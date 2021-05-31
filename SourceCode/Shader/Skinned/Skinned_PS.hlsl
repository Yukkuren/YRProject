Texture2D diffuse_map : register(t0);
SamplerState diffuse_map_sampler_state : register(s0);

#include"Skinned_mesh.hlsli"
//SP4
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
	return K*C*D;
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
	S = S*K*C;
	return S;
}
//--------------------


//SP5
//------------------------------
//		�t�H�O
//------------------------------
// color	:���݂̐F
// viewPos	:���_
// Pos		:�s�N�Z���̈ʒu
// fogColor:�t�H�O�̐F
// Near	:�t�H�O�̋ߋ���
// Far		:�t�H�O�̉�����
float3 Fog(float3 color, float3 viewPos, float3 Pos, float3 fogColor, float Near, float Far)
{
	//�t�H�O�v�Z
	float Len = length(viewPos - Pos);
	float fogalpha = saturate((Len - Near) / (Far - Near));
	return color.rgb*(1.0 - fogalpha) + fogColor*fogalpha;
}


//------------------------------
//		�������C�g
//------------------------------
// N			:�@��
// SkyColor		:��(��)�F
// GroundColor	:�n��(��)�F
float3 HemiSphereLight(float3 N, float3 SkyColor, float3 GroundColor)
{
	float skyblend = (N.y + 1.0)*0.5;
	float groundblend = 1.0 - skyblend;
	return SkyColor*skyblend + GroundColor*groundblend;
}



PSOutput main(VS_OUT pin) : SV_TARGET
{
	PSOutput Out = (PSOutput)0;

	//SP4
	float4 color = diffuse_map.Sample(diffuse_map_sampler_state,pin.texcoord);
	float3 N = normalize(pin.normal).xyz;
	//float3 E = normalize(EyePos.xyz - pin.wPos);
	//SP6
	float3 E = normalize(pin.wEyeDir);
	float3 L = normalize(-light_direction.xyz);
	//����
	float3 A = ambient_color.rgb;
	//SP5
	//����(��}(����)���C�g)
	A = ambient_color.rgb*HemiSphereLight(N, float3(0.0f, 0.0f, 1.0f), float3(1.0f, 0.0f, 0.0f));

	//�g�U����
	float3 C = light_color.rgb;
	float3 Kd = float3(1.0f, 1.0f, 1.0f);
	float3 D = Diffuse(N, L, C, Kd);
	//���ʔ���
	float3 Ks = float3(1.0f, 1.0f, 1.0f);
	//SP5
	float Shinness = 5.0f;
	//SP6
	float3 S = BlinnPhongSpecular(N, -L, C, E, Ks, Shinness);

	color.rgb *= pin.color.rgb*float3(A + D + S);

	//�t�H�O�v�Z
	//color.rgb = Fog(color.rgb, EyePos.xyz, pin.wPos, fogColor, fogNear, fogFar);

	float3 NN = normalize(pin.wNormal);	//���[���h�@��
	float3 P = pin.wPos;				//���[���h���W

	Out.Color = color;
	//Out.wNormal = float4(NN, 1.0f);
	//Out.wPosition = float4(P, 1.0f);
	Out.Luminance = color * lumi_factor;

	return Out;
}
