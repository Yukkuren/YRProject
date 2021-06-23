#include "../Scene/Scene.h"

//-----------------------------------------------------------------
//			AI(�X�e�[�g�x�[�X)
//-----------------------------------------------------------------
//�E�ȈՓI��AI�̂��߁A�R���g���[���[�̓��͂�����I�ɂ����ăL�����𓮂����Ă���
//-----------------------------------------------------------------

void SceneGame::AIControll(float elapsed_time)
{

	float dis = player2p->pos.Distance(player1p->pos);

	if (player2p->knocktimer > 0.0f&& player2p->knocktimer<target_max)
	{
		//�v���C���[���_���[�W���󂯂�
		AI2P.state = AI_Controller::AI_State::KNOCK;
		player2p->pad->Init();
	}
	if (player2p->hp == 0.0f)
	{
		AI2P.state = AI_Controller::AI_State::DEAPTH;
	}
	if (player2p->act_state == ActState::DOWN)
	{
		AI2P.state = AI_Controller::AI_State::DOWN;
		player2p->pad->Init();
	}

	/*if (player1p->act_state == ActState::SLAM)
	{
		player2p->pad->Init();
		AI2P.state = AI_Controller::AI_State::SPECIAL;
	}*/

	if (player2p->steal_escape > 0.0f)
	{
		if (!AI2P.steal_escape)
		{
			int ra = rand() % 10;
			if (ra < 5)
			{
				player2p->pad->x_input[scastI(PAD::RB)] = 1;
			}
			AI2P.steal_escape = true;
		}
	}

	switch (AI2P.state)
	{
	case AI_Controller::AI_State::INIT:
		//�ŏ��̐ݒ�
		player2p->pad->Init();
		AI2P.state = AI_Controller::AI_State::RAND_SELECT;
		AI2P.timer = 0.0f;
		AI2P.steal_escape = false;
		break;
	case AI_Controller::AI_State::RAND_SELECT:
	{
		//�����_���ōs����ݒ肷��

		if (!player2p->ground)
		{
			//�󒆂Ȃ牽�����Ȃ�
			return;
		}

		/*AI2P.state = AI_Controller::AI_State::FOREVER_GUARD;
		return;*/

		if (dis < AI2P.max_dis)
		{
			//�v���C���[���m�̋��������ȉ��Ȃ�U��������邩�K�[�h����
			while (AI2P.state == AI2P.before_state)
			{
				int ra = rand() % 10;

				if (ra < AI2P.attack_probability)
				{
					//X�{�^���A��
					AI2P.state = AI_Controller::AI_State::COMBO;
				}
				else if (ra < AI2P.guard_probability)
				{
					//�K�[�h����
					AI2P.state = AI_Controller::AI_State::GUARD;
				}
				else
				{
					//�����
					AI2P.state = AI_Controller::AI_State::LEAVE;
				}
			}
		}
		else
		{
			while (AI2P.state == AI2P.before_state)
			{
				//���ȉ��Ȃ�߂Â����̓z�[�~���O�_�b�V�����s��
				int ra = rand() % 10;

				if (ra < AI2P.track_probability)
				{
					//�z�[�~���O�_�b�V��
					AI2P.state = AI_Controller::AI_State::TRACK_DASH;
				}
				else
				{
					//�߂Â�
					AI2P.state = AI_Controller::AI_State::APPROACH;
				}
			}
		}
		AI2P.before_state = AI2P.state;
	}
		break;
	case AI_Controller::AI_State::APPROACH:
		//�߂Â�

		AI2P.timer += elapsed_time;

		if (player2p->rightOrleft > 0)
		{
			//�E����
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_R)]++;
		}
		else
		{
			//������
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_L)]++;
		}

		if (dis < AI2P.max_dis)
		{
			//�v���C���[���m�̋��������ȉ��ɂȂ����珉���X�e�[�g�ɖ߂�
			AI2P.state = AI_Controller::AI_State::INIT;
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
		}
		if (AI2P.timer > AI2P.timer_max)
		{
			//��莞�ԂɂȂ�����s����������
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
			AI2P.state = AI_Controller::AI_State::INIT;
		}
		break;
	case AI_Controller::AI_State::LEAVE:
		//�����

		AI2P.timer += elapsed_time;

		if (player2p->rightOrleft > 0)
		{
			//�E����
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 1;
			player2p->pad->x_input[scastI(PAD::L_DASH)] = 1;
		}
		else
		{
			//������
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 1;
			player2p->pad->x_input[scastI(PAD::R_DASH)] = 1;
		}

		if (AI2P.timer > AI2P.timer_max)
		{
			//��莞�ԂɂȂ�����s����������
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
			player2p->pad->x_input[scastI(PAD::R_DASH)] = 0;
			player2p->pad->x_input[scastI(PAD::L_DASH)] = 0;
			AI2P.timer = 0.0f;
			if (player2p->power > 0)
			{
				//�N���X�^��������Β��K
				AI2P.state = AI_Controller::AI_State::SPECIAL;
			}
			else
			{
				//�Ȃ���Βʏ�̃X�e�[�g��
				AI2P.state = AI_Controller::AI_State::INIT;
			}
		}
		break;
	case AI_Controller::AI_State::COMBO:
		//X�{�^����A�ł���
		AI2P.timer += elapsed_time;

		player2p->pad->x_input[scastI(PAD::X)] = 1;

		if (player2p->attack_state == AttackState::JAKU_KYO)
		{
			if (player2p->later < target_max)
			{
				//X�{�^���R���{����U��������̓K�[�h���ꂽ��I��
				player2p->pad->x_input[scastI(PAD::X)] = 0;
				AI2P.state = AI_Controller::AI_State::INIT;
			}
		}
		if (player2p->attack_state == AttackState::A_KYO)
		{
			if (player2p->later < target_max)
			{
				//X�{�^���R���{����U��������̓K�[�h���ꂽ��I��
				player2p->pad->x_input[scastI(PAD::X)] = 0;
				AI2P.state = AI_Controller::AI_State::INIT;
			}
		}

		if (AI2P.timer > AI2P.timer_max)
		{
			//��莞�ԂɂȂ�����s����������
			player2p->pad->x_input[scastI(PAD::X)] = 0;
			AI2P.state = AI_Controller::AI_State::INIT;
		}
		break;
	case AI_Controller::AI_State::TRACK_DASH:
		//�z�[�~���O�_�b�V��������
		AI2P.timer += elapsed_time;

		player2p->pad->x_input[scastI(PAD::R_TRIGGER)] = 1;

		if (player2p->hit_result == HitResult::HIT)
		{
			//�������Ă���X�{�^���A�ł�����
			player2p->pad->x_input[scastI(PAD::R_TRIGGER)] = 0;
			AI2P.state = AI_Controller::AI_State::COMBO;
			AI2P.timer = 0.0f;
		}

		if (AI2P.timer > AI2P.timer_max)
		{
			//��莞�ԂɂȂ�����s����������
			player2p->pad->x_input[scastI(PAD::R_TRIGGER)] = 0;
			AI2P.state = AI_Controller::AI_State::INIT;
		}
		break;
	case AI_Controller::AI_State::GUARD:
		//�K�[�h���悤�Ƃ���
		AI2P.timer += elapsed_time;

		if (player2p->rightOrleft > 0)
		{
			//�E����
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_RDown)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_LDown)]++;
		}
		else
		{
			//������
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_LDown)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_RDown)]++;
		}

		if (AI2P.timer > AI2P.guard_timer_max)
		{
			//��莞�ԂɂȂ�����s����������
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_RDown)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_LDown)] = 0;
			AI2P.state = AI_Controller::AI_State::INIT;
		}
		break;
	case AI_Controller::AI_State::KNOCK:
		if (player2p->knocktimer == non_target)
		{
			player2p->pad->x_input[scastI(PAD::X)] = 1;
			AI2P.state = AI_Controller::AI_State::INIT;
		}
		break;
	case AI_Controller::AI_State::DEAPTH:
		player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
		player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
		player2p->pad->x_input[scastI(PAD::X)] = 0;
		player2p->pad->x_input[scastI(PAD::R_TRIGGER)] = 0;
		player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
		player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
		player2p->pad->x_input[scastI(PAD::STICK_RDown)] = 0;
		player2p->pad->x_input[scastI(PAD::STICK_LDown)] = 0;
		player2p->pad->x_input[scastI(PAD::STICK_U)] = 0;
		break;
	case AI_Controller::AI_State::DOWN:
		player2p->pad->x_input[scastI(PAD::STICK_U)] = 1;
		AI2P.state = AI_Controller::AI_State::INIT;
		break;
	case AI_Controller::AI_State::SPECIAL:
		AI2P.timer += elapsed_time;
		player2p->pad->x_input[scastI(PAD::A)] = 1;
		if (AI2P.timer > 0.5f)
		{
			AI2P.state = AI_Controller::AI_State::INIT;
		}
		break;
	case AI_Controller::AI_State::FOREVER_GUARD:
		if (player2p->rightOrleft > 0)
		{
			//�E����
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 1;
			//player2p->pad->x_input[scastI(PAD::L_DASH)] = 1;
		}
		else
		{
			//������
			player2p->pad->x_input[scastI(PAD::STICK_L)] = 0;
			player2p->pad->x_input[scastI(PAD::STICK_R)] = 1;
			//player2p->pad->x_input[scastI(PAD::R_DASH)] = 1;
		}
		break;
	default:
		break;
	}
}