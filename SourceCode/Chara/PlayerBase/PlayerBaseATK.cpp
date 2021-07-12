#include "PlayerBase.h"
#include "../../Effect.h"
#include "../../YRSound.h"
#include "../../framework.h"

//------------------------------------------------
//					�U���̏���
//------------------------------------------------
//�E�U���n�̊֐��͂�����Ɉڂ��Ă���
//
//�E���ʂȏ��������܂Ȃ��ꍇ��AttackDefault�֐��𒆂ŉ�
//------------------------------------------------




void Player::PosKnockPlus(float vec)
{
	//�v���C���[�̈ʒu�Ɉ����Ŏ󂯎�����l��^����(�m�b�N�o�b�N)
	pos.x -= ((vec + (combo_count_player * 0.1f)) * rightOrleft);
}





//------------------------------------------------------
//				�U���֐�

void Player::AttackDefault(float decision, float elapsed_time)
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

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//�G�t�F�N�g����
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//�v���C���[�̊p�x���ˑ�������ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//�ˑ������Ȃ��ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
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
	//�G�t�F�N�g�X�V
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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
			//�����t���[����������
			timer = non_target;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;

			//�G�t�F�N�g����
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}






//---------------------------------------------------------
//			��ѓ���f�t�H���g�֐�
void Player::AttackProjectileDefault(float decision, float elapsed_time)
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

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}

			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//�v���C���[�̊p�x���ˑ�������ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//�ˑ������Ȃ��ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
			}
		}

		attack_list[now_at_list].SetAttack(&projectile_atk, rightOrleft, pos, attack_list[now_at_list].speed);

		//YRGetEffect().PlayEffect(EffectKind::DRILL, projectile_atk.back().handle, projectile_atk.back().pos.GetDXFLOAT3(), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), -90.0f * rightOrleft);

		//projectile_atk.back().effect_kind = EffectKind::DRILL;

		//SE�Đ�
		GetSound().SESinglePlay(SEKind::PROJECTILE);

		//�����t���[����������
		fream = non_target;

		//�������Ԃ�ݒ�
		timer = attack_list[now_at_list].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;

		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}

	//�G�t�F�N�g�X�V
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));

	//if (projectile_atk.empty())
	//{
	//	//�����U�����܂��o�Ă��Ȃ��Ȃ炱����return���Ď��̍U���Ɉڂ�Ȃ��悤�ɂ���
	//	return;
	//}

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

			//�G�t�F�N�g����
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

			HitBoxTransition(HitBoxState::NOGUARD);
			//�����t���[����������
			timer = non_target;
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}






// -------------------------------------------------------------- -
// **�R���{�Z�b�g�֐�**
//�E���͂��ꂽ�U���ɃR���{�X�e�[�g�������Ă����ꍇ�A
//�@���̃R���{����͂��ꂽ�n�_����J�n��true��Ԃ�
//---------------------------------------------------------------
bool Player::ComboSet()
{
	AttackState real = attack_list[scastI(attack_state)].real_attack;
	AttackState now_atk = attack_list[scastI(attack_state)].attack_name;
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
				if (combolist_X.combolist[i] == now_atk)
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
	case AttackState::COMBO_Y:
	{
		//X�{�^���R���{
		combolist_Y.now_pos = -1;
		if (!combolist_Y.combolist.empty())
		{
			for (int i = 0; i < combolist_Y.combolist.size(); i++)
			{
				//�R���{���X�g�̂ǂ̈ʒu�ɍ��̍U�������邩���m�F����
				if (combolist_Y.combolist[i] == now_atk)
				{
					combolist_Y.now_pos = i;
					break;
				}
			}
		}

		if (combolist_Y.now_pos < 0)
		{
			//���̍U�����R���{�Ɋ܂܂�Ă��Ȃ������ꍇfalse
			return false;
		}
		//�R���{���X�V
		combolist_Y.now_pos++;

		//���̍U����ݒ肷��
		truth = combolist_Y.combolist[combolist_Y.now_pos];
	}
	break;
	case AttackState::COMBO_B:
	{
		//X�{�^���R���{
		combolist_B.now_pos = -1;
		if (!combolist_B.combolist.empty())
		{
			for (int i = 0; i < combolist_B.combolist.size(); i++)
			{
				//�R���{���X�g�̂ǂ̈ʒu�ɍ��̍U�������邩���m�F����
				if (combolist_B.combolist[i] == now_atk)
				{
					combolist_B.now_pos = i;
					break;
				}
			}
		}

		if (combolist_B.now_pos < 0)
		{
			//���̍U�����R���{�Ɋ܂܂�Ă��Ȃ������ꍇfalse
			return false;
		}
		//�R���{���X�V
		combolist_B.now_pos++;

		//���̍U����ݒ肷��
		truth = combolist_B.combolist[combolist_B.now_pos];
	}
	break;
	default:
		return false;
		break;
	}

	int truth_num = scastI(truth);
	int real_num = scastI(attack_list[truth_num].real_attack);

	//�n�㔻����m�F����
	switch (attack_list[real_num].ground_on)
	{
	case Ground_C::AIR:
		if (ground)
		{
			return false;
		}
		break;
	case Ground_C::GROUND:
		if (!ground)
		{
			return false;
		}
		break;
	case Ground_C::EITHER:
		break;
	default:
		return false;
		break;
	}

	if (attack_list[real_num].attack_name == AttackState::TRACK_DASH)
	{
		if (trackgauge < 1)
		{
			return false;
		}
	}

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
	//�����蔻���������
	if (attack_list[real_num].attack_single[0].parameter[0].type == AttackKind::NO_TO_OFFSET_UP)
	{
		HitBoxTransition(HitBoxState::UP_INVINCIBLE);
	}
	else if (attack_list[real_num].attack_single[0].parameter[0].type == AttackKind::NO_TO_OFFSET)
	{
		HitBoxTransition(HitBoxState::INVINCIBLE);
	}
	else
	{
		HitBoxTransition(HitBoxState::NOGUARD);
	}
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
	//������������
	timer = non_target;
	//�J���������p�ϐ���������
	production_time = 0.0f;
	//�p�x�����ɖ߂�
	angle.y = 0.0f;
	angle.z = 0.0f;
	//���̋O�Ղ�\�����邩�ǂ������擾����
	traject_on = attack_list[real_num].traject_on;
	//�`����Z�b�g
	if (rightOrleft > 0)
	{
		anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[real_num].anim_kind)], scastI(AnimAtk::FREAM));
	}
	else
	{
		anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[real_num].anim_kind)], scastI(AnimAtk::FREAM));
	}
	//�U�����ƂɌʂ̐ݒ���s��
	AttackDetailsSet(attack_list[real_num].combo);

	return true;
}







//---------------------------------------------------------------
// **�R���{�X�V�֐�**
//�E�R���{��i�߂Ă���
//---------------------------------------------------------------
void Player::ComboUpdate()
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
	case AttackState::COMBO_Y:
	{
		//X�{�^���R���{
		//�R���{���X�V
		combolist_Y.now_pos++;
		if (combolist_Y.combolist.empty())
		{
			return;
		}
		if (combolist_Y.now_pos >= combolist_Y.combolist.size())
		{
			//�R���{�̍ŏI�n�_�������ꍇ

			return;
		}
		//���̍U����ݒ肷��
		truth = combolist_Y.combolist[combolist_Y.now_pos];
	}
	break;
	case AttackState::COMBO_B:
	{
		//X�{�^���R���{
		//�R���{���X�V
		combolist_B.now_pos++;
		if (combolist_B.combolist.empty())
		{
			return;
		}
		if (combolist_B.now_pos >= combolist_B.combolist.size())
		{
			//�R���{�̍ŏI�n�_�������ꍇ

			return;
		}
		//���̍U����ݒ肷��
		truth = combolist_B.combolist[combolist_B.now_pos];
	}
	break;
	default:
		return;
		break;
	}

	int truth_num = scastI(truth);
	int real_num = scastI(attack_list[truth_num].real_attack);

	//�n�㔻����m�F����
	switch (attack_list[real_num].ground_on)
	{
	case Ground_C::AIR:
		if (ground)
		{
			return;
		}
		break;
	case Ground_C::GROUND:
		if (!ground)
		{
			return;
		}
		break;
	case Ground_C::EITHER:
		break;
	default:
		return;
		break;
	}

	//�U�������肷��
	if (attack_list[real_num].attack_name == AttackState::TRACK_DASH)
	{
		if (trackgauge < 1)
		{
			return;
		}
	}
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
	//�����蔻���������
	if (attack_list[real_num].attack_single[0].parameter[0].type == AttackKind::NO_TO_OFFSET_UP)
	{
		HitBoxTransition(HitBoxState::UP_INVINCIBLE);
	}
	else if (attack_list[real_num].attack_single[0].parameter[0].type == AttackKind::NO_TO_OFFSET)
	{
		HitBoxTransition(HitBoxState::INVINCIBLE);
	}
	else
	{
		HitBoxTransition(HitBoxState::NOGUARD);
	}
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
	//������������
	timer = non_target;
	//�J���������p�ϐ���������
	production_time = 0.0f;
	//�p�x�����ɖ߂�
	angle.y = 0.0f;
	angle.z = 0.0f;
	//���̋O�Ղ�\�����邩�ǂ������擾����
	traject_on = attack_list[real_num].traject_on;
	//�`����Z�b�g
	if (rightOrleft > 0)
	{
		anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[real_num].anim_kind)], scastI(AnimAtk::FREAM));
	}
	else
	{
		anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[real_num].anim_kind)], scastI(AnimAtk::FREAM));
	}
	//�U�����ƂɌʂ̐ݒ���s��
	AttackDetailsSet(attack_list[real_num].combo);
}







void Player::ComboX(float decision, float elapsed_time)
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




void Player::ComboY(float decision, float elapsed_time)
{
	//Y�{�^���R���{�֐�
	if (combolist_Y.now_pos >= combolist_Y.combolist.size())
	{
		return;
	}
	AttackState truth = combolist_Y.combolist[combolist_Y.now_pos];
	int truth_num = scastI(truth);
	attack_state = attack_list[truth_num].real_attack;

	AttackSwitch(decision, elapsed_time);

	attack_state = last_attack;
}




void Player::ComboB(float decision, float elapsed_time)
{
	//B�{�^���R���{�֐�
	if (combolist_B.now_pos >= combolist_B.combolist.size())
	{
		return;
	}
	AttackState truth = combolist_B.combolist[combolist_B.now_pos];
	int truth_num = scastI(truth);
	attack_state = attack_list[truth_num].real_attack;

	AttackSwitch(decision, elapsed_time);

	attack_state = last_attack;
}






//------------------------------------------------------
//				�������Ȃ�

void Player::AttackNone(float decision, float elapsed_time)
{
	//�Ӑ}�I�ɉ������Ȃ�
}






//------------------------------------------------------
//				AttackSwicth�֐��ɓ���

void Player::AttackSwicthIn(float decision, float elapsed_time)
{
	AttackSwitch(decision, elapsed_time, attack_list[scastI(attack_state)].real_attack);
}






//------------------------------------------------------
//				�W�����v���čU��

void Player::AttackJump(float decision, float elapsed_time)
{
	constexpr float up_gravity = 20.0f;
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
		//����������
		pos.y += up_gravity * elapsed_time;
		fream -= elapsed_time;
	}
	//�d�͂̋t����t�^����
	pos.y += gravity * elapsed_time;

	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//�G�t�F�N�g����
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//�v���C���[�̊p�x���ˑ�������ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//�ˑ������Ȃ��ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
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
				a.knock_start = false;
			}
			if (a.knock_start)
			{
				PosKnockPlus(a.parameter.knockback);
				a.parameter.knockback = 0.0f;
				knock = true;
				a.knock_start = false;
				//������ւ̗͂�ݒ肷��
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
	//�G�t�F�N�g�X�V
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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
			//�����t���[����������
			timer = non_target;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;

			//�G�t�F�N�g����
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}









//------------------------------------------------------
//				�󒆂Œ��˂čU��

void Player::AttackAirUp(float decision, float elapsed_time)
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

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//�G�t�F�N�g����
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//�v���C���[�̊p�x���ˑ�������ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//�ˑ������Ȃ��ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
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
	//�G�t�F�N�g�X�V
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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
			//�����t���[����������
			timer = non_target;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;

			//�G�t�F�N�g����
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}









//------------------------------------------------------
//				�͂ݍU��

void Player::AttackSteal(float decision, float elapsed_time)
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

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//�G�t�F�N�g����
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//�v���C���[�̊p�x���ˑ�������ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//�ˑ������Ȃ��ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
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
	//�G�t�F�N�g�X�V
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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
			//�����t���[����������
			timer = non_target;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;

			//�G�t�F�N�g����
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}










//------------------------------------------------------
//				�����U��

void Player::AttackSlow(float decision, float elapsed_time)
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
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
			anim_ccodinate = ac_attack[now_at_list].fream;

			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::FREAM));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::FREAM));
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

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//�G�t�F�N�g����
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//�v���C���[�̊p�x���ˑ�������ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//�ˑ������Ȃ��ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
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
	//�G�t�F�N�g�X�V
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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
			//�����t���[����������
			timer = non_target;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;

			//�G�t�F�N�g����
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}









//------------------------------------------------------
//				���ړ����Ȃ���U��

void Player::AttackSlid(float decision, float elapsed_time)
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

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//�G�t�F�N�g����
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//�v���C���[�̊p�x���ˑ�������ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//�ˑ������Ȃ��ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}



	bool knock = false;	//��x�ł�knock_start�ɓ�������c��̓����蔻���knockback��S��0.0f�ɂ���
	if (!atk.empty())
	{
		//�U�����莞�O�i�����ĉ�]������
		speed.x = attack_list[now_at_list].advance_speed * rightOrleft;
		//angle.y += elapsed_time * (50.0f * rightOrleft);

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
	//�G�t�F�N�g�X�V
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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
			//�����t���[����������
			timer = non_target;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;

			//�G�t�F�N�g����
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

			//�㌄��ݒ�
			later = attack_list[now_at_list].later;

			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;

			//�p�x��߂�
			angle.y = 0.0f;
			//���x��߂�
			speed.x = 0.0f;

			HitBoxTransition(HitBoxState::NOGUARD);
			//�����t���[����������
			timer = non_target;
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}












//------------------------------------------------------
//				���ړ����Ȃ����]�U��

void Player::AttackSlidRoll(float decision, float elapsed_time)
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

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//�G�t�F�N�g����
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//�v���C���[�̊p�x���ˑ�������ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//�ˑ������Ȃ��ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
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
				//������ւ̗͂�ݒ肷��
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
	//�G�t�F�N�g�X�V
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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
			//�����t���[����������
			timer = non_target;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;

			//�G�t�F�N�g����
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

			//�㌄��ݒ�
			later = attack_list[now_at_list].later;

			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;

			//�p�x��߂�
			angle.y = 0.0f;
			//���x��߂�
			speed.x = 0.0f;

			HitBoxTransition(HitBoxState::NOGUARD);
			//�����t���[����������
			timer = non_target;
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}










//------------------------------------------------------
//				���ړ����Ȃ���󒆉�]�U��

void Player::AttackAirSlidRoll(float decision, float elapsed_time)
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

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//�G�t�F�N�g����
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//�v���C���[�̊p�x���ˑ�������ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//�ˑ������Ȃ��ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
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
				//������ւ̗͂�ݒ肷��
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
	//�G�t�F�N�g�X�V
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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
			//�����t���[����������
			timer = non_target;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;

			//�G�t�F�N�g����
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

			//�㌄��ݒ�
			later = attack_list[now_at_list].later;

			//�p�x��߂�
			angle.y = 0.0f;
			//���x��߂�
			speed.x = 0.0f;


			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;
			HitBoxTransition(HitBoxState::NOGUARD);
			//�����t���[����������
			timer = non_target;
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}










//------------------------------------------------------
//				���ړ����Ȃ����]�U�����Ė߂�

void Player::AttackSlidRollTurn(float decision, float elapsed_time)
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

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//�G�t�F�N�g����
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//�v���C���[�̊p�x���ˑ�������ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//�ˑ������Ȃ��ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
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
			speed.x = -(attack_list[now_at_list].advance_speed * decision);
			angle.y -= elapsed_time * (50.0f * rightOrleft);
		}
		else
		{
			//�U�����莞�O�i�����ĉ�]������
			speed.x = attack_list[now_at_list].advance_speed * rightOrleft;
			angle.y += elapsed_time * (50.0f * rightOrleft);
		}
	}

	//if (atk.empty())
	//{
	//	//�����U�����܂��o�Ă��Ȃ��Ȃ炱����return���Ď��̍U���Ɉڂ�Ȃ��悤�ɂ���
	//	return;
	//}
	//�G�t�F�N�g�X�V
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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
			//�����t���[����������
			//timer = non_target;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;

			//�G�t�F�N�g����
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

			//�㌄��ݒ�
			later = attack_list[now_at_list].later;

			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;

			//�p�x��߂�
			angle.y = 0.0f;
			//���x��߂�
			speed.x = -attack_list[now_at_list].advance_speed * decision;

			HitBoxTransition(HitBoxState::NOGUARD);

			//�����t���[����������
			timer = non_target;
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}








//------------------------------------------------------
//				�z�[�~���O�A�^�b�N�U��

void Player::AttackTrack(float decision, float elapsed_time)
{
	if (attack_state != AttackState::TRACK_DASH)
	{
		return;
	}

	rightOrleft = decision;

	//���K�����ꂽ����Ɍ������x�N�g��
	YR_Vector3	plusVec = { 0.0f,0.0f,0.0f };
	plusVec = tracking.Veccalculate(hit[1].center, decision);//�����蔻���1�̗v�f�͕K���̂̔���ɐݒ肵�Ă���

	float adjust_angle = 1.0f;

	//����Ɍ��������߂̊p�x�����߂�(����)
	if (pos.y < tracking.rival_Pos.y)
	{
		adjust_angle = -1.0f;
	}

	YR_Vector3	a_vec = YR_Vector3(pos.x, pos.y) - YR_Vector3(tracking.rival_Pos.x, tracking.rival_Pos.y);
	YR_Vector3	b_vec = YR_Vector3(pos.x, tracking.rival_Pos.y) - YR_Vector3(tracking.rival_Pos.x, tracking.rival_Pos.y);

	float upper = (a_vec.x * b_vec.x) + (a_vec.y * b_vec.y);

	float dowwnn = sqrtf((a_vec.x * a_vec.x) + (a_vec.y * a_vec.y)) * sqrtf((b_vec.x * b_vec.x) + (b_vec.y * b_vec.y));

	float dot_angle = acosf(upper / dowwnn);

	//�㌄���ݒ肳�ꂽ��͂��̊֐��ɂ͓���Ȃ�
	if (later > -1 && later < target_max)
	{
		return;
	}

	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	int now_at_num = attack_list[now_at_list].now_attack_num;

	jumpflag = false;

	//�d�͂̋t����t�^����
	pos.y += gravity * elapsed_time;

	//�X�s�[�h�����ׂ�0�ɂ���
	speed.x = 0.0f;
	speed.y = 0.0f;

	//�����t���[���ɂȂ�܂ŉ�
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;

		//������Ƃ��������Ă���
		pos.y += 5.0f * elapsed_time;

		//�����܂ŏ������p�x��ς��Ă���
		DirectX::XMVECTOR now_angle_v = DirectX::XMLoadFloat(&angle.z);
		DirectX::XMVECTOR should_angle_v = DirectX::XMLoadFloat(&dot_angle);

		DirectX::XMVECTOR lerp_v = DirectX::XMVectorLerp(now_angle_v, should_angle_v, 0.1f);

		float lerp = 0.0f;

		DirectX::XMStoreFloat(&lerp, lerp_v);

		angle.z = lerp * Getapply(-1.0f) * adjust_angle;
	}
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//�G�t�F�N�g����
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//�v���C���[�̊p�x���ˑ�������ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//�ˑ������Ȃ��ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;

		//�A�[�}�[��t�^
		HitBoxTransition(HitBoxState::PROJECTILE_ARMOR);
		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}



	bool knock = false;	//��x�ł�knock_start�ɓ�������c��̓����蔻���knockback��S��0.0f�ɂ���
	if (!atk.empty())
	{

		//��ɑ���̕����Ɍ�����
		angle.z = dot_angle * Getapply(-1.0f) * adjust_angle;

		pos.x += ((plusVec.x * chara_state.track_speed) * elapsed_time);
		pos.y += ((plusVec.y * chara_state.track_speed) * elapsed_time);

		for (auto& a : atk)
		{
			if (rightOrleft < 0)
			{
				YRGetEffect().SetAngle(a.handle, DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), 3.0f + angle.z);
			}
			else
			{
				YRGetEffect().SetAngle(a.handle, DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f), angle.z);
			}

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

	//if (atk.empty())
	//{
	//	//�����U�����܂��o�Ă��Ȃ��Ȃ炱����return���Ď��̍U���Ɉڂ�Ȃ��悤�ɂ���
	//	return;
	//}
	//�G�t�F�N�g�X�V
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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
			//�����t���[����������
			timer = non_target;
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

			//�G�t�F�N�g����
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
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












//------------------------------------------------------
//				��]�z�[�~���O�_�b�V���U��

void Player::AttackTrackRoll(float decision, float elapsed_time)
{
	//�㌄���ݒ肳�ꂽ��͂��̊֐��ɂ͓���Ȃ�
	if (later > -1 && later < target_max)
	{
		return;
	}

	rightOrleft = decision;

	//���K�����ꂽ����Ɍ������x�N�g��
	YR_Vector3	plusVec = { 0.0f,0.0f,0.0f };
	plusVec = tracking.Veccalculate(hit[1].center, decision);//�����蔻���1�̗v�f�͕K���̂̔���ɐݒ肵�Ă���

	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	int now_at_num = attack_list[now_at_list].now_attack_num;

	jumpflag = false;

	//�d�͂̋t����t�^����
	pos.y += gravity * elapsed_time;

	//�X�s�[�h�����ׂ�0�ɂ���
	speed.x = 0.0f;
	speed.y = 0.0f;

	//�����t���[���ɂȂ�܂ŉ�
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;
		//������Ƃ��������Ă���
		pos.y += 5.0f * elapsed_time;
	}

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//�G�t�F�N�g����
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//�v���C���[�̊p�x���ˑ�������ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//�ˑ������Ȃ��ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;

		//��ѓ���A�[�}�[��t�^
		HitBoxTransition(HitBoxState::PROJECTILE_ARMOR);
		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}



	bool knock = false;	//��x�ł�knock_start�ɓ�������c��̓����蔻���knockback��S��0.0f�ɂ���
	if (!atk.empty())
	{
		for (auto& a : atk)
		{

			//��ɉ�]������
			angle.z -= (50.0f * rightOrleft) * elapsed_time;

			pos.x += ((plusVec.x * chara_state.track_speed) * elapsed_time);
			pos.y += ((plusVec.y * chara_state.track_speed) * elapsed_time);

			GetSound().SEPlay(SEKind::TRACK);

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

	//if (atk.empty())
	//{
	//	//�����U�����܂��o�Ă��Ȃ��Ȃ炱����return���Ď��̍U���Ɉڂ�Ȃ��悤�ɂ���
	//	return;
	//}
	//�G�t�F�N�g�X�V
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


	//�������Ԃ��S�ďI���������Ƃ��m�F����
	if (timer < 0.0f && timer < target_max)
	{
		////�U�����S�ďI���������Ƃ��m�F����
		//if (AttackEndCheck())
		//{
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

			speed.y = 0.0f;
			if (plusVec.y > 0.5f)
			{
				//��Ɍ������Ĕ��ł���ꍇ�͑��x������
				speed.y = attack_list[now_at_list].advance_speed;
			}

			//�G�t�F�N�g����
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
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










//------------------------------------------------------
//				�W�����v���Ė��G�U��

void Player::AttackJumpEx(float decision, float elapsed_time)
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

		//�U�������܂Ŗ��G
		HitBoxTransition(HitBoxState::INVINCIBLE);

		//�d�͂̋t����t�^����
		if (!ground)
		{
			pos.y += gravity * elapsed_time;
			speed_Y.Set(0.0f);
			speed.y = 0.0f;
			speed.x = 0.0f;
		}
	}

	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);

	int now_at_num = attack_list[now_at_list].now_attack_num;
	//�����t���[���ɂȂ�����U������𐶐�����
	if (fream < 0.0f)
	{
		//�U�������������疳�G����
		HitBoxTransition(HitBoxState::NOGUARD);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//�G�t�F�N�g����
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//�v���C���[�̊p�x���ˑ�������ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//�ˑ������Ȃ��ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		speed.y = attack_list[now_at_list].advance_speed;

		//�W�����v��Ԃɂ���
		jumpflag = true;
		if (!ground)max_jump_flag = false;

		now_at_num = attack_list[now_at_list].now_attack_num;
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
			}
		}
	}

	if (timer > 0.0f && timer < target_max)
	{
		//�����t���[�������炵�Ă���
		timer -= elapsed_time;

		//�d�͂̋t����t�^����
		//pos.y += gravity * elapsed_time;

		//pos.y += attack_list[now_at_list].advance_speed * elapsed_time;
	}

	//if (atk.empty())
	//{
	//	//�����U�����܂��o�Ă��Ȃ��Ȃ炱����return���Ď��̍U���Ɉڂ�Ȃ��悤�ɂ���
	//	return;
	//}
	//�G�t�F�N�g�X�V
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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
			//�����t���[����������
			timer = non_target;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;

			//�G�t�F�N�g����
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;

			//�W�����v��Ԃɂ���
			jumpflag = true;
			max_jump_flag = true;
		}
	}
}







//------------------------------------------------------
//				�O���K�E�U��

void Player::AttackSpecial_INV(float decision, float elapsed_time)
{
	//�O�i���Ȃ��悤�ɂ���
	speed_X.Set(0.0f);

	//�㌄���ݒ肳�ꂽ��͂��̊֐��ɂ͓���Ȃ�
	if (later > -1 && later < target_max)
	{
		return;
	}

	//�C�x���g�V�[�����Đ�����
	if (!special_event.CameraEventUpdate(pos, rightOrleft, elapsed_time, this))
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

	int now_at_num = attack_list[now_at_list].now_attack_num;
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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//�G�t�F�N�g����
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//�v���C���[�̊p�x���ˑ�������ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//�ˑ������Ȃ��ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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

		//�U���������͖��G
		HitBoxTransition(HitBoxState::INVINCIBLE);

		//�������Ԃ�ݒ�
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;
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

	if (timer > 0.0f && timer < target_max)
	{
		//�����t���[�������炵�Ă���
		timer -= elapsed_time;
		if (hit_result != HitResult::GUARD)
		{
			//�U�����͑O�Ɉړ�������
			pos.x += elapsed_time * Getapply(attack_list[now_at_list].advance_speed);
		}
		else
		{
			//���G������
			HitBoxTransition(HitBoxState::NOGUARD);
		}
	}

	//if (atk.empty())
	//{
	//	//�����U�����܂��o�Ă��Ȃ��Ȃ炱����return���Ď��̍U���Ɉڂ�Ȃ��悤�ɂ���
	//	return;
	//}
	//�G�t�F�N�g�X�V
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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
			//�����t���[����������
			timer = non_target;
		}
		else
		{
			//�u���[����؂�
			lumi_material = Model::Material_Attribute::NONE;
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;

			//�G�t�F�N�g����
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

			//�㌄��ݒ�
			later = attack_list[now_at_list].later;
			//�A�j���[�V�������x���w��
			anim_ccodinate = ac_attack[now_at_list].later;
			HitBoxTransition(HitBoxState::NOGUARD);
			//�����t���[����������
			timer = non_target;
			//���G������
			HitBoxTransition(HitBoxState::NOGUARD);
			//�`����Z�b�g
			if (rightOrleft > 0)
			{
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;
		}
	}
}


void Player::SpecialAttack(float elpsed_time)
{
	//�I�[�o�[���C�h
}









//------------------------------------------------------
//				�o�[�X�g�U��

void Player::AttackBurst(float decision, float elapsed_time)
{
	//�㌄���ݒ肳�ꂽ��͂��̊֐��ɂ͓���Ȃ�
	if (later > -1 && later < target_max)
	{
		return;
	}


	//�d�͂̋t����n��
	if(!ground)pos.y += gravity * elapsed_time;
	//���x��S��0�ɂ���
	speed.x = 0.0f;
	speed_X.Set(0.0f);

	int now_at_list = scastI(attack_list[scastI(attack_state)].real_attack);
	int now_at_num = attack_list[now_at_list].now_attack_num;

	//�����t���[���ɂȂ�܂ŉ�
	if (fream < target_max)
	{
		//speed_Y.Set(0.0f);
		//�U�������̌��ʂ�ۑ�����
		hit_result = HitResult::NOT_OCCURRENCE;
		fream -= elapsed_time;

		//������ɒ��˂�悤��
		speed.y = attack_list[now_at_list].advance_speed;

		//�����蔻����o�[�X�g���G�ɂ���
		HitBoxTransition(HitBoxState::BURST_INVINCIBLE);
		KnockReset();
	}

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::TIMER));
			}

			//�G�t�F�N�g����
			if (attack_list[now_at_list].effect_param.effect_kind != EffectKind::NONE)
			{
				if (attack_list[now_at_list].effect_param.rightORleft)
				{
					//�v���C���[�̊p�x���ˑ�������ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle * rightOrleft);
				}
				else
				{
					//�ˑ������Ȃ��ꍇ
					YRGetEffect().PlayEffect(
						attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle,
						DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z),
						attack_list[now_at_list].effect_param.scale.GetDXFLOAT3(), attack_list[now_at_list].effect_param.axis.GetDXFLOAT3(), attack_list[now_at_list].effect_param.angle);
				}
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
		timer = attack_list[now_at_list].attack_single[now_at_num].parameter[0].timer;

		now_at_num = attack_list[now_at_list].now_attack_num;

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
			}
		}
	}

	if (timer > 0.0f && timer < target_max)
	{
		//�����t���[�������炵�Ă���
		timer -= elapsed_time;

		//���x��S��0�ɂ���
		speed.x = 0.0f;
		speed_X.Set(0.0f);
		speed.y = 0.0f;
		speed_Y.Set(0.0f);
	}

	//if (atk.empty())
	//{
	//	//�����U�����܂��o�Ă��Ȃ��Ȃ炱����return���Ď��̍U���Ɉڂ�Ȃ��悤�ɂ���
	//	return;
	//}
	//�G�t�F�N�g�X�V
	YRGetEffect().SetLocation(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle, DirectX::XMFLOAT3(pos.x + Getapply(attack_list[now_at_list].effect_param.distance.x), pos.y + attack_list[now_at_list].effect_param.distance.y, pos.z + attack_list[now_at_list].effect_param.distance.z));


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
			//�����t���[����������
			timer = non_target;
		}
		else
		{
			//�Ȃ��ꍇ�͌㌄�Ɉڍs����
			//�U���ԍ���������
			attack_list[now_at_list].now_attack_num = 0;

			//�G�t�F�N�g����
			YRGetEffect().StopEffect(attack_list[now_at_list].effect_param.effect_kind, attack_list[now_at_list].handle);

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
				anim->NodeChange(model_motion.model_R_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			else
			{
				anim->NodeChange(model_motion.model_L_Attack[scastI(attack_list[scastI(attack_state)].anim_kind)], scastI(AnimAtk::LATER));
			}
			//�s���I���t���O���I����
			finish = true;

			//�W�����v��Ԃɂ���
			jumpflag = true;
			if (jumpcount > 1)jumpcount--;
			max_jump_flag = true;

			//���x��S��0�ɂ���
			speed.x = 0.0f;
			speed.y = 0.0f;
			speed_X.Set(0.0f);
			speed_Y.Set(0.0f);

			ChangeFace(FaceAnim::NORMAL);

			//�����蔻���߂�
			HitBoxTransition(HitBoxState::NOGUARD);
		}
	}
}











//------------------------------
//		�͂ݍU������
bool Player::StealRangeCheck()
{
	//�ŏI�I�ɂ͒͂ݍU�����ɑ���Ɍ������Ď����I�Ƀ_�b�V������悤�ɂ�����
	return true;
	if (pos.x > tracking.rival_Pos.x)
	{
		//�v���C���[���E�ɂ���ꍇ

		//�݂͂��o�����ꍇ�̍U���o���ʒu
		float atk_dis = pos.x - attack_list[scastI(AttackState::STEAL)].attack_single[0].parameter[0].distance.x;

		//�U�����o�����Ƃ��ɑ���ɓ�����ʒu�Ȃ�
		if (tracking.rival_Pos.x >= atk_dis)
		{
			return true;
		}
		else
		{
			//������Ȃ��Ȃ瑖�点��
		}
	}
	else
	{
		//�v���C���[�����ɂ���ꍇ
		float dis = tracking.rival_Pos.x - pos.x;

		//�݂͂��o�����ꍇ�̍U���o���ʒu
		float atk_dis = pos.x + attack_list[scastI(AttackState::STEAL)].attack_single[0].parameter[0].distance.x;

		//�U�����o�����Ƃ��ɑ���ɓ�����ʒu�Ȃ�
		if (tracking.rival_Pos.x <= atk_dis)
		{
			return true;
		}
		else
		{
			//������Ȃ��Ȃ瑖�点��
		}
	}
}


//
//--------------------------------------------------------------------------------------------





//void Player::A_Jaku_Rhurf(float elapsed_time)
//{
//	//�g�p����ꍇ�̓I�[�o�[���C�h
//}
//
//void Player::A_Thu_Rhurf(float elapsed_time)
//{
//	//�g�p����ꍇ�̓I�[�o�[���C�h
//}
//
//void Player::A_Kyo_Rhurf(float elapsed_time)
//{
//	//�g�p����ꍇ�̓I�[�o�[���C�h
//}
//
//
//void Player::A_Jaku_Lhurf(float elapsed_time)
//{
//	//�g�p����ꍇ�̓I�[�o�[���C�h
//}
//
//void Player::A_Thu_Lhurf(float elapsed_time)
//{
//	//�g�p����ꍇ�̓I�[�o�[���C�h
//}
//
//void Player::A_Kyo_Lhurf(float elapsed_time)
//{
//	//�g�p����ꍇ�̓I�[�o�[���C�h
//}