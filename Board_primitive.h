#pragma once

#include <windows.h>
#include<directxmath.h>
#include<string>
#include "YRShader.h"
#include "Texture.h"

class board_primitive
{
public:
	Microsoft::WRL::ComPtr<ID3D11Buffer>			vertex_buffer;		//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>			index_buffer;		//�C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>			constant_buffer;	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>			cbuffer_off;	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	line_state;			//���`��
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	filling_state;		//�h��Ԃ��`��
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_state;

	std::shared_ptr<Texture> texture = nullptr;							//�e�N�X�`��

	board_primitive();
	board_primitive(std::shared_ptr<Texture> tex);
	board_primitive(const wchar_t* filename);

	~board_primitive(){};

	void render(
		YRShader* shader,
		DirectX::XMFLOAT3& pos,
		DirectX::XMFLOAT3& scale,
		DirectX::XMFLOAT3& angle,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		float Offset_X = 0.0f,
		float Offset_Y = 0.0f,
		const DirectX::XMFLOAT4			material_color = { 1.0f,1.0f,1.0f,1.0f },
		const bool						viewflag = true
	);

	struct vertex
	{
		DirectX::XMFLOAT3		position;
		DirectX::XMFLOAT3		normal;
		DirectX::XMFLOAT2		tex;
		DirectX::XMFLOAT4		color;
	};

	struct cbuffer
	{
		DirectX::XMFLOAT4X4		world_view_projection;	//���[���h�E�r���[�E�v���W�F�N�V���������s��
		DirectX::XMFLOAT4X4		world;					//���[���h�ϊ��s��
		DirectX::XMFLOAT4		material_color;			//�ގ��F
	};
	
	struct cbuffer_offset
	{
		float Offset_X;
		float Offset_Y;
		float dummy00;
		float dummy01;
	};
};
