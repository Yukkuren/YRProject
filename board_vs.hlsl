#include "board.hlsli"

PSInput main(VSInput input)
{
	PSInput output = (PSInput)0;

	float4 P = float4(input.position, 1.0);
	output.position = mul(P, world_view_projection);
	float4 N = normalize(mul(input.normal, world));

	//頂点カラーの設定
	output.color = material_color;
	output.color.a = material_color.a;
	output.tex = input.tex;

	//Offset値によってテクスチャ座標に変化を与える
	output.tex.x = input.tex.x + Offset_X;
	output.tex.y = input.tex.y + Offset_Y;

	return output;
}