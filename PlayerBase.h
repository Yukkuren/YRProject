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


#define		POS_Y	(0.0f)

#define		GAUGE_MAX	(100.0f)

const int	Track_max = 2;		//ホーミングダッシュ最大数

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
	WAIT,			//待機(default)
	GUARD,			//ガード(default)
	SQUAT,			//しゃがみ(default)
	MOVER,			//右移動(default)
	MOVEL,			//左移動(default)
	DASH,			//ダッシュ(default)
	BACK,			//バックステップ(default)
	JUMP,			//ジャンプ(default)
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
	NONE,
	SLOW,
	TRACK_DASH,
	STEAL,
	JAKU,
	THU,
	KYO,
	D_JAKU,
	D_THU,
	U_KYO,
	JAKU_RHURF,
	THU_RHURF,
	KYO_RHURF,
	JAKU_LHURF,
	THU_LHURF,
	KYO_LHURF,
	EXTENDATK,
};



//--------------------------------------------------
// **攻撃パラメーター構造体**
//・攻撃のパラメーターを当たり判定ごとに保存する構造体
//--------------------------------------------------
struct AttackParameter
{
public:
	YR_Vector3	center;		//攻撃判定の中心座標
	YR_Vector3	size;		//攻撃判定の大きさ
	float		timer;		//持続フレーム
	float		fream;		//発生フレーム
	float		later;		//後隙フレーム
	float		damege;		//この攻撃で与えるダメージ
	float		HB_timer;	//攻撃を当てた時の相手ののけぞり時間
	YR_Vector3	hitback;	//攻撃を当てた時の相手の吹っ飛びベクトル
	int			type;		//攻撃の属性(上段・中段・下段)
	float		knockback;	//ノックバック(Xベクトルのみ)
	bool		gaugeout;	//falseならゲージ獲得攻撃
	float		stealtimer; //掴みぬけされる時間
};


//--------------------------------------------------
// **攻撃リスト構造体**
//・攻撃のパラメーターをキャラ生成時に読み込み、
//　攻撃を行う際に当たり判定に対応したパラメーターを送る
//--------------------------------------------------
struct AttackList
{
public:
	std::string						attack_name;	//攻撃名
	std::vector<AttackParameter>	parameter;		//内部パラメーター
	int								quantity;		//生成する当たり判定の数量
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
	ActState			act_state;	//今行動可能か。また行動不可ならどういう状態か
	AttackState			attack_state;			//今何をしているか
	float				rightOrleft;	//右向きなら*1左向きなら*-1
	bool				moveflag;		//TRUEなら動いている
	float				later;			//後隙フレーム
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

public:
	//基本処理関数
	virtual void Init(YR_Vector3 InitPos) = 0;
	virtual void LoadData(std::shared_ptr<Texture> texture =nullptr) = 0;
	virtual void Uninit() = 0;
	virtual void Update(float decision,float elapsed_time) = 0;
	virtual void Draw(
		YRShader				*geoshader,
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
};

#endif // !PLAYERBASE_H_

