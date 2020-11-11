#include "Knight.h"
#include "PlayerBase.h"
#include "Key.h"
#include "YRGamePad.h"
#include "camera.h"
#include "framework.h"
#include <algorithm>
#include "World.h"
#include "Effect.h"

//#define _CRTDBG_MAP_ALLOC						// malloc�ɂ�郁�������[�N���o��CPP�t�@�C�����ƍs���o�͎w��
//#define DBG_NEW new( _NORMAL_BLOCK , __FILE__ , __LINE__)	// new �ɂ�郁�������[�N���o��CPP�t�@�C�����ƍs���o�͎w��
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
	specialfream = 0.0f;
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
	win_state = WIN_PERFORMANCE_KNIGHT::CAMERA_ZOOM;

	fream = 0.0f;

	production_time = 0.0f;
	anim_ccodinate = 2.0f;

	stop_state = 0;
	air_dash_state = AirDashState::NONE;

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

	eye_plus = YR_Vector3(0.0f, 0.0f, -10.0f);
	focus_plus = YR_Vector3(0.0f, 0.0f, 0.0f);
	face_mouth_num = FaceMouth_Num::NORMAL_MOUSE;
	ChangeFace(FaceAnim::NORMAL_LIP_SYNC);

	text_on = false;
	hit_state_n_set = false;

	lumi_material = Model::Material_Attribute::NONE;

	effect_timer = 0.0f;

	if (now_player == 1)
	{
		anim->NodeChange(model_motion.intro_R);
	}
	else
	{
		anim->NodeChange(model_motion.intro_L);
	}
}

void Knight::Uninit()
{
	//���f���n��S�ĉ������
	main.reset();
	main = nullptr;
	anim.reset();
	anim = nullptr;
	model_motion.wait_R.reset();
	model_motion.wait_R = nullptr;
	model_motion.jaku_R.reset();
	model_motion.jaku_R = nullptr;
	model_motion.special_R.reset();
	model_motion.special_R = nullptr;
	model_motion.damage_R_g_u.reset();
	model_motion.damage_R_g_u = nullptr;

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

	model_motion.model_R.clear();
}

void Knight::Update(float decision, float elapsed_time)
{
	//elapsed_time��0�A�܂�q�b�g�X�g�b�v�ȂǂŃQ�[�����~�܂��Ă���ꍇ
	if (elapsed_time == 0.0f)
	{
		//����return�ŏ����ɓ���Ȃ��悤�ɂ���
		return;
	}
	finish = false;
	DamageCheck(decision);
	WaitAnimSet();
	if (pos.y <= POS_Y)
	{
		ground = true;
		speed_Y.Set(0.0f);
	}
	else
	{
		ground = false;
	}

	AttackUpdate(elapsed_time);	//�U������̍X�V

	//-----------------------------------------------------
	// *�T�v*
	//�E�U�����󂯂�ȂǑ���s�\�̏�Ԃ̂݃X�e�[�g�ŋ敪�����A
	//�@����ȊO�͑�����󂯕t����̂�CANACT�ŏ������Ă���
	//-----------------------------------------------------
	switch (act_state)
	{
	case ActState::STATENONE:
		//�X�e�[�g��D��ꂽ��ԁB����ɒ͂܂�Ă�
		StateNone(elapsed_time);
		break;
	case ActState::WAKE:
		//�_�E������N���オ��
		WakeUp();
		break;
	case ActState::PASSIVE:
		//�󂯐g���Ƃ�
		PassiveUpdate(elapsed_time);
		break;
	case ActState::DOWN:
		//�_�E�����
		DownUpdate();
		break;
	case ActState::FALL:
		//�󒆂Ń_�E�����Ă�����
		FallUpdate(elapsed_time);
		break;
	case ActState::KNOCK:
		//�U�����󂯂Ă̂�����
		KnockUpdate(elapsed_time);
		break;
	case ActState::SLAM:
		//�@���������
		SlamUpdate(elapsed_time);
		break;
	case ActState::ATTACK:
		//�U����
		Attack(decision, elapsed_time);
		break;
	case ActState::GUARD:
		Guard(decision);
		GuardBack(elapsed_time);
		Squat();
		break;
	default:
		//���̑��ʏ���

		//�p�x��߂�
		angle.z = 0.0f;
		if (ground)
		{
			//�n�ʐݒu��
			//pos.y = POS_Y;	//�n�ʂɐݒu���Ă���Ƃ��͍������Œ肷��
			trackgauge = Track_max;	//�z�[�~���O�_�b�V���񐔂�߂�
			if (act_state == ActState::JUMP)
			{
				//�X�e�[�g���W�����v�̎���NONE�ɖ߂�
				act_state = ActState::NONE;
			}
		}
		else
		{
			//�󒆎�
			if (pos.y > POS_Y)
			{
				//�d�͂�t�^����
				pos.y -= gravity*elapsed_time;
			}
		}

		{
		//�U�����s���Ă��Ȃ��Ƃ��̏���
			if (ground && !step)
			{
				jumpcount = 2;
				air_dash_count = 1;
				if (rightOrleft != decision)
				{
					//�O��t���[���ƍ���̌������قȂ����ꍇ
					if (act_state == ActState::WAIT)
					{
						if (decision > 0)
						{
							//�E�����̎�
							anim->NodeChange(model_motion.wait_R);
							anim_ccodinate = ac_act[scastI(act_state)].fream;
						}
						else
						{
							//�������̎�
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
			//���Ⴊ�݂�K�[�h�Ȃǂ��ɔ���
			Squat();
			Guard(decision);
			//�K�[�h�łȂ���
			if (act_state != ActState::GUARD)
			{
				Step(elapsed_time);
				if (step)
				{
					//�X�e�b�v���͏������Ȃ�
				}
				else
				{

					//�n�ʂɂ��Ă���ꍇ�̂�
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
			hit[list].Update(pos, hitparam_list[list].attack_parameter[scastI(attack_state)], elapsed_time);
		}
		else
		{
			hit[list].Update(pos, hitparam_list[list].act_parameter[scastI(act_state)], elapsed_time);
		}
	}

	GuardAnimSet();

	if (!attack)
	{
		if (act_state == ActState::DASH || act_state == ActState::BACK)
		{
			//����̍s�����͎w�肵���l������ׂȂɂ����������Ȃ�
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

	EndAttackErase();			//�U������̏���

	FaceAnimation(elapsed_time);
}

void Knight::AttackInput()
{
	
	//-------------------------------------------------------------------
	// *�T�v*
	//�E�R�}���h�͓��ނ̂�
	//--------------------------------------------------------------------
	//�U���̃��X�g�̓��͔����S�Ċm�F����(0�͍U�������ׁ̈A1����m�F���Ă���)
	for (int list = 1; list < attack_list.size(); list++)
	{
		int button = scastI(attack_list[list].linkage_button);
		Command now_com = attack_list[list].linkage_command;
		if (pad->x_input[button] == 1)
		{
			pad->que.back().timer = 0;
			//�R�}���h����
			Command command = pad->com_list.CommandCheack(button, pad->input_history.data(), rightOrleft);
			if (now_com == command)
			{
				//�w�肵���R�}���h�������ŋ󒆁A�n��̏����������������ꍇ
				if (ground)
				{
					//���ݒn��ɂ���ꍇ�͍U�����󒆐�p����Ȃ��ꍇ�͍s��
					if (attack_list[list].ground_on < Ground_C::GROUND)
					{
						continue;
					}
				}
				else
				{
					//���݋󒆂ɂ���ꍇ�͍U�����n���p����Ȃ��ꍇ�͍s��
					if (attack_list[list].ground_on == Ground_C::GROUND)
					{
						continue;
					}
				}

				if (attack_list[list].squat_on)
				{
					//�U�����X�g�ɂ��Ⴊ�ݔ��肪�ݒ肳��Ă����ꍇ�A���݂̃X�e�[�g�����Ⴊ�݂��m�F����
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
					//�ݒ肳��Ă��Ȃ��ꍇ�͂��Ⴊ��łȂ����m�F����
					if (act_state == ActState::SQUAT || pad->x_input[scastI(PAD::STICK_D)] > 0 ||
						pad->x_input[scastI(PAD::STICK_LDown)] > 0 || pad->x_input[scastI(PAD::STICK_RDown)] > 0)
					{
						if (static_cast<AttackState>(list) != AttackState::TRACK_DASH)
						{
							//�z�[�~���O�_�b�V���̏ꍇ�͂��Ⴊ��łĂ��o����悤�ɔ��肷��
							continue;
						}
					}
				}
				if (attack_list[list].linkage_stick != PAD::BUTTOM_END)
				{
					//�X�e�B�b�N�̓��͂��w�肳��Ă���ꍇ�m�F����
					if (attack_list[list].linkage_stick == PAD::STICK_D)
					{
						//���X�e�B�b�N���͂̏ꍇ�͕ʂ̔�����Ƃ�
						if (pad->x_input[scastI(PAD::STICK_D)] == 0&& pad->x_input[scastI(PAD::STICK_RDown)] == 0&& pad->x_input[scastI(PAD::STICK_LDown)] == 0)
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
				//���ۂ̍U�����e
				int real = scastI(attack_list[list].real_attack);
				if (last_attack == AttackState::COMBO_X && attack_list[real].combo == last_attack)
				{
					//�R���{��Ԃɓ����Ă���ꍇ�̓R���{�֐��ōU���̃X�e�[�g�����肷��
					ComboUpdate();
					return;
				}

				
				//int next = scastI(attack_list[list].real_attack);
				if (attack_list[real].need_gauge <= gauge)
				{
					//�Q�[�W�̕K�v�ʂ��m�F����
					//�ʏ�̍U���̏ꍇ�A���ۂ̍U�����e�͓��ꂸ�ɍU���������̂܂ܓ����
					attack_state = static_cast<AttackState>(list);
				}
				else
				{
					//�Q�[�W������Ȃ��ꍇ�w�肵���Z���o��
					attack_state = attack_list[real].aid_attack_name;
				}

				//�Ō�ɍs�����U���������������ꍇ�A�R���{���m�F���đ��݂���Ȃ�R���{���J�n����
				if (last_attack == attack_state)
				{
					if (ComboSet())
					{
						return;
					}
					//attack_state = attack_list[real].next_attack;
					//next = scastI(attack_list[list].next_attack);
				}

				if (attack_state == AttackState::TRACK_DASH && trackgauge < 1)
				{
					continue;
				}

				//�U�������肷��
				//pad->com_list.Reset();
				//���ݍU�����肪�o�Ă���Ȃ�S�ď�������
				AllAttackClear();
				//���̍U�����L�����Z�����邽�߂̏�����ۑ�����
				atk_result = attack_list[real].conditions_hit;
				//�U����ۑ�����
				last_attack = attack_state;
				//�U���̌��ʂ�������
				hit_result = HitResult::NOT_OCCURRENCE;
				//�U�����t���O���I����
				attack = true;
				//�ړ��t���O���I�t��
				moveflag = false;
				//�s���X�e�[�g���U����
				act_state = ActState::ATTACK;
				//�����t���[��������
				fream = attack_list[real].attack_single[0].fream;
				//�A�j���[�V�������x���w��
				anim_ccodinate = ac_attack[real].fream;
				//�U���ԍ���������
				attack_list[real].now_attack_num = 0;
				//�U�������O�̑O�i������ݒ肷��(�n��U���̂�)
				if (attack_list[real].ground_on == Ground_C::GROUND)
				{
					speed_X.Set(attack_list[real].advance_speed);
				}
				if (!ground)
				{
					//�W�����v�t���O���ݒ肵�Ă���
					jumpflag = true;
					max_jump_flag = true;
				}
				//�㌄��������
				later = non_target;
				//�J���������p�ϐ���������
				production_time = 0.0f;
				//�`����Z�b�g
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.model_R[real], scastI(AnimAtk::FREAM));
				}
				else
				{
					anim->NodeChange(model_motion.model_L[real], scastI(AnimAtk::FREAM));
				}
				//�U�����ƂɌʂ̐ݒ���s��
				AttackDetailsSet(attack_list[real].combo);
				return;
			}
		}
	}
}

void Knight::AttackSwitch(float decision, float elapsed_time)
{
	switch (attack_state)
	{
	case AttackState::NONE:
		break;
	case AttackState::SLOW:
		//����
		Slow(elapsed_time);
		break;
	case AttackState::STEAL:
		//����̃X�e�[�g��D���B�͂�
		Steal(elapsed_time);
		break;
	case AttackState::JAKU:
		//��U��
		Jaku(elapsed_time);
		break;
	case AttackState::THU:
		//���U��(����)
		D_Thu(elapsed_time);
		break;
	case AttackState::D_KYO:
		//���̎��ɏo�鋭�U��
		Kyo(elapsed_time);
		break;
	case AttackState::D_JAKU:
		//���i��U��
		D_Jaku(elapsed_time);
		break;
	case AttackState::D_THU:
		//���i���U��
		D_Thu(elapsed_time);
		break;
	case AttackState::U_KYO:
		//��i���U��
		U_Kyo(elapsed_time);
		break;
	case AttackState::A_JAKU:
		//�󒆎�U��
		A_Jaku(elapsed_time);
		break;
	case AttackState::A_THU:
		//�󒆒��U��
		A_Thu(elapsed_time);
		break;
	case AttackState::A_KYO:
		//�󒆋��U��
		A_Kyo(elapsed_time);
		break;
	case AttackState::A_UKYO:
		//�󒆏㋭�U��
		A_UKyo(elapsed_time);
		break;
	case AttackState::JAKU_RHURF:
		Jaku_Rhurf(elapsed_time);
		break;
	case AttackState::THU_RHURF:
		Thu_Rhurf(elapsed_time);
		break;
	case AttackState::KYO_RHURF:
		Kyo_Rhurf(elapsed_time);
		break;
	case AttackState::TRACK_DASH:
		//�z�[�~���O�_�b�V��
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
		//��̎��ɏo�钆�U��
		Thu(elapsed_time);
		break;
	case AttackState::JAKU_KYO:
		//���̎��ɏo�鋭�U��
		Kyo(elapsed_time);
		break;
	case AttackState::COMBO_X:
		ComboX(decision,elapsed_time);
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
	//�󒆎�
	if (pos.y > POS_Y && speed_Y.speed == 0.0f)
	{
		//������ւ̗͂��Ȃ��ꍇ�A�d�͂�t�^����
		pos.y -= gravity * elapsed_time;
	}
	HitBoxTransition(HitBoxState::NOGUARD);

	AttackSwitch(decision, elapsed_time);

	//�U�����A�U�������������ꍇ�L�����Z�����čU������
	//�O��̍U�����ɂǂ̏����ŃL�����Z���ł��邩��ۑ����Ă邽�߁A����ȏ�Ȃ�L�����Z�����čU������
	if (hit_result >= atk_result)
	{
		AttackInput();
		Jump();
	}

	//�㌄���ɃL�����Z�����ċZ���s���邩�`�F�b�N����
	if (later > 0 && later < target_max)
	{
		later -= elapsed_time;
		//CancelList();
	}

	//�㌄����㌳�̃X�e�[�g�ɖ߂�
	if (later < 0.0f)
	{
		lumi_material = Model::Material_Attribute::NONE;
		finish = true;
		later = non_target;
		attack = false;
		//���ʂ�����������
		hit_result = HitResult::NOT_OCCURRENCE;
		//�L�����Z������������������
		atk_result = HitResult::NONE;
		//�ŏI���͓��e������������
		last_attack = AttackState::NONE;

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
			//�`����Z�b�g
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


//-------------------------------
//*�U���̃L�����Z�����X�g*
//----------------------------------------------------------------------
//�E���̃��X�g���ɂŒ�`�����s���͂ق��̍s������L�����Z�����ďo�����Ƃ��o����
//�E��{�I�ɃQ�[�W���g���Z�͑S�Z�L�����Z���o���\
//----------------------------------------------------------------------
void Knight::CancelList()
{
	//�W�����v
	Jump();

	//�z�[�~���O�_�b�V��
	if (pad->x_input[scastI(PAD::R_TRIGGER)] == 1)
	{
		if (trackgauge > 0)
		{
			//later = -1;
			pad->que.back().timer = 0;
			attack = true;
			tracking.Init();
			act_state = ActState::ATTACK;
			//�`����Z�b�g

			attack_state = AttackState::TRACK_DASH;
			moveflag = false;
			pos.y -= 50.0f;
			trackgauge--;
		}
	}

	//214�EX�R�}���h
	if (pad->x_input[scastI(PAD::X)] == 1)
	{
		Command command = pad->com_list.CommandCheack(scastI(PAD::X), pad->input_history.data(), rightOrleft);
		if (command == Command::LHURF)
		{
			later = non_target;
			attack = TRUE;
			moveflag = false;
			act_state = ActState::ATTACK;
			//�`����Z�b�g

			attack_state = AttackState::JAKU_RHURF;
			specialfream = 0;
		}
	}

	
	//�Q�[�W�Z
	if (pad->x_input[scastI(PAD::B)] == 1)
	{
		Command command = pad->com_list.CommandCheack(scastI(PAD::B), pad->input_history.data(), rightOrleft);
		if (command == Command::LHURF)
		{
			//214�EB�R�}���h
			if (power > 0)
			{
				later = -1;
				//�`����Z�b�g

				FastSet(YR_Vector3(pos.x - Getapply(50.0f), pos.y));
				attack = TRUE;
				moveflag = false;
				act_state = ActState::ATTACK;
				attack_state = AttackState::KYO_LHURF;
				//�G�t�F�N�g�Z�b�g

				pos.x = tracking.rival_Pos.x - Getapply(100.0f);
				pos.y = tracking.rival_Pos.y - 100.0f;
				power--;
			}
		}
		else if (command == Command::RHURF)
		{
			//236�EB�R�}���h
			later = -1;
			//�`����Z�b�g
			//�G�t�F�N�g�Z�b�g

			act_state = ActState::ATTACK;
			attack_state = AttackState::KYO_RHURF;
			specialfream = 0;
			power--;
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
	// *����*
	//�E�ߋ�2D�Ő��삵���Ƃ��͌����Ă�������ɂ���ĕ`���ς��Ă�����
	//�@�����3D�ł̐���ɂȂ�ׁAangle��ς��邱�ƂɎg��
	//�E�O��ł͋��U�����̉摜�̃T�C�Y���ʏ�ƈ���Ă�������
	//�@����������������炴��𓾂Ȃ������ƍl������
	//--------------------------------------------------------------------

	bool inversion = true;//���E���]�t���O

	DirectX::XMFLOAT4 material_color = { 1.0f,1.0f,1.0f,1.0f };


	//������
	if (rightOrleft < 0)
	{
		//angle.y = DirectX::XMConvertToRadians(180.0f);
		inversion = true;
		if (invincible)
		{
			//���G���
			lumi_material = Model::Material_Attribute::ALL;
		}
		else if (lumi_material == Model::Material_Attribute::ALL)
		{
			//���G����Ȃ�
			lumi_material = Model::Material_Attribute::NONE;
		}

	}
	//�E����
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
			//���G�I
			lumi_material = Model::Material_Attribute::ALL;
		}
		else if (lumi_material == Model::Material_Attribute::ALL)
		{
			lumi_material = Model::Material_Attribute::NONE;
		}
	}

	//���f���`��
	/*base_motion->Render(
		shader,
		pos.GetDXFLOAT3(),
		scale.GetDXFLOAT3(),
		angle.GetDXFLOAT3(),
		view, projection, light_direction, light_color, ambient_color, elapsed_time, 0.0f);*/

	if (scastI(YRCamera.camera_state) == now_player)
	{
		anim->UpdateAnimation(game_speed * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)],lumi_material, material_color);
	}
	if (YRCamera.camera_state == Camera::CAMERA_STATE::MAIN)
	{
		anim->UpdateAnimation(game_speed * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		anim->Draw(parallel_shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)],lumi_material, material_color);
	}
	if (YRCamera.GetRequest() == Camera::Request::WEAKEN)
	{
		anim->UpdateAnimation(game_speed* anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)],lumi_material, material_color);
	}
	if (YRCamera.camera_state == Camera::CAMERA_STATE::ZOOM_CAMERA)
	{
		anim->UpdateAnimation(game_speed * anim_ccodinate);
		anim->CalculateLocalTransform();
		anim->CalculateWorldTransform(pos.GetDXFLOAT3(), scale.GetDXFLOAT3(), angle.GetDXFLOAT3());
		anim->Draw(shader, view, projection, light_direction, light_color, ambient_color, eye_offset, face_mouth_offset[scastI(face_mouth_num)],lumi_material, material_color);
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
	//	//�g�����̕`��
	//}

	//if (atk[scastI(KNIGHTATK::THU_HADOU)].hit_ok)
	//{
	//	//���g�����̕`��B����܂Ƃ߂Ă���������
	//}

	//if (atk[scastI(KNIGHTATK::KYO_HADOU)].hit_ok)
	//{
	//	//�ł����g�����̕`��B����͑傫���Ⴄ����d���Ȃ�
	//}

	TextDraw();

	//�G�t�F�N�g
	GetEffect().CameraSet();

	//�G�t�F�N�g�`��
	GetEffect().Draw();

//�f�o�b�O��ԂȂ�
#if USE_IMGUI
		//�v���C���[�̒��S
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

		//���͊֌W��\�����Ă�
		//GetJoypadXInputState(DX_INPUT_PAD1, &input);

		//if (GetJoypadXInputState(DX_INPUT_PAD1, &input) != -1)
		//{
		//	// ��ʂ� XINPUT_STATE �̒��g��`��
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



//�c���̕`��
void Knight::DrawFastMove(YR_Vector3 position)
{
	if (!fast)
	{
		return;
	}
	//�E
	if (rightOrleft < 0)
	{
		
	}
	//��
	else
	{
		
	}
	fast = false;
}

//�c�����Z�b�g����
void Knight::FastSet(YR_Vector3 position)
{
	fast = true;
	FastPos = position;
}


//�ړ��֌W(�X�e�b�v�Ȃ�)
//-----------------------------------------------------------------
// *�T�v*
//�E�n��̏ꍇ�A�o�b�N�X�e�b�v�̏������s��(�O���ւ̓_�b�V�������̈�)
//�E�󒆂̏ꍇ�͋󒆃_�b�V���̐ݒ���s���A�����͕ʊ֐��ōs��
//-----------------------------------------------------------------
bool Knight::Step(float elapsed_time)
{
	//��
	if (!ground)
	{
		//�󒆍��X�e�b�v
		if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 1)
		{
			if (air_dash_count > 0)
			{
				//�W�����v���c���Ă���ꍇ�͋󒆃_�b�V�����s��
				//step = true;
				moveflag = false;
				jumpflag = false;
				jumpcount = 0;
				air_dash_count = 0;
				//�X�s�[�h�͂ǂ���������
				speed.x = -stepspeed;
				if (rightOrleft > 0)
				{
					//�󒆃o�b�N�_�b�V��
					act_state = ActState::AIR_B;
					air_dash_state = AirDashState::AIR_B;
					//�`����Z�b�g
					anim->NodeChange(model_motion.air_back_R, scastI(AnimAtk::FREAM));
					anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
					anim_ccodinate = ac_act[scastI(ActState::AIR_B)].fream;
				}
				else
				{
					//�󒆑O�_�b�V��
					act_state = ActState::AIR_F;
					air_dash_state = AirDashState::AIR_F;
					//�`����Z�b�g
					anim->NodeChange(model_motion.air_dash_L, scastI(AnimAtk::FREAM));
					anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
					anim_ccodinate = ac_act[scastI(ActState::AIR_F)].fream;
				}
			}
			else
			{
				//�W�����v���Ȃ��ꍇ�̓g���K�[���O��
				pad->dash_trigger = false;
			}
		}

		//�󒆉E�X�e�b�v
		if (pad->x_input[static_cast<int>(PAD::R_DASH)] == 1)
		{
			if (air_dash_count > 0)
			{
				//�W�����v���c���Ă���ꍇ�͋󒆃_�b�V�����s��
				//step = true;
				moveflag = false;
				jumpflag = false;
				jumpcount = 0;
				air_dash_count = 1;
				//�X�s�[�h�͂ǂ���������
				speed.x = stepspeed;
				if (rightOrleft > 0)
				{
					//�󒆑O�_�b�V��
					act_state = ActState::AIR_F;
					air_dash_state = AirDashState::AIR_F;
					//�`����Z�b�g
					anim->NodeChange(model_motion.air_dash_R, scastI(AnimAtk::FREAM));
					anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
					anim_ccodinate = ac_act[scastI(ActState::AIR_F)].fream;
				}
				else
				{
					//�󒆃o�b�N�_�b�V��
					act_state = ActState::AIR_B;
					air_dash_state = AirDashState::AIR_B;
					//�`����Z�b�g
					anim->NodeChange(model_motion.air_back_L, scastI(AnimAtk::FREAM));
					anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
					anim_ccodinate = ac_act[scastI(ActState::AIR_B)].fream;
				}
			}
			else
			{
				//�W�����v���Ȃ��ꍇ�̓g���K�[���O��
				pad->dash_trigger = false;
			}
		}
	}

	if (!step)
	{
		return false;
	}

	//�o�b�N�X�e�b�v
	if (act_state == ActState::BACK)
	{
		//�E����
		if (rightOrleft > 0)
		{
			//�E�����̃o�b�N�X�e�b�v��x�����̑��x��0�ɂȂ�
			//�Ȃ̂őS��0�ɂ��Ă���
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
				//���݂̃A�j���[�V�������o�b�N�X�e�b�v�̊J�n�A�j���[�V�����������ꍇ
				if (anim->GetEndAnim() == -1)
				{
					//�A�j���[�V�������I�������玝���A�j���[�V�����ɐ؂�ւ���
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
				//���݂̃A�j���[�V�������o�b�N�X�e�b�v�̎����A�j���[�V�����������ꍇ
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
			//�`����Z�b�g�͂��Ȃ����ǈړ��͈͂̃Z�b�g
			if (pos.x < Limit::Left_max)
			{
				pos.x = Limit::Left_max;
			}
			speed.x += (backstepD * elapsed_time);
			return false;
		}
		if (rightOrleft < 0)
		{
			//�t���R��
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
			//�`����Z�b�g�͂��Ȃ����ǈړ��͈͂̃Z�b�g
			if (pos.x > Limit::Right_max)
			{
				pos.x = Limit::Right_max;
			}
			if (!anim->GetLoopAnim())
			{
				//���݂̃A�j���[�V�������o�b�N�X�e�b�v�̊J�n�A�j���[�V�����������ꍇ
				if (anim->GetEndAnim() == -1)
				{
					//�A�j���[�V�������I�������玝���A�j���[�V�����ɐ؂�ւ���
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
				//���݂̃A�j���[�V�������o�b�N�X�e�b�v�̎����A�j���[�V�����������ꍇ
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

	return false;
}


//�󒆃_�b�V������
//-----------------------------------------------------------------
// *�T�v*
//�E�󒆃_�b�V���̏������s��
//�EAirDashState�ɂ���ď�����ς���
//-----------------------------------------------------------------
void Knight::AirDash(float elapsed_time)
{
	switch (air_dash_state)
	{
	case AirDashState::NONE:
		//�������Ȃ�
		break;
	case AirDashState::AIR_F:
	{
		//�󒆃_�b�V��
		speed.y = 0.0f;
		speed_X.Set(0.0f);
		speed_Y.Set(0.0f);
		//�d�͂̋t����t�^����
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
				air_dash_state = AirDashState::NONE;
				if (!attack)
				{
					//�`����Z�b�g
					act_state = ActState::JUMP;
					anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::TIMER));
					anim_ccodinate = ac_act[scastI(act_state)].timer;
				}
				return;
			}
			//�ړ��͈͂̃Z�b�g
			if (pos.x > Limit::Right_max)
			{
				pos.x = Limit::Right_max;
			}

			if (!attack)
			{
				//�U�����̓A�j���[�V�����̕ύX���s��Ȃ�
				if (!anim->GetLoopAnim())
				{
					//���݂̃A�j���[�V�������󒆃_�b�V���̊J�n�A�j���[�V�����������ꍇ
					if (anim->GetEndAnim() == -1)
					{
						//�A�j���[�V�������I�������玝���A�j���[�V�����ɐ؂�ւ���
						anim->NodeChange(model_motion.air_dash_R, scastI(AnimAtk::TIMER));
						anim_ccodinate = ac_act[scastI(act_state)].timer;
					}
				}
				else
				{
					//���݂̃A�j���[�V�������󒆃_�b�V���̎����A�j���[�V�����������ꍇ
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
				if (!attack)
				{
					//�`����Z�b�g
					act_state = ActState::JUMP;
					anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::TIMER));
					anim_ccodinate = ac_act[scastI(act_state)].timer;
				}
				return;
			}
			//�ړ��͈͂̃Z�b�g
			if (pos.x < Limit::Left_max)
			{
				pos.x = Limit::Left_max;
			}

			if (!attack)
			{
				//�U�����̓A�j���[�V�����̕ύX���s��Ȃ�
				if (!anim->GetLoopAnim())
				{
					//���݂̃A�j���[�V�������󒆃_�b�V���̊J�n�A�j���[�V�����������ꍇ
					if (anim->GetEndAnim() == -1)
					{
						//�A�j���[�V�������I�������玝���A�j���[�V�����ɐ؂�ւ���
						anim->NodeChange(model_motion.air_dash_L, scastI(AnimAtk::TIMER));
						anim_ccodinate = ac_act[scastI(act_state)].timer;
					}
				}
				else
				{
					//���݂̃A�j���[�V�������󒆃_�b�V���̎����A�j���[�V�����������ꍇ
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
		//�󒆃o�b�N�X�e�b�v
		speed.y = 0.0f;
		speed_X.Set(0.0f);
		speed_Y.Set(0.0f);
		//�d�͂̋t����t�^����
		pos.y += gravity * elapsed_time;
		if (rightOrleft > 0)
		{
			//�E����
			//�X�s�[�h���_�b�V���ȉ��ɂȂ�����I��
			if (speed.x > -dashspeed)
			{
				step = false;
				pad->dash_trigger = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				air_dash_state = AirDashState::NONE;
				moveflag = false;
				//�`����Z�b�g
				if (!attack)
				{
					act_state = ActState::JUMP;
					anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::TIMER));
					anim_ccodinate = ac_act[scastI(act_state)].timer;
				}
				return;
			}
			//�ړ��͈͂̃Z�b�g
			if (pos.x < Limit::Left_max)
			{
				pos.x = Limit::Left_max;
			}

			if (!attack)
			{
				//�U�����̓A�j���[�V�����̕ύX���s��Ȃ�
				if (!anim->GetLoopAnim())
				{
					//���݂̃A�j���[�V�������󒆃o�b�N�X�e�b�v�̊J�n�A�j���[�V�����������ꍇ
					if (anim->GetEndAnim() == -1)
					{
						//�A�j���[�V�������I�������玝���A�j���[�V�����ɐ؂�ւ���
						anim->NodeChange(model_motion.air_back_R, scastI(AnimAtk::TIMER));
						anim_ccodinate = ac_act[scastI(act_state)].timer;
					}
				}
				else
				{
					//���݂̃A�j���[�V�������󒆃o�b�N�X�e�b�v�̎����A�j���[�V�����������ꍇ
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
			//������
			//�X�s�[�h���_�b�V���ȉ��ɂȂ�����I��
			if (speed.x < dashspeed)
			{
				step = false;
				pad->dash_trigger = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				air_dash_state = AirDashState::NONE;
				moveflag = false;
				//�`����Z�b�g
				if (!attack)
				{
					act_state = ActState::JUMP;
					anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::TIMER));
					anim_ccodinate = ac_act[scastI(act_state)].timer;
				}
				return;
			}
			//�ړ��͈͂̃Z�b�g
			if (pos.x > Limit::Right_max)
			{
				pos.x = Limit::Right_max;
			}

			if (!attack)
			{
				//�A�j���[�V�����J��
				//�U�����̓A�j���[�V�����̕ύX���s��Ȃ�
				if (!anim->GetLoopAnim())
				{
					//���݂̃A�j���[�V�������󒆃o�b�N�X�e�b�v�̊J�n�A�j���[�V�����������ꍇ
					if (anim->GetEndAnim() == -1)
					{
						//�A�j���[�V�������I�������玝���A�j���[�V�����ɐ؂�ւ���
						anim->NodeChange(model_motion.air_back_L, scastI(AnimAtk::TIMER));
						anim_ccodinate = ac_act[scastI(act_state)].timer;
					}
				}
				else
				{
					//���݂̃A�j���[�V�������󒆃o�b�N�X�e�b�v�̎����A�j���[�V�����������ꍇ
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

//�ړ��֌W(������̓X�e�b�v�ȊO)
void Knight::Move(float decision)
{
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 1&& pad->x_input[scastI(PAD::STICK_R)]==0)
	{
		if (ground)
		{

			if (rightOrleft > 0)
			{
				//�E�����o�b�N�X�e�b�v
				step = true;
				act_state = ActState::BACK;
				moveflag = false;
				//�`����Z�b�g
				anim->NodeChange(model_motion.backstep_R, scastI(AnimAtk::FREAM));
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
				anim_ccodinate = ac_act[scastI(ActState::BACK)].fream;
				speed.x = -backstepS;
			}
			else
			{
				//�_�b�V��������
				act_state = ActState::DASH;
				anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::FREAM));
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//�A�j���[�V�������I��������؂�ւ���
				anim_ccodinate = ac_act[scastI(act_state)].fream;
			}
		}
	}
	if (pad->x_input[static_cast<int>(PAD::R_DASH)] == 1&& pad->x_input[scastI(PAD::STICK_L)]==0)
	{
		if (ground)
		{
			//�`����Z�b�g

			if (rightOrleft < 0)
			{
				//�������o�b�N�X�e�b�v
				step = true;
				act_state = ActState::BACK;
				moveflag = false;
				//�`����Z�b�g
				anim->NodeChange(model_motion.backstep_L, scastI(AnimAtk::FREAM));
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
				anim_ccodinate = ac_act[scastI(ActState::BACK)].fream;
				speed.x = backstepS;
			}
			else
			{
				//�_�b�V���E����
				act_state = ActState::DASH;
				anim->NodeChange(model_motion.dash_R,scastI(AnimAtk::FREAM));
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//�A�j���[�V�������I��������؂�ւ���
				anim_ccodinate = ac_act[scastI(act_state)].fream;
			}
		}
	}


	//���ړ�
	if (pad->x_input[scastI(PAD::L_DASH)] > 0)
	{
		if (!step)
		{
			speed.x = -dashspeed;
			act_state = ActState::DASH;
			if (!anim->GetLoopAnim())
			{
				//���݂̃A�j���[�V�������_�b�V���̊J�n�A�j���[�V�����������ꍇ
				if (anim->GetEndAnim() == -1)
				{
					//�A�j���[�V�������I�������玝���A�j���[�V�����ɐ؂�ւ���
					anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::TIMER));
					anim_ccodinate = ac_act[scastI(act_state)].timer;
				}
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
		//			//���݂̃A�j���[�V�������_�b�V���̊J�n�A�j���[�V�����������ꍇ
		//			if (anim->GetEndAnim()==-1)
		//			{
		//				//�A�j���[�V�������I�������玝���A�j���[�V�����ɐ؂�ւ���
		//				anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::TIMER));
		//				anim_ccodinate = ac_act[scastI(act_state)].timer;
		//			}
		//		}
		//	}
		//}
	}
	else
	{
		if (pad->x_input[scastI(PAD::STICK_L)] > 0)
		{
			speed.x = -walkspeed;
			act_state = ActState::MOVEL;
		}
	}




	//�E�ړ�
	if (pad->x_input[scastI(PAD::R_DASH)] > 0)
	{
		if (!step)
		{
			speed.x = dashspeed;
			act_state = ActState::DASH;
			if (!anim->GetLoopAnim())
			{
				//���݂̃A�j���[�V�������_�b�V���̊J�n�A�j���[�V�����������ꍇ
				if (anim->GetEndAnim() == -1)
				{
					//�A�j���[�V�������I�������玝���A�j���[�V�����ɐ؂�ւ���
					anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::TIMER));
					anim_ccodinate = ac_act[scastI(act_state)].timer;
				}
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
		//			//���݂̃A�j���[�V�������_�b�V���̊J�n�A�j���[�V�����������ꍇ
		//			if (anim->GetEndAnim()==-1)
		//			{
		//				//�A�j���[�V�������I�������玝���A�j���[�V�����ɐ؂�ւ���
		//				anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::TIMER));
		//				anim_ccodinate = ac_act[scastI(act_state)].timer;
		//			}
		//		}
		//	}
		//}
	}
	else
	{
		if (pad->x_input[scastI(PAD::STICK_R)] > 0)
		{
			speed.x = walkspeed;
			act_state = ActState::MOVER;
		}
	}

	if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 0 && pad->x_input[static_cast<int>(PAD::R_DASH)] == 0)
	{

	}
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] > 0 && pad->x_input[static_cast<int>(PAD::R_DASH)] > 0)
	{

	}

	//�ǂ����������Ă���or������Ă��Ȃ��Ȃ猳�ɖ߂�
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] == 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] == 0)
	{
		if (act_state == ActState::DASH)
		{
			//���݂̃X�e�[�g���_�b�V���������ꍇ
			act_state = ActState::ATTACK;
			attack_state = AttackState::NONE;
			attack = true;
			later = dash_later;
			atk_result = HitResult::NOT_OCCURRENCE;
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::LATER));
			}
			anim->PlayAnimation(scastI(AnimAtk::LATER), false);
			moveflag = false;
		}
		else
		{
			act_state = ActState::NONE;
		}
		moveflag = false;
		//speed.x = 0.0f;
	}
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		if (act_state == ActState::DASH)
		{
			//���݂̃X�e�[�g���_�b�V���������ꍇ
			act_state = ActState::ATTACK;
			attack_state = AttackState::NONE;
			attack = true;
			later = dash_later;
			atk_result = HitResult::NOT_OCCURRENCE;
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::LATER));
			}
			anim->PlayAnimation(scastI(AnimAtk::LATER), false);
			moveflag = false;
		}
		else
		{
			act_state = ActState::NONE;
		}
		moveflag = false;
		//speed.x = 0.0f;
	}
}

void Knight::MoveAnimSet()
{
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] > 0)
	{
		moveflag = true;

		if (rightOrleft > 0)
		{
			//�`����Z�b�g
			//�E����
			anim->NodeChange(model_motion.back_R);
			anim_ccodinate = ac_act[scastI(ActState::MOVEL)].fream;
		}
		else
		{
			//�`����Z�b�g
			//������
			anim->NodeChange(model_motion.back_L);
			anim_ccodinate = ac_act[scastI(ActState::MOVEL)].fream;
		}
	}
	if (pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		moveflag = true;

		if (rightOrleft < 0)
		{
			//�`����Z�b�g
			//������
			anim->NodeChange(model_motion.walk_L);
			anim_ccodinate = ac_act[scastI(ActState::MOVER)].fream;
		}
		else
		{
			//�`����Z�b�g
			//�E����
			anim->NodeChange(model_motion.walk_R);
			anim_ccodinate = ac_act[scastI(ActState::MOVER)].fream;
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
				//�`����Z�b�g
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.guard_R, 2);
					if (effect_timer > draw_guard_effect_interval)
					{
						GetEffect().PlayEffect(EffectKind::GUARD, DirectX::XMFLOAT3(pos.x + draw_guarf_effect_add_pos_x, pos.y, pos.z), DirectX::XMFLOAT3(0.7f,0.7f,0.7f), DirectX::XMFLOAT3(0.0f,1.0f,0.0f), 5.5f);
						effect_timer = 0.0f;
					}
				}
				else
				{
					anim->NodeChange(model_motion.guard_L, 2);
					if (effect_timer > draw_guard_effect_interval)
					{
						GetEffect().PlayEffect(EffectKind::GUARD, DirectX::XMFLOAT3(pos.x - draw_guarf_effect_add_pos_x, pos.y, pos.z), DirectX::XMFLOAT3(0.7f, 0.7f, 0.7f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), -5.5f);
						effect_timer = 0.0f;
					}
				}
				anim_ccodinate = ac_act[scastI(act_state)].timer;
			}
			else
			{
				//�`����Z�b�g
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.guard_R);
					if (effect_timer > draw_guard_effect_interval)
					{
						GetEffect().PlayEffect(EffectKind::GUARD, DirectX::XMFLOAT3(pos.x + draw_guarf_effect_add_pos_x, pos.y, pos.z), DirectX::XMFLOAT3(0.7f, 0.7f, 0.7f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 5.5f);
						effect_timer = 0.0f;
					}
				}
				else
				{
					anim->NodeChange(model_motion.guard_L);
					if (effect_timer > draw_guard_effect_interval)
					{
						GetEffect().PlayEffect(EffectKind::GUARD, DirectX::XMFLOAT3(pos.x - draw_guarf_effect_add_pos_x, pos.y, pos.z), DirectX::XMFLOAT3(0.7f, 0.7f, 0.7f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), -5.5f);
						effect_timer = 0.0f;
					}
				}
				anim_ccodinate = ac_act[scastI(act_state)].timer;
			}
		}
		else
		{
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.guard_R,1);
				if (effect_timer > draw_guard_effect_interval)
				{
					GetEffect().PlayEffect(EffectKind::GUARD, DirectX::XMFLOAT3(pos.x + draw_guarf_effect_add_pos_x, pos.y, pos.z), DirectX::XMFLOAT3(0.7f, 0.7f, 0.7f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), 5.5f);
					effect_timer = 0.0f;
				}
			}
			else
			{
				anim->NodeChange(model_motion.guard_L,1);
				if (effect_timer > draw_guard_effect_interval)
				{
					GetEffect().PlayEffect(EffectKind::GUARD, DirectX::XMFLOAT3(pos.x - draw_guarf_effect_add_pos_x, pos.y, pos.z), DirectX::XMFLOAT3(0.7f, 0.7f, 0.7f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), -5.5f);
					effect_timer = 0.0f;
				}
			}
			anim_ccodinate = ac_act[scastI(act_state)].timer;
		}
	}

}

void Knight::WaitAnimSet()
{
	if (act_state == ActState::NONE)
	{
		//�`����Z�b�g

		act_state = ActState::WAIT;
		lumi_material = Model::Material_Attribute::NONE;
		if (rightOrleft > 0)
		{
			//�E�����̎�
			anim->NodeChange(model_motion.wait_R);
			anim_ccodinate = ac_act[scastI(act_state)].fream;
		}
		else
		{
			//�������̎�
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
			//�n�C�W�����v����
			speed_Y.Set(0.0f);
			speed_X.Set(0.0f);
			pad->que.back().timer = 0;
			jumpcount = 0;
			speed.y = high_jump_speed;
			hightrigger = true;
			max_jump_flag = false;
			moveflag = false;
			//�`����Z�b�g
			act_state = ActState::JUMP;
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::FREAM));
			}
			else
			{
				anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::FREAM));
			}
			anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//�A�j���[�V�������I��������؂�ւ���
			jumpflag = true;

			//���ݍU�����肪�o�Ă���Ȃ�S�ď�������
			AllAttackClear();
			//�U���̌��ʂ�������
			hit_result = HitResult::NOT_OCCURRENCE;
			//�L�����Z���̏�����������
			atk_result = HitResult::NONE;
			//�㌄��������
			later = non_target;
			//�U���t���O���I�t��
			attack = false;
			//�U���X�e�[�g��������
			attack_state = AttackState::NONE;
			//�p�x��߂�
			angle.z = 0.0f;
			//�ŏI���͓��e������������
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
			//�����Ղ���
			//later = -1;
			attack = FALSE;
			speed_X.Set(0.0f);
			speed_Y.Set(0.0f);
			pad->que.back().timer = 0;
			jumpcount--;
			hightrigger = false;
			speed.y = jump_speed;
			max_jump_flag = false;
			moveflag = false;
			//�`����Z�b�g
			act_state = ActState::JUMP;
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::FREAM));
			}
			else
			{
				anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::FREAM));
			}
			anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//�A�j���[�V�������I��������؂�ւ���
			anim_ccodinate = ac_act[scastI(act_state)].fream;
			jumpflag = true;
			jump_can_timer = jump_max_time;
			//���ݍU�����肪�o�Ă���Ȃ�S�ď�������
			AllAttackClear();
			//�U���̌��ʂ�������
			hit_result = HitResult::NOT_OCCURRENCE;
			//�㌄��������
			later = non_target;
			//�U���t���O���I�t��
			attack = false;
			//�U���X�e�[�g��������
			attack_state = AttackState::NONE;
			//�p�x��߂�
			angle.z = 0.0f;
			//�ŏI���͓��e������������
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
				//�󒆃W�����v����
				//later = -1;
				attack = FALSE;
				speed_X.Set(0.0f);
				speed_Y.Set(0.0f);
				pad->que.back().timer = 0;
				jumpcount = 0;
				air_dash_count = 0;
				hightrigger = false;
				speed.y = jump_speed;
				max_jump_flag = false;
				moveflag = false;
				//�`����Z�b�g
				act_state = ActState::JUMP;
				if (rightOrleft > 0)
				{
					anim->NodeChange(model_motion.air_jump_R, scastI(AnimAtk::FREAM));
				}
				else
				{
					anim->NodeChange(model_motion.air_jump_L, scastI(AnimAtk::FREAM));
				}
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//�A�j���[�V�������I��������؂�ւ���
				jumpflag = true;
				jump_can_timer = jump_max_time;
				//���ݍU�����肪�o�Ă���Ȃ�S�ď�������
				AllAttackClear();
				//�U���̌��ʂ�������
				hit_result = HitResult::NOT_OCCURRENCE;
				//�L�����Z���̏�����������
				atk_result = HitResult::NONE;
				//�㌄��������
				later = non_target;
				//�U���t���O���I�t��
				attack = false;
				//�U���X�e�[�g��������
				attack_state = AttackState::NONE;
				//�p�x��߂�
				angle.z = 0.0f;
				//�ŏI���͓��e������������
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
			//���݂̃A�j���[�V�������W�����v�̊J�n�A�j���[�V�����������ꍇ
			if (anim->GetEndAnim() == -1)
			{
				//�A�j���[�V�������I�������玝���A�j���[�V�����ɐ؂�ւ���
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
					//�`����Z�b�g

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
		//�W�����v�̒��n���𔭐�����
		act_state = ActState::ATTACK;
		attack_state = AttackState::NONE;
		attack = true;
		later = jump_later;
		//�U���ŃL�����Z���ł���悤��
		atk_result = HitResult::NOT_OCCURRENCE;
		rightOrleft = decision;
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.jump_R, scastI(AnimAtk::LATER));
		}
		else
		{
			anim->NodeChange(model_motion.jump_L, scastI(AnimAtk::LATER));
		}
		anim->PlayAnimation(scastI(AnimAtk::LATER), false);
		//�U����������ׂď�������
		AllAttackClear();
	}
}


void Knight::DamageCheck(float decision)
{
	for (int i = 0; i < scastI(KNIGHTHIT::END); i++)
	{
		if (hit[i].hit)
		{
			//�U�����󂯂Ă���
			//�_���[�W�A������уx�N�g���Ȃǂ�ۑ�
			HitBoxTransition(HitBoxState::NOGUARD);
			float dg = hit[i].damege - (combo_count * 1.2f);
			if (dg <= 0)
			{
				dg = 1;
			}
			hp -= dg;
			combo_count++;
			GaugeUp(hit[i].damege / 5.0f);
			hit[i].damege = 0.0f;
			hit[i].hit = false;
			rightOrleft = decision;

			//�v���C���[�������Ԃɂ���
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
			//�L�����Z���̏�����������
			atk_result = HitResult::NONE;
			attack_state = AttackState::NONE;
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
			//�p�x��߂�
			angle.z = 0.0f;
			//�ŏI���͓��e������������
			last_attack = AttackState::NONE;
			hit_state_n_set = false;
			switch (hit[i].hit_state)
			{
			case HitStateKind::NORMAL:
				//�ʏ�U��
				act_state = ActState::KNOCK;
				steal_escape = 0.0f;
				hit[i].hit_state = HitStateKind::NORMAL;
				break;
			case HitStateKind::STEAL:
				//�͂ݍU��
				act_state = ActState::STATENONE;
				steal_escape = hit[i].steal_timer;
				hit[i].hit_state = HitStateKind::STEAL;
				hit[i].steal_timer = 0.0f;
				break;
			case HitStateKind::SLAM:
				//�@�����U��
				act_state = ActState::SLAM;
				hit[i].hit_state = HitStateKind::NORMAL;
				steal_escape = 0.0f;
				break;
			default:
				break;
			}
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
	//�p�x��߂�
	angle.z = 0.0f;
	knocktimer -= elapsed_time;

	if (pos.y<POS_Y)
	{
		pos.y = POS_Y;
		//�̂����蒆�ɒn�ʂɂ����ꍇ
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
			//�X�e�B�b�N�̉����͂�����ꍇ�͗�����Ԃɂ���
			act_state = ActState::DOWN;
			speed.y = 0.0f;
			//�`����Z�b�g
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
			//�������͂��Ă��Ȃ��ꍇ�͎����I�Ɏ󂯐g�����
			speed.x = Getapply(-passive_speed.x);
			speed.y = passive_speed.y;
			act_state = ActState::PASSIVE;
			//�`����Z�b�g
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
			//�̗͂��Ȃ��Ȃ����痎����Ԃɂ���
			act_state = ActState::FALL;
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
			//�󒆂ł̂������Ԃ��������ꂽ
			//if (pad->x_input[scastI(PAD::STICK_D)] > 0 ||
			//	pad->x_input[scastI(PAD::STICK_LDown)] > 0 ||
			//	pad->x_input[scastI(PAD::STICK_RDown)] > 0)
			//{
			//	//�X�e�B�b�N�̉����͂�����ꍇ�͗�����Ԃɂ���
			//	act_state = ActState::FALL;
			//	speed.y = 0.0f;
			//}
			//else
			{
				//���͂������Ȃ��ꍇ�͎󂯐g�������Ŏ��
				speed.x = 0.0f;
				speed.y = 0.0f;
				act_state = ActState::PASSIVE;
				//�`����Z�b�g
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
				}
				if (pad->x_input[scastI(PAD::STICK_L)] > 0)
				{
					speed.x = -passive_speed.x;
					speed.y = passive_speed.y;
				}
				if (pad->x_input[scastI(PAD::STICK_U)] > 0)
				{
					speed.y = passive_speed.y;
				}
				if (pad->x_input[scastI(PAD::STICK_D)] > 0)
				{
					speed.y = -passive_speed.y;
				}

				if (pad->x_input[scastI(PAD::STICK_R)] == 0 && pad->x_input[scastI(PAD::STICK_L)] == 0 &&
					pad->x_input[scastI(PAD::STICK_U)] == 0 && pad->x_input[scastI(PAD::STICK_D)] == 0)
				{
					speed.x = Getapply(-passive_speed.x);
					speed.y = passive_speed.y;
				}
			}
			//�`����Z�b�g

			if (trackgauge < 2)
			{
				trackgauge = 1;

			}
		}
		if (hp == 0.0f)
		{
			//�̗͂��Ȃ��Ȃ����痎����Ԃɂ���
			act_state = ActState::FALL;
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
	//�p�x��߂�
	angle.z = 0.0f;
	knocktimer -= elapsed_time;

	
	//�n�ʂɂ�����
	if (pos.y < POS_Y)
	{
		//X�����̃X�s�[�h�𐁂���уx�N�g�����琶������
		DirectX::XMFLOAT2 x_back_s = { (rightOrleft),1.0f };
		//���ς��v�Z���Ďˉe�x�N�g�����쐬
		//����
		float dot = ((hit_back_s.x * x_back_s.x) + (hit_back_s.y * x_back_s.y));
		//X�����̑��x�Ɏˉe�x�N�g����ݒ�
		speed.x = (x_back_s.x * (dot/ attenuation_slam));

		//�����蔻��ɓ��͂���Ă��鑬�x��S�ď���������
		for (int i = 0; i < hit.size(); i++)
		{
			hit[i].hitback.x = 0.0f;
			hit[i].hitback.y = 0.0f;
		}

		//�A�j���[�V������ύX����
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.slid_R, 1);
		}
		else
		{
			anim->NodeChange(model_motion.slid_L, 1);
		}
		anim->PlayAnimation(1, true);
		//�n�ʂ̂߂荞�݂�����
		pos.y = POS_Y;
	}

	//�n�ʂ������Ă��鎞
	if (ground)
	{
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

	//�̂����莞�Ԍo��
	if (knocktimer < 0.0f)
	{
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
				//�X�e�B�b�N�̉����͂�����ꍇ�͗�����Ԃɂ���
				act_state = ActState::DOWN;
				speed.y = 0.0f;
				speed.x = 0.0f;
				//�`����Z�b�g
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
				//�������͂��Ă��Ȃ��ꍇ�͎����I�Ɏ󂯐g�����
				speed.x = Getapply(-passive_speed.x);
				speed.y = passive_speed.y;
				act_state = ActState::PASSIVE;
				//�`����Z�b�g
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
				//�̗͂��Ȃ��Ȃ����痎����Ԃɂ���
				act_state = ActState::DOWN;
			}
			return;
		}
		else
		{
			act_state = ActState::FALL;
			speed.y = 0.0f;
			//�`����Z�b�g

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

void Knight::Guard(float decision)
{
	
	if (step || attack || act_state == ActState::PASSIVE|| act_state == ActState::WAKE)
	{
		//�U�����A�܂��̓X�e�b�v���A�󂯐g���Ȃ����Ȃ�
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
				hit_state_n_set = true;
			}
			else
			{
				HitBoxTransition(HitBoxState::DOWN);
				hit_state_n_set = true;
			}
		}
		else
		{
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				if (!ground)
				{
					HitBoxTransition(HitBoxState::ALL);
					hit_state_n_set = true;
				}
				else
				{
					HitBoxTransition(HitBoxState::MIDDLE);
					hit_state_n_set = true;
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
				hit_state_n_set = true;
			}
			else
			{
				HitBoxTransition(HitBoxState::DOWN);
				hit_state_n_set = true;
			}
		}
		else
		{
			if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				if (!ground)
				{
					HitBoxTransition(HitBoxState::ALL);
					hit_state_n_set = true;
				}
				else
				{
					HitBoxTransition(HitBoxState::MIDDLE);
					hit_state_n_set = true;
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
				hit_state_n_set = true;
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
			hit[i].damege = 0;
			hit[i].guard_ok = false;
			moveflag = false;
			knocktimer = hit[i].timer;
			pad->high_trigger = false;
			hightrigger = false;
			act_state = ActState::GUARD;
		}
	}

}

void Knight::GuardBack(float elapsed_time)
{
	if (act_state != ActState::GUARD)
	{
		return;
	}

	hit_state_n_set = true;
	bool hit_on = false;

	//�K�[�h�G�t�F�N�g�`��X�V
	effect_timer += elapsed_time;

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
			hit_state_n_set = false;
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
				//�`����Z�b�g
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
			hit_state_n_set = false;
			//�`����Z�b�g
			max_jump_flag = true;
			jumpflag = true;
			//�`����Z�b�g
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
		//�`����Z�b�g
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
		//�`����Z�b�g
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
			//�`����Z�b�g

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
			speed.y = -5.0f;
		}
		act_state = ActState::DOWN;
		//�`����Z�b�g

	}
	else
	{
		//�󒆎�
		if (pos.y > POS_Y)
		{
			//�d�͂�t�^����
			pos.y -= fall_force * elapsed_time;
			//speed.y += 10.0f * elapsed_time;
		}
		if (pad->x_input[scastI(PAD::X)] == 1 || pad->x_input[scastI(PAD::Y)] == 1 ||
			pad->x_input[scastI(PAD::B)] == 1 || pad->x_input[scastI(PAD::A)] == 1)
		{
			speed.x = 0.0f;
			speed.y = 0.0f;
			act_state = ActState::PASSIVE;
			//�`����Z�b�g

			HitBoxTransition(HitBoxState::INVINCIBLE);
			if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				speed.x = 20.0f;
				speed.y = -10.0f;
			}
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				speed.x = -20.0f;
				speed.y = -10.0f;
			}
			if (pad->x_input[scastI(PAD::STICK_U)] > 0)
			{
				speed.y = -20.0f;
			}
			if (pad->x_input[scastI(PAD::STICK_D)] > 0)
			{
				speed.y = 20.0f;
			}

			if (pad->x_input[scastI(PAD::STICK_R)] == 0 && pad->x_input[scastI(PAD::STICK_L)] == 0 &&
				pad->x_input[scastI(PAD::STICK_U)] == 0 && pad->x_input[scastI(PAD::STICK_D)] == 0)
			{
				speed.x = Getapply(-20.0f);
				speed.y = -10.0f;
			}
		}
	}
}



void Knight::DownUpdate()
{
	
	if (ground)
	{
		//�_�E��������̓��͂�����΃X�e�[�g��؂�ւ���
		pos.y = POS_Y;
		speed.y = 0.0f;
		speed.x = 0.0f;
		if (pad->x_input[scastI(PAD::STICK_R)] > 0)
		{
			speed.x = passive_speed.x;
			speed.y = passive_speed.y;
			act_state = ActState::PASSIVE;
			//�`����Z�b�g
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
			act_state = ActState::PASSIVE;
			//�`����Z�b�g
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
			//�`����Z�b�g
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
			}
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				speed.x = -passive_speed.x;
				speed.y = passive_speed.y;
			}
			if (pad->x_input[scastI(PAD::STICK_R)] == 0 && pad->x_input[scastI(PAD::STICK_L)] == 0)
			{
				speed.x = Getapply(-passive_speed.x);
				speed.y = passive_speed.y;
			}
		}
		if (pad->x_input[scastI(PAD::STICK_U)] > 0)
		{
			//�`����Z�b�g
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
	//�N���オ��J�n�͖��G
	HitBoxTransition(HitBoxState::INVINCIBLE);

	//�_�E�������(���S�ɋN���オ������)���G����������
	//���݂̃A�j���[�V�������N���オ�肾�����ꍇ
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
		//�E����
		angle.z += 20.0f * elapsed_time;
	}
	if (rightOrleft < 0)
	{
		//������
		angle.z -= 20.0f * elapsed_time;
	}
	
	if (pos.y < POS_Y)
	{
		pos.y = POS_Y;
	}

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
	if (speed.x == 0.0f && speed.y == 0)
	{
		HitBoxTransition(HitBoxState::NOGUARD);
		angle.z = 0.0f;
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
			//�`����Z�b�g
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
		//�`����Z�b�g

		if (pad->x_input[scastI(PAD::RB)] == 1)
		{
			steal_escape = 0.0f;
			pos.x -= Getapply(15.0f);
			act_state = ActState::NONE;
			return;
		}
		steal_escape -= elapsed_time;
	}
}


void Knight::TrackDash(float decision,float elapsed_time)
{
	if (attack_state != AttackState::TRACK_DASH)
	{
		return;
	}

	//max_jump_flag = false;
	/*speed.y = 0.0f;*/
	rightOrleft = decision;

	//���K�����ꂽ����Ɍ������x�N�g��
	YR_Vector3	plusVec = { 0.0f,0.0f,0.0f };
	plusVec = tracking.Veccalculate(hit[scastI(KNIGHTHIT::BODY)].center,decision);

	//�㌄���ݒ肳�ꂽ��͂��̊֐��ɂ͓���Ȃ�
	if (later > -1 && later < target_max)
	{
		return;
	}
	jumpflag = false;

	//�d�͂̋t����t�^����
	pos.y += gravity * elapsed_time;

	//�X�s�[�h�����ׂ�0�ɂ���
	speed.x = 0.0f;
	speed.y = 0.0f;

	//�����t���[���ɂȂ�܂ŉ�
	if (fream < target_max)
	{
		fream -= elapsed_time;

		//������Ƃ��������Ă���
		pos.y += 5.0f * elapsed_time;
	}
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	//�����t���[���ɂȂ�����U������𐶐�����
	if (fream < 0.0f)
	{
		//int attack_num = attack_list[real].now_attack_num;
		anim_ccodinate = ac_attack[now_at_list].timer;
		if (attack_list[now_at_list].now_attack_num == 0)
		{
			//����̍U���̂݃A�j���[�V������ς���
			if(rightOrleft>0)
			{
				anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L[now_at_list], scastI(AnimAtk::TIMER));
			}
		}
		if (attack_list[now_at_list].speed_on)
		{
			//�U���ɑ��x��t�^����ꍇ
			attack_list[now_at_list].SetAttack(&atk, rightOrleft, pos, attack_list[now_at_list].speed);
		}
		else
		{
			//�t�^���Ȃ��ꍇ
			attack_list[now_at_list].SetAttack(&atk, rightOrleft, pos);
		}
		fream = non_target;

		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}



	bool knock = false;	//��x�ł�knock_start�ɓ�������c��̓����蔻���knockback��S��0.0f�ɂ���
	if (!atk.empty())
	{
		//��ɉ�]������
		angle.z -= 50.0f * elapsed_time;

		pos.x += ((plusVec.x * track_speed) * elapsed_time);
		pos.y += ((plusVec.y * track_speed) * elapsed_time);

		for (auto& a : atk)
		{
			if (knock && a.attack_name == scastI(attack_state))
			{
				a.parameter.knockback = 0.0f;
			}
			if (a.knock_start && a.attack_name == scastI(attack_state))
			{
				pos.x -= a.parameter.knockback * rightOrleft;
				a.parameter.knockback = 0.0f;
				knock = true;
			}
		}
		if (knock)
		{
			//�z�[�~���O�_�b�V���͓����������_�ōU�����I������̂Ō㌄����͂���
			//�U������Y���W�𑊎�̍��W�ɌŒ肷��
			pos.y = tracking.rival_Pos.y;
			//X���W���ǌ��\�Ȉʒu�ɌŒ肷��
			pos.x = tracking.rival_Pos.x + (track_adjust_x * (-decision));
			//������ւ̑��x����͂���(������ƃz�b�v������)
			speed_X.Set(0.0f);
			//speed_Y.Set(attack_list[now_at_list].advance_speed);
			speed.y = attack_list[now_at_list].advance_speed;
			//�U�������ׂď�������
			AllAttackClear();
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;
			//�㌄��ݒ�
			later = attack_list[now_at_list].later;
			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L[now_at_list], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
			//�p�x��߂�
			angle.z = 0.0f;
			angle.x = 0.0f;
			//�W�����v�񐔂����炷
			if (jumpcount != 0)
			{
				jumpcount = 1;
			}
			//�z�[�~���O�_�b�V���񐔂����炷
			trackgauge--;
			//�W�����v��Ԃɂ���
			jumpflag = true;
			max_jump_flag = true;
		}
	}

	if (atk.empty())
	{
		//�����U�����܂��o�Ă��Ȃ��Ȃ炱����return���Ď��̍U���Ɉڂ�Ȃ��悤�ɂ���
		return;
	}

	//�U�����S�ďI���������Ƃ��m�F����
	if (AttackEndCheck())
	{
		//�܂��U�����c���Ă���Ύ��̍U���Ɉڂ�
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[now_at_list].attack_single[attack_list[now_at_list].now_attack_num].fream;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//������ւ̑��x����͂���(������ƃz�b�v������)
			speed_X.Set(0.0f);
			speed.y = 0.0f;
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;
			//�㌄��ݒ�
			later = attack_list[now_at_list].later;
			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L[now_at_list], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
			//�p�x��߂�
			angle.z = 0.0f;
			angle.x = 0.0f;
			//�W�����v�񐔂����炷
			if (jumpcount != 0)
			{
				jumpcount = 1;
			}
			//�z�[�~���O�_�b�V���񐔂����炷
			trackgauge--;
			//�W�����v��Ԃɂ���
			jumpflag = true;
			max_jump_flag = true;
		}
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
	if (gauge > GAUGE_MAX)
	{
		float away = gauge - GAUGE_MAX;
		power++;
		gauge = 0;
		gauge = away;
	}
	if (power > 4)
	{
		power = 5;
		gauge = GAUGE_MAX;
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
	//�������o�p�̃Z�b�g
	pos = YR_Vector3(1.0f, 0.0f, 0.0f);
	YRCamera.SetFocus(pos.GetDXFLOAT3());
	YRCamera.SetEye(DirectX::XMFLOAT3(0.0f, 0.0f, -25.0f));
}

bool Knight::WinPerformance()
{
	//�������o�̍s��������
	//�J������������œ�����
	switch (win_state)
	{
	case Knight::WIN_PERFORMANCE_KNIGHT::CAMERA_ZOOM:
	{
		YR_Vector3 vec, eye, focus;
		focus = YRCamera.GetFocus();
		eye = YRCamera.GetEye();
		vec = focus - eye;
		float len = vec.Length();
		if (len < 0.5f)
		{
			win_state = WIN_PERFORMANCE_KNIGHT::FINISH;
			break;
		}
		vec.Normalize();
		eye += vec * 0.1f;
		YRCamera.SetEye(eye.GetDXFLOAT3());
	}
	break;
	case Knight::WIN_PERFORMANCE_KNIGHT::FINISH:
		return true;
		break;
	default:
		break;
	}


	return false;
}

bool Knight::Intro(float elapsed_time)
{
	//�C���g���̍s��������
	//�J������������œ�����
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
		//���_��ݒ�(������)
		eye = YR_Vector3(pos.x - Getapply(52.0f), pos.y + 1.8f, pos.z + 70.0f);
		focus = YR_Vector3(pos.x, pos.y + 1.8f, pos.z + 1.8f);
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
		intro_state = INTRO_KNIGHT::WAIT;
		lip_text = RandTextSelect();
		break;
	case Knight::INTRO_KNIGHT::WAIT:
		//�w�肵���ʒu�܂ŃJ�����𓮂����Ă���(�X�V)
		eye = YR_Vector3(pos.x - Getapply(52.0f), pos.y +1.8f, pos.z + 50.0f);
		focus = YR_Vector3(pos.x, pos.y + 1.8f, pos.z + 1.8f);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f*elapsed_time);
		if (intro_timer > 1.0f)
		{
			intro_state = INTRO_KNIGHT::ZOOM_SET;
		}
		break;
	case Knight::INTRO_KNIGHT::ZOOM_SET:
		//���_��ݒ�(������)
		eye = YR_Vector3(pos.x - Getapply(25.0f), pos.y + 5.0f, pos.z - 45.0f);
		focus = YR_Vector3(pos.x, pos.y + 5.0f, pos.z);
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
		intro_state = INTRO_KNIGHT::ZOOM;
		ChangeFace(FaceAnim::NORMAL_LIP_SYNC);
		break;
	case Knight::INTRO_KNIGHT::ZOOM:
		//�w�肵���ʒu�܂ŃJ�����𓮂����Ă���(�X�V)
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
		//�w�肵���ʒu�܂ŃJ�����𓮂����Ă���(�X�V)
		eye = YR_Vector3(pos.x, pos.y + 2.5f, pos.z - 70.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 4.0f)
		{
			intro_state = INTRO_KNIGHT::PUSH;
		}
		break;
	case Knight::INTRO_KNIGHT::PUSH:
		//�w�肵���ʒu�܂ŃJ�����𓮂����Ă���(�X�V)
		eye = YR_Vector3(pos.x, pos.y + 2.5f, pos.z - 41.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 4.5f)
		{
			intro_state = INTRO_KNIGHT::PULL_2;
		}
		break;
	case Knight::INTRO_KNIGHT::PULL_2:
		//�w�肵���ʒu�܂ŃJ�����𓮂����Ă���(�X�V)
		eye = YR_Vector3(pos.x, pos.y + 30.0f, pos.z - 85.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 4.9f)
		{
			intro_state = INTRO_KNIGHT::PUSH_2;
		}
		break;
	case Knight::INTRO_KNIGHT::PUSH_2:
		//�w�肵���ʒu�܂ŃJ�����𓮂����Ă���(�X�V)
		eye = YR_Vector3(pos.x+Getapply(27.0f), pos.y + 5.4f, pos.z - 38.0f);
		focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z);
		YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 5.0f * elapsed_time);
		if (intro_timer > 5.3f)
		{
			intro_state = INTRO_KNIGHT::FIN_SET;
		}
		break;
	case Knight::INTRO_KNIGHT::FIN_SET:
		//���_��ݒ�(������)
		eye = YR_Vector3(pos.x - Getapply(30.0f), pos.y - 16.0f, pos.z - 23.0f);
		focus = YR_Vector3(pos.x , pos.y, pos.z - 5.4f);
		YRCamera.SetEye(eye.GetDXFLOAT3());
		YRCamera.SetFocus(focus.GetDXFLOAT3());
		intro_state = INTRO_KNIGHT::FIN;
		break;
	case Knight::INTRO_KNIGHT::FIN:
		//�w�肵���ʒu�܂ŃJ�����𓮂����Ă���(�X�V)
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


//�C���g����A�Q�[���J�n�܂ł̐ݒ���s��
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
}


//�J���������p�f�o�b�O�֐�
void Knight::IntroDEBUG()
{
#if USE_IMGUI
	ImGui::Begin(u8"�C���g���J����");
	int state_in = scastI(intro_state);
	ImGui::InputInt(u8"�X�e�[�g", &state_in, 1, 10);
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
#endif // USE_IMGUI

}

//�U�������蔻�肪�S�ďI�����Ă��邩�m�F����
bool Knight::AttackEndCheck()
{
	if (!atk.empty())
	{
		for (auto& a : atk)
		{
			if (!a.fin && a.attack_name == scastI(attack_state))
			{
				return false;
			}
		}
	}

	return true;
}


//�I�������U�������蔻���S�ď�������B
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

//�S�Ă̍U�������蔻�����������
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

//�U�����肪���݂��鎞�̂ݍX�V
void Knight::AttackUpdate(float elapsed_time)
{
	if (!atk.empty())
	{
		for (auto& a : atk)
		{
			a.Update(pos, elapsed_time);
			if (a.hit_result != HitResult::NONE)
			{
				//�U�����������Ă����ꍇ�A���̓��e��ۑ�����
				hit_result = a.hit_result;
				//�s���I���t���O���I����
				finish = true;
			}
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


//��̃e�N�X�`���A�j���[�V����
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
	default:
		break;
	}
}


//�u��
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
	//�E�C���N���I�������true��Ԃ�
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


//���p�N�̏���
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



//�\��ύX
void Knight::ChangeFace(FaceAnim anim)
{
	face_anim = anim;
	face_wink_time = 0.0f;
	wink_state = Wink_State::FIRST;
	face_wink_interval_timer = 0.0f;
	face_wink_time = 0.0f;
	//�\��܂΂������݂̏ꍇ�̂�
	if (face_anim == FaceAnim::NORMAL||
		face_anim ==FaceAnim::NORMAL_LIP_SYNC)
	{
		//�����_���ł܂΂����̊Ԋu������
		wink_interval =static_cast<float>((rand() % 5) + 1);
	}
}

void Knight::AttackDetailsSet(const AttackState& attack_state)
{
	//�U�����Ƃɏ��񂾂��ʂ��������L�q����
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
	case Knight::TextList::NORMAL://�ʏ�
		return std::wstring(L"�����A�n�߂悤���I");
		break;
	case Knight::TextList::WARLIKE://�D��I
		return std::wstring(L"��C�ɍs����I");
		break;
	case Knight::TextList::CRIOSITY://�D��S
		return std::wstring(L"���Ɛ킢�����́H");
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