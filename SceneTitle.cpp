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
	
}

void SceneTitle::Update(float elapsed_time)
{
	//timer++;
	if (pKeyState.zflg == 1)
	{
		FRAMEWORK.SetScene(SCENE_TABLE::SCENE_SELECT);
	}
}

void SceneTitle::Draw(float elapsed_time)
{
#if USE_IMGUI
	//ImGui
	{
		ImGui::Text("time : %d",time);
		ImGui::Text("title");
	}
#endif
}