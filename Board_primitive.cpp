#include<dxgi.h>
#include <d3d11.h>
#include <memory>
#include"Board_primitive.h"
#include "framework.h"
#include<map>

using namespace DirectX;

board_primitive::board_primitive()
{
	texture = std::make_shared<Texture>(L"./Data/Shader/decoi.png");

	HRESULT hr = S_OK;
	//���X�^���C�U�[�X�e�[�g�I�u�W�F�N�g����
	D3D11_RASTERIZER_DESC rasterizer_desc;

	rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	rasterizer_desc.FrontCounterClockwise = FALSE;
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0;
	rasterizer_desc.SlopeScaledDepthBias = 0;
	rasterizer_desc.DepthClipEnable = TRUE;
	rasterizer_desc.ScissorEnable = FALSE;
	rasterizer_desc.MultisampleEnable = FALSE;
	rasterizer_desc.AntialiasedLineEnable = FALSE;

	hr = FRAMEWORK.device->CreateRasterizerState(&rasterizer_desc, line_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	rasterizer_desc.FrontCounterClockwise = FALSE;
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0;
	rasterizer_desc.SlopeScaledDepthBias = 0;
	rasterizer_desc.DepthClipEnable = TRUE;
	rasterizer_desc.ScissorEnable = FALSE;
	rasterizer_desc.MultisampleEnable = FALSE;
	rasterizer_desc.AntialiasedLineEnable = FALSE;
	hr = FRAMEWORK.device->CreateRasterizerState(&rasterizer_desc, filling_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));




	//�[�x�X�e���V���X�e�[�g�I�u�W�F�N�g����
	D3D11_DEPTH_STENCIL_DESC depth_desc;

	depth_desc.DepthEnable = TRUE;
	depth_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_desc.DepthFunc = D3D11_COMPARISON_LESS;
	depth_desc.StencilEnable = FALSE;
	depth_desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depth_desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depth_desc.FrontFace.StencilFunc = depth_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth_desc.FrontFace.StencilDepthFailOp = depth_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_desc.FrontFace.StencilPassOp = depth_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_desc.FrontFace.StencilFailOp = depth_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;


	hr = FRAMEWORK.device->CreateDepthStencilState(&depth_desc, depth_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));



	//���_���Z�b�g

	//position,normal�̏���
	vertex vertics[] = {
		//�O��
		{ XMFLOAT3(-1.0f,1.0f,-1.0f), XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//0
		{ XMFLOAT3(1.0f, 1.0f,-1.0f), XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//1
		{ XMFLOAT3(1.0f,-1.0f,-1.0f), XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//2
		{ XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//3
		//�O�ʂ����ĉE��
		{ XMFLOAT3(1.0f, 1.0f,-1.0f),XMFLOAT3(1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//4
		{ XMFLOAT3(1.0f, 1.0f, 1.0f),XMFLOAT3(1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//5
		{ XMFLOAT3(1.0f,-1.0f, 1.0f),XMFLOAT3(1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//6
		{ XMFLOAT3(1.0f,-1.0f,-1.0f),XMFLOAT3(1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//7
		//�O�ʂ����č���
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//8
		{ XMFLOAT3(-1.0f, 1.0f,-1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//9
		{ XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//10
		{ XMFLOAT3(-1.0f,-1.0f, 1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//11
		//����
		{ XMFLOAT3(1.0f, 1.0f,1.0f),XMFLOAT3(-1.0f,-1.0f,1.0f),XMFLOAT2(0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//12
		{ XMFLOAT3(-1.0f, 1.0f,1.0f),XMFLOAT3(-1.0f,-1.0f,1.0f),XMFLOAT2(1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//13
		{ XMFLOAT3(-1.0f,-1.0f,1.0f),XMFLOAT3(-1.0f,-1.0f,1.0f),XMFLOAT2(1.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//14
		{ XMFLOAT3(1.0f,-1.0f,1.0f),XMFLOAT3(-1.0f,-1.0f, 1.0f),XMFLOAT2(0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//15
		//���
		{ XMFLOAT3(-1.0f,1.0f, 1.0f),XMFLOAT3(-1.0f,1.0f,-1.0f),XMFLOAT2(0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//16
		{ XMFLOAT3(1.0f,1.0f, 1.0f),XMFLOAT3(-1.0f, 1.0f,-1.0f),XMFLOAT2(1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//17
		{ XMFLOAT3(1.0f,1.0f,-1.0f),XMFLOAT3(-1.0f, 1.0f,-1.0f),XMFLOAT2(1.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//18
		{ XMFLOAT3(-1.0f,1.0f,-1.0f),XMFLOAT3(-1.0f,1.0f,-1.0f),XMFLOAT2(0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//19
		//����
		{ XMFLOAT3(1.0f,-1.0f, 1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//20
		{ XMFLOAT3(-1.0f,-1.0f, 1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//21
		{ XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//22
		{ XMFLOAT3(1.0f,-1.0f,-1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//23
	};
	u_int indices[] = {
		//���v���ŕ\��
		//�O��
		0, 1, 2,
		2, 3, 0,
		//�O�ʂ����ĉE��
		4, 5, 6,
		6, 7, 4,
		//�O�ʂ����č���
		8, 9, 10,
		10, 11, 8,
		//����
		12,13,14,
		14,15,12,
		//���
		16,17,18,
		18,19,16,
		//����
		20,21,22,
		22,23,20,
	};

	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.ByteWidth = sizeof(vertics);
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;// D3D11_USAGE_DYNAMIC;
	buffer_desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subresource = {};
	subresource.pSysMem = vertics;
	subresource.SysMemPitch = 0;
	subresource.SysMemSlicePitch = 0;

	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, vertex_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//�C���f�b�N�X���Z�b�g
	ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&subresource, sizeof(D3D11_SUBRESOURCE_DATA));
	buffer_desc.ByteWidth = sizeof(indices);
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;// D3D11_USAGE_DYNAMIC;
	buffer_desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	subresource.pSysMem = indices;
	subresource.SysMemPitch = 0;
	subresource.SysMemSlicePitch = 0;

	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, index_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&subresource, sizeof(D3D11_SUBRESOURCE_DATA));
	buffer_desc.ByteWidth = sizeof(cbuffer);
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;/*
	subresource.pSysMem = constant_buffer;
	subresource.SysMemPitch = 0;
	subresource.SysMemSlicePitch = 0;*/

	//subresource.pSysMem = NULL;

	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, nullptr/*&subresource*/, constant_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, nullptr/*&subresource*/, cbuffer_off.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

}


board_primitive::board_primitive(std::shared_ptr<Texture> tex)
{
	HRESULT hr = S_OK;
	//���X�^���C�U�[�X�e�[�g�I�u�W�F�N�g����
	D3D11_RASTERIZER_DESC rasterizer_desc;

	rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	rasterizer_desc.FrontCounterClockwise = FALSE;
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0;
	rasterizer_desc.SlopeScaledDepthBias = 0;
	rasterizer_desc.DepthClipEnable = TRUE;
	rasterizer_desc.ScissorEnable = FALSE;
	rasterizer_desc.MultisampleEnable = FALSE;
	rasterizer_desc.AntialiasedLineEnable = FALSE;

	hr = FRAMEWORK.device->CreateRasterizerState(&rasterizer_desc, line_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	rasterizer_desc.FrontCounterClockwise = FALSE;
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0;
	rasterizer_desc.SlopeScaledDepthBias = 0;
	rasterizer_desc.DepthClipEnable = TRUE;
	rasterizer_desc.ScissorEnable = FALSE;
	rasterizer_desc.MultisampleEnable = FALSE;
	rasterizer_desc.AntialiasedLineEnable = FALSE;
	hr = FRAMEWORK.device->CreateRasterizerState(&rasterizer_desc, filling_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));




	//�[�x�X�e���V���X�e�[�g�I�u�W�F�N�g����
	D3D11_DEPTH_STENCIL_DESC depth_desc;

	depth_desc.DepthEnable = TRUE;
	depth_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_desc.DepthFunc = D3D11_COMPARISON_LESS;
	depth_desc.StencilEnable = FALSE;
	depth_desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depth_desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depth_desc.FrontFace.StencilFunc = depth_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth_desc.FrontFace.StencilDepthFailOp = depth_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_desc.FrontFace.StencilPassOp = depth_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_desc.FrontFace.StencilFailOp = depth_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;


	hr = FRAMEWORK.device->CreateDepthStencilState(&depth_desc, depth_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));



	//���_���Z�b�g

	//position,normal�̏���
	vertex vertics[] = {
		//�O��
		{ XMFLOAT3(-1.0f,1.0f,-1.0f), XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//0
		{ XMFLOAT3(1.0f, 1.0f,-1.0f), XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//1
		{ XMFLOAT3(1.0f,-1.0f,-1.0f), XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//2
		{ XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//3
		//�O�ʂ����ĉE��
		{ XMFLOAT3(1.0f, 1.0f,-1.0f),XMFLOAT3(1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//4
		{ XMFLOAT3(1.0f, 1.0f, 1.0f),XMFLOAT3(1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//5
		{ XMFLOAT3(1.0f,-1.0f, 1.0f),XMFLOAT3(1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//6
		{ XMFLOAT3(1.0f,-1.0f,-1.0f),XMFLOAT3(1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//7
		//�O�ʂ����č���
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//8
		{ XMFLOAT3(-1.0f, 1.0f,-1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//9
		{ XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//10
		{ XMFLOAT3(-1.0f,-1.0f, 1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//11
		//����
		{ XMFLOAT3( 1.0f, 1.0f,1.0f),XMFLOAT3(-1.0f,-1.0f,1.0f),XMFLOAT2(0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//12
		{ XMFLOAT3(-1.0f, 1.0f,1.0f),XMFLOAT3(-1.0f,-1.0f,1.0f),XMFLOAT2(1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//13
		{ XMFLOAT3(-1.0f,-1.0f,1.0f),XMFLOAT3(-1.0f,-1.0f,1.0f),XMFLOAT2(1.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//14
		{ XMFLOAT3(1.0f,-1.0f,1.0f),XMFLOAT3(-1.0f,-1.0f, 1.0f),XMFLOAT2(0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//15
		//���
		{ XMFLOAT3(-1.0f,1.0f, 1.0f),XMFLOAT3(-1.0f,1.0f,-1.0f),XMFLOAT2(0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//16
		{ XMFLOAT3(1.0f,1.0f, 1.0f),XMFLOAT3(-1.0f, 1.0f,-1.0f),XMFLOAT2(1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//17
		{ XMFLOAT3(1.0f,1.0f,-1.0f),XMFLOAT3(-1.0f, 1.0f,-1.0f),XMFLOAT2(1.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//18
		{ XMFLOAT3(-1.0f,1.0f,-1.0f),XMFLOAT3(-1.0f,1.0f,-1.0f),XMFLOAT2(0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//19
		//����
		{ XMFLOAT3( 1.0f,-1.0f, 1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//20
		{ XMFLOAT3(-1.0f,-1.0f, 1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//21
		{ XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//22
		{ XMFLOAT3( 1.0f,-1.0f,-1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//23
	};
	u_int indices[] = {
		//���v���ŕ\��
		//�O��
		0, 1, 2,
		2, 3, 0,
		//�O�ʂ����ĉE��
		4, 5, 6,
		6, 7, 4,
		//�O�ʂ����č���
		8, 9, 10,
		10, 11, 8,
		//����
		12,13,14,
		14,15,12,
		//���
		16,17,18,
		18,19,16,
		//����
		20,21,22,
		22,23,20,
	};

	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.ByteWidth = sizeof(vertics);
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;// D3D11_USAGE_DYNAMIC;
	buffer_desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subresource = {};
	subresource.pSysMem = vertics;
	subresource.SysMemPitch = 0;
	subresource.SysMemSlicePitch = 0;

	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, vertex_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));




	//�C���f�b�N�X���Z�b�g
	ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&subresource, sizeof(D3D11_SUBRESOURCE_DATA));
	buffer_desc.ByteWidth = sizeof(indices);
	buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;// D3D11_USAGE_DYNAMIC;
	buffer_desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	subresource.pSysMem = indices;
	subresource.SysMemPitch = 0;
	subresource.SysMemSlicePitch = 0;

	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, index_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	texture = tex;

	ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&subresource, sizeof(D3D11_SUBRESOURCE_DATA));
	buffer_desc.ByteWidth = sizeof(cbuffer);
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;/*
	subresource.pSysMem = constant_buffer;
	subresource.SysMemPitch = 0;
	subresource.SysMemSlicePitch = 0;*/

	//subresource.pSysMem = NULL;

	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, nullptr/*&subresource*/, constant_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, nullptr/*&subresource*/, cbuffer_off.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

}



board_primitive::board_primitive(const wchar_t* filename)
{
	HRESULT hr = S_OK;
	//���X�^���C�U�[�X�e�[�g�I�u�W�F�N�g����
	D3D11_RASTERIZER_DESC rasterizer_desc;

	rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	rasterizer_desc.FrontCounterClockwise = FALSE;
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0;
	rasterizer_desc.SlopeScaledDepthBias = 0;
	rasterizer_desc.DepthClipEnable = TRUE;
	rasterizer_desc.ScissorEnable = FALSE;
	rasterizer_desc.MultisampleEnable = FALSE;
	rasterizer_desc.AntialiasedLineEnable = FALSE;

	hr = FRAMEWORK.device->CreateRasterizerState(&rasterizer_desc, line_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	rasterizer_desc.FrontCounterClockwise = FALSE;
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0;
	rasterizer_desc.SlopeScaledDepthBias = 0;
	rasterizer_desc.DepthClipEnable = TRUE;
	rasterizer_desc.ScissorEnable = FALSE;
	rasterizer_desc.MultisampleEnable = FALSE;
	rasterizer_desc.AntialiasedLineEnable = FALSE;
	hr = FRAMEWORK.device->CreateRasterizerState(&rasterizer_desc, filling_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));




	//�[�x�X�e���V���X�e�[�g�I�u�W�F�N�g����
	D3D11_DEPTH_STENCIL_DESC depth_desc;

	depth_desc.DepthEnable = TRUE;
	depth_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_desc.DepthFunc = D3D11_COMPARISON_LESS;
	depth_desc.StencilEnable = FALSE;
	depth_desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depth_desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depth_desc.FrontFace.StencilFunc = depth_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth_desc.FrontFace.StencilDepthFailOp = depth_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_desc.FrontFace.StencilPassOp = depth_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_desc.FrontFace.StencilFailOp = depth_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;


	hr = FRAMEWORK.device->CreateDepthStencilState(&depth_desc, depth_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));



	//���_���Z�b�g

	//position,normal�̏���
	vertex vertics[] = {
		//�O��
		{ XMFLOAT3(-1.0f,1.0f,-1.0f), XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//0
		{ XMFLOAT3(1.0f, 1.0f,-1.0f), XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//1
		{ XMFLOAT3(1.0f,-1.0f,-1.0f), XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//2
		{ XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//3
		//�O�ʂ����ĉE��
		{ XMFLOAT3(1.0f, 1.0f,-1.0f),XMFLOAT3(1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//4
		{ XMFLOAT3(1.0f, 1.0f, 1.0f),XMFLOAT3(1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//5
		{ XMFLOAT3(1.0f,-1.0f, 1.0f),XMFLOAT3(1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//6
		{ XMFLOAT3(1.0f,-1.0f,-1.0f),XMFLOAT3(1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//7
		//�O�ʂ����č���
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//8
		{ XMFLOAT3(-1.0f, 1.0f,-1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//9
		{ XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//10
		{ XMFLOAT3(-1.0f,-1.0f, 1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//11
		//����
		{ XMFLOAT3(1.0f, 1.0f,1.0f),XMFLOAT3(-1.0f,-1.0f,1.0f),XMFLOAT2(0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//12
		{ XMFLOAT3(-1.0f, 1.0f,1.0f),XMFLOAT3(-1.0f,-1.0f,1.0f),XMFLOAT2(1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//13
		{ XMFLOAT3(-1.0f,-1.0f,1.0f),XMFLOAT3(-1.0f,-1.0f,1.0f),XMFLOAT2(1.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//14
		{ XMFLOAT3(1.0f,-1.0f,1.0f),XMFLOAT3(-1.0f,-1.0f, 1.0f),XMFLOAT2(0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//15
		//���
		{ XMFLOAT3(-1.0f,1.0f, 1.0f),XMFLOAT3(-1.0f,1.0f,-1.0f),XMFLOAT2(0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//16
		{ XMFLOAT3(1.0f,1.0f, 1.0f),XMFLOAT3(-1.0f, 1.0f,-1.0f),XMFLOAT2(1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//17
		{ XMFLOAT3(1.0f,1.0f,-1.0f),XMFLOAT3(-1.0f, 1.0f,-1.0f),XMFLOAT2(1.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//18
		{ XMFLOAT3(-1.0f,1.0f,-1.0f),XMFLOAT3(-1.0f,1.0f,-1.0f),XMFLOAT2(0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//19
		//����
		{ XMFLOAT3(1.0f,-1.0f, 1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//20
		{ XMFLOAT3(-1.0f,-1.0f, 1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,0.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//21
		{ XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(1.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//22
		{ XMFLOAT3(1.0f,-1.0f,-1.0f),XMFLOAT3(-1.0f,-1.0f,-1.0f),XMFLOAT2(0.0f,1.0f),XMFLOAT4(1.0f,1.0f,1.0f,1.0f) },//23
	};
	u_int indices[] = {
		//���v���ŕ\��
		//�O��
		0, 1, 2,
		2, 3, 0,
		//�O�ʂ����ĉE��
		4, 5, 6,
		6, 7, 4,
		//�O�ʂ����č���
		8, 9, 10,
		10, 11, 8,
		//����
		12,13,14,
		14,15,12,
		//���
		16,17,18,
		18,19,16,
		//����
		20,21,22,
		22,23,20,
	};

	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.ByteWidth = sizeof(vertics);
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;// D3D11_USAGE_DYNAMIC;
	buffer_desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subresource = {};
	subresource.pSysMem = vertics;
	subresource.SysMemPitch = 0;
	subresource.SysMemSlicePitch = 0;

	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, vertex_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));




	//�C���f�b�N�X���Z�b�g
	ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&subresource, sizeof(D3D11_SUBRESOURCE_DATA));
	buffer_desc.ByteWidth = sizeof(indices);
	buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;// D3D11_USAGE_DYNAMIC;
	buffer_desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	subresource.pSysMem = indices;
	subresource.SysMemPitch = 0;
	subresource.SysMemSlicePitch = 0;

	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, index_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	texture = std::make_shared<Texture>(filename);

	ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&subresource, sizeof(D3D11_SUBRESOURCE_DATA));
	buffer_desc.ByteWidth = sizeof(cbuffer);
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;/*
	subresource.pSysMem = constant_buffer;
	subresource.SysMemPitch = 0;
	subresource.SysMemSlicePitch = 0;*/

	//subresource.pSysMem = NULL;

	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, nullptr/*&subresource*/, constant_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, nullptr/*&subresource*/, cbuffer_off.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

}


void  board_primitive::render(
	YRShader* shader,
	DirectX::XMFLOAT3& pos,
	DirectX::XMFLOAT3& scale,
	DirectX::XMFLOAT3& angle,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	float Offset_X,
	float Offset_Y,
	const DirectX::XMFLOAT4			material_color,
	const bool						viewflag
)
{
	
	//���[���h�ϊ��s��̏�����
	DirectX::XMMATRIX s, r, t;
	//�g��s��쐬
	s = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//��]�s��쐬
	r = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	//�ړ��s��쐬
	t = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	//�s�񍇐��ƕϊ�
	DirectX::XMMATRIX world_matrix = s * r * t;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, world_matrix);
	//���[���h�E�r���[�E�v���W�F�N�V�����s��쐬
	DirectX::XMFLOAT4X4 world_view_projection;
	DirectX::XMStoreFloat4x4(&world_view_projection, world_matrix * view * projection);


	UINT stride = sizeof(vertex);
	UINT offset = 0;
	//�e�N�X�`���Z�b�g
	texture->Set(0);

	//�萔�o�b�t�@�̃o�C���h
	cbuffer cb = {};
	cb.world = world;
	cb.world_view_projection = world_view_projection;
	cb.material_color = material_color;
	FRAMEWORK.context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cb, 0, 0);
	FRAMEWORK.context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
	FRAMEWORK.context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

	//�I�t�Z�b�g�l�̃Z�b�g
	cbuffer_offset cboff = {};
	cboff.Offset_X = Offset_X;
	cboff.Offset_Y = Offset_Y;
	cboff.dummy00 = 0.0f;
	cboff.dummy01 = 0.0f;
	FRAMEWORK.context->UpdateSubresource(cbuffer_off.Get(), 0, 0, &cboff, 0, 0);
	FRAMEWORK.context->VSSetConstantBuffers(3, 1, cbuffer_off.GetAddressOf());
	FRAMEWORK.context->PSSetConstantBuffers(3, 1, cbuffer_off.GetAddressOf());

	//���_�o�b�t�@�̃o�C���h
	FRAMEWORK.context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);

	//�C���f�b�N�X�o�b�t�@�̃o�C���h
	FRAMEWORK.context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, offset);

	FRAMEWORK.context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	shader->Acivate();

	//�X�e�[�g�E�V�F�[�_�[�ݒ�
	if (viewflag)
	{
		FRAMEWORK.context->RSSetState(filling_state.Get());
	}
	else
	{
		FRAMEWORK.context->RSSetState(line_state.Get());
	}
	/*FRAMEWORK.context->IASetInputLayout(input_layout);
	FRAMEWORK.context->VSSetShader(vertex_shader, NULL, 0);
	FRAMEWORK.context->PSSetShader(pixel_shader, NULL, 0);*/

	//�v���~�e�B�u�̕`��
	FRAMEWORK.context->DrawIndexed(36, NULL, NULL);

	shader->Inactivate();
}