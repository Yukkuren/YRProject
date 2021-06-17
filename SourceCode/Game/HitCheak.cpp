#include "HitCheak.h"
#include <stdlib.h>
#include <time.h>
#include "../YR_VectorMaster.h"
#include "../camera.h"

#include "../Effect.h"
#include "../YRSound.h"
#include "../framework.h"

#include "World.h"
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

				if (attack[atknum].pos.x - attack[atknum].parameter.size.x<hit[hitnum].center.x + hit[hitnum].parameter.size.x &&
					attack[atknum].pos.x + attack[atknum].parameter.size.x>hit[hitnum].center.x - hit[hitnum].parameter.size.x)
				{
					if (attack[atknum].pos.y - attack[atknum].parameter.size.y<hit[hitnum].center.y + hit[hitnum].parameter.size.y &&
						attack[atknum].pos.y + attack[atknum].parameter.size.y>hit[hitnum].center.y - hit[hitnum].parameter.size.y)
					{
						HitResultState flag = HitResultState::GUARD_OK;


						//�e���e�ɂ̓K�[�h�ł�����̂����L�q���Ă����A����ȊO�͑S���܂Ƃ߂ăq�b�g�������Ƃɂ���
						switch (attack[atknum].parameter.type)
						{
						case AttackKind::UP:
							//��i�U��
							if (hit[hitnum].state == HitBoxState::MIDDLE)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::ALL)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else if (hit[hitnum].state == HitBoxState::UP_INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else if (hit[hitnum].state == HitBoxState::SLIDE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else
							{
								flag = HitResultState::HIT;
							}
							break;
						case AttackKind::MIDDLE:
							//���i�U��
							if (hit[hitnum].state == HitBoxState::MIDDLE)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::DOWN)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::ALL)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else if (hit[hitnum].state == HitBoxState::SLIDE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else
							{
								flag = HitResultState::HIT;
							}
							break;
						case AttackKind::DOWN:
							//���i�U��
							if (hit[hitnum].state == HitBoxState::DOWN)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else if (hit[hitnum].state == HitBoxState::ALL)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::SLIDE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else
							{
								flag = HitResultState::HIT;
							}
							break;
						case AttackKind::STEAL:
							//�����U��
							if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else if (hit[hitnum].state == HitBoxState::SLIDE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else
							{
								flag = HitResultState::STATE_NONE;
							}
							break;
						case AttackKind::SLAM:
							//�@�����U��
							if (hit[hitnum].state == HitBoxState::MIDDLE)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::ALL)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else if (hit[hitnum].state == HitBoxState::UP_INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else if (hit[hitnum].state == HitBoxState::SLIDE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else
							{
								flag = HitResultState::SLAM;
							}
							break;
						case AttackKind::PROJECTILE:
							//��ѓ���U��
							if (hit[hitnum].state == HitBoxState::ALL)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else if (hit[hitnum].state == HitBoxState::MIDDLE)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::DOWN)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::PROJECTILE_ARMOR)
							{
								flag = HitResultState::ARMOR;
							}
							else if (hit[hitnum].state == HitBoxState::SLIDE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else
							{
								flag = HitResultState::HIT;
							}
							break;
						case AttackKind::TRACK:
							//�z�[�~���O�_�b�V��
							if (hit[hitnum].state == HitBoxState::ALL)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::MIDDLE)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::DOWN)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else if (hit[hitnum].state == HitBoxState::UP_INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else if (hit[hitnum].state == HitBoxState::SLIDE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else
							{
								flag = HitResultState::HIT;
							}
							break;
						case AttackKind::NO_TO_OFFSET:
							//���E���Ȃ��U��
							if (hit[hitnum].state == HitBoxState::ALL)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else if (hit[hitnum].state == HitBoxState::MIDDLE)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::DOWN)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::SLIDE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else
							{
								flag = HitResultState::HIT;
							}
							break;
						case AttackKind::NO_TO_OFFSET_UP:
							//��i�U���Ƒ��E���Ȃ��U��
							if (hit[hitnum].state == HitBoxState::ALL)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else if (hit[hitnum].state == HitBoxState::MIDDLE)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::DOWN)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::SLIDE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else
							{
								flag = HitResultState::HIT;
							}
							break;
						case AttackKind::DOWN_ATTACK:
							//�_�E������U��
							if (hit[hitnum].state == HitBoxState::MIDDLE)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::DOWN)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::ALL)
							{
								flag = HitResultState::GUARD_OK;
							}
							else if (hit[hitnum].state == HitBoxState::INVINCIBLE)
							{
								flag = HitResultState::AVOIDANCE;
							}
							else
							{
								flag = HitResultState::DOWN_HIT;
							}
							break;
						default:
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
							hit[hitnum].param = attack[atknum].parameter.param;
							if (attack[atknum].parameter.type != AttackKind::PROJECTILE)
							{
								for (int n = 0; n < attack.size(); n++)
								{
									attack[n].parameter.param.Reset();
									attack[n].hit_ok = false;
								}
							}
							else
							{
								attack[atknum].parameter.param.Reset();
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
							hit[hitnum].param = attack[atknum].parameter.param;
							//Hitcheak::timer = ((attack[atknum].parameter.damege*0.1f) / hitstop_adjust);
							if (player == 1)
							{
								Hitcheak::stop1p = true;
							}
							if (player == 2)
							{
								Hitcheak::stop2p = true;
							}

							if (attack[atknum].pos.x >= hit[hitnum].center.x)
							{
								float dis = attack[atknum].pos.x - hit[hitnum].center.x;
								dis *= 0.5f;
								effectpos.x = hit[hitnum].center.x + dis;
							}
							if (hit[hitnum].center.x > attack[atknum].pos.x)
							{
								float dis = hit[hitnum].center.x - attack[atknum].pos.x;
								dis *= 0.5f;
								effectpos.x = attack[atknum].pos.x + dis;
							}

							if (attack[atknum].pos.y >= hit[hitnum].center.y)
							{
								float dis = attack[atknum].pos.y - hit[hitnum].center.y;
								dis *= 0.5f;
								effectpos.y = hit[hitnum].center.y + dis;
							}

							if (hit[hitnum].center.y > attack[atknum].pos.y)
							{
								float dis = hit[hitnum].center.y - attack[atknum].pos.y;
								dis *= 0.5f;
								effectpos.y = attack[atknum].pos.y + dis;
							}

							//�G�t�F�N�g����
							YRGetEffect().PlayEffect(EffectKind::DAMAGE, effectpos.GetDXFLOAT3(), DirectX::XMFLOAT3(8.0f, 8.0f, 8.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);

							//SE�Đ�
							GetSound().SESinglePlay(SEKind::HIT_SWORD);
							//SE�Đ�
							GetSound().SESinglePlay(SEKind::HIT);

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
								//SE�Đ�
								GetSound().SESinglePlay(SEKind::UP_ATTACK);
								break;
							default:
								Hitcheak::timer = 0.05f;
								break;
							}

							if (attack[atknum].parameter.type != AttackKind::PROJECTILE)
							{
								for (int n = 0; n < attack.size(); n++)
								{
									attack[n].parameter.param.Reset();
									attack[n].hit_ok = false;
								}
							}
							else
							{
								attack[atknum].parameter.param.Reset();
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
							hit[hitnum].param = attack[atknum].parameter.param;
							hit[hitnum].hit = true;

							//SE�Đ�
							GetSound().SESinglePlay(SEKind::HIT);
							//�U���������������Ƃ�ۑ�����
							for (int n = 0; n < attack.size(); n++)
							{
								attack[n].parameter.param.Reset();
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
							hit[hitnum].param = attack[atknum].parameter.param;
							//Hitcheak::timer = ((attack[atknum].parameter.damege*0.1f) / hitstop_adjust);
							if (player == 1)
							{
								Hitcheak::stop1p = true;
							}
							if (player == 2)
							{
								Hitcheak::stop2p = true;
							}

							if (attack[atknum].pos.x >= hit[hitnum].center.x)
							{
								float dis = attack[atknum].pos.x - hit[hitnum].center.x;
								dis *= 0.5f;
								effectpos.x = hit[hitnum].center.x + dis;
							}
							if (hit[hitnum].center.x > attack[atknum].pos.x)
							{
								float dis = hit[hitnum].center.x - attack[atknum].pos.x;
								dis *= 0.5f;
								effectpos.x = attack[atknum].pos.x + dis;
							}

							if (attack[atknum].pos.y >= hit[hitnum].center.y)
							{
								float dis = attack[atknum].pos.y - hit[hitnum].center.y;
								dis *= 0.5f;
								effectpos.y = hit[hitnum].center.y + dis;
							}

							if (hit[hitnum].center.y > attack[atknum].pos.y)
							{
								float dis = hit[hitnum].center.y - attack[atknum].pos.y;
								dis *= 0.5f;
								effectpos.y = attack[atknum].pos.y + dis;
							}

							//�G�t�F�N�g�Đ�
							YRGetEffect().PlayEffect(EffectKind::DAMAGE, effectpos.GetDXFLOAT3(), DirectX::XMFLOAT3(8.0f, 8.0f, 8.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);

							//SE�Đ�
							GetSound().SESinglePlay(SEKind::HIT_SWORD);
							//SE�Đ�
							GetSound().SESinglePlay(SEKind::HIT);

							for (int n = 0; n < attack.size(); n++)
							{
								attack[n].parameter.param.Reset();
								attack[n].hit_ok = false;
							}
							//�U���������������Ƃ�ۑ�����

							if (pos.y > slam_up_line)
							{
								//���������ȏゾ�����ꍇ�A�@�����U���Ɉڍs���J�����Y�[�����s��
								hit[hitnum].hit_state = HitStateKind::SLAM;
								Hitcheak::timer = 0.45f;
								YRCamera.RequestCamera(player);
								//SE�Đ�
								GetSound().SESinglePlay(SEKind::UP_ATTACK);
							}
							else if (pos.y > slam_zoom_line)
							{
								//���������ȏゾ�����ꍇ�A�ʏ�̍U���ŃJ�����Y�[�����s��
								hit[hitnum].hit_state = HitStateKind::NORMAL;
								Hitcheak::timer = 0.45f;
								YRCamera.RequestCamera(player);
								//SE�Đ�
								GetSound().SESinglePlay(SEKind::UP_ATTACK);
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
							hit[hitnum].param = attack[atknum].parameter.param;
							//Hitcheak::timer = ((attack[atknum].parameter.damege*0.1f) / hitstop_adjust);
							if (player == 1)
							{
								Hitcheak::stop1p = true;
							}
							if (player == 2)
							{
								Hitcheak::stop2p = true;
							}

							if (attack[atknum].pos.x >= hit[hitnum].center.x)
							{
								float dis = attack[atknum].pos.x - hit[hitnum].center.x;
								dis *= 0.5f;
								effectpos.x = hit[hitnum].center.x + dis;
							}
							if (hit[hitnum].center.x > attack[atknum].pos.x)
							{
								float dis = hit[hitnum].center.x - attack[atknum].pos.x;
								dis *= 0.5f;
								effectpos.x = attack[atknum].pos.x + dis;
							}

							if (attack[atknum].pos.y >= hit[hitnum].center.y)
							{
								float dis = attack[atknum].pos.y - hit[hitnum].center.y;
								dis *= 0.5f;
								effectpos.y = hit[hitnum].center.y + dis;
							}

							if (hit[hitnum].center.y > attack[atknum].pos.y)
							{
								float dis = hit[hitnum].center.y - attack[atknum].pos.y;
								dis *= 0.5f;
								effectpos.y = attack[atknum].pos.y + dis;
							}

							//�G�t�F�N�g�Đ�
							YRGetEffect().PlayEffect(EffectKind::DAMAGE, effectpos.GetDXFLOAT3(), DirectX::XMFLOAT3(8.0f, 8.0f, 8.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);

							//SE�Đ�
							GetSound().SESinglePlay(SEKind::HIT_SWORD);
							//SE�Đ�
							GetSound().SESinglePlay(SEKind::HIT);

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
								//SE�Đ�
								GetSound().SESinglePlay(SEKind::UP_ATTACK);
								break;
							default:
								Hitcheak::timer = 0.05f;
								break;
							}

							if (attack[atknum].parameter.type != AttackKind::PROJECTILE)
							{
								for (int n = 0; n < attack.size(); n++)
								{
									attack[n].parameter.param.Reset();
									attack[n].hit_ok = false;
								}
							}
							else
							{
								attack[atknum].parameter.param.Reset();
								attack[atknum].hit_ok = false;
							}
							//�U���������������Ƃ�ۑ�����
							attack[atknum].hit_result = HitResult::HIT;
							hit[hitnum].hit_state = HitStateKind::ARMOR;
							//PlaySE(SE_HIT);
							return add;
						}
						break;
						case HitResultState::DOWN_HIT:
						{
							//�_�E���U��
							float add = 0.0f;
							if (!attack[atknum].parameter.gaugeout)
							{
								add = attack[atknum].parameter.gauge_get;
							}
							hit[hitnum].hit = true;
							attack[atknum].hit_ok = false;
							attack[atknum].knock_start = true;
							hit[hitnum].param = attack[atknum].parameter.param;
							//Hitcheak::timer = ((attack[atknum].parameter.damege*0.1f) / hitstop_adjust);
							if (player == 1)
							{
								Hitcheak::stop1p = true;
							}
							if (player == 2)
							{
								Hitcheak::stop2p = true;
							}

							if (attack[atknum].pos.x >= hit[hitnum].center.x)
							{
								float dis = attack[atknum].pos.x - hit[hitnum].center.x;
								dis *= 0.5f;
								effectpos.x = hit[hitnum].center.x + dis;
							}
							if (hit[hitnum].center.x > attack[atknum].pos.x)
							{
								float dis = hit[hitnum].center.x - attack[atknum].pos.x;
								dis *= 0.5f;
								effectpos.x = attack[atknum].pos.x + dis;
							}

							if (attack[atknum].pos.y >= hit[hitnum].center.y)
							{
								float dis = attack[atknum].pos.y - hit[hitnum].center.y;
								dis *= 0.5f;
								effectpos.y = hit[hitnum].center.y + dis;
							}

							if (hit[hitnum].center.y > attack[atknum].pos.y)
							{
								float dis = hit[hitnum].center.y - attack[atknum].pos.y;
								dis *= 0.5f;
								effectpos.y = attack[atknum].pos.y + dis;
							}

							//�G�t�F�N�g�Đ�
							YRGetEffect().PlayEffect(EffectKind::DAMAGE, effectpos.GetDXFLOAT3(), DirectX::XMFLOAT3(8.0f, 8.0f, 8.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);

							//SE�Đ�
							GetSound().SESinglePlay(SEKind::HIT_SWORD);
							//SE�Đ�
							GetSound().SESinglePlay(SEKind::HIT);

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
								//SE�Đ�
								GetSound().SESinglePlay(SEKind::UP_ATTACK);
								break;
							default:
								Hitcheak::timer = 0.05f;
								break;
							}

							if (attack[atknum].parameter.type != AttackKind::PROJECTILE)
							{
								for (int n = 0; n < attack.size(); n++)
								{
									attack[n].parameter.param.Reset();
									attack[n].hit_ok = false;
								}
							}
							else
							{
								attack[atknum].parameter.param.Reset();
								attack[atknum].hit_ok = false;
							}
							//�U���������������Ƃ�ۑ�����
							attack[atknum].hit_result = HitResult::HIT;
							hit[hitnum].hit_state = HitStateKind::DOWN;
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


void Hitcheak::HitPlayer(std::vector <HitBox>& pl1, YR_Vector3& pos1, std::vector <HitBox>& pl2, YR_Vector3& pos2, float elapsed_time, const YR_Vector3 old_pos1, const YR_Vector3 old_pos2)
{
	//�̂̓����蔻���1�ŌŒ肷��ׁA1�����Ŕ��肷��
	int p1_h = 1;
	int p2_h = 1;

	if (pl1[p1_h].center.x - pl1[p1_h].parameter.size.x<pl2[p2_h].center.x + pl2[p2_h].parameter.size.x &&
		pl1[p1_h].center.x + pl1[p1_h].parameter.size.x>pl2[p2_h].center.x - pl2[p2_h].parameter.size.x)
	{
		if (pl1[p1_h].center.y - pl1[p1_h].parameter.size.y<pl2[p2_h].center.y + pl2[p2_h].parameter.size.y &&
			pl1[p1_h].center.y + pl1[p1_h].parameter.size.y>pl2[p2_h].center.y - pl2[p2_h].parameter.size.y)
		{
			float moving_correction = (pl1[p1_h].parameter.size.x / 2.0f) + (pl2[p2_h].parameter.size.x / 2.0f);


			if (pos1.x < pos2.x)
			{
				while (1)
				{
					float distance = pos2.x - pos1.x;
					pos1.x -= correction_value * elapsed_time;
					pos2.x += correction_value * elapsed_time;
					if (distance > moving_correction)
					{
						break;
					}
				}
			}
			if (pos1.x > pos2.x)
			{
				while (1)
				{
					float distance = pos1.x - pos2.x;
					pos1.x += correction_value * elapsed_time;
					pos2.x -= correction_value * elapsed_time;
					if (distance > moving_correction)
					{
						break;
					}
				}
			}
			if (pos1.x == pos2.x)
			{
				if (old_pos2.x >= old_pos1.x)
				{
					while (1)
					{
						float distance = pos2.x - pos1.x;
						pos1.x -= correction_value * elapsed_time;
						pos2.x += correction_value * elapsed_time;
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
						float distance = pos1.x - pos2.x;
						pos1.x += correction_value * elapsed_time;
						pos2.x -= correction_value * elapsed_time;
						if (distance > moving_correction)
						{
							break;
						}
					}
				}
			}

			FRAMEWORK.scenegame.player1p->StopHitParamUpdate();
			FRAMEWORK.scenegame.player2p->StopHitParamUpdate();
			return;
		}
	}
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

							HitResultAttack result = HitResultAttack::OFFSET;


							//1P���̃X�e�[�g�m�F
							switch (atk1p.parameter.type)
							{
							case AttackKind::LOCK:
								//���b�N�Z
								continue;
								break;
							case AttackKind::PROJECTILE:
								//��ѓ���
								break;
							case AttackKind::TRACK:
								//�z�[�~���O�_�b�V��
								if (atk2p.parameter.type == AttackKind::PROJECTILE)
								{
									//��ѓ���Ƃ͑��E���Ȃ�
									continue;
								}
								break;
							case AttackKind::NO_TO_OFFSET:
								//���E���Ȃ��U��
								if (atk2p.parameter.type != AttackKind::NO_TO_OFFSET)
								{
									//���E�U���ȊO�Ƃ͑��E���Ȃ�
									continue;
								}
								break;
							case AttackKind::NO_TO_OFFSET_UP:
								//��i�U���Ƃ͑��E���Ȃ��U��
								if (atk2p.parameter.type == AttackKind::UP||
									atk2p.parameter.type == AttackKind::TRACK ||
									atk2p.parameter.type == AttackKind::SLAM)
								{
									//��i�A�z�[�~���O�_�b�V���܂��͒@�����U���Ƃ͑��E���Ȃ�
									continue;
								}
								break;
							case AttackKind::DOWN_ATTACK:
								//�_�E���U��
								if (atk2p.parameter.type != AttackKind::DOWN_ATTACK)
								{
									if (atk2p.parameter.type == AttackKind::PROJECTILE)
									{
										//��ѓ���ɂ͈���I�ɑł�����
										result = HitResultAttack::P2_OFFSET;
									}
									else
									{
										//�_�E���U���ȊO�Ƃ͑��E���Ȃ�
										continue;
									}
								}
								break;
							default:
								if (atk1p.parameter.param.damage - offset_near_damage > atk2p.parameter.param.damage)
								{
									//�U���̃_���[�W��2P�̍U���̃_���[�W�͈̔͊O�Ȃ瑊�E���Ȃ�
									continue;
								}
								result = HitResultAttack::OFFSET;
								break;
							}

							//2P���̃X�e�[�g�m�F
							switch (atk2p.parameter.type)
							{
							case AttackKind::LOCK:
								//���b�N�Z
								continue;
								break;
							case AttackKind::PROJECTILE:
								//��ѓ���
								break;
							case AttackKind::TRACK:
								//�z�[�~���O�_�b�V��
								if (atk1p.parameter.type == AttackKind::PROJECTILE)
								{
									//��ѓ���ɂ͑��E���Ȃ�
									continue;
								}
								break;
							case AttackKind::NO_TO_OFFSET:
								//���E���Ȃ��U��
								if (atk1p.parameter.type != AttackKind::NO_TO_OFFSET)
								{
									//���E�U���ȊO�Ƃ͑��E���Ȃ�
									continue;
								}
								break;
							case AttackKind::NO_TO_OFFSET_UP:
								//��i�U���Ƃ͑��E���Ȃ��U��
								if (atk1p.parameter.type == AttackKind::UP ||
									atk1p.parameter.type == AttackKind::TRACK ||
									atk1p.parameter.type == AttackKind::SLAM)
								{
									//��i�A�z�[�~���O�_�b�V���܂��͒@�����U���Ƃ͑��E���Ȃ�
									continue;
								}
								break;
							case AttackKind::DOWN_ATTACK:
								//�_�E���U��
								if (atk1p.parameter.type != AttackKind::DOWN_ATTACK)
								{
									if (atk1p.parameter.type == AttackKind::PROJECTILE)
									{
										//��ѓ���ɂ͈���I�ɑł�����
										result = HitResultAttack::P1_OFFSET;
									}
									else
									{
										//�_�E���U���ȊO�Ƃ͑��E���Ȃ�
										continue;
									}
								}
								break;
							default:
								if (atk2p.parameter.param.damage - offset_near_damage > atk1p.parameter.param.damage)
								{
									//�U���̃_���[�W��1P�̍U���̃_���[�W�͈̔͊O�Ȃ瑊�E���Ȃ�
									continue;
								}
								result = HitResultAttack::OFFSET;
								break;
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


							switch (result)
							{
							case HitResultAttack::OFFSET:
								//���E
							{
								add1P = atk1p.parameter.gauge_get;
								add2P = atk2p.parameter.gauge_get;
								if (atk1p.parameter.type == AttackKind::DOWN_ATTACK)
								{
									add1P = atk1p.parameter.param.damage;
								}
								if (atk1p.parameter.type == AttackKind::DOWN_ATTACK)
								{
									add2P = atk2p.parameter.param.damage;
								}
								if (atk1p.pos.x >= atk2p.pos.x)
								{
									float dis = atk1p.pos.x - atk2p.pos.x;
									dis *= 0.5f;
									clashpos.x = atk2p.pos.x + dis;
								}
								if (atk2p.pos.x > atk1p.pos.x)
								{
									float dis = atk2p.pos.x - atk1p.pos.x;
									dis *= 0.5f;
									clashpos.x = atk1p.pos.x + dis;
								}

								if (atk1p.pos.y >= atk2p.pos.y)
								{
									float dis = atk1p.pos.y - atk2p.pos.y;
									dis *= 0.5f;
									clashpos.y = atk2p.pos.y + dis;
								}
								if (atk2p.pos.y > atk1p.pos.y)
								{
									float dis = atk2p.pos.y - atk1p.pos.y;
									dis *= 0.5f;
									clashpos.y = atk1p.pos.y + dis;
								}

								//�G�t�F�N�g�Đ�
								YRGetEffect().PlayEffect(EffectKind::OFFSET, clashpos.GetDXFLOAT3(), DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);

								//SE�Đ�
								GetSound().SESinglePlay(SEKind::OFFSET);

								atk1p.parameter.param.Reset();
								atk1p.hit_ok = false;
								atk1p.knock_start = true;
								//attack[n].HB_timer = 0;

								atk2p.parameter.param.Reset();
								//attack[n].HB_timer = 0;
								atk2p.hit_ok = false;
								atk2p.knock_start = true;

								atk1p.hit_result = HitResult::GUARD;
								atk2p.hit_result = HitResult::GUARD;
							}
								break;
							case HitResultAttack::P1_OFFSET:
								//1P�������E
							{
								add1P = 0.0f;
								add2P = atk2p.parameter.gauge_get;
								if (atk1p.pos.x >= atk2p.pos.x)
								{
									float dis = atk1p.pos.x - atk2p.pos.x;
									dis *= 0.5f;
									clashpos.x = atk2p.pos.x + dis;
								}
								if (atk2p.pos.x > atk1p.pos.x)
								{
									float dis = atk2p.pos.x - atk1p.pos.x;
									dis *= 0.5f;
									clashpos.x = atk1p.pos.x + dis;
								}

								if (atk1p.pos.y >= atk2p.pos.y)
								{
									float dis = atk1p.pos.y - atk2p.pos.y;
									dis *= 0.5f;
									clashpos.y = atk2p.pos.y + dis;
								}
								if (atk2p.pos.y > atk1p.pos.y)
								{
									float dis = atk2p.pos.y - atk1p.pos.y;
									dis *= 0.5f;
									clashpos.y = atk1p.pos.y + dis;
								}

								//�G�t�F�N�g�Đ�
								YRGetEffect().PlayEffect(EffectKind::OFFSET, clashpos.GetDXFLOAT3(), DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);

								//SE�Đ�
								GetSound().SESinglePlay(SEKind::OFFSET);

								atk1p.parameter.param.Reset();
								atk1p.knock_start = true;
								atk1p.hit_ok = false;

								atk1p.hit_result = HitResult::GUARD;
							}
								break;
							case HitResultAttack::P2_OFFSET:
								//2P�������E
							{
								add1P = atk1p.parameter.gauge_get;
								add2P = 0.0f;
								if (atk1p.pos.x >= atk2p.pos.x)
								{
									float dis = atk1p.pos.x - atk2p.pos.x;
									dis *= 0.5f;
									clashpos.x = atk2p.pos.x + dis;
								}
								if (atk2p.pos.x > atk1p.pos.x)
								{
									float dis = atk2p.pos.x - atk1p.pos.x;
									dis *= 0.5f;
									clashpos.x = atk1p.pos.x + dis;
								}

								if (atk1p.pos.y >= atk2p.pos.y)
								{
									float dis = atk1p.pos.y - atk2p.pos.y;
									dis *= 0.5f;
									clashpos.y = atk2p.pos.y + dis;
								}
								if (atk2p.pos.y > atk1p.pos.y)
								{
									float dis = atk2p.pos.y - atk1p.pos.y;
									dis *= 0.5f;
									clashpos.y = atk1p.pos.y + dis;
								}

								//�G�t�F�N�g�Đ�
								YRGetEffect().PlayEffect(EffectKind::OFFSET, clashpos.GetDXFLOAT3(), DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);

								//SE�Đ�
								GetSound().SESinglePlay(SEKind::OFFSET);

								atk2p.parameter.param.Reset();
								//attack[n].HB_timer = 0;
								atk2p.hit_ok = false;
								atk2p.knock_start = true;

								atk2p.hit_result = HitResult::GUARD;
							}
								break;
							default:
								break;
							}
						}
					}
				}

				if (!atk1p.hit_ok)
				{
					break;
				}
			}
		}
	}
}