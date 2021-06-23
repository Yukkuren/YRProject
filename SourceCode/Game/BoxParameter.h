#pragma once

#include "../YR_VectorMaster.h"

//----------------------------------------------------------------
//	BoxParameter
//----------------------------------------------------------------
//・HitBox、AttackBox両方で使用する共通のパラメーターを保持する構造体
//・それぞれのクラスに変数として持たせる
//----------------------------------------------------------------

struct BoxParameter
{
	float		damage;			//与える(受ける)ダメージ
	float		HB_timer;		//攻撃を当てた(受けた)時ののけぞり時間
	YR_Vector3	hitback;		//攻撃を当てた時の相手の吹っ飛びベクトル
	float		steal_timer;	//掴みぬけされる時間
	float		guard_shaving;	//ガード削り値
	YR_Vector3	guard_back;		//ガードされた(した)時の吹っ飛びベクトル
	float		guard_timer;	//ガードされた(した)時のガード硬直時間

	BoxParameter() :damage(0.0f), HB_timer(0.0f), hitback(0.0f, 0.0f, 0.0f),
		guard_shaving(0.0f), steal_timer(0.0f), guard_back(0.0f, 0.0f, 0.0f), guard_timer(0.1f) {};

	void Reset()
	{
		damage = 0.0f;
		//HB_timer = 0.0f;
		hitback = YR_Vector3(0.0f, 0.0f, 0.0f);
		steal_timer = 0.0f;
		guard_shaving = 0.0f;
		guard_back = YR_Vector3(0.0f, 0.0f, 0.0f);
		guard_timer = 0.0f;
	}
};
