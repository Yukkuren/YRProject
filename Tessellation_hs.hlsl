//// 入力制御点
//struct VS_CONTROL_POINT_OUTPUT
//{
//	float3 vPosition : WORLDPOS;
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

//// パッチ定数関数
//HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
//	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
//	uint PatchID : SV_PrimitiveID)
//{
//	HS_CONSTANT_DATA_OUTPUT Output;

//	// ここにコードを挿入して出力を計算します
//	Output.EdgeTessFactor[0] =
//		Output.EdgeTessFactor[1] =
//		Output.EdgeTessFactor[2] =
//		Output.InsideTessFactor = 15; // たとえば、代わりに動的テセレーション係数を計算できます

//	return Output;
//}

//[domain("tri")]
//[partitioning("fractional_odd")]
//[outputtopology("triangle_cw")]
//[outputcontrolpoints(3)]
//[patchconstantfunc("CalcHSPatchConstants")]
//HS_CONTROL_POINT_OUTPUT main(
//	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
//	uint i : SV_OutputControlPointID,
//	uint PatchID : SV_PrimitiveID )
//{
//	HS_CONTROL_POINT_OUTPUT Output;

//	// ここにコードを挿入して出力を計算します
//	Output.vPosition = ip[i].vPosition;

//	return Output;
//}

#include "Tessellation.hlsli"

//HSConstantOutput HSConstant(
//InputPatch<HSInput, 3> ip,
//uint pid : SV_PrimitiveID)
//{
//    HSConstantOutput output = (HSConstantOutput) 0;;
//    output.factor[0] = divide;
//    output.factor[1] = divide;
//    output.factor[2] = divide;
//    output.inner_factor = divide;
//    return output;
//}
HSConstantOutput HSConstant(
	InputPatch<HSInput, 3> patch /*,uint pid : SV_PrimitiveID*/)
{
    HSConstantOutput output = (HSConstantOutput) 0;;
#if 1
    output.factor[0] = divide;
    output.factor[1] = divide;
    output.factor[2] = divide;
    output.inner_factor = divide;
#else
	float3 retf;
	float  ritf, uitf;
	ProcessTriTessFactorsAvg(divide, divide, retf, ritf, uitf);
	output.factor[0] = retf.x;
	output.factor[1] = retf.y;
	output.factor[2] = retf.z;
	output.inner_factor = ritf;
#endif
    return output;
}

//[domain("tri")]
//[partitioning("integer")]
//[outputtopology("triangle_cw")]
//[outputcontrolpoints(3)]
//[patchconstantfunc("HSConstant")]
//DSInput main(InputPatch<HSInput, 3> input,
//uint cpid : SV_OutputControlPointID,
//uint pid : SV_PrimitiveID)
//{
//    DSInput output = (DSInput) 0;
//    output.Position = input[cpid].Position;
//    output.Color = input[cpid].Color;
//    output.Tex = input[cpid].Tex;
//    output.Normal = input[cpid].Normal;
//    return output;
//}

[domain("tri")] // tri, quad, isoline
[partitioning("integer")] // integer, fractional_eve, fractional_odd, pow2
[outputtopology("triangle_cw")] // point, line, triangle_ccw, triangle_cw
[outputcontrolpoints(3)]
[patchconstantfunc("HSConstant")]
DSInput main(InputPatch<HSInput, 3> patch,
	uint cpid : SV_OutputControlPointID /*, uint pid : SV_PrimitiveID*/)
{
#if 0
	DSInput output = (DSInput)0;
	output.Position = patch[cpid].Position;
	output.Color = patch[cpid].Color;
	output.Tex = patch[cpid].Tex;
	output.Normal = patch[cpid].Normal;
	return output;
#else
    return patch[cpid];
#endif
}

