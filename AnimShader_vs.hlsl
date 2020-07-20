#include "AnimShader.hlsli"

GSInput main(VSInput input)
{
	GSInput output;
	output.Position = input.Position;
	output.Size = input.Size;
	output.UV_pos = input.UV_pos;
	output.UV_size = input.UV_size;
	output.Color = input.Color;

	return output;
}