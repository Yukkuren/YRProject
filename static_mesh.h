#pragma once
#include <windows.h>
#include<d3d11.h>
#include<directxmath.h>
#include <wrl.h>
#include<string>
#include<vector>

class Static_mesh
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
	ID3D11ShaderResourceView *shader_view;
	D3D11_TEXTURE2D_DESC texture2d_desc;
	ID3D11SamplerState *sampler_state;
	ID3D11Resource *pResouce;
	unsigned int indexsize;
	std::wstring Texture_name;
	std::wstring mtl_name;

	Static_mesh(const wchar_t* obj_name);

	~Static_mesh()
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
		shader_view->Release();
		sampler_state->Release();
	};

	//void Mesh_Load(ID3D11Device *device, const wchar_t *obj_file);

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
		DirectX::XMFLOAT2		texcoord;
	};

	struct cbuffer
	{
		DirectX::XMFLOAT4X4		world_view_projection;	//ワールド・ビュー・プロジェクション合成行列
		DirectX::XMFLOAT4X4		world;					//ワールド変換行列
		DirectX::XMFLOAT4		material_color;			//材質色
		DirectX::XMFLOAT4		light_direction;		//ライト進行方向
	};

	struct subset
	{
		std::wstring usemtl;
		u_int index_start = 0;	//start number of index buffer
		u_int index_count = 0;	//number of virtices(indices)
	};
	std::vector<subset>subsets;

	struct material
	{
		std::wstring newmtl;
		DirectX::XMFLOAT3 Ka = { 0.2f,0.2f,0.2f };
		DirectX::XMFLOAT3 Kd = { 0.8f,0.8f,0.8f };
		DirectX::XMFLOAT3 Ks = { 1.0f,1.0f,1.0f };
		u_int illum = 1;
		std::wstring map_Kd;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>shader_resouce_view;
	};
	std::vector<material>materials;
};

namespace MeshTexShader
{
	HRESULT OBJ_load
	(
		unsigned int *indexsize,
		const wchar_t *obj_file,
		ID3D11Buffer**vertex_buffer,
		ID3D11Buffer**index_buffer, 
		ID3D11Buffer**constant_buffer,
		std::wstring *mtl_name,
		std::wstring *texture_name,
		std::vector<Static_mesh::material> *materials,
		std::vector<Static_mesh::subset> *subsets
	);

	HRESULT TextureNameGet
	(
		std::wstring *texture_name, 
		std::wstring *mtl_file,
		std::vector<Static_mesh::material> *materials
	);

	HRESULT TextureLoad
	(
		std::wstring* wchar,
		D3D11_TEXTURE2D_DESC *texture2d_desc,
		ID3D11ShaderResourceView **pShader
	);
	HRESULT create_vertex(const char *cso_file, ID3D11VertexShader **vert, D3D11_INPUT_ELEMENT_DESC *layout, UINT numElements, ID3D11InputLayout **input);
	HRESULT CreatePixel(const char *ps_file, ID3D11PixelShader **pixel);

}