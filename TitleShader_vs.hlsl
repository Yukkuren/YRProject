#include "TitleShader.hlsli"
//--------------------------------------------
//	���_�V�F�[�_�[
//--------------------------------------------

PSInput main(VSInput input)
{
	PSInput output = (PSInput)0;

	// �o�͒l�ݒ�
	output.fragCoord = input.Tex;

	return output;
}