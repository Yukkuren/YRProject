#include "Scene.h"
#include "HitCheak.h"
#include "framework.h"
#include "YRSound.h"

//-------------------------------------------------------------
// **シーン概要**
//・このシーンではSceneGameのロードを行う
//・SceneSelectから送られてきた情報をもとにデータの読み込みを行い、
//	読み込みがすべて完了したらSceneGameをセットする
//-------------------------------------------------------------
float Hitcheak::timer = 0.0f;
bool Hitcheak::hit = false;
bool Hitcheak::stop1p = false;
bool Hitcheak::stop2p = false;
float Hitcheak::add1P = 0.0f;
float Hitcheak::add2P = 0.0f;
bool Hitcheak::clash = false;
YR_Vector3 Hitcheak::clashpos = { 0.0f,0.0f };
YR_Vector3 Hitcheak::effectpos = { 0.0f,0.0f };
int Hitcheak::effecttimer = 0;
//int Hitcheak::hei = 0;

void SceneLoad::Init()
{
	timer = 0;
	load_fin = false;
	load_state = 0;
	fado_alpha = 1.0f;
	fado_start = false;
	Game_load_fin = false;

	//シェーダー作成
	if (spriteShader == nullptr)
	{
		spriteShader = std::make_unique<YRShader>(ShaderType::SPRITE);
		spriteShader->Create("./Data/Shader/sprite_vs.cso", "./Data/Shader/sprite_ps.cso");
	}
	GetSound().BGMPlay(BGMKind::LOAD);
}

void SceneLoad::LoadData()
{
	//「ロード画面で表示する画像等」をロードする
	if (load_img == nullptr)
	{
		load_img = std::make_unique<Sprite>(
			L"./Data/Image/UI/GameLoad/load_image.png",
			300.0f,
			400.0f,
			3,
			4,
			100.0f,
			100.0f);
	}

	if (load_bg == nullptr)
	{
		load_bg = std::make_unique<Sprite>(
			L"./Data/Image/BG/Load_BG.png",
			3840.0f,
			2160.0f);
	}

	load_state = 2;
}

void SceneLoad::UnInit()
{
	//「ロード画面で表示する画像等」のみ解放する
	load_img.reset();
	load_img = nullptr;

	//シェーダーも解放
	spriteShader.reset();
	spriteShader = nullptr;

	if (load_state < 8)
	{
		if (t->joinable())
		{
			t->join();
		}
		delete t;
	}
}

void SceneLoad::Update(float elapsedTime)
{
	//タイマーは常に更新する
	timer += elapsedTime;
	GetSound().FadeIn(elapsedTime);
	//「ロード画面で表示する画像等」がロードできた
	if (load_fin)
	{
		if (!fado_start)
		{
			if (fado_alpha > 0.0f)
			{
				fado_alpha -= FADE_MIX(elapsedTime);
			}
		}
	}

	//「ゲームメイン画面で使用するモデル等」がロードできた
	if (Game_load_fin)
	{
		//ロード画面を挟むため
		if (timer > 1.0f)
		{
			fado_start = true;
		}
		if (fado_start)
		{
			if (FedoOut(elapsedTime))
			{
				//フェードアウトが終わったらゲームメインへ遷移
				//FRAMEWORK.SetScene(SCENE_TABLE::SCENE_GAME);
				GetSound().BGMStop(BGMKind::LOAD);
				FRAMEWORK.SetScene(SCENE_TABLE::SCENE_GAME);
				UnInit();
				return;
			}
		}
	}
	else
	{
		//-----------------------------------------------------------------------------------------
		//		*概要*
		//・「ロード画面で表示する画像等」と「ゲームメイン画面で使用するモデル等」の2つをロードするので
		//	スレッドを2回立てて順番にロードしている
		//・load_fin：「ロード画面で表示する画像等」がロードできた
		//・Game_load_fin：「ゲームメイン画面で使用するモデル等」がロードできた
		//-----------------------------------------------------------------------------------------
		switch (load_state)
		{
		case 0:
			//まずはロード画面で表示する画像等」のロード
			load_state = 1;
			t = new std::thread(&SceneLoad::LoadData, this);
			break;
		case 1:
			//ロードモデルロード中
			break;
		case 2:
			//ロード終了後に何か処理を入れるときのためにあけている
			load_state = 3;
			break;
		case 3:
			//一度スレッドを開放する
			if (t->joinable())
			{
				t->join();
			}
			delete t;
			load_fin = true;
			load_state = 4;
			break;
		case 4:
			//意図的に1フレームずらしている
			load_state = 5;
			break;
		case 5:
			//次に「ゲームメイン画面で使用するモデル等」のロード
			load_state = 6;
			t = new std::thread(&SceneGame::LoadData, &FRAMEWORK.scenegame);
			break;
		case 6:
			//ゲームモデルロード中
			break;
		case 7:
			//ロード終了後に何か処理を入れるときのためにあけている
			load_state = 8;
			break;
		case 8:
			//スレッド解放
			if (t->joinable())
			{
				t->join();
			}
			delete t;
			load_state = 9;
			break;
		case 9:
#ifdef EXIST_IMGUI
			if (Get_Use_ImGui())
			{
				Game_load_fin = true;
			}
#endif // EXIST_IMGUI

			if (timer > 10.0f)
			{
				Game_load_fin = true;
			}
			break;
		default:
			break;
		}
	}
}

void SceneLoad::Draw(float elapsedTime)
{
	//「ロード画面で表示する画像等」がロードできた
	if (load_fin)
	{
		load_bg->DrawRotaGraph(
			spriteShader.get(),
			static_cast<float>(FRAMEWORK.SCREEN_WIDTH) / 2.0f,
			static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) / 2.0f,
			0.0f,
			0.5f
		);

		load_img->DrawRotaDivGraph(
			spriteShader.get(),
			FRAMEWORK.SCREEN_WIDTH * 0.92f,
			FRAMEWORK.SCREEN_HEIGHT * 0.85f+(sinf(timer)*20.0f),
			0.0f,
			2.0f,
			0.05f,
			elapsedTime);
	}

	//フェード用画像描画
	FRAMEWORK.fade_img->DrawRotaGraph(spriteShader.get(), FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 1.0f,SpriteMask::NONE, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, fado_alpha));

#ifdef EXIST_IMGUI
	//ImGui
	if(Get_Use_ImGui())
	{
		ImGui::Text("Load");
		ImGui::Text("time : %f", timer);
		ImGui::Text("load_state : %d", load_state);
	}
#endif
}

bool SceneLoad::FedoOut(float elapsed_time)
{
	fado_alpha += FADE_MIX(elapsed_time);

	if (fado_alpha > 1.0f)
	{
		return true;
	}

	return false;
}