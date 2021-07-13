#pragma once

#include <memory>
#include <d3d11.h>
#include "YRModel.h"
//#include "framework.h"

//-----------------------------------------------------------------
//			ModelAnim�N���X
//-----------------------------------------------------------------
//�E���f���f�[�^���A�j���[�V���������邽�߂̃N���X
//�E�g�����͂��̃N���X�𐶐�����Ƃ��Ƀ��b�V�������郂�f����Model�N���X��n��
//�E���̌�A���b�V���̖����A�j���[�V�����݂̂�Model�N���X���Z�b�g���邱�Ƃ�
//  ���b�V���̃A�j���[�V�������o����
//-----------------------------------------------------------------

class ModelAnim
{
public:
	ModelAnim(const std::shared_ptr<Model>& resource);
	~ModelAnim() {
		m_model_resource.reset();
		model_resource_anim.reset();
		m_nodes.clear();
	}

	struct Node
	{
		std::string name;
		Node* parent;
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
		DirectX::XMFLOAT4X4	local_transform;
		DirectX::XMFLOAT4X4	world_transform;
	};

	// �A�j���[�V����
	bool IsPlayAnimation() const { return m_current_animation >= 0; }
	void PlayAnimation(int animation_index, bool loop = false);
	void UpdateAnimation(float elapsed_time);
	void DebugUpdateAnimation(float timer);

	// �s��v�Z
	void CalculateLocalTransform();
	void CalculateWorldTransform(
		const DirectX::XMFLOAT3& pos,
		const DirectX::XMFLOAT3& scale,
		const DirectX::XMFLOAT3& angle);

	const std::vector<Node>& GetNodes() const { return m_nodes; }
	const Model* GetModelResource() const { return m_model_resource.get(); }

	DirectX::XMFLOAT4X4 Draw(
		YRShader* shader,
		const DirectX::XMMATRIX&	view,
		const DirectX::XMMATRIX&	projection,
		const DirectX::XMFLOAT4&	light_direction,
		const DirectX::XMFLOAT4&	light_color,
		const DirectX::XMFLOAT4&	ambient_color,
		const DirectX::XMFLOAT2&	off_set_eye = { 0.0f,0.0f },
		const DirectX::XMFLOAT2&	off_set_mouse = { 0.0f,0.0f },
		const Model::Material_Attribute& blur_material= Model::Material_Attribute::SWORD,
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
		);



	void Draw(
		YRShader* shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const D3D_PRIMITIVE_TOPOLOGY& topology,
		const DirectX::XMFLOAT4& ambient_color,
		const DirectX::XMFLOAT2& off_set_eye = { 0.0f,0.0f },
		const DirectX::XMFLOAT2& off_set_mouse = { 0.0f,0.0f },
		const Model::Material_Attribute& blur_material = Model::Material_Attribute::SWORD,
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
	);

	void NodeChange(const std::shared_ptr<Model>& resource);
	void NodeChange(const std::shared_ptr<Model>& resource,int anim_num);

	//bool AnimCheack(const std::shared_ptr<Model>& resource, int anim_num);

	const bool &GetLoopAnim()const  { return m_loop_animation; };	//�Đ����̃A�j���[�V���������[�v���邩��Ԃ�
	const int& GetEndAnim()const { return m_current_animation; };	//�Đ����̃A�j���[�V�������I����������Ԃ�(-1�Ȃ�I��)

	const bool& GetIntermediate()const { return model_resource_anim->intermediate; };	//�Đ����̃A�j���[�V���������ԃA�j���[�V�������ǂ�����Ԃ�

private:
	std::shared_ptr<Model>			m_model_resource;
	std::shared_ptr<Model>			model_resource_anim = nullptr;
	std::vector<Node>				m_nodes;
	int								m_current_animation = -1;
	float							m_current_seconds = 0.0f;
	bool							m_loop_animation = false;
	bool							m_end_animation = false;
	DirectX::XMMATRIX				world_matrix;

	static const int MaxBones = 128;

	struct cbuffer
	{
		DirectX::XMFLOAT4X4		world_view_projection;
		DirectX::XMFLOAT4X4		world;
		DirectX::XMFLOAT4		material_color;
		DirectX::XMFLOAT4		light_direction;
		DirectX::XMFLOAT4		light_color;
		DirectX::XMFLOAT4		ambient_color;
		DirectX::XMFLOAT4X4		bone_transforms[MaxBones];
		DirectX::XMFLOAT4		eyePos;						//�J�����̈ʒu
		DirectX::XMFLOAT4X4		view;						//�g�D�[���V�F�[�_�[�pview�s��
		DirectX::XMFLOAT4X4		projection;					//�g�D�[���V�F�[�_�[�pprojection�s��
		DirectX::XMFLOAT3		at;							//eye����focus�Ɍ��������K���x�N�g��
		float					lumi_factor;				//�P�x�����p(0.0f~1.0f)
		float					Offset_X;					//�e�N�X�`���̈ʒu����X
		float					Offset_Y;					//�e�N�X�`���̈ʒu����Y
		float					dummy00;
		float					dummy01;
		DirectX::XMFLOAT4		dummy02;
	};


	Microsoft::WRL::ComPtr<ID3D11Buffer>			constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	m_rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	m_depth_stencil_state;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		m_sampler_state;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_dummy_srv;

	DirectX::XMFLOAT4X4 coodinate_conversion = {
		1,0,0,0,
		0,0,1,0,
		0,1,0,0,
		0,0,0,1
	};
};
