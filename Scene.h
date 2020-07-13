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
	float	fedo_alpha;
	bool	fedo_start;

	virtual void Init() {};
	virtual void Update(float elapsed_time) {};
	virtual void Draw(float elapsed_time) {};
};

//プレイヤー制御構造体
//{
//	HP_MAX～：プレイヤーのHPの最大値
//	ratio～：HP描画時に使用
//	power～：ゲージの色
//	correction_value：意味は知らん。なんか画像の位置調整に使ってる
//	gauge～：描画時に使用。こちらはゲージ
//	pos～：プレイヤーの初期位置
//}
struct PlayerALL
{
	float				HP_MAX1P = 0.0f;
	float				HP_MAX2P = 0.0f;
	float				ratio1P	= 0.0f;
	float				ratio2P = 0.0f;
	DirectX::XMFLOAT4	power1P = { 0.0f,0.0f,0.0f,0.0f };
	DirectX::XMFLOAT4	power2P = { 0.0f,0.0f,0.0f,0.0f };
	float				correction_value = 0.0f;
	float				gauge1P = 0.0f;
	float				gauge2P = 0.0f;
	YR_Vector3			pos1P{ -5.0f,0.0f };
	YR_Vector3			pos2P{ 5.0f,0.0f };

	
};

class SceneGame : public SceneBase
{
private:
	POINT mouse_pos;
public:
	float	timer;
	int		sco[6];

	enum MAIN_LOOP
	{
		INTRO1P = 0,
		INTRO2P,
		READY,
		MAIN,
		FINISH,
		WIN1P,
		WIN2P,
		GAME_FIN,
	};

	enum JUDGE_VICTORY
	{
		NO_VICTORY = 0,
		VICTORY1P,
		VICTORY2P,
		DROW,
	};

	MAIN_LOOP		main_loop;	//この変数でゲームメインの遷移を管理する

	std::unique_ptr<geometric_primitive>	geo = nullptr;
	std::unique_ptr<Skinned_mesh>	skin = nullptr;

	//画像関係
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
	std::array<int, 3>		p1combo;
	std::array<int, 3>		p2combo;

	//画像
	std::unique_ptr<Skinned_mesh> box = nullptr;
	std::shared_ptr<Texture> box_texture = nullptr;
	MeshMotion motion;

	//シェーダー
	std::unique_ptr<YRShader> skinShader = nullptr;
	std::unique_ptr<YRShader> spriteShader = nullptr;
	std::unique_ptr<YRShader> geoShader = nullptr;

	//プレイヤー管理系
	PlayerALL						PL;
	std::unique_ptr<Player>			player1p	= nullptr;
	std::unique_ptr<Player>			player2p	= nullptr;
	std::unique_ptr<GamepadBase>	pad1		= nullptr;
	std::unique_ptr<GamepadBase>	pad2		= nullptr;

	//ゲームループ制御変数
	bool			pause;			//ポーズ中
	bool			start;			//対戦開始
	JUDGE_VICTORY	judge;			//勝敗の有無
	float			start_timer;	//開始までの時間。Are You Ready?
	bool			end;			//勝敗がついた
	bool			fin;			//全て終わった
	float			endtimer;		//勝敗がついてから勝利画面に移行するまでに使用
	float			mix_fedo;		//フェードインの速度変更用

public:

	void				Init();
	void				Update(float elapsed_time);
	void				Draw(float elapsed_time);
	void				UnInit();
	void				LoadData();
	bool				FedoOut(float elapsed_time);

	void				PadSet(int select1);
	void				PadSet(int select1, int select2);
	void				Winjudge();
	DirectX::XMFLOAT4	ColorSet(int power);
	void				ComboImageSet();

	DirectX::XMFLOAT2	Distance(DirectX::XMFLOAT2& s_pos, DirectX::XMFLOAT2& e_pos);
	void				ScoreImageSet();

public:
	//ゲーム処理関数
	void	SetPlayerCharacter(std::unique_ptr<Player>& player, int select);
	void	PauseUpdate();
	void	TrackSet();
	void	FinUpdate();
	void	StartSet();
	void	FinSet();
};



class SceneTitle : public SceneBase
{
	
public:
	std::unique_ptr<Sprite> test = nullptr;
	std::thread* t = NULL;
	int					load_state;
	bool				load_fin;

	GamePad1			g1;
	GamePad2			g2;

	//シェーダー
	std::unique_ptr<YRShader> spriteShader = nullptr;

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
	std::unique_ptr<YRShader> spriteShader = nullptr;

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
	std::unique_ptr<YRShader> spriteShader = nullptr;

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

class SceneTest : public SceneBase
{
public:
	float	timer;

	std::unique_ptr<Sprite> test = nullptr;
	std::unique_ptr<Skinned_mesh> box = nullptr;
	std::shared_ptr<Texture> box_texture = nullptr;
	MeshMotion motion;

	std::unique_ptr<geometric_primitive>	geo = nullptr;

	//シェーダー
	std::unique_ptr<YRShader> skinShader = nullptr;
	std::unique_ptr<YRShader> spriteShader = nullptr;
	std::unique_ptr<YRShader> geoShader = nullptr;

	//テクスチャ
	std::unique_ptr<Texture> color_texture = nullptr;
	std::unique_ptr<Texture> normal_texture = nullptr;
	std::unique_ptr<Texture> position_texture = nullptr;

public:
	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
};