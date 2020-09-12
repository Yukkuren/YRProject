#include "Scene.h"
#include "framework.h"
#include <stdlib.h>
#include <time.h>
#include "YRMouse.h"
#include "framework.h"
#include "HitCheak.h"
#include "World.h"

//キャラクターインクルード
#include "Knight.h"

//------------------------------------------------
// **シーン概要**
//・このシーンの処理でプレイヤーモデルのロードは行わない
//
//------------------------------------------------


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
	fedo_alpha			= 1.0f;
	fedo_start			= false;
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
	mix_fedo			= 3.0f;
	main_loop = MAIN_LOOP::INTRO1P;	//最初は1Pのイントロから開始

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
	if (ParallelToonShader == nullptr)
	{
		ParallelToonShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::TOON);
		ParallelToonShader->Create("./Data/Shader/ParallelToon_vs.cso", "./Data/Shader/ParallelToon_ps.cso", "./Data/Shader/ParallelToon_gs.cso");
	}
	if (ToonShader == nullptr)
	{
		ToonShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::TOON);
		ToonShader->Create("./Data/Shader/ToonShader_vs.cso", "./Data/Shader/ToonShader_ps.cso", "./Data/Shader/ToonShader_gs.cso");
	}

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
}







void SceneGame::LoadData()
{
	//モデル等のロード
	//この関数はSceneLoadで別スレッドとして動かす
	if (geo == nullptr)
	{
		geo = std::make_unique<geometric_primitive>();
	}

	//if (skin == nullptr)
	//{
	//	//skin = std::make_unique<Skinned_mesh>("./Data/FBX/knight.fbx");
	//}
	/*if (knight2P_texture == nullptr)
	{
		knight2P_texture = std::make_shared<Texture>(L"./Data/FBX/Knight/knight_tex_nofaces.png");
	}*/
	
#if USE_IMGUI
	/*if (box_texture == nullptr)
	{
		box_texture = std::make_shared<Texture>(L"./Data/FBX/danbo_fbx/texture/danbo_face_c2.png");
	}

	if (box == nullptr)
	{
		box = std::make_unique<Skinned_mesh>("./Data/FBX/danbo_fbx/danbo_taiki.fbx", box_texture);
	}*/
#endif // USE_IMGUI

	

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
	

	

	//選択したキャラクターをそれぞれ生成する
	SetPlayerCharacter(player1p, FRAMEWORK.sceneselect.select_p1);
	SetPlayerCharacter(player2p, FRAMEWORK.sceneselect.select_p2);
	//生成後初期化する(座標系、HP、UI座標など)
	player1p->Init(PL.pos1P);
	player2p->Init(PL.pos2P);
	player1p->LoadData(1);
	player2p->LoadData(2);
	//キャラにどのプレイヤーが操作しているかの情報を与える
	player1p->now_player = 1;
	player2p->now_player = 2;
	PL.HP_MAX1P = player1p->hp;
	PL.HP_MAX2P = player2p->hp;
	PL.ratio1P = player1p->hp / PL.HP_MAX1P * 800.0f;
	PL.ratio2P = player2p->hp / PL.HP_MAX2P * 800.0f;
	PL.correction_value = 800.0f - PL.ratio1P;
	//タイトルで決定したパッドの順番をこちらにも設定して初期化
	PadSet(FRAMEWORK.scenetitle.select_p1, FRAMEWORK.scenetitle.select_p2);
	player1p->pad->Init();
	player2p->pad->Init();

	//ロード終了
	FRAMEWORK.sceneload.load_state = 7;
}






void SceneGame::UnInit()
{
	//プレイヤーのUninit関数を回す
	player1p->Uninit();
	player2p->Uninit();
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
}





void SceneGame::StartSet()
{
	//イントロ終了後のゲーム画面のセット

	//カメラ初期座標
	Scene_eye = YR_Vector3(0.0f, 6.0f, -140.0f);
	Scene_focus = YR_Vector3(0.0f, 6.0f, 0.0f);
	Scene_up = YR_Vector3(0.0f, 1.0f, 0.0f);
	Scene_fov = 10.0f * 0.01745f;
	Scene_aspect = 1920.0f / 1080.0f;
	Scene_nearZ = 1.4f;
	Scene_farZ = 1000.0f;

	//カメラ初期設定
	YRCamera.SetEye(Scene_eye.GetDXFLOAT3());			//視点
	YRCamera.SetFocus(Scene_focus.GetDXFLOAT3());			//注視点
	YRCamera.SetUp(Scene_up.GetDXFLOAT3());				//上方向
	YRCamera.SetPerspective(Scene_fov, Scene_aspect, Scene_nearZ, Scene_farZ);
	//YRCamera.SetPerspective(1080.0f, 1920.0f, 0.0001f, 1000000.0f);

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
	
	if (fedo_start)
	{
		//フェードアウト中
		switch (main_loop)
		{
			//イントロを飛ばす場合はゲームメインへ
		case SceneGame::INTRO1P:
		case SceneGame::INTRO2P:
			fedo_alpha += (elapsed_time * 5.0f);
			if (fedo_alpha > 1.0f)
			{
				main_loop = MAIN_LOOP::READY;
				YRCamera.camera_state = Camera::CAMERA_STATE::MAIN;
				//ここでメイン画面設定関数を呼ぶ
				StartSet();
				fedo_start = false;
			}
			break;
		case SceneGame::READY:
			break;
		case SceneGame::MAIN:
			if (FedoOut(elapsed_time))
			{
				UnInit();
				FRAMEWORK.SetScene(SCENE_TABLE::SCENE_SELECT);
			}
			break;
		case SceneGame::FINISH:
			fedo_alpha += (elapsed_time * 5.0f);
			if (fedo_alpha > 1.0f)
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
				mix_fedo = 5.0f;
				fedo_start = false;
			}
			break;
		case SceneGame::WIN1P:
		case SceneGame::WIN2P:
			if (FedoOut(elapsed_time))
			{
				fedo_start = false;
				main_loop = SceneGame::GAME_FIN;
				//ここでゲーム終了後の画面設定をする
				FinSet();
			}
			break;
		case SceneGame::GAME_FIN:
			if (FedoOut(elapsed_time))
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
			player1p->pad->Update(elapsed_time);
			player2p->pad->Update(elapsed_time);

			//1Pイントロ更新
			if (player1p->Intro())
			{
				main_loop = MAIN_LOOP::INTRO2P;
			}

			//途中ボタンが押されたときはスキップ
			if (player1p->pad->x_input[scastI(PAD::X)] == 1 ||
				player2p->pad->x_input[scastI(PAD::X)] == 1)
			{
				fedo_start = true;
			}
			break;
		case SceneGame::INTRO2P:
			//プレイヤー2のイントロ
			YRCamera.camera_state = Camera::CAMERA_STATE::PLAYER2P;
			//パッドの更新
			player1p->pad->Update(elapsed_time);
			player2p->pad->Update(elapsed_time);

			//2Pイントロ更新
			if (player2p->Intro())
			{
				fedo_start = true;
			}

			//途中ボタンが押されたらスキップ
			if (player1p->pad->x_input[scastI(PAD::X)] == 1 ||
				player2p->pad->x_input[scastI(PAD::X)] == 1)
			{
				fedo_start = true;
			}
			break;
		case SceneGame::READY:
			//イントロ終了後のフェードイン
			mix_fedo = 3.0f;
			if (fedo_alpha < 0.1f)
			{
				main_loop = MAIN_LOOP::MAIN;
			}
			break;
		case SceneGame::MAIN:
			if (start)
			{
				//イントロがすべて終わり、カウントも終えゲームが開始された
				
				//パッド更新
				player1p->pad->Update(elapsed_time);
				player2p->pad->Update(elapsed_time);
				if (pause)
				{
					//ポーズ中
					PauseUpdate();
				}
				else
				{
					//対戦中


					//カメラリクエスト更新
					CameraRequest(elapsed_time);

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
							if (hit_stop_elapsed > 0.05f)
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
					Limit::Set(player1p->pos, player2p->pos);

					//ホーミングダッシュ用の値を変更する
					TrackSet();

					if (end)
					{
						//勝敗がついた
						endtimer += elapsed_time;
						
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
							fedo_start = true;
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
							pause = TRUE;
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
							player1p->Update(pl1_rightorleft, elapsed_time * p1_elapsed_time);
							player2p->Update(pl2_rightorleft, elapsed_time * p2_elapsed_time);
							break;
						case Camera::CAMERA_STATE::PLAYER1P:
							player1p->Update(pl1_rightorleft, elapsed_time * p1_elapsed_time);
							break;
						case Camera::CAMERA_STATE::PLAYER2P:
							player2p->Update(pl2_rightorleft, elapsed_time * p2_elapsed_time);
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
					start_timer += elapsed_time;
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
			if (fedo_alpha < 0.1f)
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
			player1p->pad->Update(elapsed_time);
			player2p->pad->Update(elapsed_time);

			if (player1p->WinPerformance())
			{
				fedo_start = true;
			}

			//途中ボタンが押されたらスキップ
			if (player1p->pad->x_input[scastI(PAD::X)] == 1 ||
				player2p->pad->x_input[scastI(PAD::X)] == 1)
			{
				fedo_start = true;
			}
			break;
		case SceneGame::WIN2P:

			//パッドの更新
			player1p->pad->Update(elapsed_time);
			player2p->pad->Update(elapsed_time);

			if (player2p->WinPerformance())
			{
				fedo_start = true;
			}

			//途中ボタンが押されたらスキップ
			if (player1p->pad->x_input[scastI(PAD::X)] == 1 ||
				player2p->pad->x_input[scastI(PAD::X)] == 1)
			{
				fedo_start = true;
			}
			break;
		case SceneGame::GAME_FIN:
			FinUpdate();
			break;
		default:
			break;
		}
		//フェードアウトがスタートしてない場合は画面を映す
		if (fedo_alpha > 0.0f)
		{
			fedo_alpha -= (elapsed_time * mix_fedo);
		}
	}
}







void SceneGame::Draw(float elapsed_time)
{
	static float light_color[4] = { 1,1,1,1 };
	//ライト方向
	static DirectX::XMFLOAT4 light_direction = DirectX::XMFLOAT4(-1, 0, 1, 0);
	static DirectX::XMFLOAT4 ambient_color(0.3f, 0.3f, 0.3f, 0.5f);
	static float anim_count = 0.0f;
	static DirectX::XMFLOAT3 box_angle = { DirectX::XMConvertToRadians(-90.0f),0.0f,0.0f };
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
		ImGui::Checkbox("Camera_Debug", &camera_move_debug);
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
		ImGui::InputFloat("box_angle.x", &box_angle.x, 0.01f, 0.01f);
		ImGui::InputFloat("box_angle.y", &box_angle.y, 0.01f, 0.01f);
		ImGui::InputFloat("box_angle.z", &box_angle.z, 0.01f, 0.01f);
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
	test->DrawRotaGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 0.5f);
	
	/*geo->render(
		geoShader.get(),
		DirectX::XMFLOAT3(3.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(5.0f, 3.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		V,
		P,
		DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f)
	);*/

	switch (main_loop)
	{
	case SceneGame::INTRO1P:
		//1Pのイントロ
		//プレイヤー描画
		player1p->Draw(ParallelToonShader.get(),ToonShader.get(),V, P, light_direction, lightColor, ambient_color, elapsed_time);
		break;
	case SceneGame::INTRO2P:
		//2Pのイントロ
		//プレイヤー描画
		player2p->Draw(ParallelToonShader.get(), ToonShader.get(), V, P, light_direction, lightColor, ambient_color, elapsed_time);
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
		player1p->Draw(ParallelToonShader.get(), ToonShader.get(), V, P, light_direction, lightColor, ambient_color, elapsed_time*p1_elapsed_time);
		player2p->Draw(ParallelToonShader.get(), ToonShader.get(), V, P, light_direction, lightColor, ambient_color, elapsed_time*p2_elapsed_time);
		
		/*skin->Render(
			skinShader.get(), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f),
			DirectX::XMFLOAT3(DirectX::XMConvertToRadians(90.0f), 0.0f, 0.0f),
			V, P, light_direction, lightColor, ambient_color, elapsed_time, 0.0f
		);*/

#if USE_IMGUI
		player1p->DrawDEBUG(geoShader.get(), V, P, light_direction, lightColor, ambient_color, elapsed_time*p1_elapsed_time);
		player2p->DrawDEBUG(geoShader.get(), V, P, light_direction, lightColor, ambient_color, elapsed_time*p2_elapsed_time);
		
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
		player1p->Draw(ParallelToonShader.get(), ToonShader.get(), V, P, light_direction, lightColor, ambient_color, elapsed_time);
		break;
	case SceneGame::WIN2P:
		//プレイヤー描画
		player2p->Draw(ParallelToonShader.get(), ToonShader.get(), V, P, light_direction, lightColor, ambient_color, elapsed_time);
		break;
	case SceneGame::GAME_FIN:
		break;
	default:
		break;
	}

	

	//フェード用画像描画
	FRAMEWORK.fedo_img->DrawRotaGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 1.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, fedo_alpha));
	
}

bool SceneGame::FedoOut(float elapsed_time)
{
	fedo_alpha += FEDO_MIX(elapsed_time);

	if (fedo_alpha > 1.0f)
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
	//ポーズ中行う処理
	if (player1p->pad->x_input[scastI(PAD::START)] == 1 || player2p->pad->x_input[scastI(PAD::START)] == 1)
	{
		pause = FALSE;
		//pauseTimer = 20;
	}
	if (player1p->pad->x_input[scastI(PAD::LB)] == 1 ||player2p->pad->x_input[scastI(PAD::LB)] == 1)
	{
		//セレクト画面に戻る
		fedo_start = true;
		//FRAMEWORK.SetScene(SCENE_SELECT);
	}
}




void SceneGame::TrackSet()
{
	//ホーミングダッシュ用の値を変更する
	player1p->tracking.rival_Pos = player2p->pos;
	player2p->tracking.rival_Pos = player1p->pos;
	//player1p->rival_state = player2p->state;
	//player2p->rival_state = player1p->state;
}


void SceneGame::FinUpdate()
{
	//ゲーム終了後の処理
	fedo_start = true;
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
		float elap = 50.0f;

		DirectX::XMVECTOR scene_eye_vector = DirectX::XMLoadFloat3(&Scene_eye.GetDXFLOAT3());
		DirectX::XMVECTOR eye_vector = DirectX::XMLoadFloat3(&eye);
		DirectX::XMVECTOR scene_focus_vector = DirectX::XMLoadFloat3(&Scene_focus.GetDXFLOAT3());
		DirectX::XMVECTOR focus_vector = DirectX::XMLoadFloat3(&focus);
		DirectX::XMVECTOR scene_fov_vector = DirectX::XMLoadFloat(&Scene_fov);
		DirectX::XMVECTOR fov_vector = DirectX::XMLoadFloat(&fov);
		
		DirectX::XMVECTOR eye_larp = DirectX::XMVectorLerp(eye_vector, scene_eye_vector, 0.05f);
		DirectX::XMVECTOR focus_larp = DirectX::XMVectorLerp(focus_vector, scene_focus_vector, 0.05f);
		DirectX::XMVECTOR fov_larp = DirectX::XMVectorLerp(fov_vector, scene_fov_vector, 0.9);

		
		DirectX::XMStoreFloat3(&eye, eye_larp);
		DirectX::XMStoreFloat3(&focus, focus_larp);
		DirectX::XMStoreFloat(&fov, fov_larp);

		/*if (eye.x > Scene_eye.x)
			{
				eye.x -= elapsed_time*elap;
			}
			if (eye.x < Scene_eye.x)
			{
				eye.x += elapsed_time * elap;
			}
			if (eye.y > Scene_eye.y)
			{
				eye.y -= elapsed_time * elap;
			}
			if (eye.y < Scene_eye.y)
			{
				eye.y += elapsed_time * elap;
			}
			if (eye.z > Scene_eye.z)
			{
				eye.z -= elapsed_time*elap;
			}
			if (eye.z < Scene_eye.z)
			{
				eye.z += elapsed_time * elap;
			}
			if (focus.x > Scene_focus.x)
			{
				focus.x -= elapsed_time * elap;
			}
			if (focus.x < Scene_focus.x)
			{
				focus.x += elapsed_time * elap;
			}
			if (focus.y > Scene_focus.y)
			{
				focus.y -= elapsed_time * elap;
			}
			if (focus.y < Scene_focus.y)
			{
				focus.y += elapsed_time * elap;
			}
			if (focus.z > Scene_focus.z)
			{
				focus.z -= elapsed_time * elap;
			}
			if (focus.z < Scene_focus.z)
			{
				focus.z += elapsed_time * elap;
			}*/
		/*if (fov < Scene_fov)
			{
				fov += elapsed_time*elap;
			}
			if (fov > Scene_fov)
			{
				fov -= elapsed_time*elap;
			}*/
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
//----------------------------------------------------------------------------------------------