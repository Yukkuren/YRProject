#include "framework.h"
#include<dxgi.h>
#include <d3d11.h>
#include <memory>
#include"Geometric_primitive.h"
#include"static_mesh.h"
#include "collision.h"
#include "YRSound.h"
#include "Effect.h"

//この変数で表示、非表示を切り替える
static bool use_ImGui = false;
static bool use_debug = false;

bool Get_Use_ImGui()
{
#ifdef EXIST_IMGUI
	return use_ImGui;
#endif // !EXIST_IMGUI
	return false;
}

bool Get_Debug_Draw()
{
#ifdef EXIST_IMGUI
	return use_debug;
#endif // !EXIST_IMGUI
	return false;
}


void BlendCreate(ID3D11Device *device, ID3D11BlendState **blendstate)
{
	HRESULT hr = S_OK;

	D3D11_BLEND_DESC blend_desc;

	//無効
	blend_desc.AlphaToCoverageEnable = FALSE;
	blend_desc.IndependentBlendEnable = FALSE;
	blend_desc.RenderTarget[0].BlendEnable = FALSE;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = device->CreateBlendState(&blend_desc, &blendstate[Blend::NONE]);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//アルファ
	blend_desc.AlphaToCoverageEnable = FALSE;
	blend_desc.IndependentBlendEnable = FALSE;
	blend_desc.RenderTarget[0].BlendEnable = TRUE;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = device->CreateBlendState(&blend_desc, &blendstate[Blend::ALPHA]);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//加算
	blend_desc.AlphaToCoverageEnable = FALSE;
	blend_desc.IndependentBlendEnable = FALSE;
	blend_desc.RenderTarget[0].BlendEnable = TRUE;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = device->CreateBlendState(&blend_desc, &blendstate[Blend::ADD]);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//減算
	blend_desc.AlphaToCoverageEnable = FALSE;
	blend_desc.IndependentBlendEnable = FALSE;
	blend_desc.RenderTarget[0].BlendEnable = TRUE;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = device->CreateBlendState(&blend_desc, &blendstate[Blend::SUBTRACT]);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//
	blend_desc.AlphaToCoverageEnable = FALSE;
	blend_desc.IndependentBlendEnable = FALSE;
	blend_desc.RenderTarget[0].BlendEnable = FALSE;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = device->CreateBlendState(&blend_desc, &blendstate[Blend::REPLACE]);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	//乗算
	blend_desc.AlphaToCoverageEnable = FALSE;
	blend_desc.IndependentBlendEnable = FALSE;
	blend_desc.RenderTarget[0].BlendEnable = FALSE;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = device->CreateBlendState(&blend_desc, &blendstate[Blend::MULTIPLY]);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	blend_desc.AlphaToCoverageEnable = FALSE;
	blend_desc.IndependentBlendEnable = FALSE;
	blend_desc.RenderTarget[0].BlendEnable = FALSE;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = device->CreateBlendState(&blend_desc, &blendstate[6]);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	blend_desc.AlphaToCoverageEnable = FALSE;
	blend_desc.IndependentBlendEnable = FALSE;
	blend_desc.RenderTarget[0].BlendEnable = FALSE;
	blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = device->CreateBlendState(&blend_desc, &blendstate[7]);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}


bool framework::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rd;
	for (int i = 0; i < Rasterizer_Size; i++)
	{
		switch (i)
		{
		case RS_CULL_BACK:
			ZeroMemory(&rd, sizeof(rd));
			rd.FillMode = D3D11_FILL_SOLID;
			rd.CullMode = D3D11_CULL_BACK;
			rd.FrontCounterClockwise = FALSE;
			rd.DepthBias = 0;
			rd.DepthBiasClamp = 0;
			rd.SlopeScaledDepthBias = 0;
			rd.DepthClipEnable = TRUE;
			rd.ScissorEnable = FALSE;
			rd.MultisampleEnable = FALSE;
			rd.AntialiasedLineEnable = FALSE;
			break;
		case RS_CULL_FRONT:
			ZeroMemory(&rd, sizeof(rd));
			rd.FillMode = D3D11_FILL_SOLID;
			rd.CullMode = D3D11_CULL_FRONT;
			rd.FrontCounterClockwise = FALSE;
			rd.DepthBias = 0;
			rd.DepthBiasClamp = 0;
			rd.SlopeScaledDepthBias = 0;
			rd.DepthClipEnable = TRUE;
			rd.ScissorEnable = FALSE;
			rd.MultisampleEnable = FALSE;
			rd.AntialiasedLineEnable = FALSE;
			break;
		case RS_WIRE:
			ZeroMemory(&rd, sizeof(rd));
			rd.FillMode = D3D11_FILL_WIREFRAME;
			rd.CullMode = D3D11_CULL_BACK;
			rd.FrontCounterClockwise = FALSE;
			rd.DepthBias = 0;
			rd.DepthBiasClamp = 0;
			rd.SlopeScaledDepthBias = 0;
			rd.DepthClipEnable = TRUE;
			rd.ScissorEnable = FALSE;
			rd.MultisampleEnable = FALSE;
			rd.AntialiasedLineEnable = FALSE;
			break;
		case RS_NONE:
			ZeroMemory(&rd, sizeof(rd));
			rd.FillMode = D3D11_FILL_SOLID;
			rd.CullMode = D3D11_CULL_NONE;
			rd.FrontCounterClockwise = FALSE;
			rd.DepthBias = 0;
			rd.DepthBiasClamp = 0;
			rd.SlopeScaledDepthBias = 0;
			rd.DepthClipEnable = TRUE;
			rd.ScissorEnable = FALSE;
			rd.MultisampleEnable = FALSE;
			rd.AntialiasedLineEnable = FALSE;
			break;
		default:
			break;
		}

		HRESULT hr = device->CreateRasterizerState(&rd, rasterizer_state[i].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}
	return true;
}

bool framework::CreateDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC dd;

	for (int i = 0; i < Depth_Size; i++)
	{
		switch (i)
		{
		case DS_FALSE:
			ZeroMemory(&dd, sizeof(dd));
			dd.DepthEnable = FALSE;
			dd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dd.DepthFunc = D3D11_COMPARISON_LESS;
			dd.StencilEnable = TRUE;
			dd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			dd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
			dd.FrontFace.StencilFunc = dd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			dd.FrontFace.StencilDepthFailOp = dd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			dd.FrontFace.StencilPassOp = dd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			dd.FrontFace.StencilFailOp = dd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			break;
		case DS_TRUE:
			ZeroMemory(&dd, sizeof(dd));
			dd.DepthEnable = TRUE;
			dd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dd.DepthFunc = D3D11_COMPARISON_LESS;
			dd.StencilEnable = TRUE;
			dd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			dd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
			dd.FrontFace.StencilFunc = dd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			dd.FrontFace.StencilDepthFailOp = dd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			dd.FrontFace.StencilPassOp = dd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			dd.FrontFace.StencilFailOp = dd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			break;
		case DS_WRITE_FALSE:
			ZeroMemory(&dd, sizeof(dd));
			dd.DepthEnable = TRUE;
			dd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
			dd.DepthFunc = D3D11_COMPARISON_LESS;
			dd.StencilEnable = TRUE;
			dd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			dd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
			dd.FrontFace.StencilFunc = dd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			dd.FrontFace.StencilDepthFailOp = dd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			dd.FrontFace.StencilPassOp = dd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			dd.FrontFace.StencilFailOp = dd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			break;
		default:
			break;

			HRESULT hr = device->CreateDepthStencilState(&dd, depthstencil_state[i].GetAddressOf());
			_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		}
	}
	return true;
}


void framework::Clear(DWORD color)
{
	// デフォルトレンダーターゲットビュー設定
	context->OMSetRenderTargets(1, view.GetAddressOf(), depth.Get());

	float clearColor[4];
	for (int i = 3; i >= 0; i--) {
		clearColor[i] = ((color >> 8 * (3 - i)) & 0x000000FF) / 255.0f;
	}
	context->ClearRenderTargetView(view.Get(), clearColor);
	context->ClearDepthStencilView(depth.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetDepthStencilState(depthstencil_state[DS_TRUE].Get(), 1);

}

bool framework::initialize()
{
	//D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
	//D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;

	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	if (width < SCREEN_WIDTH)
	{
		width = SCREEN_WIDTH;
	}

	if (height < SCREEN_HEIGHT)
	{
		height = SCREEN_HEIGHT;
	}

	UINT createDeviceFlags = 0;

#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG


	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_UNKNOWN,
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);


	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};
	D3D_FEATURE_LEVEL feature_level;
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	for (D3D_DRIVER_TYPE drivertype : driverTypes)
	{
		hr = D3D11CreateDevice(
			nullptr,
			drivertype,
			nullptr,
			createDeviceFlags,
			featureLevels,
			numFeatureLevels,
			D3D11_SDK_VERSION,
			device.GetAddressOf(),
			&feature_level,
			context.GetAddressOf()
		);
		if (hr == S_OK)
			break;
	}
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	BOOL enable_4x_msaa = TRUE;

	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	//ZeroMemory(&sd, sizeof(sd));
	swap_chain_desc.BufferDesc.Width = width;
	swap_chain_desc.BufferDesc.Height = height;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.BufferDesc.ScanlineOrdering= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	UINT msaa_quality_level;
	hr=device.Get()->CheckMultisampleQualityLevels(swap_chain_desc.BufferDesc.Format, 4, &msaa_quality_level);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.OutputWindow = hwnd;
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.SampleDesc.Count = enable_4x_msaa ? 4 : 1;
	swap_chain_desc.SampleDesc.Quality = enable_4x_msaa ? msaa_quality_level - 1 : 0;
	//ここを変えればフルスクリーン
#ifdef _DEBUG
	swap_chain_desc.Windowed = TRUE;
#else
	swap_chain_desc.Windowed = FALSE;
#endif // _DEBUG

	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_desc.Flags = 0;


	/*for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		g_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &swap_chain_desc, &chain, &device, &g_featureLevel, &context);
		if (SUCCEEDED(hr))
			break;
	}*/
	Microsoft::WRL::ComPtr<IDXGIFactory1> dxgi_factory;
	Microsoft::WRL::ComPtr<IDXGIDevice> dxgi_device;

	hr = device.Get()->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(dxgi_device.GetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
	hr = dxgi_device->GetAdapter(adapter.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(dxgi_factory.GetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	hr = dxgi_factory->CreateSwapChain(device.Get(), &swap_chain_desc, chain.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	if (FAILED(hr))
		return false;


	YRBlend.Init();
	BlendCreate(device.Get(), YRBlend.blendstate);


	D3D11_TEXTURE2D_DESC back_buffer_desc;

	ID3D11Texture2D* pBackBuffer = NULL;
	hr = chain.Get()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return false;

	hr = device.Get()->CreateRenderTargetView(pBackBuffer, NULL, view.GetAddressOf());
	pBackBuffer->Release();
	if (FAILED(hr))
		return false;

	pBackBuffer->GetDesc(&back_buffer_desc);

	//context->OMSetRenderTargets(1, &view, NULL);

	ID3D11Texture2D *depth_stencil_buffer;

	D3D11_TEXTURE2D_DESC depth_stencil_buffer_desc = back_buffer_desc;
	depth_stencil_buffer_desc.MipLevels = 1;
	depth_stencil_buffer_desc.ArraySize = 1;
	depth_stencil_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_stencil_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_stencil_buffer_desc.CPUAccessFlags = 0;
	depth_stencil_buffer_desc.MiscFlags = 0;
	hr = device.Get()->CreateTexture2D(&depth_stencil_buffer_desc, NULL, &depth_stencil_buffer);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
	depth_stencil_view_desc.Format = depth_stencil_buffer_desc.Format;
	depth_stencil_view_desc.ViewDimension= enable_4x_msaa ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
	depth_stencil_view_desc.Flags = 0;
	depth_stencil_view_desc.Texture2D.MipSlice = 0;
	hr = device.Get()->CreateDepthStencilView(depth_stencil_buffer, &depth_stencil_view_desc, &depth);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));


	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context.Get()->RSSetViewports(1, &vp);

#ifdef EXIST_IMGUI

	//ImGui初期設定
	ImGui_ImplDX11_Init(device.Get(), context.Get());

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	io.Fonts->AddFontFromFileTTF("./Data/Font/mplus-1p-regular.ttf", 20.0f, NULL,
		io.Fonts->GetGlyphRangesJapanese());

	ImGui::StyleColorsDark();

#endif



	//cube = new geometric_primitive(device);
	//obj = new Static_mesh(device, L"./Mr.Incredible/Mr.Incredible.obj");
	//skin = std::make_unique< Skinned_mesh>(device.Get(), "./FBX/dummy_data.fbx");
	/*for (int i = 0; i < 10; i++)
	{
		skin[i] = std::make_unique< Skinned_mesh>(device.Get(), "./FBX/danbo_fbx/danbo_taiki.fbx");
	}*/
	//skin2 = std::make_unique< Skinned_mesh>(device.Get(), "./FBX/Pikachu/PikachuM.fbx");
	//skin2 = std::make_unique< Skinned_mesh>(device.Get(), "./FBX/danbo_fbx/danbo_taiki.fbx");
	//skin2 = std::make_unique< Skinned_mesh>(device.Get(), "./FBX/danbo_fbx/danbo_taiki.fbx");
	//skin = std::make_unique< Skinned_mesh>(device.Get(), "./FBX/danbo_fbx/danbo_atk.fbx");
	//ground = std::make_unique<Skinned_mesh>(device.Get(), "./FBX/002_cube.fbx");
	//motion.MeshSet(skin2);
	//motion.AnimReset();
	//motion2.MeshSet(skin2);
	//motion2.AnimReset();
	//skin = std::make_unique< Skinned_mesh>(device.Get(), "./FBX/anime.fbx");
	//skin = std::make_unique< Skinned_mesh>(device.Get(), "./FBX/005_cube.fbx");

	//const wchar_t wc[]= L"player-sprite.png";

	/*for (auto &p : sprite)
	{
		p = new Sprite(device, L"player-sprites.png");
	}*/
	//sprite[0] = new Sprite(device, L"logos.jpg");
	//sprite[1] = new Sprite(device, L"n64.png");


	//シェーダー

	/*sprite[0] = new Sprite(device, L"Assets/Image/ECC.png");
	sprite[1] = new Sprite(device, L"Assets/Image/fire.png");
	sprite[2] = new Sprite(device, L"Assets/Image/fire2.png");*/

	//サウンド初期化
	GetSound().Init();

	//エフェクト初期化
	YRGetEffect().Init();

	SetScene(SCENE_TABLE::SCENE_TITLE);
	scene->Init();

	//カメラ初期設定
	YRCamera.SetEye(DirectX::XMFLOAT3(10, 5, -25));			//視点
	YRCamera.SetFocus(DirectX::XMFLOAT3(0, 0, 0));			//注視点
	YRCamera.SetUp(DirectX::XMFLOAT3(0, 1, 0));				//上方向
	YRCamera.SetPerspective(30 * 0.01745f, 1280.0f / 720.0f, 0.0001f, 1000000);

	depth_stencil_buffer->Release();

	if (fade_img == nullptr)
	{
		fade_img = std::make_unique<Sprite>(L"./Data/Shader/white_bourd.png", 1920.0f, 1080.0f);
	}

	if (font == nullptr)
	{
		//font = std::make_unique<Font>(device.Get(), "Data/Font/MS Gothic/MS Gothic.fnt", 1024);
		font = std::make_unique<Font>(device.Get(), "Data/Font/03_SmartFont/03_SmartFont.fnt", 1024);
	}

	pColSprite.SetSprite();

	CreateRasterizerState();
	CreateDepthStencilState();

	return true;
}
void framework::update(float elapsed_time/*Elapsed seconds from last frame*/)
{
#ifdef EXIST_IMGUI

	if (pKeyState.spaceflg == 1 && pKeyState.lshiftflg > 0)
	{
		use_ImGui = !use_ImGui;
	}

	if (pKeyState.bflg == 1 && pKeyState.lshiftflg > 0)
	{
		use_debug = !use_debug;
	}

#endif // EXIST_IMGUI

	YRCamera.Active();
	pMouse.Update();
	GetSound().Update();
	scene->Update(elapsed_time);
}
void framework::render(float elapsed_time/*Elapsed seconds from last frame*/)
{
	context->ClearState();

	SetViewPort((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT);

	HRESULT hr = S_OK;

	UINT m4xMsaaQuality;
	device.Get()->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
	//assert(m4xMsaaQuality > 0);



	FLOAT ClearColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //red,green,blue,alpha
	context.Get()->ClearRenderTargetView(view.Get(), ClearColor);
	context.Get()->ClearDepthStencilView(depth.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//合成設定
	context.Get()->OMSetBlendState(YRBlend.blendstate[Blend::NONE],NULL, 0xffffffff);
	context.Get()->OMSetRenderTargets(1, view.GetAddressOf(), depth.Get());

	BlendSet(Blend::ALPHA);
	scene->Draw(elapsed_time);
	GetSound().SEFinCheack();

	GetSound().SoundDebugDrow();

#ifdef EXIST_IMGUI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
	//0:可変フレーム。1:60FPS固定。2:30FPS固定
#ifdef _DEBUG
	chain.Get()->Present(1, 0);
#else
	chain.Get()->Present(1, 0);
#endif // _DEBUG
}

void framework::SetViewPort(float width, float height)
{
	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context.Get()->RSSetViewports(1, &vp);
}

bool framework::CreateConstantBuffer(ID3D11Buffer** pCB, u_int size)
{
	D3D11_BUFFER_DESC buffer_desc{};
	ZeroMemory(&buffer_desc, sizeof(D3D11_BUFFER_DESC));
	buffer_desc.ByteWidth = size;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	HRESULT hr = FRAMEWORK.device->CreateBuffer(&buffer_desc, nullptr, pCB);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	return true;
}

