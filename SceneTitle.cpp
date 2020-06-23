#include "Scene.h"
#include "framework.h"

//-------------------------------------------------------------
// **�V�[���T�v**
//�E�{�^���������ƃR���g���[���[�ݒ肪�o��悤�ɂ���
//�@1P,2P�̓��͂����肷��
//�E�ݒ肵����Ó]�BSceneSelect�֑J�ڂ���
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