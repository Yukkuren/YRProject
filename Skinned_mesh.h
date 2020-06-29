#pragma once

#include<Windows.h>
#include<directxmath.h>
#include "YRShader.h"
#include<array>
#include<memory>

//#include "framework.h"


#define MAX_BONE_INFLUENCES 4
#define BONE_MAX	(32)

struct bone_influence
{
	int index;		//index of bone
	float weight;	//weight of bone
};
typedef std::vector<bone_influence> bone_influences_per_control_point;


class Skinned_mesh
{
private:
	//���C�s�b�N�p�|���S��
	/*struct Face
	{
		std::array<DirectX::XMFLOAT3, 3> position;
		int materialIndex;
	};
	std::vector<Face> faces;*/
public:
	//���C�s�b�N�֐�
	//int RayPick(
	//	const DirectX::XMFLOAT3& startPosition,	//���C���΂��J�n���W
	//	const DirectX::XMFLOAT3& endPosition,	//���C���΂��I�����W
	//	DirectX::XMFLOAT3* outPosition,			//���C�������������W
	//	DirectX::XMFLOAT3* outNormal,			//���C�����������ʂ̖@��
	//	float* outLength						//���C�����������ʂ܂ł̋���
	//);											//�߂�l�}�e���A���ԍ�
public:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			input_layout;
	//ID3D11Buffer				*vertex_buffer;
	//ID3D11Buffer				*index_buffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				constant_buffer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		line_state;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		filling_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depth_state;
	//ID3D11Resource				*pResouce;
	D3D11_TEXTURE2D_DESC texture2d_desc;
	unsigned int indexsize;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_state;
	
	
	//�`��p�ϐ�
	//DirectX::XMMATRIX	world_matrix;
	//DirectX::XMFLOAT4X4 world;

	//UNIT.21
	//convert coodinate system from 'Up:+Z FRONT:+Y RIGHT HAND' to 'UP:+Y FRONT:+Z LEFT-HAND'
	DirectX::XMFLOAT4X4 coodinate_conversion = {
		1,0,0,0,
		0,0,1,0,
		0,1,0,0,
		0,0,0,1
	};
	//Y����Z�������ւ���

	Skinned_mesh(const char *fbx_filename);

	~Skinned_mesh()
	{
		//vertex_shader->Release();
		//pixel_shader->Release();
		//input_layout->Release();
		////vertex_buffer->Release();
		////index_buffer->Release();
		//constant_buffer->Release();
		//line_state->Release();
		//filling_state->Release();
		//depth_state->Release();
		////pResouce->Release();
	}


	//��{�I�ɂ��������g��(position,scale,angle�����͌^)
	void Render(
		YRShader					*shader,
		DirectX::XMFLOAT3			&pos,
		DirectX::XMFLOAT3			&scale,
		DirectX::XMFLOAT3			&angle,
		const DirectX::XMMATRIX		&view,
		const DirectX::XMMATRIX		&projection,
		const DirectX::XMFLOAT4		&light_direction,
		const DirectX::XMFLOAT4		&light_color,
		const DirectX::XMFLOAT4		&ambient_color,
		//UNIT.23
		float						elapsed_time,
		float						anime_count,		//�A�j���̃J�E���g
		const DirectX::XMFLOAT4			material_color = { 1.0f,1.0f,1.0f,1.0f },
		const bool						viewflag = true
	);

	void Render(
		YRShader					*shader,
		DirectX::XMFLOAT3			&pos,
		DirectX::XMFLOAT3			&scale,
		DirectX::XMVECTOR			&orientation,
		const DirectX::XMMATRIX		&view,
		const DirectX::XMMATRIX		&projection,
		const DirectX::XMFLOAT4		&light_direction,
		const DirectX::XMFLOAT4		&light_color,
		const DirectX::XMFLOAT4		&ambient_color,
		//UNIT.23
		float						elapsed_time,
		float						anime_count,		//�A�j���̃J�E���g
		const DirectX::XMFLOAT4			material_color = { 1.0f,1.0f,1.0f,1.0f },
		const bool						viewflag = true
	);

	//������͏]���̂���
	//void Render(
	//	ID3D11DeviceContext			*context,
	//	const DirectX::XMFLOAT4X4	&world_view,
	//	const DirectX::XMFLOAT4X4	&world_matrix,
	//	const DirectX::XMFLOAT4		&light_direction,
	//	const DirectX::XMFLOAT4		&material_color,
	//	const DirectX::XMFLOAT4		&light_color,
	//	const DirectX::XMFLOAT4		&ambient_color,
	//	bool						viewflag,
	//	//UNIT.23
	//	float						elapsed_time,
	//	float						anime_count		//�A�j���̃J�E���g
	//);


	struct Vertex
	{
		DirectX::XMFLOAT3		position;
		DirectX::XMFLOAT3		normal;
		DirectX::XMFLOAT2		texcoord;
		FLOAT bone_weights[MAX_BONE_INFLUENCES] = { 1,0,0,0 };
		INT bone_indices[MAX_BONE_INFLUENCES] = {};
	};

	struct cbuffer
	{
		DirectX::XMFLOAT4X4		world_view_projection;
		DirectX::XMFLOAT4X4		world;
		DirectX::XMFLOAT4		material_color;
		DirectX::XMFLOAT4		light_direction;
		DirectX::XMFLOAT4		light_color;
		DirectX::XMFLOAT4		ambient_color;
		DirectX::XMFLOAT4X4		bone_transforms[BONE_MAX];
		DirectX::XMFLOAT4		eyePos;
	};

	struct material
	{
		DirectX::XMFLOAT4 color = { 0.8f,0.8f,0.8f,1.0f };
		ID3D11ShaderResourceView *shader_resource_view;
		~material()
		{
			if (shader_resource_view != nullptr)
			{
				shader_resource_view->Release();
			}
		}
	};
	//material diffuse;
	struct subset
	{
		u_int index_start = 0;	//	start number of index buffer
		u_int index_count = 0;	//	number of vertics (indices)
		material diffuse;
	};
	//std::vector<subset> subsets;

	//UNIT.22
	struct bone
	{
		DirectX::XMFLOAT4X4 transform;
	};
	//UNIT.23
	typedef std::vector<bone> skeletal;
	struct Skeletal_animation : public std::vector<skeletal>
	{
		float sampling_time = 1 / 24.0f;
		float animation_tick = 0.0f;
		//bool anim_fin = false;
	};

	struct mesh
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer>vertex_buffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>index_buffer;
		std::vector<subset> subsets;
		DirectX::XMFLOAT4X4 global_transform = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		//UNIT.22
		//std::vector<Skinned_mesh::bone> skeletal;
		//UNIT.23
		Skeletal_animation skeletal_animation;
	};
	std::vector<mesh>meshes;

	/*DirectX::XMFLOAT4X4 Interpolation(
		int i,
		float animation_tick,
		float sampling_time,
		std::vector<bone> &skeletal,
		std::vector<bone> &skeletal2
	);*/

	//�A�j���[�V�����Ǘ��p�ϐ�

	bool FinCheck()
	{
		for (auto& mesh : meshes)
		{
			/*if (!mesh.skeletal_animation.anim_fin)
			{
				return false;
			}*/
			int frame = mesh.skeletal_animation.animation_tick / mesh.skeletal_animation.sampling_time;
			if (frame < mesh.skeletal_animation.size())
			{
				return false;
			}
		}
		return true;
	}

	void AllFalse()
	{
		for (auto& mesh : meshes)
		{
			mesh.skeletal_animation.animation_tick = 0;
			//mesh.skeletal_animation.anim_fin = false;
		}
	}
	//void AnimReset();		//�A�j���[�V���������Z�b�g����
	//bool AnimFinCheck();	//�A�j���[�V�������I����Ă��邩�`�F�b�N
	//bool AnimFinCheck(float check);	//�����t���A�j���[�V�������I����Ă��邩�`�F�b�N
};



class MeshMotion
{
public:
	Skinned_mesh*	skin;		//���f���t���ւ��p�A�h���X
	float			anim_timer;	//���[�V�����p�^�C�}�[
public:
	//���[�V�����f�[�^���Z�b�g����
	void MeshSet(std::unique_ptr<Skinned_mesh>& mesh);

	//�A�j���[�V���������Z�b�g����
	void AnimReset();

	//���̂ݕ`��(�A�j���[�V�������I����true)
	bool Draw(
		YRShader			*shader,
		DirectX::XMFLOAT3& pos,
		DirectX::XMFLOAT3& scale,
		DirectX::XMFLOAT3& angle,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time,
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
	);

	//�A���`��
	void DrawContinue(
		YRShader		*shader,
		DirectX::XMFLOAT3& pos,
		DirectX::XMFLOAT3& scale,
		DirectX::XMFLOAT3& angle,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time,
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
	);

	//�A�j���[�V���������p
	void DrawMesh(
		YRShader		*shader,
		DirectX::XMFLOAT3 pos,
		DirectX::XMFLOAT3& scale,
		DirectX::XMFLOAT3& angle,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
	);

	//���݂̃A�j���[�V�����̏�ԂŒ�~�`��
	void DrawStop(
		YRShader		*shader,
		DirectX::XMFLOAT3 pos,
		DirectX::XMFLOAT3& scale,
		DirectX::XMFLOAT3& angle,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time,
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
	);


	//�p������s��g�p

	//���̂ݕ`��(�A�j���[�V�������I����true)
	bool Draw(
		YRShader		*shader,
		DirectX::XMFLOAT3& pos,
		DirectX::XMFLOAT3& scale,
		DirectX::XMVECTOR& orientation,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time,
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
	);

	//�A���`��
	void DrawContinue(
		YRShader		*shader,
		DirectX::XMFLOAT3& pos,
		DirectX::XMFLOAT3& scale,
		DirectX::XMVECTOR& orientation,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time,
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
	);

	//�A�j���[�V���������p
	void DrawMesh(
		YRShader* shader,
		DirectX::XMFLOAT3 pos,
		DirectX::XMFLOAT3& scale,
		DirectX::XMVECTOR& orientation,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
	);

	//���݂̃A�j���[�V�����̏�ԂŒ�~�`��
	void DrawStop(
		YRShader		*shader,
		DirectX::XMFLOAT3 pos,
		DirectX::XMFLOAT3& scale,
		DirectX::XMVECTOR& orientation,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time,
		const DirectX::XMFLOAT4		material_color = { 1.0f,1.0f,1.0f,1.0f }
	);
};