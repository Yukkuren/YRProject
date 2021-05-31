#pragma once

#include <windows.h>
#include<directxmath.h>
#include<string>
#include "../YRShader.h"
#include "../Texture.h"

//-----------------------------------------------------------------
//			ジオメトリックプリミティブクラス
//-----------------------------------------------------------------
//・指定した大きさのポリゴンを生成できるクラス
//-----------------------------------------------------------------

class geometric_primitive
{
public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>			vertex_buffer;		//頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>			index_buffer;		//インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>			constant_buffer;	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	line_state;			//線描画
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	filling_state;		//塗りつぶし描画
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_state;

	//std::shared_ptr<Texture> texture = nullptr;							//テクスチャ

	geometric_primitive();
	//geometric_primitive(std::shared_ptr<Texture> tex);

	~geometric_primitive()
	{
		/*vertex_shader->Release();
		pixel_shader->Release();
		input_layout->Release();
		vertex_buffer->Release();
		index_buffer->Release();
		constant_buffer->Release();
		line_state->Release();
		filling_state->Release();
		depth_state->Release();
		pResouce->Release();*/
	};

	void render(
		YRShader		*shader,
		const DirectX::XMFLOAT3& pos,
		const DirectX::XMFLOAT3& scale,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		//UNIT.23
		const DirectX::XMFLOAT4			material_color = { 1.0f,1.0f,1.0f,1.0f },
		const bool						viewflag = true
	);

	void render(
		YRShader* shader,
		Texture* texture,
		const DirectX::XMFLOAT3& model_pos,
		const DirectX::XMFLOAT3& pos,
		const DirectX::XMFLOAT3& scale,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		//UNIT.23
		const DirectX::XMFLOAT4			material_color = { 1.0f,1.0f,1.0f,1.0f },
		const bool						viewflag = true
	);

	struct vertex
	{
		DirectX::XMFLOAT3		position;
		DirectX::XMFLOAT3		normal;
	};

	struct cbuffer
	{
		DirectX::XMFLOAT4X4		world_view_projection;	//ワールド・ビュー・プロジェクション合成行列
		DirectX::XMFLOAT4X4		world;					//ワールド変換行列
		DirectX::XMFLOAT4		material_color;			//材質色
		DirectX::XMFLOAT4		eyePos;
		DirectX::XMFLOAT4X4		view;
		DirectX::XMFLOAT4X4		projection;
		DirectX::XMFLOAT3		at;						//カメラ座標からfocusまでの単位ベクトル
		float					dummy;
	};
};

//namespace ALL
//{
//	/*HRESULT create_vertex(const char *cso_file, ID3D11VertexShader **vert, D3D11_INPUT_ELEMENT_DESC *layout, UINT numElements, ID3D11InputLayout **input);
//	HRESULT CreatePixel(const char *ps_file, ID3D11PixelShader **pixel);*/
//}