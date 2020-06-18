#include "Scene.h"
#include "framework.h"
//#include "Player.h"

void SceneClear::Init()
{
	
}

void SceneClear::Update(float elapsed_time)
{
	//timer++;
	/*if (pKeyState.zflg == 1)
	{
		pFRAMEWORK.SetScene(SCENE_TABLE::SCENE_TITLE);
	}*/
}

void SceneClear::Draw(float elapsed_time)
{
	
}

void SceneClear::ScoreImageSet()
{
	int s[6];
	int dScore = 0;/* pPlayer.score;*/
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