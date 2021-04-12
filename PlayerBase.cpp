#include "PlayerBase.h"
#include "Key.h"
#include "YRGamePad.h"
#include "camera.h"
#include "framework.h"
#include <algorithm>
#include "World.h"
#include "Effect.h"
#include "YRSound.h"

//-----------------------------------------------------------------
//			Player�N���X
//-----------------------------------------------------------------
//	�E���̃N���X���L�����̊��N���X�ƂȂ�ׁA�قƂ�ǂ̓����
//	  ���̃N���X���ōs���B�L�������ƂɕK�v�ȏ����͊֐����I�[�o�[���C�h����
//-----------------------------------------------------------------

void Player::Init(YR_Vector3 InitPos)
{
	//������
	pos = InitPos;

	//fream = non_target;
	fream = 0.0f;
	timer = non_target;
	later = non_target;

	speed.x = 0;
	speed.y = 0.0f;
	jumpcount = 2;
	air_dash_count = 1;
	max_jump_flag = false;
	hp = chara_state.max_hp;
	gravity = 40.0f;
	down_force = 200.0f;
	fall_force = 100.0f;
	knocktimer = 0.0f;
	ground = true;
	jumpflag = false;
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


	production_time = 0.0f;

	stop_state = 0;
	air_dash_state = AirDashState::NONE;
	power = 1;

	draw_count = 0;

	light_direction = DirectX::XMFLOAT4(-1.0f, -0.1, 1.0f, 0.0f);
#pragma region HITBOXINIT
	for (int act = 0; act < hit.size(); act++)
	{
		hit[act].Init();
	}
#pragma endregion

	moveflag = false;
	attack = FALSE;
	attack_state = AttackState::NONE;
	last_attack = AttackState::NONE;
	act_state = ActState::NONE;
	later = non_target;

	jump_can_timer = 0.0f;

	passive_timer = 0.0f;

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

	traject_on = false;

	CharaInit();
}






void Player::Update(float decision, float elapsed_time)
{
	//�X�V����

	//elapsed_time��0�A�܂�q�b�g�X�g�b�v�ȂǂŃQ�[�����~�܂��Ă���ꍇ
	if (elapsed_time == 0.0f)
	{
		//����return�ŏ����ɓ���Ȃ��悤�ɂ���
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
	// *�T�v*
	//�E�U�����󂯂�ȂǑ���s�\�̏�Ԃ̂݃X�e�[�g�ŋ敪�����A
	//�@����ȊO�͑�����󂯕t����̂�default�ŏ������Ă���
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
	case ActState::DOWN_HIT:
		//�_�E���q�b�g���
		DownHitUpdate(elapsed_time);
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
				pos.y -= gravity * elapsed_time;
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
				if (!moveflag)
				{
					rightOrleft = decision;
				}
				if (speed.x > 0.0f)
				{
					speed.x -= chara_state.brake_speed * elapsed_time;
					if (speed.x < 0.0f)
					{
						speed.x = 0;
					}
				}
				if (speed.x < 0.0f)
				{
					speed.x += chara_state.brake_speed * elapsed_time;
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
						MoveAnimSet();
						Move(rightOrleft);
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

	AttackUpdate(elapsed_time);	//�U������̍X�V

	JumpUpdate(decision, elapsed_time);
	AirDash(elapsed_time);

	//���x�ɂ��ʒu�̍X�V
	pos.x += ((speed_X.Update(elapsed_time) * elapsed_time) * rightOrleft);
	pos.x += (speed.x * elapsed_time);

	speed.y += (speed_Y.Update(elapsed_time) * elapsed_time);
	pos.y += (speed.y * elapsed_time);

	//�����蔻��̍X�V
	//�R���{����ʂ̃X�e�[�g�ɂ���Ă͕ʓr�������K�v�ɂȂ邽�߁A
	//switch���Ōʂɕ����Ă���
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

	//�A�j���[�V���������X�V
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
	EndAttackErase();			//�U������̏���
}









void Player::AttackInput()
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
			if (!pad->que.empty())
			{
				pad->que.back().timer = 0;
			}
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
						if (static_cast<AttackState>(list) != AttackState::TRACK_DASH &&
							static_cast<AttackState>(list) != AttackState::VERSATILE_ATTACK &&
							static_cast<AttackState>(list) != AttackState::A_VERSATILE_ATTACK)
						{
							//�z�[�~���O�_�b�V���ƃv���C���[�I���Z�̏ꍇ�͂��Ⴊ��łĂ��o����悤�ɔ��肷��
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
						//�X�e�B�b�N�̓��͂��w�肳��Ă���ꍇ�m�F����
						if (attack_list[list].linkage_stick == PAD::STICK_D)
						{
							//���X�e�B�b�N���͂̏ꍇ�͕ʂ̔�����Ƃ�
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
				//���ۂ̍U�����e
				int real = scastI(attack_list[list].real_attack);
				if (attack_list[real].combo == last_attack)
				{
					if (last_attack == AttackState::COMBO_X || last_attack == AttackState::COMBO_Y || last_attack == AttackState::COMBO_B)
					{
						//�R���{��Ԃɓ����Ă���ꍇ�̓R���{�֐��ōU���̃X�e�[�g�����肷��
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
					//�Q�[�W�̕K�v�ʂ��m�F����
					//�ʏ�̍U���̏ꍇ�A���ۂ̍U�����e�͓��ꂸ�ɍU���������̂܂ܓ����
					attack_state = static_cast<AttackState>(list);
					if (attack_list[real].need_power > 0)
					{
						//�Q�[�W�������U���������ꍇ�̓Q�[�W�����炷
						power -= attack_list[real].need_power;
					}
				}
				else
				{
					//�Q�[�W������Ȃ��ꍇ�w�肵���Z���o��
					attack_state = attack_list[real].aid_attack_name;
					real = scastI(attack_list[scastI(attack_list[real].aid_attack_name)].real_attack);
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
				//�p�x�����ɖ߂�
				angle.y = 0.0f;
				angle.z = 0.0f;
				//�����蔻���������
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
				//������������
				timer = non_target;
				//�J���������p�ϐ���������
				production_time = 0.0f;
				//�O�Ղ�\�����邩�ǂ������擾����
				traject_on = attack_list[real].traject_on;
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







void Player::Attack(float decision, float elapsed_time)
{
	//�U�����ɓ���֐�
	if (ground)
	{
		if (speed.x > 0.0f)
		{
			speed.x -= chara_state.brake_speed * elapsed_time;
			if (speed.x < 0.0f)
			{
				speed.x = 0;
			}
		}
		if (speed.x < 0.0f)
		{
			speed.x += chara_state.brake_speed * elapsed_time;
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

	AttackSwitch(decision, elapsed_time);

	//�U�����A�U�������������ꍇ�L�����Z�����čU������
	//�O��̍U�����ɂǂ̏����ŃL�����Z���ł��邩��ۑ����Ă邽�߁A����ȏ�Ȃ�L�����Z�����čU������
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
		angle.y = 0.0f;

		if (ground)
		{
			//�n�ʂɂ��Ă���ꍇ
			if (act_state != ActState::WAIT)
			{

				//�����A�����͂�����Ă����ꍇ�A�X�e�[�g�����Ⴊ�݂ɂ���
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
			//�󒆂ɂ���ꍇ

			//�X�e�[�g���W�����v�ɂ���
			act_state = ActState::JUMP;

			//�U���͉������Ă��Ȃ���Ԃɂ���
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
		}
	}
}







void Player::AttackSwitch(float decision, float elapsed_time, AttackState attack_state)
{
	//�U�����ƂɊ֐����w�肷��֐�

	AttackState now_attack_state;	//���݂̍U��

	//�����ōU�����w�肵�Ă���΂��̍U�������݂̍U���Ƃ��Ďg�p����
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
		//��ѓ���(��)
		Jaku_Rhurf(elapsed_time);
		break;
	case AttackState::THU_RHURF:
		//��ѓ���(��)
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
	case AttackState::A_JAKU_LHURF:
		A_Jaku_Lhurf(elapsed_time);
		break;
	case AttackState::COMBO_X:
		ComboX(decision, elapsed_time);
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








void Player::MoveAnimSet()
{
	//�X�e�B�b�N�𗼕��Ƃ��|���Ă���ꍇ
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

	//�X�e�B�b�N�̍������͂��ꂽ
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] == 1)
	{
		moveflag = true;

		if (rightOrleft > 0)
		{
			//�`����Z�b�g
			//�E����
			//���
			anim->NodeChange(model_motion.back_R);
			anim_ccodinate = ac_act[scastI(ActState::RETREAT)].fream;
		}
		//�����͍폜(�_�b�V���̂�)
		else
		{
			//�`����Z�b�g
			//������
			/*anim->NodeChange(model_motion.walk_L);
			anim_ccodinate = ac_act[scastI(ActState::MOVEL)].fream;*/
			//�_�b�V��������
			act_state = ActState::DASH;
			anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::FREAM));
			anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//�A�j���[�V�������I��������؂�ւ���
			anim_ccodinate = ac_act[scastI(act_state)].fream;
		}
	}
	//�X�e�B�b�N�̉E�����͂��ꂽ
	if (pad->x_input[static_cast<int>(PAD::STICK_R)] == 1)
	{
		moveflag = true;

		if (rightOrleft < 0)
		{
			//�`����Z�b�g
			//������
			//���
			anim->NodeChange(model_motion.back_L);
			anim_ccodinate = ac_act[scastI(ActState::RETREAT)].fream;
		}
		//�����͍폜(�_�b�V���̂�)
		else
		{
			//�`����Z�b�g
			//�E����
			/*anim->NodeChange(model_motion.walk_R);
			anim_ccodinate = ac_act[scastI(ActState::MOVER)].fream;*/
			//�_�b�V���E����
			act_state = ActState::DASH;
			anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::FREAM));
			anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//�A�j���[�V�������I��������؂�ւ���
			anim_ccodinate = ac_act[scastI(act_state)].fream;
		}
	}

	if (!moveflag)
	{
		//�U����ɃX�e�B�b�N�����͂��ꂽ�܂܂��ƃA�j���[�V�����̑J�ڂ��s���Ȃ�����
		//moveflag���m�F���Ĉړ����Ă��邩�ǂ����𔻕ʂ��Ă���
		//���ɃX�e�B�b�N��|���Ă���
		if (pad->x_input[static_cast<int>(PAD::STICK_L)] > 0)
		{
			moveflag = true;

			if (rightOrleft > 0)
			{
				//�`����Z�b�g
				//�E����
				//���
				anim->NodeChange(model_motion.back_R);
				anim_ccodinate = ac_act[scastI(ActState::RETREAT)].fream;
			}
			//�����͍폜(�_�b�V���̂�)
			else
			{
				//�`����Z�b�g
				//������
				/*anim->NodeChange(model_motion.walk_L);
				anim_ccodinate = ac_act[scastI(ActState::MOVEL)].fream;*/
				//�_�b�V��������
				act_state = ActState::DASH;
				anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::FREAM));
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//�A�j���[�V�������I��������؂�ւ���
				anim_ccodinate = ac_act[scastI(act_state)].fream;
			}
		}
		//�E�X�e�B�b�N��|���Ă���
		if (pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
		{
			moveflag = true;

			if (rightOrleft < 0)
			{
				//�`����Z�b�g
				//������
				//���
				anim->NodeChange(model_motion.back_L);
				anim_ccodinate = ac_act[scastI(ActState::RETREAT)].fream;
			}
			//�����͍폜(�_�b�V���̂�)
			else
			{
				//�`����Z�b�g
				//�E����
				/*anim->NodeChange(model_motion.walk_R);
				anim_ccodinate = ac_act[scastI(ActState::MOVER)].fream;*/
				//�_�b�V���E����
				act_state = ActState::DASH;
				anim->NodeChange(model_motion.dash_R, scastI(AnimAtk::FREAM));
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//�A�j���[�V�������I��������؂�ւ���
				anim_ccodinate = ac_act[scastI(act_state)].fream;
			}
		}
	}

}





void Player::Move(float decision)
{
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 1 && pad->x_input[scastI(PAD::STICK_R)] == 0)
	{
		if (ground)
		{

			if (rightOrleft > 0)
			{
				//�E�����o�b�N�X�e�b�v
				step = true;
				act_state = ActState::BACK;
				moveflag = false;
				GetSound().SESinglePlay(SEKind::BACKSTEP);
				//�`����Z�b�g
				anim->NodeChange(model_motion.backstep_R, scastI(AnimAtk::FREAM));
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
				anim_ccodinate = ac_act[scastI(ActState::BACK)].fream;
				speed.x = -chara_state.backstepS;
			}
			else
			{
				//�_�b�V��������
				//�_�b�V���̑J�ڂ͕ʂōs�����ߏ���
				//act_state = ActState::DASH;
				//anim->NodeChange(model_motion.dash_L, scastI(AnimAtk::FREAM));
				//anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//�A�j���[�V�������I��������؂�ւ���
				//anim_ccodinate = ac_act[scastI(act_state)].fream;
			}
		}
	}
	if (pad->x_input[static_cast<int>(PAD::R_DASH)] == 1 && pad->x_input[scastI(PAD::STICK_L)] == 0)
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
				GetSound().SESinglePlay(SEKind::BACKSTEP);
				//�`����Z�b�g
				anim->NodeChange(model_motion.backstep_L, scastI(AnimAtk::FREAM));
				anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
				anim_ccodinate = ac_act[scastI(ActState::BACK)].fream;
				speed.x = chara_state.backstepS;
			}
			else
			{
				//�_�b�V���E����
				//�_�b�V���̑J�ڂ͕ʂōs�����ߏ���
				//act_state = ActState::DASH;
				//anim->NodeChange(model_motion.dash_R,scastI(AnimAtk::FREAM));
				//anim->PlayAnimation(scastI(AnimAtk::FREAM), false);//�A�j���[�V�������I��������؂�ւ���
				//anim_ccodinate = ac_act[scastI(act_state)].fream;
			}
		}
	}


	//���ړ�
	if (pad->x_input[scastI(PAD::STICK_L)] > 0)
	{
		if (!step)
		{
			if (rightOrleft < 0)
			{
				//������
				speed.x = -chara_state.dashspeed;
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
			else
			{
				speed.x = -chara_state.walkspeed;
				act_state = ActState::RETREAT;
			}
		}
	}




	//�E�ړ�
	if (pad->x_input[scastI(PAD::STICK_R)] > 0)
	{
		if (!step)
		{
			if (rightOrleft > 0)
			{
				//�E����
				speed.x = chara_state.dashspeed;
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
			else
			{
				//������
				speed.x = chara_state.walkspeed;
				act_state = ActState::RETREAT;
			}
		}
	}

	MoveStop();
}



void Player::MoveStop()
{
	//�ړ���~������
	//�ȑO�͌㌄��ݒ肵�Ă���������̓s����폜

	//�X�e�B�b�N�������|���Ă��Ȃ��ꍇ
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] == 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] == 0)
	{
		if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 0 && pad->x_input[static_cast<int>(PAD::R_DASH)] == 0)
		{
			speed.x = 0.0f;
			if (act_state != ActState::WAIT)
			{
				act_state = ActState::NONE;
			}
			moveflag = false;
		}
	}

	//�X�e�B�b�N�𗼕��Ƃ��|���Ă���ꍇ
	if (pad->x_input[static_cast<int>(PAD::STICK_L)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		speed.x = 0.0f;
		if (act_state != ActState::WAIT)
		{
			act_state = ActState::NONE;
		}
		moveflag = false;
	}
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] > 0 && pad->x_input[static_cast<int>(PAD::R_DASH)] > 0)
	{
		speed.x = 0.0f;
		if (act_state != ActState::WAIT)
		{
			act_state = ActState::NONE;
		}
		moveflag = false;
	}
	if (pad->x_input[static_cast<int>(PAD::L_DASH)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_R)] > 0)
	{
		speed.x = 0.0f;
		if (act_state != ActState::WAIT)
		{
			act_state = ActState::NONE;
		}
		moveflag = false;
	}
	if (pad->x_input[static_cast<int>(PAD::R_DASH)] > 0 && pad->x_input[static_cast<int>(PAD::STICK_L)] > 0)
	{
		speed.x = 0.0f;
		if (act_state != ActState::WAIT)
		{
			act_state = ActState::NONE;
		}
		moveflag = false;
	}
}


//�ړ��֌W(�X�e�b�v�Ȃ�)
//-----------------------------------------------------------------
// *�T�v*
//�E�n��̏ꍇ�A�o�b�N�X�e�b�v�̏������s��(�O���ւ̓_�b�V�������̈�)
//�E�󒆂̏ꍇ�͋󒆃_�b�V���̐ݒ���s���A�����͕ʊ֐��ōs��
//-----------------------------------------------------------------
bool Player::Step(float elapsed_time)
{
	//��
	if (!ground)
	{
		//�󒆍��X�e�b�v
		if (pad->x_input[scastI(PAD::L_TRIGGER)] == 1)
		{
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				//�X�e�B�b�N�����ɓ|���Ă��鎞
				if (air_dash_count == 1)
				{
					//�_�b�V���J�E���g���c���Ă���ꍇ�͋󒆃_�b�V�����s��
					//step = true;
					moveflag = false;
					jumpflag = false;
					jumpcount = 0;
					air_dash_count = 0;
					GetSound().SESinglePlay(SEKind::HIGH_JUMP);
					//�X�s�[�h�͂ǂ���������
					speed.x = -chara_state.stepspeed;
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
			else if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				//�X�e�B�b�N���E�ɓ|���Ă��鎞
				if (air_dash_count == 1)
				{
					//�_�b�V���J�E���g���c���Ă���ꍇ�͋󒆃_�b�V�����s��
					//step = true;
					moveflag = false;
					jumpflag = false;
					jumpcount = 0;
					air_dash_count = 1;
					//�X�s�[�h�͂ǂ���������
					speed.x = chara_state.stepspeed;
					GetSound().SESinglePlay(SEKind::HIGH_JUMP);
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
			else
			{
				if (air_dash_count == 1)
				{
					//�_�b�V���J�E���g���c���Ă���ꍇ�͋󒆃_�b�V�����s��
					//step = true;
					moveflag = false;
					jumpflag = false;
					jumpcount = 0;
					air_dash_count = 1;
					GetSound().SESinglePlay(SEKind::HIGH_JUMP);
					if (rightOrleft > 0)
					{
						//�󒆑O�_�b�V��
						speed.x = chara_state.stepspeed;
						act_state = ActState::AIR_F;
						air_dash_state = AirDashState::AIR_F;
						//�`����Z�b�g
						anim->NodeChange(model_motion.air_dash_R, scastI(AnimAtk::FREAM));
						anim->PlayAnimation(scastI(AnimAtk::FREAM), false);
						anim_ccodinate = ac_act[scastI(ActState::AIR_F)].fream;
					}
					else
					{
						//�󒆑O�_�b�V��
						//�X�s�[�h�͂ǂ���������
						speed.x = -chara_state.stepspeed;
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
		}

		//�󒆍��X�e�b�v
		if (pad->x_input[static_cast<int>(PAD::L_DASH)] == 1)
		{
			if (air_dash_count == 1)
			{
				//�_�b�V���J�E���g���c���Ă���ꍇ�͋󒆃_�b�V�����s��
				//step = true;
				moveflag = false;
				jumpflag = false;
				jumpcount = 0;
				air_dash_count = 0;
				GetSound().SESinglePlay(SEKind::HIGH_JUMP);
				//�X�s�[�h�͂ǂ���������
				speed.x = -chara_state.stepspeed;
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
			if (air_dash_count == 1)
			{
				//�_�b�V���J�E���g���c���Ă���ꍇ�͋󒆃_�b�V�����s��
				//step = true;
				moveflag = false;
				jumpflag = false;
				jumpcount = 0;
				air_dash_count = 1;
				//�X�s�[�h�͂ǂ���������
				speed.x = chara_state.stepspeed;
				GetSound().SESinglePlay(SEKind::HIGH_JUMP);
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
			if (speed.x < (-chara_state.backstepS / 5.0f))
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
				if (speed.x > -chara_state.backstepS)
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
			//�ړ��͈͂̃Z�b�g
			if (pos.x < Limit::Left_max)
			{
				pos.x = Limit::Left_max;
			}
			speed.x += (chara_state.backstepD * elapsed_time);
			return false;
		}
		if (rightOrleft < 0)
		{
			//�t������
			if (speed.x < 0.0f)
			{
				step = false;
				speed.x = 0.0f;
				pad->dash_trigger = false;
				moveflag = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				act_state = ActState::NONE;
				lumi_material = Model::Material_Attribute::NONE;
				return true;
			}
			if (speed.x > (chara_state.backstepS / 5.0f))
			{
				HitBoxTransition(HitBoxState::INVINCIBLE);
			}
			//�ړ��͈͂̃Z�b�g
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
				if (speed.x > chara_state.backstepS)
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
			speed.x -= (chara_state.backstepD * elapsed_time);
			return false;
		}
	}
	else
	{
		step = false;
	}

	return false;
}


//�󒆃_�b�V������
//-----------------------------------------------------------------
// *�T�v*
//�E�󒆃_�b�V���̏������s��
//�EAirDashState�ɂ���ď�����ς���
//-----------------------------------------------------------------
void Player::AirDash(float elapsed_time)
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
			if (speed.x < chara_state.dashspeed)
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
					if (speed.x < chara_state.stepspeed)
					{
						anim->NodeChange(model_motion.air_dash_R, scastI(AnimAtk::LATER));
						anim->PlayAnimation(scastI(AnimAtk::LATER), false);
						anim_ccodinate = ac_act[scastI(act_state)].later;
					}
				}
			}

			speed.x -= (chara_state.stepD * elapsed_time);
			return;
		}
		if (rightOrleft < 0)
		{
			if (speed.x > -chara_state.dashspeed)
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
					if (speed.x > -chara_state.stepspeed)
					{
						anim->NodeChange(model_motion.air_dash_L, scastI(AnimAtk::LATER));
						anim->PlayAnimation(scastI(AnimAtk::LATER), false);
						anim_ccodinate = ac_act[scastI(act_state)].later;
					}
				}
			}

			speed.x += (chara_state.stepD * elapsed_time);
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
			if (speed.x > -chara_state.dashspeed)
			{
				step = false;
				pad->dash_trigger = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				air_dash_state = AirDashState::NONE;
				moveflag = false;
				air_dash_count = 0;
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
					if (speed.x > -chara_state.stepspeed)
					{
						anim->NodeChange(model_motion.air_back_R, scastI(AnimAtk::LATER));
						anim->PlayAnimation(scastI(AnimAtk::LATER), false);
						anim_ccodinate = ac_act[scastI(act_state)].later;
					}
				}
			}

			speed.x += (chara_state.stepD * elapsed_time);

			return;
		}
		if (rightOrleft < 0)
		{
			//������
			//�X�s�[�h���_�b�V���ȉ��ɂȂ�����I��
			if (speed.x < chara_state.dashspeed)
			{
				step = false;
				pad->dash_trigger = false;
				pad->x_input[scastI(PAD::R_DASH)] = 0;
				pad->x_input[scastI(PAD::L_DASH)] = 0;
				air_dash_state = AirDashState::NONE;
				moveflag = false;
				air_dash_count = 0;
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
					if (speed.x < chara_state.stepspeed)
					{
						anim->NodeChange(model_motion.air_back_L, scastI(AnimAtk::LATER));
						anim->PlayAnimation(scastI(AnimAtk::LATER), false);
						anim_ccodinate = ac_act[scastI(act_state)].later;
					}
				}
			}

			speed.x -= (chara_state.stepD * elapsed_time);
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



void Player::Jump()
{
	//�W�����v�J�n����
	if (step)
	{
		//�X�e�b�v���̓W�����v�ł��Ȃ�����return
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
			GetSound().SESinglePlay(SEKind::HIGH_JUMP);
			jumpcount = 0;
			speed.y = chara_state.high_jump_speed;
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
			angle.y = 0.0f;
			//�ŏI���͓��e������������
			last_attack = AttackState::NONE;

			if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				if (speed.x < chara_state.dashspeed)
				{
					speed.x = chara_state.dashspeed;
				}
				return;
			}
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				if (speed.x > -chara_state.dashspeed)
				{
					speed.x = -chara_state.dashspeed;

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
			GetSound().SESinglePlay(SEKind::JUMP);
			jumpcount--;
			hightrigger = false;
			speed.y = chara_state.jump_speed;
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
			angle.y = 0.0f;
			//�ŏI���͓��e������������
			last_attack = AttackState::NONE;

			if (pad->x_input[scastI(PAD::STICK_R)] > 0)
			{
				if (speed.x < chara_state.walkspeed)
				{
					speed.x = chara_state.walkspeed;
				}
				return;
			}
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				if (speed.x > -chara_state.walkspeed)
				{
					speed.x = -chara_state.walkspeed;

				}
				return;
			}
			speed.x = 0.0f;
		}
	}
	else
	{
		//�󒆃W�����v�̏ꍇ�͏������قȂ�
		//�X�e�B�b�N����ɓ|���Ă���ƒn��W�����v�������㒼���ɋ󒆃W�����v���o�Ă��܂�����
		//�W�����v�^�C�}�[��0�ȉ��ɂȂ�܂ł͋󒆃W�����v���o�Ȃ��悤�ɂ��Ă���
		if (jumpcount > 0 && jump_can_timer <= 0.0f)
		{
			if (pad->x_input[scastI(PAD::STICK_U)] == 1)
			{
				//�󒆃W�����v����
				//later = -1;
				attack = FALSE;
				speed_X.Set(0.0f);
				speed_Y.Set(0.0f);
				pad->que.back().timer = 0;
				GetSound().SESinglePlay(SEKind::JUMP);
				jumpcount = 0;
				air_dash_count = 0;
				hightrigger = false;
				speed.y = chara_state.jump_speed;
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
				angle.y = 0.0f;
				//�ŏI���͓��e������������
				last_attack = AttackState::NONE;

				if (pad->x_input[scastI(PAD::STICK_R)] > 0)
				{
					if (speed.x < chara_state.walkspeed)
					{
						speed.x = chara_state.walkspeed;
					}
					return;
				}
				if (pad->x_input[scastI(PAD::STICK_L)] > 0)
				{
					if (speed.x > -chara_state.walkspeed)
					{
						speed.x = -chara_state.walkspeed;
					}
					return;
				}
				speed.x = 0.0f;
			}
		}
	}
}



void Player::JumpUpdate(float decision, float elapsed_time)
{
	//�W�����v�̍X�V����
	if (jumpcount < 2 && jumpflag)
	{
		//���݃W�����v���Ă���悤�Ȃ����
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
		//�W�����v�^�C�}�[�����炵�Ă���
		if (jump_can_timer > 0.0f)
		{
			jump_can_timer -= elapsed_time;
		}

		//�W�����v�̍ō����x�ɓ��B���Ă��Ȃ��ꍇ
		if (!max_jump_flag)
		{
			if (hightrigger)
			{
				speed.y += (chara_state.high_jump_speed * elapsed_time);
				if (speed.y > chara_state.high_jump_max)
				{
					//�W�����v�̑��x�����ɒB����
					max_jump_flag = true;
				}
			}
			else
			{
				speed.y += (chara_state.jump_speed * elapsed_time);
				if (speed.y > chara_state.jump_max)
				{
					//�W�����v�̑��x�����ɒB����
					max_jump_flag = true;
				}
			}
		}
	}

	//�W�����v�̑��x�����ɒB�������
	if (max_jump_flag)
	{
		if (!ground)
		{
			if (attack)
			{
				//�U�����̏ꍇ�U�����������Ă���Α��x�͉����Ȃ�
				if (hit_result <= HitResult::HIT && combo_count == 0)
				{
					speed.y -= (down_force * elapsed_time);
				}
			}
			else
			{
				//���x�������ė�����悤�ɂ���
				speed.y -= (down_force * elapsed_time);
			}

		}
		if (hightrigger)
		{
			if (speed.y - (gravity * elapsed_time) < 0.0f)
			{
				hightrigger = false;
			}
		}
	}
	if (pos.y < POS_Y)
	{
		//�n�ʂɂ����ꍇ

		//�W�����v�̃J�E���g��߂�
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
		hit_result = HitResult::NOT_OCCURRENCE;
		atk_result = HitResult::NOT_OCCURRENCE;
		rightOrleft = decision;
		angle.y = 0.0f;
		//�p�x�����ɖ߂�
		angle.y = 0.0f;
		angle.z = 0.0f;
		timer = non_target;
		//���n����炷
		GetSound().SESinglePlay(SEKind::LANDING);
		//�`����Z�b�g
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


void Player::DamageCheck(float decision)
{
	//�_���[�W����
	for (int i = 0; i < hit.size(); i++)
	{
		if (hit[i].hit)
		{
			//�U�����󂯂Ă���
			GetSound().SEStop(SEKind::SLIDE);
			YRGetEffect().StopEffect(EffectKind::SMOKE);
			rightOrleft = decision;
			//�������Ƃɐݒ�
			switch (hit[i].hit_state)
			{
			case HitStateKind::NORMAL:
				//�ʏ�U��
				act_state = ActState::KNOCK;
				steal_escape = 0.0f;
				hit[i].hit_state = HitStateKind::NORMAL;
				timer = non_target;
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
					//������уx�N�g�����}�C�i�X�̏ꍇ
					if (pos.y <= POS_Y)
					{
						//��������3����1�̑��x��X�ɓn��
						if (hit[i].hitback.x >= 0.0f)
						{
							hit[i].hitback.x += (-hit[i].hitback.y * 0.3f);
						}
						else
						{
							hit[i].hitback.x += (hit[i].hitback.y * 0.3f);
						}
						//�n�ʂɂ��Ă���ꍇ�͉������̑��x��0�ɂ���
						hit[i].hitback.y = 0.0f;
						//�̂����莞�Ԃ�3����1�ɂ���
						hit[i].timer *= 0.25f;
					}
				}
				break;
			case HitStateKind::STEAL:
				//�͂ݍU��
				act_state = ActState::STATENONE;
				steal_escape = hit[i].steal_timer;
				hit[i].hit_state = HitStateKind::STEAL;
				hit[i].steal_timer = 0.0f;
				timer = non_target;
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
				//�@�����U��
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
				timer = non_target;
				break;
			case HitStateKind::DOWN:
				//�_�E���U��
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
				//�A�[�}�[����������
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

			//�_���[�W�A������уx�N�g���Ȃǂ�ۑ�
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
			//�p�x�����ɖ߂�
			angle.y = 0.0f;
			angle.z = 0.0f;
			//�ŏI���͓��e������������
			last_attack = AttackState::NONE;
		}
	}
}






void Player::KnockUpdate(float elapsed_time)
{
	//�̂����菈��
	bool pflag = false;		//�m�b�N�o�b�N������������true�ɂ���
	for (int i = 0; i < hit.size(); i++)
	{
		if (hit[i].hitback.x != 0.0f)
		{
			pos.x += hit[i].hitback.x * elapsed_time;
			if (pos.x > Limit::Right_max)
			{
				//���̑��x�ȏ�ŕǂɓ��������ꍇ���˕Ԃ�悤�ɂ���
				if (hit[i].hitback.x > Reflection_range_min)
				{
					hit[i].hitback.x = (-hit[i].hitback.x * Reflection_attenuation_factor);
					YRGetEffect().PlayEffect(EffectKind::WALL_SHOCK, DirectX::XMFLOAT3(pos.x - 5.0f, pos.y, pos.z), DirectX::XMFLOAT3(5.0f, 5.0f, 5.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMConvertToRadians(90.0f));
				}
			}
			if (pos.x < Limit::Left_max)
			{
				//���̑��x�ȏ�ŕǂɓ��������ꍇ���˕Ԃ�悤�ɂ���
				if (hit[i].hitback.x < -Reflection_range_min)
				{
					hit[i].hitback.x = (-hit[i].hitback.x * Reflection_attenuation_factor);
					YRGetEffect().PlayEffect(EffectKind::WALL_SHOCK, DirectX::XMFLOAT3(pos.x + 5.0f, pos.y, pos.z), DirectX::XMFLOAT3(5.0f, 5.0f, 5.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), DirectX::XMConvertToRadians(-90.0f));
				}
			}
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
	//�p�x��߂�
	angle.z = 0.0f;
	knocktimer -= elapsed_time;

	if (pos.y < POS_Y)
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
			GetSound().SESinglePlay(SEKind::SLAM);
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
			speed.x = Getapply(-chara_state.passive_speed.x);
			speed.y = chara_state.passive_speed.y;
			//�󂯐g���Ԃ����
			passive_timer = passive_max_time;
			act_state = ActState::PASSIVE;
			GetSound().SESinglePlay(SEKind::PASSIVE);
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
			if (hp <= 0.0f)
			{
				//�̗͂��Ȃ��Ȃ����痎����Ԃɂ���
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
		}
		return;
	}

	if (knocktimer < 0.0f)
	{
		//�̂����莞�Ԃ��o�߂���

		//�R���{�J�E���g��0�ɂ���
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
			if (hp <= 0.0f)
			{
				//�̗͂������ꍇ�͎����I�Ɏ󂯐g�����Ȃ��悤�ɂ���
			}
			else
			{
				//���͂������Ȃ��ꍇ�͎󂯐g�������Ŏ��
				speed.x = 0.0f;
				speed.y = 0.0f;
				act_state = ActState::PASSIVE;
				GetSound().SESinglePlay(SEKind::PASSIVE);
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
					speed.x = chara_state.passive_speed.x;
					speed.y = chara_state.passive_speed.y;
					//�󂯐g���Ԃ����
					passive_timer = passive_max_time;
				}
				if (pad->x_input[scastI(PAD::STICK_L)] > 0)
				{
					speed.x = -chara_state.passive_speed.x;
					speed.y = chara_state.passive_speed.y;
					//�󂯐g���Ԃ����
					passive_timer = passive_max_time;
				}
				if (pad->x_input[scastI(PAD::STICK_U)] > 0)
				{
					speed.y = chara_state.passive_speed.y;
					//�󂯐g���Ԃ����
					passive_timer = passive_max_time;
				}
				if (pad->x_input[scastI(PAD::STICK_D)] > 0)
				{
					speed.y = -chara_state.passive_speed.y;
					//�󂯐g���Ԃ����
					passive_timer = passive_max_time;
				}

				if (pad->x_input[scastI(PAD::STICK_R)] == 0 && pad->x_input[scastI(PAD::STICK_L)] == 0 &&
					pad->x_input[scastI(PAD::STICK_U)] == 0 && pad->x_input[scastI(PAD::STICK_D)] == 0)
				{
					speed.x = Getapply(-chara_state.passive_speed.x);
					speed.y = chara_state.passive_speed.y;
					//�󂯐g���Ԃ����
					passive_timer = passive_max_time;
				}
			}

			//�z�[�~���O�_�b�V���񐔂�0�ȉ��ɂȂ�Ȃ��悤��
			if (trackgauge < 2)
			{
				trackgauge = 1;

			}
		}
		if (hp == 0.0f)
		{
			//�̗͂��Ȃ��Ȃ����痎����Ԃɂ���
			act_state = ActState::FALL;
			speed.y = 0.0f;
			jumpflag = true;
			max_jump_flag = true;
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
		knocktimer = 0.0f;
		for (int i = 0; i < hit.size(); i++)
		{
			hit[i].timer = 0.0f;
			hit[i].hitback = YR_Vector3(0.0f, 0.0f);
		}
	}
}







void Player::FallUpdate(float elapsed_time)
{
	//����������

	if (ground)
	{
		//�n�ʂɂ�����X�e�[�g���_�E���ɂ���
		pos.y = POS_Y;
		if (hp > 0)
		{
			speed.y = 5.0f;
		}
		act_state = ActState::DOWN;
	}
	else
	{
		//�󒆎�
		if (pos.y < POS_Y)
		{
			//�d�͂�t�^����
			pos.y -= gravity * elapsed_time;
		}
	}
}





void Player::DownHitUpdate(float elapsed_time)
{
	//�_�E�����̏���

	bool pflag = false;		//�m�b�N�o�b�N������������true�ɂ���

	//���x�����Z���Ă���
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
	//�p�x��߂�
	angle.z = 0.0f;

	if (knocktimer > 0.0f)
	{
		knocktimer -= elapsed_time;
	}
	else
	{
		//�����蔻��ɓ��͂���Ă��鑬�x��S�ď���������
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
				//�_�E����Ԃɂ���
				act_state = ActState::DOWN;
				speed.y = 0.0f;
				speed.x = 0.0f;
				for (int i = 0; i < hit.size(); i++)
				{
					hit[i].timer = 0.0f;
					hit[i].hitback = YR_Vector3(0.0f, 0.0f);
				}
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
		}
	}

	//�n�ʂɂ�����
	if (pos.y < POS_Y)
	{
		combo_count = 0;
		knocktimer = 0.0f;
		//�_�E����Ԃɂ���
		act_state = ActState::DOWN;
		GetSound().SESinglePlay(SEKind::SLAM);
		speed.y = 0.0f;
		speed.x = 0.0f;
		for (int i = 0; i < hit.size(); i++)
		{
			hit[i].timer = 0.0f;
			hit[i].hitback = YR_Vector3(0.0f, 0.0f);
		}
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
		//�n�ʂ̂߂荞�݂�����
		pos.y = POS_Y;
	}
}





void Player::DownUpdate()
{
	//�_�E��������

	if (ground)
	{
		//�_�E��������̓��͂�����΃X�e�[�g��؂�ւ���
		pos.y = POS_Y;
		speed.y = 0.0f;
		speed.x = 0.0f;
		ChangeFace(FaceAnim::NORMAL);
		if (pad->x_input[scastI(PAD::STICK_R)] > 0)
		{
			speed.x = chara_state.passive_speed.x;
			speed.y = chara_state.passive_speed.y;
			//�󂯐g���Ԃ����
			passive_timer = passive_max_time;
			act_state = ActState::PASSIVE;
			GetSound().SESinglePlay(SEKind::PASSIVE);
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
			speed.x = -chara_state.passive_speed.x;
			speed.y = chara_state.passive_speed.y;
			//�󂯐g���Ԃ����
			passive_timer = passive_max_time;
			act_state = ActState::PASSIVE;
			GetSound().SESinglePlay(SEKind::PASSIVE);
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
			GetSound().SESinglePlay(SEKind::PASSIVE);
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
				speed.x = chara_state.passive_speed.x;
				speed.y = chara_state.passive_speed.y;
				//�󂯐g���Ԃ����
				passive_timer = passive_max_time;
			}
			if (pad->x_input[scastI(PAD::STICK_L)] > 0)
			{
				speed.x = -chara_state.passive_speed.x;
				speed.y = chara_state.passive_speed.y;
				//�󂯐g���Ԃ����
				passive_timer = passive_max_time;
			}
			if (pad->x_input[scastI(PAD::STICK_R)] == 0 && pad->x_input[scastI(PAD::STICK_L)] == 0)
			{
				speed.x = Getapply(-chara_state.passive_speed.x);
				speed.y = chara_state.passive_speed.y;
				//�󂯐g���Ԃ����
				passive_timer = passive_max_time;
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






void Player::SlamUpdate(float elapsed_time)
{
	//�@������ꏈ��

	bool pflag = false;		//�m�b�N�o�b�N������������true�ɂ���

	DirectX::XMFLOAT2 hit_back_s = { 0.0f,0.0f };		//���̃t���[�����ɉ��Z���ꂽ���x

	//���x�����Z����
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
		speed.x = (x_back_s.x * (dot / attenuation_slam));

		//�����蔻��ɓ��͂���Ă��鑬�x��S�ď���������
		for (int i = 0; i < hit.size(); i++)
		{
			hit[i].hitback.x = 0.0f;
			hit[i].hitback.y = 0.0f;
		}

		//�A�j���[�V������ύX����
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
		//�n�ʂ̂߂荞�݂�����
		pos.y = POS_Y;

		YRGetEffect().PlayEffect(EffectKind::SHOCKWAVE, DirectX::XMFLOAT3(pos.x, pos.y - 5.0f, pos.z), DirectX::XMFLOAT3(10.0f, 20.0f, 10.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);

		float smoke_angle = 0.0f;
		if (rightOrleft < 1)
		{
			smoke_angle = 90.0f;
		}
		YRGetEffect().PlayEffect(EffectKind::SMOKE, DirectX::XMFLOAT3(pos.x + Getapply(5.0f), pos.y - 5.0f, pos.z), DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), smoke_angle);
	}

	//�n�ʂ������Ă��鎞
	if (ground)
	{
		GetSound().SEPlay(SEKind::SLIDE);
		HitBoxTransition(HitBoxState::SLIDE);
		YRGetEffect().SetLocation(EffectKind::SMOKE, DirectX::XMFLOAT3(pos.x + Getapply(5.0f), pos.y - 5.0f, pos.z));
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
		GetSound().SEStop(SEKind::SLIDE);
		YRGetEffect().StopEffect(EffectKind::SMOKE);
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
			else if (hp == 0.0f)
			{
				//�̗͂��Ȃ��ꍇ�͗�����Ԃɂ���
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
				speed.x = Getapply(-chara_state.passive_speed.x);
				speed.y = chara_state.passive_speed.y;
				//�󂯐g���Ԃ����
				passive_timer = passive_max_time;
				act_state = ActState::PASSIVE;
				GetSound().SESinglePlay(SEKind::PASSIVE);
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
		for (int i = 0; i < hit.size(); i++)
		{
			hit[i].timer = 0.0f;
			hit[i].hitback = YR_Vector3(0.0f, 0.0f);
		}
	}
}







void Player::PassiveUpdate(float elapsed_time)
{
	//�󂯐g����

	if (rightOrleft > 0)
	{
		//�E����
		angle.z += 30.0f * elapsed_time;
	}
	if (rightOrleft < 0)
	{
		//������
		angle.z -= 30.0f * elapsed_time;
	}

	//�n�ʂɂ߂荞�܂Ȃ��悤��
	if (pos.y < POS_Y)
	{
		pos.y = POS_Y;
	}

	//�󂯐g���Ԃ����炵�Ă���
	passive_timer -= elapsed_time;

	//���ꂼ��̑��x�����Z�A���Z����0�ɋ߂Â��Ă���
	if (speed.x > 0)
	{
		speed.x -= (chara_state.passive_brake * elapsed_time);
		if (speed.x < 0.0f)
		{
			speed.x = 0.0f;
		}
	}
	if (speed.x < 0)
	{
		speed.x += (chara_state.passive_brake * elapsed_time);
		if (speed.x > 0.0f)
		{
			speed.x = 0.0f;
		}
	}
	if (speed.y > 0)
	{
		speed.y -= (chara_state.passive_brake * elapsed_time);
		if (speed.y < 0.0f)
		{
			speed.y = 0.0f;
		}
	}
	if (speed.y < 0)
	{
		speed.y += (chara_state.passive_brake * elapsed_time);
		if (speed.y > 0.0f)
		{
			speed.y = 0.0f;
		}
	}

	//�󂯐g���Ԃ�0�ɂȂ�����I��
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





void Player::WakeUp()
{
	//�N���オ�菈��

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






void Player::GuardAnimSet()
{
	//�K�[�h���̃A�j���[�V�����J�ڏ���
	if (act_state == ActState::GUARD)
	{
		if (ground)
		{
			//�X�e�B�b�N�̉������͂���Ă���ꍇ�͉��i�K�[�h��
			if (pad->x_input[static_cast<int>(PAD::STICK_RDown)] > 0 || pad->x_input[static_cast<int>(PAD::STICK_LDown)] > 0)
			{
				//�`����Z�b�g
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
				//����ȊO�̓K�[�h�A�j���V������
				//�`����Z�b�g
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
			//�󒆃K�[�h�A�j���[�V����
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.guard_R, 1);
			}
			else
			{
				anim->NodeChange(model_motion.guard_L, 1);
			}
			anim_ccodinate = ac_act[scastI(act_state)].timer;
		}
	}

}







void Player::Guard(float decision)
{
	//�K�[�h����

	if (step || attack || act_state == ActState::PASSIVE || act_state == ActState::WAKE)
	{
		//�U�����A�܂��̓X�e�b�v���A�󂯐g���Ȃ����Ȃ�
		return;
	}

	//�����蔻��̃K�[�h��Ԃ���������(���̌�̔��菈���ŃK�[�h���Ă����ꍇ�͕t�^���Ȃ���)
	HitBoxTransition(HitBoxState::NOGUARD);

	//���͔��菈��
	//�E�����͂�����Ă����ꍇ�͉��i�K�[�h�ɂȂ�
	//�E�󒆂ɂ���ꍇ�͋󒆃K�[�h�ɂȂ�
	if (rightOrleft > 0)
	{
		//�E�����̏ꍇ
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
		//�������̏ꍇ
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
		//�󒆂̏ꍇ
		if (decision != rightOrleft)
		{
			//�����Ă�������ƌ��݂̏������̕������Ⴄ�ꍇ�͗����̌����Ŕ�������
			//(�W�����v�ő�����щz�������ɃK�[�h�o���Ȃ����ۂ̋~�Ϗ��u)
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
			//�K�[�h���o���Ă��邩�ǂ����𓖂��蔻������Ĕ��肷��
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
			//�x�N�g���������������ꍇ�O���ɂ���
			if (rightOrleft > 0)
			{
				//�E������
				if (hit[i].hitback.x > 0.0f)
				{
					hit[i].hitback.x = -hit[i].hitback.x;
				}
			}
			else
			{
				//������
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







void Player::GuardBack(float elapsed_time)
{
	//�K�[�h�o�b�N����

	if (act_state != ActState::GUARD)
	{
		return;
	}

	bool hit_on = false;	//�K�[�h�o�b�N������������true�ɂ���


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







void Player::WaitAnimSet()
{
	//�ҋ@�A�j���[�V�����J�ڏ���
	if (act_state == ActState::NONE)
	{
		act_state = ActState::WAIT;
		lumi_material = Model::Material_Attribute::NONE;
		//�`����Z�b�g
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






void Player::NoneChange()
{
	//�K�[�h���Ă��Ȃ����ɉ������͂���Ă��Ȃ��ꍇ�̓X�e�[�g��NONE�ɂ��鏈��

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







void Player::Squat()
{
	//���Ⴊ�ݏ���

	//�K�[�h���A�_�b�V�����A�o�b�N�X�e�b�v�A�󒆃_�b�V�����͏������s��Ȃ�
	if (act_state == ActState::GUARD || act_state == ActState::DASH || act_state == ActState::BACK || act_state == ActState::AIR_B || act_state == ActState::AIR_F)
	{
		return;
	}
	//�n�ʂɂ��Ă��Ȃ��Ƃ��͏������s��Ȃ�
	if (!ground)
	{
		return;
	}
	if (pad->x_input[scastI(PAD::STICK_D)] == 1 || pad->x_input[scastI(PAD::STICK_RDown)] == 1 || pad->x_input[scastI(PAD::STICK_LDown)] == 1)
	{
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
		//�������͂���Ă��Ȃ��ꍇ�͑ҋ@�ɖ߂�
		if (act_state != ActState::WAIT)
		{
			act_state = ActState::NONE;
		}
	}
}





void Player::StateNone(float elapsed_time)
{
	//�X�e�[�g�D��ꂽ��Ԃ̏���

	if (steal_escape > 0.0f)
	{
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

			speed.x = Getapply(-chara_state.passive_speed.x);
			speed.y = chara_state.passive_speed.y;
			//�󂯐g���Ԃ����
			passive_timer = passive_max_time;
			return;
		}
		steal_escape -= elapsed_time;
	}
}





void Player::GaugeUp(float add)
{
	//�Q�[�W�A�p���[��������

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





//�U�������蔻�肪�S�ďI�����Ă��邩�m�F����
bool Player::AttackEndCheck()
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







//�I�������U�������蔻���S�ď�������B
void Player::EndAttackErase()
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
}





//�S�Ă̍U�������蔻�����������
void Player::AllAttackClear()
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
void Player::AttackUpdate(float elapsed_time)
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






//�U�����肪���݂��鎞�̂ݍX�V(�����蔻�肪�擾�������ʂ��v���C���[�ɑ���)
void Player::HitResultUpdate()
{
	if (!atk.empty())
	{
		for (auto& a : atk)
		{
			if (a.hit_result != HitResult::NONE)
			{
				//�U�����������Ă����ꍇ�A���̓��e��ۑ�����
				hit_result = a.hit_result;
				//�s���I���t���O���I����
				finish = true;
			}
		}
	}
}





void Player::HitBoxTransition(HitBoxState state)
{
	//�S�Ă̓����蔻��̃X�e�[�g��ύX����

	for (int h = 0; h < hit.size(); h++)
	{
		hit[h].state = state;
	}
}





float Player::Getapply(float n)
{
	//���݌����Ă�������ɍ��킹��������Ԃ�
	return n * rightOrleft;
}





void Player::AttackDetailsSet(const AttackState& attack_state)
{
	//�U�����Ƃɏ��񂾂��ʂ��������L�q����
	//��{�͉������Ȃ����g�p����L�����̓I�[�o�[���C�h���邱��
}




void Player::StopUpdate()
{
	//�q�b�g�X�g�b�v���s������

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





void Player::StopEnd()
{
	//�q�b�g�X�g�b�v�I�����ɍs������

	if (stop_pos == YR_Vector3(0.0f, 0.0f))
	{
		return;
	}
	pos = stop_pos;
	stop_pos = YR_Vector3(0.0f, 0.0f);
}





void Player::StopHitParamUpdate()
{
	//�f�o�b�O�p�R�[�h
	//Pause���Ƀp�����[�^�[���������悤�ɂ��邽�߂̊֐�
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






//�\��ύX
void Player::ChangeFace(FaceAnim anim)
{
	//�ʏ�͉����s��Ȃ��B
	//�e�N�X�`���A�j���[�V�����ŕ\���ύX���郂�f�����g�p����ꍇ�͂��̊֐����I�[�o�[���C�h����
	//�\��̕ύX�������s���B
	return;
}




void Player::TextDraw()
{
	//�ݒ肳�ꂽ�e�L�X�g��\������

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




void Player::DrawCutIn(
	YRShader* shader,
	float elapsed_time
)
{
	//�J�b�g�C���̕\��

	float center_x = static_cast<float>(FRAMEWORK.SCREEN_WIDTH) / 2.0f;
	float center_y = static_cast<float>(FRAMEWORK.SCREEN_HEIGHT) / 2.0f;

	float fream = 0.03f;
	float size = 3.0f;

	bool anim_fin = cutFrame->AnimFinCheck();

	if (anim_fin)
	{
		cut_in_timer += elapsed_time;
		{
			//�ʏ�̕`��(�Ō�̃A�j���[�V������`�悵������)
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
		if (cut_in_timer < cut_in_max_time)
		{
			//�J�b�g�C���Đ�
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

	//���̎��Ԉȏ�ɂȂ�����t�Đ����J�n����
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