#include "GaussShader.hlsli"

float4 main(const VSOutput input) : SV_TARGET0
{
	float4 result = 0;

	for (int i = 0; i < SampleCount; ++i)
	{
		result += Offset[i].z * ColorBuffer.Sample(ColorSampler, input.Tex + Offset[i].xy);
	}

	result.w = 1.0f;

	return result;
}