#include "Knight.h"
#include "PlayerBase.h"
#include "Key.h"
#include "YRGamePad.h"
#include "camera.h"

//------------------------------------------------------
//				�U���֐�


void Knight::Slow(float elapsed_time)
{
	AttackDefault(elapsed_time);
}


void Knight::AttackDefault(float elapsed_time)
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
		for (auto& a : atk)
		{
			if (knock)
			{
				a.parameter.knockback = 0.0f;
				a.knock_start = false;
			}
			if (a.knock_start)
			{
				pos.x -= a.parameter.knockback * rightOrleft;
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
		}
	}
}


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
			attack_list[now_at_list].SetAttack(&atk, rightOrleft, pos, YR_Vector3(attack_list[now_at_list].speed.x*rightOrleft, attack_list[now_at_list].speed.y));
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
		for (auto& a : atk)
		{
			if (knock)
			{
				a.parameter.knockback = 0.0f;
			}
			if (a.knock_start)
			{
				pos.x -= a.parameter.knockback * rightOrleft;
				a.parameter.knockback = 0.0f;
				knock = true;
			}
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

	//�����t���[���ɂȂ�����U������𐶐�����
	if (fream < 0.0f)
	{
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NONE;
		//�O�i���Ȃ��悤�ɂ���
		speed_X.Set(0.0f);
		int attack_num = attack_list[scastI(attack_state)].now_attack_num;
		anim_ccodinate = ac_attack[scastI(attack_state)].timer;
		attack_list[scastI(attack_state)].SetAttack(&atk, rightOrleft,pos);
		fream = non_target;
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.u_kyo_R, scastI(AnimAtk::TIMER));
		}
		else
		{
			anim->NodeChange(model_motion.u_kyo_L, scastI(AnimAtk::TIMER));
		}
	}

	int now_at_list = scastI(attack_state);

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
				pos.x -= a.parameter.knockback * rightOrleft;
				a.parameter.knockback = 0.0f;
				knock = true;
			}
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
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L[now_at_list], scastI(AnimAtk::LATER));
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
	pos.y += gravity * elapsed_time;
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	//�����t���[���ɂȂ�����U������𐶐�����
	if (fream < 0.0f)
	{
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NONE;
		//�O�i���Ȃ��悤�ɂ���
		speed_X.Set(0.0f);

		speed_Y.Set(attack_list[now_at_list].advance_speed);

		//int attack_num = attack_list[real].now_attack_num;
		anim_ccodinate = ac_attack[now_at_list].timer;
		if (attack_list[now_at_list].now_attack_num == 0)
		{
			//����̍U���̂݃A�j���[�V������ς���
			if (rightOrleft > 0)
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
		for (auto& a : atk)
		{
			if (knock)
			{
				a.parameter.knockback = 0.0f;
				a.knock_start = false;
			}
			if (a.knock_start)
			{
				pos.x -= a.parameter.knockback * rightOrleft;
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
		}
	}
}


void Knight::P_Kyo(float elapsed_time)
{
	AttackDefault(elapsed_time);
}


void Knight::Hadouken(float elapsed_time)
{
	AttackDefault(elapsed_time);
}


void Knight::Thu_Hadouken(float elapsed_time)
{
	AttackDefault(elapsed_time);
}



void Knight::Kyo_Hadouken(float elapsed_time)
{
	AttackDefault(elapsed_time);
	
}



void Knight::Panish_N(float elapsed_time)
{
	AttackDefault(elapsed_time);
}



void Knight::Panish_H(float elapsed_time)
{
	AttackDefault(elapsed_time);
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


	if (fream < 0.0f)
	{
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NONE;
		//�O�i���Ȃ��悤�ɂ���
		speed_X.Set(0.0f);
		int attack_num = attack_list[scastI(attack_state)].now_attack_num;
		anim_ccodinate = ac_attack[scastI(attack_state)].timer;
		attack_list[scastI(attack_state)].SetAttack(&atk, rightOrleft,pos);
		fream = non_target;
		if (rightOrleft > 0)
		{
			anim->NodeChange(model_motion.special_R, scastI(AnimAtk::TIMER));
		}
		else
		{
			anim->NodeChange(model_motion.special_L, scastI(AnimAtk::TIMER));
		}
		YRCamera.RequestCamera(Camera::Request::RELEASE, now_player);
	}

	int now_at_list = scastI(attack_state);

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
				pos.x -= a.parameter.knockback;
				a.parameter.knockback = 0.0f;
				knock = true;
			}
		}
		//pos.x += elapsed_time * Getapply(50.0f);
		pos.x += elapsed_time * Getapply(150.0f);
	}

	if (atk.empty())
	{
		//�����U�����܂��o�Ă��Ȃ��Ȃ炱����return���Ď��̍U���Ɉڂ�Ȃ��悤�ɂ���
		return;
	}

	if (AttackEndCheck())
	{
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
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L[now_at_list], scastI(AnimAtk::LATER));
			}
		}
	}
}


//---------------------------------------------------------------
// **�R���{�Z�b�g�֐�**
//�E���͂��ꂽ�U���ɃR���{�X�e�[�g�������Ă����ꍇ�A
//�@���̃R���{����͂��ꂽ�n�_����J�n��true��Ԃ�
//---------------------------------------------------------------
bool Knight::ComboSet()
{
	AttackState real = attack_list[scastI(attack_state)].real_attack;
	AttackState truth = attack_list[scastI(attack_state)].real_attack;
	AttackState combo = attack_list[scastI(attack_state)].combo;

	switch (combo)
	{
	case AttackState::COMBO_X:
	{
		//X�{�^���R���{
		combolist_X.now_pos = -1;
		if (!combolist_X.combolist.empty())
		{
			for (int i = 0; i < combolist_X.combolist.size(); i++)
			{
				//�R���{���X�g�̂ǂ̈ʒu�ɍ��̍U�������邩���m�F����
				if (combolist_X.combolist[i] == real)
				{
					combolist_X.now_pos = i;
					break;
				}
			}
		}

		if (combolist_X.now_pos < 0)
		{
			//���̍U�����R���{�Ɋ܂܂�Ă��Ȃ������ꍇfalse
			return false;
		}
		//�R���{���X�V
		combolist_X.now_pos++;

		//���̍U����ݒ肷��
		truth = combolist_X.combolist[combolist_X.now_pos];
	}
		break;
	default:
		return false;
		break;
	}

	int truth_num = scastI(truth);
	int real_num = scastI(attack_list[truth_num].real_attack);

	//�U�������肷��
	//���ݍU�����肪�o�Ă���Ȃ�S�ď�������
	AllAttackClear();
	//���̍U�����L�����Z�����邽�߂̏�����ۑ�����
	atk_result = attack_list[real_num].conditions_hit;
	//�U����ۑ�����
	last_attack = combo;
	//�U�����e���R���{��
	attack_state = combo;
	//�U���̌��ʂ�������
	hit_result = HitResult::NOT_OCCURRENCE;
	//�U�����t���O���I����
	attack = true;
	//�ړ��t���O���I�t��
	moveflag = false;
	//�s���X�e�[�g���U����
	act_state = ActState::ATTACK;
	//�����t���[��������
	fream = attack_list[real_num].attack_single[0].fream;
	//�A�j���[�V�������x���w��
	anim_ccodinate = ac_attack[real_num].fream;
	//�U���ԍ���������
	attack_list[real_num].now_attack_num = 0;
	//�U�������O�̑O�i������ݒ肷��(�n��U���̂�)
	if (attack_list[real_num].ground_on == Ground_C::GROUND)
	{
		speed_X.Set(attack_list[real_num].advance_speed);
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
		anim->NodeChange(model_motion.model_R[real_num], scastI(AnimAtk::FREAM));
	}
	else
	{
		anim->NodeChange(model_motion.model_L[real_num], scastI(AnimAtk::FREAM));
	}
	//�U�����ƂɌʂ̐ݒ���s��
	AttackDetailsSet(attack_list[real_num].combo);

	return true;
}

//---------------------------------------------------------------
// **�R���{�X�V�֐�**
//�E�R���{��i�߂Ă���
//---------------------------------------------------------------
void Knight::ComboUpdate()
{
	AttackState truth = last_attack;
	AttackState combo = last_attack;

	switch (combo)
	{
	case AttackState::COMBO_X:
	{
		//X�{�^���R���{
		//�R���{���X�V
		combolist_X.now_pos++;
		if (combolist_X.combolist.empty())
		{
			return;
		}
		if (combolist_X.now_pos >= combolist_X.combolist.size())
		{
			//�R���{�̍ŏI�n�_�������ꍇ

			return;
		}
		//���̍U����ݒ肷��
		truth = combolist_X.combolist[combolist_X.now_pos];
	}
	break;
	default:
		return;
		break;
	}

	int truth_num = scastI(truth);
	int real_num = scastI(attack_list[truth_num].real_attack);

	//�U�������肷��
	//���ݍU�����肪�o�Ă���Ȃ�S�ď�������
	AllAttackClear();
	//���̍U�����L�����Z�����邽�߂̏�����ۑ�����
	atk_result = attack_list[real_num].conditions_hit;
	//�U����ۑ�����
	last_attack = combo;
	//�U�����e���R���{��
	attack_state = combo;
	//�U���̌��ʂ�������
	hit_result = HitResult::NOT_OCCURRENCE;
	//�U�����t���O���I����
	attack = true;
	//�ړ��t���O���I�t��
	moveflag = false;
	//�s���X�e�[�g���U����
	act_state = ActState::ATTACK;
	//�����t���[��������
	fream = attack_list[real_num].attack_single[0].fream;
	//�A�j���[�V�������x���w��
	anim_ccodinate = ac_attack[real_num].fream;
	//�U���ԍ���������
	attack_list[real_num].now_attack_num = 0;
	//�U�������O�̑O�i������ݒ肷��(�n��U���̂�)
	if (attack_list[real_num].ground_on == Ground_C::GROUND)
	{
		speed_X.Set(attack_list[real_num].advance_speed);
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
		anim->NodeChange(model_motion.model_R[real_num], scastI(AnimAtk::FREAM));
	}
	else
	{
		anim->NodeChange(model_motion.model_L[real_num], scastI(AnimAtk::FREAM));
	}
	//�U�����ƂɌʂ̐ݒ���s��
	AttackDetailsSet(attack_list[real_num].combo);
}


void Knight::ComboX(float decision, float elapsed_time)
{
	//X�{�^���R���{�֐�
	if (combolist_X.now_pos >= combolist_X.combolist.size())
	{
		return;
	}
	AttackState truth = combolist_X.combolist[combolist_X.now_pos];
	int truth_num = scastI(truth);
	attack_state = attack_list[truth_num].real_attack;

	AttackSwitch(decision, elapsed_time);

	attack_state = last_attack;
}

//
//--------------------------------------------------------------------------------------------