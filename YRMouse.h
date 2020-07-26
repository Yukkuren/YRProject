#pragma once

#include<directxmath.h>

//-----------------------------------------------------
//	各ボタンの状態参照は「_state」の変数で確認する
//	0:押されていない
//	1:押した瞬間
//	2以上:押されている
//-----------------------------------------------------


class YRMouse
{
private:
	bool left_click = false;	//マウス左クリック
	bool right_click = false;	//マウス右クリック
	bool cursor_click = false;	//マウス中央クリック
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
