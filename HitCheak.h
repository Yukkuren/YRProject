#ifndef _HITCHEAK_H_
#define _HIT_CHEAK_H_

#include"HitBox.h"
#include"AttackBox.h"

constexpr float guard_damege_adj = 3.0f;	//ガードされた時のダメージ補正値
constexpr float guard_back_adj_x = 2.5f;	//ガードされた時の押出補正値X
constexpr float guard_back_adj_y = 1.5f;	//ガードされた時の押出補正値Y
constexpr float guard_stop_adj = 2.5f;	//ガードされた時のガード硬直補正値
constexpr float correction_value = 30.0f;	//プレイヤー同士の押出し補正値
constexpr float slam_up_line = 33.0f;		//叩きつけ状態になる高さ
constexpr float slam_zoom_line = 20.0f;		//叩きつけ攻撃を受けてカメラズームになる高さ

enum class HitResultState : int
{
	GUARD_OK = 0,	//ガード成功
	HIT,			//被弾
	AVOIDANCE,		//無敵で回避
	STATE_NONE,		//掴まれた
	SLAM,			//叩きつけ
	ARMOR,			//アーマーが入った
	DOWN_HIT,		//ダウン攻撃がヒット
};

class Hitcheak
{
public:
	static float HitCheak(std::vector<AttackBox>& attack, std::vector<HitBox>& hit, int player,YR_Vector3 pos);
	static void HitCheakAttack(std::vector<AttackBox>& attack1p, std::vector<AttackBox>& attack2p);
	//static void HitCheak2(AttackBox *attack, int a_max, HitBox *hit, int h_max);
	static void HitPlayer(std::vector <HitBox>& pl1, float& pos1, std::vector <HitBox>& pl2, float& pos2,float elapsed_time);
	static float timer;
	static bool hit;
	static bool stop1p;
	static bool stop2p;
	static float add1P;
	static float add2P;
	static bool clash;
	static YR_Vector3 clashpos;
	static YR_Vector3 effectpos;
	static int effecttimer;
	//static int hei;
};
#endif // !_HITCHEAK_H_