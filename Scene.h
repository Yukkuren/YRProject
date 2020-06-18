#pragma once

#include"./imgui/imgui.h"
#include"./imgui/imgui_impl_win32.h"
#include"./imgui/imgui_impl_dx11.h"
#include"Key.h"
#include<directxmath.h>
#include<memory>
#include<Windows.h>

class SceneBase
{
public:
	virtual void Init() {};
	virtual void Update(float elapsed_time) {};
	virtual void Draw(float elapsed_time) {};
};

class SceneGame : public SceneBase
{
private:
	POINT mouse_pos;
public:

	int sco[6];
	//std::unique_ptr<Collision> test;
	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);

	DirectX::XMFLOAT2 Distance(DirectX::XMFLOAT2& s_pos, DirectX::XMFLOAT2& e_pos);
	void ScoreImageSet();
};

class SceneTitle : public SceneBase
{
public:
	int timer;
	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
};

class SceneClear : public SceneBase
{
public:
	int sco[6];
	int timer;
	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
	void ScoreImageSet();
};

class SceneOver : public SceneBase
{
public:
	int timer;
	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
};