#include "Knight.h"
#include "../../Key.h"
#include "../../Game/YRGamePad.h"
#include "../../camera.h"
#include "../../Effect.h"
#include "../../YRSound.h"
#include "../../framework.h"


//------------------------------------------------
//					�U���̏���
//------------------------------------------------
//�E�U���̏����͋Z�ɂ���ĈقȂ���̂����邽��
//  ������Ɉڂ��Ă���
//
//�E���ʂȏ��������܂Ȃ��ꍇ��AttackDefault�֐��𒆂ŉ�
//------------------------------------------------


//------------------------------------------------------
//				�U���֐�


void Knight::Jaku(float elapsed_time)
{
	AttackDefault(elapsed_time);
}

void Knight::Thu(float elapsed_time)
{

	AttackDefault(elapsed_time);
}

void Knight::Kyo(float elapsed_time)
{
	//�㌄���ݒ肳�ꂽ��͂��̊֐��ɂ͓���Ȃ�
	if (later > -1 && later < target_max)
	{
		return;
	}

	//�����t���[���ɂȂ�܂ŉ�
	if (fream < target_max)
	{
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;
	}
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	//�����t���[���ɂȂ�����U������𐶐�����
	if (fream < 0.0f)
	{
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NONE;
		//�O�i���Ȃ��悤�ɂ���
		speed_X.Set(0.0f);
		//int attack_num = attack_list[real].now_attack_num;
		anim_ccodinate = ac_attack[now_at_list].timer;
		if (attack_list[now_at_list].now_attack_num == 0)
		{
			//����̍U���̂݃A�j���[�V������ς���
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::TIMER));
			}
		}
		if (attack_list[now_at_list].speed_on)
		{
			//�U���ɑ��x��t�^����ꍇ
			attack_list[now_at_list].SetAttack(&atk, rightOrleft, pos, YR_Vector3(attack_list[now_at_list].speed.x*rightOrleft, attack_list[now_at_list].speed.y));
		}
		else
		{
			//�t�^���Ȃ��ꍇ
			attack_list[now_at_list].SetAttack(&atk, rightOrleft, pos);
		}
		fream = non_target;

		//�������Ԃ�ݒ�
		timer = attack_list[now_at_list].attack_single[0].parameter[0].timer;

		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}



	bool knock = false;	//��x�ł�knock_start�ɓ�������c��̓����蔻���knockback��S��0.0f�ɂ���
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
				PosKnockPlus(a.parameter.knockback);
				a.parameter.knockback = 0.0f;
				knock = true;
			}
		}
	}

	if (timer > 0.0f && timer < target_max)
	{
		//�����t���[�������炵�Ă���
		timer -= elapsed_time;
	}

	//if (atk.empty())
	//{
	//	//�����U�����܂��o�Ă��Ȃ��Ȃ炱����return���Ď��̍U���Ɉڂ�Ȃ��悤�ɂ���
	//	return;
	//}

	//�������Ԃ��S�ďI���������Ƃ��m�F����
	if (timer < 0.0f)
	{
	////�U�����S�ďI���������Ƃ��m�F����
	//if (AttackEndCheck())
	//{
		//�܂��U�����c���Ă���Ύ��̍U���Ɉڂ�
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[now_at_list].attack_single[attack_list[now_at_list].now_attack_num].fream;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;
			//�㌄��ݒ�
			later = attack_list[now_at_list].later;
			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;
			HitBoxTransition(HitBoxState::NOGUARD);
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
		}
	}
}


void Knight::D_Jaku(float elapsed_time)
{
	AttackDefault(elapsed_time);
}


void Knight::D_Thu(float elapsed_time)
{
	AttackDefault(elapsed_time);
}

void Knight::U_Kyo(float elapsed_time)
{
	constexpr float up_gravity = 20.0f;
	//�㌄���ݒ肳�ꂽ��͂��̊֐��ɂ͓���Ȃ�
	if (later > -1 && later < target_max)
	{
		//�d�͂�t�^����
		//pos.y -= up_gravity * elapsed_time;
		return;
	}

	//�����t���[���ɂȂ�܂ŉ�
	if (fream < target_max)
	{
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NOT_OCCURRENCE;
		//����������
		pos.y += up_gravity * elapsed_time;
		fream -= elapsed_time;
	}
	//�d�͂̋t����t�^����
	pos.y += gravity * elapsed_time;
	int now_at_list = scastI(attack_state);
	//�����t���[���ɂȂ�����U������𐶐�����
	if (fream < 0.0f)
	{
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NONE;
		//�O�i���Ȃ��悤�ɂ���
		speed_X.Set(0.0f);
		anim_ccodinate = ac_attack[now_at_list].timer;
		if (attack_list[now_at_list].speed_on)
		{
			//�U���ɑ��x��t�^����ꍇ
			attack_list[now_at_list].SetAttack(&atk, rightOrleft, pos, YR_Vector3(attack_list[now_at_list].speed.x * rightOrleft, attack_list[now_at_list].speed.y));
		}
		else
		{
			//�t�^���Ȃ��ꍇ
			attack_list[now_at_list].SetAttack(&atk, rightOrleft, pos);
		}
		fream = non_target;
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::TIMER));
		}
		else
		{
			anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::TIMER));
		}
		//�������Ԃ�ݒ�
		timer = attack_list[now_at_list].attack_single[0].parameter[0].timer;
	}

	bool knock = false;	//��x�ł�knock_start�ɓ�������c��̓����蔻���knockback��S��0.0f�ɂ���
	if (!atk.empty())
	{
		//�d�͂�t�^����
		pos.y -= up_gravity * elapsed_time;
		for (auto& a : atk)
		{
			if (knock)
			{
				a.parameter.knockback = 0.0f;
			}
			if (a.knock_start)
			{
				PosKnockPlus(a.parameter.knockback);
				a.parameter.knockback = 0.0f;
				knock = true;
			}
		}
	}

	if (timer > 0.0f && timer < target_max)
	{
		//�����t���[�������炵�Ă���
		timer -= elapsed_time;
	}


	//�������Ԃ��S�ďI���������Ƃ��m�F����
	if (timer < 0.0f)
	{
	////�U�����S�ďI���������Ƃ��m�F����
	//if (AttackEndCheck())
	//{
		//�܂��U�����c���Ă���Ύ��̍U���Ɉڂ�
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[scastI(attack_state)].attack_single[attack_list[now_at_list].now_attack_num].fream;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;
			//�㌄��ݒ�
			later = attack_list[now_at_list].later;
			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;
			HitBoxTransition(HitBoxState::NOGUARD);
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
		}
	}
}

//�󒆎�
void Knight::A_Jaku(float elapsed_time)
{
	AttackDefault(elapsed_time);
}


//�󒆒�
void Knight::A_Thu(float elapsed_time)
{
	AttackDefault(elapsed_time);
}


//�󒆋�
void Knight::A_Kyo(float elapsed_time)
{
	AttackDefault(elapsed_time);
}


//�󒆏㋭
void Knight::A_UKyo(float elapsed_time)
{
	//�㌄���ݒ肳�ꂽ��͂��̊֐��ɂ͓���Ȃ�
	if (later > -1 && later < target_max)
	{
		return;
	}

	//�����t���[���ɂȂ�܂ŉ�
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;
	}
	//�d�͂̋t����t�^����
	if (speed.y < 30.0f)
	{
		pos.y += gravity * elapsed_time;
	}
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	//�����t���[���ɂȂ�����U������𐶐�����
	if (fream < 0.0f)
	{
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NONE;
		//�O�i���Ȃ��悤�ɂ���
		speed_X.Set(0.0f);

		if (speed.y < 30.0f)
		{
			speed_Y.Set(attack_list[now_at_list].advance_speed);
		}

		//int attack_num = attack_list[real].now_attack_num;
		anim_ccodinate = ac_attack[now_at_list].timer;
		if (attack_list[now_at_list].now_attack_num == 0)
		{
			//����̍U���̂݃A�j���[�V������ς���
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::TIMER));
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

		//�������Ԃ�ݒ�
		timer = attack_list[now_at_list].attack_single[0].parameter[0].timer;
		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}



	bool knock = false;	//��x�ł�knock_start�ɓ�������c��̓����蔻���knockback��S��0.0f�ɂ���
	if (!atk.empty())
	{
		for (auto& a : atk)
		{
			if (knock)
			{
				a.parameter.knockback = 0.0f;
				a.knock_start = false;
			}
			if (a.knock_start)
			{
				PosKnockPlus(a.parameter.knockback);
				a.parameter.knockback = 0.0f;
				knock = true;
				a.knock_start = false;
				//������ւ̗͂�ݒ肷��
				if (attack_list[now_at_list].ground_on == Ground_C::AIR)
				{
					//speed.y = 0.0f;
				}
			}
		}
	}


	if (timer > 0.0f && timer < target_max)
	{
		//�����t���[�������炵�Ă���
		timer -= elapsed_time;
	}

	//�������Ԃ��S�ďI���������Ƃ��m�F����
	if (timer < 0.0f)
	{
	////�U�����S�ďI���������Ƃ��m�F����
	//if (AttackEndCheck())
	//{
		//�܂��U�����c���Ă���Ύ��̍U���Ɉڂ�
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[now_at_list].attack_single[attack_list[now_at_list].now_attack_num].fream;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;
			//�㌄��ݒ�
			later = attack_list[now_at_list].later;
			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;
			HitBoxTransition(HitBoxState::NOGUARD);
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}

//----------------------------------
//		�͂ݍU��
void Knight::Steal(float elapsed_time)
{
	//�㌄���ݒ肳�ꂽ��͂��̊֐��ɂ͓���Ȃ�
	if (later > -1 && later < target_max)
	{
		return;
	}

	//��Ń_�b�V�����Ē݂͂ɂ�����悤�ɂ���


	//�����t���[���ɂȂ�܂ŉ�
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;
	}
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	//�����t���[���ɂȂ�����U������𐶐�����
	if (fream < 0.0f)
	{
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NONE;
		//�O�i���Ȃ��悤�ɂ���
		speed_X.Set(0.0f);

		//int attack_num = attack_list[real].now_attack_num;
		anim_ccodinate = ac_attack[now_at_list].timer;
		if (attack_list[now_at_list].now_attack_num == 0)
		{
			//����̍U���̂݃A�j���[�V������ς���
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::TIMER));
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

		//�������Ԃ�ݒ�
		timer = attack_list[now_at_list].attack_single[0].parameter[0].timer;
		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}



	bool knock = false;	//��x�ł�knock_start�ɓ�������c��̓����蔻���knockback��S��0.0f�ɂ���
	if (!atk.empty())
	{
		for (auto& a : atk)
		{
			if (knock)
			{
				a.parameter.knockback = 0.0f;
				a.knock_start = false;
			}
			if (a.knock_start)
			{
				PosKnockPlus(a.parameter.knockback);
				a.parameter.knockback = 0.0f;
				knock = true;
				a.knock_start = false;
			}
		}
	}

	if (knock)
	{
		//�͂ݍU�����q�b�g�����瓊���U���Ɉڍs����

		int slow = scastI(AttackState::SLOW);
		int steal = scastI(AttackState::STEAL);

		attack_state = AttackState::SLOW;
		AllAttackClear();
		HitBoxTransition(HitBoxState::NOGUARD);
		//���̍U�����L�����Z�����邽�߂̏�����ۑ�����
		atk_result = attack_list[slow].conditions_hit;
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
		fream = attack_list[slow].attack_single[0].fream;
		//�U���ԍ���������
		attack_list[slow].now_attack_num = 0;
		//�㌄��������
		later = non_target;
		//������������
		timer = non_target;
		//�������Ԃ��Z�b�g����
		steal_escape = attack_list[steal].attack_single[0].parameter[0].param.steal_timer;
		//�U�����ƂɌʂ̐ݒ���s��
		AttackDetailsSet(attack_list[slow].combo);
		return;
	}

	if (timer > 0.0f && timer < target_max)
	{
		//�����t���[�������炵�Ă���
		timer -= elapsed_time;
	}

	//if (atk.empty())
	//{
	//	//�����U�����܂��o�Ă��Ȃ��Ȃ炱����return���Ď��̍U���Ɉڂ�Ȃ��悤�ɂ���
	//	return;
	//}

	//�������Ԃ��S�ďI���������Ƃ��m�F����
	if (timer < 0.0f)
	{
	////�U�����S�ďI���������Ƃ��m�F����
	//if (AttackEndCheck())
	//{
		//�܂��U�����c���Ă���Ύ��̍U���Ɉڂ�
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[now_at_list].attack_single[attack_list[now_at_list].now_attack_num].fream;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;
			//�㌄��ݒ�
			later = attack_list[now_at_list].later;
			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;
			HitBoxTransition(HitBoxState::NOGUARD);
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}


//---------------------------------------------
//		�����U��
void Knight::Slow(float elapsed_time)
{
	//�㌄���ݒ肳�ꂽ��͂��̊֐��ɂ͓���Ȃ�
	if (later > -1 && later < target_max)
	{
		return;
	}

	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	if (steal_escape > 0.0f && steal_escape < target_max)
	{
		steal_escape -= elapsed_time;

		if (rival_state != ActState::STATENONE)
		{
			//�U���ԍ���������
			attack_list[scastI(attack_list[scastI(attack_state)].real_attack)].now_attack_num = 0;
			//�㌄��ݒ�
			later = attack_list[scastI(attack_list[scastI(attack_state)].real_attack)].later;
			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[scastI(attack_list[scastI(attack_state)].real_attack)].later;
			HitBoxTransition(HitBoxState::NOGUARD);
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].real_attack)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].real_attack)], scastI(AnimAtk::LATER));
			}
			//�͂ݔ������ꂽ��͂������悤�ɂ���
			speed.x = (-300.0f * rightOrleft);
			steal_escape = non_target;
			//�s���I���t���O���I����
			finish = true;
		}

		if (steal_escape < 0.0f)
		{
			steal_escape = non_target;
			int steal = scastI(AttackState::STEAL);

			if (rightOrleft > 0)
			{
				if (pad->x_input[scastI(PAD::STICK_L)] > 0)
				{
					FRAMEWORK.scenegame.PlayerMoveReq(now_player, pos);
					pos = tracking.rival_Pos;
					rightOrleft = -rightOrleft;
				}
			}
			else
			{
				if (pad->x_input[scastI(PAD::STICK_R)] > 0)
				{
					FRAMEWORK.scenegame.PlayerMoveReq(now_player, pos);
					pos = tracking.rival_Pos;
					rightOrleft = -rightOrleft;
				}
			}
			int slow = scastI(AttackState::SLOW);
			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[slow].fream;

			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[slow], scastI(AnimAtk::FREAM));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[slow], scastI(AnimAtk::FREAM));
			}
		}
		else
		{
			return;
		}
	}

	//�����t���[���ɂȂ�܂ŉ�
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;
	}
	//�����t���[���ɂȂ�����U������𐶐�����
	if (fream < 0.0f)
	{
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NONE;
		speed_X.Set(attack_list[now_at_list].advance_speed);

		//int attack_num = attack_list[real].now_attack_num;
		anim_ccodinate = ac_attack[now_at_list].timer;
		if (attack_list[now_at_list].now_attack_num == 0)
		{
			//����̍U���̂݃A�j���[�V������ς���
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::TIMER));
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

		//�������Ԃ�ݒ�
		timer = attack_list[now_at_list].attack_single[0].parameter[0].timer;
		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}



	bool knock = false;	//��x�ł�knock_start�ɓ�������c��̓����蔻���knockback��S��0.0f�ɂ���
	if (!atk.empty())
	{
		for (auto& a : atk)
		{
			if (knock)
			{
				a.parameter.knockback = 0.0f;
				a.knock_start = false;
			}
			if (a.knock_start)
			{
				PosKnockPlus(a.parameter.knockback);
				a.parameter.knockback = 0.0f;
				knock = true;
				a.knock_start = false;
				//������ւ̗͂�ݒ肷��
				if (attack_list[now_at_list].ground_on == Ground_C::AIR)
				{
					//speed.y = 0.0f;
				}
			}
		}
	}


	if (timer > 0.0f && timer < target_max)
	{
		//�����t���[�������炵�Ă���
		timer -= elapsed_time;
	}

	//if (atk.empty())
	//{
	//	//�����U�����܂��o�Ă��Ȃ��Ȃ炱����return���Ď��̍U���Ɉڂ�Ȃ��悤�ɂ���
	//	return;
	//}

	//�������Ԃ��S�ďI���������Ƃ��m�F����
	if (timer < 0.0f)
	{
	////�U�����S�ďI���������Ƃ��m�F����
	//if (AttackEndCheck())
	//{
		//�܂��U�����c���Ă���Ύ��̍U���Ɉڂ�
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[now_at_list].attack_single[attack_list[now_at_list].now_attack_num].fream;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;
			//�㌄��ݒ�
			later = attack_list[now_at_list].later;
			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;
			HitBoxTransition(HitBoxState::NOGUARD);
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}


void Knight::Kyo_Lhurf(float elapsed_time)
{
	AttackDefault(elapsed_time);
}


void Knight::Jaku_Rhurf(float elapsed_time)
{
	AttackProjectileDefault(elapsed_time);
}


void Knight::Thu_Rhurf(float elapsed_time)
{
	//�㌄���ݒ肳�ꂽ��͂��̊֐��ɂ͓���Ȃ�
	if (later > -1 && later < target_max)
	{
		return;
	}

	//�����t���[���ɂȂ�܂ŉ�
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;
		timer = non_target;
	}
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	//�����t���[���ɂȂ�����U������𐶐�����
	if (fream < 0.0f)
	{
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NONE;
		//�O�i���Ȃ��悤�ɂ���
		speed_X.Set(0.0f);

		//int attack_num = attack_list[real].now_attack_num;
		anim_ccodinate = ac_attack[now_at_list].timer;
		if (attack_list[now_at_list].now_attack_num == 0)
		{
			//����̍U���̂݃A�j���[�V������ς���
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::TIMER));
			}
		}


		attack_list[now_at_list].SetAttack(&projectile_atk, rightOrleft, pos, attack_list[now_at_list].speed);

		//YRGetEffect().PlayEffect(EffectKind::FIRE_DRILL, projectile_atk.back().handle, projectile_atk.back().pos.GetDXFLOAT3(), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), -90.0f * rightOrleft);

		//projectile_atk.back().effect_kind = EffectKind::FIRE_DRILL;

		//SE�Đ�
		GetSound().SESinglePlay(SEKind::PROJECTILE);

		//�����t���[����������
		fream = non_target;

		//�������Ԃ�ݒ�
		timer = attack_list[now_at_list].timer;

		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}


	if (timer > 0.0f && timer < target_max)
	{
		//�����t���[�������炵�Ă���
		timer -= elapsed_time;
	}

	//�������Ԃ��S�ďI���������Ƃ��m�F����
	if (timer < 0.0f)
	{
		//�܂��U�����c���Ă���Ύ��̍U���Ɉڂ�
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[now_at_list].attack_single[attack_list[now_at_list].now_attack_num].fream;
			//�����t���[����������
			timer = non_target;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;
			//�㌄��ݒ�
			later = attack_list[now_at_list].later;
			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;

			HitBoxTransition(HitBoxState::NOGUARD);
			//�����t���[����������
			timer = non_target;
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}



void Knight::Kyo_Rhurf(float elapsed_time)
{
	//�㌄���ݒ肳�ꂽ��͂��̊֐��ɂ͓���Ȃ�
	if (later > -1 && later < target_max)
	{
		return;
	}

	//�����t���[���ɂȂ�܂ŉ�
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;
		timer = non_target;
	}
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	//�����t���[���ɂȂ�����U������𐶐�����
	if (fream < 0.0f)
	{
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NONE;
		//�O�i���Ȃ��悤�ɂ���
		speed_X.Set(0.0f);

		//int attack_num = attack_list[real].now_attack_num;
		anim_ccodinate = ac_attack[now_at_list].timer;
		if (attack_list[now_at_list].now_attack_num == 0)
		{
			//����̍U���̂݃A�j���[�V������ς���
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::TIMER));
			}
		}


		attack_list[now_at_list].SetAttack(&projectile_atk, rightOrleft, pos, attack_list[now_at_list].speed);

		//YRGetEffect().PlayEffect(EffectKind::POWER_DRILL, projectile_atk.back().handle, projectile_atk.back().pos.GetDXFLOAT3(), DirectX::XMFLOAT3(1.5f, 1.5f, 1.5f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), -90.0f * rightOrleft);

		//projectile_atk.back().effect_kind = EffectKind::POWER_DRILL;

		//SE�Đ�
		GetSound().SESinglePlay(SEKind::PROJECTILE);

		//�����t���[����������
		fream = non_target;

		//�������Ԃ�ݒ�
		timer = attack_list[now_at_list].timer;

		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}


	if (timer > 0.0f && timer < target_max)
	{
		//�����t���[�������炵�Ă���
		timer -= elapsed_time;
	}

	//�������Ԃ��S�ďI���������Ƃ��m�F����
	if (timer < 0.0f)
	{
		//�܂��U�����c���Ă���Ύ��̍U���Ɉڂ�
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[now_at_list].attack_single[attack_list[now_at_list].now_attack_num].fream;
			//�����t���[����������
			timer = non_target;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;
			//�㌄��ݒ�
			later = attack_list[now_at_list].later;
			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;

			HitBoxTransition(HitBoxState::NOGUARD);
			//�����t���[����������
			timer = non_target;
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}


//--------------------------------------
//�@214�R�}���h��
void Knight::Jaku_Lhurf(float elapsed_time)
{
	//�㌄���ݒ肳�ꂽ��͂��̊֐��ɂ͓���Ȃ�
	if (later > -1 && later < target_max)
	{
		return;
	}
	speed_X.Set(0.0f);
	//�����t���[���ɂȂ�܂ŉ�
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;
	}
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	//�����t���[���ɂȂ�����U������𐶐�����
	if (fream < 0.0f)
	{
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NONE;
		//�O�i���Ȃ��悤�ɂ���
		speed_X.Set(0.0f);

		//int attack_num = attack_list[real].now_attack_num;
		anim_ccodinate = ac_attack[now_at_list].timer;
		if (attack_list[now_at_list].now_attack_num == 0)
		{
			//����̍U���̂݃A�j���[�V������ς���
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::TIMER));
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

		//atk.back().effect_kind = EffectKind::TORNADE;
		//YRGetEffect().PlayEffect(EffectKind::TORNADE, atk.back().handle, atk.back().pos.GetDXFLOAT3(), DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);
		fream = non_target;
		//�������Ԃ�ݒ�
		timer = attack_list[now_at_list].attack_single[0].parameter[0].timer;

		GetSound().SESinglePlay(SEKind::TORNADO);

		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}

	bool knock = false;	//��x�ł�knock_start�ɓ�������c��̓����蔻���knockback��S��0.0f�ɂ���
	if (!atk.empty())
	{
		//�U�����莞�O�i�����ĉ�]������
		speed.x = attack_list[now_at_list].advance_speed * rightOrleft;
		angle.y += elapsed_time * (50.0f * rightOrleft);

		for (auto& a : atk)
		{
			if (knock)
			{
				a.parameter.knockback = 0.0f;
				a.knock_start = false;
			}
			if (a.knock_start)
			{
				PosKnockPlus(a.parameter.knockback);
				a.parameter.knockback = 0.0f;
				knock = true;
				a.knock_start = false;
			}
		}
	}

	if (timer > 0.0f && timer < target_max)
	{
		//�����t���[�������炵�Ă���
		timer -= elapsed_time;
	}


	//�������Ԃ��S�ďI���������Ƃ��m�F����
	if (timer < 0.0f)
	{
	////�U�����S�ďI���������Ƃ��m�F����
	//if (AttackEndCheck())
	//{
		//�܂��U�����c���Ă���Ύ��̍U���Ɉڂ�
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[now_at_list].attack_single[attack_list[now_at_list].now_attack_num].fream;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;
			//�㌄��ݒ�
			later = attack_list[now_at_list].later;
			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;
			//�p�x��߂�
			angle.y = 0.0f;
			//���x��߂�
			speed.x = 0.0f;
			HitBoxTransition(HitBoxState::NOGUARD);
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}


//--------------------------------------
//�@��214�R�}���h��
void Knight::A_Jaku_Lhurf(float elapsed_time)
{
	//�㌄���ݒ肳�ꂽ��͂��̊֐��ɂ͓���Ȃ�
	if (later > -1 && later < target_max)
	{
		return;
	}
	speed_X.Set(0.0f);
	//�����t���[���ɂȂ�܂ŉ�
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;
	}
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	//�����t���[���ɂȂ�����U������𐶐�����
	if (fream < 0.0f)
	{
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NONE;
		//�O�i���Ȃ��悤�ɂ���
		speed_X.Set(0.0f);

		//int attack_num = attack_list[real].now_attack_num;
		anim_ccodinate = ac_attack[now_at_list].timer;
		if (attack_list[now_at_list].now_attack_num == 0)
		{
			//����̍U���̂݃A�j���[�V������ς���
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::TIMER));
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
		//atk.back().effect_kind = EffectKind::TORNADE;
		//YRGetEffect().PlayEffect(EffectKind::TORNADE, atk.back().handle, atk.back().pos.GetDXFLOAT3(), DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);

		GetSound().SESinglePlay(SEKind::TORNADO);

		fream = non_target;

		//�������Ԃ�ݒ�
		timer = attack_list[now_at_list].attack_single[0].parameter[0].timer;

		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}

	bool knock = false;	//��x�ł�knock_start�ɓ�������c��̓����蔻���knockback��S��0.0f�ɂ���
	if (!atk.empty())
	{
		//�U�����莞�O�i�����ĉ�]������
		speed.x = attack_list[now_at_list].advance_speed * rightOrleft;
		angle.y += elapsed_time * (50.0f * rightOrleft);

		//�󒆂Ȃ̂ŏ����ӂ���Ƃ�����
		pos.y += (gravity * elapsed_time);
		for (auto& a : atk)
		{
			if (knock)
			{
				a.parameter.knockback = 0.0f;
				a.knock_start = false;
			}
			if (a.knock_start)
			{
				PosKnockPlus(a.parameter.knockback);
				a.parameter.knockback = 0.0f;
				knock = true;
				a.knock_start = false;
			}
		}
	}

	if (timer > 0.0f && timer < target_max)
	{
		//�����t���[�������炵�Ă���
		timer -= elapsed_time;
	}



	//�������Ԃ��S�ďI���������Ƃ��m�F����
	if (timer < 0.0f)
	{
	////�U�����S�ďI���������Ƃ��m�F����
	//if (AttackEndCheck())
	//{
		//�܂��U�����c���Ă���Ύ��̍U���Ɉڂ�
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[now_at_list].attack_single[attack_list[now_at_list].now_attack_num].fream;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;
			//�㌄��ݒ�
			later = attack_list[now_at_list].later;
			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;
			//�p�x��߂�
			angle.y = 0.0f;
			//���x��߂�
			speed.x = 0.0f;
			HitBoxTransition(HitBoxState::NOGUARD);
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}



void Knight::Thu_Lhurf(float elapsed_time)
{
	//�㌄���ݒ肳�ꂽ��͂��̊֐��ɂ͓���Ȃ�
	if (later > -1 && later < target_max)
	{
		return;
	}
	speed_X.Set(0.0f);
	//�����t���[���ɂȂ�܂ŉ�
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;
	}
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	//�����t���[���ɂȂ�����U������𐶐�����
	if (fream < 0.0f)
	{
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NONE;
		//�O�i���Ȃ��悤�ɂ���
		speed_X.Set(0.0f);

		//int attack_num = attack_list[real].now_attack_num;
		anim_ccodinate = ac_attack[now_at_list].timer;
		if (attack_list[now_at_list].now_attack_num == 0)
		{
			//����̍U���̂݃A�j���[�V������ς���
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::TIMER));
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

		//atk.back().effect_kind = EffectKind::TORNADE;
		//YRGetEffect().PlayEffect(EffectKind::TORNADE, atk.back().handle, atk.back().pos.GetDXFLOAT3(), DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);
		fream = non_target;

		GetSound().SESinglePlay(SEKind::TORNADO);
		//speed.x = attack_list[now_at_list].advance_speed * rightOrleft;

		//�������Ԃ�ݒ�
		timer = attack_list[now_at_list].attack_single[0].parameter[0].timer;

		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}

	bool knock = false;	//��x�ł�knock_start�ɓ�������c��̓����蔻���knockback��S��0.0f�ɂ���
	if (!atk.empty())
	{
		for (auto& a : atk)
		{
			if (knock)
			{
				a.parameter.knockback = 0.0f;
				a.knock_start = false;
			}
			if (a.knock_start)
			{
				PosKnockPlus(a.parameter.knockback);
				a.parameter.knockback = 0.0f;
				knock = true;
				a.knock_start = false;
				//�������ԍĐݒ�
				timer = attack_list[now_at_list].attack_single[0].parameter[0].timer;
			}
		}
	}

	if (timer > 0.0f && timer < target_max)
	{
		//�����t���[�������炵�Ă���
		timer -= elapsed_time;
		if (atk.empty())
		{
			//�����邩�K�[�h���ꂽ���ނ����Ȃ����]������
			speed.x = -attack_list[now_at_list].advance_speed * rightOrleft;
			angle.y -= elapsed_time * (50.0f * rightOrleft);
		}
		else
		{
			//�U�����莞�O�i�����ĉ�]������
			speed.x = attack_list[now_at_list].advance_speed * rightOrleft;
			angle.y += elapsed_time * (50.0f * rightOrleft);
		}
	}


	//�������Ԃ��S�ďI���������Ƃ��m�F����
	if (timer < 0.0f)
	{
	////�U�����S�ďI���������Ƃ��m�F����
	//if (AttackEndCheck())
	//{
		//�܂��U�����c���Ă���Ύ��̍U���Ɉڂ�
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[now_at_list].attack_single[attack_list[now_at_list].now_attack_num].fream;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;
			//�㌄��ݒ�
			later = attack_list[now_at_list].later;
			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;
			//�p�x��߂�
			angle.y = 0.0f;
			//���x��߂�
			speed.x = -attack_list[now_at_list].advance_speed * rightOrleft;
			HitBoxTransition(HitBoxState::NOGUARD);
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}


//�z�[�~���O�_�b�V��
void Knight::TrackDash(float decision, float elapsed_time)
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
	plusVec = tracking.Veccalculate(hit[scastI(KNIGHTHIT::BODY)].center, decision);

	//�㌄���ݒ肳�ꂽ��͂��̊֐��ɂ͓���Ȃ�
	if (later > -1 && later < target_max)
	{
		return;
	}

	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);

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
	//�����t���[���ɂȂ�����U������𐶐�����
	if (fream < 0.0f)
	{
		//int attack_num = attack_list[real].now_attack_num;
		anim_ccodinate = ac_attack[now_at_list].timer;
		if (attack_list[now_at_list].now_attack_num == 0)
		{
			//����̍U���̂݃A�j���[�V������ς���
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::TIMER));
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

		//�������Ԃ�ݒ�
		timer = attack_list[now_at_list].attack_single[0].parameter[0].timer;

		HitBoxTransition(HitBoxState::PROJECTILE_ARMOR);
		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));

		//YRGetEffect().PlayEffect(EffectKind::TRACK, pos.GetDXFLOAT3(), DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);
	}



	bool knock = false;	//��x�ł�knock_start�ɓ�������c��̓����蔻���knockback��S��0.0f�ɂ���
	if (!atk.empty())
	{
		//��ɉ�]������
		angle.z -= (50.0f*rightOrleft) * elapsed_time;

		pos.x += ((plusVec.x * chara_state.track_speed) * elapsed_time);
		pos.y += ((plusVec.y * chara_state.track_speed) * elapsed_time);

		GetSound().SEPlay(SEKind::TRACK);

		//YRGetEffect().SetLocation(EffectKind::TRACK, pos.GetDXFLOAT3());

		for (auto& a : atk)
		{
			if (knock && a.attack_name == scastI(attack_state))
			{
				a.parameter.knockback = 0.0f;
			}
			if (a.knock_start && a.attack_name == scastI(attack_state))
			{
				PosKnockPlus(a.parameter.knockback);
				a.parameter.knockback = 0.0f;
				knock = true;
			}
		}
	}

	if (knock)
	{
		//�z�[�~���O�_�b�V���͓����������_�ōU�����I������̂Ō㌄����͂���
		//�U������Y���W�𑊎�̍��W�ɌŒ肷��
		pos.y = tracking.rival_Pos.y;
		//X���W���ǌ��\�Ȉʒu�ɌŒ肷��
		pos.x = tracking.rival_Pos.x + (track_adjust_x * (-decision));
		//������ւ̑��x����͂���
		speed_X.Set(0.0f);
		//speed_Y.Set(attack_list[now_at_list].advance_speed);
		speed.y = attack_list[now_at_list].advance_speed;


		//YRGetEffect().StopEffect(EffectKind::TRACK);

		//�G�t�F�N�g���~�߂�ׁA��x�X�V�������|����
		AttackUpdate(0.0f);

		//�U�������ׂď�������
		AllAttackClear();
		//�U���ԍ���������
		attack_list[now_at_list].now_attack_num = 0;
		//������������
		timer = non_target;
		//�㌄��ݒ�
		later = attack_list[now_at_list].later;
		//�A�j���[�V�������x���w��
		anim_ccodinate = ac_attack[now_at_list].later;
		HitBoxTransition(HitBoxState::NOGUARD);
		//�`����Z�b�g
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::LATER));
		}
		else
		{
			anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::LATER));
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
	else
	{
		if (timer > 0.0f && timer < target_max)
		{
			//�Œ�ۏ�Ƃ���1�t���[���͎�������
			if (timer < attack_list[now_at_list].attack_single[0].parameter[0].timer - elapsed_time)
			{
				YR_Vector3 dis = tracking.rival_Pos - pos;
				if (dis.Length() < attack_list[now_at_list].attack_single[0].parameter[0].size.x)
				{
					//������Ȃ������ꍇ�A���̋����ȏ�ɋ߂Â��Ă���΋����I�ɏI��������

					//�G�t�F�N�g���~�߂�ׁA��x�X�V�������|����
					AttackUpdate(0.0f);
					//�U�������ׂď�������
					AllAttackClear();
					//�^�C�}�[���}�C�i�X�ɂ���
					timer = -0.1f;
					//YRGetEffect().StopEffect(EffectKind::TRACK);
				}
			}
		}
	}


	if (timer > 0.0f && timer < target_max)
	{
		//�����t���[�������炵�Ă���
		timer -= elapsed_time;
	}


	//�������Ԃ��S�ďI���������Ƃ��m�F����
	if (timer < 0.0f&&timer<target_max)
	{
	////�U�����S�ďI���������Ƃ��m�F����
	//if (AttackEndCheck())
	//{
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
			if (plusVec.y > 0.5f)
			{
				//��Ɍ������Ĕ��ł���ꍇ�͑��x������
				speed.y = attack_list[now_at_list].advance_speed;
			}
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;
			//�㌄��ݒ�
			later = attack_list[now_at_list].later;
			//������������
			timer = non_target;
			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
			//�p�x��߂�
			angle.z = 0.0f;
			angle.x = 0.0f;
			HitBoxTransition(HitBoxState::NOGUARD);
			//�W�����v�񐔂����炷
			if (jumpcount != 0)
			{
				jumpcount = 1;
			}
			//�z�[�~���O�_�b�V���񐔂����炷
			trackgauge--;
			if (pos.y > POS_Y)
			{
				//�W�����v��Ԃɂ���
				jumpflag = true;
				max_jump_flag = true;
			}
		}
	}

}


void Knight::ExtendATK(float elapsed_time)
{
	AttackDefault(elapsed_time);
}



void Knight::SpecialAttack(float elapsed_time)
{
	if (later > -1 && later < target_max)
	{
		return;
	}

	if (fream < target_max)
	{
		//�U�������̌��ʂ�ۑ�����
		YRCamera.RequestCamera(Camera::Request::HOLD, now_player);
		if (now_player > 1)
		{
			YRCamera.camera_state = Camera::CAMERA_STATE::PLAYER2P;
		}
		else
		{
			YRCamera.camera_state = Camera::CAMERA_STATE::PLAYER1P;
		}

		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;

		//YR_Vector3	origin_focus;
		//YR_Vector3	origin_eye;
		YR_Vector3	focus;
		YR_Vector3	eye;

		//YRCamera.SetEye(eye.GetDXFLOAT3());
		//YRCamera.SetFocus(focus.GetDXFLOAT3());


		if (fream > 1.5f)
		{
			//�J�����̃X�e�[�g���X�V�ł͂Ȃ��ꍇ�͏������ɂ���
			if (camera_state_knight != CAMERA_STATE_KNIGHT::SECOND)
			{
				camera_state_knight = CAMERA_STATE_KNIGHT::FIRST;
			}
			YRCamera.SetFov(50.0f * 0.01745f);
		}
		else if (fream > 1.0f)
		{
			//�J�����̃X�e�[�g���X�V�ł͂Ȃ��ꍇ�͏������ɂ���
			if (camera_state_knight != CAMERA_STATE_KNIGHT::FOUR)
			{
				camera_state_knight = CAMERA_STATE_KNIGHT::THIRD;
			}
			ChangeFace(FaceAnim::YARUKI);
			YRCamera.SetFov(50.0f * 0.01745f);
		}
		else if (fream > 0.2f)
		{
			//�J�����̃X�e�[�g���X�V�ł͂Ȃ��ꍇ�͏������ɂ���
			if (camera_state_knight != CAMERA_STATE_KNIGHT::SIX)
			{
				camera_state_knight = CAMERA_STATE_KNIGHT::FIVE;
			}
			YRCamera.SetFov(50.0f * 0.01745f);
		}
		else
		{
			camera_state_knight = CAMERA_STATE_KNIGHT::SEVEN;
		}

		//���o�ŃJ�����𓮂���
		switch (camera_state_knight)
		{
		case Knight::CAMERA_STATE_KNIGHT::FIRST:
			//���_��ݒ�(������)
			focus = YR_Vector3(pos.x, pos.y, pos.z - 2.0f);
			eye = YR_Vector3(pos.x, pos.y, pos.z - 8.5f);
			YRCamera.SetEye(eye.GetDXFLOAT3());
			YRCamera.SetFocus(focus.GetDXFLOAT3());
			camera_state_knight = CAMERA_STATE_KNIGHT::SECOND;
			GetSound().SESinglePlay(SEKind::INTRO_WIND);
			break;
		case Knight::CAMERA_STATE_KNIGHT::SECOND:
			//�w�肵���ʒu�܂ŃJ�����𓮂����Ă���(�X�V)
			focus = YR_Vector3(pos.x, pos.y + 2.5f, pos.z - 2.0f);
			eye = YR_Vector3(pos.x, pos.y + 5.5f, pos.z - 8.5f);
			YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 0.05f);
			break;
		case Knight::CAMERA_STATE_KNIGHT::THIRD:
			//���_��ݒ�(������)
			focus = YR_Vector3(pos.x, pos.y, pos.z);
			eye = YR_Vector3(pos.x - Getapply(12.0f), pos.y - 2.0f, pos.z - 7.0f);
			YRCamera.SetEye(eye.GetDXFLOAT3());
			YRCamera.SetFocus(focus.GetDXFLOAT3());
			camera_state_knight = CAMERA_STATE_KNIGHT::FOUR;
			break;
		case Knight::CAMERA_STATE_KNIGHT::FOUR:
			//�w�肵���ʒu�܂ŃJ�����𓮂����Ă���(�X�V)
			focus = YR_Vector3(pos.x, pos.y, pos.z);
			eye = YR_Vector3(pos.x - Getapply(7.0f), pos.y - 2.0f, pos.z - 7.0f);
			YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 0.05f);
			break;
		case Knight::CAMERA_STATE_KNIGHT::FIVE:
			//���_��ݒ�(������)
			focus = YR_Vector3(pos.x + Getapply(2.0f), pos.y - 6.0f, pos.z);
			eye = YR_Vector3(focus.x + Getapply(9.0f), focus.y + 2.0f, focus.z - 6.0f);
			YRCamera.SetEye(eye.GetDXFLOAT3());
			YRCamera.SetFocus(focus.GetDXFLOAT3());
			camera_state_knight = CAMERA_STATE_KNIGHT::SIX;
			GetSound().SESinglePlay(SEKind::SPECIAL_ATTACK2);
			break;
		case Knight::CAMERA_STATE_KNIGHT::SIX:
			//�w�肵���ʒu�܂ŃJ�����𓮂����Ă���(�X�V)
			eye = YR_Vector3(pos.x + Getapply(8.5f), pos.y + 2.8f, pos.z - 5.5f);
			focus = YR_Vector3(pos.x - Getapply(5.5f), pos.y, pos.z + 3.0f);
			YRCamera.SpecifiedLerp(eye.GetDXFLOAT3(), focus.GetDXFLOAT3(), 0.05f);
			break;
		case Knight::CAMERA_STATE_KNIGHT::SEVEN:
			//�J���������X�Ƀ��C���ɖ߂�
			YRCamera.RequestCamera(Camera::Request::WEAKEN, now_player);
			break;
		}
	}

	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	if (fream < 0.0f)
	{
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NONE;
		//�O�i���Ȃ��悤�ɂ���
		speed_X.Set(0.0f);
		//int attack_num = attack_list[scastI(attack_state)].now_attack_num;
		anim_ccodinate = ac_attack[now_at_list].timer;
		attack_list[now_at_list].SetAttack(&atk, rightOrleft,pos);

		//�G�t�F�N�g����
		YRGetEffect().PlayEffect(EffectKind::SPECIAL_DRILL, atk.back().handle, atk.back().pos.GetDXFLOAT3(), DirectX::XMFLOAT3(3.0f, 3.0f, 3.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), -90.0f * rightOrleft);

		atk.back().effect_param.effect_kind = EffectKind::SPECIAL_DRILL;

		fream = non_target;
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::TIMER));
		}
		else
		{
			anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::TIMER));
		}
		YRCamera.RequestCamera(Camera::Request::RELEASE, now_player);
		GetSound().SEStop(SEKind::INTRO_WIND);
		GetSound().SEStop(SEKind::SPECIAL_ATTACK2);

		YRGetEffect().StopEffect(EffectKind::WIND);

		//SE�Đ�
		GetSound().SESinglePlay(SEKind::SPECIAL_ATTACK3);

		//�U���������͖��G
		HitBoxTransition(HitBoxState::INVINCIBLE);

		//�������Ԃ�ݒ�
		timer = attack_list[now_at_list].attack_single[0].parameter[0].timer;
	}

	bool knock = false;	//��x�ł�knock_start�ɓ�������c��̓����蔻���knockback��S��0.0f�ɂ���
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
				PosKnockPlus(a.parameter.knockback);
				a.parameter.knockback = 0.0f;
				knock = true;
			}
		}
	}

	if (timer > 0.0f && timer < target_max)
	{
		//�����t���[�������炵�Ă���
		timer -= elapsed_time;
		if (hit_result != HitResult::GUARD)
		{
			//�U�����͑O�Ɉړ�������
			pos.x += elapsed_time * Getapply(150.0f);
		}
		else
		{
			//���G������
			HitBoxTransition(HitBoxState::NOGUARD);
		}
	}

	//�������Ԃ��S�ďI���������Ƃ��m�F����
	if (timer < 0.0f)
	{
	/*if (AttackEndCheck())
	{*/
		if (attack_list[now_at_list].now_attack_num < attack_list[now_at_list].attack_max)
		{
			fream = attack_list[now_at_list].attack_single[attack_list[now_at_list].now_attack_num].fream;
		}
		else
		{
			lumi_material = Model::Material_Attribute::NONE;
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;
			//�㌄��ݒ�
			later = attack_list[now_at_list].later;
			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;
			//���G������
			HitBoxTransition(HitBoxState::NOGUARD);
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[now_at_list], scastI(AnimAtk::LATER));
			}
		}
	}
}