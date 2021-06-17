#include	"CoList.h"
#include	"YRGamePad.h"


Command CommandList::CommandCheack(int last, int* hist, float distans)
{
	//--------------------------------------------------------------
	//=========================================
	//		コマンド確認関数
	//=========================================
	//・入力の履歴を最新のものからさかのぼっていき、
	//  過去にコマンドが入力されているか動かを判定する
	//---------------------------------------------------------------

	trigger = false;
	state = Command::NOCOMMAND;

	//Listの何処を見たかを保存する
	numI = 0;

	//コマンド判定タイマーが一定の値以内ならコマンド判定を取るようにする(コマンド受付猶予)
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
	//引数で最後に入力されたボタンをもらっている(攻撃用ボタンのX、Y、B、RBボタンなど)
	lastinput = last;

	//Listの中身を後ろから見ていく
	for (int i = 9; i > -1; i--)
	{
		if (histry[i] != -1)
		{
			//最後に入力されたものと違うものであれば
			if (histry[i] != lastinput)
			{
				if (histry[i] == scastI(PAD::STICK_R))
				{
					//入力されたのが右だった
					state = Command::RHURF;
					numI = i;
					break;
				}
				else if (histry[i] == scastI(PAD::STICK_L))
				{
					//入力されたのが左だった
					state = Command::LHURF;
					numI = i;
					break;
				}
				else
				{
					//コマンドは無かった
					return Command::NOCOMMAND;
				}
			}
		}
	}

	if (numI < 2)
	{
		//残っているのが2以下ならコマンドは絶対に成立しないのでここで区切る
		return Command::NOCOMMAND;
	}

	//Listの中身を判定した場所から見ていく
	//※コマンドは正しく入力されていればListの中で「↓」「↓→」「→」と並んでいるはずなので
	//一つずつ後ろを参照していく
	switch (state)
	{
	case Command::RHURF:
		//最後に入力されたのは右だった
		if (histry[numI - 1] == scastI(PAD::STICK_RDown))
		{
			//その前に入力されたのが右下だった
			if (histry[numI - 2] == scastI(PAD::STICK_D))
			{
				//更にその前に入力されたのが下だった
				if (distans > 0)
				{
					//プレイヤーは右を向いている
					trigger = true;
					return Command::RHURF;
				}
				if (distans < 0)
				{
					//プレイヤーは左を向いている
					trigger = true;
					return Command::LHURF;
				}
			}
		}
		else
		{
			//コマンドはない
			return Command::NOCOMMAND;
		}
		break;
	case Command::LHURF:
		//最後に入力されたのは左だった
		if (histry[numI - 1] == scastI(PAD::STICK_LDown))
		{
			//その前に入力されたのが左下だった
			if (histry[numI - 2] == scastI(PAD::STICK_D))
			{
				//更にその前に入力されたのが下だった
				if (distans < 0)
				{
					//プレイヤーは右を向いている
					trigger = true;
					return Command::RHURF;
				}
				if (distans > 0)
				{
					//プレイヤーは左を向いている
					trigger = true;
					return Command::LHURF;
				}
			}
		}
		else
		{
			//コマンドはない
			return Command::NOCOMMAND;
		}
		break;
	default:
		break;
	}
	trigger = true;
	//コマンドはない
	return Command::NOCOMMAND;
}

void CommandList::Reset()
{
	command_timer = 0.0f;
}