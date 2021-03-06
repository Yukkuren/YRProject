#include"Skinned_mesh.h"
#include<fbxsdk.h>
#pragma comment(lib, "shlwapi.lib")
using namespace fbxsdk;
#include<functional>
#include"DirectXTK-master\Inc\WICTextureLoader.h"
#include<string>
#include<map>
#include<wrl.h>
#include<Shlwapi.h>
#include <codecvt>
#include "framework.h"
#include <locale>

using convert_t = std::codecvt_utf8<wchar_t>;
std::wstring_convert<convert_t, wchar_t> strconverter;

std::string to_string(std::wstring wstr)
{
	return strconverter.to_bytes(wstr);
}

std::wstring to_wstring(std::string str)
{
	return strconverter.from_bytes(str);
}

namespace ALL_SkinSet
{
	static std::map < std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> > tex;
	HRESULT TextureLoad(const wchar_t* wchar, D3D11_TEXTURE2D_DESC *texture2d_desc, ID3D11ShaderResourceView **pShader)
	{
		HRESULT hr = S_OK;

		ID3D11Resource *pResouse;
		//std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>::iterator it = tex.find(wchar);
		auto it = tex.find(wchar);
		if (it != tex.end())
		{
			*pShader = it->second.Get();
			(*pShader)->AddRef();
			(*pShader)->GetResource(&pResouse);
		}

		else
		{
			hr = DirectX::CreateWICTextureFromFile(FRAMEWORK.device.Get(), wchar, &pResouse, pShader);
			std::wstring extension = PathFindExtensionW(wchar);
			tex.insert(std::make_pair(wchar, *pShader));
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}
		ID3D11Texture2D *texture2d;
		hr = pResouse->QueryInterface(&texture2d);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		texture2d->GetDesc(texture2d_desc);

		texture2d->Release();
		pResouse->Release();

		return hr;
	}
}

//UNIT.22
void fetch_bone_matrices(FbxMesh *fbx_mesh, std::vector<Skinned_mesh::bone> &skeletal, FbxTime time) {
	const int number_of_deformers = fbx_mesh->GetDeformerCount(FbxDeformer::eSkin);  for (int index_of_deformer = 0; index_of_deformer < number_of_deformers; ++index_of_deformer) {
		FbxSkin *skin = static_cast<FbxSkin *>(fbx_mesh->GetDeformer(index_of_deformer, FbxDeformer::eSkin));

		const int number_of_clusters = skin->GetClusterCount();
		skeletal.resize(number_of_clusters);
		for (int index_of_cluster = 0; index_of_cluster < number_of_clusters; ++index_of_cluster)
		{
			Skinned_mesh::bone &bone = skeletal.at(index_of_cluster);

			FbxCluster *cluster = skin->GetCluster(index_of_cluster);
			FbxAMatrix reference_global_init_position;
			cluster->GetTransformMatrix(reference_global_init_position);

			// this matrix trnasforms coordinates of the initial pose from bone space to global space
			FbxAMatrix cluster_global_init_position;
			cluster->GetTransformLinkMatrix(cluster_global_init_position);

			// this matrix trnasforms coordinates of the current pose from bone space to global space
			FbxAMatrix cluster_global_current_position;
			cluster_global_current_position = cluster->GetLink()->EvaluateGlobalTransform(time);

			// this matrix trnasforms coordinates of the current pose from mesh space to global space
			FbxAMatrix reference_global_current_position;
			reference_global_current_position = fbx_mesh->GetNode()->EvaluateGlobalTransform(time);

			// Matrices are defined using the Column Major scheme. When a FbxAMatrix represents a transformation
			// (translation, rotation and scale), the last row of the matrix represents the translation part of the
			// transformation.
			FbxAMatrix transform = reference_global_current_position.Inverse() *cluster_global_current_position
				* cluster_global_init_position.Inverse() * reference_global_init_position;

			// convert FbxAMatrix(transform) to XMDLOAT4X4(bone.transform)

			for (int row = 0; row < 4; ++row)
			{
				for (int col = 0; col < 4; ++col)
				{
					bone.transform.m[row][col] = static_cast<float>(transform.Get(row, col));
				}
			}
		}
	}
}

void fetch_bone_influences(const FbxMesh *fbx_mesh, std::vector<bone_influences_per_control_point> &influences) {
	const int number_of_control_points = fbx_mesh->GetControlPointsCount();
	influences.resize(number_of_control_points);

	const int number_of_deformers = fbx_mesh->GetDeformerCount(FbxDeformer::eSkin);
	for (int index_of_deformer = 0; index_of_deformer < number_of_deformers; ++index_of_deformer)
	{
		FbxSkin *skin = static_cast<FbxSkin *>(fbx_mesh->GetDeformer(index_of_deformer, FbxDeformer::eSkin));

		const int number_of_clusters = skin->GetClusterCount();
		for (int index_of_cluster = 0; index_of_cluster < number_of_clusters; ++index_of_cluster)
		{
			FbxCluster* cluster = skin->GetCluster(index_of_cluster);

			const int number_of_control_point_indices = cluster->GetControlPointIndicesCount();
			const int *array_of_control_point_indices = cluster->GetControlPointIndices();
			const double *array_of_control_point_weights = cluster->GetControlPointWeights();

			for (int i = 0; i < number_of_control_point_indices; ++i)
			{
				bone_influences_per_control_point &influences_per_control_point = influences.at(array_of_control_point_indices[i]);
				bone_influence influence;
				influence.index = index_of_cluster;
				influence.weight = static_cast<float>(array_of_control_point_weights[i]);
				influences_per_control_point.push_back(influence);
			}
		}
	}
}

//UNIT.23
void fetch_animations(FbxMesh *fbx_mesh, Skinned_mesh::Skeletal_animation &skeletal_animation, u_int sampling_rate = 0)
{
	// Get the list of all the animation stack.
	FbxArray<FbxString *> array_of_animation_stack_names;
	fbx_mesh->GetScene()->FillAnimStackNameArray(array_of_animation_stack_names);

	// Get the number of animations.
	int number_of_animations = array_of_animation_stack_names.Size();

	if (number_of_animations > 0)
	{
		// Get the FbxTime per animation's frame.
		FbxTime::EMode time_mode = fbx_mesh->GetScene()->GetGlobalSettings().GetTimeMode();
		FbxTime frame_time;
		frame_time.SetTime(0, 0, 0, 1, 0, time_mode);

		sampling_rate = sampling_rate > 0 ? sampling_rate : static_cast<u_int>(frame_time.GetFrameRate(time_mode));
		float sampling_time = 1.0f / sampling_rate;
		skeletal_animation.sampling_time = sampling_time;
		skeletal_animation.animation_tick = 0.0f;

		FbxString *animation_stack_name = array_of_animation_stack_names.GetAt(0);
		FbxAnimStack * current_animation_stack = fbx_mesh->GetScene()->FindMember<FbxAnimStack>(animation_stack_name->Buffer());
		fbx_mesh->GetScene()->SetCurrentAnimationStack(current_animation_stack);

		FbxTakeInfo *take_info = fbx_mesh->GetScene()->GetTakeInfo(animation_stack_name->Buffer());
		FbxTime start_time = take_info->mLocalTimeSpan.GetStart();
		FbxTime end_time = take_info->mLocalTimeSpan.GetStop();

		FbxTime sampling_step;
		sampling_step.SetTime(0, 0, 1, 0, 0, time_mode);
		sampling_step = static_cast<FbxLongLong>(static_cast<float>(sampling_step.Get()) * sampling_time);
		for (FbxTime current_time = start_time; current_time < end_time; current_time += sampling_step)
		{
			Skinned_mesh::skeletal skeletal;
			fetch_bone_matrices(fbx_mesh, skeletal, current_time);
			skeletal_animation.push_back(skeletal);
		}
	}
	for (int i = 0; i < number_of_animations; i++)
	{
		delete array_of_animation_stack_names[i];
	}
	//※sampling_rate は 1 秒あたりにサンプリングする回数、０の場合は FBX データのアニメーションフレームレートを使う
}

bool Fbx_Tex_LevelCheck(std::string* fbx_name, std::string* tex_file_name)
{
	//--------------------------------------------------------------------------------------
	//			テクスチャファイルがfbxファイルと同じ場所にあるかどうかの判定
	//--------------------------------------------------------------------------------------
	//・手順１：	fbxファイル読み込み時の文字列から「/」の位置を後から判定し、
	//			2つめの場所から最後の「/」までの文字列を抜き出す
	//・手順２：	テクスチャの文字列にも同じことをし、一致していた場合切り離しと合成の処理を行う
	//--------------------------------------------------------------------------------------

	//手順１
	int slash = 0;
	int first_num = 0;
	int second_num = 0;
	for (int i = fbx_name->size()-1; i > 0; i--)
	{
		if (fbx_name->at(i) == '/')
		{
			slash++;
		}
		if (slash == 1)
		{
			first_num++;
		}
		if (slash == 2)
		{
			second_num = i;
			break;
		}
	}
	std::string f_name_space = fbx_name->substr(second_num + 1, first_num - 1);

	//手順２
	slash = 0;
	first_num = 0;
	second_num = 0;
	for (int i = tex_file_name->size()-1; i > 0; i--)
	{
		if (tex_file_name->at(i) == '/' || tex_file_name->at(i) == '\\')
		{
			slash++;
		}
		if (slash == 1)
		{
			first_num++;
		}
		if (slash == 2)
		{
			second_num = i;
			break;
		}
	}
	std::string t_name_space = tex_file_name->substr(second_num, first_num);

	//同じ階層にあるか判定
	if (f_name_space == t_name_space)
	{
		return true;
	}

	return false;
}


Skinned_mesh::Skinned_mesh(const char* fbx_filename, u_int sampling_rate)
{
	//FBXにテクスチャが設定されている場合
	Load(fbx_filename,sampling_rate);
}


Skinned_mesh::Skinned_mesh(const char* fbx_filename, std::shared_ptr<Texture> tex, u_int sampling_rate)
{
	//手動でテクスチャを指定して読み込む場合
	Load(fbx_filename,tex,sampling_rate);
}



bool Skinned_mesh::Load(const char *fbx_filename, u_int sampling_rate)
{
	//FBXに設定されているテクスチャを読み込む
	HRESULT hr = S_OK;
	//Create the FBX SDK manager
	FbxManager* manager = FbxManager::Create();

	// Create an IOSettings object. IOSROOT is defined in Fbxiosettingspath.h.
	manager->SetIOSettings(FbxIOSettings::Create(manager, IOSROOT));


	// Create an importer.
	FbxImporter* importer = FbxImporter::Create(manager, "");


	// Initialize the importer.
	bool import_status = false;
	import_status = importer->Initialize(fbx_filename, -1, manager->GetIOSettings());
	_ASSERT_EXPR_A(import_status, importer->GetStatus().GetErrorString());


	// Create a new scene so it can be populated by the imported file.
	FbxScene* scene = FbxScene::Create(manager, "");


	// Import the contents of the file into the scene.
	import_status = importer->Import(scene);
	_ASSERT_EXPR_A(import_status, importer->GetStatus().GetErrorString());
	importer->Destroy();



	// Convert mesh, NURBS and patch into triangle mesh
	fbxsdk::FbxGeometryConverter geometry_converter(manager);
	geometry_converter.Triangulate(scene, /*replace*/true);



	// Fetch node attributes and materials under this node recursively. Currently only mesh.
	std::vector<FbxNode*> fetched_meshes;
	std::vector<FbxNode*> fetched_skeleton;
	std::function<void(FbxNode*)> traverse = [&](FbxNode* node) {
		if (node) {
			FbxNodeAttribute *fbx_node_attribute = node->GetNodeAttribute();
			if (fbx_node_attribute) {
				switch (fbx_node_attribute->GetAttributeType()) {
				case FbxNodeAttribute::eMesh:
					fetched_meshes.push_back(node);
					break;
				case FbxNodeAttribute::eSkeleton:
					fetched_skeleton.push_back(node);
					break;
				}
			}
			for (int i = 0; i < node->GetChildCount(); i++)
				traverse(node->GetChild(i));
		}
	};
	traverse(scene->GetRootNode());

	meshes.resize(fetched_meshes.size());



	for (size_t i = 0; i < fetched_meshes.size(); i++)
	{
		std::vector<Vertex> vertices; // Vertex buffer
		std::vector<u_int> indices;  // Index buffer
		FbxMesh *fbx_mesh = fetched_meshes.at(i)->GetMesh();

		std::vector<bone_influences_per_control_point> bone_influences;
		fetch_bone_influences(fbx_mesh, bone_influences);

		mesh &mesh = meshes.at(i);

		FbxAMatrix global_transform = fbx_mesh->GetNode()->EvaluateGlobalTransform(0);
		for (int row = 0; row < 4; row++)
		{
			for (int column = 0; column < 4; column++)
			{
				mesh.global_transform.m[row][column] = static_cast<float>(global_transform[row][column]);
			}
		}


		// Fetch mesh data
		u_int vertex_count = 0;
		const int number_of_materials = fbx_mesh->GetNode()->GetMaterialCount();
		const int number_of_polygons = fbx_mesh->GetPolygonCount();


		//UNIT.22
		// Fetch bone Matrices
		//FbxTime::EMode time_mode = fbx_mesh->GetScene()->GetGlobalSettings().GetTimeMode();
		//FbxTime frame_time;
		//frame_time.SetTime(0, 0, 0, 1, 0, time_mode);
		//fetch_bone_matrices(fbx_mesh, mesh.skeletal, frame_time * 20);	//pose at frame 20

		//UNIT.23
		fetch_animations(fbx_mesh, mesh.skeletal_animation,sampling_rate);

		mesh.subsets.resize(number_of_materials);//UNIT18
		if (number_of_materials < 1)
		{
			mesh.subsets.resize(1);
		}
		for (int index_of_material = 0; index_of_material < number_of_materials; ++index_of_material)
		{
			subset &subset = mesh.subsets.at(index_of_material);
			const FbxSurfaceMaterial *surface_material = fbx_mesh->GetNode()->GetMaterial(index_of_material);

			const FbxProperty property = surface_material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			const FbxProperty factor = surface_material->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
			if (property.IsValid() && factor.IsValid())
			{
				FbxDouble3 color = property.Get<FbxDouble3>();
				double f = factor.Get<FbxDouble>();
				subset.diffuse.color.x = static_cast<float>(color[0] * f);
				subset.diffuse.color.y = static_cast<float>(color[1] * f);
				subset.diffuse.color.z = static_cast<float>(color[2] * f);
				subset.diffuse.color.w = 1.0f;
			}
			if (property.IsValid())
			{
				const int number_of_textures = property.GetSrcObjectCount<FbxFileTexture>();
				if (number_of_textures)
				{
					const FbxFileTexture* file_texture = property.GetSrcObject<FbxFileTexture>();
					if (file_texture)
					{
						const char *filename = file_texture->GetRelativeFileName();
						std::string he = file_texture->GetRelativeFileName();
						//const char *filename2 = file_texture->GetFileName();
						/*if (filename2[0] == 'C')
						{
							int hoge = 0;
						}*/

						std::string fbx_name = fbx_filename;
						std::string tex_file_name = filename;
						std::string tex_name;
						//階層判定
						//if (Fbx_Tex_LevelCheck(&fbx_name, &tex_file_name))
						//{
						//	//同じ階層だった場合の処理を入れる
						//	int hoge = 0;
						//}
						//else
						//{
						//相対パスが絶対パスだった場合の処理(大体Maya)
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
							tex_file_name = tex_file_name.substr(slash+1, size);
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
						//文字列作成
						tex_name = fbx_name.substr(0, fbx_name.find_last_of('/'));
						tex_name += '/';
						tex_name += tex_file_name;
						//}

						//stringをwstringに。wstringをwchar_t*に。
						std::wstring tex_wname= to_wstring(tex_name);
						const wchar_t* w_tex = tex_wname.data();


						ALL_SkinSet::TextureLoad(w_tex, &texture2d_desc, &subset.diffuse.shader_resource_view);

						/*hr = DirectX::CreateWICTextureFromFile(device, fi, &pResouse, &diffuse.shader_resource_view);
						hr = pResouse->QueryInterface(&texture2d);
						_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
						texture2d->GetDesc(&texture2d_desc);
						texture2d->Release();
						pResouse->Release();*/
					}
				}
				else
				{
					const wchar_t* wc = L"./Data/Shader/decoi.png";
					ALL_SkinSet::TextureLoad(wc, &texture2d_desc, &subset.diffuse.shader_resource_view);
				}
			}
		}




		if (number_of_materials > 0)
		{
			//Count the faces of each material
			for (int index_of_polygon = 0; index_of_polygon < number_of_polygons; index_of_polygon++)
			{
				const u_int material_index = fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(index_of_polygon);
				mesh.subsets.at(material_index).index_count += 3;
			}

			//Record the offset (how many vertex)
			int offset = 0;
			for (subset &subset : mesh.subsets)
			{
				subset.index_start = offset;
				offset += subset.index_count;
				//This will be used as counter in the following produres. reset to zero
				subset.index_count = 0;
			}
		}

		const FbxVector4 *array_of_control_points = fbx_mesh->GetControlPoints();
		indices.resize(number_of_polygons * 3);//UNIT18
		for (int index_of_polygon = 0; index_of_polygon < number_of_polygons; index_of_polygon++)
		{
			//UNIT18
			//The material for current face
			int index_of_material = 0;
			if (number_of_materials > 0)
			{
				index_of_material = fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(index_of_polygon);
			}

			//Where should I save the vertex attribute index. according to the material
			subset &subset = mesh.subsets.at(index_of_material);
			const int indext_offset = subset.index_start + subset.index_count;

			for (int index_of_vertex = 0; index_of_vertex < 3; index_of_vertex++) {
				Vertex vertex;
				const int index_of_control_point = fbx_mesh->GetPolygonVertex(index_of_polygon, index_of_vertex);
				vertex.position.x = static_cast<float>(array_of_control_points[index_of_control_point][0]);
				vertex.position.y = static_cast<float>(array_of_control_points[index_of_control_point][1]);
				vertex.position.z = static_cast<float>(array_of_control_points[index_of_control_point][2]);
				bone_influences_per_control_point point;
				point = bone_influences.at(index_of_control_point);
				int i = 0;
				for (auto& p : point)
				{
					vertex.bone_indices[i] = p.index;
					vertex.bone_weights[i] = p.weight;
					const char* nalme = fbx_mesh->GetName();
					assert(i < MAX_BONE_INFLUENCES);
					if (i >= MAX_BONE_INFLUENCES)
					{
						break;
					}
					i++;
				}


				fbxsdk::FbxStringList uv_names;
				fbx_mesh->GetUVSetNames(uv_names);
				FbxVector2 uv;
				bool unmapped_uv;
				if (fbx_mesh->GetElementUVCount() > 0)
				{
					fbx_mesh->GetPolygonVertexUV(index_of_polygon, index_of_vertex, uv_names[0], uv, unmapped_uv);
					vertex.texcoord.x = static_cast<float>(uv[0]);
					vertex.texcoord.y = 1.0f-static_cast<float>(uv[1]);
				}
				if (fbx_mesh->GetElementNormalCount() > 0)
				{
					FbxVector4 normal;
					fbx_mesh->GetPolygonVertexNormal(index_of_polygon, index_of_vertex, normal);
					vertex.normal.x = static_cast<float>(normal[0]);
					vertex.normal.y = static_cast<float>(normal[1]);
					vertex.normal.z = static_cast<float>(normal[2]);
				}

				vertices.push_back(vertex);
				//indices.push_back(vertex_count);
				indices.at(indext_offset + index_of_vertex) = static_cast<u_int>(vertex_count);
				vertex_count += 1;
			}
			subset.index_count += 3;
		}

		indexsize = indices.size();


		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.ByteWidth = vertices.size()*sizeof(Skinned_mesh::Vertex);
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;// D3D11_USAGE_DYNAMIC;D3D11_USAGE_IMMUTABLE
		buffer_desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subresource = {};
		subresource.pSysMem = vertices.data();
		subresource.SysMemPitch = 0;
		subresource.SysMemSlicePitch = 0;

		hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, &mesh.vertex_buffer);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		//インデックス情報セット
		ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&subresource, sizeof(D3D11_SUBRESOURCE_DATA));
		buffer_desc.ByteWidth = indices.size() * sizeof(u_int);
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;// D3D11_USAGE_DYNAMIC;
		buffer_desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		subresource.pSysMem = indices.data();
		subresource.SysMemPitch = 0;
		subresource.SysMemSlicePitch = 0;

		hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, &mesh.index_buffer);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		//indexsize = indices2.size();

	}

	manager->Destroy();
	// Initialize Direct3D COM objects

	D3D11_RASTERIZER_DESC rasterizer_desc;

	rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	rasterizer_desc.FrontCounterClockwise = FALSE;
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0;
	rasterizer_desc.SlopeScaledDepthBias = 0;
	rasterizer_desc.DepthClipEnable = TRUE;
	rasterizer_desc.ScissorEnable = FALSE;
	rasterizer_desc.MultisampleEnable = FALSE;
	rasterizer_desc.AntialiasedLineEnable = FALSE;

	hr = FRAMEWORK.device->CreateRasterizerState(&rasterizer_desc, line_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_FRONT;
	rasterizer_desc.FrontCounterClockwise = FALSE;
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0;
	rasterizer_desc.SlopeScaledDepthBias = 0;
	rasterizer_desc.DepthClipEnable = TRUE;
	rasterizer_desc.ScissorEnable = FALSE;
	rasterizer_desc.MultisampleEnable = FALSE;
	rasterizer_desc.AntialiasedLineEnable = FALSE;
	hr = FRAMEWORK.device->CreateRasterizerState(&rasterizer_desc, filling_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_FRONT;
	rasterizer_desc.FrontCounterClockwise = TRUE;
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0;
	rasterizer_desc.SlopeScaledDepthBias = 0;
	rasterizer_desc.DepthClipEnable = TRUE;
	rasterizer_desc.ScissorEnable = FALSE;
	rasterizer_desc.MultisampleEnable = FALSE;
	rasterizer_desc.AntialiasedLineEnable = FALSE;
	hr = FRAMEWORK.device->CreateRasterizerState(&rasterizer_desc, filling_state_inverse.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//深度ステンシルステートオブジェクト生成
	D3D11_DEPTH_STENCIL_DESC depth_desc;

	depth_desc.DepthEnable = TRUE;
	depth_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_desc.DepthFunc = D3D11_COMPARISON_LESS;
	depth_desc.StencilEnable = FALSE;
	depth_desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depth_desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depth_desc.FrontFace.StencilFunc = depth_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth_desc.FrontFace.StencilDepthFailOp = depth_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_desc.FrontFace.StencilPassOp = depth_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_desc.FrontFace.StencilFailOp = depth_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;


	hr = FRAMEWORK.device->CreateDepthStencilState(&depth_desc, depth_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_BUFFER_DESC buffer_desc{};
	D3D11_SUBRESOURCE_DATA subresource = {};
	ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&subresource, sizeof(D3D11_SUBRESOURCE_DATA));
	buffer_desc.ByteWidth = sizeof(cbuffer);
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;/*
	subresource.pSysMem = constant_buffer;
	subresource.SysMemPitch = 0;
	subresource.SysMemSlicePitch = 0;*/

	//subresource.pSysMem = NULL;

	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_SAMPLER_DESC sampler_desc;
	//ID3D11SamplerState *samplestate;

	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MipLODBias = 0.0f;
	sampler_desc.MaxAnisotropy = 16;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	DirectX::XMFLOAT4 bColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	memcpy(sampler_desc.BorderColor, &bColor, sizeof(DirectX::XMFLOAT4));
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = FRAMEWORK.device->CreateSamplerState(&sampler_desc, sampler_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	return true;
}



bool Skinned_mesh::Load(const char* fbx_filename, std::shared_ptr<Texture> tex, u_int sampling_rate)
{
	//手動でテクスチャを読み込む
	HRESULT hr = S_OK;
	//Create the FBX SDK manager
	FbxManager* manager = FbxManager::Create();

	// Create an IOSettings object. IOSROOT is defined in Fbxiosettingspath.h.
	manager->SetIOSettings(FbxIOSettings::Create(manager, IOSROOT));


	// Create an importer.
	FbxImporter* importer = FbxImporter::Create(manager, "");


	// Initialize the importer.
	bool import_status = false;
	import_status = importer->Initialize(fbx_filename, -1, manager->GetIOSettings());
	_ASSERT_EXPR_A(import_status, importer->GetStatus().GetErrorString());


	// Create a new scene so it can be populated by the imported file.
	FbxScene* scene = FbxScene::Create(manager, "");


	// Import the contents of the file into the scene.
	import_status = importer->Import(scene);
	_ASSERT_EXPR_A(import_status, importer->GetStatus().GetErrorString());


	// Convert mesh, NURBS and patch into triangle mesh
	fbxsdk::FbxGeometryConverter geometry_converter(manager);
	geometry_converter.Triangulate(scene, /*replace*/true);


	// Fetch node attributes and materials under this node recursively. Currently only mesh.
	std::vector<FbxNode*> fetched_meshes;
	std::function<void(FbxNode*)> traverse = [&](FbxNode* node) {
		if (node) {
			FbxNodeAttribute* fbx_node_attribute = node->GetNodeAttribute();
			if (fbx_node_attribute) {
				switch (fbx_node_attribute->GetAttributeType()) {
				case FbxNodeAttribute::eMesh:
					fetched_meshes.push_back(node);
					break;
				}
			}
			for (int i = 0; i < node->GetChildCount(); i++)
				traverse(node->GetChild(i));
		}
	};
	traverse(scene->GetRootNode());

	int fetched_size = fetched_meshes.size();

	meshes.resize(fetched_meshes.size());
	for (size_t i = 0; i < fetched_meshes.size(); i++)
	{
		std::vector<Vertex> vertices; // Vertex buffer
		std::vector<u_int> indices;  // Index buffer
		FbxMesh* fbx_mesh = fetched_meshes.at(i)->GetMesh();

		std::vector<bone_influences_per_control_point> bone_influences;
		fetch_bone_influences(fbx_mesh, bone_influences);

		mesh& mesh = meshes.at(i);

		FbxAMatrix global_transform = fbx_mesh->GetNode()->EvaluateGlobalTransform(0);
		for (int row = 0; row < 4; row++)
		{
			for (int column = 0; column < 4; column++)
			{
				mesh.global_transform.m[row][column] = static_cast<float>(global_transform[row][column]);
			}
		}


		// Fetch mesh data
		u_int vertex_count = 0;
		const int number_of_materials = fbx_mesh->GetNode()->GetMaterialCount();
		const int number_of_polygons = fbx_mesh->GetPolygonCount();


		//UNIT.22
		// Fetch bone Matrices
		//FbxTime::EMode time_mode = fbx_mesh->GetScene()->GetGlobalSettings().GetTimeMode();
		//FbxTime frame_time;
		//frame_time.SetTime(0, 0, 0, 1, 0, time_mode);
		//fetch_bone_matrices(fbx_mesh, mesh.skeletal, frame_time * 20);	//pose at frame 20

		//UNIT.23
		fetch_animations(fbx_mesh, mesh.skeletal_animation,sampling_rate);

		mesh.subsets.resize(number_of_materials);//UNIT18
		if (number_of_materials < 1)
		{
			mesh.subsets.resize(1);
		}
		for (int index_of_material = 0; index_of_material < number_of_materials; ++index_of_material)
		{
			subset& subset = mesh.subsets.at(index_of_material);
			const FbxSurfaceMaterial* surface_material = fbx_mesh->GetNode()->GetMaterial(index_of_material);

			const FbxProperty property = surface_material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			const FbxProperty factor = surface_material->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
			if (property.IsValid() && factor.IsValid())
			{
				FbxDouble3 color = property.Get<FbxDouble3>();
				double f = factor.Get<FbxDouble>();
				subset.diffuse.color.x = static_cast<float>(color[0] * f);
				subset.diffuse.color.y = static_cast<float>(color[1] * f);
				subset.diffuse.color.z = static_cast<float>(color[2] * f);
				subset.diffuse.color.w = 1.0f;
			}
			if (property.IsValid())
			{
				const int number_of_textures = property.GetSrcObjectCount<FbxFileTexture>();
			}
		}

		if (number_of_materials > 0)
		{
			//Count the faces of each material
			for (int index_of_polygon = 0; index_of_polygon < number_of_polygons; index_of_polygon++)
			{
				const u_int material_index = fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(index_of_polygon);
				mesh.subsets.at(material_index).index_count += 3;
			}

			//Record the offset (how many vertex)
			int offset = 0;
			for (subset& subset : mesh.subsets)
			{
				subset.index_start = offset;
				offset += subset.index_count;
				//This will be used as counter in the following produres. reset to zero
				subset.index_count = 0;
			}
		}

		const FbxVector4* array_of_control_points = fbx_mesh->GetControlPoints();
		indices.resize(number_of_polygons * 3);//UNIT18
		for (int index_of_polygon = 0; index_of_polygon < number_of_polygons; index_of_polygon++)
		{
			//UNIT18
			//The material for current face
			int index_of_material = 0;
			if (number_of_materials > 0)
			{
				index_of_material = fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(index_of_polygon);
			}

			//Where should I save the vertex attribute index. according to the material
			subset& subset = mesh.subsets.at(index_of_material);
			const int indext_offset = subset.index_start + subset.index_count;

			for (int index_of_vertex = 0; index_of_vertex < 3; index_of_vertex++) {
				Vertex vertex;
				const int index_of_control_point = fbx_mesh->GetPolygonVertex(index_of_polygon, index_of_vertex);
				vertex.position.x = static_cast<float>(array_of_control_points[index_of_control_point][0]);
				vertex.position.y = static_cast<float>(array_of_control_points[index_of_control_point][1]);
				vertex.position.z = static_cast<float>(array_of_control_points[index_of_control_point][2]);
				bone_influences_per_control_point point;
				point = bone_influences.at(index_of_control_point);
				int i = 0;
				for (auto& p : point)
				{
					//assert(i >= 4);
					vertex.bone_indices[i] = p.index;
					vertex.bone_weights[i] = p.weight;
					i++;
					if (i >= MAX_BONE_INFLUENCES)
					{
						break;
					}
				}

				fbxsdk::FbxStringList uv_names;
				fbx_mesh->GetUVSetNames(uv_names);
				FbxVector2 uv;
				bool unmapped_uv;
				if (fbx_mesh->GetElementUVCount() > 0)
				{
					fbx_mesh->GetPolygonVertexUV(index_of_polygon, index_of_vertex, uv_names[0], uv, unmapped_uv);
					vertex.texcoord.x = static_cast<float>(uv[0]);
					vertex.texcoord.y = 1.0f - static_cast<float>(uv[1]);
				}
				if (fbx_mesh->GetElementNormalCount() > 0)
				{
					FbxVector4 normal;
					fbx_mesh->GetPolygonVertexNormal(index_of_polygon, index_of_vertex, normal);
					vertex.normal.x = static_cast<float>(normal[0]);
					vertex.normal.y = static_cast<float>(normal[1]);
					vertex.normal.z = static_cast<float>(normal[2]);
				}

				vertices.push_back(vertex);
				//indices.push_back(vertex_count);
				indices.at(indext_offset + index_of_vertex) = static_cast<u_int>(vertex_count);
				vertex_count += 1;
			}
			subset.index_count += 3;
		}

		indexsize = indices.size();


		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.ByteWidth = vertices.size() * sizeof(Skinned_mesh::Vertex);
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;// D3D11_USAGE_DYNAMIC;D3D11_USAGE_IMMUTABLE
		buffer_desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subresource = {};
		subresource.pSysMem = vertices.data();
		subresource.SysMemPitch = 0;
		subresource.SysMemSlicePitch = 0;

		hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, &mesh.vertex_buffer);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		//インデックス情報セット
		ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&subresource, sizeof(D3D11_SUBRESOURCE_DATA));
		buffer_desc.ByteWidth = indices.size() * sizeof(u_int);
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;// D3D11_USAGE_DYNAMIC;
		buffer_desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		subresource.pSysMem = indices.data();
		subresource.SysMemPitch = 0;
		subresource.SysMemSlicePitch = 0;

		hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, &mesh.index_buffer);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	manager->Destroy();

	D3D11_RASTERIZER_DESC rasterizer_desc;

	rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	rasterizer_desc.FrontCounterClockwise = FALSE;
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0;
	rasterizer_desc.SlopeScaledDepthBias = 0;
	rasterizer_desc.DepthClipEnable = TRUE;
	rasterizer_desc.ScissorEnable = FALSE;
	rasterizer_desc.MultisampleEnable = FALSE;
	rasterizer_desc.AntialiasedLineEnable = FALSE;

	hr = FRAMEWORK.device->CreateRasterizerState(&rasterizer_desc, line_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_FRONT;
	rasterizer_desc.FrontCounterClockwise = FALSE;
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0;
	rasterizer_desc.SlopeScaledDepthBias = 0;
	rasterizer_desc.DepthClipEnable = TRUE;
	rasterizer_desc.ScissorEnable = FALSE;
	rasterizer_desc.MultisampleEnable = FALSE;
	rasterizer_desc.AntialiasedLineEnable = FALSE;
	hr = FRAMEWORK.device->CreateRasterizerState(&rasterizer_desc, filling_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_FRONT;
	rasterizer_desc.FrontCounterClockwise = TRUE;
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0;
	rasterizer_desc.SlopeScaledDepthBias = 0;
	rasterizer_desc.DepthClipEnable = TRUE;
	rasterizer_desc.ScissorEnable = FALSE;
	rasterizer_desc.MultisampleEnable = FALSE;
	rasterizer_desc.AntialiasedLineEnable = FALSE;
	hr = FRAMEWORK.device->CreateRasterizerState(&rasterizer_desc, filling_state_inverse.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//深度ステンシルステートオブジェクト生成
	D3D11_DEPTH_STENCIL_DESC depth_desc;

	depth_desc.DepthEnable = TRUE;
	depth_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_desc.DepthFunc = D3D11_COMPARISON_LESS;
	depth_desc.StencilEnable = FALSE;
	depth_desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depth_desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	depth_desc.FrontFace.StencilFunc = depth_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth_desc.FrontFace.StencilDepthFailOp = depth_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_desc.FrontFace.StencilPassOp = depth_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_desc.FrontFace.StencilFailOp = depth_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;


	hr = FRAMEWORK.device->CreateDepthStencilState(&depth_desc, depth_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_BUFFER_DESC buffer_desc{};
	//D3D11_SUBRESOURCE_DATA subresource = {};
	ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
	//ZeroMemory(&subresource, sizeof(D3D11_SUBRESOURCE_DATA));
	buffer_desc.ByteWidth = sizeof(cbuffer);
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, nullptr/*&subresource*/, constant_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_SAMPLER_DESC sampler_desc;
	//ID3D11SamplerState *samplestate;

	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MipLODBias = 0.0f;
	sampler_desc.MaxAnisotropy = 16;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	DirectX::XMFLOAT4 bColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	memcpy(sampler_desc.BorderColor, &bColor, sizeof(DirectX::XMFLOAT4));
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = FRAMEWORK.device->CreateSamplerState(&sampler_desc, sampler_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//テクスチャセット
	texture = tex;

	return true;
}



//-----------------------------------------------
//		説明：座標、大きさ、回転を直接入力して描画する。
//			 ビュー行列とプロジェクション行列は今までと同じように。
//			 マテリアルのカラーとviewflagは書かなくてもいい
void Skinned_mesh::Render(
	YRShader					*shader,
	const DirectX::XMFLOAT3			&pos,				//モデルの位置
	const DirectX::XMFLOAT3			&scale,				//モデルの大きさ
	const DirectX::XMFLOAT3			&angle,				//モデルの回転
	const DirectX::XMMATRIX		&view,				//ビュー行列
	const DirectX::XMMATRIX		&projection,		//プロジェクション行列
	const DirectX::XMFLOAT4		&light_direction,	//ライト進行方向
	const DirectX::XMFLOAT4		&light_color,		//ライトのカラー
	const DirectX::XMFLOAT4		&ambient_color,		//環境光
	float						elapsed_time,
	float						anime_count,
	bool						inversion,
	const DirectX::XMFLOAT4		material_color,	//材質色
	const bool					viewflag		//線・塗りつぶし描画フラグ
)
{
	//ワールド変換行列の初期化
	DirectX::XMMATRIX s, r, t;
	//拡大行列作成
	s = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//回転行列作成
	r = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	//移動行列作成
	t = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	DirectX::XMFLOAT4X4 mirror =
	{
		-1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	DirectX::XMMATRIX	m = DirectX::XMLoadFloat4x4(&mirror);

	//行列合成と変換
	DirectX::XMMATRIX world_matrix = s*r*t;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, world_matrix);


	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	//ワールド・ビュー・プロジェクション行列作成
	DirectX::XMFLOAT4X4 world_view_projection;
	DirectX::XMStoreFloat4x4(&world_view_projection, world_matrix* view*projection);
	//context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//ステート・シェーダー設定
	if (viewflag)
	{
		FRAMEWORK.context->RSSetState(filling_state.Get());
	}
	else
	{
		FRAMEWORK.context->RSSetState(line_state.Get());
	}

	FRAMEWORK.context->OMSetDepthStencilState(depth_state.Get(), 1);

	FRAMEWORK.context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	shader->Acivate();

	//定数バッファのバインド
	for (auto& mesh : meshes)
	{
		//頂点バッファのバインド
		FRAMEWORK.context->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);

		//インデックスバッファのバインド
		FRAMEWORK.context->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, offset);


		for (auto& subset : mesh.subsets)
		{
			cbuffer cb = {};


			for (int i = 0; i < BONE_MAX; i++)
			{
				cb.bone_transforms[i] = {
					1,0,0,0,
					0,1,0,0,
					0,0,1,0,
					0,0,0,1
				};
			}

			//UNIT.23
			if (mesh.skeletal_animation.size() > 0)
			{

				mesh.skeletal_animation.animation_tick = anime_count;
				int index = static_cast<int>(mesh.skeletal_animation.animation_tick / mesh.skeletal_animation.sampling_time);
				int indexR = index + 1;
				/*if (indexR > static_cast<int>(mesh.skeletal_animation.size()) - 1)
				{
					indexR = 0;
				}*/
				if (index >= static_cast<int>(mesh.skeletal_animation.size()) - 1)
				{
					index = static_cast<int>(mesh.skeletal_animation.size()) - 1;
					indexR = 0;
					//mesh.skeletal_animation.animation_tick = 0;
					//mesh.skeletal_animation.anim_fin = true;
				}

				std::vector<bone> &skeletal = mesh.skeletal_animation.at(static_cast<int>(index));
				std::vector<bone>& skeletal2 = mesh.skeletal_animation.at(static_cast<int>(indexR));
				size_t number_of_bones = skeletal.size();
				_ASSERT_EXPR(number_of_bones < BONE_MAX, L"'the number_of_bones' exceeds MAX_BONES.");
				for (size_t i = 0; i < number_of_bones; i++)
				{
					//XMStoreFloat4x4(&cb.bone_transforms[i], XMLoadFloat4x4(&skeletal.at(i).transform));
					float left = static_cast<float>(index)* mesh.skeletal_animation.sampling_time;
					float right = static_cast<float>(indexR)* mesh.skeletal_animation.sampling_time;
					float t = (mesh.skeletal_animation.animation_tick - left) / (right - left);

					XMVECTOR	S, R, T;
					XMVECTOR	S2, R2, T2;
					XMMatrixDecompose(&S, &R, &T, XMLoadFloat4x4(&skeletal.at(i).transform));
					XMMatrixDecompose(&S2, &R2, &T2, XMLoadFloat4x4(&skeletal2.at(i).transform));

					XMVECTOR	SA, RA, TA;
					SA = XMVectorLerp(S, S2,t);
					RA = XMQuaternionSlerp(R, R2, t);
					TA = XMVectorLerp(T, T2, t);

					XMFLOAT4 up = { 0,0,1,0 };
					XMVECTOR up_v = XMLoadFloat4(&up);

					XMVECTOR qaxis = XMQuaternionRotationAxis(up_v, XMConvertToRadians(180.0f));

					XMMATRIX W;

					W =
						XMMatrixScalingFromVector(SA) *
						XMMatrixRotationQuaternion(RA) *
						XMMatrixTranslationFromVector(TA);

					XMStoreFloat4x4(&cb.bone_transforms[i], W);
					/*XMStoreFloat4x4(&cb.bone_transforms[i],
						XMLoadFloat4x4(
							&Interpolation(
								i,
								mesh.skeletal_animation.animation_tick,
								mesh.skeletal_animation.sampling_time,
								skeletal,
								skeletal2)
						)
					);*/
				}
				mesh.skeletal_animation.animation_tick = anime_count;
				//mesh.skeletal_animation.animation_tick += elapsed_time;
			}
			//補間処理は行っていない

			/*static float angle = 0;
			DirectX::XMStoreFloat4x4(&cb.bone_transforms[0], DirectX::XMMatrixRotationRollPitchYaw(0, 0, sinf(angle * 0.01745f)));
			DirectX::XMStoreFloat4x4(&cb.bone_transforms[1], DirectX::XMMatrixRotationRollPitchYaw(0, 0, sinf(angle * 0.01745f)));
			DirectX::XMStoreFloat4x4(&cb.bone_transforms[2], DirectX::XMMatrixRotationRollPitchYaw(0, 0, sinf(angle * 0.01745f)));
			angle += 0.1f;*/
			DirectX::XMStoreFloat4x4(&cb.world_view_projection,
				(DirectX::XMLoadFloat4x4(&mesh.global_transform)*
					DirectX::XMLoadFloat4x4(&coodinate_conversion)*
					DirectX::XMLoadFloat4x4(&world_view_projection)));
			DirectX::XMStoreFloat4x4(&cb.world,
				(DirectX::XMLoadFloat4x4(&mesh.global_transform)*
					DirectX::XMLoadFloat4x4(&coodinate_conversion)*
					DirectX::XMLoadFloat4x4(&world)));
			cb.light_direction = light_direction;
			cb.light_color = light_color;
			cb.ambient_color = ambient_color;
			cb.material_color.x = material_color.x*subset.diffuse.color.x;
			cb.material_color.y = material_color.y*subset.diffuse.color.y;
			cb.material_color.z = material_color.z*subset.diffuse.color.z;
			cb.material_color.w = material_color.w*subset.diffuse.color.w;
			//☆
			cb.eyePos.x = YRCamera.GetEye().x;
			cb.eyePos.y = YRCamera.GetEye().y;
			cb.eyePos.z = YRCamera.GetEye().z;
			cb.eyePos.w = 1.0f;
			DirectX::XMFLOAT4X4 v;
			DirectX::XMStoreFloat4x4(&v, view);
			cb.view = v;
			DirectX::XMFLOAT4X4 p;
			DirectX::XMStoreFloat4x4(&p, projection);
			cb.projection = p;
			cb.at = YRCamera.GetAt();
			cb.dummy = 0.0f;

			/*if (inversion)
			{
				cb.inverse = 1;
			}
			else
			{
				cb.inverse = 0;
			}*/
			FRAMEWORK.context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cb, 0, 0);
			FRAMEWORK.context->VSSetConstantBuffers(NULL, 1, constant_buffer.GetAddressOf());
			FRAMEWORK.context->PSSetConstantBuffers(NULL, 1, constant_buffer.GetAddressOf());
			FRAMEWORK.context->GSSetConstantBuffers(NULL, 1, constant_buffer.GetAddressOf());
			if (texture)
			{
				texture->Set(0);
			}
			else
			{
				FRAMEWORK.context->PSSetShaderResources(0, 1, &subset.diffuse.shader_resource_view);
			}
			FRAMEWORK.context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());
			//プリミティブの描画
			FRAMEWORK.context->DrawIndexed(subset.index_count, subset.index_start, NULL);
		}
	}
	shader->Inactivate();
}

//-----------------------------------------------
//		説明：座標、大きさ、クォータニオン行列を直接入力して描画する。
//			 ビュー行列とプロジェクション行列は今までと同じように。
//			 マテリアルのカラーとviewflagは書かなくてもいい
//			こちらはまだ未実装
void Skinned_mesh::Render(
	YRShader					*shader,
	DirectX::XMFLOAT3			&pos,				//モデルの位置
	DirectX::XMFLOAT3			&scale,				//モデルの大きさ
	DirectX::XMVECTOR			&orientation,		//モデルの回転
	const DirectX::XMMATRIX		&view,				//ビュー行列
	const DirectX::XMMATRIX		&projection,		//プロジェクション行列
	const DirectX::XMFLOAT4		&light_direction,	//ライト進行方向
	const DirectX::XMFLOAT4		&light_color,		//ライトのカラー
	const DirectX::XMFLOAT4		&ambient_color,		//環境光
	float						elapsed_time,
	float						anime_count,
	const DirectX::XMFLOAT4		material_color,	//材質色
	const bool					viewflag		//線・塗りつぶし描画フラグ
)
{
	//ワールド変換行列の初期化
	DirectX::XMMATRIX s, r, t;
	//拡大行列作成
	s = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//回転行列作成
	r = DirectX::XMMatrixRotationQuaternion(orientation);
	//移動行列作成
	t = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	//行列合成と変換
	DirectX::XMMATRIX world_matrix = s*r*t;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, world_matrix);


	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	//ワールド・ビュー・プロジェクション行列作成
	DirectX::XMFLOAT4X4 world_view_projection;
	DirectX::XMStoreFloat4x4(&world_view_projection, world_matrix*view*projection);
	//context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//ステート・シェーダー設定
	if (viewflag)
	{
		FRAMEWORK.context->RSSetState(filling_state.Get());
	}
	else
	{
		FRAMEWORK.context->RSSetState(line_state.Get());
	}

	FRAMEWORK.context->OMSetDepthStencilState(depth_state.Get(), 1);

	FRAMEWORK.context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	/*FRAMEWORK.context->IASetInputLayout(input_layout.Get());
	FRAMEWORK.context->VSSetShader(vertex_shader.Get(), NULL, 0);
	FRAMEWORK.context->PSSetShader(pixel_shader.Get(), NULL, 0);*/
	shader->Acivate();

	//定数バッファのバインド
	for (auto& mesh : meshes)
	{
		//頂点バッファのバインド
		FRAMEWORK.context->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);

		//インデックスバッファのバインド
		FRAMEWORK.context->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, offset);


		for (auto& subset : mesh.subsets)
		{
			cbuffer cb = {};


			for (int i = 0; i < BONE_MAX; i++)
			{
				cb.bone_transforms[i] = {
					1,0,0,0,
					0,1,0,0,
					0,0,1,0,
					0,0,0,1
				};
			}

			//UNIT.23
			if (mesh.skeletal_animation.size() > 0)
			{

				mesh.skeletal_animation.animation_tick = anime_count;
				int frame = static_cast<int>(mesh.skeletal_animation.animation_tick / mesh.skeletal_animation.sampling_time);
				if (frame > static_cast<int>(mesh.skeletal_animation.size()) - 1)
				{
					frame = static_cast<int>(mesh.skeletal_animation.size()) - 1;
					//mesh.skeletal_animation.animation_tick = 0;
					//mesh.skeletal_animation.anim_fin = true;
				}
				/*std::vector<bone> &skeletal2 = mesh.skeletal_animation.at(frame);
				if (frame+1 < mesh.skeletal_animation.size()-1)
				{
				skeletal2 = mesh.skeletal_animation.at(frame+1);
				}*/

				std::vector<bone> &skeletal = mesh.skeletal_animation.at(frame);
				size_t number_of_bones = skeletal.size();
				_ASSERT_EXPR(number_of_bones < BONE_MAX, L"'the number_of_bones' exceeds MAX_BONES.");
				for (size_t i = 0; i < number_of_bones; i++)
				{
					XMStoreFloat4x4(&cb.bone_transforms[i], XMLoadFloat4x4(&skeletal.at(i).transform));
					/*XMStoreFloat4x4(&cb.bone_transforms[i],
					XMLoadFloat4x4(
					&Interpolation(
					i,
					mesh.skeletal_animation.animation_tick,
					mesh.skeletal_animation.sampling_time,
					skeletal,
					skeletal2)
					)
					);*/
				}
				mesh.skeletal_animation.animation_tick = anime_count;
				//mesh.skeletal_animation.animation_tick += elapsed_time;
			}
			//補間処理は行っていない

			/*static float angle = 0;

			DirectX::XMStoreFloat4x4(&cb.bone_transforms[0], DirectX::XMMatrixRotationRollPitchYaw(0, 0, sinf(angle * 0.01745f)));
			DirectX::XMStoreFloat4x4(&cb.bone_transforms[1], DirectX::XMMatrixRotationRollPitchYaw(0, 0, sinf(angle * 0.01745f)));
			DirectX::XMStoreFloat4x4(&cb.bone_transforms[2], DirectX::XMMatrixRotationRollPitchYaw(0, 0, sinf(angle * 0.01745f)));
			angle += 0.1f;*/
			DirectX::XMStoreFloat4x4(&cb.world_view_projection,
				(DirectX::XMLoadFloat4x4(&mesh.global_transform)*
					DirectX::XMLoadFloat4x4(&coodinate_conversion)*
					DirectX::XMLoadFloat4x4(&world_view_projection)));
			DirectX::XMStoreFloat4x4(&cb.world,
				(DirectX::XMLoadFloat4x4(&mesh.global_transform)*
					DirectX::XMLoadFloat4x4(&coodinate_conversion)*
					DirectX::XMLoadFloat4x4(&world)));
			cb.light_direction = light_direction;
			cb.light_color = light_color;
			cb.ambient_color = ambient_color;
			cb.material_color.x = material_color.x*subset.diffuse.color.x;
			cb.material_color.y = material_color.y*subset.diffuse.color.y;
			cb.material_color.z = material_color.z*subset.diffuse.color.z;
			cb.material_color.w = material_color.w*subset.diffuse.color.w;
			cb.eyePos.x = YRCamera.GetEye().x;
			cb.eyePos.y = YRCamera.GetEye().y;
			cb.eyePos.z = YRCamera.GetEye().z;
			cb.eyePos.w = 1.0f;
			FRAMEWORK.context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cb, 0, 0);
			FRAMEWORK.context->VSSetConstantBuffers(NULL, 1, constant_buffer.GetAddressOf());
			FRAMEWORK.context->PSSetConstantBuffers(NULL, 1, constant_buffer.GetAddressOf());
			if (texture)
			{
				texture->Set(0);
			}
			else
			{
				FRAMEWORK.context->PSSetShaderResources(0, 1, &subset.diffuse.shader_resource_view);
			}
			FRAMEWORK.context->PSSetSamplers(0, 1, sampler_state.GetAddressOf());
			//プリミティブの描画
			FRAMEWORK.context->DrawIndexed(subset.index_count, subset.index_start, NULL);
		}
	}

	shader->Inactivate();
}

