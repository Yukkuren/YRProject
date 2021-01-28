#include "Trajectory.h"
#include "framework.h"

#undef min

void Trajectory::Init(size_t max_count)
{
	HRESULT hr = S_OK;

	if (sampler_wrap == nullptr)
	{
		sampler_wrap = std::make_shared<Sampler>(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	}

	std::vector<PosData> vertex;
	vertex.resize(max_count * 2);

	this->max_count = vertex.size();

	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.ByteWidth = sizeof(vertex);
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subresource = {};
	subresource.pSysMem = vertex.data();
	subresource.SysMemPitch = 0;
	subresource.SysMemSlicePitch = 0;

	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, vertex_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//ラスタライザーステートオブジェクト生成
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
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
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




	//深度ステンシルステートオブジェクト生成
	D3D11_DEPTH_STENCIL_DESC depth_desc;

	depth_desc.DepthEnable = FALSE;
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

	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, nullptr/*&subresource*/, constant_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	Index_size = 0;

	//テクスチャ生成
	if (texture == nullptr)
	{
		texture = std::make_unique<Texture>(L"./Data/Image/Character/Knight/Trajectory.png");
	}

	//シェーダー生成
	if (shader == nullptr)
	{
		shader = std::make_unique<YRShader>(ShaderType::TRAJECTORY);
		shader->Create("./Data/Shader/Trajectory_vs.cso", "./Data/Shader/Trajectory_ps.cso");
	}
}

void Trajectory::Update(float elapsed_time)
{
	//頂点情報セット
	HRESULT hr = S_OK;

	//軌跡位置保存変数
	std::vector<PosBuffer>		UsedArrayBuffer;

	//セットしてある軌跡の位置からアルファ値が0ではないものを保存していく
	if (posArray.size() > 0)
	{
		for (int i = 0; i < posArray.size(); i++)
		{
			if (posArray[i].alpha > 0.0f)
			{
				//アルファ値が0以上なら生成。ついでにアルファ値を下げていく
				UsedArrayBuffer.push_back(PosBuffer());
				UsedArrayBuffer.back().head = posArray[i].head;
				UsedArrayBuffer.back().tail = posArray[i].tail;
				UsedArrayBuffer.back().alpha = posArray[i].alpha;
				posArray[i].alpha -= elapsed_time;
			}
		}

		DeletePos();

		//バッファをセットするために位置を送る
		std::vector<PosData> vertex;
		vertex.resize(UsedArrayBuffer.size() * 2);
		float amount = 1.0f / (UsedArrayBuffer.size() - 1);
		float v = 0;

		for (size_t i = 0, j = 0; i < vertex.size() && j < UsedArrayBuffer.size(); i += 2, ++j)
		{
			vertex[i].pos = UsedArrayBuffer[j].head;
			vertex[i].alpha = UsedArrayBuffer[j].alpha;
			vertex[i].uv = XMFLOAT2(1.0f, v);
			vertex[i + 1].pos = UsedArrayBuffer[j].tail;
			vertex[i + 1].alpha = UsedArrayBuffer[j].alpha;
			vertex[i + 1].uv = DirectX::XMFLOAT2(0.0f,v);
			v += amount;
		}


		HRESULT hr = S_OK;
		D3D11_MAP maptype = D3D11_MAP_WRITE_DISCARD;
		//ID3D11Resource *resouse;
		/*UINT mapflug;*/
		D3D11_MAPPED_SUBRESOURCE mapsub;

		hr = FRAMEWORK.context->Map(vertex_buffer.Get(), 0, maptype, 0, &mapsub);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		PosData *vertics = static_cast<PosData*>(mapsub.pData);

		Index_size = std::min(vertex.size(), max_count);

		size_t i_size = std::min(vertex.size(), max_count);

		for (size_t i = 0; i < i_size; i++)
		{
			vertics[i].pos = vertex[i].pos;
			vertics[i].uv = vertex[i].uv;
			vertics[i].alpha = vertex[i].alpha;
		}

		FRAMEWORK.context->Unmap(vertex_buffer.Get(), 0);
		//D3D11_BUFFER_DESC buffer_desc{};
		//buffer_desc.ByteWidth = vertex.size();
		//buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		//buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;// D3D11_USAGE_DYNAMIC;
		//buffer_desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
		//buffer_desc.MiscFlags = 0;
		//buffer_desc.StructureByteStride = 0;

		//D3D11_SUBRESOURCE_DATA subresource = {};
		//subresource.pSysMem = vertex.data();
		//subresource.SysMemPitch = 0;
		//subresource.SysMemSlicePitch = 0;

		//hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, vertex_buffer.GetAddressOf());
		//_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		////インデックス情報セット
		//ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
		//ZeroMemory(&subresource, sizeof(D3D11_SUBRESOURCE_DATA));
		//buffer_desc.ByteWidth = sizeof(indices);
		//buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		//buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;// D3D11_USAGE_DYNAMIC;
		//buffer_desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
		//buffer_desc.MiscFlags = 0;
		//buffer_desc.StructureByteStride = 0;
		//subresource.pSysMem = indices;
		//subresource.SysMemPitch = 0;
		//subresource.SysMemSlicePitch = 0;

		//hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, index_buffer.GetAddressOf());
		//_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	}
	else
	{
		Index_size = 0;
	}
}

void Trajectory::SetTrajectoryPos(const DirectX::XMFLOAT3& headPos, const DirectX::XMFLOAT3& tailPos)
{
	//剣の先端と根本の位置を登録する
	posArray.push_back(PosBuffer());
	posArray.back().head = headPos;
	posArray.back().tail = tailPos;
	posArray.back().alpha = 1.0f;
}


void Trajectory::DeletePos()
{
	//アルファ値が0以下の場合、情報を削除する
	auto result = std::remove_if(posArray.begin(), posArray.end(),
		[](PosBuffer& p)
		{
			return p.alpha <= 0.0f;
		});
	posArray.erase(result, posArray.end());
}

void Trajectory::render(
	const DirectX::XMFLOAT3& pos,
	const DirectX::XMFLOAT3& scale,
	const DirectX::XMFLOAT3& angle,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& material_color,
	const bool				viewflag
)
{
	if (vertex_buffer != nullptr)
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
		DirectX::XMMATRIX world_matrix = s * r * t;
		DirectX::XMFLOAT4X4 world;
		DirectX::XMStoreFloat4x4(&world, world_matrix);
		//ワールド・ビュー・プロジェクション行列作成
		DirectX::XMFLOAT4X4 world_view_projection;
		DirectX::XMStoreFloat4x4(&world_view_projection, world_matrix * view * projection);

		UINT stride = sizeof(PosData);
		UINT offset = 0;
		//頂点バッファのバインド
		FRAMEWORK.context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);


		FRAMEWORK.context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//定数バッファのバインド
		cbuffer cb = {};
		cb.world = world;
		cb.world_view_projection = world_view_projection;
		cb.material_color = material_color;
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
		cb.dummy2 = { 0.0f,0.0f,0.0f,0.0f };
		FRAMEWORK.context->UpdateSubresource(constant_buffer.Get(), 0, 0, &cb, 0, 0);
		FRAMEWORK.context->VSSetConstantBuffers(NULL, 1, constant_buffer.GetAddressOf());
		FRAMEWORK.context->PSSetConstantBuffers(NULL, 1, constant_buffer.GetAddressOf());

		sampler_wrap->Set(0);

		//ステート・シェーダー設定
		if (viewflag)
		{
			FRAMEWORK.context->RSSetState(filling_state.Get());
		}
		else
		{
			FRAMEWORK.context->RSSetState(line_state.Get());
		}

		shader->Acivate();
		texture->Set(0);

		//プリミティブの描画
		FRAMEWORK.context->DrawIndexed(Index_size, NULL, NULL);

		shader->Inactivate();
	}
}