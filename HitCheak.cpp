#include "HitCheak.h"
#include <stdlib.h>
#include <time.h>
#include "YR_VectorMaster.h"
#include "camera.h"
//#include "Music.h"

const float guard_gauge_get = 3.0f;

float Hitcheak::HitCheak(std::vector<AttackBox> &attack, std::vector<HitBox>& hit, int player, YR_Vector3 pos)
{
	if (attack.empty())
	{
		//UŒ‚”»’è‚ª‘¶İ‚µ‚È‚¢ê‡‚Íreturn
		return 0.0f;
	}

	for (int atknum = 0; atknum < attack.size(); atknum++)
	{
		if (attack[atknum].hit_ok)
		{
			for (int hitnum = 0; hitnum < hit.size(); hitnum++)
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
						HitResultState flag = HitResultState::GUARD_OK;
						//0:ƒK[ƒh¬Œ÷
						//1:”í’e
						//2:–³“G‚Å‰ñ”ğ
						//3:’Í‚Ü‚ê‚½
						//4:’@‚«‚Â‚¯

						switch (attack[atknum].parameter.type)
						{
						case AttackKind::UP:
							//ã’iUŒ‚
							if (hit[hitnum].state == HitBoxState::NOGUARD)
							{
								flag = HitResultState::HIT;
							}
							if (hit[hitnum].state == HitBoxState::DOWN)
							{
								flag = HitResultState::HIT;
							}
							if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							break;
						case AttackKind::MIDDLE:
							//’†’iUŒ‚
							if (hit[hitnum].state == HitBoxState::NOGUARD)
							{
								flag = HitResultState::HIT;
							}
							if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							break;
						case AttackKind::DOWN:
							//‰º’iUŒ‚
							if (hit[hitnum].state == HitBoxState::NOGUARD)
							{
								flag = HitResultState::HIT;
							}
							if (hit[hitnum].state == HitBoxState::MIDDLE)
							{
								flag = HitResultState::HIT;
							}
							if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							break;
						case AttackKind::STEAL:
							//“Š‚°UŒ‚
							if (hit[hitnum].state == HitBoxState::NOGUARD)
							{
								flag = HitResultState::STATE_NONE;
							}
							if (hit[hitnum].state == HitBoxState::MIDDLE)
							{
								flag = HitResultState::STATE_NONE;
							}
							if (hit[hitnum].state == HitBoxState::DOWN)
							{
								flag = HitResultState::STATE_NONE;
							}
							if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							break;
						case AttackKind::SLAM:
							//’@‚«‚Â‚¯UŒ‚
							if (hit[hitnum].state == HitBoxState::NOGUARD)
							{
								flag = HitResultState::SLAM;
							}
							if (hit[hitnum].state == HitBoxState::DOWN)
							{
								flag = HitResultState::SLAM;
							}
							if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							break;
						case AttackKind::PROJECTILE:
							//”ò‚Ñ“¹‹ïUŒ‚
							if (hit[hitnum].state == HitBoxState::NOGUARD)
							{
								flag = HitResultState::HIT;
							}
							if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							break;
						}


						switch (flag)
						{
						case HitResultState::GUARD_OK:
						{
							//ƒK[ƒh‚ª¬Œ÷‚µ‚½
							float add = 0.0f;
							if (!attack[atknum].parameter.gaugeout)
							{
								add = attack[atknum].parameter.gauge_get / guard_gauge_get;
							}
							hit[hitnum].guard_ok = true;
							attack[atknum].hit_ok = false;
							attack[atknum].knock_start = true;
							hit[hitnum].damege = attack[atknum].parameter.damege / guard_damege_adj;
							hit[hitnum].timer = attack[atknum].parameter.HB_timer / guard_stop_adj;
							hit[hitnum].hitback.x = attack[atknum].parameter.hitback.x / guard_back_adj;
							hit[hitnum].hitback.y = attack[atknum].parameter.hitback.y / guard_back_adj;
							for (int n = 0; n < attack.size(); n++)
							{
								attack[n].parameter.damege = 0;
								//attack[n].HB_timer = 0;
								attack[n].parameter.hitback = YR_Vector3(0.0f, 0.0f);
								attack[n].hit_ok = false;
							}
							//UŒ‚‚ªƒK[ƒh‚³‚ê‚½‚±‚Æ‚ğ•Û‘¶‚·‚é
							attack[atknum].hit_result = HitResult::GUARD;
							return add;
						}
						break;
						case HitResultState::HIT:
						{
							//”í’e‚µ‚½
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

							if (attack[atknum].parameter.distance.x >= hit[hitnum].center.x)
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
								Hitcheak::timer = 0.25f;
								break;
							case HitStopTime::LONG:
								Hitcheak::timer = 0.35f;
								break;
							case HitStopTime::ZOOM:
								Hitcheak::timer = 0.45f;
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
							//UŒ‚‚ª“–‚½‚Á‚½‚±‚Æ‚ğ•Û‘¶‚·‚é
							attack[atknum].hit_result = HitResult::HIT;
							hit[hitnum].hit_state = HitStateKind::NORMAL;
							//PlaySE(SE_HIT);
							return add;
						}
						break;
						case HitResultState::AVOIDANCE:
						{
							//–³“G‚Å‰ñ”ğ‚µ‚½
							return 0.0f;
						}
						break;
						case HitResultState::STATE_NONE:
						{
							//’Í‚Ü‚ê‚½
							hit[hitnum].hit_state = HitStateKind::STEAL;
							hit[hitnum].steal_timer = attack[atknum].parameter.stealtimer;
							hit[hitnum].hit = true;
							//UŒ‚‚ª“–‚½‚Á‚½‚±‚Æ‚ğ•Û‘¶‚·‚é
							for (int n = 0; n < attack.size(); n++)
							{
								attack[n].parameter.damege = 0;
								attack[n].parameter.hitback = YR_Vector3(0.0f, 0.0f);
								attack[n].hit_ok = false;
							}
							attack[atknum].knock_start = true;
							attack[atknum].hit_result = HitResult::HIT;
							return 0.0f;
						}
						break;
						case HitResultState::SLAM:
						{
							//’@‚«—‚³‚ê‚½
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

							if (attack[atknum].parameter.distance.x >= hit[hitnum].center.x)
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

							for (int n = 0; n < attack.size(); n++)
							{
								attack[n].parameter.damege = 0;
								//attack[n].HB_timer = 0;
								attack[n].parameter.hitback = YR_Vector3(0.0f, 0.0f);
								attack[n].hit_ok = false;
							}
							//UŒ‚‚ª“–‚½‚Á‚½‚±‚Æ‚ğ•Û‘¶‚·‚é

							if (attack[atknum].pos.y > slam_up_line)
							{
								//‚‚³‚ªˆê’èˆÈã‚¾‚Á‚½ê‡A’@‚«‚Â‚¯UŒ‚‚ÉˆÚs‚µƒJƒƒ‰ƒY[ƒ€‚ğs‚¤
								hit[hitnum].hit_state = HitStateKind::SLAM;
								Hitcheak::timer = 0.45f;
								YRCamera.RequestCamera(player);
							}
							else if (attack[atknum].pos.y > slam_zoom_line)
							{
								//‚‚³‚ªˆê’èˆÈã‚¾‚Á‚½ê‡A’Êí‚ÌUŒ‚‚ÅƒJƒƒ‰ƒY[ƒ€‚ğs‚¤
								hit[hitnum].hit_state = HitStateKind::NORMAL;
								Hitcheak::timer = 0.45f;
								YRCamera.RequestCamera(player);
							}
							else
							{
								hit[hitnum].hit_state = HitStateKind::NORMAL;
								Hitcheak::timer = 0.35f;
							}
							attack[atknum].hit_result = HitResult::HIT;
							//PlaySE(SE_HIT);
							return add;
						}
						break;
						default:
							break;
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

void Hitcheak::HitPlayer(HitBox* pl1, float& pos1, HitBox* pl2, float& pos2,float elapsed_time)
{
	//0‚ª•K‚¸‘Ì‚Ì“–‚½‚è”»’è‚É‚È‚éˆ×0‚¾‚¯‚Å”»’è‚µ‚Ä‚¢‚é
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
					pos1 -= correction_value * elapsed_time;
					pos2 += correction_value * elapsed_time;
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
					pos1 += correction_value * elapsed_time;
					pos2 -= correction_value * elapsed_time;
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
						pos1 -= correction_value * elapsed_time;
						pos2 += correction_value * elapsed_time;
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
						pos1 += correction_value * elapsed_time;
						pos2 -= correction_value * elapsed_time;
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
					if (attack1p[atknum].pos.x - attack1p[atknum].parameter.size.x<attack2p[hitnum].pos.x + attack2p[hitnum].parameter.size.x &&
						attack1p[atknum].pos.x + attack1p[atknum].parameter.size.x>attack2p[hitnum].pos.x - attack2p[hitnum].parameter.size.x)
					{
						if (attack1p[atknum].pos.y - attack1p[atknum].parameter.size.y<attack2p[hitnum].pos.y + attack2p[hitnum].parameter.size.y &&
							attack1p[atknum].pos.y + attack1p[atknum].parameter.size.y>attack2p[hitnum].pos.y - attack2p[hitnum].parameter.size.y)
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
							Hitcheak::timer = 0.0f;

							switch (attack1p[atknum].parameter.HS_timer)
							{
							case HitStopTime::SHORT:
								Hitcheak::timer += 0.05f;
								break;
							case HitStopTime::NORMAL:
								Hitcheak::timer += 0.10f;
								break;
							case HitStopTime::LONG:
								Hitcheak::timer += 0.15f;
								break;
							case HitStopTime::ZOOM:
								Hitcheak::timer += 0.20f;
								break;
							default:
								Hitcheak::timer += 0.05f;
								break;
							}

							switch (attack2p[atknum].parameter.HS_timer)
							{
							case HitStopTime::SHORT:
								Hitcheak::timer += 0.05f;
								break;
							case HitStopTime::NORMAL:
								Hitcheak::timer += 0.10f;
								break;
							case HitStopTime::LONG:
								Hitcheak::timer += 0.15f;
								break;
							case HitStopTime::ZOOM:
								Hitcheak::timer += 0.20f;
								break;
							default:
								Hitcheak::timer += 0.05f;
								break;
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
							attack2p[atknum].knock_start = true;
							attack1p[atknum].parameter.damege = 0;
							attack1p[atknum].hit_ok = false;
							attack1p[atknum].knock_start = true;
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
							attack1p[atknum].hit_result = HitResult::GUARD;
							attack2p[atknum].hit_result = HitResult::GUARD;
						}
					}
				}
			}
		}
	}
}