#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include<wrl.h>
#include <vector>
using namespace DirectX;


//INPUT_ELEMENT_DESC管理用クラス
class INPUT_ELEMENT_DESC
{
private:

	D3D11_INPUT_ELEMENT_DESC pos		=	{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 };
	D3D11_INPUT_ELEMENT_DESC normal		=	{ "NORMAL",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 };
	D3D11_INPUT_ELEMENT_DESC tex		=	{ "TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 };
	D3D11_INPUT_ELEMENT_DESC uv_pos		=	{ "UV_POS",	0,	DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 };
	D3D11_INPUT_ELEMENT_DESC uv_size	=	{ "UV_SIZE",	0,	DXGI_FORMAT_R32G32_FLOAT,		0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 };
	D3D11_INPUT_ELEMENT_DESC wight		=	{ "WEIGHTS",	0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 };
	D3D11_INPUT_ELEMENT_DESC bone		=	{ "BONES",		0,	DXGI_FORMAT_R32G32B32A32_UINT,	0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 };
	D3D11_INPUT_ELEMENT_DESC color		=	{ "COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	D3D11_APPEND_ALIGNED_ELEMENT,	D3D11_INPUT_PER_VERTEX_DATA,	0 };


	//コンストラクタで頂点データを作成
	INPUT_ELEMENT_DESC()
	{
		//skineed_mesh(FBX)
		skin_element_desc.push_back(pos);
		skin_element_desc.push_back(normal);
		skin_element_desc.push_back(tex);
		skin_element_desc.push_back(wight);
		skin_element_desc.push_back(bone);

		//static_mesh(OBJ)
		static_element_desc.push_back(pos);
		static_element_desc.push_back(normal);
		static_element_desc.push_back(tex);

		//sprite(png)
		sprite_element_desc.push_back(pos);
		sprite_element_desc.push_back(color);
		sprite_element_desc.push_back(tex);

		//geometric
		geometric_element_desc.push_back(pos);

		//board
		board_element_desc.push_back(pos);
		board_element_desc.push_back(normal);
		board_element_desc.push_back(tex);
		board_element_desc.push_back(color);

		//anim
		anim_element_desc.push_back(pos);
		anim_element_desc.push_back(tex);
		anim_element_desc.push_back(uv_pos);
		anim_element_desc.push_back(uv_size);
		anim_element_desc.push_back(color);

		//toon
		toon_element_desc.push_back(pos);
		toon_element_desc.push_back(normal);
		toon_element_desc.push_back(tex);
		toon_element_desc.push_back(wight);
		toon_element_desc.push_back(bone);
		
		//toGbuf
		toGbuf_element_desc.push_back(pos);
		toGbuf_element_desc.push_back(normal);
		toGbuf_element_desc.push_back(tex);
		toGbuf_element_desc.push_back(color);

		//spriteEx
		spriteEx_element_desc.push_back(pos);
		spriteEx_element_desc.push_back(normal);
		spriteEx_element_desc.push_back(tex);
		spriteEx_element_desc.push_back(color);

		//flat
		flat_element_desc.push_back(pos);
		flat_element_desc.push_back(normal);
		flat_element_desc.push_back(tex);
		flat_element_desc.push_back(wight);
		flat_element_desc.push_back(bone);

		//gauss
		gauss_element_desc.push_back(pos);
		gauss_element_desc.push_back(normal);
		gauss_element_desc.push_back(tex);
		gauss_element_desc.push_back(color);

		//multigauss
		multi_gauss_element_desc.push_back(pos);
		multi_gauss_element_desc.push_back(normal);
		multi_gauss_element_desc.push_back(tex);
		multi_gauss_element_desc.push_back(color);

		//fur
		fur_element_desc.push_back(pos);
		fur_element_desc.push_back(normal);
		fur_element_desc.push_back(tex);
		fur_element_desc.push_back(wight);
		fur_element_desc.push_back(bone);
	};
public:
	enum ShaderType
	{
		SKIN,
		STATIC,
		SPRITE,
		GEO,
		BOARD,
		ANIM,
		TOON,
		TOGBUF,
		SPRITE_EX,
		FLAT,
		GAUSS,
		MULTI_GAUSS,
		FUR,
	};

	std::vector<D3D11_INPUT_ELEMENT_DESC> skin_element_desc;
	std::vector<D3D11_INPUT_ELEMENT_DESC> static_element_desc;
	std::vector<D3D11_INPUT_ELEMENT_DESC> sprite_element_desc;
	std::vector<D3D11_INPUT_ELEMENT_DESC> geometric_element_desc;
	std::vector<D3D11_INPUT_ELEMENT_DESC> board_element_desc;
	std::vector<D3D11_INPUT_ELEMENT_DESC> anim_element_desc;
	std::vector<D3D11_INPUT_ELEMENT_DESC> toon_element_desc;
	std::vector<D3D11_INPUT_ELEMENT_DESC> toGbuf_element_desc;
	std::vector<D3D11_INPUT_ELEMENT_DESC> spriteEx_element_desc;
	std::vector<D3D11_INPUT_ELEMENT_DESC> flat_element_desc;
	std::vector<D3D11_INPUT_ELEMENT_DESC> gauss_element_desc;
	std::vector<D3D11_INPUT_ELEMENT_DESC> multi_gauss_element_desc;
	std::vector<D3D11_INPUT_ELEMENT_DESC> fur_element_desc;

	static INPUT_ELEMENT_DESC &getInstance()
	{
		static INPUT_ELEMENT_DESC instance;
		return instance;
	}

};
#define				YRINPUT_ELEMENT_DESC (INPUT_ELEMENT_DESC::getInstance())

class YRShader
{
public:
private:
	INPUT_ELEMENT_DESC::ShaderType type;
protected:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		VSShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		PSShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11GeometryShader>	GSShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11HullShader>		HSShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DomainShader>		DSShader = nullptr;

	Microsoft::WRL::ComPtr<ID3D11InputLayout>		VertextLayout = nullptr;
	
public:
	//YRShader() {};
	YRShader(INPUT_ELEMENT_DESC::ShaderType type) : type(type) {};
	virtual ~YRShader() {};

private:
	HRESULT create_vertex(const char* cso_file, ID3D11VertexShader** vert, D3D11_INPUT_ELEMENT_DESC* layout, UINT numElements, ID3D11InputLayout** input);
	HRESULT CreatePixel(const char* ps_file, ID3D11PixelShader** pixel);
	HRESULT create_gs_from_cso(const char* cso_name, ID3D11GeometryShader** geometry_shader);
	HRESULT create_ds_from_cso(const char* cso_name, ID3D11DomainShader** domain_shader);
	HRESULT create_hs_from_cso(const char* cso_name, ID3D11HullShader** hull_shader);

	bool Create_element(const char* VS_cso_file);

public:
	bool Create(const char* VS_cso_file, const char* PS_cso_file);
	bool Create(const char* VS_cso_file, const char* PS_cso_file, const char* GS_cso_file);
	bool Create(const char* VS_cso_file, const char* PS_cso_file, const char* DS_cso_file, const char* HS_cso_file);
	bool Create(const char* VS_cso_file, const char* PS_cso_file, const char* GS_cso_file, const char* DS_cso_file, const char* HS_cso_file);
	
	void Acivate();		//動作
	void Inactivate();	//停止
};
