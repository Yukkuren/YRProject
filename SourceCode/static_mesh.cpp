#include<dxgi.h>
#include <d3d11.h>
#include <memory>
#include"Model/Geometric_primitive.h"
#include "framework.h"
#include<map>
#include"static_mesh.h"
#include<fstream>
#include<string>
#include"../DirectXTK-master\Inc\WICTextureLoader.h"

namespace MeshTexShader
{
	//static wchar_t *objflilename=0;


	HRESULT OBJ_load(
		unsigned int *indexsize,
		const wchar_t *obj_file,
		ID3D11Buffer**vertex_buffer,
		ID3D11Buffer**index_buffer,
		ID3D11Buffer**constant_buffer,
		std::wstring *mtl_name,
		std::wstring *texture_name,
		std::vector<Static_mesh::material> *materials,
		std::vector<Static_mesh::subset> *subsets
	)
	{

		HRESULT hr = S_OK;

		struct OBJ_Buffer_Layout
		{
			OBJ_Buffer_Layout(ID3D11Buffer *vert, ID3D11Buffer *index, ID3D11Buffer *constant) : pVertex(vert), pIndex(index),pConstant(constant) {}
			Microsoft::WRL::ComPtr<ID3D11Buffer> pVertex;
			Microsoft::WRL::ComPtr<ID3D11Buffer> pIndex;
			Microsoft::WRL::ComPtr<ID3D11Buffer> pConstant;
		};

		static std::map<const wchar_t *, OBJ_Buffer_Layout> objcache;

		std::vector<Static_mesh::vertex> vertices;
		std::vector<u_int> indices;
		u_int current_index = 0;
		//int count = 1;

		std::vector<DirectX::XMFLOAT3> positions;
		std::vector<DirectX::XMFLOAT3> normals;
		std::vector<DirectX::XMFLOAT2> texcoord;
		std::wstring name_Front;
		std::wstring name_Second;

		auto it = objcache.find(obj_file);
		if (it != objcache.end())
		{
			*vertex_buffer = it->second.pVertex.Get();
			(*vertex_buffer)->AddRef();
			*index_buffer = it->second.pIndex.Get();
			(*index_buffer)->AddRef();
			*constant_buffer = it->second.pConstant.Get();
			(*constant_buffer)->AddRef();
			return S_OK;
		}

		std::wifstream fin(obj_file);
		_ASSERT_EXPR(fin, L"'OBJ file not fuund.");
		wchar_t command[256];

		while (fin)
		{
			fin >> command;
			if (0 == wcscmp(command, L"mtllib"))
			{
				fin >> name_Second;
				fin.ignore(1024, L'\n');
			}
			if (0 == wcscmp(command, L"usemtl"))
			{
				if (!subsets->empty())
				{
					subsets->back().index_count = indices.size();
				}
				//count++;
				subsets->push_back(Static_mesh::subset());
				fin >> subsets->back().usemtl;
				subsets->back().index_start = indices.size();
				fin.ignore(1024, L'\n');
			}
			if (0 == wcscmp(command, L"v"))
			{
				float x, y, z;
				fin >> x >> y >> z;
				positions.push_back(DirectX::XMFLOAT3(x, y, z));
				fin.ignore(1024, L'\n');
			}
			else if (0 == wcscmp(command, L"vn"))
			{
				FLOAT i, j, k;
				fin >> i >> j >> k;
				normals.push_back(DirectX::XMFLOAT3(i, j, k));
				fin.ignore(1024, L'\n');
			}
			else if (0 == wcscmp(command, L"vt"))
			{
				float p, n;
				fin >> p >> n;
				texcoord.push_back(DirectX::XMFLOAT2(p, -n));
				fin.ignore(1024, L'\n');
			}
			else if (0 == wcscmp(command, L"f"))
			{
				for (u_int i = 0; i < 3; i++)
				{
					Static_mesh::vertex vertex;
					u_int v, vt, vn;
					//count++;
					fin >> v;
					vertex.position = positions[v - 1];
					if (L'/' == fin.peek())
					{
						//count++;
						fin.ignore();
						if (L'/' != fin.peek())
						{
							//count++;
							fin >> vt;
							vertex.texcoord = texcoord[vt - 1];
						}
						if (L'/' == fin.peek())
						{
							//count++;
							fin.ignore();
							fin >> vn;
							vertex.normal = normals[vn - 1];
						}
					}
					vertices.push_back(vertex);
					indices.push_back(current_index++);
				}
				//subsets->back().index_count++;
				fin.ignore(1024, L'\n');
			}
			else
			{
				fin.ignore(1024, L'\n');
			}
		}
		subsets->back().index_count = indices.size();

		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.ByteWidth = sizeof(Static_mesh::vertex)*vertices.size();
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;// D3D11_USAGE_DYNAMIC;
		buffer_desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA subresource = {};
		subresource.pSysMem = vertices.data();
		subresource.SysMemPitch = 0;
		subresource.SysMemSlicePitch = 0;

		hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, vertex_buffer);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		//インデックス情報セット
		ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&subresource, sizeof(D3D11_SUBRESOURCE_DATA));
		buffer_desc.ByteWidth = indices.size()*sizeof(u_int);
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;// D3D11_USAGE_DYNAMIC;
		buffer_desc.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		subresource.pSysMem = indices.data();
		*indexsize = indices.size();
		/*for (auto &sub : *subsets)
		{
			sub.index_count = indices.size();
		}*/
		subresource.SysMemPitch = 0;
		subresource.SysMemSlicePitch = 0;

		hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, &subresource, index_buffer);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&subresource, sizeof(D3D11_SUBRESOURCE_DATA));
		buffer_desc.ByteWidth = sizeof(Static_mesh::cbuffer);
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;/*
		subresource.pSysMem = constant_buffer;
		subresource.SysMemPitch = 0;
		subresource.SysMemSlicePitch = 0;*/

		//subresource.pSysMem = NULL;

		hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, nullptr/*&subresource*/, constant_buffer);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


		fin.close();
		name_Front = obj_file;
		*mtl_name = name_Front.substr(0, name_Front.find_last_of(L'/'));
		*mtl_name += L'/';
		*mtl_name += name_Second;

		TextureNameGet(texture_name, mtl_name,materials);


		return hr;
	}


	HRESULT TextureNameGet(std::wstring *texture_name, std::wstring *mtl_name, std::vector<Static_mesh::material> *materials)
	{
		HRESULT hr = S_OK;
		std::wifstream fin(*mtl_name);
		_ASSERT_EXPR(fin, L"'MTL file not fuund.");
		wchar_t command[256];
		std::wstring name_Front;
		std::wstring name_Second;
		//std::wstring *mtl_name3;

		while (fin)
		{
			fin >> command;
			if (0 == wcscmp(command, L"newmtl"))
			{
				materials->push_back(Static_mesh::material());
			}
			if (0 == wcscmp(command, L"map_Kd"))
			{
			fin >> name_Second;
			name_Front = *mtl_name;
			materials->back().map_Kd = name_Front.substr(0, name_Front.find_last_of(L'/'));
			materials->back().map_Kd += L'/';
			materials->back().map_Kd += name_Second;
			fin.ignore(1024, L'\n');
			}
			if (0 == wcscmp(command, L"illum"))
			{
				float i;
				fin >> i;
				materials->back().illum = static_cast<u_int>(i);
			}
			if (0 == wcscmp(command, L"newmtl"))
			{
				fin >> materials->back().newmtl;
			}
			if (0 == wcscmp(command, L"Ka"))
			{
				float x, y, z;
				fin >> x >> y >> z;
				materials->back().Ka = DirectX::XMFLOAT3(x, y, z);
				fin.ignore(1024, L'\n');
			}
			else if (0 == wcscmp(command, L"Kd"))
			{
				float l, m, n;
				fin >> l >> m >> n;
				materials->back().Kd = DirectX::XMFLOAT3(l, m, n);
				fin.ignore(1024, L'\n');
			}
			else if (0 == wcscmp(command, L"Ks"))
			{
				float o, b, j;
				fin >> o >> b >> j;
				materials->back().Ks = DirectX::XMFLOAT3(o, b, j);
				fin.ignore(1024, L'\n');
			}
		}
		fin.close();
		name_Front = *mtl_name;
		*texture_name=name_Front.substr(0, name_Front.find_last_of(L'/'));
		*texture_name += L'/';
		*texture_name += name_Second;
		return hr;
	}



	static std::map < std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> > tex;
	HRESULT TextureLoad(std::wstring* wchar, D3D11_TEXTURE2D_DESC *texture2d_desc, ID3D11ShaderResourceView **pShader)
	{
		HRESULT hr = S_OK;

		ID3D11Resource *pResouse;
		//std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>::iterator it = tex.find(wchar);
		auto it = tex.find(*wchar);
		//_ASSERT_EXPR(tex, L"'Texture file not fuund.");
		if (it != tex.end())
		{
			*pShader = it->second.Get();
			(*pShader)->AddRef();
			(*pShader)->GetResource(&pResouse);
		}

		else
		{
			const wchar_t *file_name = wchar->data();
			hr = DirectX::CreateWICTextureFromFile(FRAMEWORK.device.Get(), file_name, &pResouse, pShader);
			//std::wstring extension = PathFindExtensionW(wchar);
			tex.insert(std::make_pair(file_name, *pShader));
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
		std::unique_ptr<unsigned char[]> cso_data = std::make_unique< unsigned char[]>(cso_sz);
		fread(cso_data.get(), cso_sz, 1, fp);
		fclose(fp);

		hr = FRAMEWORK.device->CreateVertexShader(cso_data.get(), cso_sz, NULL, vert);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		hr = FRAMEWORK.device->CreateInputLayout(layout, numElements, cso_data.get(),
			cso_sz, input);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		//delete[]cso_data;


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














Static_mesh::Static_mesh(const wchar_t* obj_name)
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

	MeshTexShader::OBJ_load(
		&indexsize,
		obj_name,
		&vertex_buffer,
		&index_buffer,
		&constant_buffer,
		&mtl_name,
		&Texture_name,
		&materials,
		&subsets
		);

	//↓OBJ_Load内で呼ぶようにしたので消去
	//ALL::TextureNameGet(&Texture_name, L"./Cup/Cup.mtl");


	MeshTexShader::TextureLoad(&Texture_name, &texture2d_desc, &shader_view);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	for (auto &mate : materials)
	{
		MeshTexShader::TextureLoad(&mate.map_Kd, &texture2d_desc, &mate.shader_resouce_view);
	}


	D3D11_SAMPLER_DESC sampler_desc;
	//ID3D11SamplerState *samplestate;

	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MipLODBias = 0.0f;
	sampler_desc.MaxAnisotropy = 16;
	sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	memcpy(sampler_desc.BorderColor, &DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), sizeof(DirectX::XMFLOAT4));
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;


	hr = FRAMEWORK.device->CreateSamplerState(&sampler_desc, &sampler_state);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

}

void  Static_mesh::render(
	YRShader					*shader,
	const DirectX::XMFLOAT4X4	&world_view,		//ワールド・ビュー・プロジェクション合成行列
	const DirectX::XMFLOAT4X4	&world_matrix,		//ワールド変換行列
	const DirectX::XMFLOAT4		&light_direction,	//ライト進行方向
	const DirectX::XMFLOAT4		&material_color,	//材質色
	bool						viewflag			//線・塗りつぶし描画フラグ
)
{
	UINT stencil = 1;
	FRAMEWORK.context->OMSetDepthStencilState(depth_state, stencil);

	UINT stride = sizeof(vertex);
	UINT offset = 0;
	//頂点バッファのバインド
	FRAMEWORK.context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
	//インデックスバッファのバインド
	FRAMEWORK.context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);

	FRAMEWORK.context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//定数バッファのバインド
	cbuffer cb = {};
	cb.world = world_matrix;
	cb.world_view_projection = world_view;
	cb.light_direction = light_direction;
	cb.material_color = material_color;
	//context->UpdateSubresource(constant_buffer, 0, 0, &cb, 0, 0);
	//context->VSSetConstantBuffers(0, 1, &constant_buffer);


	//ステート・シェーダー設定
	if (viewflag)
	{
		FRAMEWORK.context->RSSetState(line_state);
	}
	else
	{
		FRAMEWORK.context->RSSetState(filling_state);
	}
	shader->Acivate();

	//プリミティブの描画
	/*context->DrawIndexed(indexsize, NULL, NULL);
	context->Draw(4, 0);*/
	for (auto &material : materials)
	{
		cbuffer data;
		data.world_view_projection = world_view;
		data.world = world_matrix;
		data.light_direction = light_direction;
		data.material_color.x = material_color.x*material.Kd.x;
		data.material_color.y = material_color.y*material.Kd.y;
		data.material_color.z = material_color.z*material.Kd.z;
		data.material_color.w = material_color.w;
		FRAMEWORK.context->UpdateSubresource(constant_buffer, 0, 0, &data, 0, 0);
		FRAMEWORK.context->VSSetConstantBuffers(0, 1, &constant_buffer);
		FRAMEWORK.context->PSSetShaderResources(0, 1, material.shader_resouce_view.GetAddressOf());
		FRAMEWORK.context->PSSetSamplers(0, 1, &sampler_state);
		for (auto &subset : subsets)
		{
			if (material.newmtl == subset.usemtl)
			{
				FRAMEWORK.context->DrawIndexed(subset.index_count, subset.index_start, 0);
			}
		}
	}
	shader->Inactivate();
}