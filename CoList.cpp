#include	"CoList.h"
#include	"YRGamePad.h"


Command CommandList::CommandCheack(int last, int* hist, float distans)
{
	//--------------------------------------------------------------
	//=========================================
	//		コマンド確認関数
	//=========================================
	//
	//
	//---------------------------------------------------------------

	trigger = false;
	state = Command::NOCOMMAND;
	numI = 0;
	if (command_timer > TIMER_LIMIT || command_timer < 0.0f)
	{
		return Command::NOCOMMAND;
	}

	command_timer = 0;
	for (int i = 0; i < 10; i++)
	{
		histry[i] = hist[i];
	}
	lastinput = last;

	for (int i = 9; i > -1; i--)
	{
		if (histry[i] != -1)
		{
			if (histry[i] != lastinput)
			{
				if (histry[i] == scastI(PAD::STICK_R))
				{
					state = Command::RHURF;
					numI = i;
					break;
				}
				else if (histry[i] == scastI(PAD::STICK_L))
				{
					state = Command::LHURF;
					numI = i;
					break;
				}
				else
				{
					return Command::NOCOMMAND;
				}
			}
		}
	}

	if (numI < 2)
	{
		return Command::NOCOMMAND;
	}

	switch (state)
	{
	case Command::RHURF:
		if (histry[numI - 1] == scastI(PAD::STICK_RDown))
		{
			if (histry[numI - 2] == scastI(PAD::STICK_D))
			{
				if (distans > 0)
				{
					return Command::RHURF;
				}
				if (distans < 0)
				{
					return Command::LHURF;
				}
			}
		}
		else
		{
			return Command::NOCOMMAND;
		}
		break;
	case Command::LHURF:
		if (histry[numI - 1] == scastI(PAD::STICK_LDown))
		{
			if (histry[numI - 2] == scastI(PAD::STICK_D))
			{
				if (distans < 0)
				{
					return Command::RHURF;
				}
				if (distans > 0)
				{
					return Command::LHURF;
				}
			}
		}
		break;
	default:
		break;
	}

	return Command::NOCOMMAND;
}

void CommandList::Reset()
{
	command_timer = 0;
}