#ifndef _HITBOX_H_
#define _HITBOX_H_

#include "YR_VECTOR3.h"

class HitBox
{
public:
	YR_Vector3	center;			//当たり判定の中心座標
	YR_Vector3	size;			//当たり判定のサイズ
	bool		hit;			//当たっていたらTRUE
	float		damege;			//受けたダメージ
	//bool		hitother;		//相手と当たっていたらtrue
	int			timer;			//攻撃を受けた時ののけぞり時間
	YR_Vector3	hitback;		//攻撃を受けた時の吹っ飛びベクトル
	int			guard;			//ガードしている場合対応した数値が入る
	bool		guard_ok;		//ガード成功ならtrue
	bool		steal;			//掴まれたらtrue
	int			steal_timer;	//投げ抜け可能時間


	void Init(YR_Vector3 cent, YR_Vector3 range);		//全てゼロクリア(プレイヤーの初期位置に当たり判定をつけるためプレイヤーの座標をもらっている)
	void Draw();								//DEBUG_MODEがtrueの時のみ描画
	void Update(YR_Vector3 cent, YR_Vector3 range);	//毎回座標とサイズを受け取って更新

	enum
	{
		NOGUARD,
		MIDDLE,
		DOWN,
		INVINCIBLE,
		ALL
	};
};

#endif // !_HITBOX_H_
