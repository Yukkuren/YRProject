#include "Knight.h"
#include "PlayerBase.h"
#include "Key.h"
#include "YRGamePad.h"

//**************************************************************************************************************
//				UŒ‚ŠÖ”




void Knight::Slow(float elapsed_time)
{
	/*YR_Vector3 cent{ pos.x + Getapply(100.0f),pos.y };
	YR_Vector3 range{ 200.0f,50.0f };
	atk[scastI(KNIGHTATK::ONE)].Update(
		cent, range, 5, 5, 15, 20, 40, YR_Vector3(Getapply(10.0f), 30.0f), AttackBox::MIDDLE, Getapply(0.0f),elapsed_time);*/
}



void Knight::Jaku(float elapsed_time)
{
	if (later > -1 && later < target_max)
	{
		return;
	}

	if (fream < 100.0f)
	{
		fream -= elapsed_time;
	}
	

	if (fream < 0.0f)
	{
		attack_list[scastI(attack_state)].SetAttack(&atk, rightOrleft);
		fream = 110.0f;
	}

	int now_at_list = scastI(attack_state);

	bool knock = false;	//ˆê“x‚Å‚àknock_start‚É“ü‚Á‚½‚çŽc‚è‚Ì“–‚½‚è”»’è‚Ìknockback‚ð‘S‚Ä0.0f‚É‚·‚é
	if (!atk.empty())
	{
		for (auto& a : atk)
		{
			if (knock)
			{
				a.parameter.knockback = 0.0f;
			}
			if (a.knock_start)
			{
				pos.x -= a.parameter.knockback;
				a.parameter.knockback = 0.0f;
				if (!ground)
				{
					speed_Y.Set(60.0f);
				}
				knock = true;
			}
		}
	}

	if (AttackEndCheck())
	{
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[scastI(attack_state)].attack_single[attack_list[now_at_list].now_attack_num].fream;
		}
		else
		{
			later = attack_list[now_at_list].later;
		}
	}

	specialfream = 0;
	/*if (atk[scastI(KNIGHTATK::ONE)].knock_start)
	{
		pos.x -= atk[scastI(KNIGHTATK::ONE)].knockback;
		atk[scastI(KNIGHTATK::ONE)].knockback = 0.0f;;
		if (!ground)
		{
			speed_Y.Set(60.0f);
		}
	}
	YR_Vector3 cent{ pos.x + Getapply(100.0f),pos.y };
	YR_Vector3 range{ 50.0f,50.0f };
	if (ground)
	{
		atk[scastI(KNIGHTATK::ONE)].Update(
			cent, range, 5, 5, 15, 10, 15, YR_Vector3(Getapply(5.0f), 0.0f), AttackBox::MIDDLE, Getapply(10.0f),elapsed_time);
	}
	else
	{
		atk[scastI(KNIGHTATK::ONE)].Update(
			cent, range, 5, 5, 15, 10, 20, YR_Vector3(Getapply(5.0f), 20.0f), AttackBox::MIDDLE, Getapply(10.0f),elapsed_time);
	}
	specialfream = 0;*/
}

void Knight::Thu(float fream, float elapsed_time)
{
	if (atk[scastI(KNIGHTATK::ONE)].knock_start)
	{
		pos.x -= atk[scastI(KNIGHTATK::ONE)].knockback;
		atk[scastI(KNIGHTATK::ONE)].knockback = 0.0f;;
		if (!ground)
		{
			speed_Y.Set(60.0f);
		}
	}
	YR_Vector3 cent{ pos.x + Getapply(120.0f),pos.y };
	YR_Vector3 range{ 100.0f,200.0f };
	if (fream != 0)
	{
		if (ground)
		{
			atk[scastI(KNIGHTATK::ONE)].Update(
				cent, range, 3, fream, 20, 30, 40, YR_Vector3(Getapply(20.0f), 0.0f), AttackBox::MIDDLE, Getapply(30.0f),elapsed_time);
		}
		else
		{
			atk[scastI(KNIGHTATK::ONE)].Update(
				cent, range, 3, fream, 20, 30, 40, YR_Vector3(Getapply(20.0f), 30.0f), AttackBox::MIDDLE, Getapply(30.0f),elapsed_time);
		}
	}
	else
	{
		if (ground)
		{
			atk[scastI(KNIGHTATK::ONE)].Update(
				cent, range, 3, 20, 20, 30, 40, YR_Vector3(Getapply(20.0f), 0.0f), AttackBox::MIDDLE, Getapply(30.0f),elapsed_time);
		}
		else
		{
			atk[scastI(KNIGHTATK::ONE)].Update(
				cent, range, 3, 20, 20, 30, 40, YR_Vector3(Getapply(20.0f), 30.0f), AttackBox::MIDDLE, Getapply(30.0f),elapsed_time);
		}
	}
	specialfream = 0;
}

void Knight::Kyo(float fream, float elapsed_time)
{
	if (atk[scastI(KNIGHTATK::ONE)].knock_start)
	{
		pos.x -= atk[scastI(KNIGHTATK::ONE)].knockback;
		atk[scastI(KNIGHTATK::ONE)].knockback = 0.0f;;
	}
	YR_Vector3 cent{ pos.x + Getapply(300.0f),pos.y };
	YR_Vector3 range{ 150.0f,150.0f };
	if (specialfream != 0)
	{
		atk[scastI(KNIGHTATK::ONE)].Update(
			cent, range, 5, specialfream, 30, 50, 50, YR_Vector3(Getapply(20.0f), 35.0f), AttackBox::MIDDLE, Getapply(50.0f),elapsed_time);
		cent = { pos.x + Getapply(100.0f),pos.y };
		range = { 100.0f,50.0f };
		atk[scastI(KNIGHTATK::TWO)].Update(
			cent, range, 5, specialfream, 30, 50, 50, YR_Vector3(Getapply(20.0f), 35.0f), AttackBox::MIDDLE, Getapply(50.0f),elapsed_time);
	}
	else
	{
		atk[scastI(KNIGHTATK::ONE)].Update(
			cent, range, 5, 25, 20, 50, 50, YR_Vector3(Getapply(20.0f), 35.0f), AttackBox::MIDDLE, Getapply(50.0f),elapsed_time);
		cent = { pos.x + Getapply(100.0f),pos.y };
		range = { 100.0f,50.0f };
		atk[scastI(KNIGHTATK::TWO)].Update(
			cent, range, 5, 25, 20, 50, 50, YR_Vector3(Getapply(20.0f), 35.0f), AttackBox::MIDDLE, Getapply(50.0f),elapsed_time);
	}
	specialfream = 0;
}


void Knight::D_Jaku(float elapsed_time)
{
	Hitplus[scastI(KNIGHTHIT::BODY)] = YR_Vector3(0.0f, 15.0f);
	hit[scastI(KNIGHTHIT::BODY)].size = YR_Vector3(65.0f, 130.0f);
	Hitplus[scastI(KNIGHTHIT::LEG)] = YR_Vector3(8.0f, 163.0f);
	hit[scastI(KNIGHTHIT::LEG)].size = YR_Vector3(86.0f, 40.0f);
	if (atk[scastI(KNIGHTATK::ONE)].knock_start)
	{
		pos.x -= atk[scastI(KNIGHTATK::ONE)].knockback;
		atk[scastI(KNIGHTATK::ONE)].knockback = 0.0f;;
	}
	YR_Vector3 cent{ pos.x + Getapply(60.0f),pos.y + 150.0f };
	YR_Vector3 range{ 50.0f,50.0f };
	atk[scastI(KNIGHTATK::ONE)].Update(
		cent, range, 2, 2, 2, 5, 15, YR_Vector3(Getapply(3.0f), 0.0f), AttackBox::DOWN, Getapply(5.0f),elapsed_time);
	specialfream = 0;
}


void Knight::D_Thu(float fream, float elapsed_time)
{
	Hitplus[scastI(KNIGHTHIT::BODY)] = YR_Vector3(-31.0f, 134.0f);
	hit[scastI(KNIGHTHIT::BODY)].size = YR_Vector3(110.0f, 62.0f);
	Hitplus[scastI(KNIGHTHIT::LEG)] = YR_Vector3(110.0f, 149.0f);
	hit[scastI(KNIGHTHIT::LEG)].size = YR_Vector3(39.0f, 47.0f);
	if (atk[scastI(KNIGHTATK::ONE)].knock_start)
	{
		pos.x -= atk[scastI(KNIGHTATK::ONE)].knockback;
		atk[scastI(KNIGHTATK::ONE)].knockback = 0.0f;;
	}
	YR_Vector3 cent{ pos.x + Getapply(100.0f),pos.y + 150.0f };
	YR_Vector3 range{ 100.0f,50.0f };
	if (fream != 0)
	{//3
		atk[scastI(KNIGHTATK::ONE)].Update(
			cent, range, 3, fream, 35, 30, 30, YR_Vector3(Getapply(10.0f), 10.0f), AttackBox::DOWN, Getapply(30.0f),elapsed_time);
	}
	else
	{
		atk[scastI(KNIGHTATK::ONE)].Update(
			cent, range, 3, 6, 35, 30, 30, YR_Vector3(Getapply(10.0f), 10.0f), AttackBox::DOWN, Getapply(30.0f),elapsed_time);
	}
	if (later < 0)
	{
		pos.x += Getapply(20.0f);
	}
	specialfream = 0;
}

void Knight::U_Kyo(float fream, float elapsed_time)
{
	if (atk[scastI(KNIGHTATK::ONE)].knock_start)
	{
		pos.x -= atk[scastI(KNIGHTATK::ONE)].knockback;
		atk[scastI(KNIGHTATK::ONE)].knockback = 0.0f;;
	}
	YR_Vector3 cent{ pos.x + Getapply(100.0f),pos.y - 100.0f };
	YR_Vector3 range{ 150.0f,150.0f };
	if (specialfream != 0)
	{
		if (ground)
		{
			atk[scastI(KNIGHTATK::ONE)].Update(
				cent, range, 5, specialfream, 30, 50, 30, YR_Vector3(Getapply(50.0f), 10.0f), AttackBox::UP, Getapply(50.0f),elapsed_time);
		}
		else
		{
			cent = { pos.x + Getapply(100.0f),pos.y + 50.0f };
			atk[scastI(KNIGHTATK::ONE)].Update(
				cent, range, 5, specialfream, 30, 50, 30, YR_Vector3(Getapply(50.0f), -25.0f), AttackBox::UP, Getapply(50.0f),elapsed_time);
		}
	}
	else
	{
		if (ground)
		{
			atk[scastI(KNIGHTATK::ONE)].Update(
				cent, range, 5, 20, 30, 50, 30, YR_Vector3(Getapply(50.0f), 10.0f), AttackBox::UP, Getapply(50.0f),elapsed_time);
		}
		else
		{
			cent = { pos.x + Getapply(100.0f),pos.y + 50.0f };
			atk[scastI(KNIGHTATK::ONE)].Update(
				cent, range, 5, 20, 30, 50, 30, YR_Vector3(Getapply(50.0f), -25.0f), AttackBox::UP, Getapply(50.0f),elapsed_time);
		}
	}
	specialfream = 0;
}

void Knight::P_Kyo(float elapsed_time)
{
	if (atk[scastI(KNIGHTATK::ONE)].knock_start)
	{
		pos.x -= atk[scastI(KNIGHTATK::ONE)].knockback;
		atk[scastI(KNIGHTATK::ONE)].knockback = 0.0f;;
	}
	pos.y -= gravity;
	YR_Vector3 cent{ pos.x + Getapply(100.0f),pos.y - 100.0f };
	YR_Vector3 range{ 150.0f,150.0f };
	atk[scastI(KNIGHTATK::ONE)].Update(
		cent, range, 5, 10, 20, 80, 50, YR_Vector3(Getapply(50.0f), 40.0f), AttackBox::UP, Getapply(50.0f), 0,elapsed_time);
}


void Knight::Hadouken(float elapsed_time)
{
	YR_Vector3 cent{ pos.x ,pos.y };
	YR_Vector3 range{ 0.1f,0.1f };
	atk[scastI(KNIGHTATK::ONE)].Update(
		cent, range, 1, 15, 15, 1, 0, YR_Vector3(Getapply(0.0f), 0.0f), AttackBox::MIDDLE, Getapply(0.0f),elapsed_time);
	if (atk[scastI(KNIGHTATK::ONE)].hit_ok)
	{
		atk[scastI(KNIGHTATK::ONE)].damege = 0;
		if (!atk[scastI(KNIGHTATK::HADOU)].hit_ok)
		{
			hadou.x = pos.x + Getapply(100);
			hadou.y = pos.y;
			atk[scastI(KNIGHTATK::HADOU)].Init();
			atk[scastI(KNIGHTATK::HADOU)].hit_ok = true;
			hadouspeed = Getapply(30);
		}
	}
	specialfream = 0;
}


void Knight::Thu_Hadouken(float elapsed_time)
{
	YR_Vector3 cent{ pos.x ,pos.y };
	YR_Vector3 range{ 0.1f,0.1f };
	atk[scastI(KNIGHTATK::ONE)].Update(
		cent, range, 1, 15, 15, 1, 0, YR_Vector3(Getapply(0.0f), 0.0f), AttackBox::MIDDLE, Getapply(0.0f),elapsed_time);
	if (atk[scastI(KNIGHTATK::ONE)].hit_ok)
	{
		atk[scastI(KNIGHTATK::ONE)].damege = 0;
		if (!atk[scastI(KNIGHTATK::HADOU)].hit_ok)
		{
			hadou.x = pos.x + Getapply(100);
			hadou.y = pos.y;
			atk[scastI(KNIGHTATK::THU_HADOU)].Init();
			atk[scastI(KNIGHTATK::THU_HADOU)].hit_ok = true;
			hadouspeed = Getapply(50);
		}
	}
	specialfream = 0;
}



void Knight::Kyo_Hadouken(float elapsed_time)
{
	YR_Vector3 cent{ pos.x ,pos.y };
	YR_Vector3 range{ 0.1f,0.1f };
	atk[scastI(KNIGHTATK::ONE)].Update(
		cent, range, 1, 15, 15, 1, 0, YR_Vector3(Getapply(0.0f), 0.0f), AttackBox::MIDDLE, Getapply(0.0f),elapsed_time);
	if (atk[scastI(KNIGHTATK::ONE)].hit_ok)
	{
		atk[scastI(KNIGHTATK::ONE)].damege = 0;
		hadou.x = pos.x + Getapply(100);
		hadou.y = pos.y;
		atk[scastI(KNIGHTATK::KYO_HADOU)].Init();
		atk[scastI(KNIGHTATK::KYO_HADOU)].hit_ok = true;
		hadouspeed = Getapply(60);
	}
	specialfream = 0;
}




void Knight::HadouUpdate(float elapsed_time)
{
	if (!atk[scastI(KNIGHTATK::HADOU)].hit_ok)
	{
		return;
	}

	YR_Vector3 cent{ hadou.x,hadou.y };
	YR_Vector3 range{ 50.0f,50.0f };

	atk[scastI(KNIGHTATK::HADOU)].UpdateMissile(
		cent, range, 60, 15, 20, YR_Vector3(Getapply(5.0f), 20.0f), AttackBox::MIDDLE,elapsed_time);
	hadou.x += hadouspeed;
}

void Knight::Thu_HadouUpdate(float elapsed_time)
{
	if (!atk[scastI(KNIGHTATK::THU_HADOU)].hit_ok)
	{
		return;
	}

	YR_Vector3 cent{ hadou.x,hadou.y };
	YR_Vector3 range{ 50.0f,50.0f };

	atk[scastI(KNIGHTATK::THU_HADOU)].UpdateMissile(
		cent, range, 60, 20, 5, YR_Vector3(Getapply(5.0f), 0.0f), AttackBox::MIDDLE,elapsed_time);
	hadou.x += hadouspeed;
}


void Knight::Kyo_HadouUpdate(float elapsed_time)
{
	if (!atk[scastI(KNIGHTATK::KYO_HADOU)].hit_ok)
	{
		return;
	}

	YR_Vector3 cent{ hadou.x,hadou.y };
	YR_Vector3 range{ 150.0f,150.0f };

	atk[scastI(KNIGHTATK::KYO_HADOU)].UpdateMissile(
		cent, range, 60, 30, 30, YR_Vector3(Getapply(20.0f), 30.0f), AttackBox::MIDDLE, 0,elapsed_time);
	hadou.x += hadouspeed;
}





void Knight::Panish_N(float elapsed_time)
{
	/*if (atk[scastI(RYUATK::ONE)].knock_start)
	{
		pos.x -= atk[scastI(RYUATK::ONE)].knockback;
		atk[scastI(RYUATK::ONE)].knockback = 0.0f;;
		if (!ground)
		{
			speed_Y.Set(60.0f);
		}
	}*/
	YR_Vector3 cent{ pos.x ,pos.y };
	YR_Vector3 range{ 1.0f,1.0f };
	atk[scastI(KNIGHTATK::ONE)].Update(
		cent, range, 5, 25, 5, 0, 0, YR_Vector3(Getapply(0.0f), 0.0f), AttackBox::MIDDLE, Getapply(0.0f),elapsed_time);

	if (atk[scastI(KNIGHTATK::ONE)].fream == 0)
	{
		FastSet(pos);
		pos.x += Getapply(400.0f);
		pos.y -= 500.0f;
	}
	pos.y -= gravity * 0.75f;
	specialfream = 0;
}



void Knight::Panish_H(float elapsed_time)
{
	YR_Vector3 cent{ pos.x ,pos.y };
	YR_Vector3 range{ 1.0f,1.0f };
	atk[scastI(KNIGHTATK::ONE)].Update(
		cent, range, 5, 35, 5, 0, 0, YR_Vector3(Getapply(0.0f), 0.0f), AttackBox::MIDDLE, Getapply(0.0f),elapsed_time);

	if (atk[scastI(KNIGHTATK::ONE)].fream == 0)
	{
		FastSet(pos);
		pos.x = tracking.rival_Pos.x - Getapply(150.0f);
		pos.y = tracking.rival_Pos.y - 400.0f;
	}

	pos.y -= gravity * 0.75f;

	specialfream = 0;
}


void Knight::ExtendATK(float elapsed_time)
{
	if (atk[scastI(KNIGHTATK::ONE)].knock_start)
	{
		pos.x -= atk[scastI(KNIGHTATK::ONE)].knockback;
		atk[scastI(KNIGHTATK::ONE)].knockback = 0.0f;;
	}
	YR_Vector3 cent{ pos.x + Getapply(100.0f),pos.y - 50 };
	YR_Vector3 range{ 50.0f,200.0f };
	if (ground)
	{
		atk[scastI(KNIGHTATK::ONE)].Update(
			cent, range, 5, 3, 80, 10, 50, YR_Vector3(Getapply(30.0f), 30.0f), AttackBox::MIDDLE, Getapply(0.0f),elapsed_time);
	}
	else
	{
		atk[scastI(KNIGHTATK::ONE)].Update(
			cent, range, 5, 3, 80, 10, 50, YR_Vector3(Getapply(30.0f), 30.0f), AttackBox::MIDDLE, Getapply(0.0f),elapsed_time);
	}
	if (atk[scastI(KNIGHTATK::ONE)].timer > 0)
	{
		for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
		{
			hit[i].guard = HitBox::INVINCIBLE;
		}
	}

	specialfream = 0;
}


//
//***************************************************************************************************************