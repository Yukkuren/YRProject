#include "TitleShader.hlsli"
//--------------------------------------------
//	頂点シェーダー
//--------------------------------------------

PSInput main(VSInput input)
{
	PSInput output = (PSInput)0;

	// 出力値設定
	output.fragCoord = input.Tex;

	return output;
}