#pragma once

#include <windows.h>
#include<d3d11.h>
#include<directxmath.h>
#include <wrl.h>
#include<string>

class geometric_primitive
{
public:
	ID3D11VertexShader		*vertex_shader;
	ID3D11PixelShader		*pixel_shader;
	ID3D11InputLayout		*input_layout;
	ID3D11Buffer			*vertex_buffer;			//���_�o�b�t�@
	ID3D11Buffer			*index_buffer;			//�C���f�b�N�X�o�b�t�@
	ID3D11Buffer			*constant_buffer;		//�萔�o�b�t�@
	ID3D11RasterizerState	*line_state;			//���`��
	ID3D11RasterizerState	*filling_state;			//�h��Ԃ��`��
	ID3D11DepthStencilState	*depth_state;
	ID3D11Resource *pResouce;

	geometric_primitive();

	~geometric_primitive()
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
	};

	void render(
		const DirectX::XMFLOAT4X4	&world_view,		//���[���h�E�r���[�E�v���W�F�N�V���������s��
		const DirectX::XMFLOAT4X4	&world_matrix,		//���[���h�ϊ��s��
		const DirectX::XMFLOAT4		&light_direction,	//���C�g�i�s����
		const DirectX::XMFLOAT4		&material_color,	//�ގ��F
		bool						viewflag			//���E�h��Ԃ��`��t���O
	);

	struct vertex
	{
		DirectX::XMFLOAT3		position;
		DirectX::XMFLOAT3		normal;
	};

	struct cbuffer
	{
		DirectX::XMFLOAT4X4		world_view_projection;	//���[���h�E�r���[�E�v���W�F�N�V���������s��
		DirectX::XMFLOAT4X4		world;					//���[���h�ϊ��s��
		DirectX::XMFLOAT4		material_color;			//�ގ��F
		DirectX::XMFLOAT4		light_direction;		//���C�g�i�s����
	};
};

namespace ALL
{
	HRESULT create_vertex(const char *cso_file, ID3D11VertexShader **vert, D3D11_INPUT_ELEMENT_DESC *layout, UINT numElements, ID3D11InputLayout **input);
	HRESULT CreatePixel(const char *ps_file, ID3D11PixelShader **pixel);
}
