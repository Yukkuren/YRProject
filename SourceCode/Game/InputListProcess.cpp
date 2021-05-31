#include "../Scene/Scene.h"
#include "../framework.h"

void InputGamePadDraw::Init()
{
	//������
	p1_input_list.clear();
	p2_input_list.clear();
}


void InputGamePadDraw::Load()
{
	//�X�v���C�g�ǂݍ���
	if (button_img == nullptr)
	{
		button_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/button.png", 1280.0f, 2560.0f, 2, 4, 640.0f, 640.0f);
	}
	if (stick_img == nullptr)
	{
		stick_img = std::make_unique<Sprite>(L"./Data/Image/UI/GameScene/stick.png", 1280.0f, 3200.0f, 2, 5, 640.0f, 640.0f);
	}
}


void InputGamePadDraw::Update(Player* player1p, Player* player2p)
{
	//���ꂼ��̃v���C���[�̓��͂��擾���A���̓��X�g�ɓ���Ă���
	//���肵�����̓f�[�^���{�^���A�X�e�B�b�N�ɂ��ꂼ�ꊄ�蓖�Ă�

	//1P���͎擾
	bool Generated = false;
	for (int i = 0; i < player1p->pad->x_input.size(); i++)
	{
		if (player1p->pad->x_input[i] == 1)
		{
			//���̃t���[���ɓ��͂���Ă���
			int button = GetButtonNum(i);
			int stick = GetStickNum(i);

			if (button != -1 || stick != -1)
			{
				if (!Generated)
				{
					//1�t���[���Ɉ�x������������
					p1_input_list.push_back(Input_list_pad());
					Generated = true;
				}
			}
			if (stick != -1)
			{
				//�X�e�B�b�N�̓��͂��������ꍇ�A���͂�ۑ�����
				p1_input_list.back().stick_input = stick;
			}
			if (button != -1)
			{
				//�{�^���̓��͂��������ꍇ�A���X�g�𐶐������͂�ۑ�����
				p1_input_list.back().button_input.push_back(int());
				p1_input_list.back().button_input.back() = button;

				//�R�}���h������s���A�������Ă���ꍇ�͂��ꂼ��ۑ�����
				Command command = player1p->pad->com_list.CommandCheack(i, player1p->pad->input_history.data(), player1p->rightOrleft);
				if (command == Command::LHURF)
				{
					p1_input_list.back().stick_input = 9;
				}
				if (command == Command::RHURF)
				{
					p1_input_list.back().stick_input = 8;
				}
			}
		}

		if (Generated)
		{
			//���̃t���[���Ő������s���Ă����ꍇ
			if (p1_input_list.back().stick_input == -1)
			{
				//�����X�e�B�b�N�̓��͂��Ȃ������ꍇ
				if (player1p->pad->x_input[i] > 0)
				{
					int stick = GetStickNum(i);
					if (stick != -1)
					{
						p1_input_list.back().stick_input = stick;
						p1_input_list.back().pushed = true;
					}
				}
			}
		}
		//10�ȏ�ɂȂ�����擪����폜
		if (p1_input_list.size() > 10)
		{
			p1_input_list.pop_front();
		}
	}


	//2P���͎擾
	Generated = false;
	for (int i = 0; i < player2p->pad->x_input.size(); i++)
	{
		if (player2p->pad->x_input[i] == 1)
		{
			//���̃t���[���ɓ��͂���Ă���
			int button = GetButtonNum(i);
			int stick = GetStickNum(i);

			if (button != -1 || stick != -1)
			{
				if (!Generated)
				{
					//1�t���[���Ɉ�x������������
					p2_input_list.push_back(Input_list_pad());
					Generated = true;
				}
			}
			if (stick != -1)
			{
				//�X�e�B�b�N�̓��͂��������ꍇ�A���͂�ۑ�����
				p2_input_list.back().stick_input = stick;
			}
			if (button != -1)
			{
				//�{�^���̓��͂��������ꍇ�A���X�g�𐶐������͂�ۑ�����
				p2_input_list.back().button_input.push_back(int());
				p2_input_list.back().button_input.back() = button;

				//�R�}���h������s���A�������Ă���ꍇ�͂��ꂼ��ۑ�����
				Command command = player2p->pad->com_list.CommandCheack(i, player2p->pad->input_history.data(), player2p->rightOrleft);
				if (command == Command::LHURF)
				{
					p2_input_list.back().stick_input = 9;
				}
				if (command == Command::RHURF)
				{
					p2_input_list.back().stick_input = 8;
				}
			}
		}

		if (Generated)
		{
			//���̃t���[���Ő������s���Ă����ꍇ
			if (p2_input_list.back().stick_input == -1)
			{
				//�����X�e�B�b�N�̓��͂��Ȃ������ꍇ
				if (player2p->pad->x_input[i] > 0)
				{
					int stick = GetStickNum(i);
					if (stick != -1)
					{
						p2_input_list.back().stick_input = stick;
						p2_input_list.back().pushed = true;
					}
				}
			}
		}
		//10�ȏ�ɂȂ�����擪����폜
		if (p2_input_list.size() > 10)
		{
			p2_input_list.pop_front();
		}
	}

}

void InputGamePadDraw::Draw(YRShader* shader)
{
	float screen_weight = static_cast<float>(FRAMEWORK.SCREEN_WIDTH);
	float screen_height = static_cast<float>(FRAMEWORK.SCREEN_HEIGHT);
	float size = 0.15f;
	float adjust_height = 100.0f;
	float adjust_weight_stick_p1 = 0.03f;
	float adjust_weight_stick_p2 = 0.97f;
	float adjust_weight_button_p1 = 0.09f;
	float adjust_weight_button_p2 = 0.92f;
	float adjust_weight_button = 100.0f;

	//1P���͕\��
	int i = 0;
	for (auto& il : p1_input_list)
	{
		if (il.stick_input != -1)
		{
			//�l��-1�̏ꍇ�͕\�����Ȃ�
			if (il.pushed)
			{
				stick_img->DrawRotaDivGraph(
					shader,
					screen_weight * adjust_weight_stick_p1,
					(i + 1) * adjust_height,
					0.0f,
					size,
					il.stick_input,
					SpriteMask::NONE,
					DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f));
			}
			else
			{
				stick_img->DrawRotaDivGraph(
					shader,
					screen_weight * adjust_weight_stick_p1,
					(i + 1) * adjust_height,
					0.0f,
					size,
					il.stick_input);
			}
		}
		if (!il.button_input.empty())
		{
			for (int j = 0; j < il.button_input.size(); j++)
			{
				//�l���Ȃ��ꍇ�͕\�����Ȃ�
				button_img->DrawRotaDivGraph(
					shader,
					screen_weight * adjust_weight_button_p1 + (j * adjust_weight_button),
					(i + 1) * adjust_height,
					0.0f,
					size,
					il.button_input[j]);
			}
		}
		i++;
	}

	//2P���͕\��
	i = 0;
	for (auto& il : p2_input_list)
	{
		if (il.stick_input != -1)
		{
			//�l��-1�̏ꍇ�͕\�����Ȃ�
			if (il.pushed)
			{
				stick_img->DrawRotaDivGraph(
					shader,
					screen_weight * adjust_weight_stick_p2,
					(i + 1) * adjust_height,
					0.0f,
					size,
					il.stick_input,
					SpriteMask::NONE,
					DirectX::XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f));
			}
			else
			{
				stick_img->DrawRotaDivGraph(
					shader,
					screen_weight * adjust_weight_stick_p2,
					(i + 1) * adjust_height,
					0.0f,
					size,
					il.stick_input);
			}
		}
		if (!il.button_input.empty())
		{
			for (int j = 0; j < il.button_input.size(); j++)
			{
				//�l���Ȃ��ꍇ�͕\�����Ȃ�
				button_img->DrawRotaDivGraph(
					shader,
					screen_weight * adjust_weight_button_p2 - (j * adjust_weight_button),
					(i + 1) * adjust_height,
					0.0f,
					size,
					il.button_input[j]);
			}
		}
		i++;
	}
}

int InputGamePadDraw::GetButtonNum(int button)
{
	//�����œn���ꂽ�{�^�����z��̂ǂ̏ꏊ�ɂ��邩��Ԃ�
	int returnNum = -1;
	for (int i = 0; i < button_img_list.size(); i++)
	{
		if (button_img_list[i] == button)
		{
			returnNum = i;
		}
	}
	return returnNum;
}


int InputGamePadDraw::GetStickNum(int stick)
{
	//�����œn���ꂽ�X�e�B�b�N�ԍ����z��̂ǂ̏ꏊ�ɂ��邩��Ԃ�
	int returnNum = -1;
	for (int i = 0; i < stick_img_list.size(); i++)
	{
		if (stick_img_list[i] == stick)
		{
			returnNum = i;
		}
	}
	return returnNum;
}

void InputGamePadDraw::UnInit()
{
	button_img.reset();
	stick_img.reset();
	button_img = nullptr;
	stick_img = nullptr;
}