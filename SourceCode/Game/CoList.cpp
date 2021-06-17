#include	"CoList.h"
#include	"YRGamePad.h"


Command CommandList::CommandCheack(int last, int* hist, float distans)
{
	//--------------------------------------------------------------
	//=========================================
	//		�R�}���h�m�F�֐�
	//=========================================
	//�E���̗͂������ŐV�̂��̂��炳���̂ڂ��Ă����A
	//  �ߋ��ɃR�}���h�����͂���Ă��邩�����𔻒肷��
	//---------------------------------------------------------------

	trigger = false;
	state = Command::NOCOMMAND;

	//List�̉�������������ۑ�����
	numI = 0;

	//�R�}���h����^�C�}�[�����̒l�ȓ��Ȃ�R�}���h��������悤�ɂ���(�R�}���h��t�P�\)
	if (command_timer > TIMER_LIMIT || command_timer <= 0.0f)
	{
		trigger = true;
		return Command::NOCOMMAND;
	}

	//command_timer = 0;
	for (int i = 0; i < 10; i++)
	{
		histry[i] = hist[i];
	}
	//�����ōŌ�ɓ��͂��ꂽ�{�^����������Ă���(�U���p�{�^����X�AY�AB�ARB�{�^���Ȃ�)
	lastinput = last;

	//List�̒��g����납�猩�Ă���
	for (int i = 9; i > -1; i--)
	{
		if (histry[i] != -1)
		{
			//�Ō�ɓ��͂��ꂽ���̂ƈႤ���̂ł����
			if (histry[i] != lastinput)
			{
				if (histry[i] == scastI(PAD::STICK_R))
				{
					//���͂��ꂽ�̂��E������
					state = Command::RHURF;
					numI = i;
					break;
				}
				else if (histry[i] == scastI(PAD::STICK_L))
				{
					//���͂��ꂽ�̂���������
					state = Command::LHURF;
					numI = i;
					break;
				}
				else
				{
					//�R�}���h�͖�������
					return Command::NOCOMMAND;
				}
			}
		}
	}

	if (numI < 2)
	{
		//�c���Ă���̂�2�ȉ��Ȃ�R�}���h�͐�΂ɐ������Ȃ��̂ł����ŋ�؂�
		return Command::NOCOMMAND;
	}

	//List�̒��g�𔻒肵���ꏊ���猩�Ă���
	//���R�}���h�͐��������͂���Ă����List�̒��Łu���v�u�����v�u���v�ƕ���ł���͂��Ȃ̂�
	//��������Q�Ƃ��Ă���
	switch (state)
	{
	case Command::RHURF:
		//�Ō�ɓ��͂��ꂽ�͉̂E������
		if (histry[numI - 1] == scastI(PAD::STICK_RDown))
		{
			//���̑O�ɓ��͂��ꂽ�̂��E��������
			if (histry[numI - 2] == scastI(PAD::STICK_D))
			{
				//�X�ɂ��̑O�ɓ��͂��ꂽ�̂���������
				if (distans > 0)
				{
					//�v���C���[�͉E�������Ă���
					trigger = true;
					return Command::RHURF;
				}
				if (distans < 0)
				{
					//�v���C���[�͍��������Ă���
					trigger = true;
					return Command::LHURF;
				}
			}
		}
		else
		{
			//�R�}���h�͂Ȃ�
			return Command::NOCOMMAND;
		}
		break;
	case Command::LHURF:
		//�Ō�ɓ��͂��ꂽ�͍̂�������
		if (histry[numI - 1] == scastI(PAD::STICK_LDown))
		{
			//���̑O�ɓ��͂��ꂽ�̂�����������
			if (histry[numI - 2] == scastI(PAD::STICK_D))
			{
				//�X�ɂ��̑O�ɓ��͂��ꂽ�̂���������
				if (distans < 0)
				{
					//�v���C���[�͉E�������Ă���
					trigger = true;
					return Command::RHURF;
				}
				if (distans > 0)
				{
					//�v���C���[�͍��������Ă���
					trigger = true;
					return Command::LHURF;
				}
			}
		}
		else
		{
			//�R�}���h�͂Ȃ�
			return Command::NOCOMMAND;
		}
		break;
	default:
		break;
	}
	trigger = true;
	//�R�}���h�͂Ȃ�
	return Command::NOCOMMAND;
}

void CommandList::Reset()
{
	command_timer = 0.0f;
}