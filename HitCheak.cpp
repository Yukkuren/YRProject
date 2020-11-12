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
		//�U�����肪���݂��Ȃ��ꍇ��return
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
						//0:�K�[�h����
						//1:��e
						//2:���G�ŉ��
						//3:�͂܂ꂽ
						//4:�@����

						switch (attack[atknum].parameter.type)
						{
						case AttackKind::UP:
							//��i�U��
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
							if (hit[hitnum].state == HitBoxState::UP_INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							if (hit[hitnum].state == HitBoxState::PROJECTILE_ARMOR)
							{
								flag = HitResultState::HIT;
							}
							break;
						case AttackKind::MIDDLE:
							//���i�U��
							if (hit[hitnum].state == HitBoxState::NOGUARD)
							{
								flag = HitResultState::HIT;
							}
							if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							if (hit[hitnum].state == HitBoxState::PROJECTILE_ARMOR)
							{
								flag = HitResultState::HIT;
							}
							break;
						case AttackKind::DOWN:
							//���i�U��
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
							if (hit[hitnum].state == HitBoxState::PROJECTILE_ARMOR)
							{
								flag = HitResultState::HIT;
							}
							break;
						case AttackKind::STEAL:
							//�����U��
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
							if (hit[hitnum].state == HitBoxState::PROJECTILE_ARMOR)
							{
								flag = HitResultState::STATE_NONE;
							}
							break;
						case AttackKind::SLAM:
							//�@�����U��
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
							if (hit[hitnum].state == HitBoxState::PROJECTILE_ARMOR)
							{
								flag = HitResultState::HIT;
							}
							break;
						case AttackKind::PROJECTILE:
							//��ѓ���U��
							if (hit[hitnum].state == HitBoxState::NOGUARD)
							{
								flag = HitResultState::HIT;
							}
							if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							if (hit[hitnum].state == HitBoxState::PROJECTILE_ARMOR)
							{
								flag = HitResultState::ARMOR;
							}
							break;
						case AttackKind::TRACK:
							//�z�[�~���O�_�b�V��
							if (hit[hitnum].state == HitBoxState::NOGUARD)
							{
								flag = HitResultState::HIT;
							}
							if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							if (hit[hitnum].state == HitBoxState::UP_INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							if (hit[hitnum].state == HitBoxState::PROJECTILE_ARMOR)
							{
								flag = HitResultState::HIT;
							}
							break;
						case AttackKind::NO_TO_OFFSET:
							//���E���Ȃ��U��
							if (hit[hitnum].state == HitBoxState::NOGUARD)
							{
								flag = HitResultState::HIT;
							}
							if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							break;
						case AttackKind::NO_TO_OFFSET_UP:
							//��i�U���Ƒ��E���Ȃ��U��
							if (hit[hitnum].state == HitBoxState::NOGUARD)
							{
								flag = HitResultState::HIT;
							}
							if (hit[hitnum].state == HitBoxState::PROJECTILE_ARMOR)
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
							//�K�[�h����������
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
							hit[hitnum].hitback.x = attack[atknum].parameter.hitback.x / guard_back_adj_x;
							hit[hitnum].hitback.y = attack[atknum].parameter.hitback.y / guard_back_adj_y;
							if (attack[atknum].parameter.type != AttackKind::PROJECTILE)
							{
								for (int n = 0; n < attack.size(); n++)
								{
									attack[n].parameter.damege = 0;
									//attack[n].HB_timer = 0;
									attack[n].parameter.hitback = YR_Vector3(0.0f, 0.0f);
									attack[n].hit_ok = false;
								}
							}
							else
							{
								attack[atknum].parameter.damege = 0;
								//attack[n].HB_timer = 0;
								attack[atknum].parameter.hitback = YR_Vector3(0.0f, 0.0f);
								attack[atknum].hit_ok = false;
							}
							//�U�����K�[�h���ꂽ���Ƃ�ۑ�����
							attack[atknum].hit_result = HitResult::GUARD;
							return add;
						}
						break;
						case HitResultState::HIT:
						{
							//��e����
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

							if (attack[atknum].parameter.type != AttackKind::PROJECTILE)
							{
								for (int n = 0; n < attack.size(); n++)
								{
									attack[n].parameter.damege = 0;
									//attack[n].HB_timer = 0;
									attack[n].parameter.hitback = YR_Vector3(0.0f, 0.0f);
									attack[n].hit_ok = false;
								}
							}
							else
							{
								attack[atknum].parameter.damege = 0;
								//attack[n].HB_timer = 0;
								attack[atknum].parameter.hitback = YR_Vector3(0.0f, 0.0f);
								attack[atknum].hit_ok = false;
							}
							//�U���������������Ƃ�ۑ�����
							attack[atknum].hit_result = HitResult::HIT;
							hit[hitnum].hit_state = HitStateKind::NORMAL;
							//PlaySE(SE_HIT);
							return add;
						}
						break;
						case HitResultState::AVOIDANCE:
						{
							//���G�ŉ������
							return 0.0f;
						}
						break;
						case HitResultState::STATE_NONE:
						{
							//�͂܂ꂽ
							hit[hitnum].hit_state = HitStateKind::STEAL;
							hit[hitnum].steal_timer = attack[atknum].parameter.stealtimer;
							hit[hitnum].hit = true;
							//�U���������������Ƃ�ۑ�����
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
							//�@�������ꂽ
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
							//�U���������������Ƃ�ۑ�����

							if (attack[atknum].pos.y > slam_up_line)
							{
								//���������ȏゾ�����ꍇ�A�@�����U���Ɉڍs���J�����Y�[�����s��
								hit[hitnum].hit_state = HitStateKind::SLAM;
								Hitcheak::timer = 0.45f;
								YRCamera.RequestCamera(player);
							}
							else if (attack[atknum].pos.y > slam_zoom_line)
							{
								//���������ȏゾ�����ꍇ�A�ʏ�̍U���ŃJ�����Y�[�����s��
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
						case HitResultState::ARMOR:
						{
							//�A�[�}�[�őς���
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

							if (attack[atknum].parameter.type != AttackKind::PROJECTILE)
							{
								for (int n = 0; n < attack.size(); n++)
								{
									attack[n].parameter.damege = 0;
									//attack[n].HB_timer = 0;
									attack[n].parameter.hitback = YR_Vector3(0.0f, 0.0f);
									attack[n].hit_ok = false;
								}
							}
							else
							{
								attack[atknum].parameter.damege = 0;
								//attack[n].HB_timer = 0;
								attack[atknum].parameter.hitback = YR_Vector3(0.0f, 0.0f);
								attack[atknum].hit_ok = false;
							}
							//�U���������������Ƃ�ۑ�����
							attack[atknum].hit_result = HitResult::HIT;
							hit[hitnum].hit_state = HitStateKind::ARMOR;
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
	//0���K���̂̓����蔻��ɂȂ��0�����Ŕ��肵�Ă���
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
		auto& atk1p = attack1p[atknum];
		if (atk1p.hit_ok)
		{
			for (int hitnum = 0; hitnum < attack2p.size(); hitnum++)
			{
				auto& atk2p = attack2p[hitnum];
				if (atk2p.hit_ok)
				{
					if (atk1p.pos.x - atk1p.parameter.size.x<atk2p.pos.x + atk2p.parameter.size.x &&
						atk1p.pos.x + atk1p.parameter.size.x>atk2p.pos.x - atk2p.parameter.size.x)
					{
						if (atk1p.pos.y - atk1p.parameter.size.y<atk2p.pos.y + atk2p.parameter.size.y &&
							atk1p.pos.y + atk1p.parameter.size.y>atk2p.pos.y - atk2p.parameter.size.y)
						{
							if(atk1p.parameter.type == AttackKind::NO_TO_OFFSET &&
								atk2p.parameter.type != AttackKind::NO_TO_OFFSET)
							{
								//1p�̍U�������E���Ȃ��U��������
								continue;
							}
							if (atk2p.parameter.type == AttackKind::NO_TO_OFFSET &&
								atk1p.parameter.type != AttackKind::NO_TO_OFFSET)
							{
								//2p�̍U�������E���Ȃ��U��������
								continue;
							}
							//�����Ƃ����E���Ȃ��U���Ȃ瑊�E������

							if (atk1p.parameter.type == AttackKind::NO_TO_OFFSET_UP)
							{
								//1p�̍U������i�Ƃ͑��E���Ȃ��U��������
								if (atk2p.parameter.type == AttackKind::UP||
									atk2p.parameter.type == AttackKind::TRACK)
								{
									//2P�̍U������i�A�܂��̓z�[�~���O�_�b�V���������ꍇ
									continue;
								}
							}
							if (atk2p.parameter.type == AttackKind::NO_TO_OFFSET_UP)
							{
								//2p�̍U������i�Ƃ͑��E���Ȃ��U��������
								if (atk1p.parameter.type == AttackKind::UP ||
									atk1p.parameter.type == AttackKind::TRACK)
								{
									//1P�̍U������i�A�܂��̓z�[�~���O�_�b�V���������ꍇ
									continue;
								}
							}

							if (atk1p.parameter.type == AttackKind::TRACK)
							{
								//1p�̍U�����z�[�~���O�_�b�V��
								if (atk2p.parameter.type == AttackKind::PROJECTILE)
								{
									//2P�̍U������ѓ�������ꍇ
									continue;
								}
							}
							if (atk2p.parameter.type == AttackKind::TRACK)
							{
								//2p�̍U�����z�[�~���O�_�b�V��
								if (atk1p.parameter.type == AttackKind::PROJECTILE)
								{
									//1P�̍U������ѓ�������ꍇ
									continue;
								}
							}


							clash = true;
							Hitcheak::timer = 0.0f;

							switch (atk1p.parameter.HS_timer)
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

							switch (atk2p.parameter.HS_timer)
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

							add1P = atk1p.parameter.damege;
							add2P = atk2p.parameter.damege;
							if (atk1p.parameter.distance.x >= atk2p.parameter.distance.x)
							{
								float dis = atk1p.parameter.distance.x - atk2p.parameter.distance.x;
								dis /= 2;
								clashpos.x = atk2p.parameter.distance.x + dis;
							}
							if (atk2p.parameter.distance.x > atk1p.parameter.distance.x)
							{
								float dis = atk2p.parameter.distance.x - atk1p.parameter.distance.x;
								dis /= 2;
								clashpos.x = atk1p.parameter.distance.x + dis;
							}

							if (atk1p.parameter.distance.y >= atk2p.parameter.distance.y)
							{
								float dis = atk1p.parameter.distance.y - atk2p.parameter.distance.y;
								dis /= 2;
								clashpos.y = atk2p.parameter.distance.y + dis;
							}
							if (atk2p.parameter.distance.y > atk1p.parameter.distance.y)
							{
								float dis = atk2p.parameter.distance.y - atk1p.parameter.distance.y;
								dis /= 2;
								clashpos.y = atk1p.parameter.distance.y + dis;
							}

							atk2p.parameter.damege = 0;
							atk2p.hit_ok = false;
							atk2p.knock_start = true;
							atk1p.parameter.damege = 0;
							atk1p.hit_ok = false;
							atk1p.knock_start = true;

							if (atk1p.parameter.type != AttackKind::PROJECTILE)
							{
								for (int n = 0; n < attack1p.size(); n++)
								{
									attack1p[n].parameter.damege = 0;
									//attack[n].HB_timer = 0;
									attack1p[n].parameter.hitback = YR_Vector3(0.0f, 0.0f);
									attack1p[n].hit_ok = false;
								}
							}
							else
							{
								atk1p.parameter.damege = 0;
								//attack[n].HB_timer = 0;
								atk1p.parameter.hitback = YR_Vector3(0.0f, 0.0f);
								atk1p.hit_ok = false;
							}
							if (atk2p.parameter.type != AttackKind::PROJECTILE)
							{
								for (int n = 0; n < attack2p.size(); n++)
								{
									attack2p[n].parameter.damege = 0;
									//attack[n].HB_timer = 0;
									attack2p[n].parameter.hitback = YR_Vector3(0.0f, 0.0f);
									attack2p[n].hit_ok = false;
								}
							}
							else
							{
								atk2p.parameter.damege = 0;
								//attack[n].HB_timer = 0;
								atk2p.parameter.hitback = YR_Vector3(0.0f, 0.0f);
								atk2p.hit_ok = false;
							}
							atk1p.hit_result = HitResult::GUARD;
							atk2p.hit_result = HitResult::GUARD;
						}
					}
				}
			}
		}
	}
}