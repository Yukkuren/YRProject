#include "HitCheak.h"
#include <stdlib.h>
#include <time.h>
#include "YR_VectorMaster.h"
#include "camera.h"

#include "Effect.h"
#include "YRSound.h"
#include "framework.h"
//#include "Music.h"

const float guard_gauge_get = 3.0f;

float Hitcheak::HitCheak(std::vector<AttackBox> &attack, std::vector<HitBox>& hit, int player, YR_Vector3 pos)
{
	if (attack.empty())
	{
		//攻撃判定が存在しない場合はreturn
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
						//0:ガード成功
						//1:被弾
						//2:無敵で回避
						//3:掴まれた
						//4:叩きつけ


						//各内容にはガードできるものだけ記述していき、それ以外は全部まとめてヒットしたことにする
						switch (attack[atknum].parameter.type)
						{
						case AttackKind::UP:
							//上段攻撃
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
							//中段攻撃
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
							//下段攻撃
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
							//投げ攻撃
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
							//叩きつけ攻撃
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
							//飛び道具攻撃
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
							//ホーミングダッシュ
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
							//相殺しない攻撃
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
							//上段攻撃と相殺しない攻撃
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
							//ダウンする攻撃
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
							//ガードが成功した
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
							//攻撃がガードされたことを保存する
							attack[atknum].hit_result = HitResult::GUARD;
							return add;
						}
						break;
						case HitResultState::HIT:
						{
							//被弾した

							/*if (hei == 0)
							{
								hei++;
								return 0.0f;
							}
							hei++;*/

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

							//エフェクト生成
							YRGetEffect().PlayEffect(EffectKind::SWORD, effectpos.GetDXFLOAT3(), DirectX::XMFLOAT3(8.0f, 8.0f, 8.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);

							//SE再生
							GetSound().SESinglePlay(SEKind::HIT_SWORD);
							//SE再生
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
								//SE再生
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
							//攻撃が当たったことを保存する
							attack[atknum].hit_result = HitResult::HIT;
							hit[hitnum].hit_state = HitStateKind::NORMAL;
							//PlaySE(SE_HIT);
							return add;
						}
						break;
						case HitResultState::AVOIDANCE:
						{
							//無敵で回避した
							return 0.0f;
						}
						break;
						case HitResultState::STATE_NONE:
						{
							//掴まれた
							hit[hitnum].hit_state = HitStateKind::STEAL;
							hit[hitnum].steal_timer = attack[atknum].parameter.stealtimer;
							hit[hitnum].hit = true;

							//SE再生
							GetSound().SESinglePlay(SEKind::HIT);
							//攻撃が当たったことを保存する
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
							//叩き落された
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

							//エフェクト再生
							YRGetEffect().PlayEffect(EffectKind::SWORD, effectpos.GetDXFLOAT3(), DirectX::XMFLOAT3(8.0f, 8.0f, 8.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);

							//SE再生
							GetSound().SESinglePlay(SEKind::HIT_SWORD);
							//SE再生
							GetSound().SESinglePlay(SEKind::HIT);

							for (int n = 0; n < attack.size(); n++)
							{
								attack[n].parameter.damege = 0;
								//attack[n].HB_timer = 0;
								attack[n].parameter.hitback = YR_Vector3(0.0f, 0.0f);
								attack[n].hit_ok = false;
							}
							//攻撃が当たったことを保存する

							if (pos.y > slam_up_line)
							{
								//高さが一定以上だった場合、叩きつけ攻撃に移行しカメラズームを行う
								hit[hitnum].hit_state = HitStateKind::SLAM;
								Hitcheak::timer = 0.45f;
								YRCamera.RequestCamera(player);
								//SE再生
								GetSound().SESinglePlay(SEKind::UP_ATTACK);
							}
							else if (pos.y > slam_zoom_line)
							{
								//高さが一定以上だった場合、通常の攻撃でカメラズームを行う
								hit[hitnum].hit_state = HitStateKind::NORMAL;
								Hitcheak::timer = 0.45f;
								YRCamera.RequestCamera(player);
								//SE再生
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
							//アーマーで耐えた
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

							//エフェクト再生
							YRGetEffect().PlayEffect(EffectKind::SWORD, effectpos.GetDXFLOAT3(), DirectX::XMFLOAT3(8.0f, 8.0f, 8.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);

							//SE再生
							GetSound().SESinglePlay(SEKind::HIT_SWORD);
							//SE再生
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
								//SE再生
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
							//攻撃が当たったことを保存する
							attack[atknum].hit_result = HitResult::HIT;
							hit[hitnum].hit_state = HitStateKind::ARMOR;
							//PlaySE(SE_HIT);
							return add;
						}
						break;
						case HitResultState::DOWN_HIT:
						{
							//ダウン攻撃
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

							//エフェクト再生
							YRGetEffect().PlayEffect(EffectKind::SWORD, effectpos.GetDXFLOAT3(), DirectX::XMFLOAT3(8.0f, 8.0f, 8.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);

							//SE再生
							GetSound().SESinglePlay(SEKind::HIT_SWORD);
							//SE再生
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
								//SE再生
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
							//攻撃が当たったことを保存する
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

void Hitcheak::HitPlayer(std::vector <HitBox>& pl1, float& pos1, std::vector <HitBox>& pl2, float& pos2, float elapsed_time)
{
	//体の当たり判定は1で固定する為、1だけで判定する
	int p1_h = 1;
	int p2_h = 1;

	if (pl1[p1_h].center.x - pl1[p1_h].parameter.size.x<pl2[p2_h].center.x + pl2[p2_h].parameter.size.x &&
		pl1[p1_h].center.x + pl1[p1_h].parameter.size.x>pl2[p2_h].center.x - pl2[p2_h].parameter.size.x)
	{
		if (pl1[p1_h].center.y - pl1[p1_h].parameter.size.y<pl2[p2_h].center.y + pl2[p2_h].parameter.size.y &&
			pl1[p1_h].center.y + pl1[p1_h].parameter.size.y>pl2[p2_h].center.y - pl2[p2_h].parameter.size.y)
		{
			//pl1[0].hitother = true;
			//pl2[0].hitother = true;
			float moving_correction = (pl1[p1_h].parameter.size.x / 2.0f) + (pl2[p2_h].parameter.size.x / 2.0f);
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

			FRAMEWORK.scenegame.player1p->StopHitParamUpdate();
			FRAMEWORK.scenegame.player2p->StopHitParamUpdate();
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

							HitResultAttack result = HitResultAttack::OFFSET;


							//1P側のステート確認
							switch (atk1p.parameter.type)
							{
							case AttackKind::LOCK:
								//ロック技
								continue;
								break;
							case AttackKind::PROJECTILE:
								//飛び道具
								break;
							case AttackKind::TRACK:
								//ホーミングダッシュ
								if (atk2p.parameter.type == AttackKind::PROJECTILE)
								{
									//飛び道具には一方的に打ち勝つ
									result = HitResultAttack::P2_OFFSET;
								}
								break;
							case AttackKind::NO_TO_OFFSET:
								//相殺しない攻撃
								if (atk2p.parameter.type != AttackKind::NO_TO_OFFSET)
								{
									//相殺攻撃以外とは相殺しない
									continue;
								}
								break;
							case AttackKind::NO_TO_OFFSET_UP:
								//上段攻撃とは相殺しない攻撃
								if (atk2p.parameter.type == AttackKind::UP||
									atk2p.parameter.type == AttackKind::TRACK ||
									atk2p.parameter.type == AttackKind::SLAM)
								{
									//上段、ホーミングダッシュまたは叩きつけ攻撃とは相殺しない
									continue;
								}
								break;
							case AttackKind::DOWN_ATTACK:
								//ダウン攻撃
								if (atk2p.parameter.type != AttackKind::DOWN_ATTACK)
								{
									if (atk2p.parameter.type == AttackKind::PROJECTILE)
									{
										//飛び道具には一方的に打ち勝つ
										result = HitResultAttack::P2_OFFSET;
									}
									else
									{
										//ダウン攻撃以外とは相殺しない
										continue;
									}
								}
								break;
							default:
								result = HitResultAttack::OFFSET;
								break;
							}

							//2P側のステート確認
							switch (atk2p.parameter.type)
							{
							case AttackKind::LOCK:
								//ロック技
								continue;
								break;
							case AttackKind::PROJECTILE:
								//飛び道具
								break;
							case AttackKind::TRACK:
								//ホーミングダッシュ
								if (atk1p.parameter.type == AttackKind::PROJECTILE)
								{
									//飛び道具には一方的に打ち勝つ
									result = HitResultAttack::P1_OFFSET;
								}
								break;
							case AttackKind::NO_TO_OFFSET:
								//相殺しない攻撃
								if (atk1p.parameter.type != AttackKind::NO_TO_OFFSET)
								{
									//相殺攻撃以外とは相殺しない
									continue;
								}
								break;
							case AttackKind::NO_TO_OFFSET_UP:
								//上段攻撃とは相殺しない攻撃
								if (atk1p.parameter.type == AttackKind::UP ||
									atk1p.parameter.type == AttackKind::TRACK ||
									atk1p.parameter.type == AttackKind::SLAM)
								{
									//上段、ホーミングダッシュまたは叩きつけ攻撃とは相殺しない
									continue;
								}
								break;
							case AttackKind::DOWN_ATTACK:
								//ダウン攻撃
								if (atk1p.parameter.type != AttackKind::DOWN_ATTACK)
								{
									if (atk1p.parameter.type == AttackKind::PROJECTILE)
									{
										//飛び道具には一方的に打ち勝つ
										result = HitResultAttack::P1_OFFSET;
									}
									else
									{
										//ダウン攻撃以外とは相殺しない
										continue;
									}
								}
								break;
							default:
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
								//相殺
							{
								add1P = atk1p.parameter.damege;
								add2P = atk2p.parameter.damege;
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

								//エフェクト再生
								YRGetEffect().PlayEffect(EffectKind::OFFSET, clashpos.GetDXFLOAT3(), DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);

								//SE再生
								GetSound().SESinglePlay(SEKind::OFFSET);

								atk1p.parameter.damege = 0;
								atk1p.hit_ok = false;
								atk1p.knock_start = true;
								//attack[n].HB_timer = 0;
								atk1p.parameter.hitback = YR_Vector3(0.0f, 0.0f);

								atk2p.parameter.damege = 0;
								//attack[n].HB_timer = 0;
								atk2p.parameter.hitback = YR_Vector3(0.0f, 0.0f);
								atk2p.hit_ok = false;
								atk2p.knock_start = true;

								atk1p.hit_result = HitResult::GUARD;
								atk2p.hit_result = HitResult::GUARD;
							}
								break;
							case HitResultAttack::P1_OFFSET:
								//1Pだけ相殺
							{
								add1P = 0.0f;
								add2P = atk2p.parameter.damege;
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

								//エフェクト再生
								YRGetEffect().PlayEffect(EffectKind::OFFSET, clashpos.GetDXFLOAT3(), DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);

								//SE再生
								GetSound().SESinglePlay(SEKind::OFFSET);

								atk1p.parameter.damege = 0;
								atk1p.knock_start = true;
								atk1p.parameter.hitback = YR_Vector3(0.0f, 0.0f);
								atk1p.hit_ok = false;

								atk1p.hit_result = HitResult::GUARD;
							}
								break;
							case HitResultAttack::P2_OFFSET:
								//2Pだけ相殺
							{
								add1P = atk1p.parameter.damege;
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

								//エフェクト再生
								YRGetEffect().PlayEffect(EffectKind::OFFSET, clashpos.GetDXFLOAT3(), DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);

								//SE再生
								GetSound().SESinglePlay(SEKind::OFFSET);

								atk2p.parameter.damege = 0;
								//attack[n].HB_timer = 0;
								atk2p.parameter.hitback = YR_Vector3(0.0f, 0.0f);
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