#ifndef _ATTACKBOX_H_
#define _ATTACKBOX_H_
#include "../YR_VectorMaster.h"
#include "../YRShader.h"
#include "../Effect.h"

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

	AttackParameter() : distance(0.0f, 0.0f, 0.0f), size(0.0f, 0.0f, 0.0f), timer(0.0f), damege(0.0f), HB_timer(0.0f), hitback(0.0f, 0.0f, 0.0f),
		type(AttackKind::MIDDLE), knockback(0.0f), gaugeout(false), stealtimer(0.0f), HS_timer(HitStopTime::SHORT), gauge_get(0.0f) {};
};


struct EffectParameter
{
	EffectKind			effect_kind;	//エフェクトの種類
	YR_Vector3			distance;		//攻撃判定との座標の位置の差
	YR_Vector3			scale;			//大きさ
	YR_Vector3			axis;			//角度の方向
	float				angle;			//角度
	bool				rightORleft;	//角度をプレイヤーに依存させるか

	EffectParameter() : effect_kind(EffectKind::NONE), distance(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f), axis(0.0f, 0.0f, 0.0f),
	angle(0.0f),rightORleft(false){}
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

	EffectParameter		effect_param;//エフェクトのパラメーター
	Effekseer::Handle	handle;		//エフェクトのハンドル

	AttackBox() : fin(false), attack(false), hit_ok(true), knock_start(false), plus(false), rightOrleft(0.0f), hit_result(HitResult::NONE), attack_name(0),
		parameter(), pos(0.0f, 0.0f, 0.0f), speed(0.0f, 0.0f, 0.0f), plus_speed(0.0f, 0.0f, 0.0f), effect_param(), handle() {};
	~AttackBox();

	void Init(int attack_name, AttackParameter& param, float rightOrleft, YR_Vector3 pl_pos, EffectParameter effect_param);
	void Init(int attack_name, AttackParameter& param, float rightOrleft, YR_Vector3 pl_pos, YR_Vector3 plus_speed, EffectParameter effect_param);
	void Update(YR_Vector3 pl_pos, float elapsed_time, YR_Vector3 effect_pos = YR_Vector3(0.0f, 0.0f));
	void Draw(YRShader* shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color);			//DEBUG_MODEがtrueの時のみ描画
	void SpeedPlus(YR_Vector3 plus_speed,float elapsed_time);
};
#endif // !_ATTACKBOX_H_


