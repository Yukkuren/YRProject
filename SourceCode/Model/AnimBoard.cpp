#include "AnimBoard.h"
#include "../framework.h"

AnimBoard::AnimBoard(std::shared_ptr<Texture> texture,int split_size, XMFLOAT2 tex_size, XMINT2 tex_split, XMFLOAT2 tex_max_size)
{
	HRESULT hr = S_OK;
	this->texture = texture;
	this->split_size = split_size;
	type = 0;
	this->tex_size = tex_size;
	this->tex_split = tex_split;
	this->tex_max_size = tex_max_size;

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


	VERTEX v = {
		//位置
		//板ポリの半分のワールド空間のサイズ
		//テクスチャ空間の切り取る左上の座標
		//テクスチャ空間の切り取る全体サイズ
		//色
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT2(1,1),
		XMFLOAT2(1,1),
		XMFLOAT2(1,1),
		XMFLOAT4(1,1,1,0), //左上
	};

	//	頂点バッファ作成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(v); //一頂点分のバッファ
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	hr = FRAMEWORK.device->CreateBuffer(&bd, NULL, VertexBuffer.GetAddressOf());
	assert(SUCCEEDED(hr));

	// 定数バッファ生成
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(CBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;

	hr = FRAMEWORK.device->CreateBuffer(&cbd, nullptr, ConstantBuffer.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void AnimBoard::Render(
	YRShader* shader,
	DirectX::XMFLOAT3& pos,
	DirectX::XMFLOAT2& size,
	DirectX::XMFLOAT3& angle,
	float						speed,
	const DirectX::XMMATRIX&	view,
	const DirectX::XMMATRIX&	projection,
	float						elapsed_time,
	float						alpha,
	const bool					viewflag
)
{

	//シェーダ有効化
	shader->Acivate();


	XMMATRIX s, r;

	//拡大行列作成
	s = XMMatrixScaling(size.x, size.y, 0.0f);
	//回転行列作成
	r = XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	XMMATRIX SR_Matrix = s * r;

	//定数バッファの更新
	CBuffer cb;
	XMFLOAT4X4 fv, fp;
	XMStoreFloat4x4(&fv, view);
	cb.view = fv;
	XMStoreFloat4x4(&fp, projection);
	cb.projection = fp;
	XMStoreFloat4x4(&cb.SR_Matrix, SR_Matrix);
	FRAMEWORK.context->UpdateSubresource(ConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
	FRAMEWORK.context->VSSetConstantBuffers(0, 1, ConstantBuffer.GetAddressOf());
	FRAMEWORK.context->GSSetConstantBuffers(0, 1, ConstantBuffer.GetAddressOf());
	FRAMEWORK.context->PSSetConstantBuffers(0, 1, ConstantBuffer.GetAddressOf());

	//ブレンドステート設定
	FRAMEWORK.BlendSet(Blend::ALPHA);
	//ステート・シェーダー設定
	if (viewflag)
	{
		FRAMEWORK.context->RSSetState(filling_state.Get());
	}
	else
	{
		FRAMEWORK.context->RSSetState(line_state.Get());
	}
	//デプスステンシルステート設定
	FRAMEWORK.context->OMSetDepthStencilState(depth_state.Get(), 1);
	//プリミティブ・トポロジーをセット
	FRAMEWORK.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	//テクスチャ設定
	if (texture) texture->Set(0);
	VERTEX v;
	v.Pos = pos;
	int nxb = type % tex_split.x;
	int nyb = type / tex_split.x;
	v.UV_Pos.x = static_cast<float>(nxb) * tex_size.x / tex_max_size.x;
	v.UV_Pos.y = static_cast<float>(nyb) * tex_size.y / tex_max_size.y;
	v.UV_Size.x = tex_size.x / tex_max_size.x;
	v.UV_Size.y = tex_size.y / tex_max_size.y;
	v.Color.x = type;
	v.Color.y = v.Color.z = 1.0f;
	v.Color.w = alpha;
	v.Size = size;

	//頂点データ更新
	FRAMEWORK.context->UpdateSubresource(VertexBuffer.Get(), 0, nullptr, &v, 0, 0);

	//バーテックスバッファーをセット
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	FRAMEWORK.context->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &stride, &offset);

	//レンダリング(インデックス付き)
	FRAMEWORK.context->Draw(1, 0);

	elapsed += elapsed_time;

	if (speed < elapsed)
	{
		type++;
		elapsed = 0.0f;
		if (type >= split_size)
		{
			type = 0;
		}
	}

	//シェーダ無効化
	shader->Inactivate();
}