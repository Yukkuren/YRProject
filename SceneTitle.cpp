#include "Scene.h"
#include "framework.h"

//-------------------------------------------------------------
// **シーン概要**
//・ボタンを押すとコントローラー設定が出るようにして
//　1P,2Pの入力を決定する
//・設定したら暗転。SceneSelectへ遷移する
//-------------------------------------------------------------

void SceneTitle::Init()
{
	timer = 0;
	load_fin = false;
	load_state = 0;
	fedo_alpha = 1.0f;
	fedo_start = false;
}

void SceneTitle::LoadData()
{
	if (test == nullptr)
	{
		test = std::make_unique<Sprite>(L"./Data/Image/BG/mementos.png", 1920.0f, 1080.0f);
	}

	load_state = 3;
}

void SceneTitle::UnInit()
{
	test.reset();
	test = nullptr;
}

void SceneTitle::Update(float elapsed_time)
{
	if (load_fin)
	{
		//timer++;
		if (pKeyState.zflg == 1)
		{
			fedo_start = true;
		}
		if (fedo_start)
		{
			if (FedoOut(elapsed_time))
			{
				FRAMEWORK.SetScene(SCENE_TABLE::SCENE_SELECT);
				UnInit();
				return;
			}
		}
		else
		{
			if (fedo_alpha > 0.0f)
			{
				fedo_alpha -= FEDO_MIX(elapsed_time);
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
			//ロード終了後に何か処理を入れるときのためにあけている
			load_state = 4;
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
	if (load_fin)
	{
		test->DrawGraph(FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f);
	}

	FRAMEWORK.fedo_img->DrawRotaGraph(FRAMEWORK.SCREEN_WIDTH / 2.0f, FRAMEWORK.SCREEN_HEIGHT / 2.0f, 0.0f, 1.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, fedo_alpha));

#if USE_IMGUI
	//ImGui
	{
		ImGui::Text("title");
		ImGui::Text("time : %f",timer);
		ImGui::Text("load_state : %d", load_state);
	}
#endif
}

bool SceneTitle::FedoOut(float elapsed_time)
{
	fedo_alpha += FEDO_MIX(elapsed_time);

	if (fedo_alpha > 1.0f)
	{
		return true;
	}

	return false;
}