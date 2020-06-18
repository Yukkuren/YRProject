#pragma once

#include <windows.h>
#include<d3d11.h>
#include<directxmath.h>
#include <wrl.h>
#include<string>

class geometric_primitive
{
public:
	ID3D11VertexShader		*vertex_shader;
	ID3D11PixelShader		*pixel_shader;
	ID3D11InputLayout		*input_layout;
	ID3D11Buffer			*vertex_buffer;			//頂点バッファ
	ID3D11Buffer			*index_buffer;			//インデックスバッファ
	ID3D11Buffer			*constant_buffer;		//定数バッファ
	ID3D11RasterizerState	*line_state;			//線描画
	ID3D11RasterizerState	*filling_state;			//塗りつぶし描画
	ID3D11DepthStencilState	*depth_state;
	ID3D11Resource *pResouce;

	geometric_primitive();

	~geometric_primitive()
	{
		vertex_shader->Release();
		pixel_shader->Release();
		input_layout->Release();
		vertex_buffer->Release();
		index_buffer->Release();
		constant_buffer->Release();
		line_state->Release();
		filling_state->Release();
		depth_state->Release();
		pResouce->Release();
	};

	void render(
		const DirectX::XMFLOAT4X4	&world_view,		//ワールド・ビュー・プロジェクション合成行列
		const DirectX::XMFLOAT4X4	&world_matrix,		//ワールド変換行列
		const DirectX::XMFLOAT4		&light_direction,	//ライト進行方向
		const DirectX::XMFLOAT4		&material_color,	//材質色
		bool						viewflag			//線・塗りつぶし描画フラグ
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
		DirectX::XMFLOAT4		light_direction;		//ライト進行方向
	};
};

namespace ALL
{
	HRESULT create_vertex(const char *cso_file, ID3D11VertexShader **vert, D3D11_INPUT_ELEMENT_DESC *layout, UINT numElements, ID3D11InputLayout **input);
	HRESULT CreatePixel(const char *ps_file, ID3D11PixelShader **pixel);
}
