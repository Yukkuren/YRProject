#include	"CoList.h"
#include	"YRGamePad.h"


int CommandList::CommandCheack(int last, int* hist, float distans)
{
	trigger = false;
	state = NOCOMMAND;
	numI = 0;
	if (command_timer > TIMER_LIMIT || command_timer == 0)
	{
		return NOCOMMAND;
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
					state = RHURF;
					numI = i;
					break;
				}
				else if (histry[i] == scastI(PAD::STICK_L))
				{
					state = LHURF;
					numI = i;
					break;
				}
				else
				{
					return NOCOMMAND;
				}
			}
		}
	}

	if (numI < 2)
	{
		return NOCOMMAND;
	}

	switch (state)
	{
	case RHURF:
		if (histry[numI - 1] == scastI(PAD::STICK_RDown))
		{
			if (histry[numI - 2] == scastI(PAD::STICK_D))
			{
				if (distans > 0)
				{
					return RHURF;
				}
				if (distans < 0)
				{
					return LHURF;
				}
			}
		}
		else
		{
			return NOCOMMAND;
		}
		break;
	case LHURF:
		if (histry[numI - 1] == scastI(PAD::STICK_LDown))
		{
			if (histry[numI - 2] == scastI(PAD::STICK_D))
			{
				if (distans < 0)
				{
					return RHURF;
				}
				if (distans > 0)
				{
					return LHURF;
				}
			}
		}
		break;
	default:
		break;
	}

	return NOCOMMAND;
}

void CommandList::Reset()
{
	command_timer = 0;
}