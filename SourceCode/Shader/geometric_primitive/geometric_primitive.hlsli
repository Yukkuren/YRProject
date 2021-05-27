struct VS_OUT
{
	float4 position : SV_POSITION;
};

cbuffer CONSTANT_BUFFER : register(b0)
{
	row_major float4x4 world_view_projection;
	row_major float4x4 world;
	float4 material_color;
	float4 EyePos;					//カメラ座標
	row_major float4x4 view;		//ビュー行列
	row_major float4x4 projection;	//透視投影行列
	float3 at;						//カメラ座標からカメラの注視点へ向かう単位ベクトル
	float dummy;
}