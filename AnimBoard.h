#pragma once

#include "Texture.h"
#include "YRShader.h"
#include <memory>

class AnimBoard
{
private:
	float w, h;			//�傫��
	int type;			//���݂̉摜�̐؂���ԍ�
	int split_size;		//�摜�̕�����
	XMFLOAT2 tex_size;	//�摜�̐؂���1���̑傫��
	XMINT2	tex_split;	//�摜�̏c��������
	XMFLOAT2 tex_max_size;//�摜�S�̂̏c���̑傫��
	float elapsed;

	// ���_�f�[�^
	Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
	// �萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer;
	//���`��
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	line_state;
	//�h��Ԃ��`��
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	filling_state;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_state;
	//�e�N�X�`�����p
	std::shared_ptr<Texture> texture = nullptr;
	// �萔�o�b�t�@�̃f�[�^��`
	struct CBuffer {
		XMFLOAT4X4	view;			// �r���[�ϊ��s��
		XMFLOAT4X4	projection;		// �����ϊ��s��
		XMFLOAT4X4	SR_Matrix;		//�X�P�[���Ɖ�]�s��̍�����̍s��
	};
public:
	AnimBoard(
		std::shared_ptr<Texture> texture,
		int split_size,			//�摜�̕�������
		XMFLOAT2 tex_size,		//�摜�̐؂���1���̑傫��
		XMINT2 tex_split,		//�摜�̏c��������
		XMFLOAT2 tex_max_size	//�摜�S�̂̏c���̑傫��
	);
	void Render(
		YRShader* shader,
		DirectX::XMFLOAT3& pos,
		DirectX::XMFLOAT2& size,
		DirectX::XMFLOAT3& angle,
		float					speed,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		float					elapsed_time,
		float					alpha = 1.0f,
		const bool				viewflag = true
	);


	struct VERTEX
	{
		XMFLOAT3 Pos;	//�ʒu
		XMFLOAT2 Size;	//�|���̔����̃��[���h��Ԃ̃T�C�Y
		XMFLOAT2 UV_Pos;//�e�N�X�`����Ԃ̐؂��鍶��̍��W(0�`1)
		XMFLOAT2 UV_Size;//�e�N�X�`����Ԃ̐؂���S�̃T�C�Y
		XMFLOAT4 Color;	//���_�F
	};
};
