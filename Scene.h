#pragma once

#include "./imgui/imgui.h"
#include "./imgui/imgui_impl_win32.h"
#include "./imgui/imgui_impl_dx11.h"
#include "Key.h"
#include <directxmath.h>
#include <memory>
#include< Windows.h>
#include "YR_VECTOR3.h"
#include "YRGamePad.h"
#include <thread>
#include "PlayerBase.h"
#include "sprite.h"


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

class SceneLoad : public SceneBase
{
public:
	int timer;
	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
};

class SceneSelect : public SceneBase
{
public:
	std::thread*		t = NULL;
	int					load_state;
	bool				load_fin;

	YR_Vector3			p1;
	YR_Vector3			p2;
	YR_Vector3			knight_pos;
	YR_Vector3			kenpos;
	float				Rato;
	GamePad1			g1;
	GamePad2			g2;
	bool				p1Enter;
	bool				p2Enter;
	int					timer;
	bool				end;
	int					fedoTimer;
	int					Fedotimer;

	int					select_p1;
	int					select_p2;

public:
	std::unique_ptr<Sprite>	back_img = nullptr;
	std::unique_ptr<Sprite>	knight_icon = nullptr;
	std::unique_ptr<Sprite>	ken_icon = nullptr;
	std::unique_ptr<Sprite>	select_img = nullptr;
	//std::unique_ptr<Sprite>	back = nullptr;

public:

	void				Init();
	void				Update(float elapsed_time);
	void				Draw(float elapsed_time);

	void				LoadData();

	YR_Vector3			PosSet(int select);
	bool				Fedo();
};