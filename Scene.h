#pragma once

#include "./imgui/imgui.h"
#include "./imgui/imgui_impl_win32.h"
#include "./imgui/imgui_impl_dx11.h"
#include "Key.h"
#include <directxmath.h>
//#include <memory>
#include< Windows.h>
#include "YR_VECTOR3.h"
#include "YRGamePad.h"
#include <thread>
#include "PlayerBase.h"
#include "sprite.h"
#include "YRShader.h"
#include "Geometric_primitive.h"

enum class INPUT_PLAYER : int
{
	P1 = 0,
	NONE,
	P2
};

class SceneBase
{
public:

	//fedo_alphaの値を0.0f~1.0fに動かしてフェードさせる
	//fedo_startはフェードアウトするかどうかのフラグ。falseの場合はfedo_alhaを下げ続ける
	float fedo_alpha;
	bool fedo_start;

	virtual void Init() {};
	virtual void Update(float elapsed_time) {};
	virtual void Draw(float elapsed_time) {};
};

//プレイヤー制御構造体
struct PlayerALL
{
	float HP_MAX1P = 0.0f;
	float HP_MAX2P = 0.0f;
	float ratio1P = 0.0f;
	float ratio2P = 0.0f;
	int power1P = 0;
	int power2P = 0;
	float correction_value = 0.0f;
	float gauge1P = 0.0f;
	float gauge2P = 0.0f;
	YR_Vector3 pos1P{ -1.0f,0.0f };
	YR_Vector3 pos2P{ 2.0f,0.0f };

	
};

class SceneGame : public SceneBase
{
private:
	POINT mouse_pos;
public:
	float timer;
	int sco[6];

	std::unique_ptr<geometric_primitive> geo = nullptr;
	//画像
	std::unique_ptr<Sprite> test = nullptr;
	std::unique_ptr<Sprite> HP_img = nullptr;
	std::unique_ptr<Sprite> win1P_img = nullptr;
	std::unique_ptr<Sprite> win2P_img = nullptr;
	std::unique_ptr<Sprite> draw_img = nullptr;
	std::unique_ptr<Sprite> HPbar_img = nullptr;
	std::unique_ptr<Sprite> KO_img = nullptr;
	std::unique_ptr<Sprite> gauge_img = nullptr;
	std::unique_ptr<Sprite> gaugecase_img = nullptr;
	std::unique_ptr<Sprite> font_img = nullptr;
	std::unique_ptr<Sprite> call_img = nullptr;
	std::unique_ptr<Sprite> effect_img = nullptr;
	std::shared_ptr<Sprite> p1combo_img = nullptr;
	std::shared_ptr<Sprite> p2combo_img = nullptr;


	//シェーダー
	std::unique_ptr<YRShader> skinShader;
	std::unique_ptr<YRShader> spriteShader;
	std::unique_ptr<YRShader> geoShader;

	PlayerALL PL;
	std::unique_ptr<Player> player1p;
	std::unique_ptr<Player> player2p;
	std::unique_ptr<GamepadBase> pad1;
	std::unique_ptr<GamepadBase> pad2;

	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
	void UnInit();
	void LoadData();
	bool FedoOut(float elapsed_time);

	void PadSet(int select1);
	void PadSet(int select1, int select2);
	int Winjudge();
	int ColorSet(int power);
	void ComboImageSet();

	DirectX::XMFLOAT2 Distance(DirectX::XMFLOAT2& s_pos, DirectX::XMFLOAT2& e_pos);
	void ScoreImageSet();

public:
	//ゲーム処理関数
	void SetPlayerCharacter(std::unique_ptr<Player>& player, int select);
};



class SceneTitle : public SceneBase
{
	
public:
	std::unique_ptr<Sprite> test;
	std::thread* t = NULL;
	int					load_state;
	bool				load_fin;

	GamePad1			g1;
	GamePad2			g2;

	//シェーダー
	std::unique_ptr<YRShader> spriteShader;

	//画像描画関係
	YR_Vector3			p1;
	YR_Vector3			p2;
	YR_Vector3			none_pos;
	YR_Vector3			p1_pos;
	YR_Vector3			p2_pos;
	float				Rato;
	bool				p1Enter;
	bool				p2Enter;
	float				timer;
	bool				end;

	int					select_p1;
	int					select_p2;

public:
	std::unique_ptr<Sprite>	knight_icon = nullptr;
	std::unique_ptr<Sprite>	ken_icon = nullptr;
	std::unique_ptr<Sprite>	select_img = nullptr;

	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
	void LoadData();
	void UnInit();

	bool FedoOut(float elapsed_time);
	YR_Vector3			PosSet(int select);
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
	//ロード関係
	std::thread* t = NULL;
	int					load_state;
	bool				load_fin;
	bool				Game_load_fin;

	//ロード時の画像
	std::unique_ptr<Sprite>	load_img = nullptr;

	//シェーダー
	std::unique_ptr<YRShader> spriteShader;

	float timer;
	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
	void UnInit();
	void LoadData();
	bool FedoOut(float elapsed_time);

};

class SceneSelect : public SceneBase
{
public:
	//ロード関係
	std::thread* t = NULL;
	int					load_state;
	bool				load_fin;

	//シェーダー
	std::unique_ptr<YRShader> spriteShader;

	//画像描画関係
	YR_Vector3			p1;
	YR_Vector3			p2;
	YR_Vector3			knight_pos;
	YR_Vector3			kenpos;
	float				Rato;
	bool				p1Enter;
	bool				p2Enter;
	float				timer;
	bool				end;
	
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
	void				UnInit();
	void				LoadData();
	bool				FedoOut(float elapsed_time);

	YR_Vector3			PosSet(int select);
};