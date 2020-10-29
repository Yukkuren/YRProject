#include "HitCheak.h"
#include <stdlib.h>
#include <time.h>
#include "YR_VectorMaster.h"
#include "camera.h"
//#include "Music.h"

const float guard_gauge_get = 3.0f;

float Hitcheak::HitCheak(std::vector<AttackBox> &attack, HitBox* hit, int h_max, int player, YR_Vector3 pos)
{
	if (attack.empty())
	{
		//çUåÇîªíËÇ™ë∂ç›ÇµÇ»Ç¢èÍçáÇÕreturn
		return 0.0f;
	}

	for (int atknum = 0; atknum < attack.size(); atknum++)
	{
		if (attack[atknum].hit_ok)
		{
			for (int hitnum = 0; hitnum < h_max; hitnum++)
			{
				//float a = (pos.x + attack[atknum].parameter.distance.x) - attack[atknum].parameter.size.x;
				//float b = hit[hitnum].center.x + hit[hitnum].parameter.size.x;
				//float c = (pos.x + attack[atknum].parameter.distance.x) + attack[atknum].parameter.size.x;
				//float d = hit[hitnum].center.x - hit[hitnum].parameter.size.x;
				//float e = (pos.y + attack[atknum].parameter.distance.y) - attack[atknum].parameter.size.y;
				//float f = hit[hitnum].center.y + hit[hitnum].parameter.size.y;
				//float g = (pos.y + attack[atknum].parameter.distance.y) + attack[atknum].parameter.size.y;
				//float h = hit[hitnum].center.y - hit[hitnum].parameter.size.y;

				if (attack[atknum].pos.x - attack[atknum].parameter.size.x<hit[hitnum].center.x + hit[hitnum].parameter.size.x &&
					attack[atknum].pos.x + attack[atknum].parameter.size.x>hit[hitnum].center.x - hit[hitnum].parameter.size.x)
				{
					if (attack[atknum].pos.y - attack[atknum].parameter.size.y<hit[hitnum].center.y + hit[hitnum].parameter.size.y &&
						attack[atknum].pos.y + attack[atknum].parameter.size.y>hit[hitnum].center.y - hit[hitnum].parameter.size.y)
					{
						int flag = 0;
						//0:ÉKÅ[Éhê¨å˜
						//1:îÌíe
						//2:ñ≥ìGÇ≈âÒî
						//3:íÕÇ‹ÇÍÇΩ

						switch (attack[atknum].parameter.type)
						{
						case AttackBox::UP:
							if (hit[hitnum].parameter.state == HitBoxState::NOGUARD)
							{
								flag = 1;
							}
							if (hit[hitnum].parameter.state == HitBoxState::DOWN)
							{
								flag = 1;
							}
							if (hit[hitnum].parameter.state == HitBoxState::INVINCIBLE)
							{
								flag = 2;
							}
							break;
						case AttackBox::MIDDLE:
							if (hit[hitnum].parameter.state == HitBoxState::NOGUARD)
							{
								flag = 1;
							}
							if (hit[hitnum].parameter.state == HitBoxState::INVINCIBLE)
							{
								flag = 2;
							}
							break;
						case AttackBox::DOWN:
							if (hit[hitnum].parameter.state == HitBoxState::NOGUARD)
							{
								flag = 1;
							}
							if (hit[hitnum].parameter.state == HitBoxState::MIDDLE)
							{
								flag = 1;
							}
							if (hit[hitnum].parameter.state == HitBoxState::INVINCIBLE)
							{
								flag = 2;
							}
							break;
						case AttackBox::STEAL:
							if (hit[hitnum].parameter.state == HitBoxState::NOGUARD)
							{
								flag = 3;
							}
							if (hit[hitnum].parameter.state == HitBoxState::MIDDLE)
							{
								flag = 3;
							}
							if (hit[hitnum].parameter.state == HitBoxState::DOWN)
							{
								flag = 3;
							}
							if (hit[hitnum].parameter.state == HitBoxState::INVINCIBLE)
							{
								flag = 2;
							}
							break;
						}



						if (flag == 1)
						{
							//îÌíeÇµÇΩ
							float add = 0.0f;
							if (!attack[atknum].parameter.gaugeout)
							{
								add = attack[atknum].parameter.gauge_get;
							}
							hit[hitnum].hit = true;
							attack[atknum].hit_ok = false;
							attack[atknum].knock_start = true;
							hit[hitnum].damege = attack[atknum].parameter.damege;
							hit[hitnum].timer = attack[atknum].parameter.HB_timer;
							hit[hitnum].hitback = attack[atknum].parameter.hitback;
							//Hitcheak::timer = ((attack[atknum].parameter.damege*0.1f) / hitstop_adjust);
							if (player == 1)
							{
								Hitcheak::stop1p = true;
							}
							if (player == 2)
							{
								Hitcheak::stop2p = true;
							}

							if (attack[atknum].parameter.distance.x >= hit[atknum].center.x)
							{
								float dis = attack[atknum].parameter.distance.x - hit[hitnum].center.x;
								dis /= 2.0f;
								effectpos.x = hit[hitnum].center.x + dis;
								effecttimer = 10;
							}
							if (hit[hitnum].center.x > attack[atknum].parameter.distance.x)
							{
								float dis = hit[hitnum].center.x - attack[atknum].parameter.distance.x;
								dis /= 2.0f;
								effectpos.x = attack[atknum].parameter.distance.x + dis;
								effecttimer = 10;
							}

							if (attack[atknum].parameter.distance.y >= hit[hitnum].center.y)
							{
								float dis = attack[atknum].parameter.distance.y - hit[hitnum].center.y;
								dis /= 2.0f;
								effectpos.y = hit[hitnum].center.y + dis;
								effecttimer = 10;
							}
							if (hit[hitnum].center.y > attack[atknum].parameter.distance.y)
							{
								float dis = hit[hitnum].center.y - attack[atknum].parameter.distance.y;
								dis /= 2.0f;
								effectpos.y = attack[atknum].parameter.distance.y + dis;
								effecttimer = 10;
							}
							switch (attack[atknum].parameter.HS_timer)
							{
							case HitStopTime::SHORT:
								Hitcheak::timer = 0.05f;
								break;
							case HitStopTime::NORMAL:
								Hitcheak::timer = 0.15f;
								break;
							case HitStopTime::LONG:
								Hitcheak::timer = 0.25f;
								break;
							case HitStopTime::ZOOM:
								Hitcheak::timer = 0.35f;
								YRCamera.RequestCamera(player);
								break;
							default:
								Hitcheak::timer = 0.05f;
								break;
							}

							for (int n = 0; n < attack.size(); n++)
							{
								attack[n].parameter.damege = 0;
								//attack[n].HB_timer = 0;
								attack[n].parameter.hitback = YR_Vector3(0.0f, 0.0f);
								attack[n].hit_ok = false;
							}
							//çUåÇÇ™ìñÇΩÇ¡ÇΩÇ±Ç∆Çï€ë∂Ç∑ÇÈ
							attack[atknum].hit_result = HitResult::HIT;
							//PlaySE(SE_HIT);
							return add;
						}
						if (flag == 0)
						{
							//ÉKÅ[ÉhÇ™ê¨å˜ÇµÇΩ
							float add = 0.0f;
							if (!attack[atknum].parameter.gaugeout)
							{
								add = attack[atknum].parameter.gauge_get / guard_gauge_get;
							}
							hit[hitnum].guard_ok = true;
							attack[atknum].hit_ok = false;
							attack[atknum].knock_start = true;
							hit[hitnum].damege = attack[atknum].parameter.damege / guard_damege_adj;
							hit[hitnum].timer = attack[atknum].parameter.HB_timer;
							hit[hitnum].hitback.x = attack[atknum].parameter.hitback.x / guard_damege_adj;
							hit[hitnum].hitback.y = attack[atknum].parameter.hitback.y / guard_damege_adj;
							for (int n = 0; n < attack.size(); n++)
							{
								attack[n].parameter.damege = 0;
								//attack[n].HB_timer = 0;
								attack[n].parameter.hitback = YR_Vector3(0.0f, 0.0f);
								attack[n].hit_ok = false;
							}
							//çUåÇÇ™ÉKÅ[ÉhÇ≥ÇÍÇΩÇ±Ç∆Çï€ë∂Ç∑ÇÈ
							attack[atknum].hit_result = HitResult::GUARD;
							return add;
						}
						if (flag == 2)
						{
							//ñ≥ìGÇ≈âÒîÇµÇΩ
							return 0.0f;
						}

						if (flag == 3)
						{
							//íÕÇ‹ÇÍÇΩ
							hit[hitnum].steal = true;
							hit[hitnum].steal_timer = 20;
							//çUåÇÇ™ìñÇΩÇ¡ÇΩÇ±Ç∆Çï€ë∂Ç∑ÇÈ
							attack[atknum].hit_result = HitResult::HIT;
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
	//0Ç™ïKÇ∏ëÃÇÃìñÇΩÇËîªíËÇ…Ç»ÇÈà◊0ÇæÇØÇ≈îªíËÇµÇƒÇ¢ÇÈ
	if (pl1[0].center.x - pl1[0].parameter.size.x<pl2[0].center.x + pl2[0].parameter.size.x &&
		pl1[0].center.x + pl1[0].parameter.size.x>pl2[0].center.x - pl2[0].parameter.size.x)
	{
		if (pl1[0].center.y - pl1[0].parameter.size.y<pl2[0].center.y + pl2[0].parameter.size.y &&
			pl1[0].center.y + pl1[0].parameter.size.y>pl2[0].center.y - pl2[0].parameter.size.y)
		{
			//pl1[0].hitother = true;
			//pl2[0].hitother = true;
			float moving_correction = (pl1[0].parameter.size.x / 2.0f) + (pl2[0].parameter.size.x / 2.0f);
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


void Hitcheak::HitCheakAttack(std::vector<AttackBox> &attack1p,std::vector<AttackBox> &attack2p)
{
	add1P = 0.0f;
	add2P = 0.0f;
	if (attack1p.empty())
	{
		return;
	}
	if (attack2p.empty())
	{
		return;
	}

	for (int atknum = 0; atknum < attack1p.size(); atknum++)
	{
		if (attack1p[atknum].hit_ok)
		{
			for (int hitnum = 0; hitnum < attack2p.size(); hitnum++)
			{
				if (attack2p[hitnum].hit_ok)
				{
					if (attack1p[atknum].parameter.distance.x - attack1p[atknum].parameter.size.x<attack2p[hitnum].parameter.distance.x + attack2p[hitnum].parameter.size.x &&
						attack1p[atknum].parameter.distance.x + attack1p[atknum].parameter.size.x>attack2p[hitnum].parameter.distance.x - attack2p[hitnum].parameter.size.x)
					{
						if (attack1p[atknum].parameter.distance.y - attack1p[atknum].parameter.size.y<attack2p[hitnum].parameter.distance.y + attack2p[hitnum].parameter.size.y &&
							attack1p[atknum].parameter.distance.y + attack1p[atknum].parameter.size.y>attack2p[hitnum].parameter.distance.y - attack2p[hitnum].parameter.size.y)
						{
							if (attack1p[atknum].parameter.gaugeout && !attack2p[hitnum].parameter.gaugeout)
							{
								attack2p[hitnum].parameter.damege = 0;
								attack2p[hitnum].hit_ok = false;
								return;
							}
							if (attack2p[hitnum].parameter.gaugeout && !attack1p[atknum].parameter.gaugeout)
							{
								attack1p[atknum].parameter.damege = 0;
								attack1p[atknum].hit_ok = false;
								return;
							}

							clash = true;
							Hitcheak::timer = ((attack1p[atknum].parameter.damege + attack2p[hitnum].parameter.damege) / 4);
							if (Hitcheak::timer > 40)
							{
								Hitcheak::timer = 40;
							}
							add1P = attack1p[atknum].parameter.damege;
							add2P = attack2p[hitnum].parameter.damege;
							if (attack1p[atknum].parameter.distance.x >= attack2p[hitnum].parameter.distance.x)
							{
								float dis = attack1p[atknum].parameter.distance.x - attack2p[hitnum].parameter.distance.x;
								dis /= 2;
								clashpos.x = attack2p[hitnum].parameter.distance.x + dis;
							}
							if (attack2p[hitnum].parameter.distance.x > attack1p[atknum].parameter.distance.x)
							{
								float dis = attack2p[hitnum].parameter.distance.x - attack1p[atknum].parameter.distance.x;
								dis /= 2;
								clashpos.x = attack1p[atknum].parameter.distance.x + dis;
							}

							if (attack1p[atknum].parameter.distance.y >= attack2p[hitnum].parameter.distance.y)
							{
								float dis = attack1p[atknum].parameter.distance.y - attack2p[hitnum].parameter.distance.y;
								dis /= 2;
								clashpos.y = attack2p[hitnum].parameter.distance.y + dis;
							}
							if (attack2p[hitnum].parameter.distance.y > attack1p[atknum].parameter.distance.y)
							{
								float dis = attack2p[hitnum].parameter.distance.y - attack1p[atknum].parameter.distance.y;
								dis /= 2;
								clashpos.y = attack1p[atknum].parameter.distance.y + dis;
							}

							attack2p[hitnum].parameter.damege = 0;
							attack2p[hitnum].hit_ok = false;
							attack1p[atknum].parameter.damege = 0;
							attack1p[atknum].hit_ok = false;
							for (int n = 0; n < attack1p.size(); n++)
							{
								attack1p[n].parameter.damege = 0;
								//attack[n].HB_timer = 0;
								attack1p[n].parameter.hitback = YR_Vector3(0.0f, 0.0f);
								attack1p[n].hit_ok = false;
							}
							for (int n = 0; n < attack2p.size(); n++)
							{
								attack2p[n].parameter.damege = 0;
								//attack[n].HB_timer = 0;
								attack2p[n].parameter.hitback = YR_Vector3(0.0f, 0.0f);
								attack2p[n].hit_ok = false;
							}
						}
					}
				}
			}
		}
	}
}