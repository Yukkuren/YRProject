#pragma once

#include "./imgui/imgui.h"
#include "./imgui/imgui_impl_win32.h"
#include "./imgui/imgui_impl_dx11.h"
#include "Key.h"
#include <directxmath.h>
//#include <memory>
#include< Windows.h>
#include "YR_VectorMaster.h"
#include "YRGamePad.h"
#include <thread>
#include "PlayerBase.h"
#include "sprite.h"
#include "YRShader.h"
#include "YRModelAnim.h"
#include "Geometric_primitive.h"
#include "Board_primitive.h"
#include "Sampler.h"
#include "AnimBoard.h"
#include "collision.h"


constexpr float start_time = 2.0f;			//ゲームが開始するまでの時間
constexpr float ready_time = 1.0f;			//Readyの表示を出す時間
constexpr float end_slow_time = 3.0f;		//試合終了後スローにする時間
constexpr float slow_adjust = 0.2f;			//スロー補正値
constexpr float game_end_time = 5.0f;		//ゲームが終了して勝利画面に移行する時間

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
	float	fado_alpha;
	bool	fado_start;

	virtual void Init() {};
	virtual void Update(float elapsed_time) {};
	virtual void Draw(float elapsed_time) {};
	virtual void UnInit() {};
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
	YR_Vector3			pos1P{ -8.0f,0.0f };
	YR_Vector3			pos2P{ 8.0f,0.0f };

	
};

//--------------------------------------------------
//			ステージクラス
//--------------------------------------------------
//	ステージ用のモデル、シェーダーをもつクラス
//	ステージの描画などもこのクラスで行う
//--------------------------------------------------
class Stage
{
public:

	enum class StageType : int
	{
		NORMAL = 0,
	};

public:
	//FBXデータ
	//std::shared_ptr<Model> sky = nullptr;
	std::unique_ptr<Skinned_mesh> sky_data = nullptr;
	std::shared_ptr<Model> stage_data = nullptr;
	//std::unique_ptr<ModelAnim> sky_draw = nullptr;
	std::unique_ptr<ModelAnim> stage_draw = nullptr;
	
	//シェーダー
	std::unique_ptr<YRShader> skyShader = nullptr;
	std::unique_ptr<YRShader> skinShader = nullptr;
	std::unique_ptr<YRShader> ToonShader = nullptr;

	YR_Vector3	Sky_Pos = YR_Vector3(0.0f, 0.0f, 0.0f);
	YR_Vector3	Sky_Scale = YR_Vector3(1.0f, 1.0f, 1.0f);
	YR_Vector3	Sky_Angle = YR_Vector3(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f);

	YR_Vector3	Stage_Pos = YR_Vector3(0.0f, 0.0f, 0.0f);
	YR_Vector3	Stage_Scale = YR_Vector3(1.0f, 1.0f, 1.0f);
	YR_Vector3	Stage_Angle = YR_Vector3(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f);

	void Init(StageType type);
	void Uninit();
	void Draw(const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time);
};



class AI_Controller
{
public:
	const float max_dis = 16.0f;		//近づくかどうかの距離目安
	const int attack_probability = 4;	//攻撃の確率
	const int track_probability = 3;	//ホーミングダッシュの確率
	const int guard_probability = 7;	//ガードの確率
	const float timer_max = 2.0f;		//タイマーの最大値
	const float guard_timer_max = 1.0f;	//ガードタイマーの最大値

	float timer = 0.0f;

	enum class AI_State : int
	{
		INIT = 0,		//最初の設定
		RAND_SELECT,	//ランダムで行動を決定する
		APPROACH,		//近づく
		LEAVE,			//離れる
		COMBO,			//Xボタンを連打する
		TRACK_DASH,		//ホーミングダッシュを行う
		GUARD,			//ガードしようとする
		KNOCK,			//のけぞった
		DEAPTH,			//体力がなくなった
		DOWN,			//ダウン中
		END,
	};

	AI_State state = AI_State::INIT;
public:
	void Init()
	{
		state = AI_State::INIT;
		timer = 0.0f;
	}
};




class SceneGame : public SceneBase
{
private:
	POINT mouse_pos;
	bool camera_move_debug;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	m_depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	m_depth_stencil_state2;
public:

	//2Pの行動制御列挙
	enum class Player2PControl : int
	{
		OPERATION,	//操作
		SUSPENSION,	//動かない
		AI,			//AI
		END,		//終了コマンド
	}; Player2PControl pl2_con = Player2PControl::OPERATION;

	float	timer = 0.0f;
	int		sco[6] = { 0,0,0,0,0,0 };

	float p1_elapsed_time = 0.0f;
	float p2_elapsed_time = 0.0f;

	float hit_stop_elapsed;				//ヒットストップ時に使用

	float game_speed;					//ゲーム全体のスピードを変更する変数

	//カメラステートがメインの時のカメラ座標
	YR_Vector3	Scene_eye;
	YR_Vector3	Scene_focus;
	YR_Vector3	Scene_up;
	float		Scene_fov = 0.0f;		//画角
	float		Scene_aspect = 0.0f;	//アスペクト比
	float		Scene_nearZ = 0.0f;		//ニアクリップ面までの距離
	float		Scene_farZ = 0.0f;		//ファークリップ面までの距離

	YR_Vector3	Start_Scene_eye;		//ゲーム開始時の初期カメラ座標
	YR_Vector3	Start_Scene_focus;		//ゲーム開始時の初期カメラ方向

	enum class MAIN_LOOP :int
	{
		INTRO1P = 0,
		INTRO2P,
		READY,
		MAIN,
		FINISH,
		WIN1P,
		WIN2P,
		DRAW,
		GAME_FIN,
	};

	enum class JUDGE_VICTORY : int
	{
		NO_VICTORY = 0,
		VICTORY1P,
		VICTORY2P,
		DRAW,
	};

	MAIN_LOOP		main_loop;	//この変数でゲームメインの遷移を管理する

	std::unique_ptr<geometric_primitive>	geo = nullptr;		//板ポリデータ。当たり判定の描画に使う
	//std::unique_ptr<Skinned_mesh>	skin = nullptr;

	Stage					stage;				//ステージの実体。モデルのデータ、シェーダーを持つ

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
	std::unique_ptr<Sprite> pause_img = nullptr;
	std::array<int, 3>		p1combo;
	std::array<int, 3>		p2combo;


	//画面描画用テクスチャ
	std::unique_ptr<Texture> color_texture = nullptr;
	//std::unique_ptr<Texture> normal_texture = nullptr;
	//std::unique_ptr<Texture> position_texture = nullptr;
	std::unique_ptr<Texture> luminance_texture = nullptr;
	std::unique_ptr<Texture> multi_blur_texture = nullptr;
	std::array<std::unique_ptr<Texture>, 10> blur_texture = { nullptr };

	//Gbuffer用スプライト
	std::unique_ptr<Sprite>	sprite = nullptr;

	//サンプラー
	std::shared_ptr<Sampler> sampler_clamp = nullptr;

	//シェーダー
	std::unique_ptr<YRShader> skinShader = nullptr;
	std::unique_ptr<YRShader> spriteShader = nullptr;
	std::unique_ptr<YRShader> geoShader = nullptr;
	std::unique_ptr<YRShader> ParallelToonShader = nullptr;
	std::unique_ptr<YRShader> ToonShader = nullptr;
	std::unique_ptr<YRShader> gaussShader = nullptr;
	std::unique_ptr<YRShader> multi_gaussShader = nullptr;
	std::unique_ptr<YRShader> spriteEx = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	constantBuffer = nullptr;

	//プレイヤー管理系
	PlayerALL						PL;
	std::unique_ptr<Player>			player1p = nullptr;
	std::unique_ptr<Player>			player2p = nullptr;
	std::unique_ptr<GamepadBase>	pad1 = nullptr;
	std::unique_ptr<GamepadBase>	pad2 = nullptr;

	//ゲームループ制御変数
	bool			pause = false;						//ポーズ中
	bool			start = false;						//対戦開始
	JUDGE_VICTORY	judge = JUDGE_VICTORY::NO_VICTORY;	//勝敗の有無
	float			start_timer = 0.0f;					//開始までの時間。Are You Ready?
	bool			end = false;						//勝敗がついた
	bool			fin = false;						//全て終わった
	float			endtimer = 0.0f;					//勝敗がついてから勝利画面に移行するまでに使用
	float			mix_fade = 0.0f;					//フェードインの速度変更用
	bool			blur_on = false;					//ブルームをかけるフラグ

	AI_Controller	AI2P;								//2PのAI情報

public:

	void				Init();
	void				Update(float elapsed_time);
	void				Draw(float elapsed_time);
	void				UnInit();
	void				LoadData();
	bool				FadeOut(float elapsed_time);

	void				PadSet(int select1);
	void				PadSet(int select1, int select2);
	void				Winjudge();
	DirectX::XMFLOAT4	ColorSet(int power);
	void				ComboImageSet();

	DirectX::XMFLOAT2	Distance(DirectX::XMFLOAT2& s_pos, DirectX::XMFLOAT2& e_pos);
	void				ScoreImageSet();

	void				Control2PState(float elapsed_time);

public:
	//ゲーム処理関数
	void	SetPlayerCharacter(std::unique_ptr<Player>* player, int select);	//selectによってプレイヤーを生成する
	void	PauseUpdate();		//ポーズ中行う処理
	void	TrackSet();			//ホーミングダッシュ用の値を変更する
	void	FinUpdate();		//ゲーム終了後の処理
	void	StartSet();			//イントロ終了後のゲーム画面のセット(カメラ)
	void	FinSet();			//ゲーム終了後のゲーム画面のセット(カメラ)
	void	CameraUpdate(float elapsed_time);		//カメラのステートがMAINにある場合のカメラ処理を行う
	void	CameraRequest(float elapsed_time);	//カメラのリクエストを確認し、リクエストがあれば処理を行う

	void SetRenderTexture();

	void NullSetRenderTexture();


	void RenderTexture();

	void RenderBlur();

	void AIControll(float elapsed_time);

	/*struct CB_Multi_Render_Target
	{
		DirectX::XMFLOAT4	light_direction = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	light_color = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	ambient_color = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	eye_pos = { 0.0f,0.0f,0.0f,0.0f };
	};*/
};


class SceneTitle : public SceneBase
{
	
public:
	std::unique_ptr<Sprite> test = nullptr;
	std::thread* t = NULL;
	int					load_state = 0;
	bool				load_fin = false;

	GamePad1			g1;
	GamePad2			g2;

	//シェーダー
	std::unique_ptr<YRShader> spriteShader = nullptr;
	std::unique_ptr<YRShader> titleShader = nullptr;
	std::unique_ptr<YRShader> spriteEx = nullptr;

	//画像描画関係
	YR_Vector3			p1;
	YR_Vector3			p2;
	YR_Vector3			none_pos;
	YR_Vector3			p1_pos;
	YR_Vector3			p2_pos;
	float				Rato = 0.0f;
	bool				p1Enter = false;
	bool				p2Enter = false;
	float				timer = 0.0f;
	bool				end = false;

	int					select_p1 = 0;
	int					select_p2 = 0;

public:
	std::unique_ptr<Sprite>	knight_icon = nullptr;
	std::unique_ptr<Sprite>	ken_icon = nullptr;
	std::unique_ptr<Sprite>	select_img = nullptr;
	std::unique_ptr<Sprite>	title_img = nullptr;
	std::unique_ptr<Sprite> choice_img = nullptr;

	//画面描画用テクスチャ
	std::unique_ptr<Texture> color_texture = nullptr;
	//std::unique_ptr<Texture> normal_texture = nullptr;
	//std::unique_ptr<Texture> position_texture = nullptr;
	std::unique_ptr<Texture> luminance_texture = nullptr;

	std::unique_ptr<Texture> title_texture = nullptr;

	//Gbuffer用スプライト
	std::unique_ptr<Sprite>	sprite = nullptr;

	//サンプラー
	std::shared_ptr<Sampler> sampler_clamp = nullptr;

	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	constantBuffer = nullptr;

	Title_CBuffer cbuffer_param;

	void LoadData();

	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
	void UnInit();

	bool FedoOut(float elapsed_time);
	YR_Vector3			PosSet(int select);


	void SetRenderTexture();

	void NullSetRenderTexture();

	void RenderTexture(float elapsed_time);

	void SceneRender(float elapsed_time);

	enum class VS_MODE : int
	{
		PLAYER = 0,
		CPU,
	};

	VS_MODE vs_mode = VS_MODE::CPU;

	enum class STATE : int
	{
		HOME,
		SELECT,
		END,
	};
	STATE state = STATE::HOME;
};

class SceneClear : public SceneBase
{
public:
	int sco[6] = { 0,0,0,0,0,0 };
	float timer = 0.0f;
	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
	void ScoreImageSet();
	void UnInit() {};
};

class SceneOver : public SceneBase
{
public:
	float timer = 0.0f;
	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
	void UnInit() {};
};

class SceneLoad : public SceneBase
{
public:
	//ロード関係
	std::thread* t = NULL;
	int					load_state = 0;
	bool				load_fin = false;
	bool				Game_load_fin = false;

	//ロード時の画像
	std::unique_ptr<Sprite>	load_img = nullptr;
	std::unique_ptr<Sprite>	load_bg = nullptr;

	//シェーダー
	std::unique_ptr<YRShader> spriteShader = nullptr;

	float timer = 0.0f;
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
	int					load_state = 0;
	bool				load_fin = false;

	//シェーダー
	std::unique_ptr<YRShader> spriteShader = nullptr;

	//画像描画関係
	YR_Vector3			p1;
	YR_Vector3			p2;
	YR_Vector3			knight_pos;
	YR_Vector3			kenpos;
	float				Rato = 0.0f;
	bool				p1Enter = false;
	bool				p2Enter = false;
	float				timer = 0.0f;
	bool				end = false;

	int					select_p1 = 0;
	int					select_p2 = 0;

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
	bool				FadoOut(float elapsed_time);

	YR_Vector3			PosSet(int select);
};

class SceneTest : public SceneBase
{
public:
	float	timer = 0.0f;

	std::unique_ptr<Sprite> test = nullptr;
	std::shared_ptr<Model> knight = nullptr;
	std::shared_ptr<Model> wait_R = nullptr;
	std::shared_ptr<Texture> box_texture = nullptr;
	std::shared_ptr<Texture> board_texture = nullptr;
	std::unique_ptr<ModelAnim> motion = nullptr;
	std::unique_ptr<Skinned_mesh> sky = nullptr;

	std::unique_ptr<geometric_primitive>	geo = nullptr;
	std::unique_ptr<board_primitive>		board = nullptr;
	std::unique_ptr<AnimBoard>				anim = nullptr;


	std::unique_ptr<board_primitive>		bisuko = nullptr;
	std::unique_ptr<board_primitive>		cube = nullptr;
	std::unique_ptr<board_primitive>		plane = nullptr;

	std::shared_ptr<Texture> bisuko_normal_texture = nullptr;
	std::shared_ptr<Texture> bisuko_specular_texture = nullptr;
	std::shared_ptr<Texture> test_texture = nullptr;
	std::shared_ptr<Texture> test_normal_texture = nullptr;
	std::shared_ptr<Texture> scorpion_specular_texture = nullptr;
	std::shared_ptr<Texture> specular_texture = nullptr;


	//Gbuffer用スプライト
	std::unique_ptr<Sprite>	sprite = nullptr;

	//サンプラー
	std::shared_ptr<Sampler> sampler_wrap = nullptr;
	std::shared_ptr<Sampler> sampler_clamp = nullptr;


	//シェーダー
	std::unique_ptr<YRShader> skinShader = nullptr;
	std::unique_ptr<YRShader> spriteShader = nullptr;
	std::unique_ptr<YRShader> geoShader = nullptr;
	std::unique_ptr<YRShader> boardShader = nullptr;
	std::unique_ptr<YRShader> animShader = nullptr;
	std::unique_ptr<YRShader> toonShader = nullptr;
	std::unique_ptr<YRShader> paralleltoonShader = nullptr;
	std::unique_ptr<YRShader> toGbuffer = nullptr;
	std::unique_ptr<YRShader> spriteEx = nullptr;
	std::unique_ptr<YRShader> flatShader = nullptr;
	std::unique_ptr<YRShader> gaussShader = nullptr;
	std::unique_ptr<YRShader> multi_gaussShader = nullptr;
	std::unique_ptr<YRShader> furShader = nullptr;
	std::unique_ptr<YRShader> skyShader = nullptr;

	//テクスチャ
	std::unique_ptr<Texture> color_texture = nullptr;
	std::unique_ptr<Texture> normal_texture = nullptr;
	std::unique_ptr<Texture> position_texture = nullptr;
	std::unique_ptr<Texture> luminance_texture = nullptr;
	std::unique_ptr<Texture> multi_blur_texture = nullptr;
	std::array<std::unique_ptr<Texture>,10> blur_texture = { nullptr };
	std::unique_ptr<Texture> fur = nullptr;

	Collision circle;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	constantBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	constantBuffer_Gauss = nullptr;

	DirectX::XMFLOAT3 knight_angle = { DirectX::XMConvertToRadians(-90.0f),0.0f,0.0f };
	DirectX::XMFLOAT3 knight_pos = { 0.0f,0.0f,20.0f };

public:
	void Init();
	void Update(float elapsed_time);
	void Draw(float elapsed_time);
	void UnInit() {
		knight.reset();
	};

	void RenderTexture(
		const DirectX::XMMATRIX&	view,
		const DirectX::XMMATRIX&	projection,
		const DirectX::XMFLOAT4&	light_direction,
		const DirectX::XMFLOAT4&	light_color,
		const DirectX::XMFLOAT4&	ambient_color,
		float						elapsed_time);

	void RenderBlur(
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time);

public:
	struct CB_Multi_Render_Target
	{
		DirectX::XMFLOAT4	light_direction = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	light_color = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	ambient_color = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	eye_pos = { 0.0f,0.0f,0.0f,0.0f };
		float Density = 0.0f;		//毛の密度
		float Distance = 0.0f;		//毛の長さ
		float dummy1 = 0.0f;
		float dummy2 = 0.0f;
	};
};