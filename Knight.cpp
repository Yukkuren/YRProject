#include "Knight.h"
#include "PlayerBase.h"
#include "Key.h"
#include "YRGamePad.h"
#include "camera.h"
#include "framework.h"
#include <algorithm>
#include "World.h"
#include "Effect.h"
#include "YRSound.h"

//#define _CRTDBG_MAP_ALLOC						// mallocによるメモリリーク検出でCPPファイル名と行数出力指定
//#define DBG_NEW new( _NORMAL_BLOCK , __FILE__ , __LINE__)	// new によるメモリリーク検出でCPPファイル名と行数出力指定
//#include <stdio.h>
//#include <crtdbg.h>


Knight::~Knight() = default;

void Knight::Init(YR_Vector3 InitPos)
{
	//ImGui::Begin("RYU HitBox");
	pos = InitPos;
	scale = YR_Vector3( 0.1f,0.1f,0.1f );
	//scale = YR_Vector3( 0.05f,0.05f,0.05f );
	//angle = YR_Vector3(DirectX::XMConvertToRadians(-90.0f), 0.0f, 0.0f);
	angle = YR_Vector3(0.0f, 0.0f, 0.0f);

	fream = non_target;
	timer = non_target;
	later = non_target;

	speed.x = 0;
	speed.y = 0.0f;
	jumpcount = 2;
	air_dash_count = 1;
	max_jump_flag = false;
	hp = 1000;
	gravity = 40.0f;
	down_force = 200.0f;
	fall_force = 100.0f;
	knocktimer = 0.0f;
	ground = true;
	jumpflag = false;
	drawset = false;
	fast = false;
	finish = false;
	stop_pos = { 0.0f,0.0f };
	step = false;
	hightrigger = false;
	trackgauge = 2;
	gauge = 0;
	power = 0;
	steal_escape = 0;
	speed_X.Set(0.0f);
	speed_Y.Set(0.0f);
	combo_count = 0;
	intro_state = INTRO_KNIGHT::SET;
	win_state = WIN_PERFORMANCE_KNIGHT::SET;

	fream = 0.0f;

	production_time = 0.0f;
	anim_ccodinate = 2.0f;

	stop_state = 0;
	air_dash_state = AirDashState::NONE;
	power = 1;

	light_direction = DirectX::XMFLOAT4(-1.0f, -0.1, 1.0f, 0.0f);
#pragma region HITBOXINIT
	for (int act = 0; act < hit.size(); act++)
	{
		hit[act].Init();
	}
#pragma endregion

	hadouspeed = 0.0f;
	moveflag = false;
	attack = FALSE;
	attack_state = AttackState::NONE;
	last_attack = AttackState::NONE;
	act_state = ActState::NONE;
	later = non_target;
	hadou = { pos.x + Getapply(100),pos.y };

	eye_offset = { 0.0f,0.0f };
	mouth_offset = { 0.0f,0.0f };

	face_anim = FaceAnim::NORMAL;
	face_wink_time = 0.0f;
	face_wink_interval_timer = 0.0f;
	wink_state = Wink_State::FIRST;
	jump_can_timer = 0.0f;
	lip_sync_time = 0.0f;

	camera_state_knight = CAMERA_STATE_KNIGHT::FIRST;
	intro_timer = 0.0f;
	win_timer = 0.0f;

	eye_plus = YR_Vector3(0.0f, 0.0f, -10.0f);
	focus_plus = YR_Vector3(0.0f, 0.0f, 0.0f);
	face_mouth_num = FaceMouth_Num::NORMAL_MOUSE;
	passive_timer = 0.0f;
	ChangeFace(FaceAnim::NORMAL_LIP_SYNC);

	text_on = false;

	lumi_material = Model::Material_Attribute::NONE;


	if (now_player == 1)
	{
		anim->NodeChange(model_motion.intro_R);
	}
	else
	{
		anim->NodeChange(model_motion.intro_L);
	}

	cut_in_timer = 0.0f;
}

void Knight::Uninit()
{
	//モデル系を全て解放する
	main.reset();
	main = nullptr;
	anim.reset();
	anim = nullptr;
	model_motion.wait_R.reset();
	model_motion.wait_R = nullptr;
	model_motion.special_R.reset();
	model_motion.special_R = nullptr;
	model_motion.damage_R_g_u.reset();
	model_motion.damage_R_g_u = nullptr;
	model_motion.guard_R.reset();
	model_motion.guard_R = nullptr;
	model_motion.slid_R.reset();
	model_motion.slid_R = nullptr;
	model_motion.air_back_R.reset();
	model_motion.air_back_R = nullptr;
	model_motion.air_dash_R.reset();
	model_motion.air_dash_R = nullptr;
	model_motion.passive_R.reset();
	model_motion.passive_R = nullptr;
	model_motion.squat_R.reset();
	model_motion.squat_R = nullptr;
	model_motion.walk_R.reset();
	model_motion.walk_R = nullptr;
	model_motion.back_R.reset();
	model_motion.back_R = nullptr;
	model_motion.dash_R.reset();
	model_motion.dash_R = nullptr;
	model_motion.backstep_R.reset();
	model_motion.backstep_R = nullptr;
	model_motion.jump_R.reset();
	model_motion.jump_R = nullptr;
	model_motion.air_jump_R.reset();
	model_motion.air_jump_R = nullptr;
	model_motion.intro_R.reset();
	model_motion.intro_R = nullptr;
	model_motion.win_R.reset();
	model_motion.win_R = nullptr;

	model_motion.wait_L.reset();
	model_motion.wait_L = nullptr;
	model_motion.special_L.reset();
	model_motion.special_L = nullptr;
	model_motion.damage_L_g_u.reset();
	model_motion.damage_L_g_u = nullptr;
	model_motion.guard_L.reset();
	model_motion.guard_L = nullptr;
	model_motion.slid_L.reset();
	model_motion.slid_L = nullptr;
	model_motion.air_back_L.reset();
	model_motion.air_back_L = nullptr;
	model_motion.air_dash_L.reset();
	model_motion.air_dash_L = nullptr;
	model_motion.passive_L.reset();
	model_motion.passive_L = nullptr;
	model_motion.squat_L.reset();
	model_motion.squat_L = nullptr;
	model_motion.walk_L.reset();
	model_motion.walk_L = nullptr;
	model_motion.back_L.reset();
	model_motion.back_L = nullptr;
	model_motion.dash_L.reset();
	model_motion.dash_L = nullptr;
	model_motion.backstep_L.reset();
	model_motion.backstep_L = nullptr;
	model_motion.jump_L.reset();
	model_motion.jump_L = nullptr;
	model_motion.air_jump_L.reset();
	model_motion.air_jump_L = nullptr;
	model_motion.intro_L.reset();
	model_motion.intro_L = nullptr;
	model_motion.win_L.reset();
	model_motion.win_L = nullptr;


	model_motion.jaku_R.reset();
	model_motion.thu_R.reset();
	model_motion.kyo_R.reset();
	model_motion.d_jaku_R.reset();
	model_motion.d_thu_R.reset();
	model_motion.u_kyo_R.reset();
	model_motion.a_jaku_R.reset();
	model_motion.a_thu_R.reset();
	model_motion.a_kyo_R.reset();
	model_motion.a_ukyo_R.reset();
	model_motion.steal_R.reset();
	model_motion.slow_R.reset();
	model_motion.track_R.reset();
	model_motion.jaku_rh_R.reset();
	model_motion.thu_rh_R.reset();
	model_motion.kyo_rh_R.reset();
	model_motion.jaku_lh_R.reset();
	model_motion.thu_lh_R.reset();
	model_motion.kyo_lh_R.reset();
	model_motion.disire_s_R.reset();
	model_motion.disire_m_R.reset();
	model_motion.extend_R.reset();

	model_motion.jaku_R = nullptr;
	model_motion.thu_R = nullptr;
	model_motion.kyo_R = nullptr;
	model_motion.d_jaku_R = nullptr;
	model_motion.d_thu_R = nullptr;
	model_motion.u_kyo_R = nullptr;
	model_motion.a_jaku_R = nullptr;
	model_motion.a_thu_R = nullptr;
	model_motion.a_kyo_R = nullptr;
	model_motion.a_ukyo_R = nullptr;
	model_motion.steal_R = nullptr;
	model_motion.slow_R = nullptr;
	model_motion.track_R = nullptr;
	model_motion.jaku_rh_R = nullptr;
	model_motion.thu_rh_R = nullptr;
	model_motion.kyo_rh_R = nullptr;
	model_motion.jaku_lh_R = nullptr;
	model_motion.thu_lh_R = nullptr;
	model_motion.kyo_lh_R = nullptr;
	model_motion.disire_s_R = nullptr;
	model_motion.disire_m_R = nullptr;
	model_motion.extend_R = nullptr;


	model_motion.jaku_L.reset();
	model_motion.thu_L.reset();
	model_motion.kyo_L.reset();
	model_motion.d_jaku_L.reset();
	model_motion.d_thu_L.reset();
	model_motion.u_kyo_L.reset();
	model_motion.a_jaku_L.reset();
	model_motion.a_thu_L.reset();
	model_motion.a_kyo_L.reset();
	model_motion.a_ukyo_L.reset();
	model_motion.steal_L.reset();
	model_motion.slow_L.reset();
	model_motion.track_L.reset();
	model_motion.jaku_rh_L.reset();
	model_motion.thu_rh_L.reset();
	model_motion.kyo_rh_L.reset();
	model_motion.jaku_lh_L.reset();
	model_motion.thu_lh_L.reset();
	model_motion.kyo_lh_L.reset();
	model_motion.disire_s_L.reset();
	model_motion.disire_m_L.reset();
	model_motion.extend_L.reset();

	model_motion.jaku_L = nullptr;
	model_motion.thu_L = nullptr;
	model_motion.kyo_L = nullptr;
	model_motion.d_jaku_L = nullptr;
	model_motion.d_thu_L = nullptr;
	model_motion.u_kyo_L = nullptr;
	model_motion.a_jaku_L = nullptr;
	model_motion.a_thu_L = nullptr;
	model_motion.a_kyo_L = nullptr;
	model_motion.a_ukyo_L = nullptr;
	model_motion.steal_L = nullptr;
	model_motion.slow_L = nullptr;
	model_motion.track_L = nullptr;
	model_motion.jaku_rh_L = nullptr;
	model_motion.thu_rh_L = nullptr;
	model_motion.kyo_rh_L = nullptr;
	model_motion.jaku_lh_L = nullptr;
	model_motion.thu_lh_L = nullptr;
	model_motion.kyo_lh_L = nullptr;
	model_motion.disire_s_L = nullptr;
	model_motion.disire_m_L = nullptr;
	model_motion.extend_L = nullptr;

	model_motion.model_R.clear();
	model_motion.model_L.clear();

	atk.clear();
	projectile_atk.clear();
	hit.clear();
	attack_list.clear();
	hitparam_list.clear();
}

void Knight::Update(float decision, float elapsed_time)
{
	//elapsed_timeが0、つまりヒットストップなどでゲームが止まっている場合
	if (elapsed_time == 0.0f)
	{
		//早期returnで処理に入らないようにする
		return;
	}
	finish = false;
	DamageCheck(decision);
	WaitAnimSet();

	HitResultUpdate();

	if (pos.y <= POS_Y)
	{
		ground = true;
		speed_Y.Set(0.0f);
	}
	else
	{
		ground = false;
	}


	//-----------------------------------------------------
	// *概要*
	//・攻撃を受けるなど操作不能の状態のみステートで区分けし、
	//　それ以外は操作を受け付けるのでCANACTで処理している
	//-----------------------------------------------------
	switch (act_state)
	{
	case ActState::STATENONE:
		//ステートを奪われた状態。相手に掴まれてる
		StateNone(elapsed_time);
		break;
	case ActState::WAKE:
		//ダウンから起き上がる
		WakeUp();
		break;
	case ActState::PASSIVE:
		//受け身をとる
		PassiveUpdate(elapsed_time);
		break;
	case ActState::DOWN:
		//ダウン状態
		DownUpdate();
		break;
	case ActState::DOWN_HIT:
		//ダウンヒット状態
		DownHitUpdate(elapsed_time);
		break;
	case ActState::FALL:
		//空中でダウンしている状態
		FallUpdate(elapsed_time);
		break;
	case ActState::KNOCK:
		//攻撃を受けてのけぞる
		KnockUpdate(elapsed_time);
		break;
	case ActState::SLAM:
		//叩きつけられ状態
		SlamUpdate(elapsed_time);
		break;
	case ActState::ATTACK:
		//攻撃中
		Attack(decision, elapsed_time);
		break;
	case ActState::GUARD:
		Guard(decision);
		GuardBack(elapsed_time);
		Squat();
		break;
	default:
		//その他個別処理

		//角度を戻す
		angle.z = 0.0f;
		if (ground)
		{
			//地面設置時
			//pos.y = POS_Y;	//地面に設置しているときは高さを固定する
			trackgauge = Track_max;	//ホーミングダッシュ回数を戻す
			if (act_state == ActState::JUMP)
			{
				//ステートがジャンプの時はNONEに戻す
				act_state = ActState::NONE;
			}
		}
		else
		{
			//空中時
			if (pos.y > POS_Y)
			{
				//重力を付与する
				pos.y -= gravity*elapsed_time;
			}
		}

		{
		//攻撃を行っていないときの処理
			if (ground && !step)
			{
				jumpcount = 2;
				air_dash_count = 1;
				if (rightOrleft != decision)
				{
					//前回フレームと今回の向きが異なった場合
					if (act_state == ActState::WAIT)
					{
						if (decision > 0)
						{
							//右向きの時
							anim->NodeChange(model_motion.wait_R);
							anim_ccodinate = ac_act[scastI(act_state)].fream;
						}
						else
						{
							//左向きの時
							anim->NodeChange(model_motion.wait_L);
							anim_ccodinate = ac_act[scastI(act_state)].fream;
						}
					}
				}
				rightOrleft = decision;
				if (speed.x > 0.0f)
				{
					speed.x -= brake_speed*elapsed_time;
					if (speed.x < 0.0f)
					{
						speed.x = 0;
					}
				}
				if (speed.x < 0.0f)
				{
					speed.x += brake_speed * elapsed_time;
					if (speed.x > 0.0f)
					{
						speed.x = 0.0f;
					}
				}
			}
			//しゃがみやガードなどを先に判定
			Squat();
			Guard(decision);
			//ガードでない時
			if (act_state != ActState::GUARD)
			{
				Step(elapsed_time);
				if (step)
				{
					//ステップ中は処理しない
				}
				else
				{

					//地面についている場合のみ
					if (ground)
					{
						if (!moveflag)
						{
							MoveAnimSet();
						}
						if (moveflag)
						{
							Move(rightOrleft);
						}
					}

					AttackInput();
				}

				if (!attack)
				{
					Jump();
				}

			}
			NoneChange();

		}
		break;
	}

	if (finish)
	{
		pad->pre_input = true;
	}
	else
	{
		pad->pre_input = false;
	}

	AttackUpdate(elapsed_time);	//攻撃判定の更新

	JumpUpdate(decision,elapsed_time);
	AirDash(elapsed_time);

	pos.x += ((speed_X.Update(elapsed_time) * elapsed_time) * rightOrleft);
	pos.x += (speed.x * elapsed_time);

	speed.y += (speed_Y.Update(elapsed_time) * elapsed_time);
	pos.y += (speed.y * elapsed_time);

	for (int list = 0; list < hit.size(); list++)
	{
		if (act_state == ActState::ATTACK)
		{
			switch (attack_state)
			{
			case AttackState::COMBO_X:
			{
				AttackState truth = combolist_X.combolist[combolist_X.now_pos];
				int truth_num = scastI(truth);
				AttackState real_state = attack_list[truth_num].real_attack;
				hit[list].Update(pos, hitparam_list[list].attack_parameter[scastI(real_state)], rightOrleft, elapsed_time);
			}
				break;
			case AttackState::COMBO_Y:
			{
				AttackState truth = combolist_Y.combolist[combolist_Y.now_pos];
				int truth_num = scastI(truth);
				AttackState real_state = attack_list[truth_num].real_attack;
				hit[list].Update(pos, hitparam_list[list].attack_parameter[scastI(real_state)], rightOrleft, elapsed_time);
			}
				break;
			case AttackState::COMBO_B:
			{
				AttackState truth = combolist_B.combolist[combolist_B.now_pos];
				int truth_num = scastI(truth);
				AttackState real_state = attack_list[truth_num].real_attack;
				hit[list].Update(pos, hitparam_list[list].attack_parameter[scastI(real_state)], rightOrleft, elapsed_time);
			}
				break;
			case AttackState::VERSATILE_ATTACK:
			{
				AttackState truth = attack_list[scastI(AttackState::VERSATILE_ATTACK)].real_attack;
				int truth_num = scastI(truth);
				hit[list].Update(pos, hitparam_list[list].attack_parameter[truth_num], rightOrleft, elapsed_time);
			}
			break;
			case AttackState::A_VERSATILE_ATTACK:
			{
				AttackState truth = attack_list[scastI(AttackState::A_VERSATILE_ATTACK)].real_attack;
				int truth_num = scastI(truth);
				hit[list].Update(pos, hitparam_list[list].attack_parameter[truth_num], rightOrleft, elapsed_time);
			}
			break;
			default:
				hit[list].Update(pos, hitparam_list[list].attack_parameter[scastI(attack_state)], rightOrleft, elapsed_time);
				break;
			}
		}
		else
		{
			hit[list].Update(pos, hitparam_list[list].act_parameter[scastI(act_state)], rightOrleft, elapsed_time);
		}
	}

	GuardAnimSet();

	if (!attack)
	{
		if (act_state == ActState::DASH || act_state == ActState::BACK)
		{
			//特定の行動時は指定した値を入れる為なにも処理をしない
		}
		else
		{
			anim_ccodinate = ac_act[scastI(act_state)].fream;
		}
	}
	/*if (attack_state != AttackState::D_THU && act_state != ActState::DOWN && act_state != ActState::SQUAT && attack_state != AttackState::D_JAKU)
	{
		hit[scastI(KNIGHTHIT::BODY)].Init(pos, YR_Vector3(0.0f, 0.0f), YR_Vector3(2.0f, 2.9f));
		hit[scastI(KNIGHTHIT::LEG)].Init(pos, YR_Vector3(0.0f, 0.0f), YR_Vector3(1.4f, 0.8f));
	}

	hit[scastI(KNIGHTHIT::BODY)].Update(pos,elapsed_time);
	hit[scastI(KNIGHTHIT::LEG)].Update(pos, elapsed_time);*/


	EndAttackErase();			//攻撃判定の消去

	FaceAnimation(elapsed_time);
}


void Knight::StopHitParamUpdate()
{
	for (int list = 0; list < hit.size(); list++)
	{
		if (act_state == ActState::ATTACK)
		{
			switch (attack_state)
			{
			case AttackState::COMBO_X:
			{
				AttackState truth = combolist_X.combolist[combolist_X.now_pos];
				int truth_num = scastI(truth);
				AttackState real_state = attack_list[truth_num].real_attack;
				hit[list].Update(pos, hitparam_list[list].attack_parameter[scastI(real_state)], rightOrleft, 0.0f);
			}
			break;
			case AttackState::COMBO_Y:
			{
				AttackState truth = combolist_Y.combolist[combolist_Y.now_pos];
				int truth_num = scastI(truth);
				AttackState real_state = attack_list[truth_num].real_attack;
				hit[list].Update(pos, hitparam_list[list].attack_parameter[scastI(real_state)], rightOrleft, 0.0f);
			}
			break;
			case AttackState::COMBO_B:
			{
				AttackState truth = combolist_B.combolist[combolist_B.now_pos];
				int truth_num = scastI(truth);
				AttackState real_state = attack_list[truth_num].real_attack;
				hit[list].Update(pos, hitparam_list[list].attack_parameter[scastI(real_state)], rightOrleft, 0.0f);
			}
			break;
			case AttackState::VERSATILE_ATTACK:
			{
				AttackState truth = attack_list[scastI(AttackState::VERSATILE_ATTACK)].real_attack;
				int truth_num = scastI(truth);
				hit[list].Update(pos, hitparam_list[list].attack_parameter[truth_num], rightOrleft, 0.0f);
			}
			break;
			case AttackState::A_VERSATILE_ATTACK:
			{
				AttackState truth = attack_list[scastI(AttackState::A_VERSATILE_ATTACK)].real_attack;
				int truth_num = scastI(truth);
				hit[list].Update(pos, hitparam_list[list].attack_parameter[truth_num], rightOrleft, 0.0f);
			}
			break;
			default:
				hit[list].Update(pos, hitparam_list[list].attack_parameter[scastI(attack_state)], rightOrleft, 0.0f);
				break;
			}
		}
		else
		{
			hit[list].Update(pos, hitparam_list[list].act_parameter[scastI(act_state)], rightOrleft, 0.0f);
		}
	}
}


void Knight::AttackInput()
{

	//-------------------------------------------------------------------
	// *概要*
	//・コマンドは二種類のみ
	//--------------------------------------------------------------------
	//攻撃のリストの入力判定を全て確認する(0は攻撃無しの為、1から確認していく)
	for (int list = 1; list < attack_list.size(); list++)
	{
		int button = scastI(attack_list[list].linkage_button);
		Command now_com = attack_list[list].linkage_command;
		if (pad->x_input[button] == 1)
		{
			if (!pad->que.empty())
			{
				pad->que.back().timer = 0;
			}
			//コマンド判定
			Command command = pad->com_list.CommandCheack(button, pad->input_history.data(), rightOrleft);
			if (now_com == command)
			{
				//指定したコマンドが同じで空中、地上の条件も同じだった場合
				if (ground)
				{
					//現在地上にいる場合は攻撃が空中専用じゃない場合は行う
					if (attack_list[list].ground_on < Ground_C::GROUND)
					{
						continue;
					}
				}
				else
				{
					//現在空中にいる場合は攻撃が地上専用じゃない場合は行う
					if (attack_list[list].ground_on == Ground_C::GROUND)
					{
						continue;
					}
				}

				if (attack_list[list].squat_on)
				{
					//攻撃リストにしゃがみ判定が設定されていた場合、現在のステートがしゃがみか確認する
					if (act_state != ActState::SQUAT && pad->x_input[scastI(PAD::STICK_D)] == 0)
					{
						if (pad->x_input[scastI(PAD::STICK_LDown)] == 0 && pad->x_input[scastI(PAD::STICK_RDown)] == 0)
						{
							continue;
						}
					}
				}
				else
				{
					//設定されていない場合はしゃがんでないか確認する
					if (act_state == ActState::SQUAT || pad->x_input[scastI(PAD::STICK_D)] > 0 ||
						pad->x_input[scastI(PAD::STICK_LDown)] > 0 || pad->x_input[scastI(PAD::STICK_RDown)] > 0)
					{
						if (static_cast<AttackState>(list) != AttackState::TRACK_DASH&&
							static_cast<AttackState>(list) != AttackState::VERSATILE_ATTACK&&
							static_cast<AttackState>(list) != AttackState::A_VERSATILE_ATTACK)
						{
							//ホーミングダッシュとプレイヤー選択技の場合はしゃがんでても出せるように判定する
							continue;
						}
					}
				}
				if (attack_list[list].linkage_stick != PAD::BUTTOM_END)
				{
					if (static_cast<AttackState>(list) != AttackState::TRACK_DASH &&
						static_cast<AttackState>(list) != AttackState::VERSATILE_ATTACK &&
						static_cast<AttackState>(list) != AttackState::A_VERSATILE_ATTACK)
					{
						//スティックの入力が指定されている場合確認する
						if (attack_list[list].linkage_stick == PAD::STICK_D)
						{
							//下スティック入力の場合は別の判定もとる
							if (pad->x_input[scastI(PAD::STICK_D)] == 0 && pad->x_input[scastI(PAD::STICK_RDown)] == 0 && pad->x_input[scastI(PAD::STICK_LDown)] == 0)
							{
								continue;
							}
						}
						else
						{
							if (pad->x_input[scastI(attack_list[list].linkage_stick)] == 0)
							{
								continue;
							}
						}
					}
				}
				//実際の攻撃内容
				int real = scastI(attack_list[list].real_attack);
				if (attack_list[real].combo == last_attack)
				{
					if (last_attack == AttackState::COMBO_X || last_attack == AttackState::COMBO_Y || last_attack == AttackState::COMBO_B)
					{
						//コンボ状態に入っている場合はコンボ関数で攻撃のステートを決定する
						ComboUpdate();
						return;
					}
				}

				if (attack_list[real].attack_name == AttackState::TRACK_DASH && trackgauge < 1)
				{
					continue;
				}

				//int next = scastI(attack_list[list].real_attack);
				if (attack_list[real].need_power <= power)
				{
					//ゲージの必要量を確認する
					//通常の攻撃の場合、実際の攻撃内容は入れずに攻撃名をそのまま入れる
					attack_state = static_cast<AttackState>(list);
					if (attack_list[real].need_power > 0)
					{
						//ゲージを消費する攻撃だった場合はゲージを減らす
						power -= attack_list[real].need_power;
					}
				}
				else
				{
					//ゲージが足りない場合指定した技を出す
					attack_state = attack_list[real].aid_attack_name;
					real = scastI(attack_list[scastI(attack_list[real].aid_attack_name)].real_attack);
				}

				//最後に行った攻撃が同じだった場合、コンボを確認して存在するならコンボを開始する
				if (last_attack == attack_state)
				{
					if (ComboSet())
					{
						return;
					}
					//attack_state = attack_list[real].next_attack;
					//next = scastI(attack_list[list].next_attack);
				}


				//攻撃を決定する
				//pad->com_list.Reset();
				//現在攻撃判定が出ているなら全て消去する
				AllAttackClear();
				//この攻撃をキャンセルするための条件を保存する
				atk_result = attack_list[real].conditions_hit;
				//攻撃を保存する
				last_attack = attack_state;
				//攻撃の結果を初期化
				hit_result = HitResult::NOT_OCCURRENCE;
				//攻撃中フラグをオンに
				attack = true;
				//移動フラグをオフに
				moveflag = false;
				//行動ステートを攻撃に
				act_state = ActState::ATTACK;
				//発生フレームを決定
				fream = attack_list[real].attack_single[0].fream;
				//アニメーション速度を指定
				anim_ccodinate = ac_attack[real].fream;
				//攻撃番号を初期化
				attack_list[real].now_attack_num = 0;
				//角度を元に戻す
				angle.y = 0.0f;
				angle.z = 0.0f;
				//当たり判定を初期化
				if (attack_list[real].attack_single[0].parameter[0].type == AttackKind::NO_TO_OFFSET_UP)
				{
					HitBoxTransition(HitBoxState::UP_INVINCIBLE);
				}
				else if (attack_list[real].attack_single[0].parameter[0].type == AttackKind::NO_TO_OFFSET)
				{
					HitBoxTransition(HitBoxState::INVINCIBLE);
				}
				else
				{
					HitBoxTransition(HitBoxState::NOGUARD);
				}
				//攻撃発生前の前進距離を設定する(地上攻撃のみ)
				if (attack_list[real].ground_on == Ground_C::GROUND)
				{
					speed_X.Set(attack_list[real].advance_speed);
				}
				if (!ground)
				{
					//ジャンプフラグも設定しておく
					jumpflag = true;
					max_jump_flag = true;
				}

				//後隙を初期化
				later = non_target;
				//持続を初期化
				timer = non_target;
				//カメラ処理用変数を初期化
				production_time = 0.0f;
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.model_R[real], scastI(AnimAtk::FREAM));
				}
				else
				{
					anim->NodeChange(model_motion.model_L[real], scastI(AnimAtk::FREAM));
				}
				//攻撃ごとに個別の設定を行う
				AttackDetailsSet(attack_list[real].combo);
				return;
			}
		}
	}
}

void Knight::AttackSwitch(float decision, float elapsed_time, AttackState attack_state)
{
	AttackState now_attack_state;

	if (attack_state == AttackState::NONE)
	{
		now_attack_state = this->attack_state;
	}
	else
	{
		now_attack_state = attack_state;
	}

	switch (now_attack_state)
	{
	case AttackState::NONE:
		break;
	case AttackState::SLOW:
		//投げ
		Slow(elapsed_time);
		break;
	case AttackState::STEAL:
		//相手のステートを奪う。掴み
		Steal(elapsed_time);
		break;
	case AttackState::JAKU:
		//弱攻撃
		Jaku(elapsed_time);
		break;
	case AttackState::THU:
		//中攻撃(下中)
		D_Thu(elapsed_time);
		break;
	case AttackState::D_KYO:
		//中の次に出る強攻撃
		Kyo(elapsed_time);
		break;
	case AttackState::D_JAKU:
		//下段弱攻撃
		D_Jaku(elapsed_time);
		break;
	case AttackState::D_THU:
		//下段中攻撃
		D_Thu(elapsed_time);
		break;
	case AttackState::U_KYO:
		//上段強攻撃
		U_Kyo(elapsed_time);
		break;
	case AttackState::A_JAKU:
		//空中弱攻撃
		A_Jaku(elapsed_time);
		break;
	case AttackState::A_THU:
		//空中中攻撃
		A_Thu(elapsed_time);
		break;
	case AttackState::A_KYO:
		//空中強攻撃
		A_Kyo(elapsed_time);
		break;
	case AttackState::A_UKYO:
		//空中上強攻撃
		A_UKyo(elapsed_time);
		break;
	case AttackState::JAKU_RHURF:
		//飛び道具(弱)
		Jaku_Rhurf(elapsed_time);
		break;
	case AttackState::THU_RHURF:
		//飛び道具(中)
		Thu_Rhurf(elapsed_time);
		break;
	case AttackState::KYO_RHURF:
		Kyo_Rhurf(elapsed_time);
		break;
	case AttackState::TRACK_DASH:
		//ホーミングダッシュ
		TrackDash(decision, elapsed_time);
		break;
	case AttackState::KYO_LHURF:
		Kyo_Lhurf(elapsed_time);
		break;
	case AttackState::EXTENDATK:
		ExtendATK(elapsed_time);
		break;
	case AttackState::JAKU_LHURF:
		Jaku_Lhurf(elapsed_time);
		break;
	case AttackState::THU_LHURF:
		Thu_Lhurf(elapsed_time);
		break;
	case AttackState::SPECIAL_ATTACK:
		SpecialAttack(elapsed_time);
		break;
	case AttackState::JAKU_THU:
		//弱の次に出る中攻撃
		Thu(elapsed_time);
		break;
	case AttackState::JAKU_KYO:
		//中の次に出る強攻撃
		Kyo(elapsed_time);
		break;
	case AttackState::A_JAKU_LHURF:
		A_Jaku_Lhurf(elapsed_time);
		break;
	case AttackState::COMBO_X:
		ComboX(decision,elapsed_time);
		break;
	case AttackState::COMBO_Y:
		ComboY(decision, elapsed_time);
		break;
	case AttackState::COMBO_B:
		ComboB(decision, elapsed_time);
		break;
	case AttackState::VERSATILE_ATTACK:
		AttackSwitch(decision, elapsed_time, attack_list[scastI(now_attack_state)].real_attack);
		break;
	case AttackState::A_VERSATILE_ATTACK:
		AttackSwitch(decision, elapsed_time, attack_list[scastI(now_attack_state)].real_attack);
		break;
	default:
		break;
	}
}



void Knight::Attack(float decision, float elapsed_time)
{
	if (ground)
	{
		if (speed.x > 0.0f)
		{
			speed.x -= brake_speed * elapsed_time;
			if (speed.x < 0.0f)
			{
				speed.x = 0;
			}
		}
		if (speed.x < 0.0f)
		{
			speed.x += brake_speed * elapsed_time;
			if (speed.x > 0.0f)
			{
				speed.x = 0.0f;
			}
		}
	}
	//空中時
	if (pos.y > POS_Y && speed_Y.speed == 0.0f)
	{
		//上方向への力がない場合、重力を付与する
		pos.y -= gravity * elapsed_time;
	}

	AttackSwitch(decision, elapsed_time);

	//攻撃中、攻撃が当たった場合キャンセルして攻撃する
	//前回の攻撃時にどの条件でキャンセルできるかを保存してるため、それ以上ならキャンセルして攻撃する
	if (hit_result >= atk_result)
	{
		AttackInput();
		Jump();
	}

	if (later > 0 && later < target_max)
	{
		later -= elapsed_time;
		//CancelList();
	}

	//後隙消費後元のステートに戻す
	if (later < 0.0f)
	{
		lumi_material = Model::Material_Attribute::NONE;
		finish = true;
		later = non_target;
		attack = false;
		//結果を初期化する
		hit_result = HitResult::NOT_OCCURRENCE;
		//キャンセル条件を初期化する
		atk_result = HitResult::NONE;
		//最終入力内容を初期化する
		last_attack = AttackState::NONE;
		angle.y = 0.0f;

		if (ground)
		{
			if (act_state != ActState::WAIT)
			{

				if (pad->x_input[scastI(PAD::STICK_D)] > 0 || pad->x_input[scastI(PAD::STICK_RDown)] > 0 || pad->x_input[scastI(PAD::STICK_LDown)] > 0)
				{
					act_state = ActState::SQUAT;
					anim_ccodinate = ac_act[scastI(ActState::SQUAT)].fream;
					if (rightOrleft > 0)
					{
						anim->NodeChange(model_motion.squat_R);
					}
					else
					{
						anim->NodeChange(model_motion.squat_L);
					}

				}
				else
				{
					act_state = ActState::NONE;
					anim_ccodinate = ac_act[scastI(ActState::WAIT)].fream;
					if (rightOrleft > 0)
					{
						anim->NodeChange(model_motion.wait_R);
					}
					else
					{
						anim->NodeChange(model_motion.wait_L);
					}
				}
				attack_state = AttackState::NONE;
				ChangeFace(FaceAnim::NORMAL);
			}
		}
		else
		{
			act_state = ActState::JUMP;
			attack_state = AttackState::NONE;
			//描画をセット
			anim_ccodinate = ac_act[scastI(ActState::JUMP)].timer;
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::TIMER));
			}
			//max_jump_flag = true;
		}
	}
}



void Knight::Draw(
	YRShader				*parallel_shader,
	YRShader				*shader,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color,
	float						elapsed_time)
{

	float game_speed = elapsed_time;

	if (steal_escape > 0.0f && steal_escape < target_max)
	{
		game_speed = 0.0f;
	}

	light_direction.x = -rightOrleft;

	DrawFastMove(FastPos);
	drawset = false;

	bool invincible = false;

	if (attack_state == AttackState::EXTENDATK)
	{
		/*for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
		{
			if (hit[i].parameter.state == HitBoxState::INVINCIBLE)
			{
				invincible = true;
			}
		}*/
	}
	for (int i = 0; i < hit.size(); i++)
	{
		if (hit[i].state == HitBoxState::INVINCIBLE)
		{
			invincible = true;
		}
	}

	//--------------------------------------------------------------------
	// *メモ*
	//・過去2Dで制作したときは向いている向きによって描画を変えていたが
	//　今回は3Dでの制作になる為、angleを変えることに使う
	//・前回では強攻撃時の画像のサイズが通常と違っていたため
	//　こういう処理を取らざるを得なかったと考えられる
	//--------------------------------------------------------------------

	bool inversion = true;//左右反転フラグ

	//Blenderのモデルのサブセットには0.8のmaterial_colorが入っているため逆数を入れている
	DirectX::XMFLOAT4 material_color = { 1.1f,1.1f,1.1f,1.0f };


	//左向き
	if (rightOrleft < 0)
	{
		//angle.y = DirectX::XMConvertToRadians(180.0f);
		inversion = true;
		if (invincible)
		{
			//無敵状態
			lumi_material = Model::Material_Attribute::ALL;
		}
		else if (lumi_material == Model::Material_Attribute::ALL)
		{
			//無敵じゃない
			lumi_material = Model::Material_Attribute::NONE;
		}

	}
	//右向き
	else
	{
		//angle.y = DirectX::XMConvertToRadians(0.0f);

		/*motion.DrawContinue(
			parallel_shader,
			pos.GetDXFLOAT3(),
			scale.GetDXFLOAT3(),
			angle.GetDXFLOAT3(),
			view, projection, light_direction, light_color, ambient_color, elapsed_time * anim_ccodinate,
			inversion, material_color
		);

		anim->UpdateAnimation(elapsed_time * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(DirectX::XMFLOAT3(pos.x,pos.y,pos.z), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		anim->Draw(parallel_shader, view, projection, light_direction, light_color, ambient_color, material_color);*/

		//angle.y = DirectX::XMConvertToRadians(80.0f);
		inversion = false;
		if (invincible)
		{
			//無敵！
			lumi_material = Model::Material_Attribute::ALL;
		}
		else if (lumi_material == Model::Material_Attribute::ALL)
		{
			lumi_material = Model::Material_Attribute::NONE;
		}
	}

	//モデル描画
	/*base_motion->Render(
		shader,
		pos.GetDXFLOAT3(),
		scale.GetDXFLOAT3(),
		angle.GetDXFLOAT3(),
		view, projection, light_direction, light_color, ambient_color, elapsed_time, 0.0f);*/


	//カメラの状態によって描画が変更されるため個別で分けている

	if (scastI(YRCamera.camera_state) == now_player)
	{
		anim->UpdateAnimation(game_speed * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)],lumi_material, material_color);
		anim->CalculateWorldTransform(DirectX::XMFLOAT3(pos.x,-5.0f,pos.z+2.0f), DirectX::XMFLOAT3(scale.x, 0.001f, scale.z), DirectX::XMFLOAT3(0.0f,angle.y,0.0f));
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)], Model::Material_Attribute::NONE, DirectX::XMFLOAT4( 0.0f,0.0f,0.0f,1.0f ));
	}
	if (YRCamera.camera_state == Camera::CAMERA_STATE::MAIN)
	{
		anim->UpdateAnimation(game_speed * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		anim->Draw(parallel_shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)],lumi_material, material_color);
		anim->CalculateWorldTransform(DirectX::XMFLOAT3(pos.x, - 5.0f, pos.z + 2.0f), DirectX::XMFLOAT3(scale.x, 0.001f, scale.z), DirectX::XMFLOAT3(0.0f, angle.y, 0.0f));
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)], Model::Material_Attribute::NONE, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	if (YRCamera.GetRequest() == Camera::Request::WEAKEN)
	{
		anim->UpdateAnimation(game_speed* anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)],lumi_material, material_color);
		anim->CalculateWorldTransform(DirectX::XMFLOAT3(pos.x, - 5.0f, pos.z + 2.0f), DirectX::XMFLOAT3(scale.x, 0.001f, scale.z), DirectX::XMFLOAT3(0.0f, angle.y, 0.0f));
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)], Model::Material_Attribute::NONE, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}
	if (YRCamera.camera_state == Camera::CAMERA_STATE::ZOOM_CAMERA)
	{
		anim->UpdateAnimation(game_speed * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)],lumi_material, material_color);
		anim->CalculateWorldTransform(DirectX::XMFLOAT3(pos.x, - 5.0f, pos.z + 2.0f), DirectX::XMFLOAT3(scale.x, 0.001f, scale.z), DirectX::XMFLOAT3(0.0f, angle.y, 0.0f));
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)], Model::Material_Attribute::NONE, DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));
	}

	/*motion.DrawContinue(
			parallel_shader,
			pos.GetDXFLOAT3(),
			scale.GetDXFLOAT3(),
			angle.GetDXFLOAT3(),
			view, projection, light_direction, light_color, ambient_color, elapsed_time*anim_ccodinate,
			inversion, material_color
		);*/

	//if (atk[scastI(KNIGHTATK::HADOU)].hit_ok)
	//{
	//	//波動拳の描画
	//}

	//if (atk[scastI(KNIGHTATK::THU_HADOU)].hit_ok)
	//{
	//	//中波動拳の描画。これまとめてもいいだろ
	//}

	//if (atk[scastI(KNIGHTATK::KYO_HADOU)].hit_ok)
	//{
	//	//でかい波動拳の描画。これは大きさ違うから仕方ない
	//}

	TextDraw();

//デバッグ状態なら
#ifdef EXIST_IMGUI
		//プレイヤーの中心
		if (ground)
		{
		}
		else
		{
		}

		for (int i = 0; i < 10; i++)
		{
			int j = i * 20 + 20;
		}

		//入力関係を表示してた
		//GetJoypadXInputState(DX_INPUT_PAD1, &input);

		//if (GetJoypadXInputState(DX_INPUT_PAD1, &input) != -1)
		//{
		//	// 画面に XINPUT_STATE の中身を描画
		//	Color = GetColor(255, 255, 255);
		//	GetJoypadAnalogInput(&Xbuf, &Ybuf, DX_INPUT_PAD1);
		//	DrawFormatString(0, 0, Color, "LeftTrigger:%d RightTrigger:%d",
		//		input.LeftTrigger, input.RightTrigger);
		//	DrawFormatString(0, 16, Color, "ThumbLX:%d ThumbLY:%d",
		//		Xbuf, Ybuf);
		//	DrawFormatString(0, 32, Color, "ThumbRX:%d ThumbRY:%d",
		//		input.ThumbRX, input.ThumbRY);
		//	DrawString(0, 64, "Button", Color);
		//	for (i = 0; i < 16; i++)
		//	{
		//		DrawFormatString(64 + i % 8 * 64, 64 + i / 8 * 16, Color,
		//			"%2d:%d", i, input.Buttons[i]);
		//	}
		//}

		{
			/*bool show_test_window = true;
			bool show_another_window = true;*/
			//static float f = 0.0f;
			//ImGui::Begin("RyuHitBox");
			/*ImGui::InputFloat("BodyPosX", &Hitplus[scastI(KNIGHTHIT::BODY)].x, 0.1f, 0.1f);
			ImGui::InputFloat("BodyPosY", &Hitplus[scastI(KNIGHTHIT::BODY)].y, 0.1f, 0.1f);
			ImGui::InputFloat("BodySizeX", &hit[scastI(KNIGHTHIT::BODY)].size.x, 0.1f, 0.1f);
			ImGui::InputFloat("BodySizeY", &hit[scastI(KNIGHTHIT::BODY)].size.y, 0.1f, 0.1f);

			ImGui::InputFloat("LegPosX", &Hitplus[scastI(KNIGHTHIT::LEG)].x, 0.1f, 0.1f);
			ImGui::InputFloat("LegPosY", &Hitplus[scastI(KNIGHTHIT::LEG)].y, 0.1f, 0.1f);
			ImGui::InputFloat("LegSizeX", &hit[scastI(KNIGHTHIT::LEG)].size.x, 0.1f, 0.1f);
			ImGui::InputFloat("LegSizeY", &hit[scastI(KNIGHTHIT::LEG)].size.y, 0.1f, 0.1f);*/
			//ImGui::SliderFloat("camera.y", &camera.y, -world_max_y, world_max_y);
			//ImGui::SliderFloat("camera.x", &camera.x, -world_max_x, world_max_x);
			//ImGui::Text("player.y:%f", pos.y);
			//ImGui::Text("player.x:%f", pos.x);

			//ImGui::ColorEdit3("clear color", (float*)&ImColor(114, 144, 154));
			//if (ImGui::Button("Test Window")) show_test_window ^= 1;
			//if (ImGui::Button("Another Window")) show_another_window ^= 1;
			//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			//ImGui::End();
		}
#endif

}

void Knight::DrawCutIn(
	YRShader* shader,
	float elapsed_time
)
{
	float center_x = static_cast<float>(FRAMEWORK.SCREEN_WIDTH) / 2.0f;
	float center_y = static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) / 2.0f;

	float fream = 0.03f;
	float size = 3.0f;

	bool anim_fin = cutFrame->AnimFinCheck();

	if (anim_fin)
	{
		cut_in_timer += elapsed_time;
		{
			//通常の描画(最後のアニメーションを描画し続ける)
			cutFrame->DrawRotaDivGraph(
				shader,
				center_x,
				center_y,
				0.0f,
				size,
				cutFrame->max - 1,
				SpriteMask::FRAME
			);

			cutMask->DrawRotaDivGraph(
				shader,
				center_x,
				center_y,
				0.0f,
				size,
				cutMask->max - 1,
				SpriteMask::WRITE
			);
		}
	}
	else
	{
		if(cut_in_timer < cut_in_max_time)
		{
			//カットイン再生
			bool finish_cut = cutFrame->DrawRotaDivGraphOnec(
				shader,
				center_x,
				center_y,
				0.0f,
				size,
				fream,
				elapsed_time,
				SpriteMask::FRAME
			);

			cutMask->DrawRotaDivGraphOnec(
				shader,
				center_x,
				center_y,
				0.0f,
				size,
				fream,
				elapsed_time,
				SpriteMask::WRITE
			);
		}
	}

	//一定の時間以上になったら逆再生を開始する
	if (cut_in_timer > cut_in_max_time)
	{
		bool finish_cut = cutFrame->DrawRotaDivGraphReverse(
			shader,
			center_x,
			center_y,
			0.0f,
			size,
			fream,
			elapsed_time,
			SpriteMask::FRAME
		);

		cutMask->DrawRotaDivGraphReverse(
			shader,
			center_x,
			center_y,
			0.0f,
			size,
			fream,
			elapsed_time,
			SpriteMask::WRITE
		);
	}


	cutIn->DrawRotaGraph(
		shader,
		center_x,
		center_y,
		0.0f,
		3.0f,
		false,
		SpriteMask::INDRAW
	);
}



//残像の描画
void Knight::DrawFastMove(YR_Vector3 position)
{
	if (!fast)
	{
		return;
	}
	//右
	if (rightOrleft < 0)
	{

	}
	//左
	else
	{

	}
	fast = false;
}

//残像をセットする
void Knight::FastSet(YR_Vector3 position)
{
	fast = true;
	FastPos = position;
}


//移動関係(ステップなど)
//-----------------------------------------------------------------
// *概要*
//・地上の場合、バックステップの処理を行う(前方へはダッシュだけの為)
//・空中の場合は空中ダッシュの設定を行い、処理は別関数で行う
//-----------------------------------------------------------------
bool Knight::Step(float elapsed_time)
{
	//空中
	if (!ground)
	{
		//空中左ステップ
		if (pad->x_input[scastI(PAD::L_TRIGGER)] == 1)
		{
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				//スティックを左に倒している時
				if (air_dash_count == 1)
				{
					//ダッシュカウントが残っている場合は空中ダッシュを行う
					//step = true;
					moveflag = false;
					jumpflag = false;
					jumpcount = 0;
					air_dash_count = 0;
					GetSound().SESinglePlay(SEKind::HIGH_JUMP);
					//スピードはどっちも同じ
					speed.x = -stepspeed;
					if (rightOrleft > 0)
					{
						//空中バックダッシュ
						act_state = ActState::AIR_B;
						air_dash_state = AirDashState::AIR_B;
						//描画をセット
						anim->NodeChange(model_motion.air_back_R, scastI(AnimAtk::FREAM));
						anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
						anim_ccodinate = ac_act[scastI(ActState::AIR_B)].fream;
					}
					else
					{
						//空中前ダッシュ
						act_state = ActState::AIR_F;
						air_dash_state = AirDashState::AIR_F;
						//描画をセット
						anim->NodeChange(model_motion.air_dash_L, scastI(AnimAtk::FREAM));
						anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
						anim_ccodinate = ac_act[scastI(ActState::AIR_F)].fream;
					}
				}
				else
				{
					//ジャンプがない場合はトリガーを外す
					pad->dash_trigger = false;
				}
			}
			else if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				//スティックを右に倒している時
				if (air_dash_count == 1)
				{
					//ダッシュカウントが残っている場合は空中ダッシュを行う
					//step = true;
					moveflag = false;
					jumpflag = false;
					jumpcount = 0;
					air_dash_count = 1;
					//スピードはどっちも同じ
					speed.x = stepspeed;
					GetSound().SESinglePlay(SEKind::HIGH_JUMP);
					if (rightOrleft > 0)
					{
						//空中前ダッシュ
						act_state = ActState::AIR_F;
						air_dash_state = AirDashState::AIR_F;
						//描画をセット
						anim->NodeChange(model_motion.air_dash_R, scastI(AnimAtk::FREAM));
						anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
						anim_ccodinate = ac_act[scastI(ActState::AIR_F)].fream;
					}
					else
					{
						//空中バックダッシュ
						act_state = ActState::AIR_B;
						air_dash_state = AirDashState::AIR_B;
						//描画をセット
						anim->NodeChange(model_motion.air_back_L, scastI(AnimAtk::FREAM));
						anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
						anim_ccodinate = ac_act[scastI(ActState::AIR_B)].fream;
					}
				}
				else
				{
					//ジャンプがない場合はトリガーを外す
					pad->dash_trigger = false;
				}
			}
			else
			{
				if (air_dash_count == 1)
				{
					//ダッシュカウントが残っている場合は空中ダッシュを行う
					//step = true;
					moveflag = false;
					jumpflag = false;
					jumpcount = 0;
					air_dash_count = 1;
					GetSound().SESinglePlay(SEKind::HIGH_JUMP);
					if (rightOrleft > 0)
					{
						//空中前ダッシュ
						speed.x = stepspeed;
						act_state = ActState::AIR_F;
						air_dash_state = AirDashState::AIR_F;
						//描画をセット
						anim->NodeChange(model_motion.air_dash_R, scastI(AnimAtk::FREAM));
						anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
						anim_ccodinate = ac_act[scastI(ActState::AIR_F)].fream;
					}
					else
					{
						//空中前ダッシュ
						//スピードはどっちも同じ
						speed.x = -stepspeed;
						act_state = ActState::AIR_F;
						air_dash_state = AirDashState::AIR_F;
						//描画をセット
						anim->NodeChange(model_motion.air_dash_L, scastI(AnimAtk::FREAM));
						anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
						anim_ccodinate = ac_act[scastI(ActState::AIR_F)].fream;
					}
				}
				else
				{
					//ジャンプがない場合はトリガーを外す
					pad->dash_trigger = false;
				}
			}
		}

		//空中左ステップ
		if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 1)
		{
			if (air_dash_count == 1)
			{
				//ダッシュカウントが残っている場合は空中ダッシュを行う
				//step = true;
				moveflag = false;
				jumpflag = false;
				jumpcount = 0;
				air_dash_count = 0;
				GetSound().SESinglePlay(SEKind::HIGH_JUMP);
				//スピードはどっちも同じ
				speed.x = -stepspeed;
				if (rightOrleft > 0)
				{
					//空中バックダッシュ
					act_state = ActState::AIR_B;
					air_dash_state = AirDashState::AIR_B;
					//描画をセット
					anim->NodeChange(model_motion.air_back_R, scastI(AnimAtk::FREAM));
					anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
					anim_ccodinate = ac_act[scastI(ActState::AIR_B)].fream;
				}
				else
				{
					//空中前ダッシュ
					act_state = ActState::AIR_F;
					air_dash_state = AirDashState::AIR_F;
					//描画をセット
					anim->NodeChange(model_motion.air_dash_L, scastI(AnimAtk::FREAM));
					anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
					anim_ccodinate = ac_act[scastI(ActState::AIR_F)].fream;
				}
			}
			else
			{
				//ジャンプがない場合はトリガーを外す
				pad->dash_trigger = false;
			}
		}

		//空中右ステップ
		if (pad->x_input[static_cast<int>(PAD::R_DASH)] == 1)
		{
			if (air_dash_count == 1)
			{
				//ダッシュカウントが残っている場合は空中ダッシュを行う
				//step = true;
				moveflag = false;
				jumpflag = false;
				jumpcount = 0;
				air_dash_count = 1;
				//スピードはどっちも同じ
				speed.x = stepspeed;
				GetSound().SESinglePlay(SEKind::HIGH_JUMP);
				if (rightOrleft > 0)
				{
					//空中前ダッシュ
					act_state = ActState::AIR_F;
					air_dash_state = AirDashState::AIR_F;
					//描画をセット
					anim->NodeChange(model_motion.air_dash_R, scastI(AnimAtk::FREAM));
					anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
					anim_ccodinate = ac_act[scastI(ActState::AIR_F)].fream;
				}
				else
				{
					//空中バックダッシュ
					act_state = ActState::AIR_B;
					air_dash_state = AirDashState::AIR_B;
					//描画をセット
					anim->NodeChange(model_motion.air_back_L, scastI(AnimAtk::FREAM));
					anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
					anim_ccodinate = ac_act[scastI(ActState::AIR_B)].fream;
				}
			}
			else
			{
				//ジャンプがない場合はトリガーを外す
				pad->dash_trigger = false;
			}
		}
	}

	if (!step)
	{
		return false;
	}

	//バックステップ
	if (act_state == ActState::BACK)
	{
		//右向き
		if (rightOrleft > 0)
		{
			//右向きのバックステップはx方向の速度は0になる
			//なので全て0にしている
			if (speed.x > 0.0f)
			{
				step = false;
				speed.x = 0.0f;
				pad->dash_trigger = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				moveflag = false;
				act_state = ActState::NONE;
				lumi_material = Model::Material_Attribute::NONE;
				return true;
			}
			if (speed.x < (-backstepS / 5.0f))
			{
				HitBoxTransition(HitBoxState::INVINCIBLE);
			}

			if (!anim->GetLoopAnim())
			{
				//現在のアニメーションがバックステップの開始アニメーションだった場合
				if (anim->GetEndAnim() == -1)
				{
					//アニメーションが終了したら持続アニメーションに切り替える
					if (rightOrleft > 0)
					{
						anim->NodeChange(model_motion.backstep_R, scastI(AnimAtk::TIMER));
					}
					else
					{
						anim->NodeChange(model_motion.backstep_L, scastI(AnimAtk::TIMER));
					}
					anim_ccodinate = ac_act[scastI(act_state)].timer;
				}
			}
			else
			{
				//現在のアニメーションがバックステップの持続アニメーションだった場合
				if (speed.x > -backstepS)
				{
					if (rightOrleft > 0)
					{
						anim->NodeChange(model_motion.backstep_R, scastI(AnimAtk::LATER));
					}
					else
					{
						anim->NodeChange(model_motion.backstep_L, scastI(AnimAtk::LATER));
					}
					anim->PlayAnimation(scastI(AnimAtk::LATER), false);
					anim_ccodinate = ac_act[scastI(act_state)].later;
				}
			}
			//描画をセットはしないけど移動範囲のセット
			if (pos.x < Limit::Left_max)
			{
				pos.x = Limit::Left_max;
			}
			speed.x += (backstepD * elapsed_time);
			return false;
		}
		if (rightOrleft < 0)
		{
			//逆も然り
			if (speed.x < 0.0f)
			{
				step = false;
				speed.x = 0.0f;
				pad->dash_trigger = false;
				moveflag = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				act_state = ActState::NONE;
				return true;
			}
			if (speed.x > (backstepS / 5.0f))
			{
				HitBoxTransition(HitBoxState::INVINCIBLE);
			}
			//描画をセットはしないけど移動範囲のセット
			if (pos.x > Limit::Right_max)
			{
				pos.x = Limit::Right_max;
			}
			if (!anim->GetLoopAnim())
			{
				//現在のアニメーションがバックステップの開始アニメーションだった場合
				if (anim->GetEndAnim() == -1)
				{
					//アニメーションが終了したら持続アニメーションに切り替える
					if (rightOrleft > 0)
					{
						anim->NodeChange(model_motion.backstep_R, scastI(AnimAtk::TIMER));
					}
					else
					{
						anim->NodeChange(model_motion.backstep_L, scastI(AnimAtk::TIMER));
					}
					anim_ccodinate = ac_act[scastI(act_state)].timer;
				}
			}
			else
			{
				//現在のアニメーションがバックステップの持続アニメーションだった場合
				if (speed.x > backstepS)
				{
					if (rightOrleft > 0)
					{
						anim->NodeChange(model_motion.backstep_R, scastI(AnimAtk::LATER));
					}
					else
					{
						anim->NodeChange(model_motion.backstep_L, scastI(AnimAtk::LATER));
					}
					anim->PlayAnimation(scastI(AnimAtk::LATER), false);
					anim_ccodinate = ac_act[scastI(act_state)].later;
				}
			}
			speed.x -= (backstepD * elapsed_time);
			return false;
		}
	}
	else
	{
	step = false;
 }

	return false;
}


//空中ダッシュ処理
//-----------------------------------------------------------------
// *概要*
//・空中ダッシュの処理を行う
//・AirDashStateによって処理を変える
//-----------------------------------------------------------------
void Knight::AirDash(float elapsed_time)
{
	switch (air_dash_state)
	{
	case AirDashState::NONE:
		//何もしない
		break;
	case AirDashState::AIR_F:
	{
		//空中ダッシュ
		speed.y = 0.0f;
		speed_X.Set(0.0f);
		speed_Y.Set(0.0f);
		//重力の逆数を付与する
		pos.y += gravity * elapsed_time;
		if (rightOrleft > 0)
		{
			if (speed.x < dashspeed)
			{
				step = false;
				pad->dash_trigger = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				moveflag = false;
				air_dash_count = 0;
				air_dash_state = AirDashState::NONE;
				if (!attack)
				{
					//描画をセット
					act_state = ActState::JUMP;
					anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::TIMER));
					anim_ccodinate = ac_act[scastI(act_state)].timer;
				}
				return;
			}
			//移動範囲のセット
			if (pos.x > Limit::Right_max)
			{
				pos.x = Limit::Right_max;
			}

			if (!attack)
			{
				//攻撃中はアニメーションの変更を行わない
				if (!anim->GetLoopAnim())
				{
					//現在のアニメーションが空中ダッシュの開始アニメーションだった場合
					if (anim->GetEndAnim() == -1)
					{
						//アニメーションが終了したら持続アニメーションに切り替える
						anim->NodeChange(model_motion.air_dash_R, scastI(AnimAtk::TIMER));
						anim_ccodinate = ac_act[scastI(act_state)].timer;
					}
				}
				else
				{
					//現在のアニメーションが空中ダッシュの持続アニメーションだった場合
					if (speed.x < stepspeed)
					{
						anim->NodeChange(model_motion.air_dash_R, scastI(AnimAtk::LATER));
						anim->PlayAnimation(scastI(AnimAtk::LATER), false);
						anim_ccodinate = ac_act[scastI(act_state)].later;
					}
				}
			}

			speed.x -= (stepD * elapsed_time);
			return;
		}
		if (rightOrleft < 0)
		{
			if (speed.x > -dashspeed)
			{
				step = false;
				pad->dash_trigger = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				air_dash_state = AirDashState::NONE;
				moveflag = false;
				air_dash_count = 0;
				if (!attack)
				{
					//描画をセット
					act_state = ActState::JUMP;
					anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::TIMER));
					anim_ccodinate = ac_act[scastI(act_state)].timer;
				}
				return;
			}
			//移動範囲のセット
			if (pos.x < Limit::Left_max)
			{
				pos.x = Limit::Left_max;
			}

			if (!attack)
			{
				//攻撃中はアニメーションの変更を行わない
				if (!anim->GetLoopAnim())
				{
					//現在のアニメーションが空中ダッシュの開始アニメーションだった場合
					if (anim->GetEndAnim() == -1)
					{
						//アニメーションが終了したら持続アニメーションに切り替える
						anim->NodeChange(model_motion.air_dash_L, scastI(AnimAtk::TIMER));
						anim_ccodinate = ac_act[scastI(act_state)].timer;
					}
				}
				else
				{
					//現在のアニメーションが空中ダッシュの持続アニメーションだった場合
					if (speed.x > -stepspeed)
					{
						anim->NodeChange(model_motion.air_dash_L, scastI(AnimAtk::LATER));
						anim->PlayAnimation(scastI(AnimAtk::LATER), false);
						anim_ccodinate = ac_act[scastI(act_state)].later;
					}
				}
			}

			speed.x += (stepD * elapsed_time);
			return;
		}
	}
		break;
	case AirDashState::AIR_B:
	{
		//空中バックステップ
		speed.y = 0.0f;
		speed_X.Set(0.0f);
		speed_Y.Set(0.0f);
		//重力の逆数を付与する
		pos.y += gravity * elapsed_time;
		if (rightOrleft > 0)
		{
			//右向き
			//スピードがダッシュ以下になったら終了
			if (speed.x > -dashspeed)
			{
				step = false;
				pad->dash_trigger = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				air_dash_state = AirDashState::NONE;
				moveflag = false;
				air_dash_count = 0;
				//描画をセット
				if (!attack)
				{
					act_state = ActState::JUMP;
					anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::TIMER));
					anim_ccodinate = ac_act[scastI(act_state)].timer;
				}
				return;
			}
			//移動範囲のセット
			if (pos.x < Limit::Left_max)
			{
				pos.x = Limit::Left_max;
			}

			if (!attack)
			{
				//攻撃中はアニメーションの変更を行わない
				if (!anim->GetLoopAnim())
				{
					//現在のアニメーションが空中バックステップの開始アニメーションだった場合
					if (anim->GetEndAnim() == -1)
					{
						//アニメーションが終了したら持続アニメーションに切り替える
						anim->NodeChange(model_motion.air_back_R, scastI(AnimAtk::TIMER));
						anim_ccodinate = ac_act[scastI(act_state)].timer;
					}
				}
				else
				{
					//現在のアニメーションが空中バックステップの持続アニメーションだった場合
					if (speed.x > -stepspeed)
					{
						anim->NodeChange(model_motion.air_back_R, scastI(AnimAtk::LATER));
						anim->PlayAnimation(scastI(AnimAtk::LATER), false);
						anim_ccodinate = ac_act[scastI(act_state)].later;
					}
				}
			}

			speed.x += (stepD * elapsed_time);

			return;
		}
		if (rightOrleft < 0)
		{
			//左向き
			//スピードがダッシュ以下になったら終了
			if (speed.x < dashspeed)
			{
				step = false;
				pad->dash_trigger = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				air_dash_state = AirDashState::NONE;
				moveflag = false;
				air_dash_count = 0;
				//描画をセット
				if (!attack)
				{
					act_state = ActState::JUMP;
					anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::TIMER));
					anim_ccodinate = ac_act[scastI(act_state)].timer;
				}
				return;
			}
			//移動範囲のセット
			if (pos.x > Limit::Right_max)
			{
				pos.x = Limit::Right_max;
			}

			if (!attack)
			{
				//アニメーション遷移
				//攻撃中はアニメーションの変更を行わない
				if (!anim->GetLoopAnim())
				{
					//現在のアニメーションが空中バックステップの開始アニメーションだった場合
					if (anim->GetEndAnim() == -1)
					{
						//アニメーションが終了したら持続アニメーションに切り替える
						anim->NodeChange(model_motion.air_back_L, scastI(AnimAtk::TIMER));
						anim_ccodinate = ac_act[scastI(act_state)].timer;
					}
				}
				else
				{
					//現在のアニメーションが空中バックステップの持続アニメーションだった場合
					if (speed.x < stepspeed)
					{
						anim->NodeChange(model_motion.air_back_L, scastI(AnimAtk::LATER));
						anim->PlayAnimation(scastI(AnimAtk::LATER), false);
						anim_ccodinate = ac_act[scastI(act_state)].later;
					}
				}
			}

			speed.x -= (stepD * elapsed_time);
			return;
		}
	}
		break;
	case AirDashState::ACT_END:
		break;
	default:
		break;
	}
}

//移動関係(こちらはステップ以外)
void Knight::Move(float decision)
{
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 1&& pad->x_input[scastI(PAD::STICK_R)]==0)
	{
		if (ground)
		{

			if (rightOrleft > 0)
			{
				//右向きバックステップ
				step = true;
				act_state = ActState::BACK;
				moveflag = false;
				GetSound().SESinglePlay(SEKind::BACKSTEP);
				//描画をセット
				anim->NodeChange(model_motion.backstep_R, scastI(AnimAtk::FREAM));
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
				anim_ccodinate = ac_act[scastI(ActState::BACK)].fream;
				speed.x = -backstepS;
			}
			else
			{
				//ダッシュ左向き
				//ダッシュの遷移は別で行うため消去
				//act_state = ActState::DASH;
				//anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::FREAM));
				//anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
				//anim_ccodinate = ac_act[scastI(act_state)].fream;
			}
		}
	}
	if (pad->x_input[static_cast<int>(PAD::R_DASH)] == 1&& pad->x_input[scastI(PAD::STICK_L)]==0)
	{
		if (ground)
		{
			//描画をセット

			if (rightOrleft < 0)
			{
				//左向きバックステップ
				step = true;
				act_state = ActState::BACK;
				moveflag = false;
				GetSound().SESinglePlay(SEKind::BACKSTEP);
				//描画をセット
				anim->NodeChange(model_motion.backstep_L, scastI(AnimAtk::FREAM));
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
				anim_ccodinate = ac_act[scastI(ActState::BACK)].fream;
				speed.x = backstepS;
			}
			else
			{
				//ダッシュ右向き
				//ダッシュの遷移は別で行うため消去
				//act_state = ActState::DASH;
				//anim->NodeChange(model_motion.dash_R,scastI(AnimAtk::FREAM));
				//anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
				//anim_ccodinate = ac_act[scastI(act_state)].fream;
			}
		}
	}


	//左移動
	if (pad->x_input[scastI(PAD::STICK_L)] > 0)
	{
		if (!step)
		{
			if (rightOrleft < 0)
			{
				//左向き
				speed.x = -dashspeed;
				act_state = ActState::DASH;
				if (!anim->GetLoopAnim())
				{
					//現在のアニメーションがダッシュの開始アニメーションだった場合
					if (anim->GetEndAnim() == -1)
					{
						//アニメーションが終了したら持続アニメーションに切り替える
						anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::TIMER));
						anim_ccodinate = ac_act[scastI(act_state)].timer;
					}
				}
			}
			else
			{
				speed.x = -walkspeed;
				act_state = ActState::RETREAT;
			}
		}
		//if (rightOrleft > 0)
		//{

		//}
		//else
		//{
		//	if (!step)
		//	{
		//		speed.x = -dashspeed;
		//		act_state = ActState::DASH;
		//		if (!anim->GetLoopAnim())
		//		{
		//			//現在のアニメーションがダッシュの開始アニメーションだった場合
		//			if (anim->GetEndAnim()==-1)
		//			{
		//				//アニメーションが終了したら持続アニメーションに切り替える
		//				anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::TIMER));
		//				anim_ccodinate = ac_act[scastI(act_state)].timer;
		//			}
		//		}
		//	}
		//}
	}
	else
	{
		/*if (pad->x_input[scastI(PAD::STICK_L)] > 0)
		{
			speed.x = -walkspeed;
			act_state = ActState::MOVEL;
		}*/
	}




	//右移動
	if (pad->x_input[scastI(PAD::STICK_R)] > 0)
	{
		if (!step)
		{
			if (rightOrleft > 0)
			{
				//右向き
				speed.x = dashspeed;
				act_state = ActState::DASH;
				if (!anim->GetLoopAnim())
				{
					//現在のアニメーションがダッシュの開始アニメーションだった場合
					if (anim->GetEndAnim() == -1)
					{
						//アニメーションが終了したら持続アニメーションに切り替える
						anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::TIMER));
						anim_ccodinate = ac_act[scastI(act_state)].timer;
					}
				}
			}
			else
			{
				//左向き
				speed.x = walkspeed;
				act_state = ActState::RETREAT;
			}
		}
		//if (rightOrleft < 0)
		//{
		//
		//}
		//else
		//{
		//	if (!step)
		//	{
		//		speed.x = dashspeed;
		//		act_state = ActState::DASH;
		//		if (!anim->GetLoopAnim())
		//		{
		//			//現在のアニメーションがダッシュの開始アニメーションだった場合
		//			if (anim->GetEndAnim()==-1)
		//			{
		//				//アニメーションが終了したら持続アニメーションに切り替える
		//				anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::TIMER));
		//				anim_ccodinate = ac_act[scastI(act_state)].timer;
		//			}
		//		}
		//	}
		//}
	}
	else
	{
		/*if (pad->x_input[scastI(PAD::STICK_R)] > 0)
		{
			speed.x = walkspeed;
			act_state = ActState::MOVER;
		}*/
	}

	MoveStop();
}



void Knight::MoveStop()
{
	//移動停止時処理
	//以前は後隙を設定していたが操作の都合上削除

	//スティックを何も倒していない場合
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] == 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] == 0)
	{
		if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 0 && pad->x_input[static_cast<int>(PAD::R_DASH)] == 0)
		{
			speed.x = 0.0f;
			act_state = ActState::NONE;
			moveflag = false;
		}
	}

	//スティックを両方とも倒している場合
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		speed.x = 0.0f;
		act_state = ActState::NONE;
		moveflag = false;
	}
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] > 0 && pad->x_input[static_cast<int>(PAD::R_DASH)] > 0)
	{
		speed.x = 0.0f;
		act_state = ActState::NONE;
		moveflag = false;
	}
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		speed.x = 0.0f;
		act_state = ActState::NONE;
		moveflag = false;
	}
	if (pad->x_input[static_cast<int>(PAD::R_DASH)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_L)] > 0)
	{
		speed.x = 0.0f;
		act_state = ActState::NONE;
		moveflag = false;
	}

	//if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 0 && pad->x_input[static_cast<int>(PAD::R_DASH)] == 0)
	//{
	//	speed.x = 0.0f;
	//	//if (act_state == ActState::DASH)
	//	//{
	//	//	//現在のステートがダッシュだった場合
	//	//	act_state = ActState::ATTACK;
	//	//	attack_state = AttackState::NONE;
	//	//	attack = true;
	//	//	later = dash_later;
	//	//	atk_result = HitResult::NOT_OCCURRENCE;
	//	//	if (rightOrleft > 0)
	//	//	{
	//	//		anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::LATER));
	//	//	}
	//	//	else
	//	//	{
	//	//		anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::LATER));
	//	//	}
	//	//	anim->PlayAnimation(scastI(AnimAtk::LATER), false);
	//	//	moveflag = false;
	//	//}
	//	//else
	//	//{
	//		act_state = ActState::NONE;
	//	//}
	//	moveflag = false;
	//}
	//if (pad->x_input[static_cast<int>(PAD::L_DASH)] > 0 && pad->x_input[static_cast<int>(PAD::R_DASH)] > 0)
	//{
	//	speed.x = 0.0f;
	//	//if (act_state == ActState::DASH)
	//	//{
	//	//	//現在のステートがダッシュだった場合
	//	//	act_state = ActState::ATTACK;
	//	//	attack_state = AttackState::NONE;
	//	//	attack = true;
	//	//	later = dash_later;
	//	//	atk_result = HitResult::NOT_OCCURRENCE;
	//	//	if (rightOrleft > 0)
	//	//	{
	//	//		anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::LATER));
	//	//	}
	//	//	else
	//	//	{
	//	//		anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::LATER));
	//	//	}
	//	//	anim->PlayAnimation(scastI(AnimAtk::LATER), false);
	//	//	moveflag = false;
	//	//}
	//	//else
	//	//{
	//		act_state = ActState::NONE;
	//	//}
	//	moveflag = false;
	//}

	////どちらも押されているor押されていないなら元に戻す
	//if (pad->x_input[static_cast<int>(PAD::STICK_L)] == 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] == 0)
	//{
	//	speed.x = 0.0f;
	//	//if (act_state == ActState::DASH)
	//	//{
	//	//	//現在のステートがダッシュだった場合
	//	//	act_state = ActState::ATTACK;
	//	//	attack_state = AttackState::NONE;
	//	//	attack = true;
	//	//	later = dash_later;
	//	//	atk_result = HitResult::NOT_OCCURRENCE;
	//	//	if (rightOrleft > 0)
	//	//	{
	//	//		anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::LATER));
	//	//	}
	//	//	else
	//	//	{
	//	//		anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::LATER));
	//	//	}
	//	//	anim->PlayAnimation(scastI(AnimAtk::LATER), false);
	//	//	moveflag = false;
	//	//}
	//	//else
	//	//{
	//		act_state = ActState::NONE;
	//	//}
	//	moveflag = false;
	//	//speed.x = 0.0f;
	//}
}



void Knight::MoveAnimSet()
{
	//スティックを両方とも倒している場合
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		return;
	}
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] > 0 && pad->x_input[static_cast<int>(PAD::R_DASH)] > 0)
	{
		return;
	}
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		return;
	}
	if (pad->x_input[static_cast<int>(PAD::R_DASH)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_L)] > 0)
	{
		return;
	}

	if (pad->x_input[static_cast<int>(PAD::STICK_L)] > 0)
	{
		moveflag = true;

		if (rightOrleft > 0)
		{
			//描画をセット
			//右向き
			//後退
			anim->NodeChange(model_motion.back_R);
			anim_ccodinate = ac_act[scastI(ActState::RETREAT)].fream;
		}
		//歩きは削除(ダッシュのみ)
		else
		{
			//描画をセット
			//左向き
			/*anim->NodeChange(model_motion.walk_L);
			anim_ccodinate = ac_act[scastI(ActState::MOVEL)].fream;*/
			//ダッシュ左向き
			act_state = ActState::DASH;
			anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::FREAM));
			anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
			anim_ccodinate = ac_act[scastI(act_state)].fream;
		}
	}
	if (pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		moveflag = true;

		if (rightOrleft < 0)
		{
			//描画をセット
			//左向き
			//後退
			anim->NodeChange(model_motion.back_L);
			anim_ccodinate = ac_act[scastI(ActState::RETREAT)].fream;
		}
		//歩きは削除(ダッシュのみ)
		else
		{
			//描画をセット
			//右向き
			/*anim->NodeChange(model_motion.walk_R);
			anim_ccodinate = ac_act[scastI(ActState::MOVER)].fream;*/
			//ダッシュ右向き
			act_state = ActState::DASH;
			anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::FREAM));
			anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
			anim_ccodinate = ac_act[scastI(act_state)].fream;
		}
	}
}

void Knight::GuardAnimSet()
{
	if (act_state == ActState::GUARD)
	{
		if (ground)
		{
			if (pad->x_input[static_cast<int>(PAD::STICK_RDown)] > 0 || pad->x_input[static_cast<int>(PAD::STICK_LDown)] > 0)
			{
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.guard_R, 2);
				}
				else
				{
					anim->NodeChange(model_motion.guard_L, 2);
				}
				anim_ccodinate = ac_act[scastI(act_state)].timer;
			}
			else
			{
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.guard_R);
				}
				else
				{
					anim->NodeChange(model_motion.guard_L);
				}
				anim_ccodinate = ac_act[scastI(act_state)].timer;
			}
		}
		else
		{
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.guard_R,1);
			}
			else
			{
				anim->NodeChange(model_motion.guard_L,1);
			}
			anim_ccodinate = ac_act[scastI(act_state)].timer;
		}
	}

}

void Knight::WaitAnimSet()
{
	if (act_state == ActState::NONE)
	{
		//描画をセット

		act_state = ActState::WAIT;
		lumi_material = Model::Material_Attribute::NONE;
		if (rightOrleft > 0)
		{
			//右向きの時
			anim->NodeChange(model_motion.wait_R);
			anim_ccodinate = ac_act[scastI(act_state)].fream;
		}
		else
		{
			//左向きの時
			anim->NodeChange(model_motion.wait_L);
			anim_ccodinate = ac_act[scastI(act_state)].fream;
		}
		speed.x = 0.0f;
		speed.y = 0.0f;
		moveflag = false;
	}
}


void Knight::Jump()
{
	if (step)
	{
		return;
	}
	if (ground)
	{
		if (pad->x_input[scastI(PAD::HIGH_UP)] == 1)
		{
			//ハイジャンプする
			speed_Y.Set(0.0f);
			speed_X.Set(0.0f);
			pad->que.back().timer = 0;
			GetSound().SESinglePlay(SEKind::HIGH_JUMP);
			jumpcount = 0;
			speed.y = high_jump_speed;
			hightrigger = true;
			max_jump_flag = false;
			moveflag = false;
			//描画をセット
			act_state = ActState::JUMP;
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::FREAM));
			}
			else
			{
				anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::FREAM));
			}
			anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
			jumpflag = true;

			//現在攻撃判定が出ているなら全て消去する
			AllAttackClear();
			//攻撃の結果を初期化
			hit_result = HitResult::NOT_OCCURRENCE;
			//キャンセルの条件を初期化
			atk_result = HitResult::NONE;
			//後隙を初期化
			later = non_target;
			//攻撃フラグをオフに
			attack = false;
			//攻撃ステートを初期化
			attack_state = AttackState::NONE;
			//角度を戻す
			angle.z = 0.0f;
			angle.y = 0.0f;
			//最終入力内容を初期化する
			last_attack = AttackState::NONE;

			if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				if (speed.x < dashspeed)
				{
					speed.x = dashspeed;
				}
				return;
			}
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				if (speed.x > -dashspeed)
				{
					speed.x = -dashspeed;

				}
				return;
			}
			speed.x = 0.0f;
			return;

		}


		if (pad->x_input[scastI(PAD::STICK_U)] == 1)
		{
			//じゃんぷする
			//later = -1;
			attack = FALSE;
			speed_X.Set(0.0f);
			speed_Y.Set(0.0f);
			pad->que.back().timer = 0;
			GetSound().SESinglePlay(SEKind::JUMP);
			jumpcount--;
			hightrigger = false;
			speed.y = jump_speed;
			max_jump_flag = false;
			moveflag = false;
			//描画をセット
			act_state = ActState::JUMP;
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::FREAM));
			}
			else
			{
				anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::FREAM));
			}
			anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
			anim_ccodinate = ac_act[scastI(act_state)].fream;
			jumpflag = true;
			jump_can_timer = jump_max_time;
			//現在攻撃判定が出ているなら全て消去する
			AllAttackClear();
			//攻撃の結果を初期化
			hit_result = HitResult::NOT_OCCURRENCE;
			//後隙を初期化
			later = non_target;
			//攻撃フラグをオフに
			attack = false;
			//攻撃ステートを初期化
			attack_state = AttackState::NONE;
			//角度を戻す
			angle.z = 0.0f;
			angle.y = 0.0f;
			//最終入力内容を初期化する
			last_attack = AttackState::NONE;

			if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				if (speed.x < walkspeed)
				{
					speed.x = walkspeed;
				}
				return;
			}
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				if (speed.x > -walkspeed)
				{
					speed.x = -walkspeed;

				}
				return;
			}
			speed.x = 0.0f;
		}
	}
	else
	{
		if (jumpcount > 0&&jump_can_timer<=0.0f)
		{
			if (pad->x_input[scastI(PAD::STICK_U)] == 1)
			{
				//空中ジャンプする
				//later = -1;
				attack = FALSE;
				speed_X.Set(0.0f);
				speed_Y.Set(0.0f);
				pad->que.back().timer = 0;
				GetSound().SESinglePlay(SEKind::JUMP);
				jumpcount = 0;
				air_dash_count = 0;
				hightrigger = false;
				speed.y = jump_speed;
				max_jump_flag = false;
				moveflag = false;
				//描画をセット
				act_state = ActState::JUMP;
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.air_jump_R, scastI(AnimAtk::FREAM));
				}
				else
				{
					anim->NodeChange(model_motion.air_jump_L, scastI(AnimAtk::FREAM));
				}
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//アニメーションが終了したら切り替える
				jumpflag = true;
				jump_can_timer = jump_max_time;
				//現在攻撃判定が出ているなら全て消去する
				AllAttackClear();
				//攻撃の結果を初期化
				hit_result = HitResult::NOT_OCCURRENCE;
				//キャンセルの条件を初期化
				atk_result = HitResult::NONE;
				//後隙を初期化
				later = non_target;
				//攻撃フラグをオフに
				attack = false;
				//攻撃ステートを初期化
				attack_state = AttackState::NONE;
				//角度を戻す
				angle.z = 0.0f;
				angle.y = 0.0f;
				//最終入力内容を初期化する
				last_attack = AttackState::NONE;

				if (pad->x_input[scastI(PAD::STICK_R)] > 0)
				{
					if (speed.x < walkspeed)
					{
						speed.x = walkspeed;
					}
					return;
				}
				if (pad->x_input[scastI(PAD::STICK_L)] > 0)
				{
					if (speed.x > -walkspeed)
					{
						speed.x = -walkspeed;
					}
					return;
				}
				speed.x = 0.0f;
			}
		}
	}

}

void Knight::JumpUpdate(float decision, float elapsed_time)
{
	if (jumpcount < 2 && jumpflag)
	{
		if (!anim->GetLoopAnim())
		{
			//現在のアニメーションがジャンプの開始アニメーションだった場合
			if (anim->GetEndAnim() == -1)
			{
				//アニメーションが終了したら持続アニメーションに切り替える
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::TIMER));
				}
				else
				{
					anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::TIMER));
				}
				anim_ccodinate = ac_act[scastI(act_state)].timer;
			}
		}
		if (jump_can_timer > 0.0f)
		{
			jump_can_timer -= elapsed_time;
		}
		/*if (speed_Y.speed == 0.0f)
		{
			pos.y += (speed.y * elapsed_time);
		}*/
		if (!max_jump_flag)
		{
			if (hightrigger)
			{
				speed.y += (high_jump_speed * elapsed_time);
				if (speed.y > high_jump_max)
				{
					max_jump_flag = true;
				}
			}
			else
			{
				speed.y += (jump_speed * elapsed_time);
				/*if (speed_Y.speedY == 0.0f)
				{
					pos.y += (speed.y * elapsed_time);
				}*/
				//float hei = speed.y * elapsed_time;
				if (speed.y > jump_max)
				{
					max_jump_flag = true;
				}
			}
		}
	}

	if (max_jump_flag)
	{
		if (!ground)
		{
			if (attack)
			{
				if (hit_result <= HitResult::HIT && combo_count == 0)
				{
					speed.y -= (down_force * elapsed_time);
				}
			}
			else
			{
				speed.y -= (down_force * elapsed_time);
			}

		}
		if (hightrigger)
		{
			if (speed.y - (gravity*elapsed_time) < 0.0f)
			{
				if (!attack)
				{
					//描画をセット

				}
				hightrigger = false;
			}
		}
	}
	if (pos.y < POS_Y)
	{
		jumpcount = 2;
		air_dash_count = 1;
		max_jump_flag = false;
		hightrigger = false;
		speed.y = 0.0f;
		speed_Y.Set(0.0f);
		//act_state = ActState::NONE;
		pos.y = POS_Y;
		jumpflag = false;
		//ジャンプの着地隙を発生する
		act_state = ActState::ATTACK;
		attack_state = AttackState::NONE;
		attack = true;
		later = jump_later;
		//攻撃でキャンセルできるように
		hit_result = HitResult::NOT_OCCURRENCE;
		atk_result = HitResult::NOT_OCCURRENCE;
		rightOrleft = decision;
		angle.y = 0.0f;
		//角度を元に戻す
		angle.y = 0.0f;
		angle.z = 0.0f;
		GetSound().SESinglePlay(SEKind::LANDING);
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::LATER));
		}
		else
		{
			anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::LATER));
		}
		anim->PlayAnimation(scastI(AnimAtk::LATER), false);
		//攻撃判定をすべて消去する
		AllAttackClear();
	}
}


void Knight::DamageCheck(float decision)
{
	for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
	{
		if (hit[i].hit)
		{
			//攻撃を受けていた
			GetSound().SEStop(SEKind::SLIDE);
			rightOrleft = decision;
			//条件ごとに設定
			switch (hit[i].hit_state)
			{
			case HitStateKind::NORMAL:
				//通常攻撃
				act_state = ActState::KNOCK;
				steal_escape = 0.0f;
				hit[i].hit_state = HitStateKind::NORMAL;
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.damage_R_g_u);
				}
				else
				{
					anim->NodeChange(model_motion.damage_L_g_u);
				}
				ChangeFace(FaceAnim::Damage);
				anim_ccodinate = 5.0f;
				if (hit[i].hitback.y < 0.0f)
				{
					//吹っ飛びベクトルがマイナスの場合
					if (pos.y <= POS_Y)
					{
						//下方向の3分の1の速度をXに渡す
						if (hit[i].hitback.x >= 0.0f)
						{
							hit[i].hitback.x += (-hit[i].hitback.y * 0.3f);
						}
						else
						{
							hit[i].hitback.x += (hit[i].hitback.y * 0.3f);
						}
						//地面についている場合は下方向の速度を0にする
						hit[i].hitback.y = 0.0f;
						//のけぞり時間を3分の1にする
						hit[i].timer *= 0.25f;
					}
				}
				break;
			case HitStateKind::STEAL:
				//掴み攻撃
				act_state = ActState::STATENONE;
				steal_escape = hit[i].steal_timer;
				hit[i].hit_state = HitStateKind::STEAL;
				hit[i].steal_timer = 0.0f;
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.damage_R_g_u);
				}
				else
				{
					anim->NodeChange(model_motion.damage_L_g_u);
				}
				ChangeFace(FaceAnim::Damage);
				anim_ccodinate = 5.0f;
				break;
			case HitStateKind::SLAM:
				//叩きつけ攻撃
				act_state = ActState::SLAM;
				hit[i].hit_state = HitStateKind::NORMAL;
				steal_escape = 0.0f;
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.damage_R_g_u);
				}
				else
				{
					anim->NodeChange(model_motion.damage_L_g_u);
				}
				ChangeFace(FaceAnim::Damage);
				anim_ccodinate = 5.0f;
				break;
			case HitStateKind::DOWN:
				//ダウン攻撃
				act_state = ActState::DOWN_HIT;
				hit[i].hit_state = HitStateKind::NORMAL;
				steal_escape = 0.0f;
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.damage_R_g_u);
				}
				else
				{
					anim->NodeChange(model_motion.damage_L_g_u);
				}
				ChangeFace(FaceAnim::Damage);
				anim_ccodinate = 5.0f;
				break;
			case HitStateKind::ARMOR:
				//アーマーが発動した
			{
				steal_escape = 0.0f;
				hit[i].hit_state = HitStateKind::NORMAL;
				float dg = hit[i].damege - (combo_count * 1.2f);
				if (dg <= 0)
				{
					dg = 1;
				}
				hp -= dg;
				if (hp <= 0.0f)
				{
					hp = 0.0f;
					break;
				}
				GaugeUp(hit[i].damege / 5.0f);
				hit[i].damege = 0.0f;
				hit[i].hit = false;
				return;
			}
				break;
			default:
				break;
			}

			//ダメージ、吹っ飛びベクトルなどを保存
			HitBoxTransition(HitBoxState::NOGUARD);
			float dg = hit[i].damege - (static_cast<float>(combo_count) * 1.2f);
			if (dg <= 0.0f)
			{
				dg = 1.0f;
			}
			hp -= dg;
			if (hp <= 0.0f)
			{
				hp = 0.0f;
				act_state = ActState::DOWN_HIT;
			}
			combo_count++;
			GaugeUp(hit[i].damege / 5.0f);
			hit[i].damege = 0.0f;
			hit[i].hit = false;

			//プレイヤーをやられ状態にする
			attack = false;
			later = non_target;
			moveflag = false;
			if (jumpcount > 1)
			{
				jumpcount = 1;
			}
			air_dash_count = 1;
			jumpflag = false;
			max_jump_flag = false;
			knocktimer = hit[i].timer;
			speed.x = 0.0f;
			speed.y = 0.0f;
			step = false;
			pad->dash_trigger = false;
			pad->high_trigger = false;
			hightrigger = false;
			speed_X.Set(0.0f);
			speed_Y.Set(0.0f);
			AllAttackClear();
			hit_result = HitResult::NOT_OCCURRENCE;
			lumi_material = Model::Material_Attribute::NONE;
			//キャンセルの条件を初期化
			atk_result = HitResult::NONE;
			attack_state = AttackState::NONE;
			//角度を元に戻す
			angle.y = 0.0f;
			angle.z = 0.0f;
			//最終入力内容を初期化する
			last_attack = AttackState::NONE;
		}
	}
}

void Knight::KnockUpdate(float elapsed_time)
{
	bool pflag = false;
	for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
	{
		if (hit[i].hitback.x != 0.0f)
		{
			pos.x += hit[i].hitback.x*elapsed_time;
			if (pos.x > Limit::Right_max)
			{
				//一定の速度以上で壁に当たった場合跳ね返るようにする
				if (hit[i].hitback.x > Reflection_range_min)
				{
					hit[i].hitback.x = (-hit[i].hitback.x*Reflection_attenuation_factor);
				}
			}
			if (pos.x < Limit::Left_max)
			{
				//一定の速度以上で壁に当たった場合跳ね返るようにする
				if (hit[i].hitback.x < -Reflection_range_min)
				{
					hit[i].hitback.x = (-hit[i].hitback.x * Reflection_attenuation_factor);
				}
			}
			pflag = true;
		}
		if (hit[i].hitback.y != 0.0f)
		{
			pos.y += hit[i].hitback.y*elapsed_time;
			pflag = true;
		}
		if (pflag)
		{
			break;
		}
	}
	//角度を戻す
	angle.z = 0.0f;
	knocktimer -= elapsed_time;

	if (pos.y<POS_Y)
	{
		pos.y = POS_Y;
		//のけぞり中に地面についた場合
		knocktimer = 0.0f;
		combo_count = 0;
		for (int i = 0; i < hit.size(); i++)
		{
			hit[i].timer = 0.0f;
			hit[i].hitback = YR_Vector3(0.0f, 0.0f);
		}
		if (pad->x_input[scastI(PAD::STICK_D)] > 0 ||
			pad->x_input[scastI(PAD::STICK_LDown)] > 0 ||
			pad->x_input[scastI(PAD::STICK_RDown)] > 0)
		{
			//スティックの下入力がある場合は落下状態にする
			act_state = ActState::DOWN;
			speed.y = 0.0f;
			GetSound().SESinglePlay(SEKind::SLAM);
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.slid_R);
			}
			else
			{
				anim->NodeChange(model_motion.slid_L);
			}
			anim_ccodinate = ac_act[scastI(act_state)].timer;
		}
		else
		{
			speed.x = 0.0f;
			speed.y = 0.0f;
			//何も入力していない場合は自動的に受け身を取る
			speed.x = Getapply(-passive_speed.x);
			speed.y = passive_speed.y;
			//受け身時間を入力
			passive_timer = passive_max_time;
			act_state = ActState::PASSIVE;
			GetSound().SESinglePlay(SEKind::PASSIVE);
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.passive_R);
			}
			else
			{
				anim->NodeChange(model_motion.passive_L);
			}
			HitBoxTransition(HitBoxState::INVINCIBLE);
			if (hp <= 0.0f)
			{
				//体力がなくなったら落下状態にする
				act_state = ActState::DOWN;
				speed.y = 0.0f;
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.slid_R);
				}
				else
				{
					anim->NodeChange(model_motion.slid_L);
				}
				anim_ccodinate = ac_act[scastI(act_state)].timer;
			}
		}
		return;
	}

	if (knocktimer < 0.0f)
	{
		combo_count = 0;
		if (ground)
		{
			if (act_state != ActState::WAIT)
			{
				act_state = ActState::NONE;
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.wait_R);
				}
				else
				{
					anim->NodeChange(model_motion.wait_L);
				}
				ChangeFace(FaceAnim::NORMAL);
				anim_ccodinate = 0.0f;
			}
		}
		else
		{
			//空中でのけぞり状態が解除された
			//if (pad->x_input[scastI(PAD::STICK_D)] > 0 ||
			//	pad->x_input[scastI(PAD::STICK_LDown)] > 0 ||
			//	pad->x_input[scastI(PAD::STICK_RDown)] > 0)
			//{
			//	//スティックの下入力がある場合は落下状態にする
			//	act_state = ActState::FALL;
			//	speed.y = 0.0f;
			//}
			//else
			if (hp <= 0.0f)
			{

			}
			else
			{
				//入力が何もない場合は受け身を自動で取る
				speed.x = 0.0f;
				speed.y = 0.0f;
				act_state = ActState::PASSIVE;
				GetSound().SESinglePlay(SEKind::PASSIVE);
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.passive_R);
				}
				else
				{
					anim->NodeChange(model_motion.passive_L);
				}
				HitBoxTransition(HitBoxState::INVINCIBLE);
				if (pad->x_input[scastI(PAD::STICK_R)] > 0)
				{
					speed.x = passive_speed.x;
					speed.y = passive_speed.y;
					//受け身時間を入力
					passive_timer = passive_max_time;
				}
				if (pad->x_input[scastI(PAD::STICK_L)] > 0)
				{
					speed.x = -passive_speed.x;
					speed.y = passive_speed.y;
					//受け身時間を入力
					passive_timer = passive_max_time;
				}
				if (pad->x_input[scastI(PAD::STICK_U)] > 0)
				{
					speed.y = passive_speed.y;
					//受け身時間を入力
					passive_timer = passive_max_time;
				}
				if (pad->x_input[scastI(PAD::STICK_D)] > 0)
				{
					speed.y = -passive_speed.y;
					//受け身時間を入力
					passive_timer = passive_max_time;
				}

				if (pad->x_input[scastI(PAD::STICK_R)] == 0 && pad->x_input[scastI(PAD::STICK_L)] == 0 &&
					pad->x_input[scastI(PAD::STICK_U)] == 0 && pad->x_input[scastI(PAD::STICK_D)] == 0)
				{
					speed.x = Getapply(-passive_speed.x);
					speed.y = passive_speed.y;
					//受け身時間を入力
					passive_timer = passive_max_time;
				}
			}

			if (trackgauge < 2)
			{
				trackgauge = 1;

			}
		}
		if (hp == 0.0f)
		{
			//体力がなくなったら落下状態にする
			act_state = ActState::FALL;
			speed.y = 0.0f;
			jumpflag = true;
			max_jump_flag = true;
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.slid_R);
			}
			else
			{
				anim->NodeChange(model_motion.slid_L);
			}
			anim_ccodinate = ac_act[scastI(act_state)].timer;
		}
		knocktimer = 0.0f;
		for (int i = 0; i < hit.size(); i++)
		{
			hit[i].timer = 0.0f;
			hit[i].hitback = YR_Vector3(0.0f, 0.0f);
		}
	}
}



void Knight::SlamUpdate(float elapsed_time)
{
	bool pflag = false;

	DirectX::XMFLOAT2 hit_back_s = { 0.0f,0.0f };

	for (int i = 0; i < hit.size(); i++)
	{
		if (hit[i].hitback.x != 0.0f)
		{
			pos.x += hit[i].hitback.x * elapsed_time;
			hit_back_s.x = hit[i].hitback.x;
			pflag = true;
		}
		if (hit[i].hitback.y != 0.0f)
		{
			pos.y += hit[i].hitback.y * elapsed_time;
			hit_back_s.y = hit[i].hitback.y;
			pflag = true;
		}
		if (pflag)
		{
			break;
		}
	}
	//角度を戻す
	angle.z = 0.0f;
	knocktimer -= elapsed_time;


	//地面についた時
	if (pos.y < POS_Y)
	{
		//X方向のスピードを吹っ飛びベクトルから生成する
		DirectX::XMFLOAT2 x_back_s = { (rightOrleft),1.0f };
		//内積を計算して射影ベクトルを作成
		//内積
		float dot = ((hit_back_s.x * x_back_s.x) + (hit_back_s.y * x_back_s.y));
		//X方向の速度に射影ベクトルを設定
		speed.x = (x_back_s.x * (dot/ attenuation_slam));

		//当たり判定に入力されている速度を全て初期化する
		for (int i = 0; i < hit.size(); i++)
		{
			hit[i].hitback.x = 0.0f;
			hit[i].hitback.y = 0.0f;
		}

		//アニメーションを変更する
		ChangeFace(FaceAnim::NORMAL);
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.slid_R, 1);
		}
		else
		{
			anim->NodeChange(model_motion.slid_L, 1);
		}
		anim->PlayAnimation(1, true);
		//地面のめり込みを治す
		pos.y = POS_Y;
	}

	//地面を滑っている時
	if (ground)
	{
		GetSound().SEPlay(SEKind::SLIDE);
		HitBoxTransition(HitBoxState::SLIDE);
		if (speed.x > 0.0f)
		{
			speed.x -= (attenuation_slam * elapsed_time);
			if (speed.x < 0.0f)
			{
				speed.x = 0;
			}
		}
		if (speed.x < 0.0f)
		{
			speed.x += (attenuation_slam * elapsed_time);
			if (speed.x > 0.0f)
			{
				speed.x = 0.0f;
			}
		}
	}

	//のけぞり時間経過
	if (knocktimer < 0.0f)
	{
		GetSound().SEStop(SEKind::SLIDE);
		combo_count = 0;
		if (ground)
		{
			pos.y = POS_Y;
			knocktimer = 0.0f;
			combo_count = 0;
			for (int i = 0; i < hit.size(); i++)
			{
				hit[i].timer = 0.0f;
				hit[i].hitback = YR_Vector3(0.0f, 0.0f);
			}
			if (pad->x_input[scastI(PAD::STICK_D)] > 0 ||
				pad->x_input[scastI(PAD::STICK_LDown)] > 0 ||
				pad->x_input[scastI(PAD::STICK_RDown)] > 0)
			{
				//スティックの下入力がある場合は落下状態にする
				act_state = ActState::DOWN;
				speed.y = 0.0f;
				speed.x = 0.0f;
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.slid_R);
				}
				else
				{
					anim->NodeChange(model_motion.slid_L);
				}
				anim_ccodinate = ac_act[scastI(act_state)].timer;
			}
			else if (hp == 0.0f)
			{
				//体力がない場合は落下状態にする
				act_state = ActState::DOWN;
				speed.y = 0.0f;
				speed.x = 0.0f;
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.slid_R);
				}
				else
				{
					anim->NodeChange(model_motion.slid_L);
				}
				anim_ccodinate = ac_act[scastI(act_state)].timer;
			}
			else
			{
				speed.x = 0.0f;
				speed.y = 0.0f;
				//何も入力していない場合は自動的に受け身を取る
				speed.x = Getapply(-passive_speed.x);
				speed.y = passive_speed.y;
				//受け身時間を入力
				passive_timer = passive_max_time;
				act_state = ActState::PASSIVE;
				GetSound().SESinglePlay(SEKind::PASSIVE);
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.passive_R);
				}
				else
				{
					anim->NodeChange(model_motion.passive_L);
				}
				HitBoxTransition(HitBoxState::INVINCIBLE);
			}

			if (hp == 0.0f)
			{
				//体力がなくなったら落下状態にする
				act_state = ActState::DOWN;
			}
			return;
		}
		else
		{
			act_state = ActState::FALL;
			speed.y = 0.0f;
			//描画をセット

			if (trackgauge < 2)
			{
				trackgauge = 1;

			}
		}
		knocktimer = 0.0f;
		for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
		{
			hit[i].timer = 0.0f;
			hit[i].hitback = YR_Vector3(0.0f, 0.0f);
		}
	}
}


void Knight::DownHitUpdate(float elapsed_time)
{
	bool pflag = false;

	for (int i = 0; i < hit.size(); i++)
	{
		if (hit[i].hitback.x != 0.0f)
		{
			pos.x += hit[i].hitback.x * elapsed_time;
			pflag = true;
		}
		if (hit[i].hitback.y != 0.0f)
		{
			pos.y += hit[i].hitback.y * elapsed_time;
			pflag = true;
		}
		if (pflag)
		{
			break;
		}
	}
	//角度を戻す
	angle.z = 0.0f;

	if (knocktimer > 0.0f)
	{
		knocktimer -= elapsed_time;
	}
	else
	{
		//当たり判定に入力されている速度を全て初期化する
		for (int i = 0; i < hit.size(); i++)
		{
			hit[i].hitback.x = 0.0f;
			hit[i].hitback.y = 0.0f;
		}
		if (!ground)
		{
			max_jump_flag = true;
		}
		if (hp == 0.0f)
		{
			if (ground)
			{
				combo_count = 0;
				knocktimer = 0.0f;
				//ダウン状態にする
				act_state = ActState::DOWN;
				speed.y = 0.0f;
				speed.x = 0.0f;
				for (int i = 0; i < hit.size(); i++)
				{
					hit[i].timer = 0.0f;
					hit[i].hitback = YR_Vector3(0.0f, 0.0f);
				}
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.slid_R);
				}
				else
				{
					anim->NodeChange(model_motion.slid_L);
				}
				anim_ccodinate = ac_act[scastI(act_state)].timer;
			}
		}
	}

	//地面についた時
	if (pos.y < POS_Y)
	{
		combo_count = 0;
		knocktimer = 0.0f;
		//ダウン状態にする
		act_state = ActState::DOWN;
		GetSound().SESinglePlay(SEKind::SLAM);
		speed.y = 0.0f;
		speed.x = 0.0f;
		for (int i = 0; i < hit.size(); i++)
		{
			hit[i].timer = 0.0f;
			hit[i].hitback = YR_Vector3(0.0f, 0.0f);
		}
		//描画をセット
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.slid_R);
		}
		else
		{
			anim->NodeChange(model_motion.slid_L);
		}
		anim_ccodinate = ac_act[scastI(act_state)].timer;
		//地面のめり込みを治す
		pos.y = POS_Y;
	}
}

void Knight::Guard(float decision)
{

	if (step || attack || act_state == ActState::PASSIVE|| act_state == ActState::WAKE)
	{
		//攻撃中、またはステップ中、受け身中なら入らない
		return;
	}

	HitBoxTransition(HitBoxState::NOGUARD);

	if (rightOrleft > 0)
	{
		if (pad->x_input[scastI(PAD::STICK_LDown)] > 0)
		{
			if (!ground)
			{
				HitBoxTransition(HitBoxState::ALL);
			}
			else
			{
				HitBoxTransition(HitBoxState::DOWN);
			}
		}
		else
		{
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				if (!ground)
				{
					HitBoxTransition(HitBoxState::ALL);
				}
				else
				{
					HitBoxTransition(HitBoxState::MIDDLE);
				}
			}
		}
	}
	if (rightOrleft < 0)
	{
		if (pad->x_input[scastI(PAD::STICK_RDown)] > 0)
		{
			if (!ground)
			{
				HitBoxTransition(HitBoxState::ALL);
			}
			else
			{
				HitBoxTransition(HitBoxState::DOWN);
			}
		}
		else
		{
			if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				if (!ground)
				{
					HitBoxTransition(HitBoxState::ALL);
				}
				else
				{
					HitBoxTransition(HitBoxState::MIDDLE);
				}
			}
		}
	}

	if (!ground)
	{
		if (decision != rightOrleft)
		{
			if (pad->x_input[scastI(PAD::STICK_R)] > 0 || pad->x_input[scastI(PAD::STICK_RDown)] > 0
				|| pad->x_input[scastI(PAD::STICK_L)] > 0 || pad->x_input[scastI(PAD::STICK_LDown)] > 0)
			{
				HitBoxTransition(HitBoxState::ALL);
			}
		}
	}

	for (int i = 0; i < hit.size(); i++)
	{
		if (hit[i].guard_ok)
		{
			GaugeUp(hit[i].damege / 4.0f);
			speed_X.Set(0.0f);
			speed_Y.Set(0.0f);
			speed.x = 0.0f;
			hp -= hit[i].damege;
			if (hp < 0)
			{
				hp = 1;
			}
			if (decision != rightOrleft)
			{
				rightOrleft = decision;
			}
			//ベクトルが内側だった場合外側にする
			if (rightOrleft > 0)
			{
				//右向き時
				if (hit[i].hitback.x > 0.0f)
				{
					hit[i].hitback.x = -hit[i].hitback.x;
				}
			}
			else
			{
				//左向き
				if (hit[i].hitback.x < 0.0f)
				{
					hit[i].hitback.x = -hit[i].hitback.x;
				}
			}
			hit[i].damege = 0;
			hit[i].guard_ok = false;
			moveflag = false;
			knocktimer = hit[i].timer;
			pad->high_trigger = false;
			hightrigger = false;
			act_state = ActState::GUARD;

			GetSound().SESinglePlay(SEKind::GUARD);
			if (rightOrleft > 0)
			{
				YRGetEffect().PlayEffect(EffectKind::GUARD, DirectX::XMFLOAT3(pos.x + draw_guarf_effect_add_pos_x, pos.y, -2.0f), DirectX::XMFLOAT3(0.7f, 0.7f, 0.7f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 5.5f);
			}
			else
			{
				YRGetEffect().PlayEffect(EffectKind::GUARD, DirectX::XMFLOAT3(pos.x - draw_guarf_effect_add_pos_x, pos.y, -2.0f), DirectX::XMFLOAT3(0.7f, 0.7f, 0.7f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), -5.5f);
			}

		}
	}

}

void Knight::GuardBack(float elapsed_time)
{
	if (act_state != ActState::GUARD)
	{
		return;
	}

	bool hit_on = false;


	for (int i = 0; i < hit.size(); i++)
	{
		if (hit[i].hitback.y != 0.0f)
		{
			if (!ground)
			{
				pos.y += (hit[i].hitback.y * elapsed_time);
				hit_on = true;
			}
		}
		if (hit[i].hitback.x != 0.0f)
		{
			pos.x += (hit[i].hitback.x * elapsed_time);
			hit_on = true;
		}

		if (hit_on)
		{
			break;
		}
	}
	if (knocktimer > 0.0f)
	{
		knocktimer -= elapsed_time;
		if (knocktimer == 0)
		{
			knocktimer -= elapsed_time;
		}
	}
	if (knocktimer < 0.0f)
	{
		if (ground)
		{
			if (act_state != ActState::WAIT)
			{
				act_state = ActState::NONE;
			}
			if (pad->x_input[scastI(PAD::STICK_D)] > 0)
			{
				act_state = ActState::SQUAT;
			}
			if (pad->x_input[scastI(PAD::STICK_LDown)] > 0)
			{
				act_state = ActState::SQUAT;
			}
			if (pad->x_input[scastI(PAD::STICK_RDown)] > 0)
			{
				act_state = ActState::SQUAT;
			}
			if (act_state == ActState::SQUAT)
			{
				//描画をセット
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.squat_R);
				}
				else
				{
					anim->NodeChange(model_motion.squat_L);
				}
				anim_ccodinate = ac_act[scastI(act_state)].fream;
			}
		}
		else
		{
			act_state = ActState::JUMP;
			//描画をセット
			max_jump_flag = true;
			jumpflag = true;
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::TIMER));
			}
			anim_ccodinate = ac_act[scastI(act_state)].timer;
		}

		knocktimer = 0;
		for (int hitnum = 0; hitnum < hit.size(); hitnum++)
		{
			hit[hitnum].timer = 0.0f;
			hit[hitnum].hitback = YR_Vector3(0.0f, 0.0f);
		}
	}
	else
	{
		act_state = ActState::GUARD;
	}
}

void Knight::Squat()
{
	if (act_state == ActState::GUARD || act_state == ActState::DASH|| act_state == ActState::BACK || act_state == ActState::AIR_B || act_state == ActState::AIR_F)
	{
		return;
	}
	if (!ground)
	{
		return;
	}
	if (pad->x_input[scastI(PAD::STICK_D)] ==1 || pad->x_input[scastI(PAD::STICK_RDown)] == 1 || pad->x_input[scastI(PAD::STICK_LDown)] == 1)
	{
		//Hitplus[scastI(KNIGHTHIT::BODY)] = YR_Vector3(0.0f, 15.0f);
		//hit[scastI(KNIGHTHIT::BODY)].size = YR_Vector3(65.0f, 130.0f);
		moveflag = false;
		act_state = ActState::SQUAT;
		//描画をセット
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.squat_R);
		}
		else
		{
			anim->NodeChange(model_motion.squat_L);
		}
		anim_ccodinate = ac_act[scastI(act_state)].fream;
	}
	if (pad->x_input[scastI(PAD::STICK_D)] > 0 || pad->x_input[scastI(PAD::STICK_RDown)] > 0 || pad->x_input[scastI(PAD::STICK_LDown)] > 0)
	{
		//Hitplus[scastI(KNIGHTHIT::BODY)] = YR_Vector3(0.0f, 15.0f);
		//hit[scastI(KNIGHTHIT::BODY)].size = YR_Vector3(65.0f, 130.0f);
		moveflag = false;
		act_state = ActState::SQUAT;
		//描画をセット
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.squat_R);
		}
		else
		{
			anim->NodeChange(model_motion.squat_L);
		}
		anim_ccodinate = ac_act[scastI(act_state)].fream;
	}
	if (pad->x_input[scastI(PAD::STICK_D)] == 0 && pad->x_input[scastI(PAD::STICK_RDown)] == 0 && pad->x_input[scastI(PAD::STICK_LDown)] == 0)
	{
		if (act_state == ActState::SQUAT)
		{
			//描画をセット

		}

		if (act_state != ActState::WAIT)
		{
			act_state = ActState::NONE;
		}
	}
}


void Knight::FallUpdate(float elapsed_time)
{
	if (ground)
	{
		pos.y = POS_Y;
		if (hp > 0)
		{
			speed.y = 5.0f;
		}
		act_state = ActState::DOWN;
		//描画をセット

	}
	else
	{
		//空中時
		if (pos.y < POS_Y)
		{
			//重力を付与する
			pos.y -= gravity * elapsed_time;
			//speed.y += 10.0f * elapsed_time;
		}
		//if (pad->x_input[scastI(PAD::X)] == 1 || pad->x_input[scastI(PAD::Y)] == 1 ||
		//	pad->x_input[scastI(PAD::B)] == 1 || pad->x_input[scastI(PAD::A)] == 1)
		//{
		//	speed.x = 0.0f;
		//	speed.y = 0.0f;
		//	act_state = ActState::PASSIVE;
		//	//描画をセット

		//	HitBoxTransition(HitBoxState::INVINCIBLE);
		//	if (pad->x_input[scastI(PAD::STICK_R)] > 0)
		//	{
		//		speed.x = 20.0f;
		//		speed.y = -10.0f;
		//	}
		//	if (pad->x_input[scastI(PAD::STICK_L)] > 0)
		//	{
		//		speed.x = -20.0f;
		//		speed.y = -10.0f;
		//	}
		//	if (pad->x_input[scastI(PAD::STICK_U)] > 0)
		//	{
		//		speed.y = -20.0f;
		//	}
		//	if (pad->x_input[scastI(PAD::STICK_D)] > 0)
		//	{
		//		speed.y = 20.0f;
		//	}

		//	if (pad->x_input[scastI(PAD::STICK_R)] == 0 && pad->x_input[scastI(PAD::STICK_L)] == 0 &&
		//		pad->x_input[scastI(PAD::STICK_U)] == 0 && pad->x_input[scastI(PAD::STICK_D)] == 0)
		//	{
		//		speed.x = Getapply(-20.0f);
		//		speed.y = -10.0f;
		//	}
		//}
	}
}



void Knight::DownUpdate()
{

	if (ground)
	{
		//ダウン中特定の入力があればステートを切り替える
		pos.y = POS_Y;
		speed.y = 0.0f;
		speed.x = 0.0f;
		ChangeFace(FaceAnim::NORMAL);
		if (pad->x_input[scastI(PAD::STICK_R)] > 0)
		{
			speed.x = passive_speed.x;
			speed.y = passive_speed.y;
			//受け身時間を入力
			passive_timer = passive_max_time;
			act_state = ActState::PASSIVE;
			GetSound().SESinglePlay(SEKind::PASSIVE);
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.passive_R);
			}
			else
			{
				anim->NodeChange(model_motion.passive_L);
			}
			HitBoxTransition(HitBoxState::INVINCIBLE);
		}
		if (pad->x_input[scastI(PAD::STICK_L)] > 0)
		{
			speed.x = -passive_speed.x;
			speed.y = passive_speed.y;
			//受け身時間を入力
			passive_timer = passive_max_time;
			act_state = ActState::PASSIVE;
			GetSound().SESinglePlay(SEKind::PASSIVE);
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.passive_R);
			}
			else
			{
				anim->NodeChange(model_motion.passive_L);
			}
			HitBoxTransition(HitBoxState::INVINCIBLE);
		}
		if (pad->x_input[scastI(PAD::X)] > 0 || pad->x_input[scastI(PAD::Y)] > 0 ||
			pad->x_input[scastI(PAD::B)] > 0 || pad->x_input[scastI(PAD::A)] > 0)
		{
			speed.x = 0.0f;
			speed.y = 0.0f;
			act_state = ActState::PASSIVE;
			GetSound().SESinglePlay(SEKind::PASSIVE);
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.passive_R);
			}
			else
			{
				anim->NodeChange(model_motion.passive_L);
			}
			HitBoxTransition(HitBoxState::INVINCIBLE);
			if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				speed.x = passive_speed.x;
				speed.y = passive_speed.y;
				//受け身時間を入力
				passive_timer = passive_max_time;
			}
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				speed.x = -passive_speed.x;
				speed.y = passive_speed.y;
				//受け身時間を入力
				passive_timer = passive_max_time;
			}
			if (pad->x_input[scastI(PAD::STICK_R)] == 0 && pad->x_input[scastI(PAD::STICK_L)] == 0)
			{
				speed.x = Getapply(-passive_speed.x);
				speed.y = passive_speed.y;
				//受け身時間を入力
				passive_timer = passive_max_time;
			}
		}
		if (pad->x_input[scastI(PAD::STICK_U)] > 0)
		{
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.passive_R, 1);
			}
			else
			{
				anim->NodeChange(model_motion.passive_L, 1);
			}
			anim->PlayAnimation(1, false);
			act_state = ActState::WAKE;
		}
	}
}

void Knight::WakeUp()
{
	//起き上がり開始は無敵
	HitBoxTransition(HitBoxState::INVINCIBLE);

	//ダウンから回復(完全に起き上がった時)無敵を解除する
	//現在のアニメーションが起き上がりだった場合
	if (anim->GetEndAnim() == -1)
	{
		HitBoxTransition(HitBoxState::NOGUARD);
		act_state = ActState::NONE;
	}
}


void Knight::PassiveUpdate(float elapsed_time)
{

	if (rightOrleft > 0)
	{
		//右向き
		angle.z += 30.0f * elapsed_time;
	}
	if (rightOrleft < 0)
	{
		//左向き
		angle.z -= 30.0f * elapsed_time;
	}

	if (pos.y < POS_Y)
	{
		pos.y = POS_Y;
	}

	passive_timer -= elapsed_time;

	if (speed.x > 0)
	{
		speed.x -= (passive_brake * elapsed_time);
		if (speed.x < 0.0f)
		{
			speed.x = 0.0f;
		}
	}
	if (speed.x < 0)
	{
		speed.x+= (passive_brake * elapsed_time);
		if (speed.x > 0.0f)
		{
			speed.x = 0.0f;
		}
	}
	if (speed.y > 0)
	{
		speed.y-= (passive_brake * elapsed_time);
		if (speed.y < 0.0f)
		{
			speed.y = 0.0f;
		}
	}
	if (speed.y < 0)
	{
		speed.y+= (passive_brake * elapsed_time);
		if (speed.y > 0.0f)
		{
			speed.y = 0.0f;
		}
	}
	if (passive_timer < 0.0f)
	{
		HitBoxTransition(HitBoxState::NOGUARD);
		ChangeFace(FaceAnim::NORMAL);
		angle.z = 0.0f;
		passive_timer = 0.0f;
		if (ground)
		{
			ChangeFace(FaceAnim::NORMAL);
			act_state = ActState::NONE;
		}
		else
		{
			ChangeFace(FaceAnim::NORMAL);
			act_state = ActState::JUMP;
			max_jump_flag = true;
			jumpflag = true;
			lumi_material = Model::Material_Attribute::NONE;
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::TIMER));
			}
			anim_ccodinate = ac_act[scastI(act_state)].timer;
		}
	}
}


void Knight::StateNone(float elapsed_time)
{
	if (steal_escape > 0.0f)
	{
		//描画をセット

		if (pad->x_input[scastI(PAD::RB)] == 1)
		{
			steal_escape = 0.0f;
			speed.x = 0.0f;
			speed.y = 0.0f;
			knocktimer = 0.0f;
			for (int i = 0; i < hit.size(); i++)
			{
				hit[i].timer = 0.0f;
				hit[i].hitback = YR_Vector3(0.0f, 0.0f);
			}
			combo_count = 0;
			act_state = ActState::PASSIVE;
			GetSound().SESinglePlay(SEKind::PASSIVE);
			//描画をセット
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.passive_R);
			}
			else
			{
				anim->NodeChange(model_motion.passive_L);
			}
			HitBoxTransition(HitBoxState::INVINCIBLE);

			speed.x = Getapply(-passive_speed.x);
			speed.y = passive_speed.y;
			//受け身時間を入力
			passive_timer = passive_max_time;
			return;
		}
		steal_escape -= elapsed_time;
	}
}


//float Ryu::GetPosX()
//{
//	return pos.x;
//}

float Knight::Getapply(float n)
{
	return n * rightOrleft;
}

HitBox* Knight::GetHit()
{
	return hit.data();
}
AttackBox* Knight::GetAttack()
{
	return atk.data();
}
int Knight::GetMax(int n)
{
	if (n == 0)
	{
		return scastI(KNIGHTHIT::END);
	}
	if (n == 1)
	{
		return atk.size();
	}
	return 0;
}

//float& Ryu::GetPosX2()
//{
//	return pos.x;
//}

void Knight::NoneChange()
{
	if (act_state != ActState::GUARD)
	{
		return;
	}
	int none = 0;
	for (auto& it : pad->x_input)
	{
		if (it > 0)
		{
			none++;
		}
	}
	if (none == 0)
	{
		act_state = ActState::NONE;
	}
}

void Knight::GaugeUp(float add)
{
	gauge += add;
	if (power > 4)
	{
		power = 5;
		if (gauge > GAUGE_MAX)
		{
			gauge = GAUGE_MAX;
		}
	}
	else
	{
		if (gauge > GAUGE_MAX)
		{
			float away = gauge - GAUGE_MAX;
			power++;
			gauge = 0;
			gauge = away;
		}
	}
}


void Knight::StopUpdate()
{
	float shift_pos = 0.1f;
	if (stop_pos == YR_Vector3(0.0f, 0.0f))
	{
		stop_pos = pos;
	}

	switch (stop_state)
	{
	case 0:
		pos.x += shift_pos;
		pos.y += shift_pos;
		stop_state++;
		break;
	case 1:
		pos.x -= shift_pos;
		pos.y -= shift_pos;
		stop_state++;
		break;
	case 2:
		pos.x -= shift_pos;
		pos.y -= shift_pos;
		stop_state++;
		break;
	case 3:
		pos.x += shift_pos;
		pos.y += shift_pos;
		stop_state = 0;
		break;
	}

}
void Knight::StopEnd()
{
	if (stop_pos == YR_Vector3(0.0f, 0.0f))
	{
		return;
	}
	pos = stop_pos;
	stop_pos = YR_Vector3(0.0f, 0.0f);
}


void Knight::WinAnimSet()
{
	//勝利演出用のセット
	pos = YR_Vector3( 0.0f,0.0f,0.0f );
	angle = YR_Vector3( 0.0f,0.0f,0.0f );

	YR_Vector3 eye = YR_Vector3(pos.x, pos.y + 5.0f, pos.z - 80.0f);
	YR_Vector3 focus = YR_Vector3(pos.x, pos.y + 3.5f, pos.z);
	YRCamera.SetEye(eye.GetDXFLOAT3());
	YRCamera.SetFocus(focus.GetDXFLOAT3());
	if (now_player == 1)
	{
		anim->NodeChange(model_motion.win_R);
	}
	else
	{
		anim->NodeChange(model_motion.win_L);
	}
	anim->PlayAnimation(0, false);
	ChangeFace(FaceAnim::TOZI);
	FaceAnimation(0.0f);
	win_state = WIN_PERFORMANCE_KNIGHT::SET;
	anim_ccodinate = 1.1f;
}

bool Knight::WinPerformance(float elapsed_time)
{
	YR_Vector3 focus;
	YR_Vector3 eye;
	/*focus = YR_Vector3(pos.x + focus_plus.x, pos.y + focus_plus.y, pos.z + focus_plus.z);
	eye = YR_Vector3(pos.x + eye_plus.x, pos.y + eye_plus.y, pos.z + eye_plus.z);
	YRCamera.SetEye(eye.GetDXFLOAT3());
	YRCamera.SetFocus(focus.GetDXFLOAT3());
	win_timer += elapsed_time;*/

	float apply = 1.0f;
	if (now_player == 1)
	{
		apply = 1.0f;
	}
	else
	{
		apply = -1.0f;
	}
	//勝利演出の行動をする
	//カメラもこちらで動かす
	switch (win_state)
	{
	case Knight::WIN_PERFORMANCE_KNIGHT::SET:
		//原点を設定(初期化)
		eye = YR_Vector3(pos.x, pos.y + 5.0f, pos.z - 80.0f);
		focus = YR_Vector3(pos.x, pos.y + 3.5f, pos.z);
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
		win_state = WIN_PERFORMANCE_KNIGHT::ZOOM;
		break;
	case Knight::WIN_PERFORMANCE_KNIGHT::ZOOM:
		eye = YR_Vector3(pos.x, pos.y + 5.0f, pos.z - 50.0f);
		focus = YR_Vector3(pos.x, pos.y + 3.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 0.1f * elapsed_time);
		if (win_timer > 1.5f)
		{
			ChangeFace(FaceAnim::YEAH);
			FaceAnimation(0.0f);
			win_state = WIN_PERFORMANCE_KNIGHT::PULL;
		}
		break;
	case Knight::WIN_PERFORMANCE_KNIGHT::PULL:
		eye = YR_Vector3(pos.x+(apply*-18.0f), pos.y + 5.0f, pos.z - 43.0f);
		focus = YR_Vector3(pos.x, pos.y + 3.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 8.0f * elapsed_time);
		if (win_timer > 1.8f)
		{
			win_state = WIN_PERFORMANCE_KNIGHT::STOP;
		}
		break;
	case Knight::WIN_PERFORMANCE_KNIGHT::STOP:
		if (win_timer > 3.0f)
		{
			win_state = WIN_PERFORMANCE_KNIGHT::FINISH;
		}
		break;
	case Knight::WIN_PERFORMANCE_KNIGHT::FINISH:
		return true;
		break;
	default:
		break;
	}
	win_timer += elapsed_time;

	return false;
}

bool Knight::Intro(float elapsed_time)
{
	//イントロの行動をする
	//カメラもこちらで動かす
	YR_Vector3	focus;
	YR_Vector3	eye;

	/*focus = YR_Vector3(pos.x + Getapply(focus_plus.x), pos.y + focus_plus.y, pos.z + focus_plus.z);
	eye = YR_Vector3(pos.x + Getapply(eye_plus.x), pos.y + eye_plus.y, pos.z + eye_plus.z);*/
	/*focus = YR_Vector3(pos.x + focus_plus.x, pos.y + focus_plus.y, pos.z + focus_plus.z);
	eye = YR_Vector3(pos.x + eye_plus.x, pos.y + eye_plus.y, pos.z + eye_plus.z);
	YRCamera.SetEye(eye.GetDXFLOAT3());
	YRCamera.SetFocus(focus.GetDXFLOAT3());*/
	switch (intro_state)
	{
	case Knight::INTRO_KNIGHT::SET:
		//原点を設定(初期化)
		eye = YR_Vector3(pos.x - Getapply(52.0f), pos.y + 1.8f, pos.z + 70.0f);
		focus = YR_Vector3(pos.x, pos.y + 1.8f, pos.z + 1.8f);
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
		intro_state = INTRO_KNIGHT::WAIT;
		lip_text = RandTextSelect();
		GetSound().SESinglePlay(SEKind::INTRO_WIND);
		break;
	case Knight::INTRO_KNIGHT::WAIT:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x - Getapply(52.0f), pos.y +1.8f, pos.z + 50.0f);
		focus = YR_Vector3(pos.x, pos.y + 1.8f, pos.z + 1.8f);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f*elapsed_time);
		if (intro_timer > 1.0f)
		{
			intro_state = INTRO_KNIGHT::ZOOM_SET;
		}
		break;
	case Knight::INTRO_KNIGHT::ZOOM_SET:
		//原点を設定(初期化)
		eye = YR_Vector3(pos.x - Getapply(25.0f), pos.y + 5.0f, pos.z - 45.0f);
		focus = YR_Vector3(pos.x, pos.y + 5.0f, pos.z);
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
		intro_state = INTRO_KNIGHT::ZOOM;
		ChangeFace(FaceAnim::NORMAL_LIP_SYNC);
		break;
	case Knight::INTRO_KNIGHT::ZOOM:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x, pos.y + 5.0f, pos.z - 50.0f);
		focus = YR_Vector3(pos.x, pos.y + 5.0f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 0.5f*elapsed_time);
		text_on = true;

		if (intro_timer > 3.0f)
		{
			intro_state = INTRO_KNIGHT::PULL;
			ChangeFace(FaceAnim::YARUKI);
			text_on = false;
		}
		break;
	case Knight::INTRO_KNIGHT::PULL:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x, pos.y + 2.5f, pos.z - 70.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 4.0f)
		{
			intro_state = INTRO_KNIGHT::PUSH;
		}
		break;
	case Knight::INTRO_KNIGHT::PUSH:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x, pos.y + 2.5f, pos.z - 41.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 4.5f)
		{
			intro_state = INTRO_KNIGHT::PULL_2;
		}
		break;
	case Knight::INTRO_KNIGHT::PULL_2:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x, pos.y + 30.0f, pos.z - 85.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 4.9f)
		{
			intro_state = INTRO_KNIGHT::PUSH_2;
		}
		break;
	case Knight::INTRO_KNIGHT::PUSH_2:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x+Getapply(27.0f), pos.y + 5.4f, pos.z - 38.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 5.3f)
		{
			intro_state = INTRO_KNIGHT::FIN_SET;
		}
		break;
	case Knight::INTRO_KNIGHT::FIN_SET:
		//原点を設定(初期化)
		eye = YR_Vector3(pos.x - Getapply(30.0f), pos.y - 16.0f, pos.z - 23.0f);
		focus = YR_Vector3(pos.x , pos.y, pos.z - 5.4f);
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
		intro_state = INTRO_KNIGHT::FIN;
		break;
	case Knight::INTRO_KNIGHT::FIN:
		//指定した位置までカメラを動かしていく(更新)
		eye = YR_Vector3(pos.x - Getapply(48.0f), pos.y - 16.0f, pos.z - 23.0f);
		focus = YR_Vector3(pos.x - Getapply(5.0f), pos.y, pos.z - 5.4f);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 8.0f * elapsed_time);
		if (intro_timer > 6.0f)
		{
			intro_state = INTRO_KNIGHT::FINISH;
		}
		break;
	case Knight::INTRO_KNIGHT::FINISH:
		return true;
		break;
	default:
		break;
	}
	intro_timer += elapsed_time;
	FaceAnimation(elapsed_time);
	/*if (intro_timer > 6.0f)
	{
		return true;
	}*/

	return false;
}


//イントロ後、ゲーム開始までの設定を行う
void Knight::ReadySet()
{
	text_on = false;
	if (rightOrleft > 0)
	{
		anim->NodeChange(model_motion.wait_R);
	}
	else
	{
		anim->NodeChange(model_motion.wait_L);
	}
	ChangeFace(FaceAnim::NORMAL);
	FaceAnimation(0.0f);
	//GetSound().SEPlay(SEKind::INTRO_WIND);
}


//カメラ調整用デバッグ関数
void Knight::IntroDEBUG()
{
#ifdef EXIST_IMGUI
	if (Get_Use_ImGui())
	{
		ImGui::Begin(u8"イントロカメラ");
		int state_in = scastI(intro_state);
		ImGui::InputInt(u8"ステート", &state_in, 1, 10);
		if (ImGui::TreeNode("Input"))
		{
			ImGui::InputFloat("eye.X", &eye_plus.x, 0.1f, 1.0f);
			ImGui::InputFloat("eye.Y", &eye_plus.y, 0.1f, 1.0f);
			ImGui::InputFloat("eye.Z", &eye_plus.z, 0.1f, 1.0f);
			ImGui::InputFloat("focus.X", &focus_plus.x, 0.1f, 1.0f);
			ImGui::InputFloat("focus.Y", &focus_plus.y, 0.1f, 1.0f);
			ImGui::InputFloat("focus.Z", &focus_plus.z, 0.1f, 1.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Slider"))
		{
			ImGui::SliderFloat("eye_X", &eye_plus.x, -350.1f, 350.1f);
			ImGui::SliderFloat("eye_Y", &eye_plus.y, -350.1f, 350.1);
			ImGui::SliderFloat("eye_Z", &eye_plus.z, -350.1f, 350.1);
			ImGui::SliderFloat("focus_X", &focus_plus.x, -350.1f, 350.1);
			ImGui::SliderFloat("focus_Y", &focus_plus.y, -350.1f, 350.1);
			ImGui::SliderFloat("focus_Z", &focus_plus.z, -350.1f, 350.1);
			ImGui::TreePop();
		}
		ImGui::Text("intro_timer = %.3f", intro_timer);
		ImGui::End();
	}
#endif // USE_IMGUI

}

void Knight::WinDEBUG()
{
#ifdef EXIST_IMGUI
	if (Get_Use_ImGui())
	{
		ImGui::Begin(u8"勝利カメラ");
		int state_in = scastI(win_state);
		ImGui::InputInt(u8"ステート", &state_in, 1, 10);
		if (ImGui::TreeNode("Input"))
		{
			ImGui::InputFloat("eye.X", &eye_plus.x, 0.1f, 1.0f);
			ImGui::InputFloat("eye.Y", &eye_plus.y, 0.1f, 1.0f);
			ImGui::InputFloat("eye.Z", &eye_plus.z, 0.1f, 1.0f);
			ImGui::InputFloat("focus.X", &focus_plus.x, 0.1f, 1.0f);
			ImGui::InputFloat("focus.Y", &focus_plus.y, 0.1f, 1.0f);
			ImGui::InputFloat("focus.Z", &focus_plus.z, 0.1f, 1.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Slider"))
		{
			ImGui::SliderFloat("eye_X", &eye_plus.x, -350.1f, 350.1f);
			ImGui::SliderFloat("eye_Y", &eye_plus.y, -350.1f, 350.1);
			ImGui::SliderFloat("eye_Z", &eye_plus.z, -350.1f, 350.1);
			ImGui::SliderFloat("focus_X", &focus_plus.x, -350.1f, 350.1);
			ImGui::SliderFloat("focus_Y", &focus_plus.y, -350.1f, 350.1);
			ImGui::SliderFloat("focus_Z", &focus_plus.z, -350.1f, 350.1);
			ImGui::TreePop();
		}
		ImGui::Text("intro_timer = %.3f", win_timer);
		ImGui::End();
	}
#endif // USE_IMGUI

}

//攻撃当たり判定が全て終了しているか確認する
bool Knight::AttackEndCheck()
{
	if (!atk.empty())
	{
		for (auto& a : atk)
		{
			if (!a.fin)
			{
				return false;
			}
		}
	}

	return true;
}


//終了した攻撃当たり判定を全て消去する。
void Knight::EndAttackErase()
{
	if (!atk.empty())
	{
		auto result = std::remove_if(atk.begin(), atk.end(),
			[](AttackBox& a)
			{
				return a.fin;
			});
		atk.erase(result, atk.end());
	}

	if (!projectile_atk.empty())
	{
		auto result = std::remove_if(projectile_atk.begin(), projectile_atk.end(),
			[](AttackBox& a)
			{
				return a.fin;
			});
		projectile_atk.erase(result, projectile_atk.end());
	}

	/*if (!projectile_atk.empty())
	{
		for (std::vector<AttackBox>::iterator& m = projectile_atk.begin(); m != projectile_atk.end();)
		{
			if (m->fin)
			{
				m = projectile_atk.erase(m);
			}
			else
			{
				m++;
			}
		}
	}*/
}

//全ての攻撃当たり判定を消去する
void Knight::AllAttackClear()
{
	if (!atk.empty())
	{
		for (int a = 0; a < atk.size(); a++)
		{
			atk[a].fin = true;
		}

		EndAttackErase();
	}
}

//攻撃判定が存在する時のみ更新
void Knight::AttackUpdate(float elapsed_time)
{
	if (YRCamera.camera_state == Camera::CAMERA_STATE::MAIN)
	{
		if (!atk.empty())
		{
			for (auto& a : atk)
			{
				a.Update(pos, elapsed_time);
			}
		}

		if (!projectile_atk.empty())
		{
			for (int i = 0; i < projectile_atk.size(); i++)
			{
				projectile_atk[i].Update(pos, elapsed_time);
			}
		}
	}
}

//攻撃判定が存在する時のみ更新(当たり判定が取得した結果をプレイヤーに送る)
void Knight::HitResultUpdate()
{
	if (!atk.empty())
	{
		for (auto& a : atk)
		{
			if (a.hit_result != HitResult::NONE)
			{
				//攻撃が当たっていた場合、その内容を保存する
				hit_result = a.hit_result;
				//行動終了フラグをオンに
				finish = true;
			}
		}
	}
}


//顔のテクスチャアニメーション
void Knight::FaceAnimation(float elapsed_time)
{
	switch (face_anim)
	{
	case Knight::FaceAnim::NORMAL:
		eye_offset = face_eye_offset[scastI(FaceEye_Num::NORMAL_EYE)];
		//mouth_offset = face_mouth_offset[scastI(FaceMouth_Num::NORMAL_MOUSE)];
		face_mouth_num = FaceMouth_Num::NORMAL_MOUSE;
		face_wink_interval_timer += elapsed_time;
		if (face_wink_interval_timer > wink_interval)
		{
			face_anim = FaceAnim::WINK;
			face_wink_interval_timer = 0.0f;
		}
		break;
	case Knight::FaceAnim::NORMAL_LIP_SYNC:
		FaceLipSync(elapsed_time);
		face_wink_interval_timer += elapsed_time;
		if (face_wink_interval_timer > wink_interval)
		{
			if (FaceWink_bool(elapsed_time))
			{
				face_wink_interval_timer = 0.0f;
			}
		}
		break;
	case Knight::FaceAnim::WINK:
		FaceWink(elapsed_time);
		break;
	case Knight::FaceAnim::Damage:
		eye_offset = face_eye_offset[scastI(FaceEye_Num::KAOMOJI)];
		face_mouth_num = FaceMouth_Num::POKAN;
		break;
	case Knight::FaceAnim::YARUKI:
		eye_offset = face_eye_offset[scastI(FaceEye_Num::TURI)];
		face_mouth_num = FaceMouth_Num::NORMAL_MOUSE;
		break;
	case Knight::FaceAnim::KOUHUN:
		eye_offset = face_eye_offset[scastI(FaceEye_Num::KIRAME)];
		face_mouth_num = FaceMouth_Num::OOGUTI;
		break;
	case Knight::FaceAnim::TOZI:
		eye_offset = face_eye_offset[scastI(FaceEye_Num::CLOSE)];
		face_mouth_num = FaceMouth_Num::TOZI;
		break;
	case Knight::FaceAnim::YEAH:
		eye_offset = face_eye_offset[scastI(FaceEye_Num::TURI)];
		face_mouth_num = FaceMouth_Num::OOGUTI;
		break;
	default:
		break;
	}
}


//瞬き
void Knight::FaceWink(float elapsed_time)
{
	face_wink_time += elapsed_time;
	if (face_wink_time > 0.05f)
	{
		switch (wink_state)
		{
		case Wink_State::FIRST:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK1)];
			wink_state = Wink_State::SECOND;
			break;
		case Wink_State::SECOND:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK2)];
			wink_state = Wink_State::THIRD;
			break;
		case Wink_State::THIRD:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::CLOSE)];
			wink_state = Wink_State::FOURTH;
			break;
		case Wink_State::FOURTH:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK2)];
			wink_state = Wink_State::FIVE;
			break;
		case Wink_State::FIVE:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK1)];
			wink_state = Wink_State::SIX;
			break;
		case Wink_State::SIX:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::NORMAL_EYE)];
			wink_state = Wink_State::FIRST;
			face_anim = FaceAnim::NORMAL;
			break;
		default:
			break;
		}
		face_wink_time = 0.0f;
	}
}


bool Knight::FaceWink_bool(float elapsed_time)
{
	//ウインクが終わったらtrueを返す
	face_wink_time += elapsed_time;
	if (face_wink_time > 0.05f)
	{
		switch (wink_state)
		{
		case Wink_State::FIRST:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK1)];
			wink_state = Wink_State::SECOND;
			break;
		case Wink_State::SECOND:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK2)];
			wink_state = Wink_State::THIRD;
			break;
		case Wink_State::THIRD:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::CLOSE)];
			wink_state = Wink_State::FOURTH;
			break;
		case Wink_State::FOURTH:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK2)];
			wink_state = Wink_State::FIVE;
			break;
		case Wink_State::FIVE:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::WINK1)];
			wink_state = Wink_State::SIX;
			break;
		case Wink_State::SIX:
			eye_offset = face_eye_offset[scastI(FaceEye_Num::NORMAL_EYE)];
			wink_state = Wink_State::FIRST;
			return true;
			break;
		default:
			break;
		}
		face_wink_time = 0.0f;
	}

	return false;
}


//口パクの処理
void Knight::FaceLipSync(float elapsed_time)
{
	lip_sync_time += elapsed_time;
	if (lip_sync_time > 0.1f)
	{
		for (int i = 0; i < 300; i++)
		{
			int mouth = rand() % lip_sync_can.size();
			if (mouth != scastI(face_mouth_num))
			{
				face_mouth_num = lip_sync_can[mouth];
				break;
			}
		}

		lip_sync_time = 0.0f;
	}
}



//表情変更
void Knight::ChangeFace(FaceAnim anim)
{
	face_anim = anim;
	face_wink_time = 0.0f;
	wink_state = Wink_State::FIRST;
	face_wink_interval_timer = 0.0f;
	face_wink_time = 0.0f;
	//表情がまばたき込みの場合のみ
	if (face_anim == FaceAnim::NORMAL||
		face_anim ==FaceAnim::NORMAL_LIP_SYNC)
	{
		//ランダムでまばたきの間隔を決定
		wink_interval =static_cast<float>((rand() % 5) + 1);
	}
}

void Knight::AttackDetailsSet(const AttackState& attack_state)
{
	//攻撃ごとに初回だけ通す処理を記述する
	switch (attack_state)
	{
	case AttackState::NONE:
		break;
	case AttackState::JAKU:
		ChangeFace(FaceAnim::YARUKI);
		break;
	case AttackState::THU:
		break;
	case AttackState::D_KYO:
		break;
	case AttackState::D_JAKU:
		break;
	case AttackState::D_THU:
		break;
	case AttackState::U_KYO:
		break;
	case AttackState::A_JAKU:
		break;
	case AttackState::A_THU:
		break;
	case AttackState::A_KYO:
		break;
	case AttackState::A_UKYO:
		//speed.y = 0.0f;
		//speed_Y.Set(attack_list[scastI(attack_state)].advance_speed);
		break;
	case AttackState::STEAL:
		break;
	case AttackState::SLOW:
		break;
	case AttackState::TRACK_DASH:
		break;
	case AttackState::JAKU_RHURF:
		break;
	case AttackState::THU_RHURF:
		break;
	case AttackState::KYO_RHURF:
		GetSound().SESinglePlay(SEKind::SPECIAL_ATTACK);
		break;
	case AttackState::JAKU_LHURF:
		break;
	case AttackState::THU_LHURF:
		break;
	case AttackState::KYO_LHURF:
		break;
	case AttackState::SPECIAL_ATTACK:
		YRCamera.RequestCamera(Camera::Request::HOLD, now_player);
		camera_state_knight = CAMERA_STATE_KNIGHT::FIRST;
		ChangeFace(FaceAnim::KOUHUN);
		lumi_material= Model::Material_Attribute::SWORD;
		GetSound().SESinglePlay(SEKind::SPECIAL_ATTACK);
		YRGetEffect().PlayEffect(EffectKind::WIND, DirectX::XMFLOAT3(pos.x, pos.y - 5.0f, pos.z), DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);
		YRGetEffect().PlayEffect(EffectKind::WIND, DirectX::XMFLOAT3(pos.x, pos.y - 5.0f, pos.z), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);
		cut_in_timer = 0.0f;
		break;
	case AttackState::DESIRE_SPECIAL:
		break;
	case AttackState::DESIRE_METEOR:
		break;
	case AttackState::EXTENDATK:
		break;
	case AttackState::JAKU_THU:
		break;
	case AttackState::JAKU_KYO:
		break;
	case AttackState::ATTACK_END:
		break;
	default:
		break;
	}
}

std::wstring Knight::RandTextSelect()
{
	TextList rnd = static_cast<TextList>(rand() % scastI(TextList::TEXT_END));

	switch (rnd)
	{
	case Knight::TextList::NORMAL://通常
		return std::wstring(L"さぁ、始めようか！");
		break;
	case Knight::TextList::WARLIKE://好戦的
		return std::wstring(L"一気に行くよ！");
		break;
	case Knight::TextList::CRIOSITY://好奇心
		return std::wstring(L"私と戦いたいの？");
		break;
	case Knight::TextList::TEXT_END:
		break;
	default:
		break;
	}

}

void Knight::TextDraw()
{
	if (!text_on)
	{
		return;
	}

	FRAMEWORK.font->Begin(FRAMEWORK.context.Get());
	FRAMEWORK.font->Draw(
		static_cast<float>(FRAMEWORK.SCREEN_WIDTH / 2.0f) - 150.0f,
		static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) - 100.0f, lip_text.c_str());
	FRAMEWORK.font->End(FRAMEWORK.context.Get());
}

void Knight::HitBoxTransition(HitBoxState state)
{
	for (int h = 0; h < hit.size(); h++)
	{
		hit[h].state = state;
	}
}