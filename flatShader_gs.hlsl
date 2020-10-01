#include "flatShader.hlsli"

[maxvertexcount(3)]
void main(
	triangle GSInput input[3],				//“ü—Í
	inout TriangleStream<PSInput> Stream)	//o—Í
{
	//–Ê–@üZo
	float3 vec1 = input[1].Position.xyz - input[0].Position.xyz;
	float3 vec2 = input[2].Position.xyz - input[1].Position.xyz;
	
	float3 Normal = cross(vec1, vec2);
	float3 N = normalize(Normal);

	for (int i = 0; i < 3; i++) {
		PSInput data = (PSInput)0;
		
		data.Position = mul(input[i].Position, world_view_projection);
		data.wNormal = normalize(mul(N,(float3x3)world));
		data.Tex = input[i].Tex;
		data.Color = input[i].Color;
		data.wPos = input[i].wPos;
		Stream.Append(data);
	}
	Stream.RestartStrip();

}
