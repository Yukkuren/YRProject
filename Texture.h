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
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		RenderTargetView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>		DepthStencilView = nullptr;

	//テクスチャ情報
	D3D11_TEXTURE2D_DESC	texture2d_desc;
	

public:

	Texture();
	Texture(const wchar_t* fbx_filename);
	virtual ~Texture();
	bool Load(const wchar_t* fbx_filename);
	void Set(UINT slot, BOOL flg = TRUE);

	UINT GetWidth() { return texture2d_desc.Width; };
	UINT GetHeight() { return texture2d_desc.Height; };
	bool Create(u_int width, u_int height, DXGI_FORMAT format);

	//深度ステンシルバッファ作成
	bool CreateDepth(u_int width, u_int height, DXGI_FORMAT format = DXGI_FORMAT_R24G8_TYPELESS);

	ID3D11RenderTargetView* GetRenderTargetView() { return RenderTargetView.Get(); };
	ID3D11DepthStencilView* GetDepthStencilView() { return DepthStencilView.Get(); };

};
