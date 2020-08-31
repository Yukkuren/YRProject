#include <WICTextureLoader.h>
//#include "Logger.h"
#include "Misc.h"
#include "Font.h"

Font::Font(ID3D11Device* device, const char* filename, int maxSpriteCount)
{
	// 頂点シェーダー
	{
		// ファイルを開く
		FILE* fp = nullptr;
		fopen_s(&fp, "Data\\Shader\\Font_vs.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// ファイルのサイズを求める
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// メモリ上に頂点シェーダーデータを格納する領域を用意する
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		// 頂点シェーダー生成
		HRESULT hr = device->CreateVertexShader(csoData.get(), csoSize, nullptr, vertexShader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), csoData.get(), csoSize, inputLayout.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// ピクセルシェーダー
	{
		// ファイルを開く
		FILE* fp = nullptr;
		fopen_s(&fp, "Data\\Shader\\Font_ps.cso", "rb");
		_ASSERT_EXPR_A(fp, "CSO File not found");

		// ファイルのサイズを求める
		fseek(fp, 0, SEEK_END);
		long csoSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		// メモリ上に頂点シェーダーデータを格納する領域を用意する
		std::unique_ptr<u_char[]> csoData = std::make_unique<u_char[]>(csoSize);
		fread(csoData.get(), csoSize, 1, fp);
		fclose(fp);

		// ピクセルシェーダー生成
		HRESULT hr = device->CreatePixelShader(csoData.get(), csoSize, nullptr, pixelShader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// ブレンドステート
	{
		D3D11_BLEND_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.IndependentBlendEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = device->CreateBlendState(&desc, blendState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// 深度ステンシルステート
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		::memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = false;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;

		HRESULT hr = device->CreateDepthStencilState(&desc, depthStencilState.GetAddressOf());
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
		desc.CullMode = D3D11_CULL_NONE;
		desc.AntialiasedLineEnable = false;

		HRESULT hr = device->CreateRasterizerState(&desc, rasterizerState.GetAddressOf());
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

		HRESULT hr = device->CreateSamplerState(&desc, samplerState.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// 頂点バッファ
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * maxSpriteCount * 4);
		//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&bufferDesc, nullptr, vertexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// インデックスバッファ
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		D3D11_SUBRESOURCE_DATA subresourceData = {};

		std::unique_ptr<UINT[]> indices = std::make_unique<UINT[]>(maxSpriteCount * 6);
		// 四角形 を 三角形 ２つに展開
		// 0---1      0---1  4
		// |   |  →  |／  ／|
		// 2---3      2  3---5
		{
			UINT* p = indices.get();
			for (int i = 0; i < maxSpriteCount * 4; i += 4)
			{
				p[0] = i + 0;
				p[1] = i + 1;
				p[2] = i + 2;
				p[3] = i + 2;
				p[4] = i + 1;
				p[5] = i + 3;
				p += 6;
			}
		}

		bufferDesc.ByteWidth = static_cast<UINT>(sizeof(UINT) * maxSpriteCount * 6);
		//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		subresourceData.pSysMem = indices.get();
		subresourceData.SysMemPitch = 0; //Not use for index buffers.
		subresourceData.SysMemSlicePitch = 0; //Not use for index buffers.
		HRESULT hr = device->CreateBuffer(&bufferDesc, &subresourceData, indexBuffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	// .fnt 読み込み
	{
		FILE* fp = nullptr;
		fopen_s(&fp, filename, "rb");
		_ASSERT_EXPR_A(fp, "FNT File not found");

		fseek(fp, 0, SEEK_END);
		long fntSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		std::unique_ptr<char[]> fntData = std::make_unique<char[]>(fntSize);
		fread(fntData.get(), fntSize, 1, fp);
		fclose(fp);

		// (***.fnt)ヘッダ解析
		char* context = nullptr;	// 内部的に使用するので深く考えない
		const char* delimiter = " ,=\"\r\n";
		const char* delimiter2 = ",=\"\r\n";
		char* pToken = ::strtok_s(fntData.get(), delimiter, &context);
		_ASSERT_EXPR_A(::strcmp(pToken, "info") == 0, "FNT Format");

		// face
		::strtok_s(nullptr, delimiter, &context);
		const char* face = ::strtok_s(nullptr, delimiter2, &context);
		// size
		::strtok_s(nullptr, delimiter, &context);
		int size = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// bold
		::strtok_s(nullptr, delimiter, &context);
		int bold = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// italic
		::strtok_s(nullptr, delimiter, &context);
		int italic = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// charset
		::strtok_s(nullptr, delimiter, &context);
		const char* charset = ::strtok_s(nullptr, delimiter, &context);
		// unicode
		if (::strcmp(charset, "unicode") == 0)
			charset = "";
		else
			::strtok_s(nullptr, delimiter, &context);
		int unicode = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// stretchH
		::strtok_s(nullptr, delimiter, &context);
		int stretchH = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// smooth
		::strtok_s(nullptr, delimiter, &context);
		int smooth = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// aa
		::strtok_s(nullptr, delimiter, &context);
		int aa = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// padding
		::strtok_s(nullptr, delimiter, &context);
		int padding1 = ::atoi(::strtok_s(nullptr, delimiter, &context));
		int padding2 = ::atoi(::strtok_s(nullptr, delimiter, &context));
		int padding3 = ::atoi(::strtok_s(nullptr, delimiter, &context));
		int padding4 = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// spacing
		::strtok_s(nullptr, delimiter, &context);
		int spacing1 = ::atoi(::strtok_s(nullptr, delimiter, &context));
		int spacing2 = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// outline
		::strtok_s(nullptr, delimiter, &context);
		int outline = ::atoi(::strtok_s(nullptr, delimiter, &context));

		// common
		::strtok_s(nullptr, delimiter, &context);
		// lineHeight
		::strtok_s(nullptr, delimiter, &context);
		int lineHeight = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// base
		::strtok_s(nullptr, delimiter, &context);
		int base = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// scaleW
		::strtok_s(nullptr, delimiter, &context);
		int scaleW = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// scaleH
		::strtok_s(nullptr, delimiter, &context);
		int scaleH = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// pages
		::strtok_s(nullptr, delimiter, &context);
		int pages = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// packed
		::strtok_s(nullptr, delimiter, &context);
		int packed = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// alphaChnl
		::strtok_s(nullptr, delimiter, &context);
		int alphaChnl = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// redChnl
		::strtok_s(nullptr, delimiter, &context);
		int redChnl = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// greenChnl
		::strtok_s(nullptr, delimiter, &context);
		int greenChnl = ::atoi(::strtok_s(nullptr, delimiter, &context));
		// blueChnl
		::strtok_s(nullptr, delimiter, &context);
		int blueChnl = ::atoi(::strtok_s(nullptr, delimiter, &context));

		// ディレクトリパス取得
		char dirname[256];
		::_splitpath_s(filename, nullptr, 0, dirname, 256, nullptr, 0, nullptr, 0);
		shaderResourceViews.resize(pages);
		for (int i = 0; i < pages; i++)
		{
			// page
			::strtok_s(nullptr, delimiter, &context);
			// id
			::strtok_s(nullptr, delimiter, &context);
			int id = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// file
			::strtok_s(nullptr, delimiter, &context);
			const char* file = ::strtok_s(nullptr, delimiter2, &context);

			// 相対パスの解決
			char fname[256];
			::_makepath_s(fname, 256, nullptr, dirname, file, nullptr);

			// マルチバイト文字からワイド文字へ変換
			size_t length;
			wchar_t wfname[256];
			::mbstowcs_s(&length, wfname, 256, fname, _TRUNCATE);

			// テクスチャ読み込み
			Microsoft::WRL::ComPtr<ID3D11Resource> resource;
			HRESULT hr = DirectX::CreateWICTextureFromFile(device, wfname, resource.GetAddressOf(), shaderResourceViews.at(i).GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}

		// chars
		::strtok_s(nullptr, delimiter, &context);
		// count
		::strtok_s(nullptr, delimiter, &context);
		int count = ::atoi(::strtok_s(nullptr, delimiter, &context));

		// データ格納
		fontWidth = static_cast<float>(size);
		fontHeight = static_cast<float>(lineHeight);
		textureCount = pages;
		characterCount = count + 1;
		characterInfos.resize(characterCount);
		characterIndices.resize(0xFFFF);
		::memset(characterIndices.data(), 0, sizeof(WORD) * characterIndices.size());

		characterIndices.at(0x00) = CharacterInfo::EndCode;
		characterIndices.at(0x0a) = CharacterInfo::ReturnCode;
		characterIndices.at(0x09) = CharacterInfo::TabCode;
		characterIndices.at(0x20) = CharacterInfo::SpaceCode;

		// 文字情報解析
		int n = 1;
		for (int i = 0; i < count; i++)
		{
			// char
			::strtok_s(nullptr, delimiter, &context);
			// id
			::strtok_s(nullptr, delimiter, &context);
			int id = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// x
			::strtok_s(nullptr, delimiter, &context);
			int x = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// y
			::strtok_s(nullptr, delimiter, &context);
			int y = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// width
			::strtok_s(nullptr, delimiter, &context);
			int width = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// height
			::strtok_s(nullptr, delimiter, &context);
			int height = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// xoffset
			::strtok_s(nullptr, delimiter, &context);
			int xoffset = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// yoffset
			::strtok_s(nullptr, delimiter, &context);
			int yoffset = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// xadvance
			::strtok_s(nullptr, delimiter, &context);
			int xadvance = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// page
			::strtok_s(nullptr, delimiter, &context);
			int page = ::atoi(::strtok_s(nullptr, delimiter, &context));
			// chnl
			::strtok_s(nullptr, delimiter, &context);
			int chnl = ::atoi(::strtok_s(nullptr, delimiter, &context));

			// 文字コード格納
			if (id >= 0x10000) continue;

			CharacterInfo& info = characterInfos.at(n);

			characterIndices.at(id) = static_cast<WORD>(i + 1);

			// 文字情報格納
			info.left = static_cast<float>(x) / static_cast<float>(scaleW);
			info.top = static_cast<float>(y) / static_cast<float>(scaleH);
			info.right = static_cast<float>(x + width) / static_cast<float>(scaleW);
			info.bottom = static_cast<float>(y + height) / static_cast<float>(scaleH);
			info.xoffset = static_cast<float>(xoffset);
			info.yoffset = static_cast<float>(yoffset);
			info.xadvance = static_cast<float>(xadvance);
			info.width = static_cast<float>(width);
			info.height = static_cast<float>(height);
			info.page = page;
			info.ascii = (id < 0x100);

			n++;
		}
	}


}

void Font::Begin(ID3D11DeviceContext* context)
{
	// スクリーンサイズ取得
	D3D11_VIEWPORT viewport;
	UINT num_viewports = 1;
	context->RSGetViewports(&num_viewports, &viewport);
	screenWidth = viewport.Width;
	screenHeight = viewport.Height;

	// 頂点編集開始
	D3D11_MAPPED_SUBRESOURCE mapped_subresource;
	context->Map(vertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);

	currentVertex = reinterpret_cast<Vertex*>(mapped_subresource.pData);
	currentIndexCount = 0;
	currentPage = -1;
	subsets.clear();
}

void Font::Draw(float x, float y, const wchar_t* string)
{
	size_t length = ::wcslen(string);

	float start_x = x;
	float start_y = y;
	float space = fontWidth;

	for (size_t i = 0; i < length; ++i)
	{
		// 文字値から文字情報が格納されているコードを取得
		WORD word = static_cast<WORD>(string[i]);
		const wchar_t t = string[i];
		WORD code = characterIndices.at(word);

		// 特殊制御用コードの処理
		if (code == CharacterInfo::EndCode)
		{
			break;
		}
		else if (code == CharacterInfo::ReturnCode)
		{
			x = start_x;
			y += fontHeight;
			continue;
		}
		else if (code == CharacterInfo::TabCode)
		{
			x += space * 4;
			continue;
		}
		else if (code == CharacterInfo::SpaceCode)
		{
			x += space;
			continue;
		}

		// 文字情報を取得し、頂点データを編集
		const CharacterInfo& info = characterInfos.at(code);

		float positionX = x + info.xoffset;// + 0.5f;
		float positionY = y + info.yoffset;// + 0.5f;

		// 0---1
		// |   |
		// 2---3
		currentVertex[0].position.x = positionX;
		currentVertex[0].position.y = positionY;
		currentVertex[0].position.z = 0.0f;
		currentVertex[0].texcoord.x = info.left;
		currentVertex[0].texcoord.y = info.top;
		currentVertex[0].color.x = 1.0f;
		currentVertex[0].color.y = 1.0f;
		currentVertex[0].color.z = 1.0f;
		currentVertex[0].color.w = 1.0f;

		currentVertex[1].position.x = positionX + info.width;
		currentVertex[1].position.y = positionY;
		currentVertex[1].position.z = 0.0f;
		currentVertex[1].texcoord.x = info.right;
		currentVertex[1].texcoord.y = info.top;
		currentVertex[1].color.x = 1.0f;
		currentVertex[1].color.y = 1.0f;
		currentVertex[1].color.z = 1.0f;
		currentVertex[1].color.w = 1.0f;

		currentVertex[2].position.x = positionX;
		currentVertex[2].position.y = positionY + info.height;
		currentVertex[2].position.z = 0.0f;
		currentVertex[2].texcoord.x = info.left;
		currentVertex[2].texcoord.y = info.bottom;
		currentVertex[2].color.x = 1.0f;
		currentVertex[2].color.y = 1.0f;
		currentVertex[2].color.z = 1.0f;
		currentVertex[2].color.w = 1.0f;

		currentVertex[3].position.x = positionX + info.width;
		currentVertex[3].position.y = positionY + info.height;
		currentVertex[3].position.z = 0.0f;
		currentVertex[3].texcoord.x = info.right;
		currentVertex[3].texcoord.y = info.bottom;
		currentVertex[3].color.x = 1.0f;
		currentVertex[3].color.y = 1.0f;
		currentVertex[3].color.z = 1.0f;
		currentVertex[3].color.w = 1.0f;

		// NDC座標変換
		for (int j = 0; j < 4; ++j)
		{
			currentVertex[j].position.x = 2.0f * currentVertex[j].position.x / screenWidth - 1.0f;
			currentVertex[j].position.y = 1.0f - 2.0f * currentVertex[j].position.y / screenHeight;
		}
		currentVertex += 4;

		x += info.xadvance;

		// テクスチャが切り替わる度に描画する情報を設定
		if (currentPage != info.page)
		{
			currentPage = info.page;

			Subset subset;
			subset.shaderResourceView = shaderResourceViews.at(info.page).Get();
			subset.startIndex = currentIndexCount;
			subset.indexCount = 0;
			subsets.emplace_back(subset);
		}
		currentIndexCount += 6;
	}
}

void Font::End(ID3D11DeviceContext* context)
{
	// 頂点編集終了
	context->Unmap(vertexBuffer.Get(), 0);

	// サブセット調整
	size_t size = subsets.size();
	for (size_t i = 1; i < size; ++i)
	{
		Subset& prev = subsets.at(i - 1);
		Subset& next = subsets.at(i);
		prev.indexCount = next.startIndex - prev.startIndex;
	}
	Subset& last = subsets.back();
	last.indexCount = currentIndexCount - last.startIndex;

	// シェーダー設定
	context->VSSetShader(vertexShader.Get(), nullptr, 0);
	context->PSSetShader(pixelShader.Get(), nullptr, 0);
	context->IASetInputLayout(inputLayout.Get());

	// レンダーステート設定
	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	context->OMSetBlendState(blendState.Get(), blend_factor, 0xFFFFFFFF);
	context->OMSetDepthStencilState(depthStencilState.Get(), 0);
	context->RSSetState(rasterizerState.Get());
	context->PSSetSamplers(0, 1, samplerState.GetAddressOf());

	// 頂点バッファ設定
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->PSSetSamplers(0, 1, samplerState.GetAddressOf());

	// 描画
	for (size_t i = 0; i < size; ++i)
	{
		Subset& subset = subsets.at(i);

		context->PSSetShaderResources(0, 1, &subset.shaderResourceView);
		context->DrawIndexed(subset.indexCount, subset.startIndex, 0);
	}
}
