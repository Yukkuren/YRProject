//#pragma once

#ifndef PLAYERBASE_H_
#define PLAYERBASE_H_

#include "Skinned_mesh.h"

#include "YR_VectorMaster.h"
#include "HitBox.h"
#include "AttackBox.h"
#include "Tracking.h"
#include <memory>
#include "YRGamePad.h"
#include "Texture.h"
#include <vector>
#include <string>
#include "sprite.h"


constexpr float POS_Y = 0.0f;							//最低の高さ
constexpr float GAUGE_MAX = 100.0f;						//ゲージ最大量
constexpr int	Track_max = 2;							//ホーミングダッシュ最大数
constexpr float	non_target = 110.0f;					//この値を入れられたフレームは条件から外れるようにする
constexpr float target_max = 100.0f;					//条件式でこの値以上は外れるようにする
constexpr float attenuation_slam = 8.0f;				//滑り中速度の減衰率
constexpr float draw_guarf_effect_add_pos_x = 3.0f;		//ガードエフェクト描画時の位置補間値
constexpr float Reflection_range_min = 100.0f;			//この数値以上の吹っ飛びなら壁で跳ね返る
constexpr float Reflection_attenuation_factor = 0.8f;	//壁反射時の減衰率
constexpr float cut_in_max_time = 0.5f;					//カットイン表示時間
constexpr float passive_max_time = 0.2f;				//受け身中無敵時間

//--------------------------------------
//	**キャラ名設定
//	・新しいキャラを追加するならここに記入
//--------------------------------------
enum class PLSELECT :int
{
	KNIGHT = 0,
	KEN,
	PLSELECT_END,
};


//--------------------------------------
//	**アニメーション選択
//	・NodeChange時に送り、アニメーションを指定する
//--------------------------------------
enum class AnimAtk :int
{
	FREAM = 0,	//発生
	LATER,		//後隙
	TIMER,		//持続
};



//--------------------------------------
//	**攻撃時のプレイヤーの地面判定列挙
//--------------------------------------
enum class Ground_C :int
{
	AIR = 0,	//空中
	GROUND,		//地上
	EITHER,		//どちらでも
	END,
};



//---------------------------------------------
// **スピードクラス**
//・プレイヤーに加算するX,Y軸方向の力を処理する
//・攻撃関数実行時にSet関数を動かし、速度を入れる
//・空中攻撃時のポップなどに利用する
//----------------------------------------------
class Speed
{
public:
	float speed = 0.0f;
	const float decrease_value = 100.0f;		//スピードの減少値

	void Set(float speed)
	{
		this->speed = speed;
	};
	float Update(float elapsed_time)
	{
		if (speed > 0.0f)
		{
			speed -= (decrease_value * elapsed_time);
		}
		else
		{
			speed = 0.0f;
		}
		return speed;
	};
};


//---------------------------------------------
// **ステートクラス**
//・ActStateは攻撃以外の判断をする。判断しないものはcaseに入れない(その他の条件式回避のために使用する)
//・AttackStateはActStateがATTACKだった場合どの攻撃をしているかを判断する
//----------------------------------------------
enum class ActState : int
{
	//右向きの時
	NONE = 0,		//何もない(default)
	WAIT,			//待機(default)m
	GUARD,			//ガード(default)
	SQUAT,			//しゃがみ(default)m
	RETREAT,		//後退(default)m
	DUMMY,			//ダミー(数値補間用)
	DASH,			//ダッシュ(default)m
	BACK,			//バックステップ(default)m
	JUMP,			//ジャンプ(default)m
	AIR_F,			//空中前ダッシュ(default)
	AIR_B,			//空中後ダッシュ(default)
	STATENONE,		//ステートを奪われた状態。相手に掴まれてるor超必？
	WAKE,			//ダウンから起き上がる
	PASSIVE,		//受け身をとる
	DOWN,			//ダウン状態
	FALL,			//空中でダウンしている状態
	KNOCK,			//攻撃を受けてのけぞる
	SLAM,			//叩きつけられ状態
	DOWN_HIT,		//ダウン攻撃を受けた
	ATTACK,			//攻撃中
	ACT_END,		//(終点)
};

enum class AttackState : int
{
	NONE = 0,		//攻撃無し
	JAKU,			//弱m
	THU,			//中(下中)m
	D_KYO,			//下強(中の次に出る強攻撃)
	D_JAKU,			//下弱m
	D_THU,			//下中m
	U_KYO,			//上強(強攻撃)m
	A_JAKU,			//空弱攻撃m
	A_THU,			//空中攻撃m
	A_KYO,			//空強攻撃m
	A_UKYO,			//空上強攻撃(打ち上げ攻撃)m
	STEAL,			//掴みm
	SLOW,			//投げm
	TRACK_DASH,		//ホーミングダッシュm
	JAKU_RHURF,		//前弱必殺
	THU_RHURF,		//前中必殺
	KYO_RHURF,		//前強必殺
	JAKU_LHURF,		//後弱必殺
	THU_LHURF,		//後中必殺
	KYO_LHURF,		//後強必殺
	SPECIAL_ATTACK,	//前超必殺m
	DESIRE_SPECIAL,	//後超必殺
	DESIRE_METEOR,	//即死技
	EXTENDATK,		//無敵攻撃
	JAKU_THU,		//弱の次に出る中攻撃m
	JAKU_KYO,		//中の次に出る強攻撃m
	A_JAKU_RHURF,	//空中前弱必殺
	A_THU_RHURF,	//空中前中必殺
	A_KYO_RHURF,	//空中前強必殺
	A_JAKU_LHURF,	//空中後弱必殺
	A_THU_LHURF,	//空中後中必殺
	A_KYO_LHURF,	//空中後強必殺
	A_SPECIAL_ATTACK,//空中前超必殺
	A_DESIRE_SPECIAL,//空中後超必殺

	COMBO_X,		//Xボタンコンボ
	COMBO_Y,		//Yボタンコンボ
	COMBO_B,		//Bボタンコンボ

	VERSATILE_ATTACK,//Aボタンを押すと出る攻撃(プレイヤー選択)
	A_VERSATILE_ATTACK,//Aボタンを押すと出る空中攻撃(プレイヤー選択)

	ATTACK_END,		//最大サイズ
};

//Desire(欲望、望み)とCrystal(結晶)


//---------------------------------------------
// **空中ダッシュステート列挙**
//・NONE以外の場合はそれぞれ適用した挙動を行う
//----------------------------------------------
enum class AirDashState : int
{
	NONE = 0,		//何もない
	AIR_F,			//空中ダッシュ
	AIR_B,			//空中バックダッシュ
	ACT_END,		//(終点)
};


//---------------------------------------------------------------
// **攻撃内容保存構造体**
//・攻撃の発生フレーム、内部パラメータ変数、その生成数を保存する構造体
//---------------------------------------------------------------
struct AttackSingle
{
public:
	std::vector<AttackParameter>	parameter;		//内部パラメーター
	int								quantity = 1;	//一度の攻撃で生成する当たり判定の数量
	float							fream = 0.0f;	//発生フレーム
};


//------------------------------------------------------------------------
// **攻撃リストクラス**
//・攻撃のパラメーターをキャラ生成時に読み込み、
//　攻撃を行う際に当たり判定に対応したパラメーターを送る
//			以下、整理用
//１：攻撃発生ボタンを押す
//２：プレイヤーに発生フレームを送る
//３：発生フレームになったらSetAttack関数で当たり判定を生成
//４：当たり判定の持続は当たり判定本体で調整する為何も触らない
//５：攻撃が複数回発生する場合はattack_maxにその回数を入れてその分ループさせる
//
//		攻撃当たり判定
//１：生成時にInit関数でパラメーターを送る。
//		この時、distanceに入れるのはプレイヤーからどれだけ離れているかという値
//２：Update関数の引数にプレイヤーのposを渡し、常に位置が更新されるようにする
//３：AttackBoxクラスに「攻撃判定に付与するスピードを増加させる関数」を作り、Updateで値を変える
//etc...Update内：pos=player_pos+distance+speed;
//						speedは関数で変えられるため、posの位置が変わっていく
//-------------------------------------------------------------------------
class AttackList
{
public:
	AttackState						attack_name;	//攻撃名(プレイヤー生成時に保存)
	int								attack_max;		//攻撃回数
	std::vector<AttackSingle>		attack_single;	//攻撃内容保存
	float							later;			//後隙フレーム
	int								now_attack_num;	//現在実行している攻撃番号は何番かを示す(attack_maxまで回ったらlaterをプレイヤーに)
	PAD								linkage_button;	//どのボタンで攻撃を発生させるか
	Command							linkage_command;//どのコマンドで攻撃を発生させるか
	Ground_C						ground_on;		//攻撃は空中、地上、どちらでも発生させるか
	bool							squat_on;		//しゃがみ攻撃がどうか(trueでしゃがみ攻撃)
	int								need_power;		//攻撃を行うのに必要なゲージ量(1単位で記述)
	PAD								linkage_stick;	//どの方向への入力で攻撃を発生させるか
	AttackState						aid_attack_name;//ゲージが足りなかった場合出す技
	AttackState						real_attack;	//実際の攻撃(基本はattack_nameと同じものを入れ、特定の攻撃と同じ攻撃を出す場合はその攻撃名を入れる)
	bool							speed_on;		//スピードを入れる場合はtrue
	YR_Vector3						speed;			//攻撃判定に加算する速度
	float							advance_speed;	//攻撃時前進する距離
	AttackState						combo;			//連打した時にするコンボ
	HitResult						conditions_hit;	//キャンセルするための攻撃ヒット条件
	float							timer;			//飛び道具の場合自身に付与する持続時間
	bool							traject_on;		//剣の軌跡を表示するならtrue
public:
	AttackList() : now_attack_num(0), attack_name(AttackState::NONE), later(0.0f),
		attack_max(0), linkage_button(PAD::BUTTOM_END), linkage_command(Command::NOCOMMAND), ground_on(Ground_C::GROUND), squat_on(false),
		need_power(0), linkage_stick(PAD::BUTTOM_END), aid_attack_name(AttackState::NONE), real_attack(attack_name),
		speed_on(false), speed(0.0f, 0.0f, 0.0f), advance_speed(0.0f), combo(AttackState::NONE), conditions_hit(HitResult::HIT),timer(0.0f),traject_on(true) {};
	//攻撃当たり判定を生成する
	void SetAttack(std::vector<AttackBox> *atk, float rightOrleft, YR_Vector3 pl_pos)
	{
		for (int quantity = 0; quantity < attack_single[now_attack_num].quantity; quantity++)
		{
			atk->emplace_back();
			atk->back().Init(scastI(attack_name), attack_single[now_attack_num].parameter[quantity], rightOrleft, pl_pos);
		}
		now_attack_num++;
	}

	//飛び道具攻撃当たり判定をセットする
	void SetAttack(std::vector<AttackBox>* atk, float rightOrleft, YR_Vector3 pl_pos, YR_Vector3 plus_speed)
	{
		for (int quantity = 0; quantity < attack_single[now_attack_num].quantity; quantity++)
		{
			atk->emplace_back();
			atk->back().Init(scastI(attack_name), attack_single[now_attack_num].parameter[quantity], rightOrleft, pl_pos, plus_speed);
		}
		now_attack_num++;
	}
};




//---------------------------------------------------------------------------
// **当たり判定リストクラス**
//・当たり判定のパラメーターを各行動ごとに保存するクラス
//・プレイヤークラスのUpdateで毎回パラメーターを入れていく
//・ただし当たり判定の数はキャラによって変わる為、変数はvector型で持たせ、
//　キャラ生成時に必要な個数分生成する
//----------------------------------------------------------------------------
class HitParameterList
{
public:
	std::array<HitParameter,scastI(ActState::ACT_END)>			act_parameter;		//攻撃以外の行動時の当たり判定
	std::array<HitParameter, scastI(AttackState::ATTACK_END)>	attack_parameter;	//攻撃時の当たり判定
};




//---------------------------------------------------------------
// **アニメーション調整値保存構造体**
//・発生、持続、後隙それぞれのアニメーション速度調整値を保存する構造体
//---------------------------------------------------------------
struct Animation_Coordinate
{
public:
	float							fream = 1.0f;			//発生
	float							timer = 1.0f;			//持続
	float							later = 1.0f;			//後隙
};


//---------------------------------------------------------------
// **コンボ保存構造体**
//・コンボの種類、現在の番号を保存する構造体
//---------------------------------------------------------------
struct ComboList
{
public:
	std::vector<AttackState>		combolist;		//コンボリスト
	int								now_pos = 0;	//現在のコンボの位置
};


class Player
{
protected:
	const float jump_max_time = 0.05f;	//ジャンプしたらjump_can_timerに入れる値
public:
	//ゲーム内処理用変数
	float				hp = 0;
	int					jumpcount = 2;							//ジャンプの回数保持
	int					air_dash_count = 1;						//空中ダッシュの回数保持
	bool				jumpflag = false;						//ジャンプしている
	bool				max_jump_flag = false;					//ジャンプの最大値を超えた
	float				jump_can_timer = 0.0f;					//ジャンプ可能になるまでの時間を測定する(0.0以下になればジャンプ可能)
	bool				ground = true;							//TRUEなら地面についている
	bool				attack = false;							//TRUEなら攻撃中
	ActState			act_state = ActState::NONE;				//今の行動。また行動不可ならどういう状態か
	ActState			rival_state = ActState::NONE;			//相手の今の行動。SceneGameで代入する
	AttackState			attack_state = AttackState::NONE;		//今何の攻撃をしているか
	AttackState			last_attack = AttackState::NONE;		//最後になんの攻撃をしたか
	AirDashState		air_dash_state = AirDashState::NONE;	//空中ダッシュステート
	float				rightOrleft = 1.0f;						//右向きなら*1左向きなら*-1
	bool				moveflag = false;						//TRUEなら動いている
	float				fream = non_target;						//発生フレーム
	float				later = non_target;						//後隙フレーム(初期値にnon_targetを入れておく)
	float				timer = non_target;						//持続フレーム(初期値にnon_targetを入れておく)
	float				knocktimer = 0.0f;						//喰らい時間
	float				gravity = 0.0f;							//重力値
	float				down_force = 0.0f;						//ジャンプ時に重力とは別に働く下方向の力
	float				fall_force = 0.0f;						//空中ダウン中に重力とは別に働く下方向の力
	bool				finish = false;							//行動が終了したときのみtrue
	bool				step = false;							//ステップ中はtrue(他の処理を行わない)
	bool				hightrigger = false;					//ハイジャンプ時true、通常時false
	int					trackgauge = 2;							//追尾ダッシュの残り回数
	float				gauge = 0.0f;							//ゲージ
	int					power = 1;								//ゲージが最大になると1増える。特殊技はこっちを消費する
	float				steal_escape = 0.0f;					//投げ抜け可能時間
	int					combo_count = 0;						//コンボカウント。こっちは攻撃を受けた際に増え、相手に送るカウント
	int					now_player = 0;							//どのプレイヤーがこのキャラを操作しているか(1:1P、2:2P)
	float				anim_ccodinate = 1.0f;					//アニメーション速度を調整する変数
	int					stop_state = 0;							//ヒットストップ中の処理で使用
	float				cut_in_timer = 0.0f;					//カットインの表示時間
	float				passive_timer = 0.0f;					//受け身中の無敵時間測定変数
	int					combo_count_player = 0;					//コンボカウント。こちらは相手から受け取る
	DirectX::XMFLOAT4	light_direction =
		DirectX::XMFLOAT4(-1.0f, -0.1, 1.0f, 0.0f);				//ライトの進む方向
	HitResult			hit_result = HitResult::NOT_OCCURRENCE;	//攻撃が当たった場合の結果を保存する
	HitResult			atk_result = HitResult::HIT;			//攻撃決定時にキャンセル用の条件を保存するための変数
	std::vector<AttackBox> atk;									//攻撃当たり判定
	std::vector<AttackBox> projectile_atk;						//飛び道具当たり判定
	std::vector<HitBox>	hit;									//当たり判定
	bool				traject_on = true;						//剣の軌跡を表示する場合true(攻撃発生時にパラメーターから取得する)

	std::array<Animation_Coordinate, scastI(AttackState::ATTACK_END)>	ac_attack;	//攻撃ごとのアニメーション調整値
	std::array<Animation_Coordinate, scastI(ActState::ACT_END)>			ac_act;		//行動ごとのアニメーション調整値

	float												jump_later = 1.0f;			//ジャンプの着地隙
	float												dash_later = 1.0f;			//ダッシュの後隙

	std::vector<AttackList>								attack_list;	//攻撃のリスト。生成時に読み込み、保存する(攻撃発生時にパラメーターを送る)
	std::vector<HitParameterList>						hitparam_list;	//当たり判定のリスト。生成時に読み込み、保存する(当たり判定の数だけ生成する)
	ComboList											combolist_X;	//Xボタンコンボリスト
	ComboList											combolist_Y;	//Yボタンコンボリスト
	ComboList											combolist_B;	//Bボタンコンボリスト

public:
	//モデル用変数
	//MeshMotion						motion;
	//std::unique_ptr<Skinned_mesh>	base = nullptr;
public:
	//座標、モデル描画用変数
	YR_Vector3		pos = { 0.0f,0.0f,0.0f };
	YR_Vector3		scale = { 1.0f,1.0f,1.0f };
	YR_Vector3		angle = { 0.0f,0.0f,0.0f };
	YR_Vector3		speed = { 0.0f,0.0f,0.0f };
	YR_Vector3		stop_pos = { 0.0f,0.0f,0.0f };
	Tracking		tracking;
	Speed			speed_X;
	Speed			speed_Y;
	std::unique_ptr<GamepadBase> pad;

	int				chara_color = 0;		//キャラのカラー番号


public:
	//画像
	std::unique_ptr<Sprite> cutFrame = nullptr;		//カットインのフレーム
	std::unique_ptr<Sprite> cutMask = nullptr;		//カットインのマスク画像
	std::unique_ptr<Sprite> cutIn = nullptr;		//カットイン画像

public:
	//基本処理関数
	virtual void Init(YR_Vector3 InitPos) = 0;
	virtual void LoadData(int color_number) = 0;
	virtual void Uninit() = 0;
	virtual void Update(float decision,float elapsed_time) = 0;
	virtual void Draw(
		YRShader				*parallel_shader,
		YRShader				*shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time ) = 0;

	virtual void DrawDEBUG(
		YRShader* geoshader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time) = 0;

	virtual void DrawCutIn(
		YRShader* shader,
		float elapsed_time
	) = 0;

	virtual void TextDraw()=0;

	virtual ~Player() = default;

public:
	//行動系関数

	virtual void Move(float decision) = 0;
	virtual void MoveStop() = 0;
	virtual bool Step(float elapsed_time) = 0;
	virtual void AirDash(float elapsed_time) = 0;
	virtual void Jump() = 0;
	virtual void JumpUpdate(float decision, float elapsed_time) = 0;
	virtual void DamageCheck(float decision) = 0;
	virtual void Guard(float decision) = 0;
	virtual void Squat() = 0;
	virtual void GuardBack(float elapsed_time) = 0;
	virtual void FallUpdate(float elapsed_time) = 0;
	virtual void DownUpdate() = 0;
	virtual void PassiveUpdate(float elapsed_time) = 0;
	virtual void WakeUp() = 0;
	virtual void GaugeUp(float add) = 0;
	virtual void StateNone(float elapsed_time) = 0;
	virtual void KnockUpdate(float elapsed_time) = 0;
	virtual void DownHitUpdate(float elapsed_time) = 0;
	virtual void SlamUpdate(float elapsed_time) = 0;

	virtual void HitBoxTransition(HitBoxState state) = 0;

	virtual void AttackInput() = 0;
	virtual void Attack(float decision, float elapsed_time) = 0;
	virtual void AttackSwitch(float decision, float elapsed_time, AttackState attack_state = AttackState::NONE) = 0;

	virtual void WinAnimSet() = 0;

	virtual void NoneChange() = 0;

	virtual void StopUpdate() = 0;
	virtual void StopEnd() = 0;
	virtual void TrackDash(float decision, float elapsed_time) = 0;

	virtual bool Intro(float elapsed_time) = 0;
	virtual void IntroDEBUG() = 0;
	virtual bool WinPerformance(float elapsed_time) = 0;
	virtual void WinDEBUG() = 0;

	virtual void ReadySet() = 0;

	virtual bool AttackLoad() = 0;
	virtual bool AttackWrite() = 0;

	virtual bool ComboSet() = 0;					//コンボ開始時、どの攻撃をしているかを確認してその後のコンボをセットする
	virtual void ComboUpdate() = 0;					//コンボ更新。ステートを設定する

	virtual void StopHitParamUpdate() = 0;

public:
	virtual bool AttackEndCheck() = 0;								//攻撃当たり判定が全て終了しているか確認する
	virtual void EndAttackErase() = 0;								//終了した攻撃当たり判定を全て消去する。
	virtual void AllAttackClear() = 0;								//全ての攻撃当たり判定を消去する
	virtual void AttackUpdate(float elapsed_time) = 0;				//攻撃判定が存在する時のみ更新
	virtual void HitResultUpdate() = 0;								//攻撃判定が存在する時のみ更新(当たり判定が取得した結果をプレイヤーに送る)

};

#endif // !PLAYERBASE_H_

