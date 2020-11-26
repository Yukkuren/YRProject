#include "Scene.h"
#include "framework.h"
#include "Blur.h"


void SceneTest::Init()
{
	timer = 0.0f;

	FRAMEWORK.CreateConstantBuffer(constantBuffer.GetAddressOf(), sizeof(CB_Multi_Render_Target));
	FRAMEWORK.CreateConstantBuffer(constantBuffer_Gauss.GetAddressOf(), sizeof(GaussParamManager::GaussBlurParam));

	//シェーダー作成
	if (spriteShader == nullptr)
	{
		spriteShader = std::make_unique<YRShader>(ShaderType::SPRITE);
		spriteShader->Create("./Data/Shader/sprite_vs.cso", "./Data/Shader/sprite_ps.cso");
	}
	if (skinShader == nullptr)
	{
		skinShader = std::make_unique<YRShader>(ShaderType::SKIN);
		skinShader->Create("./Data/Shader/Skinned_VS.cso", "./Data/Shader/Skinned_PS.cso");
	}
	if (geoShader == nullptr)
	{
		geoShader = std::make_unique<YRShader>(ShaderType::GEO);
		geoShader->Create("./Data/Shader/geometric_primitive_vs.cso", "./Data/Shader/geometric_primitive_ps.cso");
	}
	if (boardShader == nullptr)
	{
		boardShader = std::make_unique<YRShader>(ShaderType::BOARD);
		boardShader->Create("./Data/Shader/board_vs.cso", "./Data/Shader/board_ps.cso");
	}
	if (animShader == nullptr)
	{
		animShader = std::make_unique<YRShader>(ShaderType::ANIM);
		animShader->Create("./Data/Shader/AnimShader_vs.cso", "./Data/Shader/AnimShader_ps.cso", "./Data/Shader/AnimShader_gs.cso");
	}
	if (toonShader == nullptr)
	{
		toonShader = std::make_unique<YRShader>(ShaderType::TOON);
		toonShader->Create("./Data/Shader/ToonShader_vs.cso", "./Data/Shader/ToonShader_ps.cso", "./Data/Shader/ToonShader_gs.cso");
	}
	if (paralleltoonShader == nullptr)
	{
		paralleltoonShader = std::make_unique<YRShader>(ShaderType::TOON);
		paralleltoonShader->Create("./Data/Shader/ParallelToon_vs.cso", "./Data/Shader/ParallelToon_ps.cso", "./Data/Shader/ParallelToon_gs.cso");
	}
	if (toGbuffer == nullptr)
	{
		toGbuffer = std::make_unique<YRShader>(ShaderType::TOGBUF);
		toGbuffer->Create("./Data/Shader/toGbuffer_vs.cso", "./Data/Shader/toGbuffer_ps.cso");
	}
	if (spriteEx == nullptr)
	{
		spriteEx = std::make_unique<YRShader>(ShaderType::SPRITE_EX);
		spriteEx->Create("./Data/Shader/SpriteEx_vs.cso", "./Data/Shader/SpriteEx_ps.cso");
	}
	if (flatShader == nullptr)
	{
		flatShader = std::make_unique<YRShader>(ShaderType::FLAT);
		flatShader->Create("./Data/Shader/flatShader_vs.cso", "./Data/Shader/flatShader_ps.cso", "./Data/Shader/flatShader_gs.cso");
	}
	if (gaussShader == nullptr)
	{
		gaussShader = std::make_unique<YRShader>(ShaderType::GAUSS);
		gaussShader->Create("./Data/Shader/GaussShader_vs.cso", "./Data/Shader/GaussShader_ps.cso");
	}
	if (multi_gaussShader == nullptr)
	{
		multi_gaussShader = std::make_unique<YRShader>(ShaderType::MULTI_GAUSS);
		multi_gaussShader->Create("./Data/Shader/MultiGaussShader_vs.cso", "./Data/Shader/MultiGaussShader_ps.cso");
	}
	if (furShader == nullptr)
	{
		furShader = std::make_unique<YRShader>(ShaderType::FUR);
		furShader->Create("./Data/Shader/furShader_vs.cso", "./Data/Shader/furShader_ps.cso", "./Data/Shader/furShader_gs.cso");
	}
	if (skyShader == nullptr)
	{
		skyShader = std::make_unique<YRShader>(ShaderType::SKY);
		skyShader->Create("./Data/Shader/SkyMapShader_vs.cso", "./Data/Shader/SkyMapShader_ps.cso");
	}

	//カメラ初期設定
	YRCamera.SetEye(DirectX::XMFLOAT3(0.0f, 5.0f, -25));			//視点
	YRCamera.SetFocus(DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f));			//注視点
	YRCamera.SetUp(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));				//上方向
	YRCamera.SetPerspective(30 * 0.01745f, 1920.0f / 1080.0f, 1.4f, 1000.0f);

	if (box_texture == nullptr)
	{
		box_texture = std::make_shared<Texture>(L"./Data/FBX/danbo_fbx/texture/danbo_face_c2.png");
	}
	if (board_texture == nullptr)
	{
		board_texture = std::make_shared<Texture>(L"./Data/Image/UI/GameScene/effect.png");
	}

	if (knight == nullptr)
	{
		//box = std::make_unique<Skinned_mesh>("./Data/FBX/Knight.fbx");
		knight = std::make_shared<Model>("./Data/FBX/Knight/knight_main.fbx");
		//knight = std::make_shared<Model>("./Data/FBX/Paimon/paimon.fbx");
	}
	if (wait_R == nullptr)
	{
		wait_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_wait_R.fbx");
	}
	if (geo == nullptr)
	{
		geo = std::make_unique<geometric_primitive>();
	}
	if (board == nullptr)
	{
		board = std::make_unique<board_primitive>(board_texture);
	}
	if (anim == nullptr)
	{
		anim = std::make_unique<AnimBoard>(board_texture, 6,XMFLOAT2(64.0f,64.0f),XMINT2(3,2),XMFLOAT2(192.0f,128.0f));
	}

	if (sky == nullptr)
	{
		sky = std::make_unique<Skinned_mesh>("./Data/FBX/SKY/skyblock_re.fbx");
	}

	//motion.MeshSet(box);
	//motion.AnimReset();
	if (motion == nullptr)
	{
		motion = std::make_unique<ModelAnim>(knight);
		motion->PlayAnimation(0, true);
		motion->NodeChange(wait_R);
	}

	//画像のロード
	if (test == nullptr)
	{
		//test = std::make_unique<Sprite>(L"./Data/Image/BG/stage1.png", 3840.0f, 2160.0f);
	}
	if (cutFrame == nullptr)
	{
		cutFrame = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/CutIn_Frame.png", 1920.0f, 1440.0f, 3, 4, 640.0f, 360.0f, 10);
	}
	if (cutMask == nullptr)
	{
		cutMask = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/CutIn_Mask.png", 1920.0f, 1440.0f, 3, 4, 640.0f, 360.0f, 10);
	}
	if (cutIn == nullptr)
	{
		cutIn = std::make_unique<Sprite>(L"./Data/Image/Character/Knight/Knight_cut.png", 1280.0f, 720.0f);
	}


	if (fur == nullptr)
	{
		fur = std::make_unique<Texture>(L"./Data/Assets/Slime/fur.png");
	}

	//テクスチャロード
	if (color_texture == nullptr)
	{
		color_texture = std::make_unique<Texture>();
		color_texture->Create(1920, 1080, DXGI_FORMAT_R8G8B8A8_UNORM);
		color_texture->CreateDepth(1920, 1080, DXGI_FORMAT_R24G8_TYPELESS);
	}
	if (normal_texture == nullptr)
	{
		normal_texture = std::make_unique<Texture>();
		normal_texture->Create(1920, 1080, DXGI_FORMAT_R16G16B16A16_FLOAT);
		normal_texture->CreateDepth(1920, 1080, DXGI_FORMAT_R24G8_TYPELESS);
	}
	if (position_texture == nullptr)
	{
		position_texture = std::make_unique<Texture>();
		position_texture->Create(1920, 1080, DXGI_FORMAT_R16G16B16A16_FLOAT);
		position_texture->CreateDepth(1920, 1080, DXGI_FORMAT_R24G8_TYPELESS);
	}
	if (luminance_texture == nullptr)
	{
		luminance_texture = std::make_unique<Texture>();
		luminance_texture->Create(1920, 1080, DXGI_FORMAT_R16G16B16A16_FLOAT);
		luminance_texture->CreateDepth(1920, 1080, DXGI_FORMAT_R24G8_TYPELESS);
	}

	float riv = 1.0f;
	if (blur_texture[0] == nullptr)
	{
		for (int i = 0; i < blur_texture.size(); i++)
		{
			if (i % 2 == 0)
			{
				riv = i;
			}
			if (i == 0)
			{
				riv = 1.0f;
			}

			blur_texture[i] = std::make_unique<Texture>();
			blur_texture[i]->Create(960.0f / riv, 540.0f / riv, DXGI_FORMAT_R16G16B16A16_FLOAT);
			blur_texture[i]->CreateDepth(960.0f / riv, 540.0f / riv, DXGI_FORMAT_R24G8_TYPELESS);
		}		
	}

	if (multi_blur_texture == nullptr)
	{
		multi_blur_texture = std::make_unique<Texture>();
		multi_blur_texture->Create(1920, 1080, DXGI_FORMAT_R16G16B16A16_FLOAT);
		multi_blur_texture->CreateDepth(1920, 1080, DXGI_FORMAT_R24G8_TYPELESS);
	}

	if (bisuko_normal_texture == nullptr)
	{
		bisuko_normal_texture = std::make_shared<Texture>(L"Data/ASSETS/nbisuko.jpg");
	}
	if (bisuko_specular_texture == nullptr)
	{
		bisuko_specular_texture = std::make_shared<Texture>(L"Data/ASSETS/sbisuko2.jpg");
	}
	if (test_texture == nullptr)
	{
		test_texture = std::make_shared<Texture>(L"Data/ASSETS/Test.png");
	}
	if (test_normal_texture == nullptr)
	{
		test_normal_texture = std::make_shared<Texture>(L"Data/ASSETS/Test_normal2.png");
	}
	if (scorpion_specular_texture == nullptr)
	{
		scorpion_specular_texture = std::make_shared<Texture>(L"Data/ASSETS/Sdaiou.png");
	}
	if (specular_texture == nullptr)
	{
		specular_texture = std::make_shared<Texture>();
		specular_texture->Create(1280, 720, DXGI_FORMAT_R8G8B8A8_UNORM);
	}


	if (bisuko == nullptr)
	{
		bisuko = std::make_unique<board_primitive>(L"Data/ASSETS/bisuko.jpg");
	}
	if (cube == nullptr)
	{
		cube = std::make_unique<board_primitive>(test_texture);
	}
	if (plane == nullptr)
	{
		plane = std::make_unique<board_primitive>(test_texture);
	}


	//サンプラー生成
	if (sampler_wrap == nullptr)
	{
		sampler_wrap = std::make_shared<Sampler>(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	}
	if (sampler_clamp == nullptr)
	{
		sampler_clamp = std::make_shared<Sampler>(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP);
	}

	//Gbuffer用スプライト
	if (sprite == nullptr)
	{
		sprite = std::make_unique<Sprite>();
	}

	circle.SetCircle(100.0f);

	cut_in = false;
	cut_timer = 0.0f;
	timer_start = false;
	cut_fin = false;
}

void SceneTest::Update(float elapsed_time)
{

	if (pKeyState.cflg == 1)
	{
		if (!cut_in)
		{
			cut_in = true;
		}
	}

	if (timer_start)
	{
		cut_timer += elapsed_time;
	}

	if (cut_timer > 0.8f)
	{
		cut_timer = 0.0f;
		timer_start = false;
		cut_fin = true;
	}
}

void SceneTest::Draw(float elapsed_time)
{
	static float light_color[4] = { 1,1,1,1 };
	//ライト方向
	static DirectX::XMFLOAT4 light_direction = DirectX::XMFLOAT4(0, -1, 1, 0);
	static DirectX::XMFLOAT4 ambient_color(0.3f, 0.3f, 0.3f, 0.5f);
	//static DirectX::XMFLOAT3 box_angle = { DirectX::XMConvertToRadians(-90.0f),0.0f,0.0f };

#ifdef EXIST_IMGUI
	//ImGui
	if(Get_Use_ImGui())
	{
		DirectX::XMFLOAT3	eye = YRCamera.GetEye();
		DirectX::XMFLOAT3	focus = YRCamera.GetFocus();
		float				fov = YRCamera.GetFov();
		fov /= 0.01745f;
		//ImGui::Text("anim : %f", motion.anim_timer);
		ImGui::Text("SceneTest");
		timer += elapsed_time;
		ImGui::Text("time : %f", timer);
		//ImGui::InputFloat("scroll", &scall, 0.01f, 100.0f);
		//ImGui::SliderFloat("rollX", &roll.x, 0.0f, 30.00f);
		//ImGui::SliderFloat("rollY", &roll.y, 0.0f, 30.00f);
		//ImGui::SliderFloat("rollZ", &roll.z, 0.0f, 30.00f);
		//ImGui::InputFloat("transX", &trans.x, 1.0f, 30.00f);
		//ImGui::InputFloat("transY", &trans.y, 1.0f, 30.00f);
		//ImGui::InputFloat("transZ", &trans.z, 1.0f, 30.00f);
		if (ImGui::TreeNode("light"))
		{
			ImGui::ColorEdit4("light_color", light_color);
			ImGui::InputFloat("light_direction.x", &light_direction.x, 0.1f, 0.1f);
			ImGui::InputFloat("light_direction.y", &light_direction.y, 0.1f, 0.1f);
			ImGui::InputFloat("light_direction.z", &light_direction.z, 0.1f, 0.1f);
			ImGui::InputFloat("light_direction.w", &light_direction.w, 0.1f, 0.1f);
			ImGui::InputFloat("ambient_color.x", &ambient_color.x, 0.01f, 0.01f);
			ImGui::InputFloat("ambient_color.y", &ambient_color.y, 0.01f, 0.01f);
			ImGui::InputFloat("ambient_color.z", &ambient_color.z, 0.01f, 0.01f);
			ImGui::InputFloat("ambient_color.w", &ambient_color.w, 0.01f, 0.01f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Camera"))
		{
			ImGui::InputFloat("eye_X", &eye.x, 0.01f, 0.01f);
			ImGui::InputFloat("eye_Y", &eye.y, 0.01f, 0.01f);
			ImGui::InputFloat("eye_Z", &eye.z, 0.01f, 0.01f);
			ImGui::InputFloat("focus_X", &focus.x, 0.01f, 0.01f);
			ImGui::InputFloat("focus_Y", &focus.y, 0.01f, 0.01f);
			ImGui::InputFloat("focus_Z", &focus.z, 0.01f, 0.01f);
			ImGui::InputFloat("fov", &fov, 1.00f, 1.00f);
			ImGui::TreePop();
		}

		YRCamera.SetEye(eye);
		YRCamera.SetFocus(focus);
		YRCamera.SetPerspective(fov * 0.01745f, 1920.0f / 1080.0f, 1.4f, 1000.0f);
	}
#endif
	DirectX::XMFLOAT4 lightColor(light_color[0], light_color[1], light_color[2], light_color[3]);

	//ビュー行列
	DirectX::XMMATRIX V;
	V = DirectX::XMLoadFloat4x4(&YRCamera.GetView());
	//投資投影行列
	DirectX::XMMATRIX P;
	P = DirectX::XMLoadFloat4x4(&YRCamera.GetProjection());

	//材質カラー
	DirectX::XMFLOAT4 color = DirectX::XMFLOAT4(1, 1, 1, 1);


	//ビュー更新
	YRCamera.Active();

	//Gbufferへの描画
	RenderTexture(V, P, light_direction, lightColor, ambient_color, elapsed_time);

static bool blur = true;
#ifdef EXIST_IMGUI
	//ImGui
	if(Get_Use_ImGui())
	{
		
		//ImGui::Checkbox(u8"ブルーム", &blur);
		if (ImGui::TreeNode("texture"))
		{
			ImGui::Image((void *)(color_texture->GetShaderResource()), ImVec2(360, 360));
			ImGui::Image((void *)(normal_texture->GetShaderResource()), ImVec2(360, 360));
			ImGui::Image((void *)(position_texture->GetShaderResource()), ImVec2(360, 360));
			ImGui::Image((void *)(luminance_texture->GetShaderResource()), ImVec2(360, 360));

			ImGui::TreePop();
		}
	}
#endif

	FRAMEWORK.framebuffer.ResetRenderTexture();
	FRAMEWORK.framebuffer.SetDefaultRTV();

	//画面のクリア
	FRAMEWORK.Clear(0x8080FFFF);

	//ビューポート設定
	FRAMEWORK.SetViewPort(FRAMEWORK.SCREEN_WIDTH, FRAMEWORK.SCREEN_HEIGHT);

	//ブレンドステート設定
	FRAMEWORK.BlendSet(Blend::ALPHA);

	//ラスタライザー設定
	FRAMEWORK.context->RSGetState(FRAMEWORK.rasterizer_state[FRAMEWORK.RS_CULL_BACK].GetAddressOf());

	//デプスステンシルステート設定
	FRAMEWORK.context->OMSetDepthStencilState(FRAMEWORK.depthstencil_state[FRAMEWORK.DS_TRUE].Get(), 1);

	//定数バッファの設定
	//CB_Multi_Render_Target cb;
	//cb.light_direction = light_direction;
	//cb.light_color = lightColor;
	//cb.ambient_color = ambient_color;
	//cb.eye_pos.x = YRCamera.GetEye().x;
	//cb.eye_pos.y = YRCamera.GetEye().y;
	//cb.eye_pos.z = YRCamera.GetEye().z;
	//cb.eye_pos.w = 1.0f;

	////定数バッファ更新
	//FRAMEWORK.context->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cb, 0, 0);
	//FRAMEWORK.context->VSSetConstantBuffers(2, 1, constantBuffer.GetAddressOf());
	//FRAMEWORK.context->PSSetConstantBuffers(2, 1, constantBuffer.GetAddressOf());


	sampler_clamp->Set(0);

	//Gbuffer描画
	sprite->render(
		spriteEx.get(),
		color_texture.get(),
		0.0f, 0.0f, 1920.0f, 1080.0f,
		0.0f, 0.0f, 1920.0f, 1080.0f,0.0f,1.0f);
	/*sprite->render(
		spriteEx.get(),
		normal_texture.get(),
		640.0f, 0.0f, 640.0f, 360.0f,
		0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f);
	sprite->render(
		spriteEx.get(),
		position_texture.get(),
		0.0f, 360.0f, 640.0f, 360.0f,
		0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f);*/

	RenderBlur(V, P, light_direction, lightColor, ambient_color, elapsed_time);


#ifdef EXIST_IMGUI
	if (Get_Use_ImGui())
	{
		if (ImGui::TreeNode(u8"合成テクスチャ"))
		{
			ImGui::Image((void*)(multi_blur_texture->GetShaderResource()), ImVec2(360, 360));
			ImGui::TreePop();
		}
	}
#endif // USE_IMGUI

	/*motion->UpdateAnimation(elapsed_time);
	motion->CalculateLocalTransform();
	motion->CalculateWorldTransform(knight_pos,
		DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f),
		knight_angle);
	motion->Draw(
		flatShader.get(),
		V, P, light_direction, lightColor, ambient_color
	);*/
}


void SceneTest::RenderTexture(
	const DirectX::XMMATRIX&	view,
	const DirectX::XMMATRIX&	projection,
	const DirectX::XMFLOAT4&	light_direction,
	const DirectX::XMFLOAT4&	light_color,
	const DirectX::XMFLOAT4&	ambient_color,
	float						elapsed_time)
{
	static float Distance = 1.0f;
	static float Density = 1.0f;
	//static DirectX::XMFLOAT3 sky_scale = { 10.0f,10.0f,10.0f };
	static float sky_scale[3] = { 1.0f,1.0f,1.0f };

#ifdef EXIST_IMGUI
	if(Get_Use_ImGui())
	{
		ImGui::InputFloat("knight_pos.x", &knight_pos.x, 0.01f, 0.01f);
		ImGui::InputFloat("knight_pos.y", &knight_pos.y, 0.01f, 0.01f);
		ImGui::InputFloat("knight_pos.z", &knight_pos.z, 0.01f, 0.01f);
		ImGui::InputFloat("knight_angle.x", &knight_angle.x, 0.01f, 0.01f);
		ImGui::InputFloat("knight_angle.y", &knight_angle.y, 0.01f, 0.01f);
		ImGui::InputFloat("knight_angle.z", &knight_angle.z, 0.01f, 0.01f);
		ImGui::InputFloat(u8"毛の長さ", &Distance, 0.01f, 0.01f);
		ImGui::InputFloat(u8"毛の密度", &Density, 0.01f, 0.01f);
		ImGui::InputFloat(u8"空.x", &sky_scale[0], 0.01f, 0.01f);
		ImGui::InputFloat(u8"空.y", &sky_scale[1], 0.01f, 0.01f);
		ImGui::InputFloat(u8"空.z", &sky_scale[2], 0.01f, 0.01f);
	}
#endif // USE_IMGUI

	/*std::array<ID3D11RenderTargetView*,4> rtv = {
		color_texture->GetRenderTargetView(),
		normal_texture->GetRenderTargetView(),
		position_texture->GetRenderTargetView(),
		luminance_texture->GetRenderTargetView()
	};*/

	/*std::vector< ID3D11RenderTargetView*>testrtv;
	testrtv.push_back(color_texture->GetRenderTargetView());
	testrtv.push_back(normal_texture->GetRenderTargetView());
	testrtv.push_back(position_texture->GetRenderTargetView());
	testrtv.push_back(luminance_texture->GetRenderTargetView());*/

	FRAMEWORK.framebuffer.SetRenderTexture(color_texture->GetRenderTargetView());
	//FRAMEWORK.framebuffer.SetRenderTexture(normal_texture->GetRenderTargetView());
	//FRAMEWORK.framebuffer.SetRenderTexture(position_texture->GetRenderTargetView());
	FRAMEWORK.framebuffer.SetRenderTexture(luminance_texture->GetRenderTargetView());

	ID3D11DepthStencilView* dsv = color_texture->GetDepthStencilView();
	//FRAMEWORK.context->OMSetRenderTargets(testrtv.size(), testrtv.data(), dsv);

	FRAMEWORK.framebuffer.Clear();

	//画面クリア
	/*float clearColor[4] = { 0.0f,0.0f,0.0f,1.0f };
	for (int i = 0; i < testrtv.size(); i++)
	{
		FRAMEWORK.context->ClearRenderTargetView(testrtv[i], clearColor);
	}*/
	FRAMEWORK.context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	//ビュー更新
	YRCamera.Active();

	//ビューポート設定
	//FRAMEWORK.SetViewPort(static_cast<float>(FRAMEWORK.SCREEN_WIDTH), static_cast<float>(FRAMEWORK.SCREEN_HEIGHT));
	//FRAMEWORK.SetViewPort(1920.0f, 1080.0f);

	//レンダーターゲットビューの設定
	FRAMEWORK.framebuffer.GetDefaultRTV();
	FRAMEWORK.framebuffer.Activate(1920.0f, 1080.0f, dsv);

	//定数バッファの設定
	CB_Multi_Render_Target cb;
	cb.light_direction = light_direction;
	cb.light_color = light_color;
	cb.ambient_color = ambient_color;
	cb.eye_pos.x = YRCamera.GetEye().x;
	cb.eye_pos.y = YRCamera.GetEye().y;
	cb.eye_pos.z = YRCamera.GetEye().z;
	cb.eye_pos.w = 1.0f;
	cb.Density = Density;
	cb.Distance = Distance;

	//定数バッファ更新
	FRAMEWORK.context->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cb, 0, 0);
	FRAMEWORK.context->VSSetConstantBuffers(2, 1, constantBuffer.GetAddressOf());
	FRAMEWORK.context->PSSetConstantBuffers(2, 1, constantBuffer.GetAddressOf());
	FRAMEWORK.context->GSSetConstantBuffers(2, 1, constantBuffer.GetAddressOf());

	//ブレンドステート設定
	FRAMEWORK.BlendSet(Blend::ALPHA);
	//ラスタライザー設定
	FRAMEWORK.context->RSSetState(FRAMEWORK.rasterizer_state[framework::RS_CULL_BACK].Get());
	//デプスステンシルステート設定
	FRAMEWORK.context->OMSetDepthStencilState(FRAMEWORK.depthstencil_state[framework::DS_TRUE].Get(), 1);
	
	//サンプラー設定
	sampler_clamp->Set(0);

	//描画
	/*static  XMFLOAT3 aY;
	aY.x = 0.0f;
	aY.y += elapsed_time;
	aY.z = 0.0f;

	bisuko->render(
		toGbuffer.get(),
		XMFLOAT3(2.0f, 2.0f, 0.0f),
		XMFLOAT3(1.0f, 2.0f, 0.0f),
		aY,
		view,
		projection
	);

	plane->render(
		toGbuffer.get(),
		XMFLOAT3(0.0f, 0.0f, 0.0f),
		XMFLOAT3(10.0f, 10.0f, 1.0f),
		XMFLOAT3(-XM_PI / 2.0f, 0.0f, 0.0f),
		view,
		projection
	);*/

	//test->DrawExtendGraph(spriteShader.get(), 0.0f, 0.0f, FRAMEWORK.SCREEN_WIDTH, FRAMEWORK.SCREEN_HEIGHT, DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f));
	
	FRAMEWORK.context->OMSetDepthStencilState(FRAMEWORK.depthstencil_state[framework::DS_WRITE_FALSE].Get(), 1);
	sampler_wrap->Set(1);
	fur->Set(1);


	sky->Render(skyShader.get(),
		knight_pos,
		DirectX::XMFLOAT3(sky_scale[0], sky_scale[1], sky_scale[2]),
		DirectX::XMFLOAT3(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f),
		view,
		projection,
		light_direction,
		light_color,
		ambient_color,
		elapsed_time,
		0.0f
	);	

	motion->UpdateAnimation(elapsed_time);
	motion->CalculateLocalTransform();
	motion->CalculateWorldTransform(knight_pos,
		DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f),
		knight_angle);
	motion->Draw(
		flatShader.get(),
		view, projection, light_direction, light_color, ambient_color
	);

	float center_x = static_cast<float>(FRAMEWORK.SCREEN_WIDTH) / 2.0f;
	float center_y = static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) / 2.0f;

	/*cutFrame->DrawRotaGraph(
		spriteShader.get(),
		center_x,
		center_y,
		0.0f,
		1.0f,
		SpriteMask::NONE
	);

	cutMask->DrawRotaGraph(
		spriteShader.get(),
		center_x,
		center_y,
		0.0f,
		1.0f,
		SpriteMask::WRITE
	);*/

	float fream = 0.03f;
	float size = 3.0f;

	if (cut_in)
	{
		if (cut_fin)
		{
			bool finish_cut = cutFrame->DrawRotaDivGraphReverse(
				spriteShader.get(),
				center_x,
				center_y,
				0.0f,
				size,
				fream,
				elapsed_time,
				SpriteMask::NONE
			);

			cutMask->DrawRotaDivGraphReverse(
				spriteShader.get(),
				center_x,
				center_y,
				0.0f,
				size,
				fream,
				elapsed_time,
				SpriteMask::WRITE
			);

			if (finish_cut)
			{
				cut_in = false;
				cut_fin = false;
			}
		}
		else
		{
			bool finish_cut = cutFrame->DrawRotaDivGraphOnec(
				spriteShader.get(),
				center_x,
				center_y,
				0.0f,
				size,
				fream,
				elapsed_time,
				SpriteMask::NONE
			);

			cutMask->DrawRotaDivGraphOnec(
				spriteShader.get(),
				center_x,
				center_y,
				0.0f,
				size,
				fream,
				elapsed_time,
				SpriteMask::WRITE
			);

			if (finish_cut)
			{
				timer_start = true;
			}
		}


	}


	cutIn->DrawRotaGraph(
		spriteShader.get(),
		center_x,
		center_y,
		0.0f,
		3.0f,
		SpriteMask::INDRAW
	);

	//circle.DrawCircle(spriteShader.get(), 400.0f, 900.0f);

	/*for (int i = 1; i < testrtv.size(); i++)
	{
		testrtv[i] = NULL;
	}
	testrtv[0] = FRAMEWORK.view.Get();*/
	//レンダーターゲットの回復
	//FRAMEWORK.context.Get()->OMSetRenderTargets(testrtv.size(), testrtv.data(), FRAMEWORK.depth.Get());
	FRAMEWORK.framebuffer.Deactivate();
	FRAMEWORK.framebuffer.SetDefaultRTV();
	FRAMEWORK.framebuffer.ResetRenderTexture();
}


void SceneTest::RenderBlur(
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	float						elapsed_time)
{
	static bool blur = true;
	static float off_x = 1.0f;
	static float off_y = 1.0f;
	static float deviation = 0.5f;
#ifdef EXIST_IMGUI
	//ImGui
	if(Get_Use_ImGui())
	{
		if (ImGui::TreeNode(u8"ブルーム"))
		{
			ImGui::Checkbox(u8"ブルーム", &blur);
			ImGui::InputFloat("offset.x", &off_x, 0.01f, 0.01f);
			ImGui::InputFloat("offset.y", &off_y, 0.01f, 0.01f);
			ImGui::InputFloat("diviation.y", &deviation, 0.01f, 0.01f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode(u8"ブルームテクスチャ"))
		{
			for (int i = 0; i < blur_texture.size(); i++)
			{
				ImGui::Image((void*)(blur_texture[i]->GetShaderResource()), ImVec2(360, 360));
			}
			ImGui::TreePop();
		}
	}
#endif


	GaussParamManager gauss;
	float w = (float)FRAMEWORK.SCREEN_WIDTH;
	float h = (float)FRAMEWORK.SCREEN_HEIGHT;

	if (blur)
	{
		//初回のみ現在の描画を利用する
		//テクスチャをセット
		FRAMEWORK.framebuffer.SetRenderTexture(blur_texture[0]->GetRenderTargetView(),true);
		ID3D11DepthStencilView* dsv = blur_texture[0]->GetDepthStencilView();

		//画面をクリア
		FRAMEWORK.framebuffer.Clear();
		FRAMEWORK.context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		//レンダーターゲットビューの設定
		FRAMEWORK.framebuffer.GetDefaultRTV();
		FRAMEWORK.SetViewPort(1920.0f, 1080.0f);
		FRAMEWORK.framebuffer.Activate(dsv);

		//ブレンドステート設定
		FRAMEWORK.BlendSet(Blend::ALPHA);
		//ラスタライザー設定
		//FRAMEWORK.context->RSSetState(FRAMEWORK.rasterizer_state[framework::RS_CULL_BACK].Get());
		//デプスステンシルステート設定
		//FRAMEWORK.context->OMSetDepthStencilState(FRAMEWORK.depthstencil_state[framework::DS_TRUE].Get(), 1);

		//サンプラー設定
		sampler_clamp->Set(0);

		gauss.CalcBlurParam(w, h, YR_Vector3(off_x, 0.0f), deviation);
		FRAMEWORK.context->UpdateSubresource(constantBuffer_Gauss.Get(), 0, NULL, &gauss.param, 0, 0);
		FRAMEWORK.context->PSSetConstantBuffers(0, 1, constantBuffer_Gauss.GetAddressOf());

		sprite->render(
			gaussShader.get(),
			luminance_texture.get(),
			0.0f, 0.0f, 1920.0f*0.5f, 1080.0f*0.5f,
			0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);

		//FRAMEWORK.framebuffer.Deactivate();
		//FRAMEWORK.framebuffer.ResetRenderTexture();
		float riv2 = 1.0f;
		float riv = 1.0f;
		//特定の回数分描画し、テクスチャに書き込む
		for (int i = 1; i < blur_texture.size(); i++)
		{
			//テクスチャをセット
			FRAMEWORK.framebuffer.SetRenderTexture(blur_texture[i]->GetRenderTargetView(),true);
			//ID3D11DepthStencilView* dsv = color_texture->GetDepthStencilView();

			//画面をクリア
			FRAMEWORK.framebuffer.Clear();
			//FRAMEWORK.context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			if (i % 2)
			{
				riv = static_cast<float>(i) - 1.0f;
				if (i == 1)
				{
					riv = 1.0f;
				}
				gauss.CalcBlurParam(w * 0.5f / riv2, h * 0.5f / riv2, YR_Vector3(0.0f, off_y), deviation);
				FRAMEWORK.context->UpdateSubresource(constantBuffer_Gauss.Get(), 0, NULL, &gauss.param, 0, 0);
				FRAMEWORK.context->PSSetConstantBuffers(0, 1, constantBuffer_Gauss.GetAddressOf());
			}
			else
			{
				riv2 = static_cast<float>(i);
				gauss.CalcBlurParam(w * 0.5f / riv2, h * 0.5f / riv2, YR_Vector3(off_x, 0.0f), deviation);
				FRAMEWORK.context->UpdateSubresource(constantBuffer_Gauss.Get(), 0, NULL, &gauss.param, 0, 0);
				FRAMEWORK.context->PSSetConstantBuffers(0, 1, constantBuffer_Gauss.GetAddressOf());
			}


			//レンダーターゲットビューの設定
			FRAMEWORK.framebuffer.Activate(dsv);

			sprite->render(
				gaussShader.get(),
				blur_texture[i-1].get(),
				0.0f, 0.0f, 1920.0f * 0.5f / riv2, 1080.0f * 0.5f / riv2,
				0.0f, 0.0f, 1920.0f * 0.5f / riv, 1080.0f * 0.5f / riv, 0.0f, 1.0f);

			//FRAMEWORK.framebuffer.Deactivate();
			//FRAMEWORK.framebuffer.ResetRenderTexture();
		}

	//全てのテクスチャを合成したマルチガウスのテクスチャを作成する
		FRAMEWORK.framebuffer.Deactivate();
		FRAMEWORK.framebuffer.ResetRenderTexture();
		FRAMEWORK.framebuffer.SetDefaultRTV();
		
		FRAMEWORK.BlendSet(Blend::ADD);

		// シェーダリソースビューを設定.
		ID3D11ShaderResourceView* pSRV[] = {
			luminance_texture->GetShaderResource(),
			blur_texture[1]->GetShaderResource(),
			blur_texture[3]->GetShaderResource(),
			blur_texture[5]->GetShaderResource(),
			blur_texture[7]->GetShaderResource(),
			blur_texture[9]->GetShaderResource(),
			//blur_texture[11]->GetShaderResource(),
		};

		sprite->render(
			multi_gaussShader.get(),
			luminance_texture.get(),
			pSRV,
			6,
			0.0f, 0.0f, 1920.0f, 1080.0f,
			0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);
		

	//ブルーム画像の描画

		//画面のクリア
		//FRAMEWORK.Clear(0x8080FFFF);

		////ビューポート設定
		//FRAMEWORK.SetViewPort(FRAMEWORK.SCREEN_WIDTH, FRAMEWORK.SCREEN_HEIGHT);

		////ラスタライザー設定
		//FRAMEWORK.context->RSGetState(FRAMEWORK.rasterizer_state[FRAMEWORK.RS_CULL_BACK].GetAddressOf());

		////デプスステンシルステート設定
		//FRAMEWORK.context->OMSetDepthStencilState(FRAMEWORK.depthstencil_state[FRAMEWORK.DS_TRUE].Get(), 1);

		//レンダーターゲットの回復
		//FRAMEWORK.context.Get()->OMSetRenderTargets(1, FRAMEWORK.view.GetAddressOf(), FRAMEWORK.depth.Get());

		////ブレンドステート設定
		//FRAMEWORK.BlendSet(Blend::ADD);

		//sprite->render(
		//	spriteEx.get(),
		//	multi_blur_texture.get(),
		//	0.0f, 0.0f, 1920.0f, 1080.0f,
		//	0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);
	}
}