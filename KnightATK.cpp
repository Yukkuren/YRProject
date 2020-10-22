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
		fream -= elapsed_time;
	}
	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	//�����t���[���ɂȂ�����U������𐶐�����
	if (fream < 0.0f)
	{
		//int attack_num = attack_list[real].now_attack_num;
		anim_ccodinate = ac_attack[now_at_list].timer;
		attack_list[now_at_list].SetAttack(&atk, rightOrleft,pos);
		fream = non_target;
		anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));

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
				if (!ground)
				{
					speed_Y.Set(60.0f);
				}
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
			attack_list[now_at_list].now_attack_num = 0;
			later = attack_list[now_at_list].later;
			anim_ccodinate = ac_attack[now_at_list].later;
			anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::LATER));
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
	AttackDefault(elapsed_time);
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
	//�㌄���ݒ肳�ꂽ��͂��̊֐��ɂ͓���Ȃ�
	if (later > -1 && later < target_max)
	{
		return;
	}

	//�����t���[���ɂȂ�܂ŉ�
	if (fream < target_max)
	{
		fream -= elapsed_time;
	}


	//�����t���[���ɂȂ�����U������𐶐�����
	if (fream < 0.0f)
	{
		int attack_num = attack_list[scastI(attack_state)].now_attack_num;
		anim_ccodinate = ac_attack[scastI(attack_state)].timer;
		attack_list[scastI(attack_state)].SetAttack(&atk, rightOrleft,pos);
		fream = non_target;
		anim->NodeChange(model_motion.u_kyo_R, scastI(AnimAtk::TIMER));

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
				pos.x -= a.parameter.knockback * rightOrleft;
				a.parameter.knockback = 0.0f;
				if (!ground)
				{
					speed_Y.Set(60.0f);
				}
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
			attack_list[now_at_list].now_attack_num = 0;
			later = attack_list[now_at_list].later;
			anim_ccodinate = ac_attack[scastI(attack_state)].later;
			anim->NodeChange(model_motion.u_kyo_R, scastI(AnimAtk::LATER));
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
	//AttackDefault(elapsed_time);
	AttackDefault(elapsed_time);
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
		int attack_num = attack_list[scastI(attack_state)].now_attack_num;
		anim_ccodinate = ac_attack[scastI(attack_state)].timer;
		attack_list[scastI(attack_state)].SetAttack(&atk, rightOrleft,pos);
		fream = non_target;
		anim->NodeChange(model_motion.special_R,scastI(AnimAtk::TIMER));
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
				if (!ground)
				{
					speed_Y.Set(60.0f);
				}
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
			attack_list[now_at_list].now_attack_num = 0;
			later = attack_list[now_at_list].later;
			anim_ccodinate = ac_attack[scastI(attack_state)].later;
			anim->NodeChange(model_motion.special_R,scastI(AnimAtk::LATER));
		}
	}

	specialfream = 0;
}


//
//--------------------------------------------------------------------------------------------