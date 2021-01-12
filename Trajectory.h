#pragma once
#include <DirectXMath.h>
#include <vector>
#include <windows.h>
#include<wrl.h>
#include <d3d11.h>
#include "YRShader.h"
#include "Texture.h"
#include <memory>

struct PosBuffer
{
	DirectX::XMFLOAT3	head = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	tail = { 0.0f,0.0f,0.0f };
	float				alpha = 1.0f;
};

struct PosData
{
	DirectX::XMFLOAT3	pos = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT2	uv = { 0.0f,0.0f };
	float				alpha = 1.0f;
};

class Trajectory
{
public:
	void Init();
	void Update(float elapsed_time);
	void SetTrajectoryPos(const DirectX::XMFLOAT3& headPos, const DirectX::XMFLOAT3& tailPos);

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

	UINT	Index_size = 0;


	std::vector<PosBuffer>		posArray;
	std::vector<PosData>		VB;

	struct cbuffer
	{
		DirectX::XMFLOAT4X4		world_view_projection;	//ワールド・ビュー・プロジェクション合成行列
		DirectX::XMFLOAT4X4		world;					//ワールド変換行列
		DirectX::XMFLOAT4		eyePos;
		DirectX::XMFLOAT4X4		view;
		DirectX::XMFLOAT4X4		projection;
		DirectX::XMFLOAT4		material_color;
		DirectX::XMFLOAT3		at;						//カメラ座標からfocusまでの単位ベクトル
		float					dummy;
	};
};
