#include "furShader.hlsli"

//--------------------------------------------
// 拡散反射関数（ランバートシェーディング） 
//--------------------------------------------
// N:法線(正規化済み)
// L:入射ベクトル(正規化済み)
// C:入射光(色・強さ)
// K:反射率(0～1.0)
float3 Diffuse(float3 N, float3 L, float3 C, float3 K)
{
	float D = dot(N, L);
	D = max(0, D);		//負の値を0にする
	return K * C * D;
}

//--------------------------------------------
// 鏡面反射関数(ブリン・フォン)
//--------------------------------------------
// N:法線(正規化済み)
// L:入射ベクトル(正規化済み)
// C:入射光(色・強さ)
// E:視点方向ベクトル(正規化済み)
// K:反射率(0～1.0)
// Power:ハイライトの強さ(輝き度)
float3 BlinnPhongSpecular(float3 N, float3 L, float3 C, float3 E,
	float3 K, float3 power)
{
	//ハーフベクトル
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

	// 環境光
	float3 A = AmbientColor.rgb;

	// 拡散反射
	float3 C = light_color.rgb;
	float3 Kd = float3(1.0f, 1.0f, 1.0f);
	float3 D = Diffuse(N, L, C, Kd);

	// 鏡面反射
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
