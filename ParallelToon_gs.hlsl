#include "ParallelToon.hlsli"


float4 MatPos(float4 POS)
{
	float3 E = EyePos.xyz;
	//float4 POS = input[0].Position + float4(input[0].Normal, 0.0) * LW;
	float4 wPOS = mul(POS, world);
	float3 P = E + at * dot(at, wPOS - E);
	float len = length(P - E);
	float h = length(wPOS - P) / len;
	h = max(0.05, h);
	//float h = (abs(len) < 0.001) ? 1.0 : length(wPOS - P) / len;
	//h *= dot(wPOS - P, at) > 0 ? -1 : 1;
	row_major float4x4 Mat = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, h, 0,
		0, 0, 0, 1
	};
	row_major float4x4 newWorld = mul(world, Mat);
	float4 Posi = mul(POS, newWorld);
	Posi = mul(Posi, view);
	Posi = mul(Posi, projection);
	Posi /= Posi.w;

	return Posi;
}


[maxvertexcount(6)]
void main(
	triangle GSInput input[3],
	uint primID : SV_PrimitiveID,
	inout TriangleStream<PSInput> Stream
)
{
	//ƒLƒƒƒ‰ƒNƒ^
	for (int i = 0; i < 3; i++)
	{
		PSInput data;

		float3 E = EyePos.xyz;

		float3 P = E + at * dot(at, input[i].wPos - E);
		float len = length(P - E);
		float h = length(input[i].wPos - P) / len;
		h = max(0.05, h);
		row_major float4x4 Mat = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, h, 0,
			0, 0, 0, 1
		};
		row_major float4x4 newWorld = mul(world, Mat);
		float4 Pos = mul(input[i].Position, newWorld);
		Pos = mul(Pos, view);
		Pos = mul(Pos, projection);
		Pos /= Pos.w;

		data.SV_Position = Pos;
		data.wNormal = input[i].wNormal;
		data.Tex = input[i].Tex;
		data.Color = input[i].Color;
		data.wPos = input[i].wPos;

		Stream.Append(data);
	}

	Stream.RestartStrip();
	//return;
	{
		//—ÖŠsü(— •Ô‚µƒ|ƒŠƒSƒ“ì¬)
		PSInput data;
		float4 P = input[0].Position + float4(input[0].Normal, 0.0) * LW;
		//float3 E = EyePos.xyz;
		//float4 POS = input[0].Position + float4(input[0].Normal, 0.0) * LW;
		//float4 wPOS = mul(POS, world);
		//float3 P = E + at * dot(at, wPOS - E);
		//float h = length(wPOS - P) / length(P - E);
		////h *= dot(input[i].wPos - P, at) > 0 ? -1 : 1;
		//row_major float4x4 Mat = {
		//	1, 0, 0, 0,
		//	0, 1, 0, 0,
		//	0, 0, h, 0,
		//	0, 0, 0, 1
		//};
		//row_major float4x4 newWorld = mul(world, Mat);
		//float4 Posi = mul(POS, newWorld);
		//Posi = mul(Posi, view);
		//Posi = mul(Posi, projection);
		//Posi /= Posi.w;

		data.SV_Position = MatPos(P);
		/*data.SV_Position = mul(P,world_view_projection);
		data.SV_Position /= data.SV_Position.w;*/
		data.wNormal = -input[0].wNormal;
		data.Tex = input[0].Tex;
		data.Color = LC;
		data.wPos = input[0].wPos;

		Stream.Append(data);
		P = input[2].Position + float4(input[2].Normal, 0.0) * LW;
		/*data.SV_Position = mul(P, world_view_projection);
		data.SV_Position /= data.SV_Position.w;*/
		data.SV_Position = MatPos(P);
		data.wNormal = -input[2].wNormal;
		data.Tex = input[2].Tex;
		data.Color = LC;
		data.wPos = input[2].wPos;
		Stream.Append(data);

		P = input[1].Position + float4(input[1].Normal, 0.0) * LW;
		data.SV_Position = MatPos(P);
		/*data.SV_Position = mul(P, world_view_projection);
		data.SV_Position /= data.SV_Position.w;*/
		data.wNormal = -input[1].wNormal;
		data.Tex = input[1].Tex;
		data.Color = LC;
		data.wPos = input[1].wPos;
		Stream.Append(data);
	}
	Stream.RestartStrip();
}