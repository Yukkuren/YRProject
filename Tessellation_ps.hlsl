#include "Tessellation.hlsli"


//--------------------------------------------
//	ハーフランバート
//--------------------------------------------
// N:法線(正規化済み)
// L:入射ベクトル(正規化済み)
// C:入射光(色・強さ)
// K:反射率

float3 HalfLambert(float3 N, float3 L, float3 C, float3 K)
{
    float D = dot(N, -L);
    D = D * 0.5 + 0.5;
	//D = D * D;	// いい感じに減衰する？
    return C * D * K;
}


//--------------------------------------------
//	鏡面反射関数（フォン）
//--------------------------------------------
// N:法線(正規化済み)
// L:入射ベクトル(正規化済み)
// C:入射光(色・強さ)
// E:視点方向ベクトル(正規化済み)
// K:反射率(0～1.0)
// Power:ハイライトの強さ(輝き度)

//フォンシェーディング
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
// リムライト
//--------------------------------------------
// N:法線
// E:視点方向ベクトル(正規化済み)
// L:入射ベクトル(正規化済み)
// LightColor :ライト色
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
//	//環境光
//    float3 A = AmbientColor.rgb;

//	// 法線取得
//    float3 N = NormalTexture.Sample(DecalSampler, input.Tex).xyz;
//    N = N * 2.0f - 1.0f;

//	// 接空間軸
//    float3 vx = normalize(input.vT);
//    float3 vy = normalize(input.vB);
//    float3 vz = normalize(input.vN);

//	// 法線ワールド変換
//    float3x3 mat = { vx, vy, vz };

//	// ワールド変換
//    N = normalize(mul(N, mat)).xyz;

//	// 拡散反射
//    float3 Kd = { 1, 1, 1 };
//    float3 D = HalfLambert(N, L, C, Kd);

//	// 鏡面反射
//    float3 Ks = { 1, 1, 1 };
//    float3 S = PhongSpecular(N, L, C, E, Ks, 50);
//	// リムライト
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
	//環境光
    float3 A = AmbientColor.rgb;

	// 法線取得
    float3 N = NormalTexture.Sample(DecalSampler, input.Tex).xyz;
    N = N * 2.0f - 1.0f;

	// 接空間軸
    float3 vx = normalize(input.vT);
    float3 vy = normalize(input.vB);
    float3 vz = normalize(input.vN);

	// 法線ワールド変換
    float3x3 mat = { vx, vy, vz };

	// ワールド変換
    N = normalize(mul(N, mat)).xyz;

	// 拡散反射
    float3 Kd = { 1, 1, 1 };
    float3 D = HalfLambert(N, L, C, Kd);

	// 鏡面反射
    float3 Ks = { 1, 1, 1 };
    float3 S = PhongSpecular(N, L, C, E, Ks, 50);
	// リムライト
    float3 R = RimLight(N, E, L, C);

    color *= input.Color;
    color.rgb *= float4(A + D + S + R, 1);

    return color;
}