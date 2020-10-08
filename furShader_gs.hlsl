#include "furShader.hlsli"

[maxvertexcount(LAYER * 3)]
void main(triangle GSInput input[3],		//入力プリミティブタイプ
	inout TriangleStream<PSInput> Stream)	//出力データタイプ
{
	//面法線算出
	float3 vec1 = input[1].Position.xyz - input[0].Position.xyz;
	float3 vec2 = input[2].Position.xyz - input[1].Position.xyz;

	float3 Normal = cross(vec1, vec2);
	float3 N = normalize(Normal);

	for (int j = 0; j < LAYER; j++) {
		for (int i = 0; i < 3; i++) {
			PSInput data = (PSInput)0;
			float4 P = input[i].Position;
			P.xyz += Normal * Distance * j;
			data.Position = mul(P, world_view_projection);
			data.wNormal = normalize(mul(input[i].Normal,(float3x3)world));
			data.Tex = input[i].Tex * Density;
			data.Color = input[i].Color;
			//data.Color.rgb = input[i].Color.rgb * ((float)j / (float)LAYER);
			//data.Color.a = input[i].Color.a * (1.0 - (float)j / (float)LAYER);
			data.wPos = input[i].wPos;
			Stream.Append(data);
		}

	}
	Stream.RestartStrip();
}
