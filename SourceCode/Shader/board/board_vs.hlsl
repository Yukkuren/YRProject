#include "board.hlsli"

PSInput main(VSInput input)
{
	PSInput output = (PSInput)0;

	float4 P = float4(input.position, 1.0);
	output.position = mul(P, world_view_projection);
	float4 N = normalize(mul(input.normal, world));

	//���_�J���[�̐ݒ�
	output.color = material_color;
	output.color.a = material_color.a;
	output.tex = input.tex;

	//Offset�l�ɂ���ăe�N�X�`�����W�ɕω���^����
	output.tex.x = input.tex.x + Offset_X;
	output.tex.y = input.tex.y + Offset_Y;

	return output;
}