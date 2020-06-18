
//--------------------------------------------
//	テクスチャ
//--------------------------------------------


Texture2D DiffuseTexture : register(t0);
SamplerState DecalSampler : register(s0);

//--------------------------------------------
//	グローバル変数
//--------------------------------------------

cbuffer CBPerMesh : register( b0 )
{
    matrix  World;
	matrix	matWVP;
};

//--------------------------------------------
//	データーフォーマット
//--------------------------------------------

struct VSInput
{
	float3 Position : POSITION;
	float3 Normal   : NORMAL;
	float2 Tex      : TEXCOORD;
	float4 Color    : COLOR;
};

struct PSInput
{
	float4 Position : SV_POSITION; 
	float2 Tex : TEXCOORD;
	float4 Color : COLOR;
};

//--------------------------------------------
//	頂点シェーダー
//--------------------------------------------

PSInput VSMain( VSInput input )
{
    PSInput output = (PSInput)0;

	float4 P = float4(input.Position, 1.0);
	// ワールド空間に変換.
	float4 worldPos = mul(World, P);


	// 射影空間に変換.
	float4 projPos = mul(matWVP, P);

	// 出力値設定.
	output.Position = projPos;
	output.Color = input.Color;
	output.Color = float4(1,1,1,1);
	output.Tex = input.Tex;

    return output;
}

//--------------------------------------------
//	ピクセルシェーダー
//--------------------------------------------

float4 PSMain( PSInput input ) : SV_TARGET0
{
	return DiffuseTexture.Sample(DecalSampler, input.Tex) * input.Color;
}
