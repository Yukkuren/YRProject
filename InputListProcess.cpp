#include "Scene.h"

void InputGamePadDraw::Init()
{
	//������
	for (int i = 0; i < p1_input_list.size(); i++)
	{
		p1_input_list[i].button_input = -1;
		p1_input_list[i].stick_input = -1;
	}

	for (int i = 0; i < p2_input_list.size(); i++)
	{
		p2_input_list[i].button_input = -1;
		p2_input_list[i].stick_input = -1;
	}
}


void InputGamePadDraw::Update(std::list<InputListor>que1, std::list<InputListor>que2)
{
	//���ꂼ��̃v���C���[�̓��͂��擾���A���̓��X�g�ɓ���Ă���
	if (!que1.empty())
	{
		int i = 0;
		for (auto& it : que1)
		{
			p1_input_list[i].kind = it.kind;
			i++;
		}
	}
	if (!que2.empty())
	{
		int i = 0;
		for (auto& it : que2)
		{
			p2_input_list[i].kind = it.kind;
			i++;
		}
	}

	//���肵�����̓f�[�^���{�^���A�X�e�B�b�N�ɂ��ꂼ�ꊄ�蓖�Ă�
}

void InputGamePadDraw::Draw(YRShader* shader)
{

}