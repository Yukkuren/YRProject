#include "Scene.h"
#include "../framework.h"
#include <stdlib.h>
#include <time.h>
#include "../YRMouse.h"
#include "../Game/HitCheak.h"
#include "../Game/World.h"
#include "../Blur.h"
#include "../YRSound.h"

//キャラクターインクルード
#include "../Chara/Knight/Knight.h"
#include "../Chara/Neru/Neru.h"

//------------------------------------------------
// **シーン概要**
//・このシーンの処理でプレイヤーモデルのロードは行わない
//
//------------------------------------------------

#ifdef EXIST_IMGUI

std::array<std::string, scastI(SceneGame::Player2PControl::END)> p2_con_name_list =
{
	u8"操作",
	u8"動かない",
	u8"AI",
	u8"無敵操作不可",
};

std::array<std::string, scastI(AI_Controller::AI_State::END)> p2_AI_state_list =
{
	u8"最初の設定",
	u8"ランダムで行動を決定する",
	u8"近づく",
	u8"離れる",
	u8"Xボタンを連打する",
	u8"ホーミングダッシュを行う",
	u8"ガードしようとする",
	u8"のけぞった",
	u8"体力がなくなった",
	u8"ダウン中"
};

std::array<std::string, scastI(Camera::CAMERA_STATE::END_GAME)+1> camera_state_list =
{
	u8"メインカメラ",
	u8"プレイヤー1P",
	u8"プレイヤー2P",
	u8"ズームカメラ",
	u8"決着時",
};

std::array<std::string, scastI(Camera::Request::ZOOM)+1> camera_request_list =
{
	u8"なし",
	u8"カメラをつかんでいる",
	u8"カメラを離した",
	u8"持つ手を弱めている",
	u8"ズームさせる",
};


#endif // USE_IMGUI


void SceneGame::SetPlayerCharacter(std::unique_ptr<Player>* player, int select)
{
	switch (select)
	{
	case scastI(PLSELECT::KNIGHT):
		*player = std::make_unique<Knight>();
		break;
	case scastI(PLSELECT::NERU):
		*player = std::make_unique<Neru>();
		break;
	default:
		assert("select is broken");
		break;
	}
	player->get()->chara_name = static_cast<PLSELECT>(select);
}


void SceneGame::Init()
{
	//ゲームループ管理系初期化
	timer				= 0.0f;
	fado_alpha			= 1.0f;
	fado_start			= false;
	pause				= false;
	start				= false;
	Hitcheak::timer = 0.0f;
	Hitcheak::hit		= false;
	Hitcheak::stop1p	= false;
	Hitcheak::stop2p	= false;
	judge				= JUDGE_VICTORY::NO_VICTORY;
	start_timer			= 0.0f;
	end					= false;
	endtimer			= 0.0f;
	mix_fade			= 1.5f;
	roll_timer			= -1.0f;	//回転の中心がずれる為マイナスの値から始めている
	main_loop = MAIN_LOOP::INTRO1P;	//最初は1Pのイントロから開始

	image_alpha = 0.0f;
	image_size = 5.0f;

	//ディザスタル関係の変数を初期化
	desastal_size_p1 = 10.0f;
	desastal_size_p2 = 10.0f;
	desastal_alpha_p1 = 0.0f;
	desastal_alpha_p2 = 0.0f;

	//初期位置Xは画面外に
	Danger_pos_p1 = { -500.0f,265.0f };
	Danger_pos_p2 = { (float)FRAMEWORK.SCREEN_WIDTH + 500.0f,265.0f };

	Danger_alpha = 1.0f;
	Danger_size = 0.5f;

	Scene_End_eye = YR_Vector3( 0.0f,0.0f,0.0f );
	fin_camera_state = FIN_CAMERA_STATE::ROLL;

	//カメラ初期設定
	StartSet();
	YRCamera.camera_state = Camera::CAMERA_STATE::MAIN;

	//画像選択位置初期化
	for (int i = 0; i < static_cast<int>(p1combo.size()); i++)
	{
		p1combo[i] = 0;
		p2combo[i] = 0;
	}


	//深度テスト切り替え用のステンシルステートを作成
	D3D11_DEPTH_STENCIL_DESC desc2;
	::memset(&desc2, 0, sizeof(desc2));
	desc2.DepthEnable = FALSE;
	desc2.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc2.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	HRESULT hr = FRAMEWORK.device.Get()->CreateDepthStencilState(&desc2, m_depth_stencil_state2.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_DEPTH_STENCIL_DESC desc;
	::memset(&desc, 0, sizeof(desc));
	desc.DepthEnable = TRUE;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = FRAMEWORK.device.Get()->CreateDepthStencilState(&desc, m_depth_stencil_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	/*motion.MeshSet(box);
	motion.AnimReset();*/

	p1_elapsed_time = 1.0f;
	p2_elapsed_time = 1.0f;
	camera_move_debug = false;
	hit_stop_elapsed = 0.0f;
	game_speed = 0.0f;
	blur_on = true;

	//2Pの動き
	//pl2_con = Player2PControl::OPERATION;

	GetSound().BGMPlay(BGMKind::GAME);

	AI2P.Init();

	input_pad.Init();

	cbuffer_param.Resolution = { 1920.0f,1080.0f,1920.0f / 1080.0f };
	cbuffer_param.brightness = 15.0f;
	//cbuffer_param.brightness = 3.0f;
	cbuffer_param.gamma = 13;
	//cbuffer_param.gamma = 6;
	cbuffer_param.spot_brightness = 1.5f;
	cbuffer_param.ray_density = 1.5f;
	//cbuffer_param.ray_density = 6.0f;
	cbuffer_param.curvature = 90.0f;
	cbuffer_param.red = 10.0f;
	cbuffer_param.green = 2.8f;
	cbuffer_param.blue = 4.0f;
	cbuffer_param.material_color = { 1.0f,1.0f,1.0f,1.0f };
	cbuffer_param.dummy1 = 0.0f;
	cbuffer_param.dummy2 = 0.0f;
	cbuffer_param.dummy3 = 0.0f;
	//UnInit();
	//FRAMEWORK.SetScene(SCENE_TABLE::SCENE_TITLE);
}







void SceneGame::LoadData()
{
	//モデル等のロード
	//この関数はSceneLoadで別スレッドとして動かす
	if (geo == nullptr)
	{
		geo = std::make_unique<geometric_primitive>();
	}

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
	if (ParallelToonShader == nullptr)
	{
		ParallelToonShader = std::make_unique<YRShader>(ShaderType::TOON);
		ParallelToonShader->Create("./Data/Shader/ParallelToon_vs.cso", "./Data/Shader/ParallelToon_ps.cso", "./Data/Shader/ParallelToon_gs.cso");
	}
	if (ToonShader == nullptr)
	{
		ToonShader = std::make_unique<YRShader>(ShaderType::TOON);
		ToonShader->Create("./Data/Shader/ToonShader_vs.cso", "./Data/Shader/ToonShader_ps.cso", "./Data/Shader/ToonShader_gs.cso");
	}
	if (animShader == nullptr)
	{
		animShader = std::make_unique<YRShader>(ShaderType::ANIM);
		animShader->Create("./Data/Shader/AnimShader_vs.cso", "./Data/Shader/AnimShader_ps.cso", "./Data/Shader/AnimShader_gs.cso");
	}

	//コンスタントバッファ作成
	FRAMEWORK.CreateConstantBuffer(constantBuffer.GetAddressOf(), sizeof(GaussParamManager::GaussBlurParam));

	//テクスチャロード
	if (color_texture == nullptr)
	{
		color_texture = std::make_unique<Texture>();
		color_texture->Create(1920, 1080, DXGI_FORMAT_R8G8B8A8_UNORM);
		color_texture->CreateDepth(1920, 1080, DXGI_FORMAT_R24G8_TYPELESS);
	}
	if (UI_texture == nullptr)
	{
		UI_texture = std::make_unique<Texture>();
		UI_texture->Create(1920, 1080, DXGI_FORMAT_R8G8B8A8_UNORM);
		UI_texture->CreateDepth(1920, 1080, DXGI_FORMAT_R24G8_TYPELESS);
	}
	if (HP_texture == nullptr)
	{
		HP_texture = std::make_unique<Texture>();
		HP_texture->Create(1920, 1080, DXGI_FORMAT_R8G8B8A8_UNORM);
		HP_texture->CreateDepth(1920, 1080, DXGI_FORMAT_R24G8_TYPELESS);
	}
	/*if (normal_texture == nullptr)
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
	}*/
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
			blur_texture[i]->Create(960.0f/riv, 540.0f/riv, DXGI_FORMAT_R16G16B16A16_FLOAT);
			blur_texture[i]->CreateDepth(960.0f / riv, 540.0f/riv, DXGI_FORMAT_R24G8_TYPELESS);
		}
	}
	if (multi_blur_texture == nullptr)
	{
		multi_blur_texture = std::make_unique<Texture>();
		multi_blur_texture->Create(1920, 1080, DXGI_FORMAT_R16G16B16A16_FLOAT);
		multi_blur_texture->CreateDepth(1920, 1080, DXGI_FORMAT_R24G8_TYPELESS);
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


	//画像のロード
	if (test == nullptr)
	{
		test = std::make_unique<Sprite>(L"./Data/Image/BG/stage1.png", 3840.0f, 2160.0f);
	}
	if (HP_img == nullptr)
	{
		HP_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/HP.png", 800.0f, 100.0f);
	}
	if (HPDamagebar_img == nullptr)
	{
		HPDamagebar_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/HP_Damage.png", 800.0f, 100.0f);
	}
	if (win1P_img == nullptr)
	{
		win1P_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/1PWIN.png", 960.0f, 384.0f);
	}
	if (win2P_img == nullptr)
	{
		win2P_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/2PWIN.png", 960.0f, 384.0f);
	}
	if (draw_img == nullptr)
	{
		draw_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/DRAW.png", 960.0f, 384.0f);
	}
	if (HPbar_img == nullptr)
	{
		HPbar_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/HPbar.png", 850.0f, 150.0f);
	}
	if (KO_img == nullptr)
	{
		KO_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/KO.png", 640.0f, 384.0f);
	}
	if (font_img == nullptr)
	{
		font_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/font.png", 2048.0f, 256.0f, 10, 1, 204.8f, 256.0f);
	}
	if (combo_img == nullptr)
	{
		combo_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/combo.png", 1280.0f, 360.0f);
	}
	if (desastal_case == nullptr)
	{
		desastal_case = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/desastal_case.png", 640.0f, 640.0f);
	}
	if (desastal_flash == nullptr)
	{
		desastal_flash = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/desastal_flash.png", 640.0f, 640.0f);
	}
	if (desastal_img == nullptr)
	{
		desastal_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/desastal.png", 1920.0f, 1280.0f, 3, 2, 640.0f, 640.0f);
	}
	if (call_img == nullptr)
	{
		call_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/call.png", 1024.0f, 512.0f, 1, 2, 1024.0f, 256.0f);
	}
	if (effect_img == nullptr)
	{
		effect_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/effect.png", 192.0f, 128.0f, 3, 2, 64.0f, 64.0f);
	}
	if (pause_img == nullptr)
	{
		pause_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/pause.png", 384.0f, 128.0f);
	}
	if (Danger_img == nullptr)
	{
		Danger_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/DANGER.png", 1280.0f, 360.0f);
	}
	if (p1_icon_img == nullptr)
	{
		p1_icon_img = std::make_shared<Texture>(L"./Data/Image/UI/GameScene/icon1.png");
	}
	if (p2_icon_img == nullptr)
	{
		p2_icon_img = std::make_shared<Texture>(L"./Data/Image/UI/GameScene/icon2.png");
	}
	if (arrow_icon_img == nullptr)
	{
		arrow_icon_img = std::make_shared<Texture>(L"./Data/Image/UI/GameScene/arrow_icon.png");
	}
	if (p1_icon_board == nullptr)
	{
		p1_icon_board = std::make_unique<AnimBoard>(p1_icon_img, 1, DirectX::XMFLOAT2(640.0f, 640.0f), DirectX::XMINT2(1, 1), DirectX::XMFLOAT2(640.0f, 640.0f));
	}
	if (p2_icon_board == nullptr)
	{
		p2_icon_board = std::make_unique<AnimBoard>(p2_icon_img, 1, DirectX::XMFLOAT2(640.0f, 640.0f), DirectX::XMINT2(1, 1), DirectX::XMFLOAT2(640.0f, 640.0f));
	}
	if (arrow_icon_board == nullptr)
	{
		arrow_icon_board = std::make_unique<AnimBoard>(arrow_icon_img, 1, DirectX::XMFLOAT2(640.0f, 640.0f), DirectX::XMINT2(1, 1), DirectX::XMFLOAT2(640.0f, 640.0f));
	}

	if (gauge_img == nullptr)
	{
		gauge_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/gauge_img.png", 640.0f, 64.0f);
	}
	if (gauge_anim1p == nullptr)
	{
		gauge_anim1p = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/gauge_anim.png", 640.0f, 64.0f);
	}
	if (gauge_anim2p == nullptr)
	{
		gauge_anim2p = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/gauge_anim.png", 640.0f, 64.0f);
		gauge_anim2p->sx = gauge_anim2p->sw;
	}
	if (gauge_case_img == nullptr)
	{
		gauge_case_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/gauge_case_img.png", 704.0f, 128.0f);
	}
	if (gauge_case_mask == nullptr)
	{
		gauge_case_mask = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/gauge_case_mask.png", 704.0f, 128.0f);
	}


	if (HPbar_base == nullptr)
	{
		HPbar_base = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/HPBar_base.png", 800.0f, 100.0f);
	}
	if (HPbar_fedo == nullptr)
	{
		HPbar_fedo = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/HPBar_fedo.png", 800.0f, 100.0f);
	}
	if (HPbar_design == nullptr)
	{
		HPbar_design = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/HPBar_design.png", 800.0f, 100.0f);
	}
	if (HPbar_case == nullptr)
	{
		HPbar_case = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/HPcase.png", 896.0f, 192.0f);
	}
	if (HPbar_mask == nullptr)
	{
		HPbar_mask = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/HPcase_mask.png", 896.0f, 192.0f);
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
	if (spriteEx == nullptr)
	{
		spriteEx = std::make_unique<YRShader>(ShaderType::SPRITE_EX);
		spriteEx->Create("./Data/Shader/SpriteEx_vs.cso", "./Data/Shader/SpriteEx_ps.cso");
	}

	if (concentrationShader == nullptr)
	{
		concentrationShader = std::make_unique<YRShader>(ShaderType::TITLE);
		concentrationShader->Create("./Data/Shader/ConcentrationShader_vs.cso", "./Data/Shader/ConcentrationShader_ps.cso");
	}


	input_pad.Load();

	//ステージの選択
	stage.Init(Stage::StageType::NORMAL);


	//選択したキャラクターをそれぞれ生成する
	SetPlayerCharacter(&player1p, FRAMEWORK.sceneselect.select_p1);
	SetPlayerCharacter(&player2p, FRAMEWORK.sceneselect.select_p2);
	player1p->now_player = 1;
	player2p->now_player = 2;
	//生成後初期化する(座標系、HP、UI座標など)
	player1p->LoadData(FRAMEWORK.sceneselect.color_p1);
	player2p->LoadData(FRAMEWORK.sceneselect.color_p2);

	PL.pos1P = { -11.0f,0.0f };
	PL.pos2P = { 11.0f,0.0f };

	player1p->Init(PL.pos1P);
	player2p->Init(PL.pos2P);
	//キャラにどのプレイヤーが操作しているかの情報を与える

	PL.HP_MAX1P = player1p->hp;
	PL.HP_MAX2P = player2p->hp;
	PL.ratio1P = player1p->hp / PL.HP_MAX1P * 800.0f;
	PL.ratio2P = player2p->hp / PL.HP_MAX2P * 800.0f;
	PL.correction_value = 800.0f - PL.ratio1P;
	//タイトルで決定したパッドの順番をこちらにも設定して初期化
	PadSet(FRAMEWORK.scenetitle.select_p1, FRAMEWORK.scenetitle.select_p2);
	player1p->pad->Init();
	player2p->pad->Init();

	//プレイヤーの向きを指定しておく
	player1p->rightOrleft = 1;
	player2p->rightOrleft = -1;

	pl1_before_hp = player1p->hp;
	pl2_before_hp = player2p->hp;

	pl1_before_power = player1p->power;
	pl2_before_power = player2p->power;

	//ロード終了
	FRAMEWORK.sceneload.load_state = 7;
}






void SceneGame::UnInit()
{
	//プレイヤーのUninit関数を回す
	player1p->Uninit();
	player1p.reset();
	player2p->Uninit();
	player2p.reset();
	//player2p = nullptr;
	//SceneGameの画像などを解放する
	test.reset();
	geo.reset();
	HP_img.reset();
	win1P_img.reset();
	win2P_img.reset();
	draw_img.reset();
	HPbar_img.reset();
	KO_img.reset();
	font_img.reset();
	call_img.reset();
	effect_img.reset();
	pause_img.reset();
	HPDamagebar_img.reset();
	gauge_img.reset();
	gauge_case_img.reset();
	gauge_case_mask.reset();
	gauge_anim1p.reset();
	gauge_anim2p.reset();
	HPbar_base.reset();
	HPbar_case.reset();
	HPbar_mask.reset();
	HPbar_fedo.reset();
	HPbar_design.reset();
	desastal_case.reset();
	desastal_img.reset();

	test = nullptr;
	geo = nullptr;
	HP_img = nullptr;
	win1P_img = nullptr;
	win2P_img = nullptr;
	draw_img = nullptr;
	HPbar_img = nullptr;
	KO_img = nullptr;
	font_img = nullptr;
	call_img = nullptr;
	effect_img = nullptr;
	pause_img = nullptr;
	HPDamagebar_img = nullptr;
	gauge_img = nullptr;
	gauge_case_img = nullptr;
	gauge_case_mask = nullptr;
	gauge_anim1p = nullptr;
	gauge_anim2p = nullptr;
	HPbar_base = nullptr;
	HPbar_case = nullptr;
	HPbar_mask = nullptr;
	HPbar_fedo = nullptr;
	HPbar_design = nullptr;
	desastal_case = nullptr;
	desastal_img = nullptr;

	//シェーダー解放
	spriteShader.reset();
	spriteShader = nullptr;
	skinShader.reset();
	skinShader = nullptr;
	geoShader.reset();
	geoShader = nullptr;
	ParallelToonShader.reset();
	ParallelToonShader = nullptr;
	ToonShader.reset();
	ToonShader = nullptr;

	sampler_wrap.reset();
	sampler_wrap = nullptr;
	sampler_clamp.reset();
	sampler_clamp = nullptr;

	input_pad.UnInit();

	//BGMを止める
	GetSound().BGMStop(BGMKind::GAME);
	GetSound().BGMStop(BGMKind::KNIGHT_WIN);

	//ステージ解放
	stage.Uninit();
}





void SceneGame::StartSet()
{
	//イントロ終了後のゲーム画面のセット

	//カメラ初期座標
	Start_Scene_eye = YR_Vector3(0.0f, 6.0f, -140.0f);
	Start_Scene_focus = YR_Vector3(0.0f, 6.0f, 0.0f);

	Scene_eye = Start_Scene_eye;
	Scene_focus = Start_Scene_focus;
	Scene_up = YR_Vector3(0.0f, 1.0f, 0.0f);
	Scene_fov = 10.0f * 0.01745f;
	Scene_aspect = 1920.0f / 1080.0f;
	Scene_nearZ = 1.4f;
	Scene_farZ = 100000.0f;

	//カメラ初期設定
	YRCamera.SetEye(Scene_eye.GetDXFLOAT3());			//視点
	YRCamera.SetFocus(Scene_focus.GetDXFLOAT3());			//注視点
	YRCamera.SetUp(Scene_up.GetDXFLOAT3());				//上方向
	YRCamera.SetPerspective(Scene_fov, Scene_aspect, Scene_nearZ, Scene_farZ);
	//YRCamera.SetPerspective(1080.0f, 1920.0f, 0.0001f, 1000000.0f);

	Limit::First_Camera_set(Start_Scene_eye);
}





void SceneGame::FinSet()
{
	//ゲーム終了後のゲーム画面のセット
	//カメラ初期設定
	YRCamera.SetEye(DirectX::XMFLOAT3(0, 0, -140));			//視点
	YRCamera.SetFocus(DirectX::XMFLOAT3(0, 0, 0));			//注視点
	YRCamera.SetUp(DirectX::XMFLOAT3(0, 1, 0));				//上方向
	YRCamera.SetPerspective(30 * 0.01745f, 1280.0f / 720.0f, 0.4f, 1000000.0f);
}







void SceneGame::Update(float elapsed_time)
{
	//基本的にそのままの速度を代入する
	game_speed = elapsed_time;

	if (fado_start)
	{
		//フェードアウト中
		switch (main_loop)
		{
			//イントロを飛ばす場合はゲームメインへ
		case MAIN_LOOP::INTRO1P:
			fado_alpha += (elapsed_time * mix_fade);
			if (fado_alpha > 1.0f)
			{
				main_loop = MAIN_LOOP::INTRO2P;
				fado_start = false;
			}
			break;
		case MAIN_LOOP::INTRO2P:
			fado_alpha += (elapsed_time * mix_fade);
			if (fado_alpha > 1.0f)
			{
				main_loop = MAIN_LOOP::READY;
				YRCamera.camera_state = Camera::CAMERA_STATE::MAIN;
				StartSet();
				player1p->ReadySet();
				player2p->ReadySet();
				fado_start = false;
				YR_Vector3 camera_screen = Limit::Set(player1p->pos, player2p->pos, Start_Scene_eye);
				Scene_eye.x = camera_screen.x;
				Scene_eye.y = camera_screen.y;
				Scene_eye.z = camera_screen.z;
				Scene_focus.x = camera_screen.x;
				Scene_focus.y = camera_screen.y;

				//カメラの挙動をステートごとに処理
				CameraUpdate(elapsed_time);
			}
			break;
		case MAIN_LOOP::READY:
			break;
		case MAIN_LOOP::MAIN:
			/*if (FadeOut(elapsed_time))
			{
				UnInit();
				FRAMEWORK.SetScene(SCENE_TABLE::SCENE_SELECT);
			}*/
			break;
		case MAIN_LOOP::FINISH:
			fado_alpha += (elapsed_time * 5.0f);
			if (fado_alpha > 1.0f)
			{
				GetSound().BGMStop(BGMKind::GAME);
				switch (judge)
				{
				case JUDGE_VICTORY::NO_VICTORY:
					break;
				case JUDGE_VICTORY::VICTORY1P:
					//ここで勝利画面設定関数を呼ぶ
					player1p->WinAnimSet();
					GetSound().BGMPlay(BGMKind::KNIGHT_WIN);
					break;
				case JUDGE_VICTORY::VICTORY2P:
					//ここで勝利画面設定関数を呼ぶ
					player2p->WinAnimSet();
					GetSound().BGMPlay(BGMKind::KNIGHT_WIN);
					break;
				case JUDGE_VICTORY::DRAW:
					//ここで勝利画面設定関数を呼ぶ

					break;
				default:
					break;
				}
				mix_fade = 5.0f;
				fado_start = false;
			}
			break;
		case MAIN_LOOP::WIN1P:
		case MAIN_LOOP::WIN2P:
		case MAIN_LOOP::DRAW:
			if (FadeOut(elapsed_time))
			{
				fado_start = false;
				main_loop = MAIN_LOOP::GAME_FIN;
				//ここでゲーム終了後の画面設定をする
				FinSet();
			}
			break;
		case MAIN_LOOP::GAME_FIN:
			if (FadeOut(elapsed_time))
			{
				UnInit();
				FRAMEWORK.SetScene(SCENE_TABLE::SCENE_TITLE);
			}
			break;
		default:
			break;
		}
	}
	else
	{
		//----------------------------------------------------------------
		//
		//					メインループ
		//
		//----------------------------------------------------------------
		switch (main_loop)
		{
		case MAIN_LOOP::INTRO1P:
			//プレイヤー1のイントロ
			YRCamera.camera_state = Camera::CAMERA_STATE::PLAYER1P;
			//パッドの更新
			player1p->pad->Update(game_speed);
			player2p->pad->Update(game_speed);
			//if (player1p->pad->x_input[scastI(PAD::START)] == 1 || player2p->pad->x_input[scastI(PAD::START)] == 1)
			//{
			//	//ポーズボタンが押された
			//	pause = !pause;
			//}

			//1Pイントロ更新
			/*if (pause)
			{
				game_speed = 0.0f;
			}*/
			if (fado_alpha > 0.8f)
			{
				game_speed = 0.0f;
			}
			if (player1p->Intro(game_speed))
			{
				main_loop = MAIN_LOOP::INTRO1P;
				fado_start = true;
			}

			//途中ボタンが押されたときはスキップ
			for (int i = 0; i < any_button.size(); i++)
			{
				if (player1p->pad->x_input[scastI(any_button[i])] == 1 || player2p->pad->x_input[scastI(any_button[i])] == 1)
				{
					fado_start = true;
					GetSound().SEStop(SEKind::INTRO_WIND);
				}
			}
			break;
		case MAIN_LOOP::INTRO2P:
			//プレイヤー2のイントロ
			YRCamera.camera_state = Camera::CAMERA_STATE::PLAYER2P;
			//パッドの更新
			player1p->pad->Update(game_speed);
			player2p->pad->Update(game_speed);

			if (fado_alpha > 0.8f)
			{
				game_speed = 0.0f;
			}

			//2Pイントロ更新
			if (player2p->Intro(game_speed))
			{
				fado_start = true;
				main_loop = MAIN_LOOP::INTRO2P;
			}
						//途中ボタンが押されたらスキップ
			for (int i = 0; i < any_button.size(); i++)
			{
				if (player1p->pad->x_input[scastI(any_button[i])] == 1 || player2p->pad->x_input[scastI(any_button[i])] == 1)
				{
					fado_start = true;
					main_loop = MAIN_LOOP::INTRO2P;
					GetSound().SEStop(SEKind::INTRO_WIND);
					player1p->pad->Init();
					player2p->pad->Init();
				}
			}
			break;
		case MAIN_LOOP::READY:
			//イントロ終了後のフェードイン
			mix_fade = 3.0f;
			if (fado_alpha < 0.1f)
			{
				main_loop = MAIN_LOOP::MAIN;
			}
			break;
		case MAIN_LOOP::MAIN:
			if (start)
			{
				//イントロがすべて終わり、カウントも終えゲームが開始された
				if (!end)
				{
					//パッド更新
					player1p->pad->Update(game_speed);
					Control2PState(game_speed);
#ifdef EXIST_IMGUI
					//デバッグ用即死技解禁コマンド
					//↓このコメントを解除してゲーム中でoキーを押すと弱攻撃が即死になる
					//if (Get_Use_ImGui())
					//{
					//	if (pKeyState.oflg == 1)
					//	{
					//		player1p->attack_list[scastI(AttackState::JAKU)].attack_single[0].parameter[0].damege = 1000.0f;
					//		//player2p->attack_list[scastI(AttackState::JAKU)].attack_single[0].parameter[0].damege = 1000.0f;
					//		player1p->pad->x_input[scastI(PAD::X)] = 1;
					//		//player2p->pad->x_input[scastI(PAD::X)] = 1;
					//	}
					//}
#endif
				}
				if (pause)
				{
					//ポーズ中
					PauseUpdate();
				}
				else
				{
					//対戦中
					timer += game_speed;

					//カメラリクエスト更新
					CameraRequest(game_speed);

					//カメラの挙動をステートごとに処理
					CameraUpdate(elapsed_time);

					//プレイヤー押出し
					if (player1p->act_state != ActState::STATENONE && player2p->act_state != ActState::STATENONE)
					{
						//プレイヤーのステートが奪われた状態以外は押し出しを行う
						Hitcheak::HitPlayer(player1p->hit, player1p->pos, player2p->hit, player2p->pos, game_speed, old_pos_p1, old_pos_p2);
					}

					//攻撃同士の判定
					Hitcheak::HitCheakAttack(player1p->atk,player2p->atk);
					player1p->GaugeUp(Hitcheak::add1P);
					player2p->GaugeUp(Hitcheak::add2P);

					Hitcheak::HitCheakAttack(player1p->projectile_atk, player2p->atk);
					player1p->GaugeUp(Hitcheak::add1P);
					player2p->GaugeUp(Hitcheak::add2P);

					Hitcheak::HitCheakAttack(player1p->atk, player2p->projectile_atk);
					player1p->GaugeUp(Hitcheak::add1P);
					player2p->GaugeUp(Hitcheak::add2P);

					Hitcheak::HitCheakAttack(player1p->projectile_atk, player2p->projectile_atk);
					player1p->GaugeUp(Hitcheak::add1P);
					player2p->GaugeUp(Hitcheak::add2P);

					//攻撃と当たり判定の判定
					Hitcheak::add1P = Hitcheak::HitCheak(player1p->atk, player2p->hit, 2,player2p->pos);
					Hitcheak::add2P = Hitcheak::HitCheak(player2p->atk, player1p->hit, 1,player1p->pos);
					player1p->GaugeUp(Hitcheak::add1P);
					player2p->GaugeUp(Hitcheak::add2P);

					Hitcheak::add1P = Hitcheak::HitCheak(player1p->projectile_atk, player2p->hit, 2, player2p->pos);
					Hitcheak::add2P = Hitcheak::HitCheak(player2p->projectile_atk, player1p->hit, 1, player1p->pos);
					player1p->GaugeUp(Hitcheak::add1P);
					player2p->GaugeUp(Hitcheak::add2P);

					/*if (Hitcheak::hei == 1)
					{
						player1p->pad->x_input[scastI(PAD::A)] = 1;
						player1p->projectile_atk.back().pos.x -= (player1p->projectile_atk.back().plus_speed.x * elapsed_time);
						Hitcheak::hei++;
					}*/

					//ヒットストップ処理
					if (Hitcheak::hit)
					{
						game_speed = 0.0f;
						Hitcheak::timer -= elapsed_time;
						hit_stop_elapsed += elapsed_time;
						if (player1p->pad->x_input[scastI(PAD::START)] == 1 || player2p->pad->x_input[scastI(PAD::START)] == 1)
						{
							//ポーズボタンが押された
							pause = true;
						}
						if (Hitcheak::timer < 0.0f)
						{
							//ヒットストップ終了
							Hitcheak::clash = false;
							Hitcheak::hit = false;
							Hitcheak::timer = 0.0f;
							player1p->StopEnd();
							player2p->StopEnd();
							Hitcheak::stop1p = false;
							Hitcheak::stop2p = false;
							player1p->stop_state = 0;
							player2p->stop_state = 0;
							YRCamera.RequestCamera(Camera::Request::RELEASE, 0);
							//カメラリクエスト更新
							CameraRequest(game_speed);
						}
						if (Hitcheak::timer != 0.0f)
						{
							/*PlayerALL::player1p->StopUpdate();
							PlayerALL::player2p->StopUpdate();*/
							YRGetEffect().DamageUpdate();
							if (hit_stop_elapsed > 0.01f)
							{
								//ヒットストップ中
								if (Hitcheak::stop1p)
								{
									player1p->StopUpdate();
								}
								if (Hitcheak::stop2p)
								{
									player2p->StopUpdate();
								}
								player1p->StopHitParamUpdate();
								player2p->StopHitParamUpdate();
								hit_stop_elapsed = 0.0f;
							}
							if (player1p->pad->x_input[scastI(PAD::START)] == 1 || player2p->pad->x_input[scastI(PAD::START)] == 1)
							{
								//ポーズボタンが押された
								pause = true;
							}
							return;
						}
					}
					if (!Hitcheak::hit && Hitcheak::timer > 0.0f)
					{
						Hitcheak::hit = true;
					}

					//カメラ座標設定
					EndORGameCameraSet(elapsed_time);

					//ホーミングダッシュ用の値を変更する
					TrackSet();

					if (end)
					{
						/*if (pKeyState.pflg == 1)
						{
							pause = !pause;
						}*/

						//勝敗がついた
						endtimer += elapsed_time;

						//勝敗決定後、静止する時間を作りたいので
						if (endtimer > end_slow_time)
						{
							game_speed = elapsed_time;
							//エフェクト更新
							YRGetEffect().Update();
						}
						else
						{
							game_speed = 0.0f;
						}

						//大体7秒後くらいに勝利画面へ
						if (endtimer > game_end_time)
						{
							main_loop = MAIN_LOOP::FINISH;
							fado_start = true;
							if (judge == JUDGE_VICTORY::DRAW)
							{
								main_loop = MAIN_LOOP::DRAW;
								timer = 0.0f;
							}
						}

						//プレイヤー更新(KO時のｳﾜｧ...ｳﾜｧ...ｳﾜｧ...ってスローになるやつ)
						//1Pが左
						if (player1p->pos.x < player2p->pos.x)
						{
							player1p->Update(1.0f, game_speed);
							player2p->Update(-1.0f, game_speed);
						}
						//2Pが左
						else
						{
							player1p->Update(-1.0f, game_speed);
							player2p->Update(1.0f, game_speed);
						}
						//プレイヤーの移動距離制限
						Limit::Stop(player1p->pos.x, player2p->pos.x);
					}
					else
					{
						//勝敗がついていない

						//エフェクト更新
						YRGetEffect().Update();

						if (player1p->pad->x_input[scastI(PAD::START)] == 1 || player2p->pad->x_input[scastI(PAD::START)] == 1)
						{
							//ポーズボタンが押された
							pause = true;
						}

						//プレイヤー更新
						//1Pが左
						float pl1_rightorleft = 0.0f;
						float pl2_rightorleft = 0.0f;
						if (player1p->pos.x < player2p->pos.x)
						{
							pl1_rightorleft = 1.0f;
							pl2_rightorleft = -1.0f;
						}
						//2Pが左
						else
						{
							pl1_rightorleft = -1.0f;
							pl2_rightorleft = 1.0f;
						}

						//座標を保存する
						old_pos_p1 = player1p->pos;
						old_pos_p2 = player2p->pos;

						switch (YRCamera.camera_state)
						{
						case Camera::CAMERA_STATE::MAIN:
							p1_elapsed_time = 1.0f;
							p2_elapsed_time = 1.0f;
							player1p->Update(pl1_rightorleft, game_speed * p1_elapsed_time);
							player2p->Update(pl2_rightorleft, game_speed* p2_elapsed_time);
							break;
						case Camera::CAMERA_STATE::PLAYER1P:
							player1p->Update(pl1_rightorleft, game_speed * p1_elapsed_time);
							break;
						case Camera::CAMERA_STATE::PLAYER2P:
							player2p->Update(pl2_rightorleft, game_speed * p2_elapsed_time);
							break;
						default:
							player1p->Update(pl1_rightorleft, game_speed * p1_elapsed_time);
							player2p->Update(pl2_rightorleft, game_speed * p2_elapsed_time);
							break;
						}

						//プレイヤーの移動距離制限
						if (player1p->act_state != ActState::STATENONE && player2p->act_state != ActState::STATENONE)
						{
							Limit::Stop(player1p->pos.x, player2p->pos.x);
						}

						DangerSound();

						DesastalFlash(game_speed);

						//勝敗判定
						Winjudge();
						if (judge != JUDGE_VICTORY::NO_VICTORY)
						{
							end = true;
							player1p->pad->Init();
							player2p->pad->Init();
							Scene_End_eye = Scene_eye;
							YRCamera.camera_state = Camera::CAMERA_STATE::END_GAME;
						}
						#ifdef EXIST_IMGUI
						if (Get_Debug_Draw())
						{
							input_pad.Update(player1p.get(), player2p.get());
						}
						#endif // EXIST_IMGUI
					}

				}
			}
			else
			{
				//ゲーム開始後カウント中に動かないようにコントローラーの入力を初期化している
				player1p->pad->Init();
				player2p->pad->Init();
				float pl1_rightorleft = 0.0f;
				float pl2_rightorleft = 0.0f;
				p1_elapsed_time = 1.0f;
				p2_elapsed_time = 1.0f;
				//1Pが左
				if (player1p->pos.x < player2p->pos.x)
				{
					pl1_rightorleft = 1.0f;
					pl2_rightorleft = -1.0f;
				}
				//2Pが左
				else
				{
					pl1_rightorleft = -1.0f;
					pl2_rightorleft = 1.0f;
				}

				player1p->Update(pl1_rightorleft, game_speed * p1_elapsed_time);
				player2p->Update(pl2_rightorleft, game_speed * p2_elapsed_time);

				//カウント中
				if (start_timer < start_time)
				{
					start_timer += game_speed;
					if (start_timer > ready_time)
					{
						GetSound().SEStop(SEKind::READY);
						GetSound().SEPlay(SEKind::SPECIAL_ATTACK);
					}
					else
					{
						GetSound().SEPlay(SEKind::READY);
					}
				}
				else
				{
					//カウントが既定に到達したのでゲーム開始
					start = true;
					GetSound().SEStop(SEKind::SPECIAL_ATTACK);
				}
			}
			break;
		case MAIN_LOOP::FINISH:
			//フェードインしたら勝敗に合わせてステートを変える
			if (fado_alpha < 0.4f)
			{
				switch (judge)
				{
				case JUDGE_VICTORY::NO_VICTORY:
					break;
				case JUDGE_VICTORY::VICTORY1P:
					main_loop = MAIN_LOOP::WIN1P;
					break;
				case JUDGE_VICTORY::VICTORY2P:
					main_loop = MAIN_LOOP::WIN2P;
					break;
				case JUDGE_VICTORY::DRAW:
					main_loop = MAIN_LOOP::DRAW;
					timer = 0.0f;
					break;
				default:
					break;
				}

			}
			break;
		case MAIN_LOOP::WIN1P:

			YRCamera.camera_state = Camera::CAMERA_STATE::PLAYER1P;
			//パッドの更新
			player1p->pad->Update(game_speed);
			player2p->pad->Update(game_speed);

#ifdef EXIST_IMGUI
			if (Get_Use_ImGui())
			{
				if (player1p->pad->x_input[scastI(PAD::START)] == 1 || player2p->pad->x_input[scastI(PAD::START)] == 1)
				{
					//ポーズボタンが押された
					pause = !pause;
				}

				//1Pイントロ更新
				if (pause)
				{
					game_speed = 0.0f;
				}
			}
#endif // EXIST_IMGUI

			if (player1p->WinPerformance(game_speed))
			{
				fado_start = true;
			}

			//途中ボタンが押されたらスキップ
			for (int i = 0; i < any_button.size(); i++)
			{
				if (player1p->pad->x_input[scastI(any_button[i])] == 1 || player2p->pad->x_input[scastI(any_button[i])] == 1)
				{
					fado_start = true;
				}
			}
			break;
		case MAIN_LOOP::WIN2P:

			YRCamera.camera_state = Camera::CAMERA_STATE::PLAYER2P;
			//パッドの更新
			player1p->pad->Update(game_speed);
			player2p->pad->Update(game_speed);

			if (player2p->WinPerformance(game_speed))
			{
				fado_start = true;
			}

			//途中ボタンが押されたらスキップ
			for (int i = 0; i < any_button.size(); i++)
			{
				if (player1p->pad->x_input[scastI(any_button[i])] == 1 || player2p->pad->x_input[scastI(any_button[i])] == 1)
				{
					fado_start = true;
				}
			}
			break;
		case MAIN_LOOP::DRAW:

			//パッドの更新
			player1p->pad->Update(game_speed);
			player2p->pad->Update(game_speed);

			if (timer > game_end_time)
			{
				fado_start = true;
			}
			else
			{
				timer += elapsed_time;
			}

			//途中ボタンが押されたらスキップ
			for (int i = 0; i < any_button.size(); i++)
			{
				if (player1p->pad->x_input[scastI(any_button[i])] == 1 || player2p->pad->x_input[scastI(any_button[i])] == 1)
				{
					fado_start = true;
				}
			}
			break;
		case MAIN_LOOP::GAME_FIN:
			FinUpdate();
			break;
		default:
			break;
		}
		//フェードアウトがスタートしてない場合は画面を映す
		if (fado_alpha > 0.0f)
		{
			fado_alpha -= (elapsed_time * mix_fade);
		}
	}
}







void SceneGame::Draw(float elapsed_time)
{
	SetHPTexture();

	static float light_color[4] = { 1,1,1,1 };
	//ライト方向
	static DirectX::XMFLOAT4 light_direction = DirectX::XMFLOAT4(-1, 0, 1, 0);
	static DirectX::XMFLOAT4 ambient_color(0.3f, 0.3f, 0.3f, 0.5f);
	//FRAMEWORK.context.Get()->OMSetRenderTargets(1, FRAMEWORK.view.GetAddressOf(), FRAMEWORK.depth.Get());

#ifdef EXIST_IMGUI
	//ImGui
	static float xxx = 0.0f;
	static float yyy = 0.0f;
	static DirectX::XMFLOAT3 eye_hit = { 0.0f,0.0f,0.0f };
	static DirectX::XMFLOAT3 focus_hit = { 0.0f,0.0f,0.0f };
	if(Get_Use_ImGui())
	{
		DirectX::XMFLOAT3	eye = YRCamera.GetEye();
		DirectX::XMFLOAT3	focus = YRCamera.GetFocus();
		float				fov = YRCamera.GetFov();
		fov /= 0.01745f;
		float				aspect = YRCamera.GetAspect();
		float				nearZ = YRCamera.GetNear();
		float				farZ = YRCamera.GetFar();
		bool show_demo_window = true;
		bool show_another_window = false;
		static int number_id = 0;

		ImGui::SetNextWindowSize(ImVec2(400, 400), 2);
		ImGui::Begin("palam", &show_another_window);
		//ImGui::Text("anim : %f", motion.anim_timer);
		ImGui::Text("time : %f", timer);
		int pl2p_con = scastI(pl2_con);
		ImGui::Text(u8"2Pの挙動 :"); ImGui::SameLine();
		ImGui::SliderInt(p2_con_name_list[pl2p_con].c_str(), &pl2p_con, 0, scastI(Player2PControl::END) - 1);
		pl2_con = static_cast<Player2PControl>(pl2p_con);
		ImGui::Checkbox("Camera_Debug", &camera_move_debug);
		if (ImGui::Button(u8"カメラをプレイヤー1に"))
		{
			eye = player1p->pos.GetDXFLOAT3();
			focus = player1p->pos.GetDXFLOAT3();
			eye.z -= 10.0f;
		}

		ImGui::Text("AIState : "); ImGui::SameLine();ImGui::Text(p2_AI_state_list[scastI(AI2P.state)].c_str());
		ImGui::Text(u8"カメラのステート : "); ImGui::SameLine(); ImGui::Text(camera_state_list[scastI(YRCamera.camera_state)].c_str());
		ImGui::Text(u8"カメラリクエストステート : "); ImGui::SameLine(); ImGui::Text(camera_request_list[scastI(YRCamera.GetRequest())].c_str());
		ImGui::Text("%f", AI2P.timer);
		float dis = player1p->pos.Distance(player2p->pos);
		ImGui::Text("%f", dis);

		//スプライトデバッグ
		ImGui::SliderFloat(u8"デバッグスプライト位置X", &sprite_debug_pos.x, -1000.0f, 2000.0f);
		ImGui::SliderFloat(u8"デバッグスプライト位置Y", &sprite_debug_pos.y, -1000.0f, 2000.0f);
		ImGui::SliderFloat(u8"デバッグスプライト大きさ", &sprite_debug_scale, 0.0f, 10.0f);


		/*ImGui::SliderFloat("Danger_p1_x", &Danger_pos_p1.x, 0.0f, 1920.0f);
		ImGui::SliderFloat("Danger_p1_y", &Danger_pos_p1.y, 0.0f, 1920.0f);
		ImGui::SliderFloat("Danger_p2_x", &Danger_pos_p2.x, 0.0f, 1920.0f);
		ImGui::SliderFloat("Danger_p2_y", &Danger_pos_p2.y, 0.0f, 1920.0f);
		ImGui::SliderFloat("Danger_size", &Danger_size, 0.0f, 10.0f);*/
		ImGui::SliderFloat("Effect_Z", &Hitcheak::effectpos.z, -50.0f, 50.0f);
		ImGui::Text("roll_timer : %f", roll_timer);
		//ImGui::InputFloat("scroll", &scall, 0.01f, 100.0f);
		//ImGui::SliderFloat("rollX", &roll.x, 0.0f, 30.00f);
		//ImGui::SliderFloat("rollY", &roll.y, 0.0f, 30.00f);
		//ImGui::SliderFloat("rollZ", &roll.z, 0.0f, 30.00f);
		//ImGui::InputFloat("transX", &trans.x, 1.0f, 30.00f);
		//ImGui::InputFloat("transY", &trans.y, 1.0f, 30.00f);
		//ImGui::InputFloat("transZ", &trans.z, 1.0f, 30.00f);
		eye.x -= eye_hit.x;
		eye.y -= eye_hit.y;
		eye.z -= eye_hit.z;
		focus.x -= focus_hit.x;
		focus.y -= focus_hit.y;
		focus.z -= focus_hit.z;
		if (ImGui::TreeNode("HitStopCamera"))
		{
			ImGui::SliderFloat("eye_X", &eye_hit.x, -360.0f, 360.0f);
			ImGui::SliderFloat("eye_Y", &eye_hit.y, -360.0f, 360.0f);
			ImGui::SliderFloat("eye_Z", &eye_hit.z, -360.0f, 360.0f);
			ImGui::SliderFloat("focus_X", &focus_hit.x, -360.0f, 360.0f);
			ImGui::SliderFloat("focus_Y", &focus_hit.y, -360.0f, 360.0f);
			ImGui::SliderFloat("focus_Z", &focus_hit.z, -360.0f, 360.0f);
			ImGui::TreePop();
		}
		if(ImGui::TreeNode("light"))
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
			ImGui::InputFloat("eye_X", &eye.x, 0.1f, 0.1f);
			ImGui::InputFloat("eye_Y", &eye.y, 0.1f, 0.1f);
			ImGui::InputFloat("eye_Z", &eye.z, 0.1f, 0.1f);
			ImGui::InputFloat("focus_X", &focus.x, 0.1f, 0.1f);
			ImGui::InputFloat("focus_Y", &focus.y, 0.1f, 0.1f);
			ImGui::InputFloat("focus_Z", &focus.z, 0.1f, 0.1f);
			ImGui::InputFloat("fov", &fov, 1.00f, 1.00f);
			ImGui::InputFloat("aspect", &aspect, 1.00f, 1.00f);
			ImGui::InputFloat("near", &nearZ, 1.00f, 1.00f);
			ImGui::InputFloat("far", &farZ, 1.00f, 1.00f);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("CameraSlider"))
		{
			ImGui::SliderFloat("eye_X", &eye.x, -350.1f, 350.1f);
			ImGui::SliderFloat("eye_Y", &eye.y, -350.1f, 350.1);
			ImGui::SliderFloat("eye_Z", &eye.z, -350.1f, 350.1);
			ImGui::SliderFloat("focus_X", &focus.x, -350.1f, 350.1);
			ImGui::SliderFloat("focus_Y", &focus.y, -350.1f, 350.1);
			ImGui::SliderFloat("focus_Z", &focus.z, -350.1f, 350.1);
			ImGui::SliderFloat("fov", &fov, 1, 360.00f);
			ImGui::SliderFloat("aspect", &aspect, 0.1f, 10.0f);
			ImGui::SliderFloat("near", &nearZ, 0.0f, 10.0f);
			ImGui::SliderFloat("far", &farZ, 0.1f, 1000000.0f);
			ImGui::TreePop();
		}
		eye.x += eye_hit.x;
		eye.y += eye_hit.y;
		eye.z += eye_hit.z;
		focus.x += focus_hit.x;
		focus.y += focus_hit.y;
		focus.z += focus_hit.z;

		YRCamera.SetEye(eye);
		YRCamera.SetFocus(focus);
		YRCamera.SetPerspective(fov* 0.01745f, aspect, nearZ, farZ);
		ImGui::Text("Right : %f", Limit::Right_max);
		ImGui::Text("Left : %f", Limit::Left_max);
		ImGui::Text("world : %f", world_max_x);

		//YRCamera.SetPerspective(fov, aspect, 0.0001f, 1000000.0f);
		ImGui::End();
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

	HPBar_Draw(V, P, light_direction, lightColor, ambient_color, elapsed_time);

	SetRenderTexture();

	//仮背景
	//test->DrawRotaGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 0.5f);

	////テスト描画(赤ポリ)
	//geo->render(
	//	geoShader.get(),
	//	DirectX::XMFLOAT3(0.0f, 0.0f, 20.0f),
	//	DirectX::XMFLOAT3(3.0f, 3.0f, 0.0f),
	//	DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
	//	V,
	//	P,
	//	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f)
	//);
	//stage.Draw(V, P, light_direction, lightColor, ambient_color, elapsed_time);
	RenderHP();

	switch (main_loop)
	{
	case MAIN_LOOP::INTRO1P:
		//1Pのイントロ
		//プレイヤー描画
		player1p->Draw(ParallelToonShader.get(),ToonShader.get(),V, P, lightColor, ambient_color, game_speed);
		player1p->IntroDEBUG();
		break;
	case MAIN_LOOP::INTRO2P:
		//2Pのイントロ
		//プレイヤー描画
		player2p->Draw(ParallelToonShader.get(), ToonShader.get(), V, P, lightColor, ambient_color, game_speed);
		break;
	case MAIN_LOOP::READY:
	case MAIN_LOOP::MAIN:
	case MAIN_LOOP::FINISH:
	case MAIN_LOOP::DRAW:
		//内部処理ではフェードをしているだけで画面に変化はない為一括


		//コンボに応じて画像セット
		ComboImageSet();

		//コンボ表示
		ComboDraw();

		//FRAMEWORK.context->OMSetDepthStencilState(m_depth_stencil_state2.Get(), 1);
		//プレイヤー描画
		player1p->Draw(ParallelToonShader.get(), ToonShader.get(), V, P, lightColor, ambient_color, game_speed*p1_elapsed_time);
		player2p->Draw(ParallelToonShader.get(), ToonShader.get(), V, P, lightColor, ambient_color, game_speed*p2_elapsed_time);

		//FRAMEWORK.context->OMSetDepthStencilState(m_depth_stencil_state.Get(), 1);

		//エフェクト
		YRGetEffect().CameraSet();

		//エフェクト描画
		YRGetEffect().Draw();

		IconDraw(V, P, elapsed_time);

		/*skin->Render(
			skinShader.get(), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f),
			DirectX::XMFLOAT3(DirectX::XMConvertToRadians(90.0f), 0.0f, 0.0f),
			V, P, light_direction, lightColor, ambient_color, elapsed_time, 0.0f
		);*/

#ifdef EXIST_IMGUI
			FRAMEWORK.context->OMSetDepthStencilState(m_depth_stencil_state2.Get(), 1);
			player1p->DrawDEBUG(geoShader.get(), V, P, light_direction, lightColor, ambient_color, game_speed * p1_elapsed_time);
			player2p->DrawDEBUG(geoShader.get(), V, P, light_direction, lightColor, ambient_color, game_speed * p2_elapsed_time);

			/*motion.DrawContinue(
			skinShader.get(),
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f),
			box_angle,
			V, P, light_direction, lightColor, ambient_color, elapsed_time
		);*/

			YRCamera.CameraMove(spriteShader.get());
			FRAMEWORK.context->OMSetDepthStencilState(m_depth_stencil_state.Get(), 1);
#endif // USE_IMGUI

		break;
	case MAIN_LOOP::WIN1P:
	{
		//プレイヤー描画
		player1p->Draw(ParallelToonShader.get(), ToonShader.get(), V, P, lightColor, ambient_color, game_speed);
		player1p->WinDEBUG();
	}
		break;
	case MAIN_LOOP::WIN2P:
	{
		//プレイヤー描画
		player2p->Draw(ParallelToonShader.get(), ToonShader.get(), V, P, lightColor, ambient_color, game_speed);
	}
		break;
	case MAIN_LOOP::GAME_FIN:
		break;
	default:
		break;
	}

	//フェード用画像描画
	//FRAMEWORK.fade_img->DrawRotaGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 1.0f, SpriteMask::NONE, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, fado_alpha));
	UI_Draw(elapsed_time);

	NullSetRenderTexture();
	//RenderHP();
	RenderTexture();
	RenderBlur();
	RenderUI();

	FRAMEWORK.framebuffer.Deactivate();
	//FRAMEWORK.framebuffer.SetDefaultRTV();
	//player1p->TextDraw();
	//player2p->TextDraw();

}



void SceneGame::IconDraw(
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	float					elapsed_time)
{

	//片方のキャラが画面下に消えた際、アイコンと向いている方向の矢印を描画する

	YR_Vector3 focus = YRCamera.GetFocus();

	if (player2p->pos.y >player1p->pos.y)
	{
		float dis =  player2p->pos.y -player1p->pos.y;

		if (dis > Icon_distance)
		{
			p1_icon_board->Render(
				animShader.get(),
				DirectX::XMFLOAT3(player1p->pos.x, focus.y - 10.0f, focus.z),
				DirectX::XMFLOAT2(1.5f, 1.5f),
				DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
				0.0f,
				view,
				projection,
				elapsed_time);

			float adjust = 0.0f;
			adjust = (-player1p->rightOrleft + 1.0f) / 2.0f;

			arrow_icon_board->Render(
				animShader.get(),
				DirectX::XMFLOAT3(player1p->pos.x + (3.5f * player1p->rightOrleft), focus.y - 10.0f, focus.z),
				DirectX::XMFLOAT2(1.0f, 1.0f),
				DirectX::XMFLOAT3(0.0f, 0.0f, DirectX::XMConvertToRadians(180.0f*adjust)),
				0.0f,
				view,
				projection,
				elapsed_time);
		}
	}


	if (player1p->pos.y>player2p->pos.y)
	{
		float dis = player1p->pos.y - player2p->pos.y;

		if (dis > Icon_distance)
		{
			p2_icon_board->Render(
				animShader.get(),
				DirectX::XMFLOAT3(player2p->pos.x, focus.y - 10.0f, focus.z),
				DirectX::XMFLOAT2(1.5f, 1.5f),
				DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
				0.0f,
				view,
				projection,
				elapsed_time);

			float adjust = 0.0f;
			adjust = (-player2p->rightOrleft + 1.0f) / 2.0f;

			arrow_icon_board->Render(
				animShader.get(),
				DirectX::XMFLOAT3(player2p->pos.x + (3.5f * player2p->rightOrleft), focus.y - 10.0f, focus.z),
				DirectX::XMFLOAT2(1.0f, 1.0f),
				DirectX::XMFLOAT3(0.0f, 0.0f, DirectX::XMConvertToRadians(180.0f * adjust)),
				0.0f,
				view,
				projection,
				elapsed_time);
		}
	}
}



void SceneGame::HPBar_Draw(
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	float elapsed_time)
{
	//ステージとUIの描画を行う

	stage.Draw(view, projection, light_direction, light_color, ambient_color, elapsed_time);

	switch (main_loop)
	{
	case SceneGame::MAIN_LOOP::INTRO1P:
	case SceneGame::MAIN_LOOP::INTRO2P:
		break;
	case SceneGame::MAIN_LOOP::READY:
	case SceneGame::MAIN_LOOP::MAIN:
	case SceneGame::MAIN_LOOP::FINISH:
	case SceneGame::MAIN_LOOP::DRAW:
		//必殺技などの時は周りを暗くする
		switch (YRCamera.GetRequest())
		{
		case Camera::Request::HOLD:
			FRAMEWORK.fade_img->DrawRotaGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 1.0f, false, SpriteMask::NONE, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.8f));
			break;
		default:
			break;
		}


		//UI描画
		//体力バー表示
		PL.ratio1P = player1p->hp / PL.HP_MAX1P * 800.0f;
		PL.ratio2P = player2p->hp / PL.HP_MAX2P * 800.0f;
		PL.correction_value = 800.0f - PL.ratio1P;

		DirectX::XMFLOAT4 hp1p_color = HPColorSet(player1p->hp, PL.HP_MAX1P);
		DirectX::XMFLOAT4 hp2p_color = HPColorSet(player2p->hp, PL.HP_MAX2P);

		if (player1p->combo_count == 1 && pl1_before_hp != player1p->hp)
		{
			PL.Damage_ratio1P = 800.0f - pl1_before_hp / PL.HP_MAX1P * 800.0f;
		}

		if (player2p->combo_count == 1 && pl2_before_hp != player2p->hp)
		{
			PL.Damage_ratio2P = pl2_before_hp / PL.HP_MAX2P * 800.0f;
		}

		if (player1p->combo_count == 0)
		{
			if (PL.Damage_ratio1P <= PL.correction_value)
			{
				PL.Damage_ratio1P += elapsed_time * 10.0f;
			}
		}

		if (player2p->combo_count == 0)
		{
			if (PL.Damage_ratio2P >= PL.ratio2P)
			{
				PL.Damage_ratio2P -= elapsed_time * 10.0f;
			}
		}

		sampler_wrap->Set(0);

		//HPbar_img->DrawExtendGraph(spriteShader.get(), 75.0f, 75.0f, 925.0f, 225.0f);
		//HPbar_img->DrawExtendGraph(spriteShader.get(), 975.0f, 75.0f, 1825.0f, 225.0f);

		HPbar_case->DrawRotaGraph(spriteShader.get(), 500.0f, 150.0f, 0.0f, 1.0f);
		HPbar_mask->DrawRotaGraph(spriteShader.get(), 500.0f, 150.0f, 0.0f, 1.0f, false, SpriteMask::WRITE);

		HPbar_case->DrawRotaGraph(spriteShader.get(), 1400.0f, 150.0f, 0.0f, 1.0f, true);
		HPbar_mask->DrawRotaGraph(spriteShader.get(), 1400.0f, 150.0f, 0.0f, 1.0f, true, SpriteMask::WRITE);

		//1PのHP
		HPDamagebar_img->DrawRectGraph(spriteShader.get(), 100.0f + PL.Damage_ratio1P, 100.0f, 800.0f - PL.ratio1P, 0.0f, (100.0f + PL.correction_value)-(100.0f + PL.Damage_ratio1P), 100.0f, false, SpriteMask::INDRAW);
		//HP_img->DrawRectGraph(spriteShader.get(), 100.0f + PL.correction_value, 100.0f, 800.0f - PL.ratio1P, 0.0f, PL.ratio1P, 100.0f,SpriteMask::INDRAW);
		HPbar_base->DrawRectGraph(spriteShader.get(), 100.0f + PL.correction_value, 100.0f, 800.0f - PL.ratio1P, 0.0f, PL.ratio1P, 100.0f, false, SpriteMask::INDRAW, hp1p_color);
		HPbar_fedo->DrawRectGraph(spriteShader.get(), 100.0f + PL.correction_value, 100.0f, 800.0f - PL.ratio1P, 0.0f, PL.ratio1P, 100.0f, false, SpriteMask::INDRAW, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
		HPbar_design->DrawRectGraph(spriteShader.get(), 100.0f + PL.correction_value, 100.0f, 800.0f - PL.ratio1P, 0.0f, PL.ratio1P, 100.0f, false, SpriteMask::INDRAW);


		//2PのHP
		HPDamagebar_img->DrawRectGraph(spriteShader.get(), 1000.0f, 100.0f, 0.0f, 0.0f, PL.Damage_ratio2P, 100.0f, false, SpriteMask::INDRAW);
		//HP_img->DrawRectGraph(spriteShader.get(), 1000.0f, 100.0f, 0.0f, 0.0f, PL.ratio2P, 100.0f);
		HPbar_base->DrawRectGraph(spriteShader.get(), 1000.0f, 100.0f, 0.0f, 0.0f, PL.ratio2P, 100.0f, true, SpriteMask::INDRAW, hp2p_color);
		HPbar_fedo->DrawRectGraph(spriteShader.get(), 1000.0f, 100.0f, 0.0f, 0.0f, PL.ratio2P, 100.0f, true, SpriteMask::INDRAW, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
		HPbar_design->DrawRectGraph(spriteShader.get(), 1000.0f, 100.0f, 0.0f, 0.0f, PL.ratio2P, 100.0f, true, SpriteMask::INDRAW);

		sampler_clamp->Set(0);

		//このフレームでのHPを保存する
		pl1_before_hp = player1p->hp;
		pl2_before_hp = player2p->hp;


		DangerDraw(game_speed);

		////エフェクト
		//YRGetEffect().CameraSet();

		////エフェクト描画
		//YRGetEffect().Draw();
		break;
	case SceneGame::MAIN_LOOP::GAME_FIN:
		break;
	default:
		break;
	}

	NullSetRenderTexture();
}




//DANGER描画、更新
void SceneGame::DangerDraw(float elapsed_time)
{
	if (end)
	{
		return;
	}

	float pos_plus_speed_D = 5000.0f;

	//サイズを大きくする、小さくするを繰り返す
	if (!pause)
	{
		float sin_timer = sinf(timer * 5.0f);
		Danger_size += (sin_timer * 0.001f);
	}

	//1PのDANGER表記
	if (player1p->hp <= (PL.HP_MAX1P * HP_Danger_point) && player1p->hp != 0.0f)
	{
		//座標を指定の位置まで移動させる(数値はImGuiにて設定した値)
		if (Danger_pos_p1.x < 350.0f)
		{
			Danger_pos_p1.x += (pos_plus_speed_D * elapsed_time);
		}
		else
		{
			Danger_pos_p1.x = 350.0f;
		}

		Danger_img->DrawRotaGraph(
			spriteShader.get(),
			Danger_pos_p1.x,
			Danger_pos_p1.y,
			0.0f,
			Danger_size,
			false,
			SpriteMask::NONE,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, Danger_alpha));
	}

	//2PのDANGER表記
	if (player2p->hp <= (PL.HP_MAX2P * HP_Danger_point) && player2p->hp != 0.0f)
	{
		//座標を指定の位置まで移動させる(数値はImGuiにて設定した値)
		if (Danger_pos_p2.x > 1570.0f)
		{
			Danger_pos_p2.x -= (pos_plus_speed_D * elapsed_time);
		}
		else
		{
			Danger_pos_p2.x = 1570.0f;
		}

		Danger_img->DrawRotaGraph(
			spriteShader.get(),
			Danger_pos_p2.x,
			Danger_pos_p2.y,
			0.0f,
			Danger_size,
			false,
			SpriteMask::NONE,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, Danger_alpha));
	}
}




//UIの描画(ゲージなど)
void SceneGame::UI_Draw(float elapsed_time)
{
	NullSetRenderTexture();

	SetUITexture();

#ifdef EXIST_IMGUI
	static float xx = 425.0f;
	static float yy = 1030.0f;
	if (Get_Use_ImGui())
	{
		ImGui::Begin("Gauge_UI");
		ImGui::SliderFloat("x", &xx, 0.0f, 1920.0f);
		ImGui::SliderFloat("y", &yy, 0.0f, 1080.0f);
		ImGui::End();
	}
#endif // EXIST_IMGUI

	//UIの描画
	switch (main_loop)
	{
	case MAIN_LOOP::INTRO1P:
		break;
	case MAIN_LOOP::INTRO2P:
		break;
	case MAIN_LOOP::READY:
	case MAIN_LOOP::MAIN:
	case MAIN_LOOP::FINISH:
	case MAIN_LOOP::DRAW:
		//内部処理ではフェードをしているだけで画面に変化はない為一括

		//UI描画


		switch (YRCamera.camera_state)
		{
		case Camera::CAMERA_STATE::PLAYER1P:
			player1p->DrawCutIn(spriteShader.get(), game_speed);
			break;
		case Camera::CAMERA_STATE::PLAYER2P:
			player2p->DrawCutIn(spriteShader.get(), game_speed);
			break;
		default:
			break;
		}

		//ゲージ描画
		PL.gauge1P = (player1p->gauge / GAUGE_MAX) * 640.0f;
		PL.gauge2P = (player2p->gauge / GAUGE_MAX) * 640.0f;

		sampler_wrap->Set(0);

		PL.power1P = ColorSet(player1p->power);
		//gauge_img_1p->DrawExtendAnimGraph(spriteShader.get(), 100.0f, 1000.0f, 100.0f + PL.gauge1P, 1064.0f, -100.0f, elapsed_time, SpriteMask::NONE, PL.power1P);
		PL.power2P = ColorSet(player2p->power);
		//gauge_img_2p->DrawExtendAnimGraph(spriteShader.get(), 1800.0f - PL.gauge2P, 1000.0f, 1800.0f, 1064.0f, 100.0f, elapsed_time, SpriteMask::NONE, PL.power2P);

		sampler_clamp->Set(0);

		//ゲージ
		gauge_case_img->DrawRotaGraph(spriteShader.get(), 425.0f, 1031.0f, 0.0f, 1.0f);
		gauge_case_mask->DrawRotaGraph(spriteShader.get(), 425.0f, 1030.0f, 0.0f, 1.0f,false,SpriteMask::WRITE);

		gauge_case_img->DrawRotaGraph(spriteShader.get(), 1475.0f, 1031.0f, 0.0f, 1.0f,true);
		gauge_case_mask->DrawRotaGraph(spriteShader.get(), 1475.0f, 1030.0f, 0.0f, 1.0f, true, SpriteMask::WRITE);

		gauge_img->DrawExtendGraph(spriteShader.get(), 100.0f, 1000.0f, 100.0f + PL.gauge1P, 1064.0f, SpriteMask::INDRAW, PL.power1P);
		gauge_img->DrawExtendGraph(spriteShader.get(), 1800.0f - PL.gauge2P, 1000.0f, 1800.0f, 1064.0f, SpriteMask::INDRAW, PL.power2P);

		gauge_anim1p->DrawExtendAnimGraph(spriteShader.get(), 100.0f, 1000.0f, 100.0f + PL.gauge1P, 1064.0f, -300.0f, elapsed_time, SpriteMask::INDRAW, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));
		gauge_anim2p->DrawExtendAnimGraph(spriteShader.get(), 1800.0f - PL.gauge2P, 1000.0f, 1800.0f, 1064.0f, 300.0f, elapsed_time, SpriteMask::INDRAW, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f));

		//ディザスタル描画
		//1P
		desastal_case->DrawRotaGraph
		(
			spriteShader.get(),
			86.0f,
			965.0f,
			0.0f,
			0.25f
		);
		desastal_img->DrawRotaDivGraph
		(
			spriteShader.get(),
			86.0f,
			965.0f,
			0.0f,
			0.25f,
			player1p->power,
			SpriteMask::NONE,
			PL.power1P
		);
		desastal_flash->DrawRotaGraph
		(
			spriteShader.get(),
			86.0f,
			965.0f,
			0.0f,
			0.25f + desastal_size_p1,
			false,
			SpriteMask::NONE,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, desastal_alpha_p1)
		);

		//2P
		desastal_case->DrawRotaGraph
		(
			spriteShader.get(),
			1838.0f,
			965.0f,
			0.0f,
			0.25f,
			true
		);
		desastal_img->DrawRotaDivGraphReverse
		(
			spriteShader.get(),
			1838.0f,
			965.0f,
			0.0f,
			0.25f,
			player2p->power,
			SpriteMask::NONE,
			PL.power2P
		);
		desastal_flash->DrawRotaGraph
		(
			spriteShader.get(),
			1838.0f,
			965.0f,
			0.0f,
			0.25f + desastal_size_p2,
			true,
			SpriteMask::NONE,
			DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, desastal_alpha_p2)
		);


#ifdef EXIST_IMGUI
		if (Get_Debug_Draw())
		{
			input_pad.Draw(spriteShader.get());
		}
#endif // EXIST_IMGUI


		if (pause)
		{
			pause_img->DrawGraph(
				spriteShader.get(),
				static_cast<float>(FRAMEWORK.SCREEN_WIDTH) / 2.0f,
				static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) / 2.0f + (sinf(timer) * 100.0f)
			);
		}

		//カウント表示
		if (!start)
		{
			//Are You Ready?
			if (start_timer < ready_time)
			{
				call_img->DrawRotaDivGraph
				(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) / 2.0f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) / 2.0f,
					0.0f,
					1.0f,
					0
				);
			}
			else
			{
				//Go!!
				call_img->DrawRotaDivGraph
				(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) / 2.0f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) / 2.0f,
					0.0f,
					1.0f,
					1
				);
			}
		}

		if (end)
		{
			//ゲームの決着がついた後
			if (endtimer < end_slow_time)
			{
				//「KO」
				KO_img->DrawRotaGraph(
					spriteShader.get(),
					static_cast<float>(FRAMEWORK.SCREEN_WIDTH) / 2.0f,
					static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) / 2.0f,
					0.0f,
					image_size,
					false,
					SpriteMask::NONE,
					DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, image_alpha)
				);
			}

			if (endtimer > end_slow_time)
			{
				if (judge == JUDGE_VICTORY::DRAW)
				{
					//「DROW」画像
					draw_img->DrawRotaGraph(
						spriteShader.get(),
						static_cast<float>(FRAMEWORK.SCREEN_WIDTH) / 2.0f,
						static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) / 2.0f,
						0.0f,
						image_size,
						false,
						SpriteMask::NONE,
						DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, image_alpha)
					);
				}
			}
		}

		break;
	case MAIN_LOOP::WIN1P:
	{
		float x_pos = static_cast<float>(FRAMEWORK.SCREEN_WIDTH) * 0.7f;
		float y_pos = static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f;
		win1P_img->DrawGraph(spriteShader.get(), x_pos, y_pos);
	}
	break;
	case MAIN_LOOP::WIN2P:
	{
		float x_pos = static_cast<float>(FRAMEWORK.SCREEN_WIDTH) * 0.7f;
		float y_pos = static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) * 0.7f;
		win2P_img->DrawGraph(spriteShader.get(), x_pos, y_pos);
	}
	break;
	case MAIN_LOOP::GAME_FIN:
		break;
	default:
		break;
	}
	/*sprite->render(
		concentrationShader.get(),
		UI_texture.get(),
		cbuffer_param,
		sampler_wrap.get(),
		constantBuffer,
		0.0f, 0.0f, 1920.0f, 1080.0f,
		0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);*/
	//フェード用画像描画
	//FRAMEWORK.fade_img->DrawRotaGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 1.0f,false, SpriteMask::NONE, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, fado_alpha));
}



bool SceneGame::FadeOut(float elapsed_time)
{
	fado_alpha += mix_fade*(elapsed_time);

	if (fado_alpha > 1.0f)
	{
		return true;
	}

	return false;
}


void SceneGame::PadSet(int select1p)
{
	if (select1p == scastI(INPUT_PLAYER::P1))
	{
		pad1 = std::make_unique<GamePad1>();
		pad2 = std::make_unique<GamePad2>();
	}
	else
	{
		pad1 = std::make_unique<GamePad2>();
		pad2 = std::make_unique<GamePad1>();
	}
}
void SceneGame::PadSet(int select1p,int select2p)
{
	if (select1p == scastI(INPUT_PLAYER::P1))
	{
		player1p->pad = std::make_unique<GamePad1>();
	}
	else
	{
		player1p->pad = std::make_unique<GamePad2>();
	}

	if (select2p == scastI(INPUT_PLAYER::P1))
	{
		player2p->pad = std::make_unique<GamePad1>();
	}
	else
	{
		player2p->pad = std::make_unique<GamePad2>();
	}
}

DirectX::XMFLOAT2 SceneGame::Distance(DirectX::XMFLOAT2& s_pos, DirectX::XMFLOAT2& e_pos)
{
	DirectX::XMFLOAT2 distance;//距離

	distance.x = s_pos.x - e_pos.x;
	if (distance.x < 0)
	{
		distance.x = -distance.x;
	}
	distance.y = s_pos.y - e_pos.y;
	if (distance.y < 0)
	{
		distance.y = -distance.y;
	}
	return distance;
}



DirectX::XMFLOAT4 SceneGame::ColorSet(int power)
{
	//ゲージの量からフォントの色を決定する
	switch (power)
	{
	case 0:
		return DirectX::XMFLOAT4(1.0f,0.4f,0.0f,1.0f);
		break;
	case 1:
		return DirectX::XMFLOAT4(0.4f,1.0f,0.0f,1.0f);
		break;
	case 2:
		return DirectX::XMFLOAT4(0.0f,0.0f,1.0f,1.0f);
		break;
	case 3:
		return DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
		break;
	case 4:
		return DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case 5:
		return DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
		break;

	}
	return DirectX::XMFLOAT4(1.0f, 0.4f, 0.0f, 1.0f);
}


DirectX::XMFLOAT4 SceneGame::HPColorSet(float hp, float max_hp)
{
	//ゲージの量からフォントの色を決定する
	if (hp >= max_hp)
	{
		return DirectX::XMFLOAT4(0.0f, 0.5f, 1.0f, 1.0f);
	}

	if (hp <= (max_hp * HP_Danger_point))
	{
		return DirectX::XMFLOAT4(1.0f, 0.2f, 0.0f, 1.0f);
	}

	return DirectX::XMFLOAT4(0.4f, 1.0f, 0.0f, 1.0f);
}




void SceneGame::Winjudge()
{
	if (player1p->hp <= 0 && player2p->hp <= 0)
	{
		player1p->hp = 0;
		player2p->hp = 0;
		judge = JUDGE_VICTORY::DRAW;
		return;
	}
	if (player1p->hp <= 0)
	{
		player1p->hp = 0;
		judge = JUDGE_VICTORY::VICTORY2P;
		return;
	}
	if (player2p->hp <= 0)
	{
		player2p->hp = 0;
		judge = JUDGE_VICTORY::VICTORY1P;
		return;
	}
	judge = JUDGE_VICTORY::NO_VICTORY;
}




void SceneGame::PauseUpdate()
{
	game_speed = 0.0f;
	//ポーズ中行う処理
	if (player1p->pad->x_input[scastI(PAD::START)] == 1 || player2p->pad->x_input[scastI(PAD::START)] == 1)
	{
		pause = !pause;
		//pauseTimer = 20;
	}
	if (player1p->pad->x_input[scastI(PAD::LB)] == 1 ||player2p->pad->x_input[scastI(PAD::LB)] == 1)
	{
		//ゲームリセット
		//fado_start = true;
		//FRAMEWORK.SetScene(SCENE_SELECT);
		player1p->Init(PL.pos1P);
		player2p->Init(PL.pos2P);
		Init();
		pause = false;
	}
	if (player1p->pad->x_input[scastI(PAD::RB)] == 1 && player1p->pad->x_input[scastI(PAD::LB)] > 0)
	{
		//タイトルに戻る
		UnInit();
		FRAMEWORK.SetScene(SCENE_TABLE::SCENE_TITLE);
	}
	if (player2p->pad->x_input[scastI(PAD::RB)] == 1 || player1p->pad->x_input[scastI(PAD::LB)] > 0)
	{
		//タイトルに戻る
		UnInit();
		FRAMEWORK.SetScene(SCENE_TABLE::SCENE_TITLE);
	}
#ifdef EXIST_IMGUI
	if (pKeyState.pflg == 1)
	{
		pause = !pause;
	}
	if (Get_Use_ImGui())
	{
		player1p->StopHitParamUpdate();
	}
#endif // EXIST_IMGUI

}




void SceneGame::TrackSet()
{
	//ホーミングダッシュ用の値を変更する
	player1p->tracking.rival_Pos = player2p->pos;
	player2p->tracking.rival_Pos = player1p->pos;
	//相手のステートも送る
	player1p->rival_state = player2p->act_state;
	player2p->rival_state = player1p->act_state;
	//コンボカウントも送る
	player1p->combo_count_player = player2p->combo_count;
	player2p->combo_count_player = player1p->combo_count;
	//フレーム情報を送る
	player1p->rival_fream = player2p->fream;
	player2p->rival_fream = player1p->fream;
}


void SceneGame::FinUpdate()
{
	//ゲーム終了後の処理
	fado_start = true;
}



void SceneGame::CameraUpdate(float elapsed_time)
{
	switch (YRCamera.camera_state)
	{
	case Camera::CAMERA_STATE::MAIN:
		//通常カメラ
		//カメラのステートがMAINにある場合のカメラ処理を行う
		if (!camera_move_debug)
		{
			YRCamera.SetEye(Scene_eye.GetDXFLOAT3());			//視点
			YRCamera.SetFocus(Scene_focus.GetDXFLOAT3());		//注視点
			YRCamera.SetUp(Scene_up.GetDXFLOAT3());				//上方向
			YRCamera.SetPerspective(Scene_fov, Scene_aspect, Scene_nearZ, Scene_farZ);
		}
		break;
	case Camera::CAMERA_STATE::PLAYER1P:
		//1Pがカメラを持っている
		break;
	case Camera::CAMERA_STATE::PLAYER2P:
		//2Pがカメラを持っている
		break;
	case Camera::CAMERA_STATE::ZOOM_CAMERA:
		//カメラがズームしている
		if (YRCamera.viblate_timer > 0)
		{
			YRCamera.viblate_timer -= elapsed_time;

			DirectX::XMFLOAT3 focus;
			focus = YRCamera.targetPos;
			float rax = rand() % 3 - 1.0f;
			float ray = rand() % 3 - 1.0f;
			float raz = rand() % 3 - 1.0f;

			float dx = rax * 0.5f * YRCamera.viblate_timer + YRCamera.targetPos.x;
			float dy = ray * 0.5f * YRCamera.viblate_timer + YRCamera.targetPos.y;
			//float dz = raz * 1.0f * YRCamera.viblate_timer + YRCamera.targetPos.z;
			focus.x = dx;
			focus.y = dy;

			YRCamera.SetFocus(focus);
		}
		break;
	default:
		break;
	}
	YRCamera.Active();
}


void SceneGame::CameraRequest(float elapsed_time)
{
	switch (YRCamera.GetRequest())
	{
	case Camera::Request::NONE:
		break;
	case Camera::Request::HOLD:
	{
		int req_player = YRCamera.GetRequestPlayer();
		if (req_player == 1)
		{
			YRCamera.camera_state = Camera::CAMERA_STATE::PLAYER1P;
			p2_elapsed_time = 0.0f;
		}
		if (req_player == 2)
		{
			YRCamera.camera_state = Camera::CAMERA_STATE::PLAYER2P;
			p1_elapsed_time = 0.0f;
		}
	}
		break;
	case Camera::Request::RELEASE:
	{
		int req_player = YRCamera.GetRequestPlayer();
		if (req_player == 0)
		{
			YRCamera.camera_state = Camera::CAMERA_STATE::MAIN;
		}
		if (req_player == 1)
		{
			YRCamera.camera_state = Camera::CAMERA_STATE::MAIN;
			p2_elapsed_time = 1.0f;
		}
		if (req_player == 2)
		{
			YRCamera.camera_state = Camera::CAMERA_STATE::MAIN;
			p1_elapsed_time = 1.0f;
		}
	}
		break;
	case Camera::Request::WEAKEN:
	{
		DirectX::XMFLOAT3 eye = YRCamera.GetEye();
		DirectX::XMFLOAT3 focus = YRCamera.GetFocus();
		float fov = YRCamera.GetFov();
		//float elap = 50.0f;

		DirectX::XMVECTOR scene_eye_vector = DirectX::XMLoadFloat3(&Scene_eye.GetDXFLOAT3());
		DirectX::XMVECTOR eye_vector = DirectX::XMLoadFloat3(&eye);
		DirectX::XMVECTOR scene_focus_vector = DirectX::XMLoadFloat3(&Scene_focus.GetDXFLOAT3());
		DirectX::XMVECTOR focus_vector = DirectX::XMLoadFloat3(&focus);
		DirectX::XMVECTOR scene_fov_vector = DirectX::XMLoadFloat(&Scene_fov);
		DirectX::XMVECTOR fov_vector = DirectX::XMLoadFloat(&fov);

		DirectX::XMVECTOR eye_larp = DirectX::XMVectorLerp(eye_vector, scene_eye_vector, 0.05f);
		DirectX::XMVECTOR focus_larp = DirectX::XMVectorLerp(focus_vector, scene_focus_vector, 0.05f);
		DirectX::XMVECTOR fov_larp = DirectX::XMVectorLerp(fov_vector, scene_fov_vector, 0.1f);


		DirectX::XMStoreFloat3(&eye, eye_larp);
		DirectX::XMStoreFloat3(&focus, focus_larp);
		DirectX::XMStoreFloat(&fov, fov_larp);

		YRCamera.SetEye(eye);
		YRCamera.SetFocus(focus);
		YRCamera.SetFov(fov);
		YRCamera.Active();

	}
		break;
	case Camera::Request::ZOOM:
	{
		//一度しか入らないようにする
		if (YRCamera.camera_state == Camera::CAMERA_STATE::ZOOM_CAMERA)
		{
			return;
		}
		YRCamera.camera_state = Camera::CAMERA_STATE::ZOOM_CAMERA;

		YR_Vector3 p_pos = { 0.0f,0.0f,0.0f };
		YR_Vector3 damage_pos = { 0.0f,0.0f,0.0f };

		if (YRCamera.damage_pl_num > 1)
		{
			//ダメージを受けたのは2P
			p_pos = player1p->pos;
			damage_pos = player2p->pos;
		}
		else
		{
			//ダメージを受けたのは1P
			p_pos = player2p->pos;
			damage_pos = player1p->pos;
		}

		//距離を算出する
		YR_Vector3 dis = damage_pos - p_pos;
		dis.x /= 2.0f;
		dis.y /= 2.0f;
		YR_Vector3 camerapos = p_pos + dis;

		//カメラの座標を算出する
		DirectX::XMFLOAT3 focus = camerapos.GetDXFLOAT3();
		DirectX::XMFLOAT3 eye = camerapos.GetDXFLOAT3();

		//ImGuiにて調整した補正値
		float x_adjust = 70.0f;
		float y_adjust = 5.0f;

		if (YRCamera.damage_pl_num > 1)
		{
			//ダメージを受けたのは2P

			if (player1p->pos.x < player2p->pos.x)
			{
				//1Pが左の場合
				eye.x -= x_adjust;
			}
			else
			{
				//1Pが右の場合
				eye.x += x_adjust;
			}
		}
		else
		{

			//ダメージを受けたのは1P
			if (player1p->pos.x < player2p->pos.x)
			{
				//2Pが右の場合
				eye.x += x_adjust;
			}
			else
			{
				//2Pが左の場合
				eye.x -= x_adjust;
			}
		}
		eye.y -= y_adjust;

		eye.z = focus.z - 100.0f;

		YRCamera.viblate_timer = 0.5f;
		YRCamera.viblate_timer = 0.0f;
		YRCamera.targetPos = focus;

		YRCamera.SetEye(eye);
		YRCamera.SetFocus(focus);
		YRCamera.Active();
	}
	default:
		break;
	}
}



//---------------------------------------------------------------------------------------
//				コンボの画像セット
void SceneGame::ComboImageSet()
{
	int s[3];
	int v[3];
	int dScore = player1p->combo_count;
	int dVcore = player2p->combo_count;
	s[0] = dScore / 100;
	dScore = dScore % 100;
	s[1] = dScore / 10;
	dScore = dScore % 10;
	s[2] = dScore;

	v[0] = dVcore / 100;
	dVcore = dVcore % 100;
	v[1] = dVcore / 10;
	dVcore = dVcore % 10;
	v[2] = dVcore;

	for (int i = 0; i < 3; i++)
	{
		p1combo[i] = s[i];
		p2combo[i] = v[i];
	}
}
//
//-------------------------------------



void SceneGame::PlayerMoveReq(int req_player_num, YR_Vector3 pos)
{
	//プレイヤークラスから呼び出す関数
	//リクエストしたプレイヤー以外のプレイヤーの位置を動かす
	switch (req_player_num)
	{
	case 1:
		player2p->pos = pos;
		break;
	case 2:
		player1p->pos = pos;
		break;
	default:
		break;
	}
}


void SceneGame::PlayerMoveReq(int req_player_num, YR_Vector3 vec, float speed)
{
	//プレイヤークラスから呼び出す関数
	//リクエストしたプレイヤー以外のプレイヤーに速度を送る
	YR_Vector3 plus_speed = vec * speed;
	switch (req_player_num)
	{
	case 1:
		player2p->speed = plus_speed;
		break;
	case 2:
		player1p->speed = plus_speed;
		break;
	default:
		break;
	}
}




void SceneGame::SetRenderTexture()
{
	FRAMEWORK.framebuffer.SetRenderTexture(color_texture->GetRenderTargetView());
	//FRAMEWORK.framebuffer.SetRenderTexture(normal_texture->GetRenderTargetView());
	//FRAMEWORK.framebuffer.SetRenderTexture(position_texture->GetRenderTargetView());
	FRAMEWORK.framebuffer.SetRenderTexture(luminance_texture->GetRenderTargetView());

	ID3D11DepthStencilView* dsv = color_texture->GetDepthStencilView();

	//画面クリア
	FRAMEWORK.framebuffer.Clear(0.0f, 0.0f, 0.0f, 0.0f);
	FRAMEWORK.context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//ビュー更新
	YRCamera.Active();

	//ビューポート設定
	//レンダーターゲットビューの設定
	//FRAMEWORK.framebuffer.GetDefaultRTV();
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




void SceneGame::SetHPTexture()
{
	FRAMEWORK.framebuffer.GetDefaultRTV();
	FRAMEWORK.framebuffer.SetRenderTexture(HP_texture->GetRenderTargetView());

	ID3D11DepthStencilView* dsv = HP_texture->GetDepthStencilView();

	//画面クリア
	FRAMEWORK.framebuffer.Clear(0.0f, 0.0f, 0.0f, 0.0f);
	FRAMEWORK.context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//ビュー更新
	YRCamera.Active();

	//ビューポート設定
	//レンダーターゲットビューの設定
	//FRAMEWORK.framebuffer.GetDefaultRTV();
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




void SceneGame::SetUITexture()
{
	FRAMEWORK.framebuffer.SetRenderTexture(UI_texture->GetRenderTargetView());

	ID3D11DepthStencilView* dsv = UI_texture->GetDepthStencilView();

	//画面クリア
	FRAMEWORK.framebuffer.Clear(0.0f,0.0f,0.0f,0.0f);
	FRAMEWORK.context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//ビュー更新
	YRCamera.Active();

	//ビューポート設定
	//レンダーターゲットビューの設定
	//FRAMEWORK.framebuffer.GetDefaultRTV();
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


void SceneGame::NullSetRenderTexture()
{
	//レンダーターゲットの回復
	//FRAMEWORK.context.Get()->OMSetRenderTargets(testrtv.size(), testrtv.data(), FRAMEWORK.depth.Get());
	FRAMEWORK.framebuffer.ResetRenderTexture();
	//framebuffer::ResetRenderTargetViews();
	FRAMEWORK.framebuffer.SetDefaultRTV();
}


void SceneGame::RenderTexture()
{
	//Gbuffer描画
	sprite->render(
		spriteEx.get(),
		color_texture.get(),
		0.0f, 0.0f, 1920.0f, 1080.0f,
		0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);

#ifdef EXIST_IMGUI
	if (Get_Use_ImGui())
	{
		if (ImGui::TreeNode(u8"カラーテクスチャ"))
		{
			ImGui::Image((void*)(color_texture->GetShaderResource()), ImVec2(360, 360));
			ImGui::Image((void*)(luminance_texture->GetShaderResource()), ImVec2(360, 360));
			ImGui::TreePop();
		}
	}
#endif // USE_IMGUI

}



void SceneGame::RenderHP()
{

	//ブレンドステート設定
	FRAMEWORK.BlendSet(Blend::ALPHA);

	//Gbuffer描画
	sprite->render(
		spriteEx.get(),
		HP_texture.get(),
		0.0f, 0.0f, 1920.0f, 1080.0f,
		0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);

#ifdef EXIST_IMGUI
	if (Get_Use_ImGui())
	{
		if (ImGui::TreeNode(u8"HPテクスチャ"))
		{
			ImGui::Image((void*)(HP_texture->GetShaderResource()), ImVec2(360, 360));
			ImGui::TreePop();
		}
	}
#endif // USE_IMGUI

}



void SceneGame::RenderUI()
{

	//ブレンドステート設定
	FRAMEWORK.BlendSet(Blend::ALPHA);

	//Gbuffer描画
	sprite->render(
		spriteEx.get(),
		UI_texture.get(),
		0.0f, 0.0f, 1920.0f, 1080.0f,
		0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);

	//フェード用画像描画
	FRAMEWORK.fade_img->DrawRotaGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 1.0f, false, SpriteMask::NONE, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, fado_alpha));

#ifdef EXIST_IMGUI
	if (Get_Use_ImGui())
	{
		if (ImGui::TreeNode(u8"UIテクスチャ"))
		{
			ImGui::Image((void*)(UI_texture->GetShaderResource()), ImVec2(360, 360));
			ImGui::TreePop();
		}
	}
#endif // USE_IMGUI

}


void SceneGame::RenderBlur()
{
	float off_x = 1.0f;
	float off_y = 1.0f;
	float deviation = 0.5f;
	//framebuffer::ResetRenderTargetViews();
	FRAMEWORK.framebuffer.Deactivate();

	GaussParamManager gauss;
	float w = (float)FRAMEWORK.SCREEN_WIDTH;
	float h = (float)FRAMEWORK.SCREEN_HEIGHT;

	if (blur_on)
	{
		//FRAMEWORK.framebuffer.GetDefaultRTV();
		//初回のみ現在の描画を利用する
		//テクスチャをセット
		FRAMEWORK.framebuffer.SetRenderTexture(blur_texture[0]->GetRenderTargetView(), true);
		ID3D11DepthStencilView* dsv = blur_texture[0]->GetDepthStencilView();

		//画面をクリア
		FRAMEWORK.framebuffer.Clear();
		FRAMEWORK.context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		//レンダーターゲットビューの設定
		//FRAMEWORK.SetViewPort(1920.0f, 1080.0f);
		FRAMEWORK.framebuffer.Activate(1920.0f, 1080.0f,dsv);

		//ブレンドステート設定
		FRAMEWORK.BlendSet(Blend::ALPHA);
		//ラスタライザー設定
		//FRAMEWORK.context->RSSetState(FRAMEWORK.rasterizer_state[framework::RS_CULL_BACK].Get());
		//デプスステンシルステート設定
		//FRAMEWORK.context->OMSetDepthStencilState(FRAMEWORK.depthstencil_state[framework::DS_TRUE].Get(), 1);

		//サンプラー設定
		sampler_clamp->Set(0);

		gauss.CalcBlurParam(w, h, YR_Vector3(off_x, 0.0f), deviation);
		FRAMEWORK.context->UpdateSubresource(constantBuffer.Get(), 0, NULL, &gauss.param, 0, 0);
		FRAMEWORK.context->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());

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
			FRAMEWORK.framebuffer.SetRenderTexture(blur_texture[i]->GetRenderTargetView(), true);
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
				FRAMEWORK.context->UpdateSubresource(constantBuffer.Get(), 0, NULL, &gauss.param, 0, 0);
				FRAMEWORK.context->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
			}
			else
			{
				riv2 = static_cast<float>(i);
				gauss.CalcBlurParam(w * 0.5f / riv2, h * 0.5f / riv2, YR_Vector3(off_x, 0.0f), deviation);
				FRAMEWORK.context->UpdateSubresource(constantBuffer.Get(), 0, NULL, &gauss.param, 0, 0);
				FRAMEWORK.context->PSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
			}

			ID3D11DepthStencilView* dsv2 = blur_texture[i]->GetDepthStencilView();
			FRAMEWORK.context->ClearDepthStencilView(dsv2, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

			//レンダーターゲットビューの設定
			FRAMEWORK.framebuffer.Activate(w * 0.5f / riv2, h * 0.5f / riv2,dsv2);


			sprite->render(
				gaussShader.get(),
				blur_texture[i - 1].get(),
				0.0f, 0.0f, 1920.0f * 0.5f / riv2, 1080.0f * 0.5f / riv2,
				0.0f, 0.0f, 1920.0f*0.5f/riv, 1080.0f*0.5f/riv, 0.0f, 1.0f);
			//FRAMEWORK.framebuffer.Deactivate();
			//FRAMEWORK.framebuffer.ResetRenderTexture();
		}

		//全てのテクスチャを合成したマルチガウスのテクスチャを作成する

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

#ifdef EXIST_IMGUI
		//ImGui
		if (Get_Use_ImGui())
		{
			if (ImGui::TreeNode(u8"ブルーム"))
			{
				ImGui::Checkbox(u8"ブルーム", &blur_on);
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
		//FRAMEWORK.framebuffer.SetDefaultRTV();

		//ブレンドステート設定
		//FRAMEWORK.BlendSet(Blend::ADD);

		/*sprite->render(
			spriteEx.get(),
			multi_blur_texture.get(),
			0.0f, 0.0f, 1920.0f, 1080.0f,
			0.0f, 0.0f, 1920.0f, 1080.0f, 0.0f, 1.0f);*/
	}
}

void SceneGame::Control2PState(float elapsed_time)
{
	switch (pl2_con)
	{
	case SceneGame::Player2PControl::OPERATION:
		//操作可能
		player2p->pad->Update(elapsed_time);
		break;
	case SceneGame::Player2PControl::SUSPENSION:
		//操作不能
		player1p->hp = 1000.0f;
		player2p->hp = 1000.0f;
		player1p->power = 5;
		player2p->power = 5;
		break;
	case SceneGame::Player2PControl::AI:
		//AI
		AIControll(elapsed_time);
		break;
	case SceneGame::Player2PControl::INVINCIVLE:
		//操作不能
		player1p->hp = 1000.0f;
		player2p->hp = 1000.0f;
		player1p->power = 5;
		player2p->power = 5;
		player2p->HitBoxTransition(HitBoxState::INVINCIBLE);
		break;
	default:
		break;
	}
}



void SceneGame::EndORGameCameraSet(float elapsed_time)
{
	//ゲーム中、もしくは終了時のカメラの座標を計算する
	if (end)
	{
		//回転に使用するタイマーを増やす
		roll_timer += elapsed_time * endtimer * 1.5f;

		//画像表示する際のアルファ値を上げていく
		if (image_alpha < 1.0f)
		{
			image_alpha += elapsed_time * 0.5f;
		}

		//画像表示する際の大きさを下げていく
		if (image_size > 1.0f)
		{
			image_size -= elapsed_time * 5.0f * endtimer;
		}

		//プレイヤーの位置からカメラの位置を決定する
		float distance = 3.0f;		//カメラ調整値
		switch (judge)
		{
		case SceneGame::JUDGE_VICTORY::VICTORY1P:
			//1Pが勝利したので2Pをカメラの中心に
			Scene_eye.y = player2p->pos.y + distance;
			Scene_focus.y = player2p->pos.y + distance;
			break;
		case SceneGame::JUDGE_VICTORY::VICTORY2P:
			//2Pが勝利したので1Pをカメラの中心に
			Scene_eye.y = player1p->pos.y + distance;
			Scene_focus.y = player1p->pos.y + distance;
			break;
		case SceneGame::JUDGE_VICTORY::DRAW:
		{
			//引き分けなので両方の中心をカメラの中心に
			if (player1p->pos.y < player2p->pos.y)
			{
				//1Pが2Pより下にいる場合
				distance = player2p->pos.y - player1p->pos.y;
				Scene_eye.y = player1p->pos.y + distance;
				Scene_focus.y = player1p->pos.y + distance;
			}
			else
			{
				distance = player1p->pos.y - player2p->pos.y;
				Scene_eye.y = player2p->pos.y + distance;
				Scene_focus.y = player2p->pos.y + distance;
			}
		}
			break;
		default:
			break;
		}

		//ゲーム終了時カメラを回転させる
		float timer = roll_timer;
		float x_circle = cosf(timer);
		float z_circle = sinf(timer);

		//カメラのステートを固定する
		YRCamera.camera_state = Camera::CAMERA_STATE::END_GAME;

		switch (fin_camera_state)
		{
		case SceneGame::FIN_CAMERA_STATE::ROLL:
		{
			//カメラを回転
			Scene_eye.x = Scene_End_eye.x + (Scene_End_eye.z * x_circle);
			Scene_eye.z = (130.0f * z_circle);

			//タイマーが一定以上になるまで回転
			if (roll_timer > 4.0f)
			{
				//現在の値がプラスになった(初期位置に戻った)らカメラの回転を止める
				if (x_circle > 0.0f)
				{
					Scene_eye.x = Scene_End_eye.x;
					Scene_eye.z = 130.0f;
					fin_camera_state = FIN_CAMERA_STATE::STOP;
				}
			}
		}
			break;
		case SceneGame::FIN_CAMERA_STATE::STOP:
		{
			YRCamera.camera_state = Camera::CAMERA_STATE::MAIN;
			//プレイヤーの位置からカメラの位置を決定する
			YR_Vector3 camera_screen = Limit::Set(player1p->pos, player2p->pos, Start_Scene_eye);


			DirectX::XMVECTOR scene_eye_vector = DirectX::XMLoadFloat3(&Scene_eye.GetDXFLOAT3());
			DirectX::XMVECTOR eye_vector = DirectX::XMLoadFloat3(&camera_screen.GetDXFLOAT3());
			DirectX::XMVECTOR scene_focus_vector = DirectX::XMLoadFloat3(&Scene_focus.GetDXFLOAT3());
			DirectX::XMVECTOR focus_vector = DirectX::XMLoadFloat3(&camera_screen.GetDXFLOAT3());

			DirectX::XMVECTOR eye_larp = DirectX::XMVectorLerp(eye_vector, scene_eye_vector, 0.05f);
			DirectX::XMVECTOR focus_larp = DirectX::XMVectorLerp(focus_vector, scene_focus_vector, 0.05f);

			DirectX::XMFLOAT3 eye = { 0.0f,0.0f,0.0f };
			DirectX::XMFLOAT3 focus = { 0.0f,0.0f,0.0f };

			DirectX::XMStoreFloat3(&eye, eye_larp);
			DirectX::XMStoreFloat3(&focus, focus_larp);

			Scene_eye = eye;
			Scene_focus.x = focus.x;
			Scene_focus.y = focus.y;
		}
			break;
		default:
			break;
		}
	}
	else
	{
		//プレイヤーの位置からカメラの位置を決定する
		YR_Vector3 camera_screen = Limit::Set(player1p->pos, player2p->pos, Start_Scene_eye);
		Scene_eye.x = camera_screen.x;
		Scene_eye.y = camera_screen.y;
		Scene_eye.z = camera_screen.z;
		Scene_focus.x = camera_screen.x;
		Scene_focus.y = camera_screen.y;
	}
}



void SceneGame::DangerSound()
{
	//HPがDANGER状態になったらサウンドを鳴らす

	//前フレームのHPがDANGERに到達していなかった場合
	if (pl1_before_hp > (PL.HP_MAX1P * HP_Danger_point))
	{
		//現在のHPがDANGERに到達していればサウンドを鳴らす
		if (player1p->hp <= (PL.HP_MAX1P * HP_Danger_point))
		{
			GetSound().SESinglePlay(SEKind::DANGER);
		}
	}

	//前フレームのHPがDANGERに到達していなかった場合
	if (pl2_before_hp > (PL.HP_MAX2P * HP_Danger_point))
	{
		//現在のHPがDANGERに到達していればサウンドを鳴らす
		if (player2p->hp <= (PL.HP_MAX2P * HP_Danger_point))
		{
			GetSound().SESinglePlay(SEKind::DANGER);
		}
	}
}


void SceneGame::DesastalFlash(float elapsed_time)
{
	//ディザスタルが増えた時に数値を変化させて光っているように見せる
	//その時、サウンドも鳴らす

	float size_up = 1.0f;
	float alpha_down = 1.0f;

	//1P
	//前回フレームと現在のディザスタルの数が異なる場合
	if (pl1_before_power != player1p->power)
	{
		desastal_alpha_p1 = 1.0f;
		desastal_size_p1 = 0.0f;
		//前回フレームより現在のディザスタルの数が多い場合
		if (pl1_before_power < player1p->power)
		{
			GetSound().SESinglePlay(SEKind::GAUGE_UP);
		}
	}
	if (desastal_size_p1 < 5.0f)
	{
		desastal_size_p1 += (size_up * elapsed_time);
	}
	if (desastal_alpha_p1 > 0.0f)
	{
		desastal_alpha_p1 -= (alpha_down * elapsed_time);
	}
	else
	{
		desastal_alpha_p1 = 0.0f;
	}

	//2P
	//前回フレームと現在のディザスタルの数が異なる場合
	if (pl2_before_power != player2p->power)
	{
		desastal_alpha_p2 = 1.0f;
		desastal_size_p2 = 0.0f;
		//前回フレームより現在のディザスタルの数が多い場合
		if (pl2_before_power < player2p->power)
		{
			GetSound().SESinglePlay(SEKind::GAUGE_UP);
		}
	}
	if (desastal_size_p2 < 5.0f)
	{
		desastal_size_p2 += (size_up * elapsed_time);
	}
	if (desastal_alpha_p2 > 0.0f)
	{
		desastal_alpha_p2 -= (alpha_down * elapsed_time);
	}
	else
	{
		desastal_alpha_p2 = 0.0f;
	}


	pl1_before_power = player1p->power;
	pl2_before_power = player2p->power;
}



void SceneGame::ComboDraw()
{
	//combo_img->DrawRotaGraph
	//(
	//	spriteShader.get(),
	//	1720.0f,
	//	420.0f,
	//	0.0f,
	//	0.3f
	//);
	//font_img->DrawRotaDivGraph
	//(
	//	spriteShader.get(),
	//	1700.0f,
	//	320.0f,
	//	0.0f,
	//	0.8,
	//	p1combo[2]
	//);
	//font_img->DrawRotaDivGraph
	//(
	//	spriteShader.get(),
	//	1700.0f -100.0f,
	//	320.0f,
	//	0.0f,
	//	0.8f,
	//	p1combo[1]
	//);
	//font_img->DrawRotaDivGraph
	//(
	//	spriteShader.get(),
	//	1700.0f - 200.0f,
	//	320.0f,
	//	0.0f,
	//	0.8f,
	//	p1combo[0]
	//);







	//combo_img->DrawRotaGraph
	//(
	//	spriteShader.get(),
	//	340.0f,
	//	420.0f,
	//	0.0f,
	//	0.3f
	//);

	//font_img->DrawRotaDivGraph
	//(
	//	spriteShader.get(),
	//	320.0f,
	//	320.0f,
	//	0.0f,
	//	0.8f,
	//	p2combo[2]
	//);
	//font_img->DrawRotaDivGraph
	//(
	//	spriteShader.get(),
	//	320.0f - 100.0f,
	//	320.0f,
	//	0.0f,
	//	0.8f,
	//	p2combo[1]
	//);
	//font_img->DrawRotaDivGraph
	//(
	//	spriteShader.get(),
	//	320.0f - 200.0f,
	//	320.0f,
	//	0.0f,
	//	0.8f,
	//	p2combo[0]
	//);












	if (player1p->combo_count > 1)
	{

		font_img->DrawRotaDivGraph
		(
			spriteShader.get(),
			1700.0f,
			320.0f,
			0.0f,
			0.8,
			p1combo[2]
		);
		if (player1p->combo_count > 9)
		{
			font_img->DrawRotaDivGraph
			(
				spriteShader.get(),
				1700.0f - 100.0f,
				320.0f,
				0.0f,
				0.8f,
				p1combo[1]
			);
		}
		if (player1p->combo_count > 99)
		{
			font_img->DrawRotaDivGraph
			(
				spriteShader.get(),
				1700.0f - 200.0f,
				320.0f,
				0.0f,
				0.8f,
				p1combo[0]
			);
		}
		combo_img->DrawRotaGraph
		(
			spriteShader.get(),
			1720.0f,
			420.0f,
			0.0f,
			0.3f
		);
	}

	if (player2p->combo_count > 1)
	{

		font_img->DrawRotaDivGraph
		(
			spriteShader.get(),
			320.0f,
			320.0f,
			0.0f,
			0.8f,
			p2combo[2]
		);
		if (player2p->combo_count > 9)
		{
			font_img->DrawRotaDivGraph
			(
				spriteShader.get(),
				320.0f - 100.0f,
				320.0f,
				0.0f,
				0.8f,
				p2combo[1]
			);
		}
		if (player2p->combo_count > 99)
		{
			font_img->DrawRotaDivGraph
			(
				spriteShader.get(),
				320.0f - 200.0f,
				320.0f,
				0.0f,
				0.8f,
				p2combo[0]
			);
		}
		combo_img->DrawRotaGraph
		(
			spriteShader.get(),
			340.0f,
			420.0f,
			0.0f,
			0.3f
		);
	}
}