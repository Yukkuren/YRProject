#include "ToonShader.hlsli"

[maxvertexcount(6)]
void main(
	triangle GSInput input[3],
	uint primID : SV_PrimitiveID,
	inout TriangleStream<PSInput> Stream
)
{
	//キャラクタ
	for (int i = 0; i < 3; i++)
	{
		PSInput data;

		data.SV_Position = mul( input[i].Position,world_view_projection);
		data.SV_Position /= data.SV_Position.w;
		data.Position = mul(input[i].Position, world);
		data.wNormal = input[i].wNormal;
		data.Tex = input[i].Tex;
		data.Color = input[i].Color;
		data.wPos = input[i].wPos;

		Stream.Append(data);
	}

	Stream.RestartStrip();


	{
		//輪郭線(裏返しポリゴン作成)
		PSInput data;
		float4 P = input[0].Position + float4(input[0].Normal, 0.0) * LW;
		data.SV_Position = mul(P,world_view_projection);
		data.SV_Position /= data.SV_Position.w;
		data.Position = mul(P, world);
		data.wNormal = -input[0].wNormal;
		data.Tex = input[0].Tex;
		data.Color = LC;
		data.wPos = input[0].wPos;

		Stream.Append(data);
		P = input[2].Position + float4(input[2].Normal, 0.0) * LW;
		data.SV_Position = mul(P, world_view_projection);
		data.SV_Position /= data.SV_Position.w;
		data.Position = mul(P, world);
		data.wNormal = -input[2].wNormal;
		data.Tex = input[2].Tex;
		data.Color = LC;
		data.wPos = input[2].wPos;
		Stream.Append(data);

		P = input[1].Position + float4(input[1].Normal, 0.0) * LW;
		data.SV_Position = mul(P, world_view_projection);
		data.SV_Position /= data.SV_Position.w;
		data.Position = mul(P, world);
		data.wNormal = -input[1].wNormal;
		data.Tex = input[1].Tex;
		data.Color = LC;
		data.wPos = input[1].wPos;
		Stream.Append(data);
	}
	Stream.RestartStrip();
}