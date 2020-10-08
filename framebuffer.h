#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <vector>

class framebuffer
{
public:
	framebuffer();
	virtual ~framebuffer() = default;
	framebuffer(framebuffer&) = delete;
	framebuffer& operator=(framebuffer&) = delete;

	std::vector<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>> render_target_view;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view_solo;
	//Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;

	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> render_target_shader_resource_view;
	//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> depth_stencil_shader_resource_view;

	//D3D11_VIEWPORT viewport;

	void SetRenderTexture(ID3D11RenderTargetView* rtv, bool solo_rtv = false);
	void ResetRenderTexture();
	void Clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void Activate(int width, int height, ID3D11DepthStencilView* pDepthStencilView);
	void Activate(ID3D11DepthStencilView* pDepthStencilView);
	void GetDefaultRTV();
	void SetViewPort(int width, int height);
	void Deactivate();
	void SetDefaultRTV();


private:
	unsigned int number_of_stored_viewports;
	D3D11_VIEWPORT default_viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> default_render_target_view;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> default_depth_stencil_view;
};
