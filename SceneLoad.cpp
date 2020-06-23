#include "Scene.h"
#include "HitCheak.h"

//-------------------------------------------------------------
// **シーン概要**
//・このシーンではSceneGameのロードを行う
//・SceneSelectから送られてきた情報をもとにデータの読み込みを行い、
//	読み込みがすべて完了したらSceneGameをセットする
//-------------------------------------------------------------
int Hitcheak::timer = 0;
bool Hitcheak::hit = false;
bool Hitcheak::stop1p = false;
bool Hitcheak::stop2p = false;
float Hitcheak::add1P = 0.0f;
float Hitcheak::add2P = 0.0f;
bool Hitcheak::clash = false;
YR_Vector3 Hitcheak::clashpos = { 0.0f,0.0f };
YR_Vector3 Hitcheak::effectpos = { 0.0f,0.0f };
int Hitcheak::effecttimer = 0;

void SceneLoad::Init()
{

}

void SceneLoad::Update(float elapsedTime)
{

}

void SceneLoad::Draw(float elapsedTime)
{

}