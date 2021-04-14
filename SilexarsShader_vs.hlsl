#include "SilexarsShader.hlsli"
//--------------------------------------------
//	頂点シェーダー
//--------------------------------------------

PSInput main(VSInput input)
{
    PSInput output = (PSInput) 0;

	// 出力値設定
    output.fragCoord = input.Tex;
    output.position = float4(input.Position, 1.0);

    return output;
}