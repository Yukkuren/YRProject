#include "PlayerBase.h"
#include "../../Effect.h"
#include "../../YRSound.h"

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
	pos.x -= vec * (rightOrleft + (combo_count_player * 0.1f));
}





//------------------------------------------------------
//				�U���֐�

void Player::AttackDefault(float elapsed_time)
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






//---------------------------------------------------------
//			��ѓ���f�t�H���g�֐�
void Player::AttackProjectileDefault(float elapsed_time)
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

		//YRGetEffect().PlayEffect(EffectKind::DRILL, projectile_atk.back().handle, projectile_atk.back().pos.GetDXFLOAT3(), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f), -90.0f * rightOrleft);

		//projectile_atk.back().effect_kind = EffectKind::DRILL;

		//SE�Đ�
		GetSound().SESinglePlay(SEKind::PROJECTILE);

		//�����t���[����������
		fream = non_target;

		//�������Ԃ�ݒ�
		timer = attack_list[now_at_list].timer;

		//anim->NodeChange(model_motion.model_R[now_at_list], scastI(AnimAtk::TIMER));
	}


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
		anim->NodeChange(model_motion.model_R_Attack[real_num], scastI(AnimAtk::FREAM));
	}
	else
	{
		anim->NodeChange(model_motion.model_L_Attack[real_num], scastI(AnimAtk::FREAM));
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
		anim->NodeChange(model_motion.model_R_Attack[real_num], scastI(AnimAtk::FREAM));
	}
	else
	{
		anim->NodeChange(model_motion.model_L_Attack[real_num], scastI(AnimAtk::FREAM));
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





void Player::A_Jaku_Rhurf(float elapsed_time)
{
	//�g�p����ꍇ�̓I�[�o�[���C�h
}

void Player::A_Thu_Rhurf(float elapsed_time)
{
	//�g�p����ꍇ�̓I�[�o�[���C�h
}

void Player::A_Kyo_Rhurf(float elapsed_time)
{
	//�g�p����ꍇ�̓I�[�o�[���C�h
}


void Player::A_Jaku_Lhurf(float elapsed_time)
{
	//�g�p����ꍇ�̓I�[�o�[���C�h
}

void Player::A_Thu_Lhurf(float elapsed_time)
{
	//�g�p����ꍇ�̓I�[�o�[���C�h
}

void Player::A_Kyo_Lhurf(float elapsed_time)
{
	//�g�p����ꍇ�̓I�[�o�[���C�h
}