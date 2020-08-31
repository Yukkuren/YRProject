#include "Texture.h"
#include "framework.h"
#include"DirectXTK-master\Inc\WICTextureLoader.h"
#include<Shlwapi.h>



Texture::Texture() : ShaderResouceView(nullptr)
{

}

Texture::Texture(const wchar_t* filename)
{
	ShaderResouceView = nullptr;
	Load(filename);
}

Texture::~Texture()
{

}

bool Texture::Load(const wchar_t* filename)
{
	
	
	HRESULT hr = S_OK;

	ID3D11Resource* pResouse;

	//一度読み込んだ画像は記録しておく
	auto it = texAll::tex.find(filename);
	if (it != texAll::tex.end())
	{
		ShaderResouceView = it->second.Get();
		(ShaderResouceView.Get())->AddRef();
		(ShaderResouceView.Get())->GetResource(&pResouse);
	}
	else
	{
		hr = DirectX::CreateWICTextureFromFile(FRAMEWORK.device.Get(), filename, &pResouse, ShaderResouceView.GetAddressOf());
		std::wstring extension = PathFindExtensionW(filename);
		texAll::tex.insert(std::make_pair(filename, ShaderResouceView));
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	ID3D11Texture2D* texture2d;
	hr = pResouse->QueryInterface(&texture2d);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	texture2d->GetDesc(&texture2d_desc);

	texture2d->Release();
	pResouse->Release();
	
	return true;
}

void Texture::Set(UINT slot, BOOL flg)
{
	if (flg == FALSE)
	{
		ID3D11ShaderResourceView* rtv[1] = { NULL };
		ID3D11SamplerState* ss[1] = { NULL };
		FRAMEWORK.context->PSSetShaderResources(slot, 1, rtv);
		FRAMEWORK.context->PSSetSamplers(slot, 1, ss);
		FRAMEWORK.context->DSSetShaderResources(slot, 1, rtv);
		FRAMEWORK.context->DSSetSamplers(slot, 1, ss);
		return;
	}
	if (ShaderResouceView)
	{
		FRAMEWORK.context->PSSetShaderResources(slot, 1, ShaderResouceView.GetAddressOf());
		FRAMEWORK.context->DSSetShaderResources(slot, 1, ShaderResouceView.GetAddressOf());
	}
}

bool Texture::Create(u_int width, u_int height, DXGI_FORMAT format)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
	HRESULT hr = S_OK;
	//テクスチャ作成
	ZeroMemory(&texture2d_desc, sizeof(texture2d_desc));
	texture2d_desc.Width = width;
	texture2d_desc.Height = height;
	texture2d_desc.MipLevels = 1;
	texture2d_desc.ArraySize = 1;
	texture2d_desc.Format = format;
	texture2d_desc.SampleDesc.Count = 1;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture2d_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texture2d_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	
	hr = FRAMEWORK.device->CreateTexture2D(&texture2d_desc, NULL, texture2d.GetAddressOf());
	assert(SUCCEEDED(hr));

	//レンダーターゲットビュー作成
	D3D11_RENDER_TARGET_VIEW_DESC rtvd;
	ZeroMemory(&rtvd, sizeof(rtvd));
	rtvd.Format = format;
	rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvd.Texture2D.MipSlice = 0;
	hr = FRAMEWORK.device->CreateRenderTargetView(texture2d.Get(), &rtvd, RenderTargetView.GetAddressOf());
	assert(SUCCEEDED(hr));

	//シェーダーリソースビュー作成
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.Format = format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = 1;
	hr = FRAMEWORK.device->CreateShaderResourceView(texture2d.Get(), &srvd, ShaderResouceView.GetAddressOf());

	assert(SUCCEEDED(hr));

	return true;
}


bool Texture::CreateDepth(u_int width, u_int height, DXGI_FORMAT format)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture2D;
	HRESULT hr = S_OK;
	//	テクスチャ作成
	ZeroMemory(&texture2d_desc, sizeof(texture2d_desc));
	texture2d_desc.Width = width;
	texture2d_desc.Height = height;
	texture2d_desc.MipLevels = 1;
	texture2d_desc.ArraySize = 1;
	texture2d_desc.Format = format;
	texture2d_desc.SampleDesc.Count = 1;
	texture2d_desc.SampleDesc.Quality = 0;
	texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
	texture2d_desc.CPUAccessFlags = 0;
	texture2d_desc.MiscFlags = 0;
	texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

	hr = FRAMEWORK.device->CreateTexture2D(&texture2d_desc, NULL, Texture2D.GetAddressOf());
	assert(SUCCEEDED(hr));

	// 深度ステンシルビュー設定
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;
	hr = FRAMEWORK.device->CreateDepthStencilView(Texture2D.Get(), &dsvd, DepthStencilView.GetAddressOf());
	assert(SUCCEEDED(hr));

	//	シェーダーリソースビュー作成
	D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	ZeroMemory(&srvd, sizeof(srvd));
	srvd.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = 1;
	hr = FRAMEWORK.device->CreateShaderResourceView(Texture2D.Get(), &srvd, ShaderResouceView.GetAddressOf());

	assert(SUCCEEDED(hr));

	return true;
}
