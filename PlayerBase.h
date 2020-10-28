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


constexpr float POS_Y = 0.0f;		//最低の高さ
constexpr float GAUGE_MAX = 100.0f;	//ゲージ最大量
constexpr int	Track_max = 2;		//ホーミングダッシュ最大数
constexpr float	non_target = 110.0f;//この値を入れられたフレームは条件から外れるようにする
constexpr float target_max = 100.0f;//条件式でこの値以上は外れるようにする

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

//---------------------------------------------
// **スピードクラス**
//・プレイヤーに加算するY軸方向の力を処理する
//・攻撃関数実行時にSet関数を動かし、速度を入れる
//・空中攻撃時のポップなどに利用する
//----------------------------------------------
class Speed
{
public:
	float speedY = 0.0f;
	const float decrease_value = 100.0f;		//スピードの減少値

	void Set(float speed)
	{
		speedY = speed;
	};
	float Update(float elapsed_time)
	{
		if (speedY > 0.0f)
		{
			speedY -= (decrease_value * elapsed_time);
		}
		else
		{
			speedY = 0.0f;
		}
		return speedY;
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
	MOVER,			//右移動(default)m
	MOVEL,			//左移動(default)m
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

	ATTACK_END,		//最大サイズ
};

//Desire(欲望、望み)とCrystal(結晶)

//---------------------------------------------------------------
// **攻撃内容保存構造体**
//・攻撃の発生フレーム、内部パラメータ変数、その生成数を保存する構造体
//---------------------------------------------------------------
struct AttackSingle
{
public:
	std::vector<AttackParameter>	parameter;		//内部パラメーター
	int								quantity;		//一度の攻撃で生成する当たり判定の数量
	float							fream;			//発生フレーム
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
	bool							ground_on;		//攻撃は空中、地上どちらで発生させるか(trueで地上発生)
	bool							squat_on;		//しゃがみ攻撃がどうか(trueでしゃがみ攻撃)
	float							need_gauge;		//攻撃を行うのに必要なゲージ量(1.0単位で記述)
	PAD								linkage_stick;	//どの方向への入力で攻撃を発生させるか
	AttackState						aid_attack_name;//ゲージが足りなかった場合出す技
	AttackState						real_attack;	//実際の攻撃(基本はattack_nameと同じものを入れ、特定の攻撃と同じ攻撃を出す場合はその攻撃名を入れる)
	bool							speed_on;		//スピードを入れる場合はtrue
	YR_Vector3						speed;			//攻撃判定に加算する速度
public:
	AttackList() : now_attack_num(0), attack_name(AttackState::NONE), later(0.0f),
		attack_max(0), linkage_button(PAD::BUTTOM_END), linkage_command(Command::NOCOMMAND), ground_on(true), squat_on(false),
		need_gauge(0.0f), linkage_stick(PAD::BUTTOM_END), aid_attack_name(AttackState::NONE), real_attack(attack_name),
		speed_on(false), speed(0.0f, 0.0f, 0.0f) {};
	//攻撃当たり判定を生成する
	void SetAttack(std::vector<AttackBox> *atk, float rightOrleft, YR_Vector3 pl_pos)
	{
		for (int quantity = 0; quantity < attack_single[now_attack_num].quantity; quantity++)
		{
			atk->push_back(AttackBox());
			atk->back().Init(attack_single[now_attack_num].parameter[quantity], rightOrleft, pl_pos);
		}
		now_attack_num++;
	}

	//飛び道具攻撃当たり判定をセットする
	void SetAttack(std::vector<AttackBox>* atk, float rightOrleft, YR_Vector3 pl_pos, YR_Vector3 plus_speed)
	{
		for (int quantity = 0; quantity < attack_single[now_attack_num].quantity; quantity++)
		{
			atk->push_back(AttackBox());
			atk->back().Init(attack_single[now_attack_num].parameter[quantity], rightOrleft, pl_pos, plus_speed);
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
	float							fream;			//発生
	float							timer;			//持続
	float							later;			//後隙
};


class Player
{
protected:
	const float jump_max_time = 0.05f;	//ジャンプしたらjump_can_timerに入れる値
public:
	//ゲーム内処理用変数
	float				hp = 0;
	int					jumpcount;		//ジャンプの回数保持
	bool				jumpflag;		//ジャンプしている
	bool				max_jump_flag;	//ジャンプの最大値を超えた
	float				jump_can_timer;	//ジャンプ可能になるまでの時間を測定する(0.0以下になればジャンプ可能)
	bool				ground;			//TRUEなら地面についている
	bool				drawset;		//特定の画像を描画したときにtrueにして通常の画像を描画しない
	bool				attack;			//TRUEなら攻撃中
	ActState			act_state;		//今行動可能か。また行動不可ならどういう状態か
	AttackState			attack_state;	//今何の攻撃をしているか
	float				rightOrleft;	//右向きなら*1左向きなら*-1
	bool				moveflag;		//TRUEなら動いている
	float				fream;			//発生フレーム
	float				later;			//後隙フレーム(初期値にnon_targetを入れておく)
	float				knocktimer;		//喰らい時間
	float				gravity;		//重力値
	float				down_force;		//ジャンプ時に重力とは別に働く下方向の力
	float				fall_force;		//空中ダウン中に重力とは別に働く下方向の力
	float				specialfream;	//弱コンボ等でフレームを減らす際の数値
	bool				finish;			//行動が終了したときのみtrue
	bool				step;			//ステップ中はtrue(他の処理を行わない)
	bool				hightrigger;	//ハイジャンプ時true、通常時false
	int					trackgauge;		//追尾ダッシュの残り回数
	float				gauge;			//ゲージ
	int					power;			//ゲージが最大になると1増える。特殊技はこっちを消費する
	int					rival_state;	//相手のステート
	float				steal_escape;	//投げ抜け可能時間
	int					combo_count;	//コンボカウント
	int					now_player;		//どのプレイヤーがこのキャラを操作しているか(1:1P、2:2P)
	float				anim_ccodinate;	//アニメーション速度を調整する変数
	int					stop_state;		//ヒットストップ中の処理で使用
	HitResult			hit_result;		//攻撃が当たった場合の結果を保存する
	std::vector<AttackBox> atk;			//当たり判定
	
	std::array<Animation_Coordinate, scastI(AttackState::ATTACK_END)>	ac_attack;	//攻撃ごとのアニメーション調整値
	std::array<Animation_Coordinate, scastI(ActState::ACT_END)>			ac_act;		//行動ごとのアニメーション調整値

	float												jump_later = 1.0f;			//ジャンプの着地隙
	float												dash_later = 1.0f;			//ダッシュの後隙

	std::vector<AttackList>								attack_list;	//攻撃のリスト。生成時に読み込み、保存する(攻撃発生時にパラメーターを送る)
	std::vector<HitParameterList>						hitparam_list;	//当たり判定のリスト。生成時に読み込み、保存する(当たり判定の数だけ生成する)

public:
	//モデル用変数
	//MeshMotion						motion;
	//std::unique_ptr<Skinned_mesh>	base = nullptr;
public:
	//座標、モデル描画用変数
	YR_Vector3		pos;
	YR_Vector3		scale;
	YR_Vector3		angle;
	YR_Vector3		speed;
	YR_Vector3		stop_pos;
	Tracking		tracking;
	Speed			speed_Y;
	std::unique_ptr<GamepadBase> pad;

	int				chara_color;		//キャラのカラー番号

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
		const DirectX::XMFLOAT4& light_direction,
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

	virtual void TextDraw()=0;

public:
	//行動系関数
	
	virtual void Move(float decision) = 0;
	virtual bool Step() = 0;
	virtual void Jump() = 0;
	virtual void JumpUpdate(float elapsed_time) = 0;
	virtual void DamageCheck() = 0;
	virtual void Guard(float decision) = 0;
	virtual void Squat() = 0;
	virtual void GuardBack(float elapsed_time) = 0;
	virtual void FallUpdate(float elapsed_time) = 0;
	virtual void DownUpdate() = 0;
	virtual void PassiveUpdate() = 0;
	virtual void WakeUp() = 0;
	virtual void GaugeUp(float add) = 0;
	virtual void CancelList() = 0;
	virtual void StateNone() = 0;
	virtual void KnockUpdate(float elapsed_time) = 0;

	virtual void HitBoxTransition(HitBoxState state) = 0;

	virtual void AttackInput() = 0;
	virtual void Attack(float decision, float elapsed_time) = 0;

	virtual void WinAnimSet() = 0;

	//virtual float GetPosX();
	virtual HitBox* GetHit() = 0;
	virtual AttackBox* GetAttack() = 0;
	virtual int GetMax(int n) = 0;
	//virtual float& GetPosX2();
	/*virtual float GetRivalPos(float &pos) = 0;*/

	virtual void NoneChange() = 0;

	virtual void StopUpdate() = 0;
	virtual void StopEnd() = 0;
	virtual void TrackDash(float decision, float elapsed_time) = 0;

	virtual bool Intro(float elapsed_time) = 0;
	virtual void IntroDEBUG() = 0;
	virtual bool WinPerformance() = 0;

	virtual void ReadySet() = 0;

	virtual bool AttackLoad() = 0;
	virtual bool AttackWrite() = 0;

public:
	virtual bool AttackEndCheck() = 0;				//攻撃当たり判定が全て終了しているか確認する
	virtual void EndAttackErase() = 0;				//終了した攻撃当たり判定を全て消去する。
	virtual void AllAttackClear() = 0;				//全ての攻撃当たり判定を消去する
	virtual void AttackUpdate(float elapsed_time) = 0;				//攻撃判定が存在する時のみ更新

};

#endif // !PLAYERBASE_H_

