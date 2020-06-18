#include<dxgi.h>
#include <d3d11.h>
#include <memory>
#include"Geometric_primitive.h"
#include "framework.h"
#include<map>


namespace ALL
{
	HRESULT create_vertex(const char *cso_file, ID3D11VertexShader **vert, D3D11_INPUT_ELEMENT_DESC *layout, UINT numElements, ID3D11InputLayout **input)
	{

		struct Vertex_and_Layout
		{
			Vertex_and_Layout(ID3D11VertexShader *vert, ID3D11InputLayout *input) : pVertex(vert), pInput(input) {}
			Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertex;
			Microsoft::WRL::ComPtr<ID3D11InputLayout> pInput;
		};
		static std::map<std::string, Vertex_and_Layout> cache;

		auto it = cache.find(cso_file);
		if (it != cache.end())
		{
			*vert = it->second.pVertex.Get();
			(*vert)->AddRef();
			*input = it->second.pInput.Get();
			(*input)->AddRef();
			return S_OK;
		}


		HRESULT hr = S_OK;
		FILE *fp = 0;

		fopen_s(&fp, cso_file, "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");
		fseek(fp, 0, SEEK_END);
		long cso_sz = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		std::unique_ptr<unsigned char[]>cso_data = std::make_unique< unsigned char[]>(cso_sz);
		fread(cso_data.get(), cso_sz, 1, fp);
		fclose(fp);

		hr = FRAMEWORK.device->CreateVertexShader(cso_data.get(), cso_sz, NULL, vert);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		hr = FRAMEWORK.device->CreateInputLayout(layout, numElements, cso_data.get(),
			cso_sz, input);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		cache.insert(std::make_pair(cso_file, Vertex_and_Layout(*vert, *input)));
		return hr;
	}

	HRESULT CreatePixel(const char *ps_file, ID3D11PixelShader **pixel)
	{
		static std::map < std::string, Microsoft::WRL::ComPtr<ID3D11PixelShader>> pixelcache;

		auto it = pixelcache.find(ps_file);
		if (it != pixelcache.end())
		{
			*pixel = it->second.Get();
			(*pixel)->AddRef();
			return S_OK;
		}

		HRESULT hr = S_OK;

		FILE *fpp = 0;

		fopen_s(&fpp, ps_file, "rb");
		_ASSERT_EXPR_A(fpp, "CSO File not found");
		fseek(fpp, 0, SEEK_END);
		long cso_szp = ftell(fpp);
		fseek(fpp, 0, SEEK_SET);
		std::unique_ptr<unsigned char[]>cso_datap = std::make_unique< unsigned char[]>(cso_szp);
		fread(cso_datap.get(), cso_szp, 1, fpp);
		fclose(fpp);

		hr = FRAMEWORK.device->CreatePixelShader(cso_datap.get(), cso_szp, NULL, pixel);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		//delete[]cso_datap;

		pixelcache.insert(std::make_pair(ps_file, *pixel));

		return hr;
	}
}

geometric_primitive::geometric_primitive()
{
	HRESULT hr = S_OK;
	
	D3D11_INPUT_ELEMENT_DESC element_desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(element_desc);

	
	//頂点シェーダーオブジェクト生成
	//入力レイアウトオブジェクト生成
	//ピクセルシェーダーオブジェクト生成
	ALL::create_vertex("./Data/Shader/geometric_primitive_vs.cso", &vertex_shader, element_desc, numElements, &input_layout);
	ALL::CreatePixel("./Data/Shader/geometric_primitive_ps.cso", &pixel_shader);


		//geometric_primitive_ps.cso
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

	hr = FRAMEWORK.device->CreateRasterizerState(&rasterizer_desc, &line_state);
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
	hr = FRAMEWORK.device->CreateRasterizerState(&rasterizer_desc, &filling_state);
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

	
	hr = FRAMEWORK.device->CreateDepthStencilState(&depth_desc, &depth_state);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));



	//頂点情報セット

	vertex vertics[] = {
		//前面
		{ DirectX::XMFLOAT3(0.0f,1.0f,0.0f),DirectX::XMFLOAT3(0.0f,0.0f,-1.0f) },//0
		{ DirectX::XMFLOAT3(1.0f,1.0f,0.0f),DirectX::XMFLOAT3(0.0f,0.0f,-1.0f) },//1
		{ DirectX::XMFLOAT3(1.0f,0.0f,0.0f),DirectX::XMFLOAT3(0.0f,0.0f,-1.0f) },//2
		{ DirectX::XMFLOAT3(0.0f,0.0f,0.0f),DirectX::XMFLOAT3(0.0f,0.0f,-1.0f) },//3
		//前面を見て右面
		{ DirectX::XMFLOAT3(1.0f,1.0f,0.0f),DirectX::XMFLOAT3(1.0f,0.0f,0.0f) },//4
		{ DirectX::XMFLOAT3(1.0f,1.0f,1.0f),DirectX::XMFLOAT3(1.0f,0.0f,0.0f) },//5
		{ DirectX::XMFLOAT3(1.0f,0.0f,1.0f),DirectX::XMFLOAT3(1.0f,0.0f,0.0f) },//6
		{ DirectX::XMFLOAT3(1.0f,0.0f,0.0f),DirectX::XMFLOAT3(1.0f,0.0f,0.0f) },//7
		//前面を見て左面
		{ DirectX::XMFLOAT3(0.0f,1.0f,1.0f),DirectX::XMFLOAT3(-1.0f,0.0f,0.0f) },//8
		{ DirectX::XMFLOAT3(0.0f,1.0f,0.0f),DirectX::XMFLOAT3(-1.0f,0.0f,0.0f) },//9
		{ DirectX::XMFLOAT3(0.0f,0.0f,0.0f),DirectX::XMFLOAT3(-1.0f,0.0f,0.0f) },//10
		{ DirectX::XMFLOAT3(0.0f,0.0f,1.0f),DirectX::XMFLOAT3(-1.0f,0.0f,0.0f) },//11
		//裏面
		{ DirectX::XMFLOAT3(1.0f,1.0f,1.0f),DirectX::XMFLOAT3(0.0f,0.0f,1.0f) },//12
		{ DirectX::XMFLOAT3(0.0f,1.0f,1.0f),DirectX::XMFLOAT3(0.0f,0.0f,1.0f) },//13
		{ DirectX::XMFLOAT3(0.0f,0.0f,1.0f),DirectX::XMFLOAT3(0.0f,0.0f,1.0f) },//14
		{ DirectX::XMFLOAT3(1.0f,0.0f,1.0f),DirectX::XMFLOAT3(0.0f,0.0f,1.0f) },//15
		//上面
		{ DirectX::XMFLOAT3(0.0f,1.0f,1.0f),DirectX::XMFLOAT3(0.0f,1.0f,0.0f) },//16
		{ DirectX::XMFLOAT3(1.0f,1.0f,1.0f),DirectX::XMFLOAT3(0.0f,1.0f,0.0f) },//17
		{ DirectX::XMFLOAT3(1.0f,1.0f,0.0f),DirectX::XMFLOAT3(0.0f,1.0f,0.0f) },//18
		{ DirectX::XMFLOAT3(0.0f,1.0f,0.0f),DirectX::XMFLOAT3(0.0f,1.0f,0.0f) },//19
		//下面
		{ DirectX::XMFLOAT3(1.0f,0.0f,1.0f),DirectX::XMFLOAT3(0.0f,-1.0f,0.0f) },//20
		{ DirectX::XMFLOAT3(0.0f,0.0f,1.0f),DirectX::XMFLOAT3(0.0f,-1.0f,0.0f) },//21
		{ DirectX::XMFLOAT3(0.0f,0.0f,0.0f),DirectX::XMFLOAT3(0.0f,-1.0f,0.0f) },//22
		{ DirectX::XMFLOAT3(1.0f,0.0f,0.0f),DirectX::XMFLOAT3(0.0f,-1.0f,0.0f) },//23
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

	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, &vertex_buffer);
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
	
	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, &index_buffer);
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

	hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, nullptr/*&subresource*/, &constant_buffer);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

}


void  geometric_primitive::render(
	const DirectX::XMFLOAT4X4	&world_view,		//ワールド・ビュー・プロジェクション合成行列
	const DirectX::XMFLOAT4X4	&world_matrix,		//ワールド変換行列
	const DirectX::XMFLOAT4		&light_direction,	//ライト進行方向
	const DirectX::XMFLOAT4		&material_color,	//材質色
	bool						viewflag			//線・塗りつぶし描画フラグ
)
{
	UINT stride = sizeof(vertex);
	UINT offset = 0;
	//頂点バッファのバインド
	FRAMEWORK.context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
	
	//インデックスバッファのバインド
	FRAMEWORK.context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, offset);

	FRAMEWORK.context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//定数バッファのバインド
	cbuffer cb = {};
	cb.world = world_matrix;
	cb.world_view_projection = world_view;
	cb.light_direction = light_direction;
	cb.material_color = material_color;
	FRAMEWORK.context->UpdateSubresource(constant_buffer,0,0,&cb,0,0);
	FRAMEWORK.context->VSSetConstantBuffers(NULL, 1, &constant_buffer);


	//ステート・シェーダー設定
	if (viewflag)
	{
		FRAMEWORK.context->RSSetState(line_state);
	}
	else
	{
		FRAMEWORK.context->RSSetState(filling_state);
	}
	FRAMEWORK.context->IASetInputLayout(input_layout);
	FRAMEWORK.context->VSSetShader(vertex_shader, NULL, 0);
	FRAMEWORK.context->PSSetShader(pixel_shader, NULL, 0);

	//プリミティブの描画
	FRAMEWORK.context->DrawIndexed(36, NULL, NULL);

	
}