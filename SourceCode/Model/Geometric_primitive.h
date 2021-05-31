#pragma once

#include <windows.h>
#include<directxmath.h>
#include<string>
#include "../YRShader.h"
#include "../Texture.h"

//-----------------------------------------------------------------
//			�W�I���g���b�N�v���~�e�B�u�N���X
//-----------------------------------------------------------------
//�E�w�肵���傫���̃|���S���𐶐��ł���N���X
//-----------------------------------------------------------------

class geometric_primitive
{
public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>			vertex_buffer;		//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>			index_buffer;		//�C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>			constant_buffer;	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	line_state;			//���`��
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	filling_state;		//�h��Ԃ��`��
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_state;

	//std::shared_ptr<Texture> texture = nullptr;							//�e�N�X�`��

	geometric_primitive();
	//geometric_primitive(std::shared_ptr<Texture> tex);

	~geometric_primitive()
	{
		/*vertex_shader->Release();
		pixel_shader->Release();
		input_layout->Release();
		vertex_buffer->Release();
		index_buffer->Release();
		constant_buffer->Release();
		line_state->Release();
		filling_state->Release();
		depth_state->Release();
		pResouce->Release();*/
	};

	void render(
		YRShader		*shader,
		const DirectX::XMFLOAT3& pos,
		const DirectX::XMFLOAT3& scale,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		//UNIT.23
		const DirectX::XMFLOAT4			material_color = { 1.0f,1.0f,1.0f,1.0f },
		const bool						viewflag = true
	);

	void render(
		YRShader* shader,
		Texture* texture,
		const DirectX::XMFLOAT3& model_pos,
		const DirectX::XMFLOAT3& pos,
		const DirectX::XMFLOAT3& scale,
		const DirectX::XMFLOAT3& angle,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		//UNIT.23
		const DirectX::XMFLOAT4			material_color = { 1.0f,1.0f,1.0f,1.0f },
		const bool						viewflag = true
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
		DirectX::XMFLOAT4		eyePos;
		DirectX::XMFLOAT4X4		view;
		DirectX::XMFLOAT4X4		projection;
		DirectX::XMFLOAT3		at;						//�J�������W����focus�܂ł̒P�ʃx�N�g��
		float					dummy;
	};
};

//namespace ALL
//{
//	/*HRESULT create_vertex(const char *cso_file, ID3D11VertexShader **vert, D3D11_INPUT_ELEMENT_DESC *layout, UINT numElements, ID3D11InputLayout **input);
//	HRESULT CreatePixel(const char *ps_file, ID3D11PixelShader **pixel);*/
//}