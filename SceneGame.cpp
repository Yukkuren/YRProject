#include "Scene.h"
#include "framework.h"
#include <stdlib.h>
#include <time.h>
#include "YRMouse.h"
#include "framework.h"
#include "HitCheak.h"

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
	timer = 0.0f;
	fedo_alpha = 1.0f;
	fedo_start = false;

	//シェーダー作成
	spriteShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::SPRITE);
	spriteShader->Create("./Data/Shader/sprite_vs.cso", "./Data/Shader/sprite_ps.cso");
	skinShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::SKIN);
	skinShader->Create("./Data/Shader/Skinned_VS.cso", "./Data/Shader/Skinned_PS.cso");
	geoShader = std::make_unique<YRShader>(INPUT_ELEMENT_DESC::ShaderType::GEO);
	geoShader->Create("./Data/Shader/board_vs.cso", "./Data/Shader/board_ps.cso");

	//カメラ初期設定
	YRCamera.SetEye(DirectX::XMFLOAT3(0, 0, -25));			//視点
	YRCamera.SetFocus(DirectX::XMFLOAT3(0, 0, 0));			//注視点
	YRCamera.SetUp(DirectX::XMFLOAT3(0, 1, 0));				//上方向
	YRCamera.SetPerspective(30 * 0.01745f, 1280.0f / 720.0f, 0.0001f, 1000000);
}







void SceneGame::LoadData()
{
	//モデル等のロード
	//この関数はSceneLoadで別スレッドとして動かす
	if (geo == nullptr)
	{
		geo = std::make_unique<geometric_primitive>();
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
	

	//選択したキャラクターをそれぞれ生成する
	SetPlayerCharacter(player1p, FRAMEWORK.sceneselect.select_p1);
	SetPlayerCharacter(player2p, FRAMEWORK.sceneselect.select_p2);
	//生成後初期化する(座標系、HP、UI座標など)
	player1p->Init(PL.pos1P);
	player2p->Init(PL.pos2P);
	player1p->LoadData();
	player2p->LoadData();
	PL.HP_MAX1P = player1p->hp;
	PL.HP_MAX2P = player2p->hp;
	PL.ratio1P = player1p->hp / PL.HP_MAX1P * 800.0f;
	PL.ratio2P = player2p->hp / PL.HP_MAX2P * 800.0f;
	PL.correction_value = 800.0f - PL.ratio1P;
	//タイトルで決定したパッドの順番をこちらにも設定して初期化
	PadSet(FRAMEWORK.sceneselect.select_p1, FRAMEWORK.sceneselect.select_p2);
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
	test = nullptr;
}







void SceneGame::Update(float elapsed_time)
{
	
	if (fedo_start)
	{
		//フェードアウト中
	}
	else
	{
		//フェードアウトがスタートしてない場合は画面を映す
		if (fedo_alpha > 0.0f)
		{
			fedo_alpha -= FEDO_MIX(elapsed_time);
		}
	}
}







void SceneGame::Draw(float elapsed_time)
{
	static float light_color[4] = { 1,1,1,1 };
	//ライト方向
	static DirectX::XMFLOAT4 light_direction = DirectX::XMFLOAT4(0, -1, 1, 0);
	static DirectX::XMFLOAT4 ambient_color(0.3f, 0.3f, 0.3f, 0.5f);
	static float anim_count = 0.0f;
#if USE_IMGUI
	//ImGui
	{
		bool show_demo_window = true;
		bool show_another_window = false;
		static int number_id = 0;
		ImGui::SetNextWindowSize(ImVec2(400, 800), 2);
		ImGui::Begin("palam", &show_another_window);
		//ImGui::Text("anim : %f", motion.anim_timer);
		ImGui::Text("time : %d", time);
		//ImGui::InputFloat("scroll", &scall, 0.01f, 100.0f);
		//ImGui::SliderFloat("rollX", &roll.x, 0.0f, 30.00f);
		//ImGui::SliderFloat("rollY", &roll.y, 0.0f, 30.00f);
		//ImGui::SliderFloat("rollZ", &roll.z, 0.0f, 30.00f);
		//ImGui::InputFloat("transX", &trans.x, 1.0f, 30.00f);
		//ImGui::InputFloat("transY", &trans.y, 1.0f, 30.00f);
		//ImGui::InputFloat("transZ", &trans.z, 1.0f, 30.00f);
		ImGui::ColorEdit4("light_color", light_color);
		ImGui::InputFloat("light_direction.x", &light_direction.x, 0.1f, 0.1f);
		ImGui::InputFloat("light_direction.y", &light_direction.y, 0.1f, 0.1f);
		ImGui::InputFloat("light_direction.z", &light_direction.z, 0.1f, 0.1f);
		ImGui::InputFloat("light_direction.w", &light_direction.w, 0.1f, 0.1f);
		ImGui::InputFloat("ambient_color.x", &ambient_color.x, 0.01f, 0.01f);
		ImGui::InputFloat("ambient_color.y", &ambient_color.y, 0.01f, 0.01f);
		ImGui::InputFloat("ambient_color.z", &ambient_color.z, 0.01f, 0.01f);
		ImGui::InputFloat("ambient_color.w", &ambient_color.w, 0.01f, 0.01f);
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
	
	//プレイヤー描画
	player1p->Draw(skinShader.get(), V, P, light_direction, lightColor, ambient_color, elapsed_time);
	player2p->Draw(skinShader.get(), V, P, light_direction, lightColor, ambient_color, elapsed_time);

	geo->render(
		geoShader.get(),
		DirectX::XMFLOAT3(3.0f, 0.0f, 0.0f),
		DirectX::XMFLOAT3(5.0f, 3.0f, 0.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		V,
		P,
		DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f)
	);


	//UI描画

	//体力バー表示
	PL.ratio1P = player1p->hp / PL.HP_MAX1P * 800.0f;
	PL.ratio2P = player2p->hp / PL.HP_MAX2P * 800.0f;
	PL.correction_value = 800.0f - PL.ratio1P;


	//コンボ表示
	



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



int SceneGame::ColorSet(int power)
{
	//ゲージの量からフォントの色を決め、ハンドル用の値を返す
	switch (power)
	{
	case 0:
		//SetDrawBright(255, 100, 0);
		return 0;
		break;
	case 1:
		//SetDrawBright(100, 255, 100);
		return 1;
		break;
	case 2:
		//SetDrawBright(0, 0, 255);
		return 2;
		break;
	case 3:
		//SetDrawBright(0, 255, 0);
		return 3;
		break;
	case 4:
		//SetDrawBright(255, 0, 0);
		return 4;
		break;
	case 5:
		//SetDrawBright(255, 255, 255);
		return 5;
		break;

	}
	return 0;
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



int SceneGame::Winjudge()
{
	if (player1p->hp <= 0 && player2p->hp <= 0)
	{
		player1p->hp = 0;
		player2p->hp = 0;
		return 3;
	}
	if (player1p->hp <= 0)
	{
		player1p->hp = 0;
		return 2;
	}
	if (player2p->hp <= 0)
	{
		player2p->hp = 0;
		return 1;
	}

	return 0;
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
		switch (s[i])
		{
		case 0:
			//IMG::p1combo[i] = IMG::font[0];
			break;
		case 1:
			//IMG::p1combo[i] = IMG::font[1];
			break;
		case 2:
			//IMG::p1combo[i] = IMG::font[2];
			break;
		case 3:
			//IMG::p1combo[i] = IMG::font[3];
			break;
		case 4:
			//IMG::p1combo[i] = IMG::font[4];
			break;
		case 5:
			//IMG::p1combo[i] = IMG::font[5];
			break;
		case 6:
			//IMG::p1combo[i] = IMG::font[6];
			break;
		case 7:
			//IMG::p1combo[i] = IMG::font[7];
			break;
		case 8:
			//IMG::p1combo[i] = IMG::font[8];
			break;
		case 9:
			//IMG::p1combo[i] = IMG::font[9];
			break;
		}

		switch (v[i])
		{
		case 0:
			//IMG::p2combo[i] = IMG::font[0];
			break;
		case 1:
			//IMG::p2combo[i] = IMG::font[1];
			break;
		case 2:
			//IMG::p2combo[i] = IMG::font[2];
			break;
		case 3:
			//IMG::p2combo[i] = IMG::font[3];
			break;
		case 4:
			//IMG::p2combo[i] = IMG::font[4];
			break;
		case 5:
			//IMG::p2combo[i] = IMG::font[5];
			break;
		case 6:
			//IMG::p2combo[i] = IMG::font[6];
			break;
		case 7:
			//IMG::p2combo[i] = IMG::font[7];
			break;
		case 8:
			//IMG::p2combo[i] = IMG::font[8];
			break;
		case 9:
			//IMG::p2combo[i] = IMG::font[9];
			break;
		}
	}
}
//
//----------------------------------------------------------------------------------------------