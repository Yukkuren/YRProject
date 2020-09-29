#pragma once

//#include <Windows.h>
//#include <directxmath.h>
//#include <d3dcompiler.h>
#include<wrl.h>
#include <d3d11.h>
#include<map>
#include<string>

namespace texAll
{
	static std::map < std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> > tex;
};

class Texture
{
protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	ShaderResouceView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	DepthShaderResouceView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		DepthStencilView = nullptr;

	//テクスチャ情報
	D3D11_TEXTURE2D_DESC	texture2d_desc;
	

public:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		RenderTargetView = nullptr;

	Texture();
	Texture(const wchar_t* filename);
	virtual ~Texture();
	bool Load(const wchar_t* filename);
	void Set(UINT slot, BOOL flg = TRUE);

	UINT GetWidth() const { return texture2d_desc.Width; };//画像の全体横サイズを返す
	UINT GetHeight() const { return texture2d_desc.Height; };//画像の全体縦サイズを返す
	bool Create(u_int width, u_int height, DXGI_FORMAT format);

	//深度ステンシルバッファ作成
	bool CreateDepth(u_int width, u_int height, DXGI_FORMAT format = DXGI_FORMAT_R24G8_TYPELESS);

	ID3D11RenderTargetView* GetRenderTargetView() { return RenderTargetView.Get(); };
	ID3D11DepthStencilView* GetDepthStencilView() { return DepthStencilView.Get(); };
	ID3D11ShaderResourceView* GetShaderResource() { return ShaderResouceView.Get(); };
	ID3D11ShaderResourceView* GetDepthShaderResource() { return DepthShaderResouceView.Get(); };

};
