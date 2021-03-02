#include "framebuffer.h"
#include "framework.h"
#include <assert.h>


//�����_�[�^�[�Q�b�g��S�ď���������
void framebuffer::ResetRenderTargetViews()
{
	ID3D11RenderTargetView* null_render[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];

	for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
	{
		null_render[i] = nullptr;
	}

	ID3D11DepthStencilView* null_depth = nullptr;

	FRAMEWORK.context->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, null_render, null_depth);
}

//�R���X�^���g�o�b�t�@
framebuffer::framebuffer(){}

//�󂯎���������_�[�^�[�Q�b�g�r���[��z��ɒǉ�����
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

//�����_�[�^�[�Q�b�g��ۑ�����z��̒��g������������
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


//�����_�[�^�[�Q�b�g���w�肵���F�ŏ���������
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

//���݃Z�b�g����Ă��郌���_�[�^�[�Q�b�g��ۑ�����
void framebuffer::GetDefaultRTV()
{
	FRAMEWORK.context->OMGetRenderTargets(1, default_render_target_view.ReleaseAndGetAddressOf(), default_depth_stencil_view.ReleaseAndGetAddressOf());
}


//�����_�[�^�[�Q�b�g�r���[���Z�b�g����
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

//�����_�[�^�[�Q�b�g�r���[���Z�b�g����(�r���[�|�[�g�w��)
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

//�����_�[�^�[�Q�b�g�r���[���O��
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

		//render_target_view[0] = default_render_target_view;

		FRAMEWORK.context->OMSetRenderTargets(render_target_view.size(), render_target_view.data()->GetAddressOf(), default_depth_stencil_view.Get());
	}

}

//�O��Z�b�g����Ă��������_�[�^�[�Q�b�g���Z�b�g����
void framebuffer::SetDefaultRTV()
{
	FRAMEWORK.context->RSSetViewports(number_of_stored_viewports, default_viewports);
	FRAMEWORK.context->OMSetRenderTargets(1, default_render_target_view.GetAddressOf(), default_depth_stencil_view.Get());
}

//�r���[�|�[�g��ݒ肷��
void framebuffer::SetViewPort(int width, int height)
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
}