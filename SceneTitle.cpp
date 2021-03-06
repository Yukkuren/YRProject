﻿#include "Scene.h"
#include "framework.h"
#include "YRSound.h"
#include "Blur.h"


//-------------------------------------------------------------
// **シーン概要**
//・ボタンを押すとモードセレクトへ
//・その後の選択でプレイヤー2のコントロールを変更する
//-------------------------------------------------------------

void SceneTitle::Init()
{
	//初期化
	load_fin = false;
	load_state = 0;
	p1_pos = { 680.0f,500.0f };
	p2_pos = { 1100.0f,500.0f };
	none_pos = { 900.0f,700.0f };
	p1 = none_pos;
	p2 = none_pos;
	Rato = 3.0f;
	g1.Init();
	g2.Init();
	fado_start = false;
	fado_alpha = 1.0f;

	select_p1 = scastI(INPUT_PLAYER::NONE);
	select_p2 = scastI(INPUT_PLAYER::NONE);
	p1Enter = false;
	p2Enter = false;
	end = false;
	timer = 0.0f;


	//タイトルシェーダー用パラメータ
	cbuffer_param.Resolution = { 1920.0f,1080.0f,(1920.0f / 1080.0f) };
	cbuffer_param.brightness = 15.0f;
	cbuffer_param.gamma = 13;
	cbuffer_param.spot_brightness = 1.5f;
	cbuffer_param.ray_density = 1.5f;
	cbuffer_param.curvature = 90.0f;
	cbuffer_param.red = 10.0f;
	cbuffer_param.green = 2.8f;
	cbuffer_param.blue = 4.0f;
	cbuffer_param.material_color = { 1.0f,1.0f,1.0f,1.0f };
	cbuffer_param.dummy1 = 0.0f;
	cbuffer_param.dummy2 = 0.0f;
	cbuffer_param.dummy3 = 0.0f;


	//スプライトのシェーダーはフェード画像に使用するため初期化の時点で読み込む
	if (spriteShader == nullptr)
	{
		spriteShader = std::make_unique<YRShader>(ShaderType::SPRITE);
		spriteShader->Create("./Data/Shader/sprite_vs.cso", "./Data/Shader/sprite_ps.cso");
	}

	//初期モードの設定
	vs_mode = VS_MODE::CPU;
	state = STATE::HOME;

	//タイトルのBGMを再生する
	GetSound().BGMPlay(BGMKind::TITLE);
}

void SceneTitle::LoadData()
{
	//スプライト読み込み
	if (test == nullptr)
	{
		test = std::make_unique<Sprite>(L"./Data/Image/BG/select.png", 1920.0f, 1080.0f);
	}
	if (select_img == nullptr)
	{
		select_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameSelect/select.png", 64.0f, 64.0f);
	}
	if (title_img == nullptr)
	{
		title_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameTitle/Title.png", 3840.0f, 2160.0f);
	}
	if (choice_img == nullptr)
	{
		choice_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameTitle/Select.png", 1920.0f, 1920.0f,1,3,1920.0f,640.0f);
	}
	if (choice_mask == nullptr)
	{
		choice_mask = std::make_unique<Sprite>(L"./Data/Image/UI/GameTitle/Select_mask.png", 1920.0f, 1920.0f,1,3,1920.0f,640.0f);
	}
	if (choice_anim == nullptr)
	{
		choice_anim = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/gauge_anim.png", 640.0f, 64.0f);
	}

	//コンスタントバッファ作成
	FRAMEWORK.CreateConstantBuffer(constantBuffer.GetAddressOf(), sizeof(Title_CBuffer));

	//テクスチャロード
	if (color_texture == nullptr)
	{
		color_texture = std::make_unique<Texture>();
		color_texture->Create(1920, 1080, DXGI_FORMAT_R8G8B8A8_UNORM);
		color_texture->CreateDepth(1920, 1080, DXGI_FORMAT_R24G8_TYPELESS);
	}

	if (luminance_texture == nullptr)
	{
		luminance_texture = std::make_unique<Texture>();
		luminance_texture->Create(1920, 1080, DXGI_FORMAT_R16G16B16A16_FLOAT);
		luminance_texture->CreateDepth(1920, 1080, DXGI_FORMAT_R24G8_TYPELESS);
	}

	//タイトルシェーダーに送る画像(ノイズ画像を読み込んでいる)
	if (title_texture == nullptr)
	{
		title_texture = std::make_unique<Texture>(L"./Data/Shader/noise.png");
	}

	//Gbuffer用スプライト
	if (sprite == nullptr)
	{
		sprite = std::make_unique<Sprite>();
	}

	//サンプラー生成
	if (sampler_clamp == nullptr)
	{
		sampler_clamp = std::make_shared<Sampler>(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP);
	}
	if (sampler_wrap == nullptr)
	{
		sampler_wrap = std::make_shared<Sampler>(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP);
	}

	//シェーダー作成
	if (spriteEx == nullptr)
	{
		spriteEx = std::make_unique<YRShader>(ShaderType::SPRITE_EX);
		spriteEx->Create("./Data/Shader/SpriteEx_vs.cso", "./Data/Shader/SpriteEx_ps.cso");
	}
	if (titleShader == nullptr)
	{
		titleShader = std::make_unique<YRShader>(ShaderType::TITLE);
		titleShader->Create("./Data/Shader/TitleShader_vs.cso", "./Data/Shader/TitleShader_ps.cso");
	}

	//シェーダーテスト用
	//・読み込むシェーダーを変えることでシェーダーの動作をチェックしている

	/*if (titleShader == nullptr)
	{
		titleShader = std::make_unique<YRShader>(ShaderType::TITLE);
		titleShader->Create("./Data/Shader/LoadShader_vs.cso", "./Data/Shader/LoadShader_ps.cso");
	}*/
	/*if (titleShader == nullptr)
	{
		titleShader = std::make_unique<YRShader>(ShaderType::TITLE);
		titleShader->Create("./Data/Shader/CharaSelectShader_vs.cso", "./Data/Shader/CharaSelectShader_ps.cso");
	}*/
	/*if (titleShader == nullptr)
	{
		titleShader = std::make_unique<YRShader>(ShaderType::TITLE);
		titleShader->Create("./Data/Shader/CyberShader_vs.cso", "./Data/Shader/CyberShader_ps.cso");
	}*/
	/*if (titleShader == nullptr)
	{
		titleShader = std::make_unique<YRShader>(ShaderType::TITLE);
		titleShader->Create("./Data/Shader/ConcentrationShader_vs.cso", "./Data/Shader/ConcentrationShader_ps.cso");
	}*/
	/*if (titleShader == nullptr)
	{
		titleShader = std::make_unique<YRShader>(ShaderType::TITLE);
		titleShader->Create("./Data/Shader/ModeSelectShader_vs.cso", "./Data/Shader/ModeSelectShader_ps.cso");
	}*/
	/*if (titleShader == nullptr)
	{
		titleShader = std::make_unique<YRShader>(ShaderType::TITLE);
		titleShader->Create("./Data/Shader/SilexarsShader_vs.cso", "./Data/Shader/SilexarsShader_ps.cso");
	}*/

	load_state = 3;
}

void SceneTitle::UnInit()
{
	//解放処理

	test.reset();
	test = nullptr;
	knight_icon.reset();
	knight_icon = nullptr;
	ken_icon.reset();
	ken_icon = nullptr;
	select_img.reset();
	select_img = nullptr;
	spriteShader.reset();
	spriteShader = nullptr;
	spriteEx.reset();
	spriteEx = nullptr;
	titleShader.reset();
	titleShader = nullptr;
	sampler_clamp.reset();
	sampler_clamp = nullptr;
	sprite.reset();
	sprite = nullptr;
	color_texture.reset();
	color_texture = nullptr;
	luminance_texture.reset();
	luminance_texture = nullptr;

	title_img.reset();
	title_img = nullptr;
	choice_img.reset();
	choice_img = nullptr;
}

void SceneTitle::Update(float elapsed_time)
{
	//ロード終了
	if (load_fin)
	{
		/*if (pKeyState.sflg == 1)
		{
			GetSound().SESinglePlay(SEKind::SELECT_ENTER);
		}
		if (pKeyState.aflg > 0)
		{
			GetSound().BGMPause(BGMKind::TITLE);
			GetSound().SEPlay(SEKind::SELECT_ENTER);
		}
		else
		{
			GetSound().BGMPlay(BGMKind::TITLE);
		}

		static bool flgo = false;
		if (pKeyState.dflg == 1)
		{
			flgo = true;
		}

		if (flgo)
		{
			if (GetSound().FadeOut(elapsed_time))
			{
				flgo = false;
			}
		}

		static bool flgi = false;
		if (pKeyState.wflg == 1)
		{
			flgi = true;
		}

		if (flgi)
		{
			if (GetSound().FadeIn(elapsed_time))
			{
				flgi = false;
			}
		}*/


		//先行会用に直接ロードに行くようにする
		if (!fado_start)
		{
			//if (pKeyState.hflg == 1 || g1.x_input[scastI(PAD::X)] == 1)
			//{
			//	GetSound().FadeOut(elapsed_time);
			//	select_p1 = scastI(INPUT_PLAYER::P1);
			//	select_p2 = scastI(INPUT_PLAYER::P2);
			//	//FRAMEWORK.scenegame.PadSet(select_p1);
			//	//UnInit();
			//	FRAMEWORK.sceneselect.select_p1 = scastI(PLSELECT::KNIGHT);
			//	FRAMEWORK.sceneselect.select_p2 = scastI(PLSELECT::KNIGHT);
			//	fado_start = true;
			//	//FRAMEWORK.SetScene(SCENE_SELECT);
			//}
		}
#ifdef EXIST_IMGUI

		if (Get_Use_ImGui())
		{
			/*if (pKeyState.tflg == 1)
			{
				GetSound().BGMStop(BGMKind::TITLE);
				select_p1 = scastI(INPUT_PLAYER::P1);
				select_p2 = scastI(INPUT_PLAYER::P2);
				FRAMEWORK.scenegame.PadSet(select_p1);
				UnInit();
				FRAMEWORK.SetScene(SCENE_TABLE::SCENE_TEST);
				return;
			}*/
		}

#endif // USE_IMGUI

		//timerはフローしないようにリセットする
		if (timer < 1000.0f)
		{
			timer += elapsed_time;
		}
		else
		{
			timer = 0;
		}

		g1.Update(elapsed_time);
		g2.Update(elapsed_time);

		//プレイヤー1のカーソル移動処理
		//if (p1Enter)
		//{
		//	if (g1.x_input[scastI(PAD::B)] == 1)
		//	{
		//		p1Enter = false;
		//	}
		//}
		//else
		//{
		//	if (g1.x_input[scastI(PAD::STICK_R)] == 1)
		//	{
		//		select_p1++;
		//		if (select_p1 > scastI(INPUT_PLAYER::P2))
		//		{
		//			select_p1 = scastI(INPUT_PLAYER::P2);
		//		}
		//	}
		//	if (g1.x_input[scastI(PAD::STICK_L)] == 1)
		//	{
		//		select_p1--;
		//		if (select_p1 < 0)
		//		{
		//			select_p1 = scastI(INPUT_PLAYER::P1);
		//		}
		//	}

		//	if (g1.x_input[scastI(PAD::A)] == 1&&select_p1!=scastI(INPUT_PLAYER::NONE))
		//	{
		//		if (select_p2 != select_p1)
		//		{
		//			p1Enter = true;
		//		}
		//		//PlaySE(SE_ENTER);
		//	}
		//}

		////プレイヤー2のカーソル移動処理
		//if (p2Enter)
		//{
		//	if (g2.x_input[scastI(PAD::B)] == 1)
		//	{
		//		p2Enter = false;
		//	}
		//}
		//else
		//{
		//	if (g2.x_input[scastI(PAD::STICK_R)] == 1)
		//	{
		//		select_p2++;
		//		if (select_p2 > scastI(INPUT_PLAYER::P2))
		//		{
		//			select_p2 = scastI(INPUT_PLAYER::P2);
		//		}
		//	}
		//	if (g2.x_input[scastI(PAD::STICK_L)] == 1)
		//	{
		//		select_p2--;
		//		if (select_p2 < 0)
		//		{
		//			select_p2 = scastI(INPUT_PLAYER::P1);
		//		}
		//	}
		//	if (g2.x_input[scastI(PAD::A)] == 1 && select_p2 != scastI(INPUT_PLAYER::NONE))
		//	{
		//		if (select_p1 != select_p2)
		//		{
		//			p2Enter = true;
		//		}
		//		//PlaySE(SE_ENTER);
		//	}
		//}


		//p1 = PosSet(select_p1);
		//p2 = PosSet(select_p2);

		////両方のプレイヤーが決定したら
		//if (p1Enter && p2Enter)
		//{
		//	end = true;
		//}
		//if (end)
		//{
		//	//フェードアウトをスタートさせる
		//	fado_start = true;
		//}


		switch (state)
		{
		case SceneTitle::STATE::HOME:
			for (int i = 0; i < any_button.size(); i++)
			{
				if (g1.x_input[scastI(any_button[i])] == 1 || g2.x_input[scastI(any_button[i])] == 1)
				{
					state = STATE::SELECT;
					GetSound().SESinglePlay(SEKind::SELECT_ENTER);
				}
			}
			break;
		case SceneTitle::STATE::SELECT:

			if (vs_mode == VS_MODE::CPU)
			{
				if (g1.x_input[scastI(PAD::STICK_R)] == 1 || g2.x_input[scastI(PAD::STICK_R)] == 1)
				{
					vs_mode = VS_MODE::PLAYER;
					//SE再生
					GetSound().SESinglePlay(SEKind::SELECT);
				}
			}
			if (vs_mode == VS_MODE::PLAYER)
			{
				if (g1.x_input[scastI(PAD::STICK_L)] == 1 || g2.x_input[scastI(PAD::STICK_L)] == 1)
				{
					vs_mode = VS_MODE::CPU;
					//SE再生
					GetSound().SESinglePlay(SEKind::SELECT);
				}
			}
			for (int i = 0; i < any_button.size(); i++)
			{
				if (g1.x_input[scastI(any_button[i])] == 1 || g2.x_input[scastI(any_button[i])] == 1)
				{
					if (any_button[i] == PAD::B)
					{
						state = STATE::HOME;
						GetSound().SESinglePlay(SEKind::SELECT_CANCEL);
					}
					else
					{
						state = STATE::END;
						GetSound().SESinglePlay(SEKind::SELECT_ENTER);
					}
				}
			}

			break;
		case SceneTitle::STATE::END:
			fado_start = true;
			GetSound().FadeOut(elapsed_time);
			break;
		default:
			break;
		}

		switch (vs_mode)
		{
		case SceneTitle::VS_MODE::PLAYER:
			FRAMEWORK.scenegame.pl2_con = SceneGame::Player2PControl::OPERATION;
			break;
		case SceneTitle::VS_MODE::CPU:
			FRAMEWORK.scenegame.pl2_con = SceneGame::Player2PControl::AI;
			break;
		default:
			break;
		}

		if (fado_start)
		{
			//if (FedoOut(elapsed_time))
			//{
			//	//フェードアウトが終わったらセレクト画面へ
			//	//ここで入力を保存する
			//	//関数にする
			//	GetSound().BGMStop(BGMKind::TITLE);
			//	FRAMEWORK.scenegame.PadSet(select_p1);
			//	FRAMEWORK.SetScene(SCENE_SELECT);
			//	UnInit();
			//	return;
			//}

			if (cbuffer_param.curvature <= 0.0f)
			{
				if (FedoOut(elapsed_time))
				{
					//フェードアウトが終わったらセレクト画面へ
					//ここで入力を保存する
					select_p1 = scastI(INPUT_PLAYER::P1);
					select_p2 = scastI(INPUT_PLAYER::P2);
					FRAMEWORK.scenegame.PadSet(select_p1);
					GetSound().BGMStop(BGMKind::TITLE);
					FRAMEWORK.sceneselect.select_p1 = scastI(PLSELECT::KNIGHT);
					FRAMEWORK.sceneselect.select_p2 = scastI(PLSELECT::KNIGHT);
					//フェードアウトが終わったらロード画面へ
					FRAMEWORK.SetScene(SCENE_TABLE::SCENE_LOAD);
					UnInit();
					return;
				}
			}
		}
		else
		{
			//フェードアウトがスタートしてない場合は画面を映す
			if (fado_alpha > 0.0f)
			{
				fado_alpha -= FADE_MIX(elapsed_time);
			}
		}

	}
	else
	{

		switch (load_state)
		{
		case 0:
			if (1/*Fedo::getInstance().FedoIn()*/)
			{
				load_state = 1;
			}
			break;
		case 1:
			load_state = 2;
			t = new std::thread(&SceneTitle::LoadData, this);
		case 2:
			break;
		case 3:
			if (1/*Fedo::getInstance().FedoOut()*/)
			{
				load_state = 4;
				//pFedo.Set();
			}
			break;
		case 4:
			t->join();
			delete t;
			load_fin = true;
			break;
		default:
			break;
		}
	}
}

void SceneTitle::Draw(float elapsed_time)
{
#ifdef EXIST_IMGUI

	static float xx = 0.0f;
	static float yy = 0.0f;
	static float xxx = 0.0f;
	static float yyy = 0.0f;
	if (Get_Use_ImGui())
	{

		float zzz = (cbuffer_param.Resolution.x / cbuffer_param.Resolution.y);
		ImGui::SliderFloat("Reso.x", &cbuffer_param.Resolution.x, 0.0f, 19200.0f);
		ImGui::SliderFloat("Reso.y", &cbuffer_param.Resolution.y, 0.0f, 10800.0f);
		ImGui::SliderFloat("Reso.z", &zzz, 0.0f, 100.0f);
		ImGui::SliderFloat("brightness", &cbuffer_param.brightness, 0.0f, 100.0f);
		ImGui::SliderFloat("gamma", &cbuffer_param.gamma, 0.0f, 100.0f);
		ImGui::SliderFloat("spot_brightness", &cbuffer_param.spot_brightness, 0.0f, 100.0f);
		ImGui::SliderFloat("ray_density", &cbuffer_param.ray_density, 0.0f, 100.0f);
		ImGui::SliderFloat("curvature", &cbuffer_param.curvature, 0.0f, 200.0f);
		ImGui::SliderFloat("red", &cbuffer_param.red, 0.0f, 10.0f);
		ImGui::SliderFloat("green", &cbuffer_param.green, 0.0f, 10.0f);
		ImGui::SliderFloat("blue", &cbuffer_param.blue, 0.0f, 10.0f);

		ImGui::SliderFloat("material_color_x", &cbuffer_param.material_color.x, 0.0f, 1.0f);
		ImGui::SliderFloat("material_color_y", &cbuffer_param.material_color.y, 0.0f, 1.0f);
		ImGui::SliderFloat("material_color_z", &cbuffer_param.material_color.z, 0.0f, 1.0f);
		ImGui::SliderFloat("material_color_w", &cbuffer_param.material_color.w, 0.0f, 1.0f);


		ImGui::SliderFloat("xx", &xx, 0.0f, 1920.0f);
		ImGui::SliderFloat("yy", &yy, 0.0f, 1080.0f);
		ImGui::SliderFloat("xxx", &xxx, 0.0f, 1920.0f);
		ImGui::SliderFloat("yyx", &yyy, 0.0f, 1080.0f);

		if (ImGui::TreeNode(u8"カラーテクスチャ"))
		{
			ImGui::Image((void*)(color_texture->GetShaderResource()), ImVec2(360, 360));
			ImGui::Image((void*)(luminance_texture->GetShaderResource()), ImVec2(360, 360));
			ImGui::TreePop();
		}
		cbuffer_param.Resolution.z = zzz;
	}
#endif // USE_IMGUI

	if (load_fin)
	{
		SetRenderTexture();

		//test->DrawGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f);

		if (fado_start)
		{
			cbuffer_param.curvature -= elapsed_time*200.0f;
		}
		cbuffer_param.iTime = timer;

		sprite->render(
			titleShader.get(),
			title_texture.get(),
			cbuffer_param,
			sampler_wrap.get(),
			constantBuffer,
			0.0f, 0.0f, 1920.0f, 1080.0f,
			0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);

		title_img->DrawRotaGraph(
			spriteShader.get(),
			static_cast<float>(FRAMEWORK.SCREEN_WIDTH) / 2.0f,
			static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.3f,
			0.0f,
			0.3f
		);


		switch (state)
		{
		case SceneTitle::STATE::HOME:
			FRAMEWORK.fade_img->DrawExtendGraph(spriteShader.get(), 0.0f, 687.0f, 1920.0f, 845.0f, SpriteMask::NONE, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));

			choice_img->DrawRotaDivGraph(
				spriteShader.get(),
				static_cast<float>(FRAMEWORK.SCREEN_WIDTH) / 2.0f,
				static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f,
				0.0f,
				0.5f,
				0
			);
			choice_mask->DrawRotaDivGraph(
				spriteShader.get(),
				static_cast<float>(FRAMEWORK.SCREEN_WIDTH) / 2.0f,
				static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f,
				0.0f,
				0.5f,
				0,
				SpriteMask::WRITE
			);

			choice_anim->DrawExtendAnimGraph(spriteShader.get(), 485.0f, 691.0f, 1434.0f, 837.0f, -300.0f, elapsed_time, SpriteMask::INDRAW);
			break;
		case SceneTitle::STATE::SELECT:
			FRAMEWORK.fade_img->DrawExtendGraph(spriteShader.get(), 0.0f, 687.0f, 1920.0f, 845.0f, SpriteMask::NONE, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
			if (vs_mode == VS_MODE::CPU)
			{
				choice_img->DrawRotaDivGraph(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) * 0.3f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f,
					0.0f,
					0.3f,
					1
				);
				choice_img->DrawRotaDivGraph(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) * 0.7f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f,
					0.0f,
					0.3f,
					2,
					SpriteMask::NONE,
					DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 0.3f)
				);

				choice_mask->DrawRotaDivGraph(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) * 0.3f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f,
					0.0f,
					0.3f,
					1,
					SpriteMask::WRITE
				);
				choice_mask->DrawRotaDivGraph(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) * 0.7f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f,
					0.0f,
					0.3f,
					2,
					SpriteMask::WRITE
				);
				choice_anim->DrawExtendAnimGraph(spriteShader.get(), 314.0f, 691.0f, 833.0f, 826.0f, -300.0f, elapsed_time, SpriteMask::INDRAW);
			}
			else
			{
				choice_img->DrawRotaDivGraph(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) * 0.3f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f,
					0.0f,
					0.3f,
					1,
					SpriteMask::NONE,
					DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 0.3f)
				);
				choice_img->DrawRotaDivGraph(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) * 0.7f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f,
					0.0f,
					0.3f,
					2
				);

				choice_mask->DrawRotaDivGraph(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) * 0.3f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f,
					0.0f,
					0.3f,
					1,
					SpriteMask::WRITE
				);
				choice_mask->DrawRotaDivGraph(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) * 0.7f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f,
					0.0f,
					0.3f,
					2,
					SpriteMask::WRITE
				);
				choice_anim->DrawExtendAnimGraph(spriteShader.get(), 1011.0f, 691.0f, 1620.0f, 826.0f, -300.0f, elapsed_time, SpriteMask::INDRAW);
			}
			break;
		case SceneTitle::STATE::END:
			if (vs_mode == VS_MODE::CPU)
			{
				choice_img->DrawRotaDivGraph(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) * 0.3f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f,
					0.0f,
					0.3f,
					1
				);
				choice_img->DrawRotaDivGraph(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) * 0.7f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f,
					0.0f,
					0.3f,
					2,
					SpriteMask::NONE,
					DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 0.3f)
				);
				choice_mask->DrawRotaDivGraph(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) * 0.3f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f,
					0.0f,
					0.3f,
					1,
					SpriteMask::WRITE
				);
				choice_mask->DrawRotaDivGraph(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) * 0.7f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f,
					0.0f,
					0.3f,
					2,
					SpriteMask::WRITE
				);
				choice_anim->DrawExtendAnimGraph(spriteShader.get(), 314.0f, 691.0f, 833.0f, 826.0f, -300.0f, elapsed_time, SpriteMask::INDRAW);
			}
			else
			{
				choice_img->DrawRotaDivGraph(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) * 0.3f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f,
					0.0f,
					0.3f,
					1,
					SpriteMask::NONE,
					DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 0.3f)
				);
				choice_img->DrawRotaDivGraph(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) * 0.7f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f,
					0.0f,
					0.3f,
					2
				);
				choice_mask->DrawRotaDivGraph(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) * 0.3f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f,
					0.0f,
					0.3f,
					1,
					SpriteMask::WRITE
				);
				choice_mask->DrawRotaDivGraph(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) * 0.7f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f,
					0.0f,
					0.3f,
					2,
					SpriteMask::WRITE
				);
				choice_anim->DrawExtendAnimGraph(spriteShader.get(), 1011.0f, 691.0f, 1620.0f, 826.0f, -300.0f, elapsed_time, SpriteMask::INDRAW);
			}
			break;
		default:
			break;
		}


		//アイコン描画
		/*knight_icon->DrawRotaGraph(
			spriteShader.get(),
			p1_pos.x,
			p1_pos.y,
			0.0f,
			Rato
		);
		ken_icon->DrawRotaGraph(
			spriteShader.get(),
			p2_pos.x,
			p2_pos.y,
			0.0f,
			Rato
		);*/

		//選択画像描画
		//1P
		//if (p1Enter)
		//{
		//	select_img->DrawRotaGraph
		//	(
		//		spriteShader.get(),
		//		p1.x,
		//		p1.y,
		//		0.0f,
		//		Rato,
		//		DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
		//	);
		//}
		//else
		//{
		//	if (static_cast<int>(timer * 10.0f) % 5 > 0)
		//	{
		//		select_img->DrawRotaGraph
		//		(
		//			spriteShader.get(),
		//			p1.x,
		//			p1.y,
		//			0.0f,
		//			Rato,
		//			DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
		//		);
		//	}
		//}

		////2P
		//if (p2Enter)
		//{
		//	select_img->DrawRotaGraph
		//	(
		//		spriteShader.get(),
		//		p2.x,
		//		p2.y,
		//		0.0f,
		//		Rato,
		//		DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)
		//	);
		//}
		//else
		//{
		//	if (static_cast<int>(timer * 10.0f) % 5 > 0)
		//	{
		//		select_img->DrawRotaGraph
		//		(
		//			spriteShader.get(),
		//			p2.x,
		//			p2.y,
		//			0.0f,
		//			Rato,
		//			DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)
		//		);
		//	}
		//}

		// フォントの描画
		{
			/*FRAMEWORK.font->Begin(FRAMEWORK.context.Get());
			FRAMEWORK.font->Draw(static_cast<float>(FRAMEWORK.SCREEN_WIDTH / 2.0f)-150.0f, static_cast<float>(FRAMEWORK.SCREEN_HEIGHT)-100.0f, L"abcdefg");
			FRAMEWORK.font->Draw(500, 50, L"テストテキスト");
			FRAMEWORK.font->End(FRAMEWORK.context.Get());*/
		}

	}


#ifdef EXIST_IMGUI
	//ImGui
	if(Get_Use_ImGui())
	{
		ImGui::Text("title");
		ImGui::Text("time : %f",timer);
		ImGui::Text("load_state : %d", load_state);
	}
#endif
	if (load_fin)
	{
		NullSetRenderTexture();
		RenderTexture(elapsed_time);
		FRAMEWORK.framebuffer.Deactivate();
		//framebuffer::ResetRenderTargetViews();

		//SceneRender(elapsed_time);
	}
	FRAMEWORK.fade_img->DrawRotaGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 1.0f, false, SpriteMask::NONE, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, fado_alpha));
}


bool SceneTitle::FedoOut(float elapsed_time)
{
	fado_alpha += FADE_MIX(elapsed_time);

	if (fado_alpha > 1.0f)
	{
		return true;
	}

	return false;
}

YR_Vector3 SceneTitle::PosSet(int select)
{
	switch (select)
	{
	case scastI(INPUT_PLAYER::P1):
		return p1_pos;
		break;
	case scastI(INPUT_PLAYER::P2):
		return p2_pos;
		break;
	case scastI(INPUT_PLAYER::NONE):
		return none_pos;
		break;
	}
	return YR_Vector3(0.0f, 0.0f);
}


void SceneTitle::SetRenderTexture()
{
	FRAMEWORK.framebuffer.GetDefaultRTV();
	FRAMEWORK.framebuffer.SetRenderTexture(color_texture->GetRenderTargetView());
	//FRAMEWORK.framebuffer.SetRenderTexture(normal_texture->GetRenderTargetView());
	//FRAMEWORK.framebuffer.SetRenderTexture(position_texture->GetRenderTargetView());
	FRAMEWORK.framebuffer.SetRenderTexture(luminance_texture->GetRenderTargetView());

	ID3D11DepthStencilView* dsv = color_texture->GetDepthStencilView();

	//画面クリア
	FRAMEWORK.framebuffer.Clear();
	FRAMEWORK.context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//ビュー更新
	YRCamera.Active();

	//ビューポート設定
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
}


void SceneTitle::NullSetRenderTexture()
{
	//レンダーターゲットの回復
	//FRAMEWORK.context.Get()->OMSetRenderTargets(testrtv.size(), testrtv.data(), FRAMEWORK.depth.Get());
	FRAMEWORK.framebuffer.ResetRenderTexture();
	FRAMEWORK.framebuffer.SetDefaultRTV();
}


void SceneTitle::RenderTexture(float elapsed_time)
{
	//Gbuffer描画
	sprite->render(
		spriteEx.get(),
		color_texture.get(),
		0.0f, 0.0f, 1920.0f, 1080.0f,
		0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);
}