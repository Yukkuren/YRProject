struct VS_OUT
{
	float4 position : SV_POSITION;
	float4 normal : NORMAL;
};

cbuffer CONSTANT_BUFFER : register(b0)
{
	row_major float4x4 world_view_projection;
	row_major float4x4 world;
	float4 material_color;
}