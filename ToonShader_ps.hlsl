#include "ToonShader.hlsli"

//-------------------------------------------- 
// トゥーンシェーダ用拡散反射関数（２階調） 
//-------------------------------------------- 

float3 ToonDiffuse(float3 N, float3 L, float3 C, float3 K)
{
	float D = dot(N, L);
	if (D < 0.3) D = 0.2;
	else D = 0.6;

	return K * C * D;
}

//--------------------------------------------
// トゥーンシェーダ用鏡面反射関数(２階調)
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


PSOutput main(PSInput input) : SV_TARGET
{
	PSOutput Out = (PSOutput)0;

	float4 color = DiffuseTexture.Sample(DecalSampler, input.Tex);
	float3 N = normalize(input.wNormal);
	float3 P = input.wPos;
	float3 E = normalize(EyePos.xyz - input.wPos);
	float3 L = normalize(-light_direction.rgb);
	// 環境光
	float3 A = ambient_color.rgb;

	// 拡散反射
	float3 C = light_color.rgb;
	float3 Kd = float3(1.0f, 1.0f, 1.0f);
	float3 D = ToonDiffuse(N, L, C, Kd);

	// 鏡面反射
	float3 Ks = float3(1, 1, 1);
	float3 S = ToonSpecular(N, L, C, E, Ks, 5);

	

	//アウトライン
	 if (input.Color.a < 0)
	 {
		Out.Color = float4(input.Color.rgb, 1);
		//Out.wNormal = float4(N, 1.0f);
		//Out.wPosition = float4(P, 1.0f);
		float4 lumi = float4(lumi_factor, lumi_factor, lumi_factor,1.0f);
		Out.Luminance = lumi;
		return Out;
	 }

	color.rgb *= input.Color.rgb * float3(A + D + S);

	Out.Color = color * material_color;
	//Out.wNormal = float4(N, 1.0f);
	//Out.wPosition = float4(P, 1.0f);
	float4 lumi = float4(lumi_factor, lumi_factor, lumi_factor, 1.0f);
	Out.Luminance = lumi;

	return Out;
}