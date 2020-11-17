#pragma comment(lib, "shlwapi.lib")
#include "sprite.h"
#include <tchar.h>
#include<stdio.h>
#include"misc.h"
#include"DirectXTK-master\Inc\WICTextureLoader.h"
#include<map>
#include<wrl.h>
#include<Shlwapi.h>
#include <memory>
#include "framework.h"


namespace TextureALL
{
	//static std::map < std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> > tex;
	HRESULT TextureLoad(ID3D11Device *device, const wchar_t* wchar, D3D11_TEXTURE2D_DESC *texture2d_desc, ID3D11ShaderResourceView **pShader)
	{
		HRESULT hr = S_OK;

		ID3D11Resource *pResouse;
		//std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>::iterator it = tex.find(wchar);
		auto it = tex.find(wchar);
		if (it != tex.end())
		{
			*pShader = it->second.Get();
			(*pShader)->AddRef();
			(*pShader)->GetResource(&pResouse);
		}

		else
		{
			hr= DirectX::CreateWICTextureFromFile(device, wchar, &pResouse, pShader);
			std::wstring extension = PathFindExtensionW(wchar);
			tex.insert(std::make_pair(wchar, *pShader));
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}
		ID3D11Texture2D *texture2d;
		hr = pResouse->QueryInterface(&texture2d);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		texture2d->GetDesc(texture2d_desc);

		texture2d->Release();
		pResouse->Release();

		return hr;
	}

	HRESULT create_vertex_file(ID3D11Device *device, const char *cso_file, ID3D11VertexShader **vert, D3D11_INPUT_ELEMENT_DESC *layout, UINT numElements,ID3D11InputLayout **input)
	{

		struct Vertex_and_Layout
		{
			Vertex_and_Layout(ID3D11VertexShader *vert, ID3D11InputLayout *input) : pVertex(vert), pInput(input) {}
			Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertex;
			Microsoft::WRL::ComPtr<ID3D11InputLayout> pInput;
		};
		static std::map<std::string, Vertex_and_Layout> cache;

		auto it = cache.find(cso_file);
		if (it != cache.end())
		{
			*vert = it->second.pVertex.Get();
			(*vert)->AddRef();
			*input = it->second.pInput.Get();
			(*input)->AddRef();
			return S_OK;
		}


		HRESULT hr = S_OK;
		FILE *fp = 0;

		fopen_s(&fp, cso_file, "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");
		fseek(fp, 0, SEEK_END);
		long cso_sz = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		std::unique_ptr<unsigned char[]>cso_data = std::make_unique< unsigned char[]>(cso_sz);
		fread(cso_data.get(), cso_sz, 1, fp);
		fclose(fp);

		hr = device->CreateVertexShader(cso_data.get(), cso_sz, NULL, vert);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		hr = device->CreateInputLayout(layout, numElements, cso_data.get(),
			cso_sz, input);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		//delete[]cso_data;


		cache.insert(std::make_pair(cso_file, Vertex_and_Layout(*vert, *input)));
		return hr;
	}

	HRESULT CreatePixel_files(ID3D11Device *device,const char *ps_file,ID3D11PixelShader **pixel)
	{
		static std::map < std::string, Microsoft::WRL::ComPtr<ID3D11PixelShader>> pixelcache;

		auto it = pixelcache.find(ps_file);
		if (it != pixelcache.end())
		{
			*pixel = it->second.Get();
			(*pixel)->AddRef();
			return S_OK;
		}

		HRESULT hr = S_OK;

		FILE *fpp = 0;

		fopen_s(&fpp, ps_file, "rb");
		_ASSERT_EXPR_A(fpp, "CSO File not found");
		fseek(fpp, 0, SEEK_END);
		long cso_szp = ftell(fpp);
		fseek(fpp, 0, SEEK_SET);
		std::unique_ptr<unsigned char[]>cso_datap = std::make_unique< unsigned char[]>(cso_szp);
		fread(cso_datap.get(), cso_szp, 1, fpp);
		fclose(fpp);

		hr = device->CreatePixelShader(cso_datap.get(), cso_szp, NULL, pixel);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		//delete[]cso_datap;

		pixelcache.insert(std::make_pair(ps_file, *pixel));

		return hr;
	}
}

void Sprite::Init(const wchar_t* wchar)
{
	vertex vertics[] = {
		{ DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT4(1,1,1,1), DirectX::XMFLOAT2(0,0) },
		{ DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT4(1,1,1,1), DirectX::XMFLOAT2(0,0) },
		{ DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT4(1,1,1,1), DirectX::XMFLOAT2(0,0) },
		{ DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT4(1,1,1,1), DirectX::XMFLOAT2(0,0) },
	};

	HRESULT hr = S_OK;
	//ID3DBlob* pVSBlob = NULL;

	D3D11_BUFFER_DESC bd = {};
	//ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(vertics);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bd.CPUAccessFlags = 0;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA InitData = {};
	//ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertics;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	hr = FRAMEWORK.device->CreateBuffer(&bd, &InitData, buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	/*D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0 , DXGI_FORMAT_R32G32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	TextureALL::create_vertex_file(FRAMEWORK.device.Get(), "./Data/Shader/sprite_vs.cso", vert.GetAddressOf(), layout, numElements, input.GetAddressOf());

	TextureALL::CreatePixel_files(FRAMEWORK.device.Get(), "./Data/Shader/sprite_ps.cso", pixel.GetAddressOf());*/

	D3D11_RASTERIZER_DESC pRaster = {};
	pRaster.FillMode = D3D11_FILL_SOLID;
	pRaster.CullMode = D3D11_CULL_NONE;
	pRaster.FrontCounterClockwise = FALSE;
	pRaster.DepthBias = 0;
	pRaster.DepthBiasClamp = 0;
	pRaster.SlopeScaledDepthBias = 0;
	pRaster.DepthClipEnable = FALSE;
	pRaster.ScissorEnable = FALSE;
	pRaster.MultisampleEnable = FALSE;
	pRaster.AntialiasedLineEnable = FALSE;

	hr = FRAMEWORK.device->CreateRasterizerState(&pRaster, rastersize.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//ID3D11ShaderResourceView *shaderview;

	//.pushback等を使って一度ロードしたものを記録していき、記録になければ新しく記録していく形にする
	//hr = DirectX::CreateWICTextureFromFile(device, wchar, &pResouse, &shader);
	TextureALL::TextureLoad(FRAMEWORK.device.Get(), wchar, &texture2d_desc, shader.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));



	D3D11_SAMPLER_DESC sample;
	//ID3D11SamplerState *samplestate;

	sample.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sample.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sample.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sample.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sample.MipLODBias = 0.0f;
	sample.MaxAnisotropy = 16;
	sample.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	DirectX::XMFLOAT4 bColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	memcpy(sample.BorderColor, &bColor, sizeof(DirectX::XMFLOAT4));
	sample.MinLOD = 0;
	sample.MaxLOD = D3D11_FLOAT32_MAX;


	/*FLOAT color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	sample.BorderColor[0] = color[0];
	sample.BorderColor[1] = color[1];
	sample.BorderColor[2] = color[2];
	sample.BorderColor[3] = color[3];*/


	hr = FRAMEWORK.device->CreateSamplerState(&sample, sampler.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_DEPTH_STENCIL_DESC depth_desc;

	depth_desc.DepthEnable = FALSE;
	depth_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_desc.DepthFunc = D3D11_COMPARISON_LESS;
	depth_desc.StencilEnable = FALSE;
	depth_desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depth_desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depth_desc.FrontFace.StencilFunc = depth_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth_desc.FrontFace.StencilDepthFailOp = depth_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_desc.FrontFace.StencilPassOp = depth_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_desc.FrontFace.StencilFailOp = depth_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;


	hr = FRAMEWORK.device->CreateDepthStencilState(&depth_desc, depthstate.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	/*D3D11_BLEND_DESC blend_desc;

	blend_desc.AlphaToCoverageEnable = TRUE;
	blend_desc.IndependentBlendEnable = FALSE;
	blend_desc.RenderTarget[0].BlendEnable = FALSE;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;*/

	//hr = device->CreateBlendState(&blend_desc, &blendstate);

	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

Sprite::Sprite()
{
	vertex_tex vertics[] = {
		{ DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT2(0,0),DirectX::XMFLOAT4(1,1,1,1)},
		{ DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT2(0,0),DirectX::XMFLOAT4(1,1,1,1)},
		{ DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT2(0,0),DirectX::XMFLOAT4(1,1,1,1)},
		{ DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT3(0,0,0),DirectX::XMFLOAT2(0,0),DirectX::XMFLOAT4(1,1,1,1)},
	};

	HRESULT hr = S_OK;
	//ID3DBlob* pVSBlob = NULL;

	D3D11_BUFFER_DESC bd = {};
	//ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(vertics);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bd.CPUAccessFlags = 0;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA InitData = {};
	//ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertics;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	hr = FRAMEWORK.device->CreateBuffer(&bd, &InitData, buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	/*D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0 , DXGI_FORMAT_R32G32_FLOAT , 0 , D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	TextureALL::create_vertex_file(FRAMEWORK.device.Get(), "./Data/Shader/sprite_vs.cso", vert.GetAddressOf(), layout, numElements, input.GetAddressOf());

	TextureALL::CreatePixel_files(FRAMEWORK.device.Get(), "./Data/Shader/sprite_ps.cso", pixel.GetAddressOf());*/

	D3D11_RASTERIZER_DESC pRaster = {};
	pRaster.FillMode = D3D11_FILL_SOLID;
	pRaster.CullMode = D3D11_CULL_NONE;
	pRaster.FrontCounterClockwise = FALSE;
	pRaster.DepthBias = 0;
	pRaster.DepthBiasClamp = 0;
	pRaster.SlopeScaledDepthBias = 0;
	pRaster.DepthClipEnable = FALSE;
	pRaster.ScissorEnable = FALSE;
	pRaster.MultisampleEnable = FALSE;
	pRaster.AntialiasedLineEnable = FALSE;

	hr = FRAMEWORK.device->CreateRasterizerState(&pRaster, rastersize.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));



	D3D11_SAMPLER_DESC sample;
	//ID3D11SamplerState *samplestate;

	sample.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sample.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sample.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sample.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sample.MipLODBias = 0.0f;
	sample.MaxAnisotropy = 16;
	sample.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	DirectX::XMFLOAT4 bColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	memcpy(sample.BorderColor, &bColor, sizeof(DirectX::XMFLOAT4));
	sample.MinLOD = 0;
	sample.MaxLOD = D3D11_FLOAT32_MAX;


	/*FLOAT color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	sample.BorderColor[0] = color[0];
	sample.BorderColor[1] = color[1];
	sample.BorderColor[2] = color[2];
	sample.BorderColor[3] = color[3];*/


	hr = FRAMEWORK.device->CreateSamplerState(&sample, sampler.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_DEPTH_STENCIL_DESC depth_desc;

	depth_desc.DepthEnable = FALSE;
	depth_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_desc.DepthFunc = D3D11_COMPARISON_LESS;
	depth_desc.StencilEnable = FALSE;
	depth_desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depth_desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depth_desc.FrontFace.StencilFunc = depth_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth_desc.FrontFace.StencilDepthFailOp = depth_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_desc.FrontFace.StencilPassOp = depth_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_desc.FrontFace.StencilFailOp = depth_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;


	hr = FRAMEWORK.device->CreateDepthStencilState(&depth_desc, depthstate.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	/*D3D11_BLEND_DESC blend_desc;

	blend_desc.AlphaToCoverageEnable = TRUE;
	blend_desc.IndependentBlendEnable = FALSE;
	blend_desc.RenderTarget[0].BlendEnable = FALSE;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;*/

	//hr = device->CreateBlendState(&blend_desc, &blendstate);

	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}


Sprite::Sprite( const wchar_t* wchar)
{
	Init(wchar);
}


Sprite::Sprite(const wchar_t* wchar, float sw, float sh)
{
	Init(wchar);
	LoadGraph(sw, sh);
}

Sprite::Sprite(const wchar_t* wchar, float sw, float sh, int numX, int numY, float n_x, float n_y)
{
	Init(wchar);
	LoadDivGraph(sw, sh, numX, numY, n_x, n_y);
}

Sprite::~Sprite()
{
	/*vert->Release();
	pixel->Release();
	input->Release();
	buffer->Release();
	rastersize->Release();
	shader->Release();
	sampler->Release();
	depthstate->Release();*/
	//blendstate->Release();
}

void Sprite::render(YRShader* shader, float dx,float dy,float dw,float dh,float sx,float sy,float sw,float sh,float angle,float r,float g,float b,float a)
{
	D3D11_VIEWPORT viewport;
	UINT num_viewport = 1;
	FRAMEWORK.context->RSGetViewports(&num_viewport, &viewport);
	float screen_width = viewport.Width;
	float screen_height = viewport.Height;

	//screen
	//左上座標
	float x0 = dx;
	float y0 = dy;

	//右上座標
	float x1 = dx + dw;
	float y1 = dy;

	//左下座標
	float x2 = dx;
	float y2 = dy + dh;

	//右下座標
	float x3 = dx + dw;
	float y3 = dy + dh;



	//texture
	//lefttop
	//float sx0 = sx;
	//float sy0 = sy;

	////righttop
	//float sx1 = sx + sw;
	//float sy1 = sy;

	////leftbottom
	//float sx2 = sx;
	//float sy2 = sy + sh;

	////rightbottom
	//float sx3 = sx + sw;
	//float sy3 = sy + sh;


	//screen
	float mx = dx + dw*0.5f;
	float my = dy + dh*0.5f;
	x0 -= mx;
	y0 -= my;
	x1 -= mx;
	y1 -= my;
	x2 -= mx;
	y2 -= my;
	x3 -= mx;
	y3 -= my;


	//texture
	/*float smx = sx + sw*0.5f;
	float smy = sy + sh*0.5f;
	sx0 -= smx;
	sy0 -= smy;
	sx1 -= smx;
	sy1 -= smy;
	sx2 -= smx;
	sy2 -= smy;
	sx3 -= smx;
	sy3 -= smy;
	*/


	float rx, ry;
	float cos = cosf(angle*0.01745f);
	float sin = sinf(angle*0.01745f);
	rx = x0;
	ry = y0;
	x0 = cos*rx + -sin*ry;
	y0 = sin*rx + cos*ry;
	rx = x1;
	ry = y1;
	x1 = cos*rx + -sin*ry;
	y1 = sin*rx + cos*ry;
	rx = x2;
	ry = y2;
	x2 = cos*rx + -sin*ry;
	y2 = sin*rx + cos*ry;
	rx = x3;
	ry = y3;
	x3 = cos*rx + -sin*ry;
	y3 = sin*rx + cos*ry;

	x0 += mx;
	y0 += my;
	x1 += mx;
	y1 += my;
	x2 += mx;
	y2 += my;
	x3 += mx;
	y3 += my;


	//screen
	x0 = 2.0f*x0 / screen_width - 1.0f;
	y0 = 1.0f - 2.0f*y0 / screen_height;
	x1 = 2.0f*x1 / screen_width - 1.0f;
	y1 = 1.0f - 2.0f*y1 / screen_height;
	x2 = 2.0f*x2 / screen_width - 1.0f;
	y2 = 1.0f - 2.0f*y2 / screen_height;
	x3 = 2.0f*x3 / screen_width - 1.0f;
	y3 = 1.0f - 2.0f*y3 / screen_height;

	//texture
	/*sx0 = 2.0f*sx0 / screen_width - 1.0f;
	sy0 = 1.0f - 2.0f*sy0 / screen_height;
	sx1 = 2.0f*sx1 / screen_width - 1.0f;
	sy1 = 1.0f - 2.0f*sy1 / screen_height;
	sx2 = 2.0f*sx2 / screen_width - 1.0f;
	sy2 = 1.0f - 2.0f*sy2 / screen_height;
	sx3 = 2.0f*sx3 / screen_width - 1.0f;
	sy3 = 1.0f - 2.0f*sy3 / screen_height;*/

	

	HRESULT hr = S_OK;
	D3D11_MAP maptype = D3D11_MAP_WRITE_DISCARD;
	//ID3D11Resource *resouse;
	/*UINT mapflug;*/
	D3D11_MAPPED_SUBRESOURCE mapsub;
	
	hr= FRAMEWORK.context->Map(buffer.Get(), 0,  maptype,0, &mapsub);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	
	vertex *vertics = static_cast<vertex *>(mapsub.pData);
	vertics[0].position.x = x0;
	vertics[0].position.y = y0;
	vertics[1].position.x = x1;
	vertics[1].position.y = y1;
	vertics[2].position.x = x2;
	vertics[2].position.y = y2;
	vertics[3].position.x = x3;
	vertics[3].position.y = y3;
	vertics[0].position.z = vertics[1].position.z = vertics[2].position.z = vertics[3].position.z = 0.0f;
	
	DirectX::XMFLOAT4 color(r, g, b,a);
	vertics[0].color = vertics[1].color = vertics[2].color=vertics[3].color=color;

	vertics[0].texcord.x = static_cast<FLOAT>(sx) / texture2d_desc.Width;
	vertics[0].texcord.y = static_cast<FLOAT>(sy) / texture2d_desc.Height;
	vertics[1].texcord.x = static_cast<FLOAT>(sx+sw) / texture2d_desc.Width;
	vertics[1].texcord.y = static_cast<FLOAT>(sy) / texture2d_desc.Height;
	vertics[2].texcord.x = static_cast<FLOAT>(sx) / texture2d_desc.Width;
	vertics[2].texcord.y = static_cast<FLOAT>(sy + sh) / texture2d_desc.Height;
	vertics[3].texcord.x = static_cast<FLOAT>(sx+sw) / texture2d_desc.Width;
	vertics[3].texcord.y = static_cast<FLOAT>(sy + sh) / texture2d_desc.Height;
	FRAMEWORK.context->Unmap(buffer.Get(), 0);
	
	UINT stencil=1;

	FRAMEWORK.context->OMSetDepthStencilState(depthstate.Get(), stencil);

	//context->OMSetBlendState(blendstate,NULL, 0xffffffff);

	UINT stride = sizeof(vertex);
	UINT offset = 0;
	FRAMEWORK.context->IASetVertexBuffers(0, 1, buffer.GetAddressOf(), &stride, &offset);
	FRAMEWORK.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	FRAMEWORK.context->RSSetState(rastersize.Get());

	/*FRAMEWORK.context->IASetInputLayout(input.Get());
	FRAMEWORK.context->VSSetShader(vert.Get(), NULL, 0);
	FRAMEWORK.context->PSSetShader(pixel.Get(), NULL, 0);*/

	shader->Acivate();
	FRAMEWORK.context->PSSetShaderResources(0, 1, this->shader.GetAddressOf());
	FRAMEWORK.context->PSSetSamplers(0, 1, sampler.GetAddressOf());

	FRAMEWORK.context->Draw(4, 0);

	shader->Inactivate();
}

void Sprite::render(
	YRShader* shader,
	Texture* tex,
	float	dx, float	dy,
	float	dw, float	dh,
	float	sx, float	sy,
	float	sw, float	sh,
	float		angle,
	float		alpha
)
{
	D3D11_VIEWPORT vp;
	UINT num = 1;

	FRAMEWORK.context->RSGetViewports(&num, &vp);

	//float vw = static_cast<float>(FRAMEWORK.SCREEN_WIDTH);
	//float vh = static_cast<float>(FRAMEWORK.SCREEN_HEIGHT);
	float screen_width = vp.Width;
	float screen_height = vp.Height;

	//screen
	//左上座標
	float x0 = dx;
	float y0 = dy;

	//右上座標
	float x1 = dx + dw;
	float y1 = dy;

	//左下座標
	float x2 = dx;
	float y2 = dy + dh;

	//右下座標
	float x3 = dx + dw;
	float y3 = dy + dh;

	//screen
	float mx = dx + dw * 0.5f;
	float my = dy + dh * 0.5f;
	x0 -= mx;
	y0 -= my;
	x1 -= mx;
	y1 -= my;
	x2 -= mx;
	y2 -= my;
	x3 -= mx;
	y3 -= my;


	float rx, ry;
	float cos = cosf(angle * 0.01745f);
	float sin = sinf(angle * 0.01745f);
	rx = x0;
	ry = y0;
	x0 = cos * rx + -sin * ry;
	y0 = sin * rx + cos * ry;
	rx = x1;
	ry = y1;
	x1 = cos * rx + -sin * ry;
	y1 = sin * rx + cos * ry;
	rx = x2;
	ry = y2;
	x2 = cos * rx + -sin * ry;
	y2 = sin * rx + cos * ry;
	rx = x3;
	ry = y3;
	x3 = cos * rx + -sin * ry;
	y3 = sin * rx + cos * ry;

	x0 += mx;
	y0 += my;
	x1 += mx;
	y1 += my;
	x2 += mx;
	y2 += my;
	x3 += mx;
	y3 += my;


	//screen
	x0 = 2.0f * x0 / screen_width - 1.0f;
	y0 = 1.0f - 2.0f * y0 / screen_height;
	x1 = 2.0f * x1 / screen_width - 1.0f;
	y1 = 1.0f - 2.0f * y1 / screen_height;
	x2 = 2.0f * x2 / screen_width - 1.0f;
	y2 = 1.0f - 2.0f * y2 / screen_height;
	x3 = 2.0f * x3 / screen_width - 1.0f;
	y3 = 1.0f - 2.0f * y3 / screen_height;


	HRESULT hr = S_OK;
	D3D11_MAP maptype = D3D11_MAP_WRITE_DISCARD;
	D3D11_MAPPED_SUBRESOURCE mapsub;

	hr = FRAMEWORK.context->Map(buffer.Get(), 0, maptype, 0, &mapsub);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//頂点データ設定
	vertex_tex* data = static_cast<vertex_tex*>(mapsub.pData);

	data[0].Pos.x = x0;
	data[0].Pos.y = y0;
	data[0].Pos.z = 0.0f;

	data[1].Pos.x = x1;
	data[1].Pos.y = y1;
	data[1].Pos.z = 0.0f;

	data[2].Pos.x = x2;
	data[2].Pos.y = y2;
	data[2].Pos.z = 0.0f;

	data[3].Pos.x = x3;
	data[3].Pos.y = y3;
	data[3].Pos.z = 0.0f;
	////回転の中心
	//float workPosX = dx + dw * 0.5f;
	//float workPosY = dy + dh * 0.5f;

	////回転処理
	//for (int i = 0; i < 4; i++) {

	//	float workX = data[i].Pos.x - workPosX;
	//	float workY = data[i].Pos.y - workPosY;
	//	data[i].Pos.x = workX * cosf(angle) - workY * sinf(angle) + workPosX;
	//	data[i].Pos.y = workX * sinf(angle) + workY * cosf(angle) + workPosY;
	//	data[i].Pos.z = 0.0f;
	//}



	// 正規化デバイス座標系

	/*for (int i = 0; i < 4; i++) {
		data[i].Pos.x = 2.0f * data[i].Pos.x / screen_width - 1.0f;
		data[i].Pos.y = 1.0f - 2.0f * data[i].Pos.y / screen_height;
		data[i].Pos.z = 0.0f;
	}*/


	//テクスチャ座標設定
	data[0].Tex.x = static_cast<FLOAT>(sx) / tex->GetWidth();
	data[0].Tex.y = static_cast<FLOAT>(sy) / tex->GetHeight();
	data[1].Tex.x = static_cast<FLOAT>(sx + sw) / tex->GetWidth();
	data[1].Tex.y = static_cast<FLOAT>(sy) / tex->GetHeight();
	data[2].Tex.x = static_cast<FLOAT>(sx) / tex->GetWidth();
	data[2].Tex.y = static_cast<FLOAT>(sy + sh) / tex->GetHeight();
	data[3].Tex.x = static_cast<FLOAT>(sx + sw) / tex->GetWidth();
	data[3].Tex.y = static_cast<FLOAT>(sy + sh) / tex->GetHeight();
	FRAMEWORK.context->Unmap(buffer.Get(), 0);


	//UV座標
	/*for (int i = 0; i < 4; i++) {
		data[i].Tex.x = data[i].Tex.x / tex->GetWidth();
		data[i].Tex.y = data[i].Tex.y / tex->GetHeight();
	}*/
	//頂点カラー
	data[0].Color = XMFLOAT4(1, 1, 1, alpha);
	data[1].Color = XMFLOAT4(1, 1, 1, alpha);
	data[2].Color = XMFLOAT4(1, 1, 1, alpha);
	data[3].Color = XMFLOAT4(1, 1, 1, alpha);
	//法線
	data[0].Normal = XMFLOAT3(0, 0, 1);
	data[1].Normal = XMFLOAT3(0, 0, 1);
	data[2].Normal = XMFLOAT3(0, 0, 1);
	data[3].Normal = XMFLOAT3(0, 0, 1);
	
	shader->Acivate();
	//頂点データ更新
	//FRAMEWORK.context->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);

	//	頂点バッファの指定
	UINT stride = sizeof(vertex_tex);
	UINT offset = 0;
	FRAMEWORK.context->IASetVertexBuffers(
		0, 1, buffer.GetAddressOf(), // スロット, 数, バッファ
		&stride,		// １頂点のサイズ
		&offset			// 開始位置
	);
	FRAMEWORK.context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	);
	FRAMEWORK.context->OMSetDepthStencilState(depthstate.Get(), 1);
	FRAMEWORK.context->RSSetState(rastersize.Get());
	//FRAMEWORK.context->PSSetShaderResources(0, 1, this->shader.GetAddressOf());
	//FRAMEWORK.context->PSSetSamplers(0, 1, sampler.GetAddressOf());
	//テクスチャの設定
	if (tex) tex->Set(0);

	FRAMEWORK.context->Draw(4, 0);
	//シェーダ無効化
	shader->Inactivate();

	if (tex) tex->Set(0, FALSE);
}


void Sprite::render(
	YRShader* shader,
	Texture* tex0,
	Title_CBuffer cbuffer_param,
	Sampler* sampler_clamp,
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer,
	float	dx, float	dy,
	float	dw, float	dh,
	float	sx, float	sy,
	float	sw, float	sh,
	float		angle,
	float		alpha
)
{
	D3D11_VIEWPORT vp;
	UINT num = 1;

	FRAMEWORK.context->RSGetViewports(&num, &vp);

	//float vw = static_cast<float>(FRAMEWORK.SCREEN_WIDTH);
	//float vh = static_cast<float>(FRAMEWORK.SCREEN_HEIGHT);
	float screen_width = vp.Width;
	float screen_height = vp.Height;

	//screen
	//左上座標
	float x0 = dx;
	float y0 = dy;

	//右上座標
	float x1 = dx + dw;
	float y1 = dy;

	//左下座標
	float x2 = dx;
	float y2 = dy + dh;

	//右下座標
	float x3 = dx + dw;
	float y3 = dy + dh;

	//screen
	float mx = dx + dw * 0.5f;
	float my = dy + dh * 0.5f;
	x0 -= mx;
	y0 -= my;
	x1 -= mx;
	y1 -= my;
	x2 -= mx;
	y2 -= my;
	x3 -= mx;
	y3 -= my;


	float rx, ry;
	float cos = cosf(angle * 0.01745f);
	float sin = sinf(angle * 0.01745f);
	rx = x0;
	ry = y0;
	x0 = cos * rx + -sin * ry;
	y0 = sin * rx + cos * ry;
	rx = x1;
	ry = y1;
	x1 = cos * rx + -sin * ry;
	y1 = sin * rx + cos * ry;
	rx = x2;
	ry = y2;
	x2 = cos * rx + -sin * ry;
	y2 = sin * rx + cos * ry;
	rx = x3;
	ry = y3;
	x3 = cos * rx + -sin * ry;
	y3 = sin * rx + cos * ry;

	x0 += mx;
	y0 += my;
	x1 += mx;
	y1 += my;
	x2 += mx;
	y2 += my;
	x3 += mx;
	y3 += my;


	//screen
	x0 = 2.0f * x0 / screen_width - 1.0f;
	y0 = 1.0f - 2.0f * y0 / screen_height;
	x1 = 2.0f * x1 / screen_width - 1.0f;
	y1 = 1.0f - 2.0f * y1 / screen_height;
	x2 = 2.0f * x2 / screen_width - 1.0f;
	y2 = 1.0f - 2.0f * y2 / screen_height;
	x3 = 2.0f * x3 / screen_width - 1.0f;
	y3 = 1.0f - 2.0f * y3 / screen_height;


	HRESULT hr = S_OK;
	D3D11_MAP maptype = D3D11_MAP_WRITE_DISCARD;
	D3D11_MAPPED_SUBRESOURCE mapsub;

	hr = FRAMEWORK.context->Map(buffer.Get(), 0, maptype, 0, &mapsub);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//頂点データ設定
	vertex_tex* data = static_cast<vertex_tex*>(mapsub.pData);

	data[0].Pos.x = x0;
	data[0].Pos.y = y0;
	data[0].Pos.z = 0.0f;

	data[1].Pos.x = x1;
	data[1].Pos.y = y1;
	data[1].Pos.z = 0.0f;

	data[2].Pos.x = x2;
	data[2].Pos.y = y2;
	data[2].Pos.z = 0.0f;

	data[3].Pos.x = x3;
	data[3].Pos.y = y3;
	data[3].Pos.z = 0.0f;
	////回転の中心
	//float workPosX = dx + dw * 0.5f;
	//float workPosY = dy + dh * 0.5f;

	////回転処理
	//for (int i = 0; i < 4; i++) {

	//	float workX = data[i].Pos.x - workPosX;
	//	float workY = data[i].Pos.y - workPosY;
	//	data[i].Pos.x = workX * cosf(angle) - workY * sinf(angle) + workPosX;
	//	data[i].Pos.y = workX * sinf(angle) + workY * cosf(angle) + workPosY;
	//	data[i].Pos.z = 0.0f;
	//}



	// 正規化デバイス座標系

	/*for (int i = 0; i < 4; i++) {
		data[i].Pos.x = 2.0f * data[i].Pos.x / screen_width - 1.0f;
		data[i].Pos.y = 1.0f - 2.0f * data[i].Pos.y / screen_height;
		data[i].Pos.z = 0.0f;
	}*/


	//テクスチャ座標設定
	data[0].Tex.x = static_cast<FLOAT>(sx) / tex0->GetWidth();
	data[0].Tex.y = static_cast<FLOAT>(sy) / tex0->GetHeight();
	data[1].Tex.x = static_cast<FLOAT>(sx + sw) / tex0->GetWidth();
	data[1].Tex.y = static_cast<FLOAT>(sy) / tex0->GetHeight();
	data[2].Tex.x = static_cast<FLOAT>(sx) / tex0->GetWidth();
	data[2].Tex.y = static_cast<FLOAT>(sy + sh) / tex0->GetHeight();
	data[3].Tex.x = static_cast<FLOAT>(sx + sw) / tex0->GetWidth();
	data[3].Tex.y = static_cast<FLOAT>(sy + sh) / tex0->GetHeight();
	FRAMEWORK.context->Unmap(buffer.Get(), 0);


	//UV座標
	/*for (int i = 0; i < 4; i++) {
		data[i].Tex.x = data[i].Tex.x / tex->GetWidth();
		data[i].Tex.y = data[i].Tex.y / tex->GetHeight();
	}*/
	//頂点カラー
	data[0].Color = XMFLOAT4(1, 1, 1, alpha);
	data[1].Color = XMFLOAT4(1, 1, 1, alpha);
	data[2].Color = XMFLOAT4(1, 1, 1, alpha);
	data[3].Color = XMFLOAT4(1, 1, 1, alpha);
	//法線
	data[0].Normal = XMFLOAT3(0, 0, 1);
	data[1].Normal = XMFLOAT3(0, 0, 1);
	data[2].Normal = XMFLOAT3(0, 0, 1);
	data[3].Normal = XMFLOAT3(0, 0, 1);

	shader->Acivate();
	//頂点データ更新
	//FRAMEWORK.context->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);

	Title_CBuffer cb;
	cb.Resolution = cbuffer_param.Resolution;
	cb.iTime = cbuffer_param.iTime;
	cb.brightness = cbuffer_param.brightness;
	cb.ray_brightness = cbuffer_param.ray_brightness;
	cb.gamma = cbuffer_param.gamma;
	cb.spot_brightness = cbuffer_param.spot_brightness;
	cb.ray_density = cbuffer_param.ray_density;
	cb.curvature = cbuffer_param.curvature;
	cb.red = cbuffer_param.red;
	cb.green = cbuffer_param.green;
	cb.blue = cbuffer_param.blue;
	cb.material_color.x = cbuffer_param.material_color.x;
	cb.material_color.y = cbuffer_param.material_color.y;
	cb.material_color.z = cbuffer_param.material_color.z;
	cb.material_color.w = cbuffer_param.material_color.w;
	cb.dummy1 = 0.0f;
	cb.dummy2 = 0.0f;
	cb.dummy3 = 0.0f;

	FRAMEWORK.context->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cb, 0, 0);
	FRAMEWORK.context->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

	sampler_clamp->Set(0);

	//	頂点バッファの指定
	UINT stride = sizeof(vertex_tex);
	UINT offset = 0;
	FRAMEWORK.context->IASetVertexBuffers(
		0, 1, buffer.GetAddressOf(), // スロット, 数, バッファ
		&stride,		// １頂点のサイズ
		&offset			// 開始位置
	);
	FRAMEWORK.context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP

	);
	FRAMEWORK.context->OMSetDepthStencilState(depthstate.Get(), 1);
	FRAMEWORK.context->RSSetState(rastersize.Get());
	//FRAMEWORK.context->PSSetShaderResources(0, 1, this->shader.GetAddressOf());
	//FRAMEWORK.context->PSSetSamplers(0, 1, sampler.GetAddressOf());
	//テクスチャの設定
	if (tex0) tex0->Set(0);

	FRAMEWORK.context->Draw(4, 0);
	//シェーダ無効化
	shader->Inactivate();

	if (tex0) tex0->Set(0, FALSE);
}


void Sprite::render(
	YRShader* shader,
	Texture* tex,
	ID3D11ShaderResourceView* const* srv,
	int		srv_size,
	float	dx, float	dy,
	float	dw, float	dh,
	float	sx, float	sy,
	float	sw, float	sh,
	float		angle,
	float		alpha
)
{
	D3D11_VIEWPORT vp;
	UINT num = 1;

	FRAMEWORK.context->RSGetViewports(&num, &vp);

	//float vw = static_cast<float>(FRAMEWORK.SCREEN_WIDTH);
	//float vh = static_cast<float>(FRAMEWORK.SCREEN_HEIGHT);
	float screen_width = vp.Width;
	float screen_height = vp.Height;

	//screen
	//左上座標
	float x0 = dx;
	float y0 = dy;

	//右上座標
	float x1 = dx + dw;
	float y1 = dy;

	//左下座標
	float x2 = dx;
	float y2 = dy + dh;

	//右下座標
	float x3 = dx + dw;
	float y3 = dy + dh;

	//screen
	float mx = dx + dw * 0.5f;
	float my = dy + dh * 0.5f;
	x0 -= mx;
	y0 -= my;
	x1 -= mx;
	y1 -= my;
	x2 -= mx;
	y2 -= my;
	x3 -= mx;
	y3 -= my;


	float rx, ry;
	float cos = cosf(angle * 0.01745f);
	float sin = sinf(angle * 0.01745f);
	rx = x0;
	ry = y0;
	x0 = cos * rx + -sin * ry;
	y0 = sin * rx + cos * ry;
	rx = x1;
	ry = y1;
	x1 = cos * rx + -sin * ry;
	y1 = sin * rx + cos * ry;
	rx = x2;
	ry = y2;
	x2 = cos * rx + -sin * ry;
	y2 = sin * rx + cos * ry;
	rx = x3;
	ry = y3;
	x3 = cos * rx + -sin * ry;
	y3 = sin * rx + cos * ry;

	x0 += mx;
	y0 += my;
	x1 += mx;
	y1 += my;
	x2 += mx;
	y2 += my;
	x3 += mx;
	y3 += my;


	//screen
	x0 = 2.0f * x0 / screen_width - 1.0f;
	y0 = 1.0f - 2.0f * y0 / screen_height;
	x1 = 2.0f * x1 / screen_width - 1.0f;
	y1 = 1.0f - 2.0f * y1 / screen_height;
	x2 = 2.0f * x2 / screen_width - 1.0f;
	y2 = 1.0f - 2.0f * y2 / screen_height;
	x3 = 2.0f * x3 / screen_width - 1.0f;
	y3 = 1.0f - 2.0f * y3 / screen_height;


	HRESULT hr = S_OK;
	D3D11_MAP maptype = D3D11_MAP_WRITE_DISCARD;
	D3D11_MAPPED_SUBRESOURCE mapsub;

	hr = FRAMEWORK.context->Map(buffer.Get(), 0, maptype, 0, &mapsub);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//頂点データ設定
	vertex_tex* data = static_cast<vertex_tex*>(mapsub.pData);

	data[0].Pos.x = x0;
	data[0].Pos.y = y0;
	data[0].Pos.z = 0.0f;

	data[1].Pos.x = x1;
	data[1].Pos.y = y1;
	data[1].Pos.z = 0.0f;

	data[2].Pos.x = x2;
	data[2].Pos.y = y2;
	data[2].Pos.z = 0.0f;

	data[3].Pos.x = x3;
	data[3].Pos.y = y3;
	data[3].Pos.z = 0.0f;

	//テクスチャ座標設定
	data[0].Tex.x = static_cast<FLOAT>(sx) / tex->GetWidth();
	data[0].Tex.y = static_cast<FLOAT>(sy) / tex->GetHeight();
	data[1].Tex.x = static_cast<FLOAT>(sx + sw) / tex->GetWidth();
	data[1].Tex.y = static_cast<FLOAT>(sy) / tex->GetHeight();
	data[2].Tex.x = static_cast<FLOAT>(sx) / tex->GetWidth();
	data[2].Tex.y = static_cast<FLOAT>(sy + sh) / tex->GetHeight();
	data[3].Tex.x = static_cast<FLOAT>(sx + sw) / tex->GetWidth();
	data[3].Tex.y = static_cast<FLOAT>(sy + sh) / tex->GetHeight();
	FRAMEWORK.context->Unmap(buffer.Get(), 0);

	//頂点カラー
	data[0].Color = XMFLOAT4(1, 1, 1, alpha);
	data[1].Color = XMFLOAT4(1, 1, 1, alpha);
	data[2].Color = XMFLOAT4(1, 1, 1, alpha);
	data[3].Color = XMFLOAT4(1, 1, 1, alpha);
	//法線
	data[0].Normal = XMFLOAT3(0, 0, 1);
	data[1].Normal = XMFLOAT3(0, 0, 1);
	data[2].Normal = XMFLOAT3(0, 0, 1);
	data[3].Normal = XMFLOAT3(0, 0, 1);

	shader->Acivate();
	//頂点データ更新
	//FRAMEWORK.context->UpdateSubresource(buffer.Get(), 0, NULL, data, 0, 0);

	//	頂点バッファの指定
	UINT stride = sizeof(vertex_tex);
	UINT offset = 0;
	FRAMEWORK.context->IASetVertexBuffers(
		0, 1, buffer.GetAddressOf(), // スロット, 数, バッファ
		&stride,		// １頂点のサイズ
		&offset			// 開始位置
	);
	FRAMEWORK.context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
	);
	FRAMEWORK.context->OMSetDepthStencilState(depthstate.Get(), 1);
	FRAMEWORK.context->RSSetState(rastersize.Get());
	//FRAMEWORK.context->PSSetShaderResources(0, 1, this->shader.GetAddressOf());
	//FRAMEWORK.context->PSSetSamplers(0, 1, sampler.GetAddressOf());
	FRAMEWORK.context->PSSetShaderResources(0, srv_size, srv);
	//FRAMEWORK.context->PSSetSamplers(0, 1, &sampler.GetAddressOf());

	FRAMEWORK.context->Draw(4, 0);

	// シェーダリソースをクリア.
	ID3D11ShaderResourceView* nullTarget[] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	FRAMEWORK.context->PSSetShaderResources(0, 7, nullTarget);
	//シェーダ無効化
	shader->Inactivate();

	//if (tex) tex->Set(0, FALSE);
}


