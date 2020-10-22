#include"DirectXTK-master\Inc\WICTextureLoader.h"
#include "YRModel.h"
#include "misc.h"
#include "framework.h"
#include <codecvt>
#include <array>


//#define _CRTDBG_MAP_ALLOC						// malloc�ɂ�郁�������[�N���o��CPP�t�@�C�����ƍs���o�͎w��
//#define DBG_NEW new( _NORMAL_BLOCK , __FILE__ , __LINE__)	// new �ɂ�郁�������[�N���o��CPP�t�@�C�����ƍs���o�͎w��
//#include <stdio.h>
//#include <crtdbg.h>

using convert_t = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_t, wchar_t> strconverterModel;

std::string to_stringModel(std::wstring wstr)
{
	return strconverterModel.to_bytes(wstr);
}

std::wstring to_wstringModel(std::string str)
{
	return strconverterModel.from_bytes(str);
}

Model::Model(const char* filename)
{
	//m_data = std::move(data);
	m_data = std::make_unique<ModelData>();

	color_texture_main = nullptr;
	color_texture_face = nullptr;

	//_CrtDumpMemoryLeaks();

	SerialNameGet(filename);

	if (FileCheck())
	{
		//�V���A���C�Y���ꂽ�o�C�i����ǂݍ���
		LoadSerial();
	}
	else
	{
		//���ʂ�FBX����ǂݍ��݁A�V���A���C�Y����
		Load(filename);
		Serialize();
	}
	// �}�e���A��
	m_materials.resize(m_data->materials.size());
	for (size_t material_index = 0; material_index < m_materials.size(); ++material_index)
	{
		auto&& src = m_data->materials.at(material_index);
		auto&& dst = m_materials.at(material_index);

		dst.color = src.color;
		// �e�N�X�`���ǂݍ���
		if (!src.texture_filename.empty())
		{
			size_t length;
			std::array<wchar_t,256> file;
			::mbstowcs_s(&length, file.data(), 256, src.texture_filename.c_str(), _TRUNCATE);

			std::array<char,256> char_file;
			
			for (int i = 0; i < file.size(); i++)
			{
				char_file[i] = file[i];
			}

			std::string fbx_name = filename;
			std::string tex_file_name = std::string(char_file.data());
			std::string tex_name;
			//�K�w����
			//if (Fbx_Tex_LevelCheck(&fbx_name, &tex_file_name))
			//{
			//	//�����K�w�������ꍇ�̏���������
			//	int hoge = 0;
			//}
			//else
			//{
			//���΃p�X����΃p�X�������ꍇ�̏���(���Maya)
			if (tex_file_name[0] == 'C')
			{
				int size = 0;
				int slash = 0;
				for (int i = tex_file_name.size() - 1; i > 0; i--)
				{

					if (tex_file_name.at(i) == '/' || tex_file_name.at(i) == '\\')
					{
						slash = i;
						break;
					}
					size++;
				}
				tex_file_name = tex_file_name.substr(slash + 1, size);
			}
			else if (tex_file_name[0] == 'D')
			{
				int size = 0;
				int slash = 0;
				for (int i = tex_file_name.size() - 1; i > 0; i--)
				{

					if (tex_file_name.at(i) == '/' || tex_file_name.at(i) == '\\')
					{
						slash = i;
						break;
					}
					size++;
				}
				tex_file_name = tex_file_name.substr(slash + 1, size);
			}
			else if (tex_file_name[0] == 'G')
			{
				int size = 0;
				int slash = 0;
				for (int i = tex_file_name.size() - 1; i > 0; i--)
				{

					if (tex_file_name.at(i) == '/' || tex_file_name.at(i) == '\\')
					{
						slash = i;
						break;
					}
					size++;
				}
				tex_file_name = tex_file_name.substr(slash + 1, size);
			}
			else if (tex_file_name[0] == 'E')
			{
				int size = 0;
				int slash = 0;
				for (int i = tex_file_name.size() - 1; i > 0; i--)
				{

					if (tex_file_name.at(i) == '/' || tex_file_name.at(i) == '\\')
					{
						slash = i;
						break;
					}
					size++;
				}
				tex_file_name = tex_file_name.substr(slash + 1, size);
			}
			else if (tex_file_name[0] == '.')
			{
				//tex_file_name = tex_file_name.substr(3, tex_file_name.size() - 3);
				int size = 0;
				int slash = 0;
				for (int i = 0; i < static_cast<int>(tex_file_name.size()) - 1; i++)
				{
					if (tex_file_name.at(i) != '/' && tex_file_name.at(i) != '\\' && tex_file_name.at(i) != '.')
					{
						slash = i;
						break;
					}
					size++;
				}
				tex_file_name = tex_file_name.substr(slash, tex_file_name.size() + 1 - size);
			}
			//������쐬
			tex_name = fbx_name.substr(0, fbx_name.find_last_of('/'));
			tex_name += '/';
			tex_name += tex_file_name;
			//}

			//string��wstring�ɁBwstring��wchar_t*�ɁB
			std::wstring tex_wname = to_wstringModel(tex_name);
			const wchar_t* w_tex = tex_wname.data();



			Microsoft::WRL::ComPtr<ID3D11Resource> resource;
			HRESULT hr = DirectX::CreateWICTextureFromFile(FRAMEWORK.device.Get(), w_tex, resource.GetAddressOf(), dst.shader_resource_view.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}
	}

	// ���b�V��
	m_meshes.resize(m_data->meshes.size());
	for (size_t mesh_index = 0; mesh_index < m_meshes.size(); ++mesh_index)
	{
		auto&& src = m_data->meshes.at(mesh_index);
		auto&& dst = m_meshes.at(mesh_index);

		// ���_�o�b�t�@
		{
			D3D11_BUFFER_DESC buffer_desc = {};
			D3D11_SUBRESOURCE_DATA subresource_data = {};

			buffer_desc.ByteWidth = static_cast<UINT>(sizeof(ModelData::Vertex) * src.vertices.size());
			//buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
			buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			buffer_desc.CPUAccessFlags = 0;
			buffer_desc.MiscFlags = 0;
			buffer_desc.StructureByteStride = 0;
			subresource_data.pSysMem = src.vertices.data();
			subresource_data.SysMemPitch = 0;
			subresource_data.SysMemSlicePitch = 0;

			HRESULT hr = FRAMEWORK.device.Get()->CreateBuffer(&buffer_desc, &subresource_data, dst.vertex_buffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}

		// �C���f�b�N�X�o�b�t�@
		{
			D3D11_BUFFER_DESC buffer_desc = {};
			D3D11_SUBRESOURCE_DATA subresource_data = {};

			buffer_desc.ByteWidth = static_cast<UINT>(sizeof(u_int) * src.indices.size());
			//buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
			buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			buffer_desc.CPUAccessFlags = 0;
			buffer_desc.MiscFlags = 0;
			buffer_desc.StructureByteStride = 0;
			subresource_data.pSysMem = src.indices.data();
			subresource_data.SysMemPitch = 0; //Not use for index buffers.
			subresource_data.SysMemSlicePitch = 0; //Not use for index buffers.
			HRESULT hr = FRAMEWORK.device.Get()->CreateBuffer(&buffer_desc, &subresource_data, dst.index_buffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}

		dst.node_index = src.node_index;

		// �T�u�Z�b�g
		dst.subsets.resize(src.subsets.size());
		for (size_t subset_index = 0; subset_index < src.subsets.size(); ++subset_index)
		{
			auto&& src_subset = src.subsets.at(subset_index);
			auto&& dst_subset = dst.subsets.at(subset_index);

			dst_subset.start_index = src_subset.start_index;
			dst_subset.index_count = src_subset.index_count;
			dst_subset.material = &m_materials.at(src_subset.material_index);
			dst_subset.material_index = static_cast<Material_Attribute>(src_subset.material_index);
		}

		// �{�[���ϊ��s��p
		dst.node_indices.resize(src.node_indices.size());
		::memcpy(dst.node_indices.data(), src.node_indices.data(), sizeof(int) * dst.node_indices.size());

		dst.inverse_transforms.resize(src.inverse_transforms.size());
		for (size_t index = 0; index < dst.inverse_transforms.size(); ++index)
		{
			dst.inverse_transforms.at(index) = &src.inverse_transforms.at(index);
		}
	}
}


Model::Model(const char* filename, std::shared_ptr<Texture> tex_main, std::shared_ptr<Texture> tex_face)
{
	//m_data = std::move(data);

	m_data = std::make_unique<ModelData>();

	Load(filename);

	// �}�e���A��
	m_materials.resize(m_data->materials.size());
	for (size_t material_index = 0; material_index < m_materials.size(); ++material_index)
	{
		auto&& src = m_data->materials.at(material_index);
		auto&& dst = m_materials.at(material_index);

		dst.color = src.color;

		// �e�N�X�`���ǂݍ���
		color_texture_main = tex_main;
		color_texture_face = tex_face;
	}

	// ���b�V��
	m_meshes.resize(m_data->meshes.size());
	for (size_t mesh_index = 0; mesh_index < m_meshes.size(); ++mesh_index)
	{
		auto&& src = m_data->meshes.at(mesh_index);
		auto&& dst = m_meshes.at(mesh_index);

		// ���_�o�b�t�@
		{
			D3D11_BUFFER_DESC buffer_desc = {};
			D3D11_SUBRESOURCE_DATA subresource_data = {};

			buffer_desc.ByteWidth = static_cast<UINT>(sizeof(ModelData::Vertex) * src.vertices.size());
			//buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
			buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			buffer_desc.CPUAccessFlags = 0;
			buffer_desc.MiscFlags = 0;
			buffer_desc.StructureByteStride = 0;
			subresource_data.pSysMem = src.vertices.data();
			subresource_data.SysMemPitch = 0;
			subresource_data.SysMemSlicePitch = 0;

			HRESULT hr = FRAMEWORK.device.Get()->CreateBuffer(&buffer_desc, &subresource_data, dst.vertex_buffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}

		// �C���f�b�N�X�o�b�t�@
		{
			D3D11_BUFFER_DESC buffer_desc = {};
			D3D11_SUBRESOURCE_DATA subresource_data = {};

			buffer_desc.ByteWidth = static_cast<UINT>(sizeof(u_int) * src.indices.size());
			//buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
			buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			buffer_desc.CPUAccessFlags = 0;
			buffer_desc.MiscFlags = 0;
			buffer_desc.StructureByteStride = 0;
			subresource_data.pSysMem = src.indices.data();
			subresource_data.SysMemPitch = 0; //Not use for index buffers.
			subresource_data.SysMemSlicePitch = 0; //Not use for index buffers.
			HRESULT hr = FRAMEWORK.device.Get()->CreateBuffer(&buffer_desc, &subresource_data, dst.index_buffer.GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}

		dst.node_index = src.node_index;

		// �T�u�Z�b�g
		dst.subsets.resize(src.subsets.size());
		for (size_t subset_index = 0; subset_index < src.subsets.size(); ++subset_index)
		{
			auto&& src_subset = src.subsets.at(subset_index);
			auto&& dst_subset = dst.subsets.at(subset_index);

			dst_subset.start_index = src_subset.start_index;
			dst_subset.index_count = src_subset.index_count;
			dst_subset.material = &m_materials.at(src_subset.material_index);
			dst_subset.material_index = static_cast<Material_Attribute>(src_subset.material_index);
		}

		// �{�[���ϊ��s��p
		dst.node_indices.resize(src.node_indices.size());
		::memcpy(dst.node_indices.data(), src.node_indices.data(), sizeof(int) * dst.node_indices.size());

		dst.inverse_transforms.resize(src.inverse_transforms.size());
		for (size_t index = 0; index < dst.inverse_transforms.size(); ++index)
		{
			dst.inverse_transforms.at(index) = &src.inverse_transforms.at(index);
		}
	}
}

// FbxDouble2 �� XMFLOAT2
inline DirectX::XMFLOAT2 FbxDouble2ToFloat2(const FbxDouble2& fbx_value)
{
	return DirectX::XMFLOAT2(
		static_cast<float>(fbx_value[0]),
		static_cast<float>(fbx_value[1])
	);
}

// FbxDouble3 �� XMFLOAT3
inline DirectX::XMFLOAT3 FbxDouble3ToFloat3(const FbxDouble3& fbx_value)
{
	return DirectX::XMFLOAT3(
		static_cast<float>(fbx_value[0]),
		static_cast<float>(fbx_value[1]),
		static_cast<float>(fbx_value[2])
	);
}

// FbxDouble4 �� XMFLOAT3
inline DirectX::XMFLOAT3 FbxDouble4ToFloat3(const FbxDouble4& fbx_value)
{
	return DirectX::XMFLOAT3(
		static_cast<float>(fbx_value[0]),
		static_cast<float>(fbx_value[1]),
		static_cast<float>(fbx_value[2])
	);
}

// FbxDouble4 �� XMFLOAT4
inline DirectX::XMFLOAT4 FbxDouble4ToFloat4(const FbxDouble4& fbx_value)
{
	return DirectX::XMFLOAT4(
		static_cast<float>(fbx_value[0]),
		static_cast<float>(fbx_value[1]),
		static_cast<float>(fbx_value[2]),
		static_cast<float>(fbx_value[3])
	);
}

// FbxDouble4 �� XMFLOAT4
inline DirectX::XMFLOAT4X4 FbxAMatrixToFloat4x4(const FbxAMatrix& fbx_value)
{
	return DirectX::XMFLOAT4X4(
		static_cast<float>(fbx_value[0][0]),
		static_cast<float>(fbx_value[0][1]),
		static_cast<float>(fbx_value[0][2]),
		static_cast<float>(fbx_value[0][3]),
		static_cast<float>(fbx_value[1][0]),
		static_cast<float>(fbx_value[1][1]),
		static_cast<float>(fbx_value[1][2]),
		static_cast<float>(fbx_value[1][3]),
		static_cast<float>(fbx_value[2][0]),
		static_cast<float>(fbx_value[2][1]),
		static_cast<float>(fbx_value[2][2]),
		static_cast<float>(fbx_value[2][3]),
		static_cast<float>(fbx_value[3][0]),
		static_cast<float>(fbx_value[3][1]),
		static_cast<float>(fbx_value[3][2]),
		static_cast<float>(fbx_value[3][3])
	);
}

// FBX�t�@�C�����烂�f���f�[�^�ǂݍ���
bool Model::Load(const char* filename)
{
	FbxManager* fbx_manager = FbxManager::Create();

	// FBX�ɑ΂�����o�͂��`����
	FbxIOSettings* fbx_ios = FbxIOSettings::Create(fbx_manager, IOSROOT);	// ���ʂȗ��R���Ȃ�����IOSROOT���w��
	fbx_manager->SetIOSettings(fbx_ios);

	// �C���|�[�^�𐶐�
	FbxImporter* fbx_importer = FbxImporter::Create(fbx_manager, "");
	bool result = fbx_importer->Initialize(filename, -1, fbx_manager->GetIOSettings());	// -1�Ńt�@�C���t�H�[�}�b�g��������
	if (result)
	{
		// Scene�I�u�W�F�N�g��FBX�t�@�C�����̏��𗬂�����
		FbxScene* fbx_scene = FbxScene::Create(fbx_manager, "scene");
		fbx_importer->Import(fbx_scene);
		fbx_importer->Destroy();	// �V�[���𗬂����񂾂�Importer�͉������OK

		// �W�I���g�����O�p�`�����Ă���
		FbxGeometryConverter fbx_geometry_converter(fbx_manager);
		fbx_geometry_converter.Triangulate(fbx_scene, true);
		fbx_geometry_converter.RemoveBadPolygonsFromMeshes(fbx_scene);

		// �f�B���N�g���p�X�擾
		char dirname[256];
		::_splitpath_s(filename, nullptr, 0, dirname, 256, nullptr, 0, nullptr, 0);

		// ���f���\�z
		result = BuildModel(dirname, fbx_scene);
	}
	else
	{
		assert("FbxImporter::Initialize() : Failed!!");
	}

	// �}�l�[�W�����
	fbx_manager->Destroy();		// �֘A���邷�ׂẴI�u�W�F�N�g����������
	return result;
}

// FBX�V�[�����烂�f���f�[�^���\�z����
bool Model::BuildModel(const char* dirname, FbxScene* fbx_scene)
{
	FbxNode* fbx_root_node = fbx_scene->GetRootNode();

	BuildNodes(fbx_root_node, -1);
	BuildMeshes(fbx_root_node);
	BuildMaterials(dirname, fbx_scene);
	BuildAnimations(fbx_scene);

	fbx_root_node->Destroy();

	return true;
}

// FBX�m�[�h���ċA�I�ɒH���ăf�[�^���\�z����
void Model::BuildNodes(FbxNode* fbx_node, int parent_node_index)
{
	FbxNodeAttribute* fbx_node_attribute = fbx_node->GetNodeAttribute();
	FbxNodeAttribute::EType fbx_node_attribute_type = FbxNodeAttribute::EType::eUnknown;


	if (fbx_node_attribute != nullptr)
	{
		fbx_node_attribute_type = fbx_node_attribute->GetAttributeType();
	}

	switch (fbx_node_attribute_type)
	{
	case FbxNodeAttribute::eUnknown:
	case FbxNodeAttribute::eNull:
	case FbxNodeAttribute::eMarker:
	case FbxNodeAttribute::eMesh:
	case FbxNodeAttribute::eSkeleton:
		BuildNode(fbx_node, parent_node_index);
		break;
	}

	// �ċA�I�Ɏq�m�[�h����������
	parent_node_index = static_cast<int>(m_data->nodes.size() - 1);
	for (int i = 0; i < fbx_node->GetChildCount(); ++i)
	{
		BuildNodes(fbx_node->GetChild(i), parent_node_index);
	}
}

// FBX�m�[�h����m�[�h�f�[�^���\�z����
void Model::BuildNode(FbxNode* fbx_node, int parent_node_index)
{
	FbxAMatrix& fbx_local_transform = fbx_node->EvaluateLocalTransform();

	ModelData::Node node;
	node.name = fbx_node->GetName();
	node.parent_index = parent_node_index;
	node.scale = FbxDouble4ToFloat3(fbx_local_transform.GetS());
	node.rotate = FbxDouble4ToFloat4(fbx_local_transform.GetQ());
	node.translate = FbxDouble4ToFloat3(fbx_local_transform.GetT());

	m_data->nodes.emplace_back(node);
}

// FBX�m�[�h���ċA�I�ɒH���ă��b�V���f�[�^���\�z����
void Model::BuildMeshes(FbxNode* fbx_node)
{
	FbxNodeAttribute* fbx_node_attribute = fbx_node->GetNodeAttribute();
	FbxNodeAttribute::EType fbx_node_attribute_type = FbxNodeAttribute::EType::eUnknown;

	if (fbx_node_attribute != nullptr)
	{
		fbx_node_attribute_type = fbx_node_attribute->GetAttributeType();
	}

	switch (fbx_node_attribute_type)
	{
	case FbxNodeAttribute::eMesh:
		BuildMesh(fbx_node, static_cast<FbxMesh*>(fbx_node_attribute));
		break;
	}

	// �ċA�I�Ɏq�m�[�h����������
	for (int i = 0; i < fbx_node->GetChildCount(); ++i)
	{
		BuildMeshes(fbx_node->GetChild(i));
	}
}

// FBX���b�V�����烁�b�V���f�[�^���\�z����
void Model::BuildMesh(FbxNode* fbx_node, FbxMesh* fbx_mesh)
{
	int fbx_control_points_count = fbx_mesh->GetControlPointsCount();
	int fbx_polygon_vertex_count = fbx_mesh->GetPolygonVertexCount();
	const int* fbx_polygon_vertices = fbx_mesh->GetPolygonVertices();

	int fbx_material_count = fbx_node->GetMaterialCount();
	int fbx_polygon_count = fbx_mesh->GetPolygonCount();

	m_data->meshes.emplace_back(ModelData::Mesh());
	ModelData::Mesh& mesh = m_data->meshes.back();
	mesh.indices.resize(fbx_polygon_count * 3);
	mesh.subsets.resize(fbx_material_count > 0 ? fbx_material_count : 1);
	mesh.node_index = FindNodeIndex(fbx_node->GetName());

	// �T�u�Z�b�g�̃}�e���A���ݒ�
	for (int fbx_material_index = 0; fbx_material_index < fbx_material_count; ++fbx_material_index)
	{
		const FbxSurfaceMaterial* fbx_surface_material = fbx_node->GetMaterial(fbx_material_index);

		ModelData::Subset& subset = mesh.subsets.at(fbx_material_index);
		subset.material_index = FindMaterialIndex(fbx_node->GetScene(), fbx_surface_material);
	}

	// �T�u�Z�b�g�̒��_�C���f�b�N�X�͈͐ݒ�
	if (fbx_material_count > 0)
	{
		for (int fbx_polygon_index = 0; fbx_polygon_index < fbx_polygon_count; ++fbx_polygon_index)
		{
			int fbx_material_index = fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(fbx_polygon_index);
			mesh.subsets.at(fbx_material_index).index_count += 3;
		}

		int offset = 0;
		for (ModelData::Subset& subset : mesh.subsets)
		{
			subset.start_index = offset;
			offset += subset.index_count;

			subset.index_count = 0;
		}
	}

	// ���_�e���̓f�[�^
	struct BoneInfluence
	{
		int use_count = 0;
		int indices[4] = { 0, 0, 0, 0 };
		float weights[4] = { 1.0f, 0.0f, 0.0f, 0.0f };

		void Add(int index, float weight)
		{
			indices[use_count] = index;
			weights[use_count] = weight;
			use_count++;
		}
	};
	// ���_�e���̓f�[�^�𒊏o����
	std::vector<BoneInfluence> bone_influences;
	{
		bone_influences.resize(fbx_control_points_count);

		FbxAMatrix fbx_geometric_transform(
			fbx_node->GetGeometricTranslation(FbxNode::eSourcePivot),
			fbx_node->GetGeometricRotation(FbxNode::eSourcePivot),
			fbx_node->GetGeometricScaling(FbxNode::eSourcePivot)
		);

		// �X�L�j���O�ɕK�v�ȏ����擾����
		int fbx_deformer_count = fbx_mesh->GetDeformerCount(FbxDeformer::eSkin);
		for (int fbx_deformer_index = 0; fbx_deformer_index < fbx_deformer_count; ++fbx_deformer_index)
		{
			FbxSkin* fbx_skin = static_cast<FbxSkin*>(fbx_mesh->GetDeformer(fbx_deformer_index, FbxDeformer::eSkin));

			int fbx_cluster_count = fbx_skin->GetClusterCount();
			for (int fbx_cluster_index = 0; fbx_cluster_index < fbx_cluster_count; ++fbx_cluster_index)
			{
				FbxCluster* fbx_cluster = fbx_skin->GetCluster(fbx_cluster_index);

				// ���_�e���̓f�[�^�𒊏o����
				{
					int fbx_cluster_control_point_indices_count = fbx_cluster->GetControlPointIndicesCount();
					const int* fbx_control_point_indices = fbx_cluster->GetControlPointIndices();
					const double* fbx_control_point_weights = fbx_cluster->GetControlPointWeights();

					for (int i = 0; i < fbx_cluster_control_point_indices_count; ++i)
					{
						BoneInfluence& bone_influence = bone_influences.at(fbx_control_point_indices[i]);

						bone_influence.Add(fbx_cluster_index, static_cast<float>(fbx_control_point_weights[i]));
					}
				}

				// �{�[���ϊ��s��p�̋t�s��̌v�Z������
				{
					// ���b�V����Ԃ��烏�[���h��Ԃւ̕ϊ��s��
					FbxAMatrix fbx_mesh_space_transform;
					fbx_cluster->GetTransformMatrix(fbx_mesh_space_transform);

					// �{�[����Ԃ��烏�[���h��Ԃւ̕ϊ��s��
					FbxAMatrix fbx_bone_space_transform;
					fbx_cluster->GetTransformLinkMatrix(fbx_bone_space_transform);

					// �{�[���t�s����v�Z����
					const char* fbx_name = fbx_node->GetName();
					FbxAMatrix fbx_inverse_transform = fbx_bone_space_transform.Inverse() * fbx_mesh_space_transform * fbx_geometric_transform;

					DirectX::XMFLOAT4X4 inverse_transform = FbxAMatrixToFloat4x4(fbx_inverse_transform);
					mesh.inverse_transforms.emplace_back(inverse_transform);

					int node_index = FindNodeIndex(fbx_cluster->GetLink()->GetName());
					mesh.node_indices.emplace_back(node_index);
				}
			}
		}
	}

	// UV�Z�b�g��
	FbxStringList fbx_uv_names;
	fbx_mesh->GetUVSetNames(fbx_uv_names);

	// ���_�f�[�^
	const FbxVector4* fbx_control_points = fbx_mesh->GetControlPoints();
	for (int fbx_polygon_index = 0; fbx_polygon_index < fbx_polygon_count; ++fbx_polygon_index)
	{
		// �|���S���ɓK�p����Ă���}�e���A���C���f�b�N�X���擾����
		int fbx_material_index = 0;
		if (fbx_material_count > 0)
		{
			fbx_material_index = fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(fbx_polygon_index);
		}

		ModelData::Subset& subset = mesh.subsets.at(fbx_material_index);
		const int index_offset = subset.start_index + subset.index_count;

		for (int fbx_vertex_index = 0; fbx_vertex_index < 3; ++fbx_vertex_index)
		{
			ModelData::Vertex vertex;

			int fbx_control_point_index = fbx_mesh->GetPolygonVertex(fbx_polygon_index, fbx_vertex_index);
			// Position
			{
				vertex.position = FbxDouble4ToFloat3(fbx_control_points[fbx_control_point_index]);
			}

			// Weight
			{
				BoneInfluence& bone_influence = bone_influences.at(fbx_control_point_index);
				vertex.bone_indices.x = bone_influence.indices[0];
				vertex.bone_indices.y = bone_influence.indices[1];
				vertex.bone_indices.z = bone_influence.indices[2];
				vertex.bone_indices.w = bone_influence.indices[3];
				vertex.bone_weights.x = bone_influence.weights[0];
				vertex.bone_weights.y = bone_influence.weights[1];
				vertex.bone_weights.z = bone_influence.weights[2];
				vertex.bone_weights.w = bone_influence.weights[3];
			}

			// Normal
			if (fbx_mesh->GetElementNormalCount() > 0)
			{
				FbxVector4 fbx_normal;
				fbx_mesh->GetPolygonVertexNormal(fbx_polygon_index, fbx_vertex_index, fbx_normal);
				vertex.normal = FbxDouble4ToFloat3(fbx_normal);
			}
			else
			{
				vertex.normal = DirectX::XMFLOAT3(0, 0, 0);
			}

			// Texcoord
			if (fbx_mesh->GetElementUVCount() > 0)
			{
				bool fbx_unmapped_uv;
				FbxVector2 fbx_uv;
				fbx_mesh->GetPolygonVertexUV(fbx_polygon_index, fbx_vertex_index, fbx_uv_names[0], fbx_uv, fbx_unmapped_uv);
				fbx_uv[1] = 1.0 - fbx_uv[1];
				vertex.texcoord = FbxDouble2ToFloat2(fbx_uv);
				/*if (vertex.texcoord.x > 1.0f)
				{
					int hoge = 0;
				}
				if (vertex.texcoord.y > 1.0f)
				{
					int hoge = 0;
				}*/
			}
			else
			{
				vertex.texcoord = DirectX::XMFLOAT2(0, 0);
			}

			mesh.indices.at(index_offset + fbx_vertex_index) = static_cast<int>(mesh.vertices.size());
			mesh.vertices.emplace_back(vertex);
		}

		subset.index_count += 3;
	}
}

// FBX�V�[������FBX�}�e���A������}�e���A���f�[�^���\�z����
void Model::BuildMaterials(const char* dirname, FbxScene* fbx_scene)
{
	int fbx_material_count = fbx_scene->GetMaterialCount();

	if (fbx_material_count > 0)
	{
		for (int fbx_material_index = 0; fbx_material_index < fbx_material_count; ++fbx_material_index)
		{
			FbxSurfaceMaterial* fbx_surface_material = fbx_scene->GetMaterial(fbx_material_index);

			BuildMaterial(dirname, fbx_surface_material);
		}
	}
	else
	{
		ModelData::Material material;
		material.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		m_data->materials.emplace_back(material);
	}
}

// FBX�}�e���A������}�e���A���f�[�^���\�z����
void Model::BuildMaterial(const char* dirname, FbxSurfaceMaterial* fbx_surface_material)
{
	bool ret = false;

	ModelData::Material material;

	FbxProperty fbx_diffuse_property = fbx_surface_material->FindProperty(FbxSurfaceMaterial::sDiffuse);
	FbxProperty fbx_diffuse_factor_property = fbx_surface_material->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
	if (fbx_diffuse_property.IsValid() && fbx_diffuse_factor_property.IsValid())
	{
		FbxDouble fbx_factor = fbx_diffuse_factor_property.Get<FbxDouble>();
		FbxDouble3 fbx_color = fbx_diffuse_property.Get<FbxDouble3>();

		material.color.x = static_cast<float>(fbx_color[0] * fbx_factor);
		material.color.y = static_cast<float>(fbx_color[1] * fbx_factor);
		material.color.z = static_cast<float>(fbx_color[2] * fbx_factor);
		material.color.w = 1.0f;
	}
#if 0
	FbxProperty fbx_transparency_factor_property = fbx_surface_material->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
	if (fbx_transparency_factor_property.IsValid())
	{
		FbxDouble fbx_factor = fbx_transparency_factor_property.Get<FbxDouble>();

		material.color.w = static_cast<float>(fbx_factor);
	}
#endif
	if (fbx_diffuse_property.IsValid())
	{
		int fbx_texture_count = fbx_diffuse_property.GetSrcObjectCount<FbxFileTexture>();
		if (fbx_texture_count > 0)
		{
			FbxFileTexture* fbx_texture = fbx_diffuse_property.GetSrcObject<FbxFileTexture>();

			/*char filename[256];
			::_makepath_s(filename, 256, nullptr, dirname, fbx_texture->GetRelativeFileName(), nullptr);
			material.texture_filename = filename;*/
			const char* filename = fbx_texture->GetRelativeFileName();
			material.texture_filename = std::string(filename);
		}
	}

	m_data->materials.emplace_back(material);
}

// �A�j���[�V�����f�[�^���\�z
void Model::BuildAnimations(FbxScene* fbx_scene)
{
	// ���ׂẴA�j���[�V���������擾
	FbxArray<FbxString*> fbx_anim_stack_names;
	fbx_scene->FillAnimStackNameArray(fbx_anim_stack_names);

	int fbx_animation_count = fbx_anim_stack_names.Size();
	for (int fbx_animation_index = 0; fbx_animation_index < fbx_animation_count; ++fbx_animation_index)
	{
		m_data->animations.emplace_back(ModelData::Animation());
		ModelData::Animation& animation = m_data->animations.back();

		// �A�j���[�V�����f�[�^�̃T���v�����O�ݒ�
		FbxTime::EMode fbx_time_mode = fbx_scene->GetGlobalSettings().GetTimeMode();
		FbxTime fbx_frame_time;
		fbx_frame_time.SetTime(0, 0, 0, 1, 0, fbx_time_mode);

		u_int sampling_rate = static_cast<u_int>(fbx_frame_time.GetFrameRate(fbx_time_mode));
		float sampling_time = 1.0f / sampling_rate;

		FbxString* fbx_anim_stack_name = fbx_anim_stack_names.GetAt(fbx_animation_index);
		FbxAnimStack* fbx_anim_stack = fbx_scene->FindMember<FbxAnimStack>(fbx_anim_stack_name->Buffer());

		// �Đ�����A�j���[�V�������w�肷��B
		fbx_scene->SetCurrentAnimationStack(fbx_anim_stack);

		// �A�j���[�V�����̍Đ��J�n���ԂƍĐ��I�����Ԃ��擾����
		FbxTakeInfo* fbx_take_info = fbx_scene->GetTakeInfo(fbx_anim_stack_name->Buffer());
		FbxTime fbx_start_time = fbx_take_info->mLocalTimeSpan.GetStart();
		FbxTime fbx_end_time = fbx_take_info->mLocalTimeSpan.GetStop();

		// ���o����f�[�^��60�t���[����ŃT���v�����O����
		FbxTime fbx_sampling_step;
		fbx_sampling_step.SetTime(0, 0, 1, 0, 0, fbx_time_mode);
		fbx_sampling_step = static_cast<FbxLongLong>(fbx_sampling_step.Get() * sampling_time);

		int start_frame = static_cast<int>(fbx_start_time.Get() / fbx_sampling_step.Get());
		int end_frame = static_cast<int>(fbx_end_time.Get() / fbx_sampling_step.Get());
		int frame_count = static_cast<int>((fbx_end_time.Get() - fbx_start_time.Get()) / fbx_sampling_step.Get());

		// �A�j���[�V�����̑ΏۂƂȂ�m�[�h��񋓂���
		std::vector<FbxNode*> fbx_nodes;
		std::function<void(FbxNode*)> traverse = [&](FbxNode* fbx_node)
		{
			FbxNodeAttribute* fbx_node_attribute = fbx_node->GetNodeAttribute();
			FbxNodeAttribute::EType fbx_node_attribute_type = FbxNodeAttribute::EType::eUnknown;
			if (fbx_node_attribute != nullptr)
			{
				fbx_node_attribute_type = fbx_node_attribute->GetAttributeType();
			}

			switch (fbx_node_attribute_type)
			{
			case FbxNodeAttribute::eUnknown:
			case FbxNodeAttribute::eNull:
			case FbxNodeAttribute::eMarker:
			case FbxNodeAttribute::eMesh:
			case FbxNodeAttribute::eSkeleton:
				fbx_nodes.emplace_back(fbx_node);
				break;
			}

			for (int i = 0; i < fbx_node->GetChildCount(); i++)
			{
				traverse(fbx_node->GetChild(i));
			}
		};
		traverse(fbx_scene->GetRootNode());

		// �A�j���[�V�����f�[�^�𒊏o����
		animation.seconds_length = frame_count * sampling_time;
		animation.keyframes.resize(frame_count + 1);

		float seconds = 0.0f;
		ModelData::Keyframe* keyframe = animation.keyframes.data();
		size_t fbx_node_count = fbx_nodes.size();
		FbxTime fbx_current_time = fbx_start_time;
		for (FbxTime fbx_current_time = fbx_start_time; fbx_current_time < fbx_end_time; fbx_current_time += fbx_sampling_step, ++keyframe)
		{
			// �L�[�t���[�����̎p���f�[�^�����o���B
			keyframe->seconds = seconds;
			keyframe->node_keys.resize(fbx_node_count);
			for (size_t fbx_node_index = 0; fbx_node_index < fbx_node_count; ++fbx_node_index)
			{
				ModelData::NodeKeyData& key_data = keyframe->node_keys.at(fbx_node_index);
				FbxNode* fbx_node = fbx_nodes.at(fbx_node_index);

				// �w�莞�Ԃ̃��[�J���s�񂩂�X�P�[���l�A��]�l�A�ړ��l�����o���B
				const FbxAMatrix& fbx_local_transform = fbx_node->EvaluateLocalTransform(fbx_current_time);

				key_data.scale = FbxDouble4ToFloat3(fbx_local_transform.GetS());
				key_data.rotate = FbxDouble4ToFloat4(fbx_local_transform.GetQ());
				key_data.translate = FbxDouble4ToFloat3(fbx_local_transform.GetT());
			}
			seconds += sampling_time;
		}
		fbx_nodes.clear();
	}

	// ��n��
	for (int i = 0; i < fbx_animation_count; i++)
	{
		delete fbx_anim_stack_names[i];
	}
}

// �m�[�h�C���f�b�N�X���擾����
int Model::FindNodeIndex(const char* name)
{
	for (size_t i = 0; i < m_data->nodes.size(); ++i)
	{
		if (m_data->nodes[i].name == name)
		{
			return static_cast<int>(i);
		}
	}
	return -1;
}

// �}�e���A���C���f�b�N�X���擾����
int Model::FindMaterialIndex(FbxScene* fbx_scene, const FbxSurfaceMaterial* fbx_surface_material)
{
	int fbx_material_count = fbx_scene->GetMaterialCount();

	for (int i = 0; i < fbx_material_count; ++i)
	{
		if (fbx_scene->GetMaterial(i) == fbx_surface_material)
		{
			return i;
		}
	}
	return -1;
}

void Model::Serialize()
{
	std::string name = Serial_name + std::string("model");
	std::ofstream ofs(name.c_str(),std::ios_base::out|std::ios_base::binary);
	std::stringstream ss;
	{
		cereal::BinaryOutputArchive o_archive(ss);
		o_archive(cereal::make_nvp("root", m_data));
	}

	ofs << ss.str();
	ofs.close();
	ss.clear();
	//std::cout << ss.str() << std::endl;

}

void Model::SerialNameGet(const char* filename)
{
	//�V���A���C�Y�ŏ����o���t�@�C���̃f�B���N�g���Ɩ��O�����߂�֐�
	Serial_name = std::string(filename);


	int size = 0;
	for (int i = Serial_name.size() - 1; i > 0; i--)
	{

		if (Serial_name.at(i) == '.')
		{
			break;
		}
		size++;
	}
	//�u.�v�܂Ő؂�o���̂Ŏg�p����ۂ͊g���q�݂̂���
	int sizes = Serial_name.size() - size;
	Serial_name = Serial_name.substr(0, sizes);
}

bool Model::FileCheck()
{
	//�V���A���C�Y���ꂽ�t�@�C�������邩�m�F����֐�
	std::string file = Serial_name + std::string("model");

	std::ifstream ifs(file.c_str());

	if (ifs.is_open())
	{
		//�t�@�C����������
		ifs.close();
		return true;
	}
	else
	{
		//�t�@�C�����Ȃ�����
		ifs.close();
		return false;
	}

	return false;
}

void Model::LoadSerial()
{
	//�o�C�i���t�@�C����ǂݍ���
	std::string name = Serial_name + std::string("model");
	std::ifstream ifs(name.c_str(), std::ios_base::in | std::ios_base::binary);
	std::stringstream ss;
	ss << ifs.rdbuf();
	{
		cereal::BinaryInputArchive i_archive(ss);
		i_archive(cereal::make_nvp("root", m_data));
	}

	ifs.close();
	ss.clear();
}

