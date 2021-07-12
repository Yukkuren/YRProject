#pragma once

#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../../imgui/imgui_impl_dx11.h"
#include "../Key.h"
#include <directxmath.h>
//#include <memory>
#include< Windows.h>
#include "../YR_VectorMaster.h"
#include "../Game/YRGamePad.h"
#include <thread>
#include "../Chara/PlayerBase/PlayerBase.h"
#include "../sprite.h"
#include "../YRShader.h"
#include "../Model/YRModelAnim.h"
#include "../Model/Geometric_primitive.h"
#include "../Model/Board_primitive.h"
#include "../Sampler.h"
#include "../Model/AnimBoard.h"
#include "../Game/collision.h"
#include "../Model/Skinned_mesh.h"


//ほとんどのシーンで使用するためヘッダーに直接書いている
constexpr float start_time = 2.0f;			//ゲームが開始するまでの時間
constexpr float ready_time = 1.0f;			//Readyの表示を出す時間
constexpr float end_slow_time = 3.0f;		//試合終了後スローにする時間
constexpr float slow_adjust = 0.2f;			//スロー補正値
constexpr float game_end_time = 5.0f;		//ゲームが終了して勝利画面に移行する時間
constexpr float HP_Danger_point = 0.3f;		//DANGER状態になるHPの割合量

//タイトル画面・ゲームシーンで選択に使用できるボタンの羅列
constexpr std::array<PAD, 10> any_button =
{
	PAD::A,
	PAD::B,
	PAD::X,
	PAD::Y,
	PAD::RB,
	PAD::LB,
	PAD::R_TRIGGER,
	PAD::L_TRIGGER,
	PAD::START,
	PAD::SELECT,
};

//コントローラーのプレイヤー保持を記録する構造体列挙
enum class INPUT_PLAYER : int
{
	NONE = 0,	//まだ何も持っていない状態
	P1, 		//プレイヤー1
	P2			//プレイヤー2
};


//全てのシーンのベース
//※新しくシーンを作成する場合はこのクラスを継承すること
class SceneBase
{
public:

	//fedo_alphaの値を0.0f~1.0fに動かしてフェードさせる
	//fedo_startはフェードアウトするかどうかのフラグ。falseの場合はfedo_alhaを下げ続ける
	float	fado_alpha;
	bool	fado_start;

	virtual void Init() {};						//初期化。読み込むのはシェーダーのみで画像などの読み込みは別スレッドを立ち上げて行う
	virtual void Update(float elapsed_time) {};	//更新処理
	virtual void Draw(float elapsed_time) {};	//描画
	virtual void UnInit() {};					//解放処理
};

//プレイヤー制御構造体
//{
//	HP_MAX～：プレイヤーのHPの最大値
//	ratio～：HP描画時に使用
//	power～：ゲージの色
//	correction_value：画像の位置調整に使ってる
//	gauge～：描画時に使用。こちらはゲージ
//	pos～：プレイヤーの初期位置
//}
struct PlayerALL
{
	float				HP_MAX1P = 0.0f;
	float				HP_MAX2P = 0.0f;
	float				ratio1P	= 0.0f;
	float				ratio2P = 0.0f;
	float				Damage_ratio1P = 0.0f;
	float				Damage_ratio2P = 0.0f;
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

	//ステージはこの構造体列挙で判別する
	enum class StageType : int
	{
		NORMAL = 0,		//通常ステージ。お城
	};

public:
	//FBXデータ
	std::unique_ptr<Skinned_mesh> sky_data = nullptr;
	std::shared_ptr<Model> stage_data = nullptr;
	std::unique_ptr<ModelAnim> stage_draw = nullptr;

	//シェーダー
	std::unique_ptr<YRShader> skyShader = nullptr;
	std::unique_ptr<YRShader> skinShader = nullptr;
	std::unique_ptr<YRShader> ToonShader = nullptr;

	//空、ステージの位置調整に使用
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


//AIコントロールクラス
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

	//AIの行動リスト
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
		SPECIAL,		//超必を打とうとする
		FOREVER_GUARD,	//一生ガードする
		END,
	};

	AI_State state = AI_State::INIT;			//現在のステート
	AI_State before_state = AI_State::INIT;		//前回のステート
	bool steal_escape = false;					//掴み抜けをされたかどうかの判定フラグ
public:
	//AIクラスは外部から全てを動かすため、ここで全て宣言している
	void Init()
	{
		state = AI_State::INIT;
		timer = 0.0f;
	}
};


//コントローラー入力内容保存・表示クラス
class InputGamePadDraw
{
public:

	struct Input_list_pad
	{
		std::vector<int> button_input;	//ボタンの種類
		int stick_input = -1;			//スティックの種類
		bool pushed = false;			//押されていた入力を取った場合true

		Input_list_pad() {
			stick_input = -1;
		};
		~Input_list_pad() {};
	};

	std::list<Input_list_pad>		p1_input_list;
	std::list<Input_list_pad>		p2_input_list;

	//画像スプライト
	std::unique_ptr<Sprite> button_img = nullptr;
	std::unique_ptr<Sprite> stick_img = nullptr;

	//ボタンのリスト
	//・入力されたボタンを画像の並び順に登録している
	const std::array<int, 8>	button_img_list =
	{
		static_cast<int>(PAD::X),
		static_cast<int>(PAD::Y),
		static_cast<int>(PAD::A),
		static_cast<int>(PAD::B),
		static_cast<int>(PAD::RB),
		static_cast<int>(PAD::LB),
		static_cast<int>(PAD::R_TRIGGER),
		static_cast<int>(PAD::L_TRIGGER)
	};

	//スティックの入力リスト
	//・入力されたスティックの向きを画像の並び順に登録している
	//最後の236、214はコマンドの名称(スティックにはない為追加している)
	const std::array<int, 10>	stick_img_list =
	{
		static_cast<int>(PAD::STICK_U),
		static_cast<int>(PAD::STICK_RUp),
		static_cast<int>(PAD::STICK_R),
		static_cast<int>(PAD::STICK_RDown),
		static_cast<int>(PAD::STICK_D),
		static_cast<int>(PAD::STICK_LDown),
		static_cast<int>(PAD::STICK_L),
		static_cast<int>(PAD::STICK_LUp),
		236,
		214
	};

	void Init();
	void Load();
	void Update(Player* player1p, Player* player2p);	//プレイヤーの情報が必要になるため引数でポインタを受け取っている
	void Draw(YRShader* shader);

	int GetButtonNum(int button);						//ボタンの履歴を記録していく
	int GetStickNum(int stick);							//スティックの履歴を記録していく

	void UnInit();
};


//ゲームシーン
class SceneGame : public SceneBase
{
private:
	POINT mouse_pos;						//マウスの座標を保存する変数
	bool camera_move_debug = false;			//カメラをデバッグ機能として動かす場合はtrue
	const float Icon_distance = 20.0f;		//アイコンを表示する高さ

	//深度ステート
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	m_depth_stencil_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	m_depth_stencil_state2;
public:

	YR_Vector2	sprite_debug_pos = { 0.0f,0.0f };
	float	sprite_debug_scale = 0.0f;

	//2Pの行動制御列挙
	enum class Player2PControl : int
	{
		OPERATION,	//操作
		SUSPENSION,	//動かない
		AI,			//AI
		INVINCIVLE,	//常に無敵で動かない
		END,		//終了コマンド
	}; Player2PControl pl2_con = Player2PControl::OPERATION;


	InputGamePadDraw	input_pad;		//入力表示クラス

	float	timer = 0.0f;				//ゲーム内タイマー

	float p1_elapsed_time = 0.0f;
	float p2_elapsed_time = 0.0f;

	float hit_stop_elapsed;				//ヒットストップ時に使用

	float game_speed;					//ゲーム全体のスピードを変更する変数

	//カメラステートがメインの時のカメラ座標
	YR_Vector3	Scene_eye;
	YR_Vector3	Scene_focus;
	YR_Vector3	Scene_up;

	//決着時にカメラの座標を保存する変数
	YR_Vector3	Scene_End_eye;

	float		Scene_fov = 0.0f;		//画角
	float		Scene_aspect = 0.0f;	//アスペクト比
	float		Scene_nearZ = 0.0f;		//ニアクリップ面までの距離
	float		Scene_farZ = 0.0f;		//ファークリップ面までの距離

	float image_alpha = 0.0f;			//画像表示時に使用するアルファ値
	float image_size = 5.0f;			//画像表示時に使用する拡大率

	YR_Vector3	Start_Scene_eye;		//ゲーム開始時の初期カメラ座標
	YR_Vector3	Start_Scene_focus;		//ゲーム開始時の初期カメラ方向

	Title_CBuffer cbuffer_param;

	YR_Vector3	old_pos_p1 = { 0.0f,0.0f,0.0f };	//1フレーム前のプレイヤー1の座標
	YR_Vector3	old_pos_p2 = { 0.0f,0.0f,0.0f };	//1フレーム前のプレイヤー2の座標

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

	//決着時カメラ回転用ステート
	enum class FIN_CAMERA_STATE : int
	{
		ROLL = 0,
		STOP,
	};
	FIN_CAMERA_STATE fin_camera_state = FIN_CAMERA_STATE::ROLL;

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
	std::unique_ptr<Sprite> HPDamagebar_img = nullptr;
	std::unique_ptr<Sprite> KO_img = nullptr;
	std::unique_ptr<Sprite> font_img = nullptr;
	std::unique_ptr<Sprite> combo_img = nullptr;
	std::unique_ptr<Sprite> desastal_case = nullptr;
	std::unique_ptr<Sprite> desastal_flash = nullptr;
	std::unique_ptr<Sprite> desastal_img = nullptr;
	std::unique_ptr<Sprite> call_img = nullptr;
	std::unique_ptr<Sprite> effect_img = nullptr;
	std::unique_ptr<Sprite> pause_img = nullptr;
	std::unique_ptr<Sprite> Danger_img = nullptr;
	std::array<int, 3>		p1combo;
	std::array<int, 3>		p2combo;
	std::shared_ptr<Texture> p1_icon_img = nullptr;
	std::shared_ptr<Texture> p2_icon_img = nullptr;
	std::shared_ptr<Texture> arrow_icon_img = nullptr;

	//板ポリ
	std::unique_ptr<AnimBoard>	p1_icon_board = nullptr;
	std::unique_ptr<AnimBoard>	p2_icon_board = nullptr;
	std::unique_ptr<AnimBoard>	arrow_icon_board = nullptr;

	std::unique_ptr<Sprite> gauge_img = nullptr;
	std::unique_ptr<Sprite> gauge_case_img = nullptr;
	std::unique_ptr<Sprite> gauge_case_mask = nullptr;
	std::unique_ptr<Sprite> gauge_anim1p = nullptr;
	std::unique_ptr<Sprite> gauge_anim2p = nullptr;

	std::unique_ptr<Sprite> HPbar_base = nullptr;
	std::unique_ptr<Sprite> HPbar_case = nullptr;
	std::unique_ptr<Sprite> HPbar_mask = nullptr;
	std::unique_ptr<Sprite> HPbar_fedo = nullptr;
	std::unique_ptr<Sprite> HPbar_design = nullptr;


	//画面描画用テクスチャ
	std::unique_ptr<Texture> color_texture = nullptr;
	std::unique_ptr<Texture> UI_texture = nullptr;
	std::unique_ptr<Texture> HP_texture = nullptr;
	//std::unique_ptr<Texture> normal_texture = nullptr;
	//std::unique_ptr<Texture> position_texture = nullptr;
	std::unique_ptr<Texture> luminance_texture = nullptr;
	std::unique_ptr<Texture> multi_blur_texture = nullptr;
	std::array<std::unique_ptr<Texture>, 10> blur_texture = { nullptr };

	//Gbuffer用スプライト
	std::unique_ptr<Sprite>	sprite = nullptr;

	//サンプラー
	std::shared_ptr<Sampler> sampler_clamp = nullptr;
	std::shared_ptr<Sampler> sampler_wrap = nullptr;

	//シェーダー
	std::unique_ptr<YRShader> skinShader = nullptr;
	std::unique_ptr<YRShader> animShader = nullptr;
	std::unique_ptr<YRShader> spriteShader = nullptr;
	std::unique_ptr<YRShader> geoShader = nullptr;
	std::unique_ptr<YRShader> ParallelToonShader = nullptr;
	std::unique_ptr<YRShader> ToonShader = nullptr;
	std::unique_ptr<YRShader> gaussShader = nullptr;
	std::unique_ptr<YRShader> multi_gaussShader = nullptr;
	std::unique_ptr<YRShader> spriteEx = nullptr;
	std::unique_ptr<YRShader> concentrationShader = nullptr;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	constantBuffer = nullptr;

	//プレイヤー管理系
	PlayerALL						PL;
	std::unique_ptr<Player>			player1p = nullptr;
	std::unique_ptr<Player>			player2p = nullptr;
	std::unique_ptr<GamepadBase>	pad1 = nullptr;
	std::unique_ptr<GamepadBase>	pad2 = nullptr;

	float							pl1_before_hp = 0.0f;
	float							pl2_before_hp = 0.0f;

	int							pl1_before_power = 0;
	int							pl2_before_power = 0;

	YR_Vector2		Danger_pos_p1 = { 0.0f,0.0f };		//1PのDANGER表記座標
	YR_Vector2		Danger_pos_p2 = { 0.0f,0.0f };		//2PのDANGER表記座標
	float			Danger_size = 0.0f;					//DANGER表記の大きさ
	float			Danger_alpha = 0.0f;				//DANGER表記のアルファ値
	float			desastal_size_p1 = 0.0f;			//1Pのディザスタル発光時のサイズ
	float			desastal_size_p2 = 0.0f;			//2Pのディザスタル発光時のサイズ
	float			desastal_alpha_p1 = 0.0f;			//1Pのディザスタル発光時のアルファ値
	float			desastal_alpha_p2 = 0.0f;			//2Pのディザスタル発光時のアルファ値

	//ゲームループ制御変数
	bool			pause = false;						//ポーズ中
	bool			start = false;						//対戦開始
	JUDGE_VICTORY	judge = JUDGE_VICTORY::NO_VICTORY;	//勝敗の有無
	float			start_timer = 0.0f;					//開始までの時間。Are You Ready?
	bool			end = false;						//勝敗がついた
	bool			fin = false;						//全て終わった
	float			endtimer = 0.0f;					//勝敗がついてから勝利画面に移行するまでに使用
	float			roll_timer = -1.0f;					//決着後にカメラを回す際に使用する
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
	DirectX::XMFLOAT4	HPColorSet(float hp, float max_hp);
	void				ComboImageSet();

	DirectX::XMFLOAT2	Distance(DirectX::XMFLOAT2& s_pos, DirectX::XMFLOAT2& e_pos);

	void				Control2PState(float elapsed_time);
	void				HPBar_Draw(	const DirectX::XMMATRIX& view,
									const DirectX::XMMATRIX& projection,
									const DirectX::XMFLOAT4& light_direction,
									const DirectX::XMFLOAT4& light_color,
									const DirectX::XMFLOAT4& ambient_color,
									float elapsed_time);
	void				UI_Draw(float elapsed_time);
	void				SetUITexture();
	void				SetHPTexture();

	void				PlayerMoveReq(int req_player_num, YR_Vector3 pos);
	void				PlayerMoveReq(int req_player_num, YR_Vector3 vec, float speed);

	void				IconDraw(
							const DirectX::XMMATRIX& view,
							const DirectX::XMMATRIX& projection,
							float					elapsed_time);

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

	void RenderUI();

	void RenderHP();

	void AIControll(float elapsed_time);

	void EndORGameCameraSet(float elapsed_time);

	void DangerDraw(float elapsed_time);

	void DangerSound();

	void DesastalFlash(float elapsed_time);

	void ComboDraw();

	/*struct CB_Multi_Render_Target
	{
		DirectX::XMFLOAT4	light_direction = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	light_color = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	ambient_color = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	eye_pos = { 0.0f,0.0f,0.0f,0.0f };
	};*/
};


//タイトルシーン
class SceneTitle : public SceneBase
{

public:
	std::unique_ptr<Sprite> test = nullptr;
	std::thread* t = NULL;
	int					load_state = 0;
	bool				load_fin = false;
	std::vector<float> testevent;

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
	std::unique_ptr<Sprite>	choice_mask = nullptr;
	std::unique_ptr<Sprite>	choice_anim = nullptr;


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
	std::shared_ptr<Sampler> sampler_wrap = nullptr;

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


//ロードシーン
//マッチ画面のイントロを再生するシーン
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
	//std::unique_ptr<Sprite>	load_bg = nullptr;

	std::shared_ptr<Sampler> sampler_wrap = nullptr;

	//シェーダー
	std::unique_ptr<YRShader> spriteShader = nullptr;

	float match_timer = 0.0f;

	float plus_match = 0.0f;

	float slow_add = 0.0f;

	float timer_Multiply = 0.0f;

	float sin_max = 0.0f;

	float space_time = 0.0f;

	float VS_size = 0.0f;

	float VS_alpha = 0.0f;

	float flash_size = 0.0f;
	float flash_alpha = 0.0f;

	float p1_pos_x = 0.0f;
	float p2_pos_x = 0.0f;

	float line_1p_x = 0.0f;
	float line_2p_x = 0.0f;

	//ラインに乗算する値
	float line_Multiply = 0.0f;

	std::unique_ptr<Sprite> flash = nullptr;

	enum class IntroState : int
	{
		P1,			//プレイヤー1
		SPACE1,		//少し間をおく
		P2,			//プレイヤー2
		SPACE2,		//少し間を置く
		P1P2,		//同時に出す
		FIN,		//sin値がmaxに達したらここに移動する(動かなくする)
	};

	IntroState intro_state = IntroState::P1;

	std::unique_ptr<Sprite> p1_cut = nullptr;
	std::unique_ptr<Sprite> p2_cut = nullptr;
	std::unique_ptr<Sprite> p1_name = nullptr;
	std::unique_ptr<Sprite> p2_name = nullptr;
	std::unique_ptr<Sprite> p1_name_edge = nullptr;
	std::unique_ptr<Sprite> p2_name_edge = nullptr;
	std::unique_ptr<Sprite> Box = nullptr;
	std::unique_ptr<Sprite> VS_Image = nullptr;

	std::unique_ptr<YRShader> titleShader = nullptr;
	std::unique_ptr<Texture> title_texture = nullptr;

	//Gbuffer用スプライト
	std::unique_ptr<Sprite>	sprite = nullptr;

	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	constantBuffer = nullptr;

	Title_CBuffer cbuffer_param;

	float				timer = 0.0f;
	void				Init();
	void				Update(float elapsed_time);
	void				Draw(float elapsed_time);
	void				UnInit();
	void				LoadData();
	bool				FedoOut(float elapsed_time);
	void				MatchStart();
	void				MatchUpdate(float elapsed_time);
	void				MatchDraw(float elapsed_time);
	void				IconLoad();
};


//セレクトシーン
class SceneSelect : public SceneBase
{
public:
	//ロード関係
	std::thread* t = NULL;
	int					load_state = 0;
	bool				load_fin = false;

	//シェーダー
	std::unique_ptr<YRShader> spriteShader = nullptr;
	std::unique_ptr<YRShader> selectShader = nullptr;
	std::unique_ptr<YRShader> spriteEx = nullptr;

	//画面描画用テクスチャ
	std::unique_ptr<Texture> color_texture = nullptr;
	std::unique_ptr<Texture> luminance_texture = nullptr;

	std::unique_ptr<Texture> select_texture = nullptr;

	//サンプラー
	std::shared_ptr<Sampler> sampler_clamp = nullptr;
	std::shared_ptr<Sampler> sampler_wrap = nullptr;

	//Gbuffer用スプライト
	std::unique_ptr<Sprite>	sprite = nullptr;

	//画像描画関係
	YR_Vector3			p1;					//プレイヤー1のカーソルの位置
	YR_Vector3			p2;					//プレイヤー2のカーソルの位置
	YR_Vector3			knight_icon_pos;	//ナイトのアイコンの位置
	YR_Vector3			p1_cut_pos;			//プレイヤー１のキャラ画像の位置
	YR_Vector3			p2_cut_pos;			//プレイヤー１のキャラ画像の位置
	YR_Vector3			name_distance;		//名前の位置(カットインからどれだけ離すか)
	float				sx = 0.0f;			//カットイン画像の描画指定位置
	float				sw = 0.0f;			//カットイン画像の描画指定大きさ
	float				icon_range = 0.0f;	//アイコン同士の距離
	float				Rato = 0.0f;		//アイコンの大きさ
	float				case_rato = 0.0f;	//アイコンケースの大きさ
	float				cursor_rato = 0.0f;	//選択したキャラ画像の大きさ
	float				cut_rato = 0.0f;	//カーソルの大きさ
	float				name_rato = 0.0f;	//名前の大きさ
	bool				p1Enter = false;	//プレイヤー1が決定したらtrue
	bool				p2Enter = false;	//プレイヤー1が決定したらtrue
	float				timer = 0.0f;
	bool				end = false;
	float				cursor_speed = 1.0f;//カーソルの速度
	float				image_alpha = 0.0f;	//画像のアルファ値
	float				p1_chara_alpha = 0.0f;//キャラ画像発光時に使用
	float				p2_chara_alpha = 0.0f;//キャラ画像発光時に使用

	YR_Vector2			ready_pos = { 0.0f,0.0f };	//Ready to Start画像の座標
	float				ready_rato = 1.0f;			//Ready to Start画像の大きさ

	//キャラ選択
	int					select_p1 = -1;		//プレイヤー1のキャラ番号
	int					select_p2 = -1;		//プレイヤー2のキャラ番号
	int					old_select_p1 = -1;	//プレイヤー1の前フレームのキャラ番号
	int					old_select_p2 = -1;	//プレイヤー2の前フレームのキャラ番号
	PLCOLOR				color_p1 = PLCOLOR::ORIGINAL;	//プレイヤー1のカラー番号
	PLCOLOR				color_p2 = PLCOLOR::ORIGINAL;	//プレイヤー2のカラー番号
	PLCOLOR				old_color_p1 = PLCOLOR::ORIGINAL;	//プレイヤー1の前フレームのカラー番号
	PLCOLOR				old_color_p2 = PLCOLOR::ORIGINAL;	//プレイヤー2の前フレームのカラー番号

	//ライン座標(必ず画面左から出てくる為、右端の座標のみ)
	YR_Vector2			line_red = { 0.0f, 0.0f };
	YR_Vector2			line_blue = { 0.0f, 0.0f };

	//ラインに乗算する値
	float line_Multiply = 0.0f;

	//Readyに乗算する値
	float ready_Multiply = 0.0f;

	//ラインの太さ(縦)
	float line_breadth = 1.0f;

public:

	//std::unique_ptr<Sprite>	select_img = nullptr;
	//std::unique_ptr<Sprite>	back = nullptr;

	//アイコン画像
	//std::array<std::unique_ptr<Sprite>, scastI(PLSELECT::PLSELECT_END)> icon_image;
	std::unique_ptr<Sprite> chara_case = nullptr;
	std::unique_ptr<Sprite> select_point = nullptr;
	std::unique_ptr<Sprite> white_box = nullptr;
	std::unique_ptr<Sprite> Box_sprite = nullptr;
	std::unique_ptr<Sprite> ready_to_start = nullptr;

	//キャラ選択判定用列挙
	enum class Select_P : int
	{
		NONE,
		P1,
		P2,
		ALL,
		P_END
	};
	std::array<std::unique_ptr<Sprite>, scastI(Select_P::P_END)> select_img;

	//キャラ画像描画用構造体
	struct Select_Chara_Case
	{
		YR_Vector3					pos;							//座標
		Select_P					select = Select_P::NONE;		//選択状態
		std::unique_ptr<Sprite>		name_image = nullptr;			//名前画像
		std::unique_ptr<Sprite>		edge_image = nullptr;			//名前縁画像
		std::array<std::unique_ptr<Sprite>,scastI(PLCOLOR::COLOR_END)>	icon_image;			//アイコン画像
	};
	std::array<Select_Chara_Case, scastI(PLSELECT::PLSELECT_END)> select_p;

	//定数バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer>	constantBuffer = nullptr;

	Title_CBuffer cbuffer_param;

public:

	void				Init();
	void				Update(float elapsed_time);
	void				Draw(float elapsed_time);
	void				UnInit();
	void				LoadData();
	void				IconLoad();
	bool				FadoOut(float elapsed_time);
	void				SetRenderTexture();
	void				NullSetRenderTexture();
	void				RenderTexture();

	void				PosSet();
	void				SelectCheck();
	void				DrawSelect(int num);
	void				DrawChara();

	void				Color_Add(PLCOLOR& color);
	void				Color_Sub(PLCOLOR& color);

	void				ColorChange();
	void				ColorAdjustment();

	void				ReadyStep(float elapsed_time);
	void				DrawReady();
};


//テストシーン
class SceneTest : public SceneBase
{
public:
	float	timer = 0.0f;

	bool cut_in = false;
	bool cut_fin = false;
	float cut_timer = 0.0f;
	bool timer_start = false;

	bool pause = false;

	bool match_on = false;
	float match_timer = 0.0f;

	float plus_match = 0.0f;

	float slow_add = 0.0f;

	float timer_Multiply = 0.0f;

	float sin_max = 0.0f;

	float space_time = 0.0f;

	float VS_size = 0.0f;

	float VS_alpha = 0.0f;

	float flash_size = 0.0f;
	float flash_alpha = 0.0f;

	float knight_1p_pos_x = 0.0f;
	float knight_2p_pos_x = 0.0f;

	float line_1p_x = 0.0f;
	float line_2p_x = 0.0f;

	float line_Multiply = 0.0f;

	std::unique_ptr<Sprite> flash = nullptr;

	enum class IntroState : int
	{
		P1,			//プレイヤー1
		SPACE1,		//少し間をおく
		P2,			//プレイヤー2
		SPACE2,		//少し間を置く
		P1P2,		//同時に出す
		FIN,		//sin値がmaxに達したらここに移動する(動かなくする)
	};

	IntroState intro_state = IntroState::P1;

	std::unique_ptr<Sprite> test = nullptr;

	std::unique_ptr<Sprite> cutFrame = nullptr;
	std::unique_ptr<Sprite> cutMask = nullptr;
	std::unique_ptr<Sprite> cutIn = nullptr;
	std::unique_ptr<Sprite> knight_1p_cut = nullptr;
	std::unique_ptr<Sprite> knight_2p_cut = nullptr;
	std::unique_ptr<Sprite> knight_name = nullptr;
	std::unique_ptr<Sprite> Box = nullptr;
	std::unique_ptr<Sprite> VS_Image = nullptr;


	DirectX::XMFLOAT3	eye_cut = { 34.156f,5.693,-14.232 };
	DirectX::XMFLOAT3	focus_cut = { 8.539f,5.693f,2.898f };
	float				fov_cut = 50.0f;
	float				near_cut = 1.4f;
	float				far_cut = 100000.0f;


	//カラーチェンジ用テクスチャ
	std::shared_ptr<Texture>	color_texture_main = nullptr;
	std::shared_ptr<Texture>	color_texture_face = nullptr;

	DirectX::XMFLOAT2	eye_offset = { 0.0f,0.0f };
	DirectX::XMFLOAT2	mouth_offset = { 0.0f,0.0f };

	std::shared_ptr<Model> knight = nullptr;
	std::shared_ptr<Model> earth = nullptr;
	std::shared_ptr<Model> wait_R = nullptr;
	std::shared_ptr<Model> special_R = nullptr;
	std::shared_ptr<Texture> board_texture = nullptr;
	std::unique_ptr<ModelAnim> motion = nullptr;
	std::unique_ptr<ModelAnim> earth_motion = nullptr;
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
	std::shared_ptr<Texture> knight_normal_map = nullptr;
	std::shared_ptr<Texture> knight_height_map = nullptr;

	std::shared_ptr<Texture> earth_normal_map = nullptr;
	std::shared_ptr<Texture> earth_height_map = nullptr;


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
	std::unique_ptr<YRShader> TessellationShader = nullptr;

	//テクスチャ
	std::unique_ptr<Texture> color_texture = nullptr;
	std::unique_ptr<Texture> normal_texture = nullptr;
	std::unique_ptr<Texture> position_texture = nullptr;
	std::unique_ptr<Texture> luminance_texture = nullptr;
	std::unique_ptr<Texture> multi_blur_texture = nullptr;
	std::array<std::unique_ptr<Texture>,10> blur_texture = { nullptr };
	std::unique_ptr<Texture> fur = nullptr;

	std::shared_ptr<Model> Neru = nullptr;
	std::unique_ptr<ModelAnim> Neru_base = nullptr;
	std::vector<std::shared_ptr<Model>> Neru_anims;

	Collision circle;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	constantBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	constantBuffer_Gauss = nullptr;

	DirectX::XMFLOAT3 knight_angle = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 knight_scale = { 0.1f,0.1f,0.1f };
	float	knight_scale_all = 0.1f;
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

	void MatchStart();

	void MatchUpdate(float elapsed_time);


	void NeruLoad();

	void NeruImGui();

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

	struct ConstantBufferForPerFrame
	{
		DirectX::XMFLOAT4	light_color = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	light_direction = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	ambient_color = { 0.0f,0.0f,0.0f,0.0f };
		DirectX::XMFLOAT4	eye_pos = { 0.0f,0.0f,0.0f,0.0f };
		float wave_time = 0.0f; //波の UV スクロール用時間
		float divide = 0.0f; //分割数（外と内）
		float dummy1 = 0.0f;
		float dummy2 = 0.0f;
	};
};