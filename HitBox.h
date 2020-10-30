#ifndef _HITBOX_H_
#define _HITBOX_H_

#include "YR_VectorMaster.h"
#include "YRShader.h"


constexpr float hitback_Adjustment = 300.0f;

enum class HitBoxState : int
{
	NOGUARD = 0,	//ガードしない
	MIDDLE,			//中段ガード
	DOWN,			//下段ガード
	INVINCIBLE,		//無敵
	ALL,			//空中ガード(掴み、ガード不可以外を全てガードできる)
	END,
};

struct HitParameter
{
	YR_Vector3	distance = { 0.0f,0.0f };		//プレイヤーの中心座標からどれだけ離れているか
	YR_Vector3	size = { 0.0f,0.0f };			//当たり判定のサイズ
	HitBoxState	state = HitBoxState::NOGUARD;	//現在の当たり判定の状態
};

class HitBox
{
public:
	//攻撃を受けた時に中身が代入される変数
	YR_Vector3	center;			//当たり判定の中心座標
	bool		hit;			//当たっていたらTRUE
	float		damege;			//受けたダメージ
	//bool		hitother;		//相手と当たっていたらtrue
	float		timer;			//攻撃を受けた時ののけぞり時間
	YR_Vector3	hitback;		//攻撃を受けた時の吹っ飛びベクトル
	//int			guard;			//ガードしている場合対応した数値が入る
	bool		guard_ok;		//ガード成功ならtrue
	bool		steal;			//掴まれたらtrue
	float		steal_timer;	//投げ抜け可能時間

	//常に変動する変数
	HitParameter	parameter;		//当たり判定のパラメータ


	void Init();		//全てゼロクリア(プレイヤーの初期位置に当たり判定をつけるためプレイヤーの座標をもらっている)
	void Draw(
		YRShader *shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color);		//DEBUG_MODEがtrueの時のみ描画
	//毎回座標とサイズを受け取って更新
	void Update(YR_Vector3 pl_pos,HitParameter& param,float elapsed_time);
	void Update(YR_Vector3 pl_pos, YR_Vector3& distance, YR_Vector3& size , float elapsed_time);

};

#endif // !_HITBOX_H_
