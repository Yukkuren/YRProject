#pragma once
#include <DirectXMath.h>
#include <vector>
#include <windows.h>
#include<wrl.h>
#include <d3d11.h>
#include "YRShader.h"
#include "Texture.h"
#include <memory>
#include "Sampler.h"

//-----------------------------------------------------------------
//			剣の軌跡クラス
//-----------------------------------------------------------------
//・剣の軌跡を表示するクラス
//・モデルの剣の座標をフレームごとに保存し、その場所に軌跡を描画する
//-----------------------------------------------------------------

//モデルから剣の情報を保存する構造体
struct PosBuffer
{
	DirectX::XMFLOAT3	head = { 0.0f,0.0f,0.0f };	//剣の先端
	DirectX::XMFLOAT3	tail = { 0.0f,0.0f,0.0f };	//剣の根本
	float				alpha = 1.0f;				//軌跡のアルファ値
};

//保存した情報から生成した頂点データを保存するクラス
struct PosData
{
	DirectX::XMFLOAT3	pos = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT2	uv = { 0.0f,0.0f };
	float				alpha = 1.0f;
};

class Trajectory
{
public:
	void Init(size_t max_count);
	void Update(float elapsed_time);
	void SetTrajectoryPos(const DirectX::XMFLOAT3& headPos, const DirectX::XMFLOAT3& tailPos);

	void DeletePos();

	void render(
		const DirectX::XMFLOAT3& pos,
		const DirectX::XMFLOAT3& scale,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& material_color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		const bool				viewflag = true
	);

	std::unique_ptr<Texture>	texture = nullptr;
	std::unique_ptr<YRShader>	shader = nullptr;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>			vertex_buffer;		//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>			index_buffer;		//インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>			constant_buffer;	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	line_state;			//線描画
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	filling_state;		//塗りつぶし描画
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_state;

	std::shared_ptr<Sampler> sampler_wrap = nullptr;

	UINT	Index_size = 0;

	size_t max_count = 0;


	std::vector<PosBuffer>		posArray;

	struct cbuffer
	{
		DirectX::XMFLOAT4X4		world_view_projection;	//ワールド・ビュー・プロジェクション合成行列
		DirectX::XMFLOAT4X4		world;					//ワールド変換行列
		DirectX::XMFLOAT4		material_color;			//マテリアルの色
		DirectX::XMFLOAT4		eyePos;					//カメラの座標
		DirectX::XMFLOAT4X4		view;					//ビュー行列
		DirectX::XMFLOAT4X4		projection;
		DirectX::XMFLOAT3		at;						//カメラ座標からfocusまでの単位ベクトル
		float					dummy;
		DirectX::XMFLOAT4		dummy2;

		cbuffer()
		{
			world_view_projection =
			{
				0.0f,0.0f,0.0f,0.0f,
				0.0f,0.0f,0.0f,0.0f,
				0.0f,0.0f,0.0f,0.0f,
				0.0f,0.0f,0.0f,0.0f
			};
			world =
			{
				0.0f,0.0f,0.0f,0.0f,
				0.0f,0.0f,0.0f,0.0f,
				0.0f,0.0f,0.0f,0.0f,
				0.0f,0.0f,0.0f,0.0f
			};
			material_color = { 0.0f,0.0f,0.0f,0.0f };
			eyePos = { 0.0f,0.0f,0.0f,0.0f };
			view =
			{
				0.0f,0.0f,0.0f,0.0f,
				0.0f,0.0f,0.0f,0.0f,
				0.0f,0.0f,0.0f,0.0f,
				0.0f,0.0f,0.0f,0.0f
			};
			projection =
			{
				0.0f,0.0f,0.0f,0.0f,
				0.0f,0.0f,0.0f,0.0f,
				0.0f,0.0f,0.0f,0.0f,
				0.0f,0.0f,0.0f,0.0f
			};
			at = { 0.0f,0.0f,0.0f };
			dummy = 0.0f;
			dummy2 = { 0.0f,0.0f,0.0f,0.0f };
		}
	};
};
