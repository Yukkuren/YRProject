//#pragma once

#ifndef PLAYERBASE_H_
#define PLAYERBASE_H_

#include "Skinned_mesh.h"

#include "YR_VECTOR3.h"
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

	void Set(float speed)
	{
		speedY = speed;
	};
	float Update(float elapsed_time)
	{
		if (speedY > 0.0f)
		{
			speedY -= elapsed_time;
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
	NONE = 0,		//何もない(default)
	WAIT,			//待機(default)m
	GUARD,			//ガード(default)
	SQUAT,			//しゃがみ(default)m
	MOVER,			//右移動(default)
	MOVEL,			//左移動(default)
	DASH,			//ダッシュ(default)
	BACK,			//バックステップ(default)
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
};

enum class AttackState : int
{
	NONE = 0,		//攻撃無し
	JAKU,			//弱m
	THU,			//中m
	KYO,			//強(対空&下強)m
	D_JAKU,			//下弱m
	D_THU,			//下中m
	U_KYO,			//上強m
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
	SPECIAL_ATTACK,	//前超必殺
	SUPER_ATTACK,	//後超必殺
	EXTENDATK,		//無敵攻撃
	ATTACK_END,		//最大サイズ
};


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
public:
	AttackList() : now_attack_num(0), attack_name(AttackState::NONE), later(0.0f), attack_max(0) {};
	//攻撃当たり判定を生成する
	void SetAttack(std::vector<AttackBox> *atk, float rightOrleft)
	{
		for (int quantity = 0; quantity < attack_single[now_attack_num].quantity; quantity++)
		{
			atk->push_back(AttackBox());
			atk->back().Init(attack_single[now_attack_num].parameter[quantity],rightOrleft);
		}
		now_attack_num++;
	}

	//飛び道具攻撃当たり判定をセットする
	void SetAttack(std::vector<AttackBox>* atk, float rightOrleft, YR_Vector3 plus_speed)
	{
		for (int quantity = 0; quantity < attack_single[now_attack_num].quantity; quantity++)
		{
			atk->push_back(AttackBox());
			atk->back().Init(attack_single[now_attack_num].parameter[quantity], rightOrleft, plus_speed);
		}
		now_attack_num++;
	}
};


class Player
{
public:
	//ゲーム内処理用変数
	float				hp = 0;
	int					jumpcount;
	bool				jumpflag;		//ジャンプしている
	bool				max_jump_flag;	//ジャンプの最大値を超えた
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
	float				specialfream;	//弱コンボ等でフレームを減らす際の数値
	bool				finish;			//行動が終了したときのみtrue
	bool				step;
	bool				hightrigger;	//ハイジャンプ時true、通常時false
	int					trackgauge;		//追尾ダッシュの残り回数
	float				gauge;			//ゲージ
	int					power;			//ゲージが最大になると1増える。特殊技はこっちを消費する
	int					rival_state;	//相手のステート
	float				steal_escape;	//投げ抜け可能時間
	int					combo_count;	//コンボカウント
	int					now_player;		//どのプレイヤーがこのキャラを操作しているか(1:1P、2:2P)
	float				anim_ccodinate;	//アニメーション速度を調整する変数
	std::vector<AttackBox> atk;


	std::vector<AttackList>		attack_list;	//攻撃のリスト。生成時に読み込み、保存する(攻撃発生時にパラメーターを送る)

public:
	//モデル用変数
	MeshMotion						motion;
	std::unique_ptr<Skinned_mesh>	base = nullptr;
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

public:
	//行動系関数
	
	virtual void Move(float decision) = 0;
	virtual bool Step() = 0;
	virtual void Jump() = 0;
	virtual void JumpUpdate() = 0;
	virtual void DamageCheck() = 0;
	virtual void Guard(float decision) = 0;
	virtual void Squat() = 0;
	virtual void GuardBack() = 0;
	virtual void FallUpdate() = 0;
	virtual void DownUpdate() = 0;
	virtual void PassiveUpdate() = 0;
	virtual void WakeUp() = 0;
	virtual void GaugeUp(float add) = 0;
	virtual void CancelList() = 0;
	virtual void StateNone() = 0;

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

	virtual bool Intro() = 0;
	virtual bool WinPerformance() = 0;

	virtual bool AttackLoad() = 0;
	virtual bool AttackWrite() = 0;

public:
	virtual bool AttackEndCheck() = 0;				//攻撃当たり判定が全て終了しているか確認する
	virtual void EndAttackErase() = 0;				//終了した攻撃当たり判定を全て消去する。
	virtual void AllAttackClear() = 0;				//全ての攻撃当たり判定を消去する
	virtual void AttackUpdate(float elapsed_time) = 0;				//攻撃判定が存在する時のみ更新

};

#endif // !PLAYERBASE_H_

