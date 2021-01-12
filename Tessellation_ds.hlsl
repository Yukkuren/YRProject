//struct DS_OUTPUT
//{
//	float4 vPosition  : SV_POSITION;
//	// TODO: 他のスタッフの変更/追加
//};

//// 出力制御点
//struct HS_CONTROL_POINT_OUTPUT
//{
//	float3 vPosition : WORLDPOS;
//};

//// 出力パッチ定数データ。
//struct HS_CONSTANT_DATA_OUTPUT
//{
//	float EdgeTessFactor[3]			: SV_TessFactor; // たとえば、クワド ドメインの [4] になります
//	float InsideTessFactor			: SV_InsideTessFactor; // たとえば、クワド ドメインの Inside[2] になります
//	// TODO: 他のスタッフの変更/追加
//};

//#define NUM_CONTROL_POINTS 3

//[domain("tri")]
//DS_OUTPUT main(
//	HS_CONSTANT_DATA_OUTPUT input,
//	float3 domain : SV_DomainLocation,
//	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
//{
//	DS_OUTPUT Output;

//	Output.vPosition = float4(
//		patch[0].vPosition*domain.x+patch[1].vPosition*domain.y+patch[2].vPosition*domain.z,1);

//	return Output;
//}

#include "Tessellation.hlsli"

[domain("tri")]
PSInput main(HSConstantOutput input,
    float3 UV : SV_DomaInLocation,
    const OutputPatch<DSInput, 3> patch)
{
    PSInput output = (PSInput) 0;
    //頂点色
    float4 C = patch[0].Color.x * UV.x + patch[1].Color.y * UV.y + patch[2].Color.z * UV.z;

    //UV 座標
    float2 Tex = patch[0].Tex * UV.x + patch[1].Tex * UV.y + patch[2].Tex * UV.z;

    //ローカル法線取得
    float3 N = patch[0].Normal * UV.x + patch[1].Normal * UV.y + patch[2].Normal * UV.z;

    //ワールド法線取得
    float3 wN = normalize(mul(N, (float3x3)world));

    //ワールド接空間
    float3 vN = wN;
    float3 vB = { 0, 1, 0.001f };
    float3 vT;
    vB = normalize(vB);
    vT = normalize(cross(vB, vN));
    vB = normalize(cross(vN, vT));
    //ハイトマップ
    float H = HeightTexture.SampleLevel(WrapSampler, Tex, 0).x;
    //頂点座標
    float3 pos = patch[0].Position * UV.x + patch[1].Position * UV.y + patch[2].Position * UV.z;
    //pos += N * H * 0.1;
    float4 P = float4(pos, 1.0);
    C.w = 1.0;

    //ワールド接空間軸（ライティング用）
    output.vT = vT;
    output.vB = vB;
    output.vN = vN;

    //情報設定
    output.Position = mul(P, world_view_projection);
    output.wPosition = mul(P,world).xyz;;
    output.Color = C;
    output.Tex = Tex;

    return output;
}