#include "Scene.h"
#include "framework.h"
#include "Blur.h"
#include "YRSound.h"
#include <math.h>
#undef max

//-----------------------------------------------------------------
//			テストシーン
//-----------------------------------------------------------------
//・ゲームに使用しようと思っているシェーダーや処理をここでテストする
//-----------------------------------------------------------------

std::array<std::string, scastI(SceneTest::IntroState::FIN)+1> intro_list =
{
	u8"プレイヤー1のみ",
	u8"プレイヤー2までの間",
	u8"プレイヤー2のみ",
	u8"同時までの間",
	u8"同時",
	u8"最後",
};


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
	if (TessellationShader == nullptr)
	{
		TessellationShader = std::make_unique<YRShader>(ShaderType::TESSELLATION);
		TessellationShader->Create("./Data/Shader/Tessellation_vs.cso", "./Data/Shader/Tessellation_ps.cso", "./Data/Shader/Tessellation_ds.cso", "./Data/Shader/Tessellation_hs.cso");
	}

	//カメラ初期設定
	YRCamera.SetEye(DirectX::XMFLOAT3(0.0f, 5.0f, -25));			//視点
	YRCamera.SetFocus(DirectX::XMFLOAT3(0.0f, 5.0f, 0.0f));			//注視点
	YRCamera.SetUp(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));				//上方向
	YRCamera.SetPerspective(30 * 0.01745f, 1920.0f / 1080.0f, 1.4f, 1000.0f);

	if (board_texture == nullptr)
	{
		board_texture = std::make_shared<Texture>(L"./Data/Image/UI/GameScene/effect.png");
	}


	color_texture_main = nullptr;
	color_texture_face = nullptr;

	//color_texture_main = std::make_shared<Texture>(L"./Data/FBX/Knight/knight_tex_nofaces2.png");
	//color_texture_face = std::make_shared<Texture>(L"./Data/FBX/Knight/knight_tex_face2.png");

	if (knight == nullptr)
	{
		if (color_texture_main != nullptr)
		{
			knight = std::make_shared<Model>("./Data/FBX/Knight/knight_main.fbx", color_texture_main, color_texture_face);
		}
		else
		{
			//box = std::make_unique<Skinned_mesh>("./Data/FBX/Knight.fbx");
			knight = std::make_shared<Model>("./Data/FBX/Knight/knight_main.fbx");
			//knight = std::make_shared<Model>("./Data/FBX/Paimon/paimon.fbx");
		}
	}

	if (earth == nullptr)
	{
		earth = std::make_shared<Model>("./Data/Assets/earth/earth.fbx");
	}

	if (wait_R == nullptr)
	{
		wait_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_wait_R.fbx");
	}

	if (special_R == nullptr)
	{
		special_R = std::make_shared<Model>("./Data/FBX/Knight/AnimationR/knight_special_R.fbx");
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

	if (earth_motion == nullptr)
	{
		earth_motion = std::make_unique<ModelAnim>(earth);
	}

	//画像のロード
	if (test == nullptr)
	{
		test = std::make_unique<Sprite>(L"./Data/Image/BG/stage1.png", 3840.0f, 2160.0f);
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
		cutIn = std::make_unique<Sprite>(L"./Data/Image/Character/Knight/Knight_cut1.png", 1280.0f, 720.0f);
	}
	if (knight_1p_cut == nullptr)
	{
		knight_1p_cut = std::make_unique<Sprite>(L"./Data/Image/Character/Knight/Knight_cut1.png", 640.0f, 192.0f);
	}
	if (knight_2p_cut == nullptr)
	{
		knight_2p_cut = std::make_unique<Sprite>(L"./Data/Image/Character/Knight/Knight_cut2.png", 640.0f, 192.0f);
	}
	if (knight_name == nullptr)
	{
		knight_name = std::make_unique<Sprite>(L"./Data/Image/Character/Knight/Knight_name.png", 640.0f, 320.0f);
	}
	if (Box == nullptr)
	{
		Box = std::make_unique<Sprite>(L"./Data/Image/UI/GameLoad/Box.png", 640.0f, 640.0f);
	}
	if (VS_Image == nullptr)
	{
		VS_Image = std::make_unique<Sprite>(L"./Data/Image/UI/GameLoad/VS.png", 640.0f, 320.0f);
	}
	if (flash == nullptr)
	{
		flash = std::make_unique<Sprite>(L"./Data/Image/UI/GameLoad/flash.png", 640.0f, 640.0f);
	}
	/*if (cutIn == nullptr)
	{
		cutIn = std::make_unique<Sprite>(L"./Data/Image/Character/Knight/EIngeCFWoAEPD2D.png", 1280.0f, 720.0f);
	}*/


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

	if (knight_normal_map == nullptr)
	{
		knight_normal_map = std::make_shared<Texture>(L"Data/FBX/Knight/knight_tex_nofaces_normal.png");
	}
	if (knight_height_map == nullptr)
	{
		knight_height_map = std::make_shared<Texture>(L"Data/FBX/Knight/knight_tex_bump.png");
	}

	if (earth_normal_map == nullptr)
	{
		earth_normal_map = std::make_shared<Texture>(L"Data/Assets/earth/earthnormal.jpg");
	}
	if (earth_height_map == nullptr)
	{
		earth_height_map = std::make_shared<Texture>(L"Data/Assets/earth/earthbump.jpg");
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

	eye_cut = { 34.156f,5.693,-14.232 };
	focus_cut = { 8.539f,5.693f,2.898f };
	fov_cut = 50.0f;
	near_cut = 1.4f;
	far_cut = 100000.0f;

	pause = false;

	eye_offset = { 0.2f,0.1f };
	mouth_offset = { 0.0f,0.0f };

	match_timer = 0.0f;
	plus_match = 60.0f;

	slow_add = 0.5f;

	timer_Multiply = 3.5f;

	sin_max = 0.05f;

	space_time = 0.8f;

	VS_size = 5.0f;

	VS_alpha = 0.0f;

	flash_size = 0.0f;
	flash_alpha = 0.0f;

	line_1p_x = 0.0f;
	line_2p_x = 0.0f;

	line_Multiply = 10000.0f;
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

	if (cut_timer > 0.5f)
	{
		cut_timer = 0.0f;
		timer_start = false;
		cut_fin = true;
	}

	if (pKeyState.pflg == 1)
	{
		pause = !pause;
	}

	if (pKeyState.oflg == 1)
	{
		YRCamera.SetEye(eye_cut);
		YRCamera.SetFocus(focus_cut);
		YRCamera.SetPerspective(fov_cut * 0.01745f, 1920.0f / 1080.0f, near_cut, far_cut);
		knight_pos.x = 14.0f;
		knight_pos.z = 0.0f;
		motion->NodeChange(special_R);
	}

	MatchUpdate(elapsed_time);
}

void SceneTest::Draw(float elapsed_time)
{
	static float light_color[4] = { 1,1,1,1 };
	//ライト方向
	static DirectX::XMFLOAT4 light_direction = DirectX::XMFLOAT4(-1.0f, -0.1, 1.0f, 0.0f);
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
			//ImGui::Image((void *)(normal_texture->GetShaderResource()), ImVec2(360, 360));
			//ImGui::Image((void *)(position_texture->GetShaderResource()), ImVec2(360, 360));
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
	FRAMEWORK.framebuffer.Deactivate();
}


void SceneTest::RenderTexture(
	const DirectX::XMMATRIX&	view,
	const DirectX::XMMATRIX&	projection,
	const DirectX::XMFLOAT4&	light_direction,
	const DirectX::XMFLOAT4&	light_color,
	const DirectX::XMFLOAT4&	ambient_color,
	float						elapsed_time)
{
	static float Distance = 0.0f;
	static float Density = 1.0f;
	//static DirectX::XMFLOAT3 sky_scale = { 10.0f,10.0f,10.0f };
	static float sky_scale[3] = { 1.0f,1.0f,1.0f };
	static float devide = 0.0f;
	static float left_x = 0.0f;
	static float right_x = 0.0f;
	static float up_y = 0.0f;
	static float down_y = 0.0f;
	float sin_match = 0.0f;
	static float line_size = 2500.0f;

	sin_match = std::max(sinf(match_timer), 0.0f);
#ifdef EXIST_IMGUI
	if(Get_Use_ImGui())
	{
		if(ImGui::Button("IntroStart"))
		{
			MatchStart();
		}

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
		ImGui::SliderFloat("knight1p_pos_x", &knight_1p_pos_x, -960.0f, 1920.0f + 960.0f);
		ImGui::SliderFloat("knight2p_pos_x", &knight_2p_pos_x, -960.0f, 1920.0f + 960.0f);
		ImGui::SliderFloat("left", &left_x, -960.0f, 1920.0f + 960.0f);
		ImGui::SliderFloat("right", &right_x, -960.0f, 1920.0f + 960.0f);
		ImGui::SliderFloat("up", &up_y, -960.0f, 1920.0f + 960.0f);
		ImGui::SliderFloat("down", &down_y, -960.0f, 1920.0f + 960.0f);
		ImGui::InputFloat(u8"Sinに乗算する値", &plus_match, 1.0f, 10.0f);
		ImGui::InputFloat(u8"最低限進む横の値", &slow_add, 1.0f, 10.0f);
		ImGui::InputFloat(u8"match_timerに乗算する値", &timer_Multiply, 1.0f, 10.0f);
		ImGui::InputFloat(u8"sinの最大値", &sin_max, 1.0f, 10.0f);
		ImGui::InputFloat(u8"間の長さ", &space_time, 0.1f, 1.0f);
		ImGui::InputFloat(u8"ラインの長さ", &line_size, 10.0f, 100.0f);
		ImGui::InputFloat(u8"ラインに乗算する値", &line_Multiply, 10.0f, 100.0f);
		ImGui::Text(intro_list[scastI(intro_state)].c_str());

		ImGui::Text("match_timer : %f", sin_match);
		ImGui::Text("line1p : %f", line_1p_x);
		ImGui::Text("line2p : %f", line_2p_x);
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
	FRAMEWORK.framebuffer.GetDefaultRTV();

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
	FRAMEWORK.framebuffer.Activate(1920.0f, 1080.0f, dsv);

	//定数バッファの設定
	/*CB_Multi_Render_Target cb;
	cb.light_direction = light_direction;
	cb.light_color = light_color;
	cb.ambient_color = ambient_color;
	cb.eye_pos.x = YRCamera.GetEye().x;
	cb.eye_pos.y = YRCamera.GetEye().y;
	cb.eye_pos.z = YRCamera.GetEye().z;
	cb.eye_pos.w = 1.0f;
	cb.Density = Density;
	cb.Distance = Distance;*/

	ConstantBufferForPerFrame cb;
	cb.light_direction = light_direction;
	cb.light_color = light_color;
	cb.ambient_color = ambient_color;
	cb.eye_pos.x = YRCamera.GetEye().x;
	cb.eye_pos.y = YRCamera.GetEye().y;
	cb.eye_pos.z = YRCamera.GetEye().z;
	static float wave_time = 0.0f;
	wave_time += elapsed_time;
	cb.wave_time = wave_time;
	cb.divide = devide;

	//定数バッファ更新
	FRAMEWORK.context->UpdateSubresource(constantBuffer.Get(), 0, NULL, &cb, 0, 0);
	FRAMEWORK.context->VSSetConstantBuffers(2, 1, constantBuffer.GetAddressOf());
	FRAMEWORK.context->PSSetConstantBuffers(2, 1, constantBuffer.GetAddressOf());
	FRAMEWORK.context->HSSetConstantBuffers(2, 1, constantBuffer.GetAddressOf());
	FRAMEWORK.context->DSSetConstantBuffers(2, 1, constantBuffer.GetAddressOf());

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

	//test->DrawExtendGraph(spriteShader.get(), 0.0f, 0.0f, FRAMEWORK.SCREEN_WIDTH, FRAMEWORK.SCREEN_HEIGHT);

	FRAMEWORK.context->OMSetDepthStencilState(FRAMEWORK.depthstencil_state[framework::DS_WRITE_FALSE].Get(), 1);

	if (pause)
	{
		//FRAMEWORK.fade_img->DrawRotaGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 1.0f, SpriteMask::NONE, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
		motion->UpdateAnimation(0.0f);
		motion->CalculateLocalTransform();
		motion->CalculateWorldTransform(knight_pos,
			DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f),
			knight_angle);
		motion->Draw(
			toonShader.get(),
			view, projection, light_direction, light_color, ambient_color,eye_offset,mouth_offset,Model::Material_Attribute::SWORD
		);

		/*earth_motion->CalculateLocalTransform();
		earth_motion->CalculateWorldTransform(knight_pos,
			DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f),
			knight_angle);
		earth_motion->Draw(
			TessellationShader.get(),
			view, projection, light_direction, light_color, ambient_color, eye_offset, mouth_offset, Model::Material_Attribute::SWORD
		);*/
	}
	else
	{
		/*sky->Render(skyShader.get(),
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
		);*/

		//sampler_wrap->Set(1);
		//fur->Set(1);
		//ブレンドステート設定

		//sampler_wrap->Set(0);
		motion->UpdateAnimation(elapsed_time);
		motion->CalculateLocalTransform();

		knight_normal_map->Set(1);
		knight_height_map->Set(2);
		earth_normal_map->Set(1);
		earth_height_map->Set(2);

		sampler_clamp->Set(0);
		sampler_wrap->Set(1);

		knight_angle.y += elapsed_time;

		/*earth_motion->CalculateLocalTransform();
		earth_motion->CalculateWorldTransform(knight_pos,
			DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f),
			knight_angle);
		earth_motion->Draw(
			TessellationShader.get(),
			view, projection,
			light_direction, light_color,
			D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
			ambient_color
		);*/

		/*motion->CalculateWorldTransform(knight_pos,
			DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f),
			knight_angle);
		motion->Draw(
			toonShader.get(),
			view, projection,
			light_direction, light_color,
			D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
			ambient_color
		);*/
		/*motion->Draw(
			skinShader.get(),
			view, projection, light_direction, light_color, ambient_color
		);

		motion->Draw(
			furShader.get(),
			view, projection, light_direction, light_color, ambient_color
		);*/
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
					SpriteMask::FRAME
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
					SpriteMask::FRAME
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
			false,
			SpriteMask::INDRAW
		);
	}

	knight_1p_cut->DrawRotaGraph(
		spriteShader.get(),
		knight_1p_pos_x,
		265.0f,
		0.0f,
		2.8f);

	knight_2p_cut->DrawRotaGraph(
		spriteShader.get(),
		knight_2p_pos_x,
		810.0f,
		0.0f,
		2.8f,
		true);

	knight_name->DrawRotaGraph(
		spriteShader.get(),
		knight_1p_pos_x - 680.0f,
		265.0f + 100.0f,
		0.0f,
		1.0f);

	knight_name->DrawRotaGraph(
		spriteShader.get(),
		knight_2p_pos_x + 680.0f,
		810.0f + 100.0f,
		0.0f,
		1.0f);

	Box->DrawExtendGraph(
		spriteShader.get(),
		line_1p_x-line_size,
		515.0f,
		line_1p_x,
		540.0f,
		SpriteMask::NONE,
		DirectX::XMFLOAT4(1.0f, 0.1f, 0.1f, 1.0f)
	);
	Box->DrawExtendGraph(
		spriteShader.get(),
		line_2p_x,
		540.0f,
		line_2p_x+line_size,
		565.0f,
		SpriteMask::NONE,
		DirectX::XMFLOAT4(0.1f, 0.1f, 1.0f, 1.0f)
	);

	flash->DrawRotaGraph(
		spriteShader.get(),
		(float)FRAMEWORK.SCREEN_WIDTH / 2.0f,
		(float)FRAMEWORK.SCREEN_HEIGHT / 2.0f,
		0.0f,
		flash_size,
		false,
		SpriteMask::NONE,
		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, flash_alpha)
	);

	if (intro_state == IntroState::FIN || intro_state == IntroState::P1P2)
	{
		if (VS_alpha < 1.0f)
		{
			VS_alpha += elapsed_time * 10.0f;
		}
		else
		{
			VS_alpha = 1.0f;
		}

		if (VS_size > 1.0f)
		{
			VS_size -= elapsed_time * 10.0f;
		}
		else
		{
			VS_size = 1.0f;
		}

		VS_Image->DrawRotaGraph(
			spriteShader.get(),
			(float)FRAMEWORK.SCREEN_WIDTH / 2.0f,
			(float)FRAMEWORK.SCREEN_HEIGHT / 2.0f,
			0.0f,
			VS_size,
			false,
			SpriteMask::NONE,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, VS_alpha)
		);
	}
	else
	{
		VS_alpha = 0.0f;
		VS_size = 3.0f;
	}



	/*cutIn->DrawRotaGraph(
		spriteShader.get(),
		center_x,
		center_y+500.0f,
		-90.0f,
		2.0f,
		SpriteMask::INDRAW
	);*/

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
		/*FRAMEWORK.SetViewPort(1920.0f, 1080.0f);
		FRAMEWORK.framebuffer.Activate(dsv);*/
		FRAMEWORK.framebuffer.Activate(1920.0f, 1080.0f, dsv);

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

			ID3D11DepthStencilView* dsv2 = blur_texture[i]->GetDepthStencilView();
			FRAMEWORK.context->ClearDepthStencilView(dsv2, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			//レンダーターゲットビューの設定
			FRAMEWORK.framebuffer.Activate(w * 0.5f / riv2, h * 0.5f / riv2, dsv2);

			sprite->render(
				gaussShader.get(),
				blur_texture[i-1].get(),
				0.0f, 0.0f, 1920.0f * 0.5f / riv2, 1080.0f * 0.5f / riv2,
				0.0f, 0.0f, 1920.0f * 0.5f / riv, 1080.0f * 0.5f / riv, 0.0f, 1.0f);

			//FRAMEWORK.framebuffer.Deactivate();
			//FRAMEWORK.framebuffer.ResetRenderTexture();
			int hoge = 0;
		}

	//全てのテクスチャを合成したマルチガウスのテクスチャを作成する
		/*FRAMEWORK.framebuffer.Deactivate();
		FRAMEWORK.framebuffer.ResetRenderTexture();
		FRAMEWORK.framebuffer.SetDefaultRTV();*/

		//テクスチャをセット
		//FRAMEWORK.framebuffer.SetRenderTexture(multi_blur_texture->GetRenderTargetView(), true);
		//ID3D11DepthStencilView* dsv = color_texture->GetDepthStencilView();

		//画面をクリア
		//FRAMEWORK.framebuffer.Clear();
		//FRAMEWORK.context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		//レンダーターゲットビューの設定
		//FRAMEWORK.framebuffer.Activate(dsv);
		//ブレンドステート設定
		//FRAMEWORK.BlendSet(Blend::ALPHA);
		//ラスタライザー設定
		//FRAMEWORK.context->RSSetState(FRAMEWORK.rasterizer_state[framework::RS_CULL_BACK].Get());
		//デプスステンシルステート設定
		//FRAMEWORK.context->OMSetDepthStencilState(FRAMEWORK.depthstencil_state[framework::DS_TRUE].Get(), 1);

		//サンプラー設定
		//sampler_clamp->Set(0);
		FRAMEWORK.framebuffer.SetDefaultRTV();
		FRAMEWORK.SetViewPort((float)FRAMEWORK.SCREEN_WIDTH, (float)FRAMEWORK.SCREEN_HEIGHT);

		//ブレンドステート設定
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

		//レンダーターゲットの回復
		//FRAMEWORK.context.Get()->OMSetRenderTargets(1, FRAMEWORK.view.GetAddressOf(), FRAMEWORK.depth.Get());

		////ブレンドステート設定
		//FRAMEWORK.BlendSet(Blend::ADD);

		/*sprite->render(
			spriteEx.get(),
			multi_blur_texture.get(),
			0.0f, 0.0f, 1920.0f, 1080.0f,
			0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);*/
	}
}


void SceneTest::MatchStart()
{
	match_on = true;
	knight_1p_pos_x = -(float)FRAMEWORK.SCREEN_WIDTH/2.0f;
	knight_2p_pos_x = (float)FRAMEWORK.SCREEN_WIDTH + (float)FRAMEWORK.SCREEN_WIDTH / 2.0f;
	line_1p_x = -(float)FRAMEWORK.SCREEN_WIDTH / 2.0f;
	line_2p_x = (float)FRAMEWORK.SCREEN_WIDTH + (float)FRAMEWORK.SCREEN_WIDTH / 2.0f;
	match_timer = 0.0f;
	flash_alpha = 0.0f;
	flash_size = 0.0f;
	intro_state = IntroState::P1;
	GetSound().BGMStop(BGMKind::LOAD);
	GetSound().BGMPlay(BGMKind::LOAD);
}

void SceneTest::MatchUpdate(float elapsed_time)
{
	if (!match_on)
	{
		return;
	}

	//knight_1p_pos_x += ((sin_match * plus_match) + slow_add);
	//knight_2p_pos_x -= ((sin_match * plus_match) + slow_add);

	switch (intro_state)
	{
	case SceneTest::IntroState::P1:
		//プレイヤー1だけ表示
	{
		//タイマー加算
		match_timer += elapsed_time * timer_Multiply;

		//sin派を使って速度に緩急をつける
		float sin_match = 0.0f;
		sin_match = std::max(sinf(match_timer), sin_max);

		//速度を加算
		knight_1p_pos_x += ((sin_match * plus_match) + slow_add);

		//ラインは画面外に行くまで加算
		if (line_1p_x < (float)FRAMEWORK.SCREEN_WIDTH)
		{
			line_1p_x += (line_Multiply * elapsed_time);
		}

		//プレイヤー1が所定の場所に到達したら次のステートへ
		if (knight_1p_pos_x > (float)FRAMEWORK.SCREEN_WIDTH + ((float)FRAMEWORK.SCREEN_WIDTH * 0.6f))
		{
			intro_state = IntroState::SPACE1;
			match_timer = 0.0f;
		}
	}
		break;
	case SceneTest::IntroState::SPACE1:
		//少し間を置く
		match_timer += elapsed_time;

		//一定の速度で画面外に
		knight_1p_pos_x += (plus_match + slow_add);
		line_1p_x += (line_Multiply * elapsed_time);

		//一定時間経ったら次のステートへ
		if (match_timer > space_time)
		{
			match_timer = 0.0f;
			intro_state = IntroState::P2;
		}
		break;
	case SceneTest::IntroState::P2:
		//プレイヤー2だけ表示
	{
		//タイマー加算
		match_timer += elapsed_time * timer_Multiply;

		//sin派を使って速度に緩急をつける
		float sin_match = 0.0f;
		sin_match = std::max(sinf(match_timer), sin_max);

		//速度を加算
		knight_2p_pos_x -= ((sin_match * plus_match) + slow_add);

		//ラインは画面外に行くまで加算
		if (line_2p_x > 0.0f)
		{
			line_2p_x -= (line_Multiply * elapsed_time);
		}

		//プレイヤー2が所定の場所に到達したら次のステートへ
		if (knight_2p_pos_x < 0.0f - ((float)FRAMEWORK.SCREEN_WIDTH * 0.6f))
		{
			intro_state = IntroState::SPACE2;
			match_timer = 0.0f;
		}
	}
		break;
	case SceneTest::IntroState::SPACE2:
		//少し間を置く
		match_timer += elapsed_time;

		//一定の速度で画面外に
		knight_2p_pos_x -= (plus_match + slow_add);
		line_2p_x -= (line_Multiply * elapsed_time);

		//光を少しずつ大きくさせながら表示させる
		if (flash_alpha < 1.0f)
		{
			flash_alpha += elapsed_time * 2.0f;
		}
		else
		{
			flash_alpha = 1.0f;
		}

		flash_size += elapsed_time * 10.0f;

		//一定時間経ったら次のステートへ
		if (match_timer > space_time)
		{
			match_timer = 0.0f;
			knight_1p_pos_x = -(float)FRAMEWORK.SCREEN_WIDTH / 2.0f;
			knight_2p_pos_x = (float)FRAMEWORK.SCREEN_WIDTH + (float)FRAMEWORK.SCREEN_WIDTH / 2.0f;
			line_1p_x = -(float)FRAMEWORK.SCREEN_WIDTH / 2.0f;
			line_2p_x = (float)FRAMEWORK.SCREEN_WIDTH + (float)FRAMEWORK.SCREEN_WIDTH / 2.0f;
			intro_state = IntroState::P1P2;
		}
		break;
	case SceneTest::IntroState::P1P2:
		//両方表示する
	{
		//タイマー加算
		match_timer += elapsed_time * timer_Multiply;

		//sin派を使って速度に緩急をつける
		float sin_match = 0.0f;
		sin_match = sinf(match_timer);

		//光を薄くしていく
		if (flash_alpha > 0.0f)
		{
			flash_alpha -= elapsed_time * 2.0f;
		}
		else
		{
			flash_alpha = 0.0f;
		}

		//sin派が0以下になったら次のステートへ
		if (sin_match < sin_max)
		{
			intro_state = IntroState::FIN;
			match_timer = 0.0f;
		}
		else
		{
			//速度を加算
			knight_1p_pos_x += ((sin_match * plus_match) + slow_add);
			knight_2p_pos_x -= ((sin_match * plus_match) + slow_add);

			//ラインは画面外に行くまで加算
			if (line_1p_x < (float)FRAMEWORK.SCREEN_WIDTH)
			{
				line_1p_x += (line_Multiply * elapsed_time);
			}
			if (line_2p_x > 0.0f)
			{
				line_2p_x -= (line_Multiply * elapsed_time);
			}
		}

	}
		break;
	case SceneTest::IntroState::FIN:
		//少しだけ動かしていく
		knight_1p_pos_x += slow_add;
		knight_2p_pos_x -= slow_add;
		break;
	default:
		break;
	}
}