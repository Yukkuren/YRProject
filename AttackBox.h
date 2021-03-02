#ifndef _ATTACKBOX_H_
#define _ATTACKBOX_H_
#include "YR_VectorMaster.h"
#include "YRShader.h"
#include "Effect.h"

//--------------------------------------------------
// **攻撃パラメーター列挙**
//・攻撃のパラメーターを当たり判定ごとに保存する構造体
//--------------------------------------------------
enum class HitStopTime : int
{
	SHORT=0,
	NORMAL,
	LONG,
	ZOOM,
	END
};

//--------------------------------------------------
// **判定列挙**
//・攻撃がヒット時に被弾したか、それともガードされたかを保存する列挙
//--------------------------------------------------
enum class HitResult : int
{

	NOT_OCCURRENCE = 0,	//攻撃がまだ出ていない
	NONE,				//当たっていない
	GUARD,				//ガードされた
	HIT,				//当たった
	NO_CANCEL,			//キャンセルできない
	END,
};

//--------------------------------------------------
// **攻撃内容列挙**
//・攻撃の内容
//--------------------------------------------------
enum class AttackKind : int
{
	UP = 0,			//上段
	MIDDLE,			//中段
	DOWN,			//下段
	STEAL,			//掴み
	SLAM,			//叩きつけ(高さが一定なら滑り状態にする)
	LOCK,			//ロック技
	PROJECTILE,		//飛び道具
	TRACK,			//ホーミングダッシュ
	NO_TO_OFFSET,	//相殺しない攻撃
	NO_TO_OFFSET_UP,//上段攻撃と相殺しない
	DOWN_ATTACK,	//ダウン攻撃
	END,
};



struct AttackParameter
{
public:
	YR_Vector3	distance;	//プレイヤーの中心座標からどれだけ離れているか
	YR_Vector3	size;		//攻撃判定の大きさ
	float		timer;		//持続フレーム
	float		damege;		//この攻撃で与えるダメージ
	float		HB_timer;	//攻撃を当てた時の相手ののけぞり時間
	YR_Vector3	hitback;	//攻撃を当てた時の相手の吹っ飛びベクトル
	AttackKind	type;		//攻撃の属性(上段・中段・下段)
	float		knockback;	//ノックバック(Xベクトルのみ)
	bool		gaugeout;	//falseならゲージ獲得攻撃
	float		stealtimer; //掴みぬけされる時間
	HitStopTime	HS_timer;	//ヒットストップ時間(3パターン)
	float		gauge_get;	//ゲージの獲得量
};


class AttackBox
{
public:
	//bool				start;		//開始フラグ(最初のみtrue。初期設定終了後はfalse)
	bool				fin;		//終了フラグ(trueで攻撃判定終了)
	bool				attack;		//TRUEなら攻撃判定が出ている
	bool				hit_ok;		//攻撃がでたときのみtrueにして当たったらfalse
	bool				knock_start;//当たったらtrue
	bool				plus;		//trueなら飛び道具。speedにplusを足していく
	float				rightOrleft;//右向きなら*1左向きなら*-1
	HitResult			hit_result;	//攻撃ヒット時の結果を保存する
	int					attack_name;//攻撃番号

	AttackParameter		parameter;	//パラメーター
	YR_Vector3			pos;		//攻撃の実際の中心
	YR_Vector3			speed;		//攻撃の移動速度
	YR_Vector3			plus_speed;	//飛び道具用の常に付与するスピード

	Effekseer::Handle	handle;		//エフェクトのハンドル
	EffectKind			effect_kind;//エフェクトの種類

	AttackBox()
	{
		effect_kind = EffectKind::NONE;

	};
	~AttackBox();

	void Init(int attack_name, AttackParameter& param, float rightOrleft, YR_Vector3 pl_pos);
	void Init(int attack_name, AttackParameter& param, float rightOrleft, YR_Vector3 pl_pos, YR_Vector3 plus_speed);
	void Update(YR_Vector3 pl_pos, float elapsed_time);
	void Draw(YRShader* shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color);			//DEBUG_MODEがtrueの時のみ描画
	void SpeedPlus(YR_Vector3 plus_speed,float elapsed_time);
};
#endif // !_ATTACKBOX_H_


