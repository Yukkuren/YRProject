#include<dxgi.h>
#include <d3d11.h>
#include <memory>
#include"Geometric_primitive.h"
#include "framework.h"
#include<map>

geometric_primitive::geometric_primitive()
{
	HRESULT hr = S_OK;
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



	//頂点情報セット

	//position,normalの順番
	vertex vertics[] = {
		//前面
		{ DirectX::XMFLOAT3(-1.0f,1.0f,-1.0f),DirectX::XMFLOAT3(-1.0f,-1.0f,-1.0f) },//0
		{ DirectX::XMFLOAT3(1.0f,1.0f,-1.0f),DirectX::XMFLOAT3(-1.0f,-1.0f,-1.0f) },//1
		{ DirectX::XMFLOAT3(1.0f,-1.0f,-1.0f),DirectX::XMFLOAT3(-1.0f,-1.0f,-1.0f) },//2
		{ DirectX::XMFLOAT3(-1.0f,-1.0f,-1.0f),DirectX::XMFLOAT3(-1.0f,-1.0f,-1.0f) },//3
		//前面を見て右面
		{ DirectX::XMFLOAT3(1.0f,1.0f,-1.0f),DirectX::XMFLOAT3(1.0f,-1.0f,-1.0f) },//4
		{ DirectX::XMFLOAT3(1.0f,1.0f,1.0f),DirectX::XMFLOAT3(1.0f,-1.0f,-1.0f) },//5
		{ DirectX::XMFLOAT3(1.0f,-1.0f,1.0f),DirectX::XMFLOAT3(1.0f,-1.0f,-1.0f) },//6
		{ DirectX::XMFLOAT3(1.0f,-1.0f,-1.0f),DirectX::XMFLOAT3(1.0f,-1.0f,-1.0f) },//7
		//前面を見て左面
		{ DirectX::XMFLOAT3(-1.0f,1.0f,1.0f),DirectX::XMFLOAT3(-1.0f,-1.0f,-1.0f) },//8
		{ DirectX::XMFLOAT3(-1.0f,1.0f,-1.0f),DirectX::XMFLOAT3(-1.0f,-1.0f,-1.0f) },//9
		{ DirectX::XMFLOAT3(-1.0f,-1.0f,-1.0f),DirectX::XMFLOAT3(-1.0f,-1.0f,-1.0f) },//10
		{ DirectX::XMFLOAT3(-1.0f,-1.0f,1.0f),DirectX::XMFLOAT3(-1.0f,-1.0f,-1.0f) },//11
		//裏面
		{ DirectX::XMFLOAT3(1.0f,1.0f,1.0f),DirectX::XMFLOAT3(-1.0f,-1.0f,1.0f) },//12
		{ DirectX::XMFLOAT3(-1.0f,1.0f,1.0f),DirectX::XMFLOAT3(-1.0f,-1.0f,1.0f) },//13
		{ DirectX::XMFLOAT3(-1.0f,-1.0f,1.0f),DirectX::XMFLOAT3(-1.0f,-1.0f,1.0f) },//14
		{ DirectX::XMFLOAT3(1.0f,-1.0f,1.0f),DirectX::XMFLOAT3(-1.0f,-1.0f,1.0f) },//15
		//上面
		{ DirectX::XMFLOAT3(-1.0f,1.0f,1.0f),DirectX::XMFLOAT3(-1.0f,1.0f,-1.0f) },//16
		{ DirectX::XMFLOAT3(1.0f,1.0f,1.0f),DirectX::XMFLOAT3(-1.0f,1.0f,-1.0f) },//17
		{ DirectX::XMFLOAT3(1.0f,1.0f,-1.0f),DirectX::XMFLOAT3(-1.0f,1.0f,-1.0f) },//18
		{ DirectX::XMFLOAT3(-1.0f,1.0f,-1.0f),DirectX::XMFLOAT3(-1.0f,1.0f,-1.0f) },//19
		//下面
		{ DirectX::XMFLOAT3(1.0f,-1.0f,1.0f),DirectX::XMFLOAT3(-1.0f,-1.0f,-1.0f) },//20
		{ DirectX::XMFLOAT3(-1.0f,-1.0f,1.0f),DirectX::XMFLOAT3(-1.0f,-1.0f,-1.0f) },//21
		{ DirectX::XMFLOAT3(-1.0f,-1.0f,-1.0f),DirectX::XMFLOAT3(-1.0f,-1.0f,-1.0f) },//22
		{ DirectX::XMFLOAT3(1.0f,-1.0f,-1.0f),DirectX::XMFLOAT3(-1.0f,-1.0f,-1.0f) },//23
	};
	u_int indices[] = {
		//時計回りで表面
		//前面
		0, 1, 2,
		2, 3, 0,
		//前面を見て右面
		4, 5, 6,
		6, 7, 4,
		//前面を見て左面
		8, 9, 10,
		10, 11, 8,
		//裏面
		12,13,14,
		14,15,12,
		//上面
		16,17,18,
		18,19,16,
		//下面
		20,21,22,
		22,23,20,
	};

	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.ByteWidth = sizeof(vertics);
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;// D3D11_USAGE_DYNAMIC;
	buffer_desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA subresource = {};
	subresource.pSysMem = vertics;
	subresource.SysMemPitch = 0;
	subresource.SysMemSlicePitch = 0;

	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, vertex_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//インデックス情報セット
	ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&subresource, sizeof(D3D11_SUBRESOURCE_DATA));
	buffer_desc.ByteWidth = sizeof(indices);
	buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;// D3D11_USAGE_DYNAMIC;
	buffer_desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	subresource.pSysMem = indices;
	subresource.SysMemPitch = 0;
	subresource.SysMemSlicePitch = 0;
	
	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, index_buffer.GetAddressOf());
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

}


void  geometric_primitive::render(
	YRShader		*shader,
	const DirectX::XMFLOAT3& pos,
	const DirectX::XMFLOAT3& scale,
	const DirectX::XMFLOAT3& angle,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	//UNIT.23
	const DirectX::XMFLOAT4			material_color,
	const bool						viewflag
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
	//行列合成と変換
	DirectX::XMMATRIX world_matrix = s * r * t;
	DirectX::XMFLOAT4X4 world;
	DirectX::XMStoreFloat4x4(&world, world_matrix);
	//ワールド・ビュー・プロジェクション行列作成
	DirectX::XMFLOAT4X4 world_view_projection;
	DirectX::XMStoreFloat4x4(&world_view_projection, world_matrix * view * projection);


	UINT stride = sizeof(vertex);
	UINT offset = 0;
	//頂点バッファのバインド
	FRAMEWORK.context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);

	
	//インデックスバッファのバインド
	FRAMEWORK.context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, offset);

	FRAMEWORK.context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

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
	FRAMEWORK.context->UpdateSubresource(constant_buffer.Get(),0,0,&cb,0,0);
	FRAMEWORK.context->VSSetConstantBuffers(NULL, 1, constant_buffer.GetAddressOf());
	FRAMEWORK.context->PSSetConstantBuffers(NULL, 1, constant_buffer.GetAddressOf());


	//ステート・シェーダー設定
	if (viewflag)
	{
		FRAMEWORK.context->RSSetState(filling_state.Get());
	}
	else
	{
		FRAMEWORK.context->RSSetState(line_state.Get());
	}
	/*FRAMEWORK.context->IASetInputLayout(input_layout);
	FRAMEWORK.context->VSSetShader(vertex_shader, NULL, 0);
	FRAMEWORK.context->PSSetShader(pixel_shader, NULL, 0);*/
	shader->Acivate();

	//プリミティブの描画
	FRAMEWORK.context->DrawIndexed(36, NULL, NULL);

	shader->Inactivate();
}