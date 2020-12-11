#pragma once
#include <DirectXMath.h>
#include <vector>
#include <windows.h>
#include<wrl.h>
#include <d3d11.h>
#include "YRShader.h"

struct PosBuffer
{
	DirectX::XMFLOAT3	head = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	tail = { 0.0f,0.0f,0.0f };
	bool				isUsed = true;
};

struct PosData
{
	DirectX::XMFLOAT3	pos = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3	normal = { 0.0f,0.0f,0.0f };
};

class Trajectory
{
public:
	void Init();
	void Update();
	void SetTrajectoryPos(const DirectX::XMFLOAT3& headPos, const DirectX::XMFLOAT3& tailPos);

	void render(
		YRShader* shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection
	);


private:
	Microsoft::WRL::ComPtr<ID3D11Buffer>			vertex_buffer;		//���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>			index_buffer;		//�C���f�b�N�X�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer>			constant_buffer;	//�萔�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	line_state;			//���`��
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	filling_state;		//�h��Ԃ��`��
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depth_state;


	std::vector<PosBuffer>		posArray;
	std::vector<PosData>		VB;

	struct cbuffer
	{
		DirectX::XMFLOAT4X4		world_view_projection;	//���[���h�E�r���[�E�v���W�F�N�V���������s��
		DirectX::XMFLOAT4X4		world;					//���[���h�ϊ��s��
		DirectX::XMFLOAT4		eyePos;
		DirectX::XMFLOAT4X4		view;
		DirectX::XMFLOAT4X4		projection;
		DirectX::XMFLOAT3		at;						//�J�������W����focus�܂ł̒P�ʃx�N�g��
		float					dummy;
	};
};
