#include "SilexarsShader.hlsli"
//--------------------------------------------
//	���_�V�F�[�_�[
//--------------------------------------------

PSInput main(VSInput input)
{
    PSInput output = (PSInput) 0;

	// �o�͒l�ݒ�
    output.fragCoord = input.Tex;
    output.position = float4(input.Position, 1.0);

    return output;
}