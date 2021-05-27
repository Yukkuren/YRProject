#include "MultiGaussShader.hlsli"

VSOutput main(VSInput input)
{
	VSOutput Out = (VSOutput)0;

	Out.Position = float4(input.Position, 1.0);
	Out.TexCoord = input.Tex;

	return Out;
}