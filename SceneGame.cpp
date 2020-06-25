#include "Scene.h"
#include "framework.h"
#include <stdlib.h>
#include <time.h>
#include "YRMouse.h"
#include "framework.h"

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
}

void SceneGame::LoadData()
{
	//モデル等のロード
	//この関数はSceneLoadで別スレッドとして動かす
	SetPlayerCharacter(player1p, FRAMEWORK.sceneselect.select_p1);
	SetPlayerCharacter(player2p, FRAMEWORK.sceneselect.select_p2);
	player1p->Init(PL.pos1P);
	player1p->Init(PL.pos1P);
	player2p->Init(PL.pos2P);
	PL.HP_MAX1P = player1p->hp;
	PL.HP_MAX2P = player2p->hp;
	PL.ratio1P = player1p->hp / PL.HP_MAX1P * 800.0f;
	PL.ratio2P = player2p->hp / PL.HP_MAX2P * 800.0f;
	PL.correction_value = 800.0f - PL.ratio1P;
	if (FRAMEWORK.sceneselect.select_p1 == scastI(INPUT_PLAYER::P1))
	{
		player1p->pad = std::make_unique<GamePad1>();
		player2p->pad = std::make_unique<GamePad2>();
	}
	else
	{
		player1p->pad = std::make_unique<GamePad1>();
		player2p->pad = std::make_unique<GamePad2>();
	}
	player1p->pad->Init();
	player2p->pad->Init();

	FRAMEWORK.sceneload.load_state = 7;
}

void SceneGame::UnInit()
{
	//プレイヤーのUninit関数を回す
	player1p->Uninit();
	player2p->Uninit();
}

void SceneGame::Update(float elapsed_time)
{
	
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
		pad1 = std::make_unique<GamePad1>();
		pad2 = std::make_unique<GamePad2>();
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