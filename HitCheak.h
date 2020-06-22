#ifndef _HITCHEAK_H_
#define _HIT_CHEAK_H_

#include"HitBox.h"
#include"AttackBox.h"

class Hitcheak
{
public:
	static float HitCheak(AttackBox* attack, int a_max, HitBox* hit, int h_max, int player);
	static void HitCheakAttack(AttackBox* attack1p, int a_max1p, AttackBox* attack2p, int a_max2p);
	//static void HitCheak2(AttackBox *attack, int a_max, HitBox *hit, int h_max);
	static void HitPlayer(HitBox* pl1, float& pos1, HitBox* pl2, float& pos2);
	static int timer;
	static bool hit;
	static bool stop1p;
	static bool stop2p;
	static float add1P;
	static float add2P;
	static bool clash;
	static YR_Vector3 clashpos;
	static YR_Vector3 effectpos;
	static int effecttimer;
};
#endif // !_HITCHEAK_H_