#include "framebuffer.h"
#include "framework.h"
#include <assert.h>

framebuffer::framebuffer()
{
	
}

void framebuffer::SetRenderTexture(ID3D11RenderTargetView* rtv, bool solo_rtv)
{
	if (solo_rtv)
	{
		render_target_view_solo = rtv;
	}
	else
	{
		render_target_view.push_back(rtv);
	}
}

void framebuffer::ResetRenderTexture()
{
	if (!render_target_view.empty())
	{
		for (int i = 0; i < render_target_view.size(); i++)
		{
			render_target_view[i].Reset();
		}
	}
	render_target_view_solo.Reset();
	render_target_view.clear();
}

void framebuffer::Clear(float r, float g, float b, float a)
{
	float colour[4] = { r, g, b, a };
	if (!render_target_view.empty())
	{
		for (int i = 0; i < render_target_view.size(); i++)
		{
			FRAMEWORK.context->ClearRenderTargetView(render_target_view[i].Get(), colour);
		}
	}
	if (render_target_view_solo)
	{
		FRAMEWORK.context->ClearRenderTargetView(render_target_view_solo.Get(), colour);
	}
}

void framebuffer::GetDefaultRTV()
{
	FRAMEWORK.context->OMGetRenderTargets(1, default_render_target_view.ReleaseAndGetAddressOf(), default_depth_stencil_view.ReleaseAndGetAddressOf());
}

void framebuffer::Activate(ID3D11DepthStencilView* pDepthStencilView)
{
	if (render_target_view.empty())
	{
		FRAMEWORK.context->OMSetRenderTargets(1, render_target_view_solo.GetAddressOf(), pDepthStencilView);
	}
	else
	{
		FRAMEWORK.context->OMSetRenderTargets(render_target_view.size(), render_target_view.data()->GetAddressOf(), pDepthStencilView);
	}
}


void framebuffer::Activate(int width, int height, ID3D11DepthStencilView* pDepthStencilView)
{
	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	number_of_stored_viewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	FRAMEWORK.context->RSGetViewports(&number_of_stored_viewports, default_viewports);
	FRAMEWORK.context->RSSetViewports(1, &vp);
	
	if (render_target_view.empty())
	{
		FRAMEWORK.context->OMSetRenderTargets(1, render_target_view_solo.GetAddressOf(), pDepthStencilView);
	}
	else
	{
		FRAMEWORK.context->OMSetRenderTargets(render_target_view.size(), render_target_view.data()->GetAddressOf(), pDepthStencilView);
	}
}

void framebuffer:: Deactivate()
{
	if (render_target_view.empty())
	{
		//render_target_view_solo = default_render_target_view;
		//FRAMEWORK.context->OMSetRenderTargets(1, render_target_view_solo.GetAddressOf(), default_depth_stencil_view.Get());
	}
	else
	{
		for (int i = 0; i < render_target_view.size(); i++)
		{
			render_target_view[i] = nullptr;
		}

		render_target_view[0] = default_render_target_view;

		FRAMEWORK.context->OMSetRenderTargets(render_target_view.size(), render_target_view.data()->GetAddressOf(), default_depth_stencil_view.Get());
	}
	FRAMEWORK.context->RSSetViewports(number_of_stored_viewports, default_viewports);
	FRAMEWORK.context->OMSetRenderTargets(1, default_render_target_view.GetAddressOf(), default_depth_stencil_view.Get());
}