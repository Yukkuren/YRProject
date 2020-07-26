#pragma once

#include<directxmath.h>

//-----------------------------------------------------
//	�e�{�^���̏�ԎQ�Ƃ́u_state�v�̕ϐ��Ŋm�F����
//	0:������Ă��Ȃ�
//	1:�������u��
//	2�ȏ�:������Ă���
//-----------------------------------------------------


class YRMouse
{
private:
	bool left_click = false;	//�}�E�X���N���b�N
	bool right_click = false;	//�}�E�X�E�N���b�N
	bool cursor_click = false;	//�}�E�X�����N���b�N
	YRMouse() {};
public:
	DirectX::XMFLOAT2 pos;

	int left_state = 0;
	int right_state = 0;
	int cursor_state = 0;
	int wheel_state = 0;

	void Update()
	{
		if (left_click)
		{
			left_state++;
		}
		else
		{
			left_state = 0;
		}
		if (right_click)
		{
			right_state++;
		}
		else
		{
			right_state = 0;
		}
		if (cursor_click)
		{
			cursor_state++;
		}
		else
		{
			cursor_state = 0;
		}
	};

	void SetLeft(bool left)
	{
		left_click = left;
	}
	void SetRight(bool right)
	{
		right_click = right;
	}
	void SetCursor(bool cursor)
	{
		cursor_click = cursor;
	}

	static YRMouse &getInstance()
	{
		static YRMouse instance;
		return instance;
	}
};

#define		pMouse	(YRMouse::getInstance())
