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


#define		POS_Y	(0.0f)

#define		GAUGE_MAX	(100.0f)


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
	float speedY = 0;

	void Set(float speed)
	{
		speedY = speed;
	};
	float Update()
	{
		if (speedY > 0)
		{
			speedY--;
		}
		else
		{
			speedY = 0;
		}
		return speedY;
	};
};


class Player
{
public:
	//ゲーム内処理用変数
	float		hp = 0;
	int			jumpcount;
	bool		jumpflag;
	bool		max_jump_flag;
	bool		ground;			//TRUEなら地面についている
	bool		drawset;		//特定の画像を描画したときにtrueにして通常の画像を描画しない
	bool		attack;			//TRUEなら攻撃中
	int			state;			//今何をしているか(enum)
	float		rightOrleft;	//右向きなら*1左向きなら*-1
	bool		moveflag;		//TRUEなら動いている
	int			later;			//後隙フレーム
	int			knocktimer;		//喰らい時間
	float		gravity;		//重力値
	int			specialfream;	//弱コンボ等でフレームを減らす際の数値
	bool		finish;			//行動が終了したときのみtrue
	bool		step;
	bool		hightrigger;	//ハイジャンプ時true、通常時false
	int			trackgauge;		//追尾ダッシュの残り回数
	float		gauge;			//ゲージ
	int			power;			//ゲージが最大になると1増える。特殊技はこっちを消費する
	int			rival_state;	//相手のステート
	int			steal_escape;	//投げ抜け可能時間
	int			combo_count;	//コンボカウント

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
	virtual void LoadData() = 0;
	virtual void Uninit() = 0;
	virtual void Update(float decision) = 0;
	virtual void Draw(
		YRShader				*shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time ) = 0;

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
	virtual void TrackDash(float decision) = 0;
};

#endif // !PLAYERBASE_H_

