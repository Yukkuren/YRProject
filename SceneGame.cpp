#include "Scene.h"
#include "framework.h"
#include <stdlib.h>
#include <time.h>
#include "YRMouse.h"
#include "framework.h"
#include "HitCheak.h"
#include "World.h"
#include "Blur.h"

//キャラクターインクルード
#include "Knight.h"

//------------------------------------------------
// **シーン概要**
//・このシーンの処理でプレイヤーモデルのロードは行わない
//
//------------------------------------------------

#if USE_IMGUI

std::array<std::string, scastI(SceneGame::Player2PControl::END)> p2_con_name_list =
{
	u8"操作",
	u8"動かない",
	u8"AI",
};

#endif // USE_IMGUI


void SceneGame::SetPlayerCharacter(std::unique_ptr<Player>& player, int select)
{
	switch (select)
	{
	case scastI(PLSELECT::KNIGHT):
		player = std::make_unique<Knight>();
		break;
	case scastI(PLSELECT::KEN):
		player = std::make_unique<Knight>();
		break;

	}
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
	mix_fado			= 1.5f;
	main_loop = MAIN_LOOP::INTRO1P;	//最初は1Pのイントロから開始

	//カメラ初期設定
	StartSet();
	YRCamera.camera_state = Camera::CAMERA_STATE::MAIN;
	
	//画像選択位置初期化
	for (int i = 0; i < static_cast<int>(p1combo.size()); i++)
	{
		p1combo[i] = 0;
		p2combo[i] = 0;
	}



	/*motion.MeshSet(box);
	motion.AnimReset();*/

	p1_elapsed_time = 1.0f;
	p2_elapsed_time = 1.0f;
	camera_move_debug = false;
	hit_stop_elapsed = 0.0f;
	game_speed = 0.0f;
	blur_on = false;

	//2Pの動き
	pl2_con = Player2PControl::SUSPENSION;
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

	//コンスタントバッファ作成
	FRAMEWORK.CreateConstantBuffer(constantBuffer.GetAddressOf(), sizeof(GaussParamManager::GaussBlurParam));
	
	//テクスチャロード
	if (color_texture == nullptr)
	{
		color_texture = std::make_unique<Texture>();
		color_texture->Create(1920, 1080, DXGI_FORMAT_R8G8B8A8_UNORM);
		color_texture->CreateDepth(1920, 1080, DXGI_FORMAT_R24G8_TYPELESS);
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


	//画像のロード
	if (test == nullptr)
	{
		test = std::make_unique<Sprite>(L"./Data/Image/BG/stage1.png", 3840.0f, 2160.0f);
	}
	if (HP_img == nullptr)
	{
		HP_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/HP.png", 800.0f, 100.0f);
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
	if (gauge_img == nullptr)
	{
		gauge_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/gauge.png", 640.0f, 64.0f);
	}
	if (gaugecase_img == nullptr)
	{
		gaugecase_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/gaugecase.png", 640.0f, 64.0f);
	}
	if (font_img == nullptr)
	{
		font_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/font.png", 640.0f, 64.0f, 10, 1, 64.0f, 64.0f);
	}
	if (call_img == nullptr)
	{
		call_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/call.png", 1024.0f, 512.0f, 1, 2, 1024.0f, 256.0f);
	}
	if (effect_img == nullptr)
	{
		effect_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/effect.png", 192.0f, 128.0f, 3, 2, 64.0f, 64.0f);
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
	

	//ステージの選択
	stage.Init(Stage::StageType::NORMAL);
	

	//選択したキャラクターをそれぞれ生成する
	SetPlayerCharacter(player1p, FRAMEWORK.sceneselect.select_p1);
	SetPlayerCharacter(player2p, FRAMEWORK.sceneselect.select_p2);
	player1p->now_player = 1;
	player2p->now_player = 2;
	//生成後初期化する(座標系、HP、UI座標など)
	player1p->LoadData(1);
	player2p->LoadData(2);
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

	//ロード終了
	FRAMEWORK.sceneload.load_state = 7;
}






void SceneGame::UnInit()
{
	//プレイヤーのUninit関数を回す
	player1p->Uninit();
	player1p.reset();
	//player1p = nullptr;
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
	gauge_img.reset();
	gaugecase_img.reset();
	font_img.reset();
	call_img.reset();
	effect_img.reset();
	test = nullptr;
	geo = nullptr;
	HP_img = nullptr;
	win1P_img = nullptr;
	win2P_img = nullptr;
	draw_img = nullptr;
	HPbar_img = nullptr;
	KO_img = nullptr;
	gauge_img = nullptr;
	gaugecase_img = nullptr;
	font_img = nullptr;
	call_img = nullptr;
	effect_img = nullptr;

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
		case SceneGame::INTRO1P:
			fado_alpha += (elapsed_time * mix_fado);
			if (fado_alpha > 1.0f)
			{
				main_loop = MAIN_LOOP::INTRO2P;
				fado_start = false;
			}
			break;
		case SceneGame::INTRO2P:
			fado_alpha += (elapsed_time * mix_fado);
			if (fado_alpha > 1.0f)
			{
				main_loop = MAIN_LOOP::READY;
				YRCamera.camera_state = Camera::CAMERA_STATE::MAIN;
				StartSet();
				player1p->ReadySet();
				player2p->ReadySet();
				fado_start = false;
			}
			break;
		case SceneGame::READY:
			break;
		case SceneGame::MAIN:
			if (FadoOut(elapsed_time))
			{
				UnInit();
				FRAMEWORK.SetScene(SCENE_TABLE::SCENE_SELECT);
			}
			break;
		case SceneGame::FINISH:
			fado_alpha += (elapsed_time * 5.0f);
			if (fado_alpha > 1.0f)
			{
				switch (judge)
				{
				case SceneGame::NO_VICTORY:
					break;
				case SceneGame::VICTORY1P:
					//ここで勝利画面設定関数を呼ぶ
					player1p->WinAnimSet();
					break;
				case SceneGame::VICTORY2P:
					//ここで勝利画面設定関数を呼ぶ
					player2p->WinAnimSet();
					break;
				case SceneGame::DROW:
					//ここで勝利画面設定関数を呼ぶ

					break;
				default:
					break;
				}
				mix_fado = 5.0f;
				fado_start = false;
			}
			break;
		case SceneGame::WIN1P:
		case SceneGame::WIN2P:
			if (FadoOut(elapsed_time))
			{
				fado_start = false;
				main_loop = SceneGame::GAME_FIN;
				//ここでゲーム終了後の画面設定をする
				FinSet();
			}
			break;
		case SceneGame::GAME_FIN:
			if (FadoOut(elapsed_time))
			{
				UnInit();
				FRAMEWORK.SetScene(SCENE_TABLE::SCENE_SELECT);
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
		case SceneGame::INTRO1P:
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
			if (player1p->Intro(game_speed))
			{
				main_loop = MAIN_LOOP::INTRO1P;
				fado_start = true;
			}

			//途中ボタンが押されたときはスキップ
			if (player1p->pad->x_input[scastI(PAD::X)] == 1 ||
				player2p->pad->x_input[scastI(PAD::X)] == 1)
			{
				fado_start = true;
			}
			break;
		case SceneGame::INTRO2P:
			//プレイヤー2のイントロ
			YRCamera.camera_state = Camera::CAMERA_STATE::PLAYER2P;
			//パッドの更新
			player1p->pad->Update(game_speed);
			player2p->pad->Update(game_speed);

			//2Pイントロ更新
			if (player2p->Intro(elapsed_time))
			{
				fado_start = true;
				main_loop = MAIN_LOOP::INTRO2P;
			}

			//途中ボタンが押されたらスキップ
			if (player1p->pad->x_input[scastI(PAD::X)] == 1 ||
				player2p->pad->x_input[scastI(PAD::X)] == 1)
			{
				fado_start = true;
				main_loop = MAIN_LOOP::INTRO2P;
			}
			break;
		case SceneGame::READY:
			//イントロ終了後のフェードイン
			mix_fado = 3.0f;
			if (fado_alpha < 0.1f)
			{
				main_loop = MAIN_LOOP::MAIN;
			}
			break;
		case SceneGame::MAIN:
			if (start)
			{
				//イントロがすべて終わり、カウントも終えゲームが開始された
				
				//パッド更新
				player1p->pad->Update(game_speed);
				Control2PState(game_speed);
				if (pause)
				{
					//ポーズ中
					PauseUpdate();
				}
				else
				{
					//対戦中


					//カメラリクエスト更新
					CameraRequest(game_speed);

					//カメラの挙動をステートごとに処理
					CameraUpdate();

					//※要変更。ステートの[2]なんて見ただけじゃ何かわからない
					if (player1p->act_state != ActState::STATENONE && player2p->act_state != ActState::STATENONE)
					{
						//プレイヤーのステートが奪われた状態以外は押し出しを行う
						//Hitcheak::HitPlayer(player1p->GetHit(), player1p->pos.x, player2p->GetHit(), player2p->pos.x);
					}

					//攻撃同士の判定
					Hitcheak::HitCheakAttack(player1p->atk,player2p->atk);
					player1p->GaugeUp(Hitcheak::add1P);
					player2p->GaugeUp(Hitcheak::add2P);

					//攻撃と当たり判定の判定
					Hitcheak::add1P = Hitcheak::HitCheak(player1p->atk, player2p->GetHit(), player2p->GetMax(0), 2,player1p->pos);
					Hitcheak::add2P = Hitcheak::HitCheak(player2p->atk, player1p->GetHit(), player1p->GetMax(0), 1,player2p->pos);
					player1p->GaugeUp(Hitcheak::add1P);
					player2p->GaugeUp(Hitcheak::add2P);

					//ヒットストップ処理
					if (Hitcheak::hit)
					{
						game_speed = 0.0f;
						Hitcheak::timer -= elapsed_time;
						hit_stop_elapsed += elapsed_time;
						if (Hitcheak::timer < 0.0f)
						{
							Hitcheak::clash = false;
							Hitcheak::hit = false;
							Hitcheak::timer = 0.0f;
							player1p->StopEnd();
							player2p->StopEnd();
							Hitcheak::stop1p = false;
							Hitcheak::stop2p = false;
							player1p->stop_state = 0;
							player2p->stop_state = 0;
						}
						if (Hitcheak::timer != 0.0f)
						{
							/*PlayerALL::player1p->StopUpdate();
							PlayerALL::player2p->StopUpdate();*/
							if (hit_stop_elapsed > 0.01f)
							{
								if (Hitcheak::stop1p)
								{
									player1p->StopUpdate();
								}
								if (Hitcheak::stop2p)
								{
									player2p->StopUpdate();
								}
								hit_stop_elapsed = 0.0f;
							}
							return;
						}
					}
					if (!Hitcheak::hit && Hitcheak::timer > 0.0f)
					{
						Hitcheak::hit = true;
					}

					//プレイヤーの位置からカメラの位置を決定する
					YR_Vector3 camera_screen = Limit::Set(player1p->pos, player2p->pos,Start_Scene_eye);
					Scene_eye.x = camera_screen.x;
					Scene_eye.y = camera_screen.y;
					Scene_eye.z = camera_screen.z;
					Scene_focus.x = camera_screen.x;
					Scene_focus.y = camera_screen.y;

					//ホーミングダッシュ用の値を変更する
					TrackSet();

					if (end)
					{
						//勝敗がついた
						endtimer += game_speed;
						
						float now_elapsed = 0.0f;

						//勝敗決定後、スローにする時間を作りたいので
						if (endtimer > 3.0f)
						{
							now_elapsed = elapsed_time;
						}
						else
						{
							now_elapsed = elapsed_time * 0.5f;
						}

						//大体7秒後くらいに勝利画面へ
						if (endtimer > 7.0f)
						{
							main_loop = MAIN_LOOP::FINISH;
							fado_start = true;
						}

						//プレイヤー更新(KO時のｳﾜｧ...ｳﾜｧ...ｳﾜｧ...ってスローになるやつ)
						//1Pが左
						if (player1p->pos.x < player2p->pos.x)
						{
							player1p->Update(1.0f, now_elapsed);
							player2p->Update(-1.0f, now_elapsed);
						}
						//2Pが左
						else
						{
							player1p->Update(-1.0f, now_elapsed);
							player2p->Update(1.0f, now_elapsed);
						}
					}
					else
					{
						//未だ勝敗はつかず

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

						switch (YRCamera.camera_state)
						{
						case Camera::CAMERA_STATE::MAIN:
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
							break;
						}

						//プレイヤーの移動距離制限(※要変更)
						if (player1p->act_state != ActState::STATENONE && player2p->act_state != ActState::STATENONE)
						{
							Limit::Stop(player1p->pos.x, player2p->pos.x);
						}

						//コンボに応じて画像セット
						ComboImageSet();

						//勝敗判定
						if (judge != JUDGE_VICTORY::NO_VICTORY)
						{
							end = true;
						}
					}

				}
			}
			else
			{
				//カウント中
				if (start_timer < start_time)
				{
					start_timer += game_speed;
				}
				else
				{
					//カウントが既定に到達したのでゲーム開始
					start = true;
				}
			}
			break;
		case SceneGame::FINISH:
			//フェードインしたら勝敗に合わせてステートを変える
			if (fado_alpha < 0.1f)
			{
				switch (judge)
				{
				case SceneGame::NO_VICTORY:
					break;
				case SceneGame::VICTORY1P:
					main_loop = MAIN_LOOP::WIN1P;
					break;
				case SceneGame::VICTORY2P:
					main_loop = MAIN_LOOP::WIN2P;
					break;
				case SceneGame::DROW:
					break;
				default:
					break;
				}
				
			}
			break;
		case SceneGame::WIN1P:
			
			//パッドの更新
			player1p->pad->Update(game_speed);
			player2p->pad->Update(game_speed);

			if (player1p->WinPerformance())
			{
				fado_start = true;
			}

			//途中ボタンが押されたらスキップ
			if (player1p->pad->x_input[scastI(PAD::X)] == 1 ||
				player2p->pad->x_input[scastI(PAD::X)] == 1)
			{
				fado_start = true;
			}
			break;
		case SceneGame::WIN2P:

			//パッドの更新
			player1p->pad->Update(game_speed);
			player2p->pad->Update(game_speed);

			if (player2p->WinPerformance())
			{
				fado_start = true;
			}

			//途中ボタンが押されたらスキップ
			if (player1p->pad->x_input[scastI(PAD::X)] == 1 ||
				player2p->pad->x_input[scastI(PAD::X)] == 1)
			{
				fado_start = true;
			}
			break;
		case SceneGame::GAME_FIN:
			FinUpdate();
			break;
		default:
			break;
		}
		//フェードアウトがスタートしてない場合は画面を映す
		if (fado_alpha > 0.0f)
		{
			fado_alpha -= (game_speed * mix_fado);
		}
	}
}







void SceneGame::Draw(float elapsed_time)
{
	static float light_color[4] = { 1,1,1,1 };
	//ライト方向
	static DirectX::XMFLOAT4 light_direction = DirectX::XMFLOAT4(-1, 0, 1, 0);
	static DirectX::XMFLOAT4 ambient_color(0.3f, 0.3f, 0.3f, 0.5f);
	//FRAMEWORK.context.Get()->OMSetRenderTargets(1, FRAMEWORK.view.GetAddressOf(), FRAMEWORK.depth.Get());
	SetRenderTexture();
#if USE_IMGUI
	//ImGui
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
		ImGui::SliderInt(p2_con_name_list[pl2p_con].c_str(), &pl2p_con, 0, scastI(Player2PControl::AI));
		pl2_con = static_cast<Player2PControl>(pl2p_con);
		ImGui::Checkbox("Camera_Debug", &camera_move_debug);
		if (ImGui::Button(u8"カメラをプレイヤー1に"))
		{
			eye = player1p->pos.GetDXFLOAT3();
			focus = player1p->pos.GetDXFLOAT3();
			eye.z -= 10.0f;
		}
		//ImGui::InputFloat("scroll", &scall, 0.01f, 100.0f);
		//ImGui::SliderFloat("rollX", &roll.x, 0.0f, 30.00f);
		//ImGui::SliderFloat("rollY", &roll.y, 0.0f, 30.00f);
		//ImGui::SliderFloat("rollZ", &roll.z, 0.0f, 30.00f);
		//ImGui::InputFloat("transX", &trans.x, 1.0f, 30.00f);
		//ImGui::InputFloat("transY", &trans.y, 1.0f, 30.00f);
		//ImGui::InputFloat("transZ", &trans.z, 1.0f, 30.00f);
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

		YRCamera.SetEye(eye);
		YRCamera.SetFocus(focus);
		YRCamera.SetPerspective(fov* 0.01745f, aspect, nearZ, farZ);
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
	stage.Draw(V, P, light_direction, lightColor, ambient_color, elapsed_time);

	switch (main_loop)
	{
	case SceneGame::INTRO1P:
		//1Pのイントロ
		//プレイヤー描画
		player1p->Draw(ParallelToonShader.get(),ToonShader.get(),V, P, light_direction, lightColor, ambient_color, game_speed);
		player1p->IntroDEBUG();
		break;
	case SceneGame::INTRO2P:
		//2Pのイントロ
		//プレイヤー描画
		player2p->Draw(ParallelToonShader.get(), ToonShader.get(), V, P, light_direction, lightColor, ambient_color, game_speed);
		break;
	case SceneGame::READY:
	case SceneGame::MAIN:
	case SceneGame::FINISH:
		//内部処理ではフェードをしているだけで画面に変化はない為一括

		//カウント表示
		if (!start)
		{
			//Are You Ready?
			if (start_timer < 1.0f)
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

		//UI描画
		//体力バー表示
		PL.ratio1P = player1p->hp / PL.HP_MAX1P * 800.0f;
		PL.ratio2P = player2p->hp / PL.HP_MAX2P * 800.0f;
		PL.correction_value = 800.0f - PL.ratio1P;

		HPbar_img->DrawExtendGraph(spriteShader.get(), 75.0f, 75.0f, 925.0f, 225.0f);
		HPbar_img->DrawExtendGraph(spriteShader.get(), 975.0f, 75.0f, 1825.0f, 225.0f);
		HP_img->DrawRectGraph(spriteShader.get(), 100.0f + PL.correction_value, 100.0f, 800.0f - PL.ratio1P, 0.0f, PL.ratio1P, 100.0f);
		HP_img->DrawRectGraph(spriteShader.get(), 1000.0f, 100.0f, 0.0f, 0.0f, PL.ratio2P, 100.0f);

		//コンボ表示

		if (player1p->combo_count > 1)
		{
			font_img->DrawRotaDivGraph
			(
				spriteShader.get(),
				300.0f,
				400.0f,
				0.0f,
				3.0f,
				p1combo[0],
				DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
			);
			if (player1p->combo_count > 9)
			{
				font_img->DrawRotaDivGraph
				(
					spriteShader.get(),
					200.0f,
					400.0f,
					0.0f,
					3.0f,
					p1combo[1],
					DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
				);
			}
			if (player1p->combo_count > 99)
			{
				font_img->DrawRotaDivGraph
				(
					spriteShader.get(),
					100.0f,
					400.0f,
					0.0f,
					3.0f,
					p1combo[2],
					DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
				);
			}
		}

		if (player2p->combo_count > 1)
		{
			font_img->DrawRotaDivGraph
			(
				spriteShader.get(),
				1600.0f,
				400.0f,
				0.0f,
				3.0f,
				p2combo[2],
				DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
			);
			if (player2p->combo_count > 9)
			{
				font_img->DrawRotaDivGraph
				(
					spriteShader.get(),
					1500.0f,
					400.0f,
					0.0f,
					3.0f,
					p2combo[1],
					DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
				);
			}
			if (player2p->combo_count > 99)
			{
				font_img->DrawRotaDivGraph
				(
					spriteShader.get(),
					1400.0f,
					400.0f,
					0.0f,
					3.0f,
					p2combo[0],
					DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)
				);
			}
		}

		//プレイヤー描画
		player1p->Draw(ParallelToonShader.get(), ToonShader.get(), V, P, light_direction, lightColor, ambient_color, game_speed*p1_elapsed_time);
		player2p->Draw(ParallelToonShader.get(), ToonShader.get(), V, P, light_direction, lightColor, ambient_color, game_speed*p2_elapsed_time);
		
		/*skin->Render(
			skinShader.get(), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f),
			DirectX::XMFLOAT3(DirectX::XMConvertToRadians(90.0f), 0.0f, 0.0f),
			V, P, light_direction, lightColor, ambient_color, elapsed_time, 0.0f
		);*/

#if USE_IMGUI
		player1p->DrawDEBUG(geoShader.get(), V, P, light_direction, lightColor, ambient_color, game_speed *p1_elapsed_time);
		player2p->DrawDEBUG(geoShader.get(), V, P, light_direction, lightColor, ambient_color, game_speed *p2_elapsed_time);
		
		/*motion.DrawContinue(
		skinShader.get(),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f),
		box_angle,
		V, P, light_direction, lightColor, ambient_color, elapsed_time
	);*/

		YRCamera.CameraMove(spriteShader.get());
#endif // USE_IMGUI
		

		//ゲージ描画
		PL.gauge1P = (player1p->gauge / GAUGE_MAX) * 640.0f;
		PL.gauge2P = (player2p->gauge / GAUGE_MAX) * 640.0f;

		PL.power1P = ColorSet(player1p->power);
		gauge_img->DrawExtendGraph(spriteShader.get(), 100.0f, 1000.0f, 100.0f + PL.gauge1P, 1064.0f, PL.power1P);
		PL.power2P = ColorSet(player2p->power);
		gauge_img->DrawExtendGraph(spriteShader.get(), 1800.0f - PL.gauge2P, 1000.0f, 1800.0f, 1064.0f, PL.power2P);

		//ゲージケース
		gaugecase_img->DrawExtendGraph(spriteShader.get(), 100.0f, 1000.0f, 100.0f + 640.0f, 1064.0f);
		gaugecase_img->DrawExtendGraph(spriteShader.get(), 1800.0f - 640.0f, 1000.0f, 1800.0f, 1064.0f);

		//ゲージの数字描画
		font_img->DrawRotaDivGraph
		(
			spriteShader.get(),
			70.0f,
			950.0f,
			0.0f,
			2.0f,
			player1p->power,
			PL.power1P
		);
		font_img->DrawRotaDivGraph
		(
			spriteShader.get(),
			1800.0f,
			950.0f,
			0.0f,
			2.0f,
			player2p->power,
			PL.power2P
		);
		break;
	case SceneGame::WIN1P:
		//プレイヤー描画
		player1p->Draw(ParallelToonShader.get(), ToonShader.get(), V, P, light_direction, lightColor, ambient_color, game_speed);
		break;
	case SceneGame::WIN2P:
		//プレイヤー描画
		player2p->Draw(ParallelToonShader.get(), ToonShader.get(), V, P, light_direction, lightColor, ambient_color, game_speed);
		break;
	case SceneGame::GAME_FIN:
		break;
	default:
		break;
	}

	//フェード用画像描画
	FRAMEWORK.fedo_img->DrawRotaGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 1.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, fado_alpha));

	NullSetRenderTexture();
	RenderTexture();
	RenderBlur();
	FRAMEWORK.framebuffer.Deactivate();
	//FRAMEWORK.framebuffer.SetDefaultRTV();
	//player1p->TextDraw();
	//player2p->TextDraw();

}

bool SceneGame::FadoOut(float elapsed_time)
{
	fado_alpha += mix_fado*(elapsed_time);

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
		return DirectX::XMFLOAT4(0.4f,1.0f,0.4f,1.0f);
		break;
	case 2:
		return DirectX::XMFLOAT4(0.0f,0.0f,1.0f,1.0f);
		break;
	case 3:
		return DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
		break;
	case 4:
		return DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case 5:
		return DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		break;

	}
	return DirectX::XMFLOAT4(1.0f, 0.4f, 0.0f, 1.0f);
}


void SceneGame::ScoreImageSet()
{
	int s[6];
	int dScore = 0;/*pPlayer.score;*/
	s[0] = dScore / 100000;
	dScore = dScore % 100000;
	s[1] = dScore / 10000;
	dScore = dScore % 10000;
	s[2] = dScore / 1000;
	dScore = dScore % 1000;
	s[3] = dScore / 100;
	dScore = dScore % 100;
	s[4] = dScore / 10;
	dScore = dScore % 10;
	s[5] = dScore;

	for (int i = 0; i < 6; i++)
	{
		switch (s[i])
		{
		case 0:
			sco[i] = 0;
			break;
		case 1:
			sco[i] = 1;
			break;
		case 2:
			sco[i] = 2;
			break;
		case 3:
			sco[i] = 3;
			break;
		case 4:
			sco[i] = 4;
			break;
		case 5:
			sco[i] = 5;
			break;
		case 6:
			sco[i] = 6;
			break;
		case 7:
			sco[i] = 7;
			break;
		case 8:
			sco[i] = 8;
			break;
		case 9:
			sco[i] = 9;
			break;
		}
	}
}



void SceneGame::Winjudge()
{
	if (player1p->hp <= 0 && player2p->hp <= 0)
	{
		player1p->hp = 0;
		player2p->hp = 0;
		judge = JUDGE_VICTORY::DROW;
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
		pause = false;
		//pauseTimer = 20;
	}
	if (player1p->pad->x_input[scastI(PAD::LB)] == 1 ||player2p->pad->x_input[scastI(PAD::LB)] == 1)
	{
		//fado_start = true;
		//FRAMEWORK.SetScene(SCENE_SELECT);
		player1p->Init(PL.pos1P);
		player2p->Init(PL.pos2P);
		Init();
		pause = false;
	}
}




void SceneGame::TrackSet()
{
	//ホーミングダッシュ用の値を変更する
	player1p->tracking.rival_Pos = player2p->pos;
	player2p->tracking.rival_Pos = player1p->pos;
}


void SceneGame::FinUpdate()
{
	//ゲーム終了後の処理
	fado_start = true;
}



void SceneGame::CameraUpdate()
{
	switch (YRCamera.camera_state)
	{
	case Camera::CAMERA_STATE::MAIN:
		//通常カメラ
		//カメラのステートがMAINにある場合のカメラ処理を行う
		if (!camera_move_debug)
		{
			YRCamera.SetEye(Scene_eye.GetDXFLOAT3());			//視点
			YRCamera.SetFocus(Scene_focus.GetDXFLOAT3());			//注視点
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


void SceneGame::SetRenderTexture()
{
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
	FRAMEWORK.framebuffer.GetDefaultRTV();
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

#if USE_IMGUI
	if (ImGui::TreeNode(u8"カラーテクスチャ"))
	{
		ImGui::Image((void*)(color_texture->GetShaderResource()), ImVec2(360, 360));
		ImGui::Image((void*)(luminance_texture->GetShaderResource()), ImVec2(360, 360));
		ImGui::TreePop();
	}
#endif // USE_IMGUI

}


void SceneGame::RenderBlur()
{
	static float off_x = 1.0f;
	static float off_y = 1.0f;
	static float deviation = 0.5f;
#if USE_IMGUI
	//ImGui
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


	GaussParamManager gauss;
	float w = (float)FRAMEWORK.SCREEN_WIDTH;
	float h = (float)FRAMEWORK.SCREEN_HEIGHT;

	if (blur_on)
	{
		//初回のみ現在の描画を利用する
		//テクスチャをセット
		FRAMEWORK.framebuffer.SetRenderTexture(blur_texture[0]->GetRenderTargetView(), true);
		ID3D11DepthStencilView* dsv = blur_texture[0]->GetDepthStencilView();

		//画面をクリア
		FRAMEWORK.framebuffer.Clear();
		FRAMEWORK.context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		//レンダーターゲットビューの設定
		FRAMEWORK.framebuffer.GetDefaultRTV();
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
			


			//レンダーターゲットビューの設定
			FRAMEWORK.framebuffer.Activate(dsv);

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
		FRAMEWORK.framebuffer.SetRenderTexture(multi_blur_texture->GetRenderTargetView(), true);
		//ID3D11DepthStencilView* dsv = color_texture->GetDepthStencilView();

		//画面をクリア
		FRAMEWORK.framebuffer.Clear();
		//FRAMEWORK.context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		//レンダーターゲットビューの設定
		FRAMEWORK.framebuffer.Activate(dsv);
		//ブレンドステート設定
		//FRAMEWORK.BlendSet(Blend::ALPHA);
		//ラスタライザー設定
		//FRAMEWORK.context->RSSetState(FRAMEWORK.rasterizer_state[framework::RS_CULL_BACK].Get());
		//デプスステンシルステート設定
		//FRAMEWORK.context->OMSetDepthStencilState(FRAMEWORK.depthstencil_state[framework::DS_TRUE].Get(), 1);

		//サンプラー設定
		//sampler_clamp->Set(0);
		FRAMEWORK.framebuffer.SetDefaultRTV();

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
		break;
	case SceneGame::Player2PControl::AI:
		//AI
		break;
	default:
		break;
	}
}