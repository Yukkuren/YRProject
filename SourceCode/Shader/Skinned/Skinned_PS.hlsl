Texture2D diffuse_map : register(t0);
SamplerState diffuse_map_sampler_state : register(s0);

#include"Skinned_mesh.hlsli"
//SP4
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
	return K*C*D;
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
	S = S*K*C;
	return S;
}
//--------------------


//SP5
//------------------------------
//		フォグ
//------------------------------
// color	:現在の色
// viewPos	:視点
// Pos		:ピクセルの位置
// fogColor:フォグの色
// Near	:フォグの近距離
// Far		:フォグの遠距離
float3 Fog(float3 color, float3 viewPos, float3 Pos, float3 fogColor, float Near, float Far)
{
	//フォグ計算
	float Len = length(viewPos - Pos);
	float fogalpha = saturate((Len - Near) / (Far - Near));
	return color.rgb*(1.0 - fogalpha) + fogColor*fogalpha;
}


//------------------------------
//		半球ライト
//------------------------------
// N			:法線
// SkyColor		:空(上)色
// GroundColor	:地面(下)色
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
	//環境光
	float3 A = ambient_color.rgb;
	//SP5
	//環境光(阪急(半球)ライト)
	A = ambient_color.rgb*HemiSphereLight(N, float3(0.0f, 0.0f, 1.0f), float3(1.0f, 0.0f, 0.0f));

	//拡散反射
	float3 C = light_color.rgb;
	float3 Kd = float3(1.0f, 1.0f, 1.0f);
	float3 D = Diffuse(N, L, C, Kd);
	//鏡面反射
	float3 Ks = float3(1.0f, 1.0f, 1.0f);
	//SP5
	float Shinness = 5.0f;
	//SP6
	float3 S = BlinnPhongSpecular(N, -L, C, E, Ks, Shinness);

	color.rgb *= pin.color.rgb*float3(A + D + S);

	//フォグ計算
	//color.rgb = Fog(color.rgb, EyePos.xyz, pin.wPos, fogColor, fogNear, fogFar);

	float3 NN = normalize(pin.wNormal);	//ワールド法線
	float3 P = pin.wPos;				//ワールド座標

	Out.Color = color;
	//Out.wNormal = float4(NN, 1.0f);
	//Out.wPosition = float4(P, 1.0f);
	Out.Luminance = color * lumi_factor;

	return Out;
}
