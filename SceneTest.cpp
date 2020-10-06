#include "Scene.h"
#include "framework.h"
#include "Blur.h"

void SceneTest::Init()
{
	timer = 0.0f;

	FRAMEWORK.CreateConstantBUffer(constantBuffer.GetAddressOf(), sizeof(CB_Multi_Render_Target));

	//シェーダー作成
	if (spriteShader == nullptr)
	{
		spriteShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::SPRITE);
		spriteShader->Create("./Data/Shader/sprite_vs.cso", "./Data/Shader/sprite_ps.cso");
	}
	if (skinShader == nullptr)
	{
		skinShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::SKIN);
		skinShader->Create("./Data/Shader/Skinned_VS.cso", "./Data/Shader/Skinned_PS.cso");
	}
	if (geoShader == nullptr)
	{
		geoShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::GEO);
		geoShader->Create("./Data/Shader/geometric_primitive_vs.cso", "./Data/Shader/geometric_primitive_ps.cso");
	}
	if (boardShader == nullptr)
	{
		boardShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::BOARD);
		boardShader->Create("./Data/Shader/board_vs.cso", "./Data/Shader/board_ps.cso");
	}
	if (animShader == nullptr)
	{
		animShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::ANIM);
		animShader->Create("./Data/Shader/AnimShader_vs.cso", "./Data/Shader/AnimShader_ps.cso", "./Data/Shader/AnimShader_gs.cso");
	}
	if (toonShader == nullptr)
	{
		toonShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::TOON);
		toonShader->Create("./Data/Shader/ToonShader_vs.cso", "./Data/Shader/ToonShader_ps.cso", "./Data/Shader/ToonShader_gs.cso");
	}
	if (toGbuffer == nullptr)
	{
		toGbuffer = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::TOGBUF);
		toGbuffer->Create("./Data/Shader/toGbuffer_vs.cso", "./Data/Shader/toGbuffer_ps.cso");
	}
	if (spriteEx == nullptr)
	{
		spriteEx = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::SPRITE_EX);
		spriteEx->Create("./Data/Shader/SpriteEx_vs.cso", "./Data/Shader/SpriteEx_ps.cso");
	}
	if (flatShader == nullptr)
	{
		flatShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::FLAT);
		flatShader->Create("./Data/Shader/flatShader_vs.cso", "./Data/Shader/flatShader_ps.cso", "./Data/Shader/flatShader_gs.cso");
	}
	if (gaussShader == nullptr)
	{
		gaussShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::GAUSS);
		gaussShader->Create("./Data/Shader/GaussShader_vs.cso", "./Data/Shader/GaussShader_ps.cso");
	}
	if (multi_gaussShader == nullptr)
	{
		multi_gaussShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::MULTI_GAUSS);
		multi_gaussShader->Create("./Data/Shader/MultiGaussShader_vs.cso", "./Data/Shader/MultiGaussShader_ps.cso");
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
	}
	if (wait_R == nullptr)
	{
		wait_R = std::make_shared<Model>("./Data/FBX/Knight/Animation/knight_wait.fbx");
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
		test = std::make_unique<Sprite>(L"./Data/Image/BG/stage1.png", 3840.0f, 2160.0f);
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

	if (blur_texture[0] == nullptr)
	{
		for (int i = 0; i < blur_texture.size(); i++)
		{
			blur_texture[i] = std::make_unique<Texture>();
			blur_texture[i]->Create(1920, 1080, DXGI_FORMAT_R16G16B16A16_FLOAT);
			blur_texture[i]->CreateDepth(1920, 1080, DXGI_FORMAT_R24G8_TYPELESS);
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
}

void SceneTest::Update(float elapsed_time)
{

}

void SceneTest::Draw(float elapsed_time)
{
	static float light_color[4] = { 1,1,1,1 };
	//ライト方向
	static DirectX::XMFLOAT4 light_direction = DirectX::XMFLOAT4(0, -1, 1, 0);
	static DirectX::XMFLOAT4 ambient_color(0.3f, 0.3f, 0.3f, 0.5f);
	static float anim_count = 0.0f;
	//static DirectX::XMFLOAT3 box_angle = { DirectX::XMConvertToRadians(-90.0f),0.0f,0.0f };

#if USE_IMGUI
	//ImGui
	/*if(1)*/{
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
#if USE_IMGUI
	//ImGui
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
	CB_Multi_Render_Target cb;
	cb.light_direction = light_direction;
	cb.light_color = lightColor;
	cb.ambient_color = ambient_color;
	cb.eye_pos.x = YRCamera.GetEye().x;
	cb.eye_pos.y = YRCamera.GetEye().y;
	cb.eye_pos.z = YRCamera.GetEye().z;
	cb.eye_pos.w = 1.0f;

	//定数バッファ更新
	FRAMEWORK.context->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cb, 0, 0);
	FRAMEWORK.context->VSSetConstantBuffers(2, 1, constantBuffer.GetAddressOf());
	FRAMEWORK.context->PSSetConstantBuffers(2, 1, constantBuffer.GetAddressOf());


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
#ifdef USE_IMGUI
	{
		ImGui::InputFloat("knight_pos.x", &knight_pos.x, 0.01f, 0.01f);
		ImGui::InputFloat("knight_pos.y", &knight_pos.y, 0.01f, 0.01f);
		ImGui::InputFloat("knight_pos.z", &knight_pos.z, 0.01f, 0.01f);
		ImGui::InputFloat("knight_angle.x", &knight_angle.x, 0.01f, 0.01f);
		ImGui::InputFloat("knight_angle.y", &knight_angle.y, 0.01f, 0.01f);
		ImGui::InputFloat("knight_angle.z", &knight_angle.z, 0.01f, 0.01f);
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
	FRAMEWORK.framebuffer.SetRenderTexture(normal_texture->GetRenderTargetView());
	FRAMEWORK.framebuffer.SetRenderTexture(position_texture->GetRenderTargetView());
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

	//定数バッファ更新
	FRAMEWORK.context->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cb, 0, 0);
	FRAMEWORK.context->VSSetConstantBuffers(2, 1, constantBuffer.GetAddressOf());
	FRAMEWORK.context->PSSetConstantBuffers(2, 1, constantBuffer.GetAddressOf());

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

	motion->UpdateAnimation(elapsed_time);
	motion->CalculateLocalTransform();
	motion->CalculateWorldTransform(knight_pos,
		DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f),
		knight_angle);
	motion->Draw(
		skinShader.get(),
		view, projection, light_direction, light_color, ambient_color
	);

	circle.DrawCircle(spriteShader.get(), 400.0f, 900.0f);

	/*for (int i = 1; i < testrtv.size(); i++)
	{
		testrtv[i] = NULL;
	}
	testrtv[0] = FRAMEWORK.view.Get();*/
	//レンダーターゲットの回復
	//FRAMEWORK.context.Get()->OMSetRenderTargets(testrtv.size(), testrtv.data(), FRAMEWORK.depth.Get());
	FRAMEWORK.framebuffer.Deactivate();
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
	static float deviation = 1.5f;
#if USE_IMGUI
	//ImGui
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
	float w = (float)FRAMEWORK.SCREEN_WIDTH / 4.0f;
	float h = (float)FRAMEWORK.SCREEN_HEIGHT / 4.0f;

	if (blur)
	{
		//初回のみ現在の描画を利用する
		//テクスチャをセット
		FRAMEWORK.framebuffer.SetRenderTexture(blur_texture[0]->GetRenderTargetView());
		ID3D11DepthStencilView* dsv = color_texture->GetDepthStencilView();

		//画面をクリア
		FRAMEWORK.framebuffer.Clear();
		FRAMEWORK.context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		//レンダーターゲットビューの設定
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

		//定数バッファ更新
		FRAMEWORK.context->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cb, 0, 0);
		FRAMEWORK.context->VSSetConstantBuffers(2, 1, constantBuffer.GetAddressOf());
		FRAMEWORK.context->PSSetConstantBuffers(2, 1, constantBuffer.GetAddressOf());

		//ブレンドステート設定
		FRAMEWORK.BlendSet(Blend::ALPHA);
		//ラスタライザー設定
		FRAMEWORK.context->RSSetState(FRAMEWORK.rasterizer_state[framework::RS_CULL_BACK].Get());
		//デプスステンシルステート設定
		FRAMEWORK.context->OMSetDepthStencilState(FRAMEWORK.depthstencil_state[framework::DS_TRUE].Get(), 1);

		//サンプラー設定
		sampler_clamp->Set(0);

		gauss.CalcBlurParam(w, h, YR_Vector3(off_x, 0.0f), deviation);
		FRAMEWORK.context->UpdateSubresource(constantBuffer.Get(), 0, NULL, &gauss.param, 0, 0);
		FRAMEWORK.context->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

		sprite->render(
			gaussShader.get(),
			luminance_texture.get(),
			0.0f, 0.0f, 1920.0f, 1080.0f,
			0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);

		FRAMEWORK.framebuffer.Deactivate();
		FRAMEWORK.framebuffer.ResetRenderTexture();

		//特定の回数分描画し、テクスチャに書き込む
		for (int i = 1; i < blur_texture.size(); i++)
		{
			//テクスチャをセット
			FRAMEWORK.framebuffer.SetRenderTexture(blur_texture[i]->GetRenderTargetView());
			//ID3D11DepthStencilView* dsv = color_texture->GetDepthStencilView();

			//画面をクリア
			FRAMEWORK.framebuffer.Clear();
			FRAMEWORK.context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			//レンダーターゲットビューの設定
			FRAMEWORK.framebuffer.Activate(1920.0f, 1080.0f, dsv);

			//定数バッファの設定
			cb.light_direction = light_direction;
			cb.light_color = light_color;
			cb.ambient_color = ambient_color;
			cb.eye_pos.x = YRCamera.GetEye().x;
			cb.eye_pos.y = YRCamera.GetEye().y;
			cb.eye_pos.z = YRCamera.GetEye().z;
			cb.eye_pos.w = 1.0f;

			//定数バッファ更新
			FRAMEWORK.context->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cb, 0, 0);
			FRAMEWORK.context->VSSetConstantBuffers(2, 1, constantBuffer.GetAddressOf());
			FRAMEWORK.context->PSSetConstantBuffers(2, 1, constantBuffer.GetAddressOf());

			//ブレンドステート設定
			FRAMEWORK.BlendSet(Blend::ALPHA);
			//ラスタライザー設定
			FRAMEWORK.context->RSSetState(FRAMEWORK.rasterizer_state[framework::RS_CULL_BACK].Get());
			//デプスステンシルステート設定
			FRAMEWORK.context->OMSetDepthStencilState(FRAMEWORK.depthstencil_state[framework::DS_TRUE].Get(), 1);

			//サンプラー設定
			sampler_clamp->Set(0);


			if (i % 2)
			{
				gauss.CalcBlurParam(w, h, YR_Vector3(0.0f, off_y), deviation);
				FRAMEWORK.context->UpdateSubresource(constantBuffer.Get(), 0, NULL, &gauss.param, 0, 0);
				FRAMEWORK.context->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
			}
			else
			{
				gauss.CalcBlurParam(w, h, YR_Vector3(off_x, 0.0f), deviation);
				FRAMEWORK.context->UpdateSubresource(constantBuffer.Get(), 0, NULL, &gauss.param, 0, 0);
				FRAMEWORK.context->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
			}

			sprite->render(
				gaussShader.get(),
				blur_texture[i-1].get(),
				0.0f, 0.0f, 1920.0f, 1080.0f,
				0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);

			FRAMEWORK.framebuffer.Deactivate();
			FRAMEWORK.framebuffer.ResetRenderTexture();
		}

	//全てのテクスチャを合成したマルチガウスのテクスチャを作成する

		//テクスチャをセット
		FRAMEWORK.framebuffer.SetRenderTexture(multi_blur_texture->GetRenderTargetView());
		//ID3D11DepthStencilView* dsv = color_texture->GetDepthStencilView();

		//画面をクリア
		FRAMEWORK.framebuffer.Clear();
		FRAMEWORK.context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		//レンダーターゲットビューの設定
		FRAMEWORK.framebuffer.Activate(1920.0f, 1080.0f, dsv);
		//ブレンドステート設定
		FRAMEWORK.BlendSet(Blend::ALPHA);
		//ラスタライザー設定
		FRAMEWORK.context->RSSetState(FRAMEWORK.rasterizer_state[framework::RS_CULL_BACK].Get());
		//デプスステンシルステート設定
		FRAMEWORK.context->OMSetDepthStencilState(FRAMEWORK.depthstencil_state[framework::DS_TRUE].Get(), 1);

		//サンプラー設定
		sampler_clamp->Set(0);

		// シェーダリソースビューを設定.
		ID3D11ShaderResourceView* pSRV[] = {
			luminance_texture->GetShaderResource(),
			blur_texture[1]->GetShaderResource(),
			blur_texture[3]->GetShaderResource(),
			blur_texture[5]->GetShaderResource(),
			blur_texture[7]->GetShaderResource(),
			blur_texture[9]->GetShaderResource(),
			blur_texture[11]->GetShaderResource(),
		};

		sprite->render(
			multi_gaussShader.get(),
			luminance_texture.get(),
			pSRV,
			0.0f, 0.0f, 1920.0f, 1080.0f,
			0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);
		FRAMEWORK.framebuffer.Deactivate();
		FRAMEWORK.framebuffer.ResetRenderTexture();

	//ブルーム画像の描画

		//画面のクリア
		//FRAMEWORK.Clear(0x8080FFFF);

		////ビューポート設定
		//FRAMEWORK.SetViewPort(FRAMEWORK.SCREEN_WIDTH, FRAMEWORK.SCREEN_HEIGHT);

		////ラスタライザー設定
		//FRAMEWORK.context->RSGetState(FRAMEWORK.rasterizer_state[FRAMEWORK.RS_CULL_BACK].GetAddressOf());

		////デプスステンシルステート設定
		//FRAMEWORK.context->OMSetDepthStencilState(FRAMEWORK.depthstencil_state[FRAMEWORK.DS_TRUE].Get(), 1);

		//ブレンドステート設定
		FRAMEWORK.BlendSet(Blend::ADD);

		sprite->render(
			spriteEx.get(),
			multi_blur_texture.get(),
			0.0f, 0.0f, 1920.0f, 1080.0f,
			0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);
	}
}