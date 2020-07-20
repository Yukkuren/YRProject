#pragma once

#include "Texture.h"
#include "YRShader.h"
#include <memory>

class AnimBoard
{
private:
	float w, h;			//大きさ
	int type;			//現在の画像の切り取り番号
	int split_size;		//画像の分割数
	XMFLOAT2 tex_size;	//画像の切り取り1枚の大きさ
	XMINT2	tex_split;	//画像の縦横分割数
	XMFLOAT2 tex_max_size;//画像全体の縦横の大きさ
	float elapsed;

	// 頂点データ
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
	//線描画
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	line_state;
	//塗りつぶし描画
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	filling_state;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_state;
	//テクスチャ利用
	std::shared_ptr<Texture> texture = nullptr;
	// 定数バッファのデータ定義
	struct CBuffer {
		XMFLOAT4X4	view;			// ビュー変換行列
		XMFLOAT4X4	projection;		// 透視変換行列
		XMFLOAT4X4	SR_Matrix;		//スケールと回転行列の合成後の行列
	};
public:
	AnimBoard(
		std::shared_ptr<Texture> texture,
		int split_size,			//画像の分割枚数
		XMFLOAT2 tex_size,		//画像の切り取り1枚の大きさ
		XMINT2 tex_split,		//画像の縦横分割数
		XMFLOAT2 tex_max_size	//画像全体の縦横の大きさ
	);
	void Render(
		YRShader* shader,
		DirectX::XMFLOAT3& pos,
		DirectX::XMFLOAT2& size,
		DirectX::XMFLOAT3& angle,
		float					speed,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		float					elapsed_time,
		float					alpha = 1.0f,
		const bool				viewflag = true
	);


	struct VERTEX
	{
		XMFLOAT3 Pos;	//位置
		XMFLOAT2 Size;	//板ポリの半分のワールド空間のサイズ
		XMFLOAT2 UV_Pos;//テクスチャ空間の切り取る左上の座標(0～1)
		XMFLOAT2 UV_Size;//テクスチャ空間の切り取る全体サイズ
		XMFLOAT4 Color;	//頂点色
	};
};
