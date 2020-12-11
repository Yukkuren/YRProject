#include "YRModelAnim.h"
#include "misc.h"
#include "framework.h"

ModelAnim::ModelAnim(const std::shared_ptr<Model>& resource)
{
	m_model_resource = resource;
	model_resource_anim = nullptr;

	// ノード
	const std::vector<ModelData::Node>& res_nodes = resource->GetNodes();

	m_nodes.resize(res_nodes.size());
	for (size_t node_index = 0; node_index < m_nodes.size(); ++node_index)
	{
		auto&& src = res_nodes.at(node_index);
		auto&& dst = m_nodes.at(node_index);

		dst.name = src.name;
		dst.parent = src.parent_index >= 0 ? &m_nodes.at(src.parent_index) : nullptr;
		dst.scale = src.scale.GetDXFLOAT3();
		dst.rotate = src.rotate;
		dst.translate = src.translate.GetDXFLOAT3();
	}

	D3D11_BUFFER_DESC buffer_desc{};
	D3D11_SUBRESOURCE_DATA subresource = {};
	ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&subresource, sizeof(D3D11_SUBRESOURCE_DATA));
	buffer_desc.ByteWidth = sizeof(cbuffer);
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	HRESULT hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	// 深度ステンシルステート
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = TRUE;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		HRESULT hr = FRAMEWORK.device.Get()->CreateDepthStencilState(&desc, m_depth_stencil_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	}

	// ラスタライザーステート
	{
		D3D11_RASTERIZER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.FrontCounterClockwise = true;
		desc.DepthBias = 0;
		desc.DepthBiasClamp = 0;
		desc.SlopeScaledDepthBias = 0;
		desc.DepthClipEnable = true;
		desc.ScissorEnable = false;
		desc.MultisampleEnable = true;
		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = FRAMEWORK.device.Get()->CreateRasterizerState(&desc, m_rasterizer_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// サンプラステート
	{
		D3D11_SAMPLER_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD = -FLT_MAX;
		desc.MaxLOD = FLT_MAX;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		HRESULT hr = FRAMEWORK.device.Get()->CreateSamplerState(&desc, m_sampler_state.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// ダミーテクスチャ
	{
		const int width = 8;
		const int height = 8;
		UINT pixels[width * height];
		::memset(pixels, 0xFF, sizeof(pixels));

		D3D11_TEXTURE2D_DESC desc = { 0 };
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA data;
		::memset(&data, 0, sizeof(data));
		data.pSysMem = pixels;
		data.SysMemPitch = width;

		Microsoft::WRL::ComPtr<ID3D11Texture2D>	texture;
		HRESULT hr = FRAMEWORK.device.Get()->CreateTexture2D(&desc, &data, texture.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		hr = FRAMEWORK.device.Get()->CreateShaderResourceView(texture.Get(), nullptr, m_dummy_srv.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}
}

// アニメーション再生
void ModelAnim::PlayAnimation(int animation_index, bool loop)
{
	m_current_animation = animation_index;
	m_loop_animation = loop;
	m_end_animation = false;
	m_current_seconds = 0.0f;
}

// アニメーション計算
void ModelAnim::UpdateAnimation(float elapsed_time)
{
	if (m_current_animation < 0)
	{
		return;
	}

	if (m_model_resource->GetAnimations().empty())
	{
		if (model_resource_anim == nullptr)
		{
			return;
		}
	}

	if (model_resource_anim->GetAnimations().empty())
	{

		const ModelData::Animation& animation = m_model_resource->GetAnimations().at(m_current_animation);

		const std::vector<ModelData::Keyframe>& keyframes = animation.keyframes;
		int key_count = static_cast<int>(keyframes.size());
		for (int key_index = 0; key_index < key_count - 1; ++key_index)
		{
			// 現在の時間がどのキーフレームの間にいるか判定する
			const ModelData::Keyframe& keyframe0 = keyframes.at(key_index);
			const ModelData::Keyframe& keyframe1 = keyframes.at(key_index + 1);
			if (m_current_seconds >= keyframe0.seconds && m_current_seconds < keyframe1.seconds)
			{
				float rate = (m_current_seconds - keyframe0.seconds / keyframe1.seconds - keyframe0.seconds);

				assert(m_nodes.size() == keyframe0.node_keys.size());
				assert(m_nodes.size() == keyframe1.node_keys.size());
				int node_count = static_cast<int>(m_nodes.size());
				for (int node_index = 0; node_index < node_count; ++node_index)
				{
					// ２つのキーフレーム間の補完計算
					const ModelData::NodeKeyData& key0 = keyframe0.node_keys.at(node_index);
					const ModelData::NodeKeyData& key1 = keyframe1.node_keys.at(node_index);

					Node& node = m_nodes[node_index];

					XMFLOAT3 scale = key0.scale.GetDXFLOAT3();


					DirectX::XMVECTOR s0 = DirectX::XMLoadFloat3(&key0.scale.GetDXFLOAT3());
					DirectX::XMVECTOR s1 = DirectX::XMLoadFloat3(&key1.scale.GetDXFLOAT3());
					DirectX::XMVECTOR r0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR r1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR t0 = DirectX::XMLoadFloat3(&key0.translate.GetDXFLOAT3());
					DirectX::XMVECTOR t1 = DirectX::XMLoadFloat3(&key1.translate.GetDXFLOAT3());

					DirectX::XMVECTOR s = DirectX::XMVectorLerp(s0, s1, rate);
					DirectX::XMVECTOR r = DirectX::XMQuaternionSlerp(r0, r1, rate);
					DirectX::XMVECTOR t = DirectX::XMVectorLerp(t0, t1, rate);

					DirectX::XMStoreFloat3(&node.scale, s);
					DirectX::XMStoreFloat4(&node.rotate, r);
					DirectX::XMStoreFloat3(&node.translate, t);
				}
				break;
			}
		}

		// 最終フレーム処理
		if (m_end_animation)
		{
			m_end_animation = false;
			m_current_animation = -1;
			return;
		}

		// 時間経過
		m_current_seconds += elapsed_time;
		if (m_current_seconds >= animation.seconds_length)
		{
			if (m_loop_animation)
			{
				m_current_seconds -= animation.seconds_length;
			}
			else
			{
				m_current_seconds = animation.seconds_length;
				m_end_animation = true;
			}
		}
	}
	else
	{
		const ModelData::Animation& animation = model_resource_anim->GetAnimations().at(m_current_animation);

		const std::vector<ModelData::Keyframe>& keyframes = animation.keyframes;
		int key_count = static_cast<int>(keyframes.size());
		for (int key_index = 0; key_index < key_count - 1; ++key_index)
		{
			// 現在の時間がどのキーフレームの間にいるか判定する
			const ModelData::Keyframe& keyframe0 = keyframes.at(key_index);
			const ModelData::Keyframe& keyframe1 = keyframes.at(key_index + 1);
			if (m_current_seconds >= keyframe0.seconds && m_current_seconds < keyframe1.seconds)
			{
				float rate = (m_current_seconds - keyframe0.seconds / keyframe1.seconds - keyframe0.seconds);

				assert(m_nodes.size() == keyframe0.node_keys.size());
				assert(m_nodes.size() == keyframe1.node_keys.size());
				int node_count = static_cast<int>(m_nodes.size());
				for (int node_index = 0; node_index < node_count; ++node_index)
				{
					// ２つのキーフレーム間の補完計算
					const ModelData::NodeKeyData& key0 = keyframe0.node_keys.at(node_index);
					const ModelData::NodeKeyData& key1 = keyframe1.node_keys.at(node_index);

					Node& node = m_nodes[node_index];

					DirectX::XMVECTOR s0 = DirectX::XMLoadFloat3(&key0.scale.GetDXFLOAT3());
					DirectX::XMVECTOR s1 = DirectX::XMLoadFloat3(&key1.scale.GetDXFLOAT3());
					DirectX::XMVECTOR r0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR r1 = DirectX::XMLoadFloat4(&key1.rotate);
					DirectX::XMVECTOR t0 = DirectX::XMLoadFloat3(&key0.translate.GetDXFLOAT3());
					DirectX::XMVECTOR t1 = DirectX::XMLoadFloat3(&key1.translate.GetDXFLOAT3());

					DirectX::XMVECTOR s = DirectX::XMVectorLerp(s0, s1, rate);
					DirectX::XMVECTOR r = DirectX::XMQuaternionSlerp(r0, r1, rate);
					DirectX::XMVECTOR t = DirectX::XMVectorLerp(t0, t1, rate);

					DirectX::XMStoreFloat3(&node.scale, s);
					DirectX::XMStoreFloat4(&node.rotate, r);
					DirectX::XMStoreFloat3(&node.translate, t);
				}
				break;
			}
		}

		// 最終フレーム処理
		if (m_end_animation)
		{
			m_end_animation = false;
			m_current_animation = -1;
			return;
		}

		// 時間経過
		m_current_seconds += elapsed_time;
		if (m_current_seconds >= animation.seconds_length)
		{
			if (m_loop_animation)
			{
				m_current_seconds -= animation.seconds_length;
			}
			else
			{
				m_current_seconds = animation.seconds_length;
				m_end_animation = true;
			}
		}
	}
}

// ローカル変換行列計算
void ModelAnim::CalculateLocalTransform()
{
	for (Node& node : m_nodes)
	{
		DirectX::XMMATRIX scale, rotate, translate, local;
		scale = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		rotate = DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(node.rotate.x, node.rotate.y, node.rotate.z, node.rotate.w));
		translate = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);

		local = scale * rotate * translate;
		DirectX::XMStoreFloat4x4(&node.local_transform, local);
	}
}

// ワールド変換行列計算
void ModelAnim::CalculateWorldTransform(
	const DirectX::XMFLOAT3& pos,
	const DirectX::XMFLOAT3& scale,
	const DirectX::XMFLOAT3& angle)
{

	//ワールド変換行列の初期化
	DirectX::XMMATRIX s, r, t;
	//拡大行列作成
	s = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//回転行列作成
	r = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
	//移動行列作成
	t = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	//行列合成と変換
	DirectX::XMMATRIX world_transform = s * r * t;
	world_matrix = world_transform;
	/*DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, world_transform);*/


	for (Node& node : m_nodes)
	{
		if (node.parent != nullptr)
		{
			DirectX::XMMATRIX local_transform = DirectX::XMLoadFloat4x4(&node.local_transform);
			DirectX::XMMATRIX parent_world_transform = DirectX::XMLoadFloat4x4(&node.parent->world_transform);
			DirectX::XMStoreFloat4x4(&node.world_transform, local_transform * parent_world_transform);
		}
		else
		{
			DirectX::XMMATRIX local_transform = DirectX::XMLoadFloat4x4(&node.local_transform);
			DirectX::XMStoreFloat4x4(&node.world_transform, local_transform);
		}
	}
}


void ModelAnim::Draw(
	YRShader* shader,
	const DirectX::XMMATRIX&	view,
	const DirectX::XMMATRIX&	projection,
	const DirectX::XMFLOAT4&	light_direction,
	const DirectX::XMFLOAT4&	light_color,
	const DirectX::XMFLOAT4&	ambient_color,
	const DirectX::XMFLOAT2&	off_set_eye,
	const DirectX::XMFLOAT2&	off_set_mouse,
	const Model::Material_Attribute&	blur_material,
	const DirectX::XMFLOAT4		material_color
)
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, world_matrix);

	DirectX::XMFLOAT4X4 world_view_projection;
	DirectX::XMStoreFloat4x4(&world_view_projection, world_matrix * view * projection);

	const std::vector<Node>& nodes = GetNodes();
	//std::vector<Model::Mesh> test = m_model_resource->GetMeshes();

	const Model* model_res = m_model_resource.get();

	/*for (auto& material : model_res->m_data->materials)
	{
		std::string file = material.texture_filename;
		const char* name = file.c_str();
		int hoge = 0;
	}

	for (auto& data : model_res->m_data->meshes)
	{
		for (auto& v : data.subsets)
		{
			int i = v.material_index;
			int hoge = 0;
		}
	}*/

	for (const Model::Mesh& mesh : model_res->GetMeshes())
	{
		// メッシュ用定数バッファ更新
		cbuffer cb;
		::memset(&cb, 0, sizeof(cb));
		if (mesh.node_indices.size() > 0)
		{
			size_t max = mesh.node_indices.size();
			for (size_t i = 0; i < max; i++)
			{
				DirectX::XMMATRIX inverse_transform = DirectX::XMLoadFloat4x4(mesh.inverse_transforms.at(i));
				DirectX::XMMATRIX world_transform = DirectX::XMLoadFloat4x4(&nodes.at(mesh.node_indices.at(i)).world_transform);
				DirectX::XMMATRIX bone_transform = inverse_transform * world_transform;
				DirectX::XMStoreFloat4x4(&cb.bone_transforms[i], bone_transform);

			}
		}
		else
		{
			cb.bone_transforms[0] = nodes.at(mesh.node_index).world_transform;
		}

		UINT stride = sizeof(ModelData::Vertex);
		UINT offset = 0;
		FRAMEWORK.context->RSSetState(m_rasterizer_state.Get());
		FRAMEWORK.context->OMSetDepthStencilState(m_depth_stencil_state.Get(), 1);
		shader->Acivate();
		FRAMEWORK.context.Get()->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);
		FRAMEWORK.context.Get()->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		FRAMEWORK.context.Get()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		DirectX::XMFLOAT4X4 global_transform =
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};

		for (const Model::Subset& subset : mesh.subsets)
		{
			DirectX::XMStoreFloat4x4(&cb.world_view_projection,
				/*DirectX::XMLoadFloat4x4(&global_transform) **/
				DirectX::XMLoadFloat4x4(&coodinate_conversion) *
				DirectX::XMLoadFloat4x4(&world_view_projection));
			DirectX::XMStoreFloat4x4(&cb.world,
				/*DirectX::XMLoadFloat4x4(&global_transform) **/
				DirectX::XMLoadFloat4x4(&coodinate_conversion) *
				DirectX::XMLoadFloat4x4(&world));
			//cb.material_color = subset.material->color;
			cb.light_direction = light_direction;
			cb.light_color = light_color;
			cb.ambient_color = ambient_color;
			cb.material_color.x = material_color.x * subset.material->color.x;
			cb.material_color.y = material_color.y * subset.material->color.y;
			cb.material_color.z = material_color.z * subset.material->color.z;
			cb.material_color.w = material_color.w * subset.material->color.w;
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
			cb.dummy00 = 0.0f;
			cb.dummy01 = 0.0f;

			//ブルームで光らせる場所を指定する
			switch (blur_material)
			{
			case Model::Material_Attribute::NONE:
				cb.lumi_factor = 0.0f;
				break;
			case Model::Material_Attribute::ALL:
				cb.lumi_factor = 1.0f;
				break;
			default:
				if (subset.material_index == blur_material)
				{
					cb.lumi_factor = 1.0f;
				}
				break;
			}

			switch (subset.material_index)
			{
			case Model::Material_Attribute::EYE:
				cb.Offset_X = off_set_eye.x;
				cb.Offset_Y = off_set_eye.y;
				if (m_model_resource->color_texture_face)
				{
					m_model_resource->color_texture_face->Set(0);
				}
				break;
			case Model::Material_Attribute::MOUSE:
				cb.Offset_X = off_set_mouse.x;
				cb.Offset_Y = off_set_mouse.y;
				if (m_model_resource->color_texture_face)
				{
					m_model_resource->color_texture_face->Set(0);
				}
				break;
			case Model::Material_Attribute::SWORD:
				cb.Offset_X = 0.0f;
				cb.Offset_Y = 0.0f;
				if (m_model_resource->color_texture_main)
				{
					m_model_resource->color_texture_main->Set(0);
				}
				break;
			default:
				cb.Offset_X = 0.0f;
				cb.Offset_Y = 0.0f;
				if (m_model_resource->color_texture_main)
				{
					m_model_resource->color_texture_main->Set(0);
				}
				break;
			}

			FRAMEWORK.context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cb, 0, 0);
			FRAMEWORK.context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
			FRAMEWORK.context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
			FRAMEWORK.context->GSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

			if (m_model_resource->color_texture_main == nullptr)
			{
				FRAMEWORK.context->PSSetShaderResources(0, 1, subset.material->shader_resource_view.Get() ? subset.material->shader_resource_view.GetAddressOf() : m_dummy_srv.GetAddressOf());
			}
			FRAMEWORK.context->PSSetSamplers(0, 1, m_sampler_state.GetAddressOf());
			FRAMEWORK.context->DrawIndexed(subset.index_count, subset.start_index, 0);
		}

		shader->Inactivate();
	}
}




void ModelAnim::Draw(
	YRShader* shader,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const D3D_PRIMITIVE_TOPOLOGY& topology,
	const DirectX::XMFLOAT4& ambient_color,
	const DirectX::XMFLOAT2& off_set_eye,
	const DirectX::XMFLOAT2& off_set_mouse,
	const Model::Material_Attribute& blur_material,
	const DirectX::XMFLOAT4		material_color
)
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, world_matrix);

	DirectX::XMFLOAT4X4 world_view_projection;
	DirectX::XMStoreFloat4x4(&world_view_projection, world_matrix * view * projection);

	const std::vector<Node>& nodes = GetNodes();
	//std::vector<Model::Mesh> test = m_model_resource->GetMeshes();

	const Model* model_res = m_model_resource.get();

	/*for (auto& material : model_res->m_data->materials)
	{
		std::string file = material.texture_filename;
		const char* name = file.c_str();
		int hoge = 0;
	}

	for (auto& data : model_res->m_data->meshes)
	{
		for (auto& v : data.subsets)
		{
			int i = v.material_index;
			int hoge = 0;
		}
	}*/

	for (const Model::Mesh& mesh : model_res->GetMeshes())
	{
		// メッシュ用定数バッファ更新
		cbuffer cb;
		::memset(&cb, 0, sizeof(cb));
		if (mesh.node_indices.size() > 0)
		{
			size_t max = mesh.node_indices.size();
			for (size_t i = 0; i < max; i++)
			{
				DirectX::XMMATRIX inverse_transform = DirectX::XMLoadFloat4x4(mesh.inverse_transforms.at(i));
				DirectX::XMMATRIX world_transform = DirectX::XMLoadFloat4x4(&nodes.at(mesh.node_indices.at(i)).world_transform);
				DirectX::XMMATRIX bone_transform = inverse_transform * world_transform;
				DirectX::XMStoreFloat4x4(&cb.bone_transforms[i], bone_transform);

			}
		}
		else
		{
			cb.bone_transforms[0] = nodes.at(mesh.node_index).world_transform;
		}

		UINT stride = sizeof(ModelData::Vertex);
		UINT offset = 0;
		FRAMEWORK.context->RSSetState(m_rasterizer_state.Get());
		FRAMEWORK.context->OMSetDepthStencilState(m_depth_stencil_state.Get(), 1);
		shader->Acivate();
		FRAMEWORK.context.Get()->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);
		FRAMEWORK.context.Get()->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		FRAMEWORK.context.Get()->IASetPrimitiveTopology(topology);

		DirectX::XMFLOAT4X4 global_transform =
		{
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};

		for (const Model::Subset& subset : mesh.subsets)
		{
			DirectX::XMStoreFloat4x4(&cb.world_view_projection,
				/*DirectX::XMLoadFloat4x4(&global_transform) **/
				DirectX::XMLoadFloat4x4(&coodinate_conversion) *
				DirectX::XMLoadFloat4x4(&world_view_projection));
			DirectX::XMStoreFloat4x4(&cb.world,
				/*DirectX::XMLoadFloat4x4(&global_transform) **/
				DirectX::XMLoadFloat4x4(&coodinate_conversion) *
				DirectX::XMLoadFloat4x4(&world));
			//cb.material_color = subset.material->color;
			cb.light_direction = light_direction;
			cb.light_color = light_color;
			cb.ambient_color = ambient_color;
			cb.material_color.x = material_color.x * subset.material->color.x;
			cb.material_color.y = material_color.y * subset.material->color.y;
			cb.material_color.z = material_color.z * subset.material->color.z;
			cb.material_color.w = material_color.w * subset.material->color.w;
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
			cb.dummy00 = 0.0f;
			cb.dummy01 = 0.0f;

			//ブルームで光らせる場所を指定する
			switch (blur_material)
			{
			case Model::Material_Attribute::NONE:
				cb.lumi_factor = 0.0f;
				break;
			case Model::Material_Attribute::ALL:
				cb.lumi_factor = 1.0f;
				break;
			default:
				if (subset.material_index == blur_material)
				{
					cb.lumi_factor = 1.0f;
				}
				break;
			}

			switch (subset.material_index)
			{
			case Model::Material_Attribute::EYE:
				cb.Offset_X = off_set_eye.x;
				cb.Offset_Y = off_set_eye.y;
				if (m_model_resource->color_texture_face)
				{
					m_model_resource->color_texture_face->Set(0);
				}
				break;
			case Model::Material_Attribute::MOUSE:
				cb.Offset_X = off_set_mouse.x;
				cb.Offset_Y = off_set_mouse.y;
				if (m_model_resource->color_texture_face)
				{
					m_model_resource->color_texture_face->Set(0);
				}
				break;
			case Model::Material_Attribute::SWORD:
				cb.Offset_X = 0.0f;
				cb.Offset_Y = 0.0f;
				if (m_model_resource->color_texture_main)
				{
					m_model_resource->color_texture_main->Set(0);
				}
				break;
			default:
				cb.Offset_X = 0.0f;
				cb.Offset_Y = 0.0f;
				if (m_model_resource->color_texture_main)
				{
					m_model_resource->color_texture_main->Set(0);
				}
				break;
			}

			FRAMEWORK.context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cb, 0, 0);
			FRAMEWORK.context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
			FRAMEWORK.context->PSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
			FRAMEWORK.context->GSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

			if (m_model_resource->color_texture_main == nullptr)
			{
				FRAMEWORK.context->PSSetShaderResources(0, 1, subset.material->shader_resource_view.Get() ? subset.material->shader_resource_view.GetAddressOf() : m_dummy_srv.GetAddressOf());
			}
			FRAMEWORK.context->PSSetSamplers(0, 1, m_sampler_state.GetAddressOf());
			FRAMEWORK.context->DrawIndexed(subset.index_count, subset.start_index, 0);
		}

		shader->Inactivate();
	}
}




void ModelAnim::NodeChange(const std::shared_ptr<Model>& resource)
{
	PlayAnimation(0, true);
	// ノード
	const std::vector<ModelData::Node>& res_nodes = resource->GetNodes();

	//m_nodes.resize(res_nodes.size());
	int res = 0;
	model_resource_anim = resource;


	ModelData::NodeKeyData dummy =
	{
		DirectX::XMFLOAT3(0.0f,0.0f,0.0f),
		YR_Vector4(0.0f,0.0f,0.0f,0.0f),
		DirectX::XMFLOAT3(0.0f,0.0f,0.0f)
	};

	for (int i = 0; i < model_resource_anim->m_data->animations[0].keyframes.size(); i++)
	{
		if (model_resource_anim->m_data->animations[0].keyframes[i].node_keys.size() != m_nodes.size())
		{
			model_resource_anim->m_data->animations[0].keyframes[i].node_keys.insert(model_resource_anim->m_data->animations[0].keyframes[i].node_keys.begin() + 1, dummy);
		}
	}

	/*if (res_nodes.size() != m_nodes.size())
	{
		for (int i = 0; i < model_resource_anim->m_data->animations.size(); i++)
		{
			for (int o = 0; o < model_resource_anim->m_data->animations[i].keyframes.size(); o++)
			{
				model_resource_anim->m_data->animations[i].keyframes[o].node_keys.insert(model_resource_anim->m_data->animations[i].keyframes[o].node_keys.begin() + 1, dummy);
			}
		}
	}*/

	res = 0;

	/*for (size_t node_index = 0; node_index < m_nodes.size(); ++node_index)
	{
		if (node_index == 1&&m_nodes.size()!=res_nodes.size())
		{
			continue;
		}
		auto&& src = res_nodes.at(res);
		auto&& dst = m_nodes.at(node_index);

		dst.name = src.name.c_str();
		dst.parent = src.parent_index >= 0 ? &m_nodes.at(src.parent_index) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;
		res++;
	}*/
	//m_current_animation = 0;
	m_current_seconds = 0.0f;
}

void ModelAnim::NodeChange(const std::shared_ptr<Model>& resource, int anim_num)
{
	PlayAnimation(anim_num, true);
	// ノード
	const std::vector<ModelData::Node>& res_nodes = resource->GetNodes();

	//m_nodes.resize(res_nodes.size());
	int res = 0;
	model_resource_anim = resource;


	ModelData::NodeKeyData dummy =
	{
		DirectX::XMFLOAT3(0.0f,0.0f,0.0f),
		YR_Vector4(0.0f,0.0f,0.0f,0.0f),
		DirectX::XMFLOAT3(0.0f,0.0f,0.0f)
	};

	for (int i = 0; i < model_resource_anim->m_data->animations[anim_num].keyframes.size(); i++)
	{
		if (model_resource_anim->m_data->animations[anim_num].keyframes[i].node_keys.size() != m_nodes.size())
		{
			model_resource_anim->m_data->animations[anim_num].keyframes[i].node_keys.insert(model_resource_anim->m_data->animations[anim_num].keyframes[i].node_keys.begin() + 1, dummy);
		}
	}

	/*if (res_nodes.size() != m_nodes.size())
	{
		for (int i = 0; i < model_resource_anim->m_data->animations.size(); i++)
		{
			for (int o = 0; o < model_resource_anim->m_data->animations[i].keyframes.size(); o++)
			{
				model_resource_anim->m_data->animations[i].keyframes[o].node_keys.insert(model_resource_anim->m_data->animations[i].keyframes[o].node_keys.begin() + 1, dummy);
			}
		}
	}*/

	res = 0;

	/*for (size_t node_index = 0; node_index < m_nodes.size(); ++node_index)
	{
		if (node_index == 1&&m_nodes.size()!=res_nodes.size())
		{
			continue;
		}
		auto&& src = res_nodes.at(res);
		auto&& dst = m_nodes.at(node_index);

		dst.name = src.name.c_str();
		dst.parent = src.parent_index >= 0 ? &m_nodes.at(src.parent_index) : nullptr;
		dst.scale = src.scale;
		dst.rotate = src.rotate;
		dst.translate = src.translate;
		res++;
	}*/
	//m_current_animation = 0;
	m_current_seconds = 0.0f;
}