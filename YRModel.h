#pragma once

#include<Windows.h>
#include<directxmath.h>
#include "YRShader.h"
#include<array>
#include<memory>
#include "Texture.h"
#include <fbxsdk.h> 

struct ModelData
{
public:
	struct Node
	{
		std::string				name;			//�m�[�h��
		int						parent_index;	//�e�m�[�h�̐�
		DirectX::XMFLOAT3		scale;
		DirectX::XMFLOAT4		rotate;
		DirectX::XMFLOAT3		translate;
	};

	struct Vertex
	{
		DirectX::XMFLOAT3		position;
		DirectX::XMFLOAT3		normal;
		DirectX::XMFLOAT2		texcoord;
		DirectX::XMFLOAT4		bone_weights;
		DirectX::XMINT4			bone_indices;
	};

	struct Subset
	{
		int					material_index;
		int					start_index;
		int					index_count;
	};

	struct Mesh
	{
		std::vector<Vertex>	vertices;
		std::vector<int>	indices;
		std::vector<Subset>	subsets;

		int					node_index;

		std::vector<int>					node_indices;
		std::vector<DirectX::XMFLOAT4X4>	inverse_transforms;
	};

	struct Material
	{
		DirectX::XMFLOAT4	color;
		std::string			texture_filename;
	};

	struct NodeKeyData
	{
		DirectX::XMFLOAT3	scale;
		DirectX::XMFLOAT4	rotate;
		DirectX::XMFLOAT3	translate;
	};

	struct Keyframe
	{
		float						seconds;
		std::vector<NodeKeyData>	node_keys;
	};
	struct Animation
	{
		float						seconds_length;
		std::vector<Keyframe>		keyframes;
	};

	std::vector<Node>		nodes;

	std::vector<Mesh>		meshes;
	std::vector<Material>	materials;

	std::vector<Animation>	animations;
};


//----------------------------------------------------------------------------


class Model
{
public:
	Model(const char* filename);
	Model(const char* filename, std::shared_ptr<Texture> tex);
	~Model() {}

private:
	// ���f���f�[�^���\�z
	bool BuildModel(const char* dirname, FbxScene* fbx_scene);

	// �m�[�h�f�[�^���\�z
	void BuildNodes(FbxNode* fbx_node,int parent_node_index);
	void BuildNode(FbxNode* fbx_node,int parent_node_index);

	// ���b�V���f�[�^���\�z
	void BuildMeshes(FbxNode* fbx_node);
	void BuildMesh(FbxNode* fbx_node, FbxMesh* fbx_mesh);

	// �}�e���A���f�[�^���\�z
	void BuildMaterials(const char* dirname, FbxScene* fbx_scene);
	void BuildMaterial(const char* dirname, FbxSurfaceMaterial* fbx_surface_material);

	// �A�j���[�V�����f�[�^���\�z
	void BuildAnimations(FbxScene* fbx_scene);

	// �C���f�b�N�X�̌���
	int FindNodeIndex(const char* name);
	int FindMaterialIndex(FbxScene* fbx_scene, const FbxSurfaceMaterial* fbx_surface_material);

	// FBX�t�@�C�����烂�f����ǂݍ���
	bool Load(const char* filename);

public:
	enum Material_Attribute : int
	{
		MAIN = 0,
		EYE,
		MOUSE,
	};	//����enum�łǂ̃e�N�X�`�����g���Ă��邩�𔻕ʂ��A�K�v�ɂ����offset�l����͂���

	struct Material
	{
		DirectX::XMFLOAT4 color = { 0.8f, 0.8f, 0.8f, 1.0f };
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
	};

	struct Subset
	{
		u_int		start_index = 0;
		u_int		index_count = 0;
		Material* material;
		Material_Attribute			material_index;
	};

	struct Mesh
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer>	vertex_buffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>	index_buffer;
		std::vector<Subset>						subsets;

		int										node_index;
		std::vector<int>						node_indices;
		std::vector<DirectX::XMFLOAT4X4*>		inverse_transforms;
	};

	const std::vector<Mesh>& GetMeshes() const { return m_meshes; }
	const std::vector<ModelData::Node>& GetNodes() const { return m_data->nodes; }
	const std::vector<ModelData::Animation>& GetAnimations() const { return m_data->animations; }
	std::vector<Mesh>		m_meshes;
	//�e�N�X�`�����p
	std::shared_ptr<Texture> texture = nullptr;
	std::unique_ptr<ModelData>	m_data;
private:
	std::vector<Material>	m_materials;
};