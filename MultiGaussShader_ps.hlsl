#include "MultiGaussShader.hlsli"

float4 FetchColor(Texture2D map, float2 uv)
{
	float4 output = 0;
	output += map.SampleLevel(ColorSampler, uv, 0);
	return output;
}

float4 main(const VSOutput input) : SV_TARGET0
{
	float4 result = 0;
	result += FetchColor(ColorBuffer0, input.TexCoord);
	result += FetchColor(ColorBuffer1, input.TexCoord);
	result += FetchColor(ColorBuffer2, input.TexCoord);
	result += FetchColor(ColorBuffer3, input.TexCoord);
	result += FetchColor(ColorBuffer4, input.TexCoord);
	result += FetchColor(ColorBuffer5, input.TexCoord);
	result += FetchColor(ColorBuffer6, input.TexCoord);

	result.w = 1.0f;
	return result;
}
