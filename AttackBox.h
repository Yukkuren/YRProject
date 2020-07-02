#ifndef _ATTACKBOX_H_
#define _ATTACKBOX_H_
#include "YR_VECTOR3.h"
#include "YRShader.h"

class AttackBox
{
public:
	YR_Vector3	center;		//攻撃判定の中心座標
	YR_Vector3	size;		//攻撃判定の大きさ
	float		timer;		//持続フレーム
	bool		start;		//開始フラグ(最初のみtrue。初期設定終了後はfalse)
	bool		fin;		//終了フラグ(trueで攻撃判定終了)
	float		fream;		//発生フレーム
	float		later;		//後隙フレーム
	bool		attack;		//TRUEなら攻撃判定が出ている
	bool		hit_ok;		//攻撃がでたときのみtrueにして当たったらfalse
	float		damege;		//この攻撃で与えるダメージ
	float		HB_timer;	//攻撃を当てた時の相手ののけぞり時間
	YR_Vector3	hitback;	//攻撃を当てた時の相手の吹っ飛びベクトル
	int			type;		//攻撃の属性(上段・中段・下段)
	float		knockback;	//ノックバック(Xベクトルのみ)
	bool		knock_start;//当たったらtrue
	bool		gaugeout;	//falseならゲージ獲得攻撃
	float		stealtimer; //掴みぬけされる時間

	void Init();			//全てfalse&ゼロクリア
	void Draw(YRShader* shader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color);			//DEBUG_MODEがtrueの時のみ描画
	//位置、大きさ、持続、発生、後隙、威力、のけぞり時間、吹っ飛ばしベクトル、段、ノックバック
	void Update(YR_Vector3 cent, YR_Vector3 range, float time, float frm, float late, float dage, int hitbackT, YR_Vector3 divhitback, int kind, float knock,float elapsed_time);
	//Updateに毎回値を入れることによって座標を動かしている。timerやlate等は攻撃発生時のみ値が代入される
	//位置、大きさ、持続、威力、のけぞり時間、吹っ飛ばしベクトル、段
	void UpdateMissile(YR_Vector3 cent, YR_Vector3 range, float time, float dage, int hitbackT, YR_Vector3 divhitback, int kind, float elapsed_time);

	//位置、大きさ、持続、威力、のけぞり時間、吹っ飛ばしベクトル、段、ゲージを獲得しない
	void UpdateMissile(YR_Vector3 cent, YR_Vector3 range, float time, float dage, int hitbackT, YR_Vector3 divhitback, int kind, int gauge, float elapsed_time);

	//位置、大きさ、持続、発生、後隙、威力、のけぞり時間、吹っ飛ばしベクトル、段、ノックバック,攻撃を継続する
	bool Update(YR_Vector3 cent, YR_Vector3 range, float time, float frm, float late, float dage, int hitbackT, YR_Vector3 divhitback, int kind, float knock, bool fin, float elapsed_time);


	//位置、大きさ、持続、威力、のけぞり時間、吹っ飛ばしベクトル、段、ゲージを獲得しない
	void Update(YR_Vector3 cent, YR_Vector3 range, float time, float frm, float late, float dage, int hitbackT, YR_Vector3 divhitback, int kind, float knock, int gauge, float elapsed_time);

	enum
	{
		UP,
		MIDDLE,
		DOWN,
		STEAL,
	};
};
#endif // !_ATTACKBOX_H_


