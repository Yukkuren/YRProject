#include "HitCheak.h"
#include <stdlib.h>
#include <time.h>
#include "YR_VECTOR3.h"
//#include "Music.h"

float Hitcheak::HitCheak(AttackBox* attack, int a_max, HitBox* hit, int h_max, int player)
{
	for (int atknum = 0; atknum < a_max; atknum++)
	{
		if (attack[atknum].hit_ok)
		{
			for (int hitnum = 0; hitnum < h_max; hitnum++)
			{
				if (attack[atknum].center.x - attack[atknum].size.x<hit[hitnum].center.x + hit[hitnum].size.x && attack[atknum].center.x + attack[atknum].size.x>hit[hitnum].center.x - hit[hitnum].size.x)
				{
					if (attack[atknum].center.y - attack[atknum].size.y<hit[hitnum].center.y + hit[hitnum].size.y && attack[atknum].center.y + attack[atknum].size.y>hit[hitnum].center.y - hit[hitnum].size.y)
					{
						int flag = 0;

						switch (attack[atknum].type)
						{
						case AttackBox::UP:
							if (hit[hitnum].guard == HitBox::NOGUARD)
							{
								flag = 1;
							}
							if (hit[hitnum].guard == HitBox::DOWN)
							{
								flag = 1;
							}
							if (hit[hitnum].guard == HitBox::INVINCIBLE)
							{
								flag = 2;
							}
							break;
						case AttackBox::MIDDLE:
							if (hit[hitnum].guard == HitBox::NOGUARD)
							{
								flag = 1;
							}
							if (hit[hitnum].guard == HitBox::INVINCIBLE)
							{
								flag = 2;
							}
							break;
						case AttackBox::DOWN:
							if (hit[hitnum].guard == HitBox::NOGUARD)
							{
								flag = 1;
							}
							if (hit[hitnum].guard == HitBox::MIDDLE)
							{
								flag = 1;
							}
							if (hit[hitnum].guard == HitBox::INVINCIBLE)
							{
								flag = 2;
							}
							break;
						case AttackBox::STEAL:
							if (hit[hitnum].guard == HitBox::NOGUARD)
							{
								flag = 3;
							}
							if (hit[hitnum].guard == HitBox::MIDDLE)
							{
								flag = 3;
							}
							if (hit[hitnum].guard == HitBox::DOWN)
							{
								flag = 3;
							}
							if (hit[hitnum].guard == HitBox::INVINCIBLE)
							{
								flag = 2;
							}
							break;
						}



						if (flag == 1)
						{

							float add = 0.0f;
							if (!attack[atknum].gaugeout)
							{
								add = attack[atknum].damege;
							}
							hit[hitnum].hit = true;
							attack[atknum].hit_ok = false;
							attack[atknum].knock_start = true;
							hit[hitnum].damege = attack[atknum].damege;
							hit[hitnum].timer = attack[atknum].HB_timer;
							hit[hitnum].hitback = attack[atknum].hitback;
							Hitcheak::timer = (attack[atknum].damege / 4);
							if (Hitcheak::timer > 40)
							{
								Hitcheak::timer = 40;
							}
							if (player == 1)
							{
								Hitcheak::stop1p = true;
							}
							if (player == 2)
							{
								Hitcheak::stop2p = true;
							}

							if (attack[atknum].center.x >= hit[atknum].center.x)
							{
								float dis = attack[atknum].center.x - hit[hitnum].center.x;
								dis /= 2;
								effectpos.x = hit[hitnum].center.x + dis;
								effecttimer = 10;
							}
							if (hit[hitnum].center.x > attack[atknum].center.x)
							{
								float dis = hit[hitnum].center.x - attack[atknum].center.x;
								dis /= 2;
								effectpos.x = attack[atknum].center.x + dis;
								effecttimer = 10;
							}

							if (attack[atknum].center.y >= hit[hitnum].center.y)
							{
								float dis = attack[atknum].center.y - hit[hitnum].center.y;
								dis /= 2;
								effectpos.y = hit[hitnum].center.y + dis;
								effecttimer = 10;
							}
							if (hit[hitnum].center.y > attack[atknum].center.y)
							{
								float dis = hit[hitnum].center.y - attack[atknum].center.y;
								dis /= 2;
								effectpos.y = attack[atknum].center.y + dis;
								effecttimer = 10;
							}

							for (int n = 0; n < a_max; n++)
							{
								attack[n].damege = 0;
								//attack[n].HB_timer = 0;
								attack[n].hitback = YR_Vector3(0.0f, 0.0f);
								attack[n].hit_ok = false;
							}
							//PlaySE(SE_HIT);
							return add;
						}
						if (flag == 0)
						{
							float add = 0.0f;
							if (!attack[atknum].gaugeout)
							{
								add = attack[atknum].damege / 3.0f;
							}
							hit[hitnum].guard_ok = true;
							attack[atknum].hit_ok = false;
							attack[atknum].knock_start = true;
							hit[hitnum].damege = attack[atknum].damege / 2;
							hit[hitnum].timer = attack[atknum].HB_timer;
							hit[hitnum].hitback.x = attack[atknum].hitback.x / 2;
							hit[hitnum].hitback.y = attack[atknum].hitback.y / 2;
							for (int n = 0; n < a_max; n++)
							{
								attack[n].damege = 0;
								//attack[n].HB_timer = 0;
								attack[n].hitback = YR_Vector3(0.0f, 0.0f);
								attack[n].hit_ok = false;
							}
							return add;
						}
						if (flag == 2)
						{
							return 0.0f;
						}

						if (flag == 3)
						{
							hit[hitnum].steal = true;
							hit[hitnum].steal_timer = 20;
							return 0.0f;
						}
					}
				}
			}
		}
	}
	return 0.0f;
}

//void Hitcheak::HitCheak2(AttackBox *attack, int a_max, HitBox *hit, int h_max)
//{
//	for (int i = 0; i < a_max; i++)
//	{
//		if (attack[i].hit_ok)
//		{
//			for (int j = 0; j < h_max; j++)
//			{
//				if (attack[i].center.x + attack[i].size.x<hit[j].center.x + hit[j].size.x&&attack[i].center.x - attack[i].size.x>hit[j].center.x - hit[j].size.x)
//				{
//					if (attack[i].center.y - attack[i].size.y<hit[j].center.y + hit[j].size.y&&attack[i].center.y + attack[i].size.y>hit[j].center.y - hit[j].size.y)
//					{
//						hit[j].hit = true;
//						attack[i].hit_ok = false;
//						hit[j].damege = attack[i].damege;
//						hit[j].timer = 20;
//						attack[i].damege = 0;
//						for (int n = 0; n < a_max; n++)
//						{
//							attack[n].damege = 0;
//						}
//					}
//				}
//			}
//		}
//	}
//}

void Hitcheak::HitPlayer(HitBox* pl1, float& pos1, HitBox* pl2, float& pos2)
{
	if (pl1[0].center.x - pl1[0].size.x<pl2[0].center.x + pl2[0].size.x && pl1[0].center.x + pl1[0].size.x>pl2[0].center.x - pl2[0].size.x)
	{
		if (pl1[0].center.y - pl1[0].size.y<pl2[0].center.y + pl2[0].size.y && pl1[0].center.y + pl1[0].size.y>pl2[0].center.y - pl2[0].size.y)
		{
			//pl1[0].hitother = true;
			//pl2[0].hitother = true;
			float moving_correction = (pl1[0].size.x / 2) + (pl2[0].size.x / 2);
			if (pos1 < pos2)
			{
				//while (1)
				//{
				//	float distance = pos2 - pos1;
				//	float p1pos = pos1;
				//	float p2pos = pos2;
				//	float p1dist = distance - (pl1[0].size.x / 2);
				//	float p2dist = distance - (pl2[0].size.x / 2);
				//	float suns = distance - (p1dist + p2dist);
				//	pos1=p1pos- suns;
				//	pos2=p2pos+ suns;
				//	if (distance > moving_correction)
				//	{
				//		//break;
				//	}
				//}
				while (1)
				{
					float distance = pos2 - pos1;
					pos1 -= 10;
					pos2 += 10;
					if (distance > moving_correction)
					{
						break;
					}
				}
			}
			if (pos1 > pos2)
			{
				while (1)
				{
					float distance = pos1 - pos2;
					pos1 += 10;
					pos2 -= 10;
					if (distance > moving_correction)
					{
						break;
					}
				}
			}
			if (pos1 == pos2)
			{

				int ra = 0;
				ra = rand() % 2;
				if (ra)
				{
					while (1)
					{
						float distance = pos2 - pos1;
						pos1 -= 10;
						pos2 += 10;
						if (distance > moving_correction)
						{
							break;
						}
					}
				}
				else
				{
					while (1)
					{
						float distance = pos1 - pos2;
						pos1 += 10;
						pos2 -= 10;
						if (distance > moving_correction)
						{
							break;
						}
					}
				}
			}
			return;
		}
	}
	//pl1[0].hitother = false;
	//pl2[0].hitother = false;
}


void Hitcheak::HitCheakAttack(AttackBox* attack1p, int a_max1p, AttackBox* attack2p, int a_max2p)
{
	add1P = 0.0f;
	add2P = 0.0f;
	for (int atknum = 0; atknum < a_max1p; atknum++)
	{
		if (attack1p[atknum].hit_ok)
		{
			for (int hitnum = 0; hitnum < a_max2p; hitnum++)
			{
				if (attack2p[hitnum].hit_ok)
				{
					if (attack1p[atknum].center.x - attack1p[atknum].size.x<attack2p[hitnum].center.x + attack2p[hitnum].size.x && attack1p[atknum].center.x + attack1p[atknum].size.x>attack2p[hitnum].center.x - attack2p[hitnum].size.x)
					{
						if (attack1p[atknum].center.y - attack1p[atknum].size.y<attack2p[hitnum].center.y + attack2p[hitnum].size.y && attack1p[atknum].center.y + attack1p[atknum].size.y>attack2p[hitnum].center.y - attack2p[hitnum].size.y)
						{
							if (attack1p[atknum].gaugeout && !attack2p[hitnum].gaugeout)
							{
								attack2p[hitnum].damege = 0;
								attack2p[hitnum].hit_ok = false;
								return;
							}
							if (attack2p[hitnum].gaugeout && !attack1p[atknum].gaugeout)
							{
								attack1p[atknum].damege = 0;
								attack1p[atknum].hit_ok = false;
								return;
							}

							clash = true;
							Hitcheak::timer = ((attack1p[atknum].damege + attack2p[hitnum].damege) / 4);
							if (Hitcheak::timer > 40)
							{
								Hitcheak::timer = 40;
							}
							add1P = attack1p[atknum].damege;
							add2P = attack2p[hitnum].damege;
							if (attack1p[atknum].center.x >= attack2p[hitnum].center.x)
							{
								float dis = attack1p[atknum].center.x - attack2p[hitnum].center.x;
								dis /= 2;
								clashpos.x = attack2p[hitnum].center.x + dis;
							}
							if (attack2p[hitnum].center.x > attack1p[atknum].center.x)
							{
								float dis = attack2p[hitnum].center.x - attack1p[atknum].center.x;
								dis /= 2;
								clashpos.x = attack1p[atknum].center.x + dis;
							}

							if (attack1p[atknum].center.y >= attack2p[hitnum].center.y)
							{
								float dis = attack1p[atknum].center.y - attack2p[hitnum].center.y;
								dis /= 2;
								clashpos.y = attack2p[hitnum].center.y + dis;
							}
							if (attack2p[hitnum].center.y > attack1p[atknum].center.y)
							{
								float dis = attack2p[hitnum].center.y - attack1p[atknum].center.y;
								dis /= 2;
								clashpos.y = attack1p[atknum].center.y + dis;
							}

							attack2p[hitnum].damege = 0;
							attack2p[hitnum].hit_ok = false;
							attack1p[atknum].damege = 0;
							attack1p[atknum].hit_ok = false;
							for (int n = 0; n < a_max1p; n++)
							{
								attack1p[n].damege = 0;
								//attack[n].HB_timer = 0;
								attack1p[n].hitback = YR_Vector3(0.0f, 0.0f);
								attack1p[n].hit_ok = false;
							}
							for (int n = 0; n < a_max2p; n++)
							{
								attack2p[n].damege = 0;
								//attack[n].HB_timer = 0;
								attack2p[n].hitback = YR_Vector3(0.0f, 0.0f);
								attack2p[n].hit_ok = false;
							}
						}
					}
				}
			}
		}
	}
}