#pragma once

#include<Windows.h>
#include<directxmath.h>
#include "YRShader.h"
#include<array>
#include<memory>
#include "Texture.h"
#include <fbxsdk.h> 
#include "YR_VectorMaster.h"


#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>



struct ModelData
{
	~ModelData()
	{
		nodes.clear();
		meshes.clear();
		materials.clear();
		animations.clear();
	}
public:
	struct Node
	{
		std::string				name;			//ノード名
		int						parent_index;	//親ノードの数
		YR_Vector3				scale;
		YR_Vector4				rotate;
		YR_Vector3				translate;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(CEREAL_NVP(name), CEREAL_NVP(parent_index), CEREAL_NVP(scale), CEREAL_NVP(rotate), CEREAL_NVP(translate));
		}
	};

	struct Vertex
	{
		YR_Vector3				position;
		YR_Vector3				normal;
		YR_Vector2				texcoord;
		YR_Vector4				bone_weights;
		YR_INT4					bone_indices;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(CEREAL_NVP(position), CEREAL_NVP(normal), CEREAL_NVP(texcoord), CEREAL_NVP(bone_weights), CEREAL_NVP(bone_indices));
		}
	};

	struct Subset
	{
		int					material_index;
		int					start_index;
		int					index_count;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(CEREAL_NVP(material_index), CEREAL_NVP(start_index), CEREAL_NVP(index_count));
		}
	};

	struct Mesh
	{
		std::vector<Vertex>	vertices;
		std::vector<int>	indices;
		std::vector<Subset>	subsets;

		int					node_index;

		std::vector<int>					node_indices;
		std::vector<YR_Vector4X4>			inverse_transforms;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(CEREAL_NVP(vertices), CEREAL_NVP(indices), CEREAL_NVP(subsets), CEREAL_NVP(node_index),
				CEREAL_NVP(node_indices), CEREAL_NVP(inverse_transforms));
		}

		~Mesh() {
			vertices.clear();
			indices.clear();
			subsets.clear();
			node_indices.clear();
			inverse_transforms.clear();
		}
	};

	struct Material
	{
		YR_Vector4			color;
		std::string			texture_filename;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(CEREAL_NVP(color), CEREAL_NVP(texture_filename));
		}
	};

	struct NodeKeyData
	{
		YR_Vector3			scale;
		YR_Vector4			rotate;
		YR_Vector3			translate;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(CEREAL_NVP(scale), CEREAL_NVP(rotate), CEREAL_NVP(translate));
		}
	};

	struct Keyframe
	{
		float						seconds;
		std::vector<NodeKeyData>	node_keys;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(CEREAL_NVP(seconds), CEREAL_NVP(node_keys));
		}

		~Keyframe() {
			node_keys.clear();
		}
	};
	struct Animation
	{
		float						seconds_length;
		std::vector<Keyframe>		keyframes;

		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(CEREAL_NVP(seconds_length), CEREAL_NVP(keyframes));
		}

		~Animation() {
			keyframes.clear();
		}
	};

	std::vector<Node>		nodes;

	std::vector<Mesh>		meshes;
	std::vector<Material>	materials;

	std::vector<Animation>	animations;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(CEREAL_NVP(nodes), CEREAL_NVP(meshes), CEREAL_NVP(materials), CEREAL_NVP(animations));
	}
};


//----------------------------------------------------------------------------


class Model
{
public:
	Model(const char* filename);
	Model(const char* filename, const std::shared_ptr<Texture> tex_main, const std::shared_ptr<Texture> tex_face);
	~Model() {
		color_texture_main.reset();
		color_texture_face.reset();
		m_data.reset();
		m_materials.clear();
		m_meshes.clear();
	}
	std::string Serial_name;

private:
	// モデルデータを構築
	bool BuildModel(const char* dirname, FbxScene* fbx_scene);

	// ノードデータを構築
	void BuildNodes(FbxNode* fbx_node,int parent_node_index);
	void BuildNode(FbxNode* fbx_node,int parent_node_index);

	// メッシュデータを構築
	void BuildMeshes(FbxNode* fbx_node);
	void BuildMesh(FbxNode* fbx_node, FbxMesh* fbx_mesh);

	// マテリアルデータを構築
	void BuildMaterials(const char* dirname, FbxScene* fbx_scene);
	void BuildMaterial(const char* dirname, FbxSurfaceMaterial* fbx_surface_material);

	// アニメーションデータを構築
	void BuildAnimations(FbxScene* fbx_scene);

	// インデックスの検索
	int FindNodeIndex(const char* name);
	int FindMaterialIndex(FbxScene* fbx_scene, const FbxSurfaceMaterial* fbx_surface_material);

	// FBXファイルからモデルを読み込み
	bool Load(const char* filename);


	void SerialNameGet(const char* filename);
	bool FileCheck();
	void Serialize();
	void LoadSerial();


public:
	enum Material_Attribute : int
	{
		MAIN = 0,
		EYE,
		MOUSE,
		SWORD,
		NONE,
		ALL,
	};	//このenumでどのテクスチャを使っているかを判別し、必要によってoffset値を入力する

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

		~Mesh() {
			subsets.clear();
			node_indices.clear();
			inverse_transforms.clear();
		}
	};

	const std::vector<Mesh>& GetMeshes() const { return m_meshes; }
	const std::vector<ModelData::Node>& GetNodes(){ return m_data->nodes; }
	const std::vector<ModelData::Animation>& GetAnimations(){ return m_data->animations; }
	std::vector<Mesh>		m_meshes;
	//テクスチャ利用
	std::shared_ptr<Texture> color_texture_main = nullptr;
	std::shared_ptr<Texture> color_texture_face = nullptr;
	std::unique_ptr<ModelData>	m_data;
private:
	std::vector<Material>	m_materials;
};