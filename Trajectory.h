#pragma once
#include <DirectXMath.h>
#include <vector>
#include <windows.h>
#include<wrl.h>
#include <d3d11.h>
#include "YRShader.h"

struct PosBuffer
{
	DirectX::XMFLOAT3	head = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	tail = { 0.0f,0.0f,0.0f };
	bool				isUsed = true;
};

struct PosData
{
	DirectX::XMFLOAT3	pos = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	normal = { 0.0f,0.0f,0.0f };
};

class Trajectory
{
public:
	void Init();
	void Update();
	void SetTrajectoryPos(const DirectX::XMFLOAT3& headPos, const DirectX::XMFLOAT3& tailPos);

	void render(
		YRShader* shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection
	);


private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>			vertex_buffer;		//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>			index_buffer;		//インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>			constant_buffer;	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	line_state;			//線描画
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	filling_state;		//塗りつぶし描画
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_state;


	std::vector<PosBuffer>		posArray;
	std::vector<PosData>		VB;

	struct cbuffer
	{
		DirectX::XMFLOAT4X4		world_view_projection;	//ワールド・ビュー・プロジェクション合成行列
		DirectX::XMFLOAT4X4		world;					//ワールド変換行列
		DirectX::XMFLOAT4		eyePos;
		DirectX::XMFLOAT4X4		view;
		DirectX::XMFLOAT4X4		projection;
		DirectX::XMFLOAT3		at;						//カメラ座標からfocusまでの単位ベクトル
		float					dummy;
	};
};
