#include	"Key.h"
#include	"YRGamePad.h"
//#include	"Common.h"
#include	"HitCheak.h"

void GamePad2::Init()
{
	/*GetJoypadXInputState(DX_INPUT_PAD1, &input);
	if (GetJoypadXInputState(DX_INPUT_PAD1, &input) != -1)
	{
	}
	else
	{
		#define LEFTTRIGGER		(Key[KEY_INPUT_A])
	}*/
	for (auto& it : input_history)
	{
		it = -1;
	}
	stickRD = false;
	bottomRD = false;
	stickLD = false;
	bottomLD = false;
	stickRU = false;
	bottomRU = false;
	stickLU = false;
	bottomLU = false;
	pre_input = false;
	dash_trigger = false;
}

void GamePad2::UnInit()
{
	if (!que.empty())
	{
		for (auto& it = que.begin(); it != que.end();)
		{
			it = que.erase(it);
		}
	}
}

void GamePad2::Update()
{
#pragma region hei

#pragma endregion
	//else
	{
		/*for (int i = 0; i<static_cast<int>(PAD::PAD_END); i++)
		{
			x_input[i] = 0;
		}*/

		if (pKeyState.k1flg > 0)
		{
			x_input[static_cast<int>(PAD::X)]++;
		}
		else
		{
			x_input[static_cast<int>(PAD::X)] = 0;
		}



		if (pKeyState.k2flg > 0)
		{
			x_input[static_cast<int>(PAD::Y)]++;
		}
		else
		{
			x_input[static_cast<int>(PAD::Y)] = 0;
		}



		if (pKeyState.k3flg > 0)
		{
			x_input[static_cast<int>(PAD::B)]++;
		}
		else
		{
			x_input[static_cast<int>(PAD::B)] = 0;
		}



		if (pKeyState.k4flg > 0)
		{
			x_input[static_cast<int>(PAD::A)]++;
		}
		else
		{
			x_input[static_cast<int>(PAD::A)] = 0;
		}


		if (pKeyState.k5flg > 0)
		{
			x_input[static_cast<int>(PAD::L_TRIGGER)]++;
		}
		else
		{
			x_input[static_cast<int>(PAD::L_TRIGGER)] = 0;
		}


		if (pKeyState.k6flg > 0)
		{
			x_input[static_cast<int>(PAD::R_TRIGGER)]++;
		}
		else
		{
			x_input[static_cast<int>(PAD::R_TRIGGER)] = 0;
		}




		if (pKeyState.leftflg > 0)
		{
			if (dash_trigger)
			{
				x_input[static_cast<int>(PAD::L_DASH)]++;
			}
			else
			{
				x_input[static_cast<int>(PAD::L_DASH)] = 0;
			}
			x_input[static_cast<int>(PAD::STICK_L)]++;
		}
		else
		{
			x_input[static_cast<int>(PAD::STICK_L)] = 0;
			x_input[static_cast<int>(PAD::L_DASH)] = 0;
		}
		if (x_input[static_cast<int>(PAD::STICK_L)] == 1)
		{
			if (!que.empty())
			{
				if (que.back().kind == scastI(PAD::STICK_L) && que.back().timer > 0)
				{
					dash_trigger = true;
				}
			}
		}



		if (pKeyState.rightflg > 0)
		{
			if (dash_trigger)
			{
				x_input[static_cast<int>(PAD::R_DASH)]++;
			}
			else
			{
				x_input[static_cast<int>(PAD::R_DASH)] = 0;
			}
			x_input[static_cast<int>(PAD::STICK_R)]++;
		}
		else
		{
			x_input[static_cast<int>(PAD::STICK_R)] = 0;
			x_input[static_cast<int>(PAD::R_DASH)] = 0;
		}
		if (x_input[static_cast<int>(PAD::STICK_R)] == 1)
		{
			if (!que.empty())
			{
				if (que.back().kind == scastI(PAD::STICK_R) && que.back().timer > 0)
				{
					dash_trigger = true;
				}
			}
		}


		if (x_input[static_cast<int>(PAD::STICK_R)] == 0 && x_input[static_cast<int>(PAD::STICK_L)] == 0)
		{
			dash_trigger = false;
		}



		if (pKeyState.k7flg > 0)
		{
			x_input[static_cast<int>(PAD::START)]++;
		}
		else
		{
			x_input[static_cast<int>(PAD::START)] = 0;
		}



		if (pKeyState.upflg > 0)
		{
			x_input[scastI(PAD::STICK_U)]++;
		}
		else
		{
			x_input[scastI(PAD::STICK_U)] = 0;
		}



		if (pKeyState.downflg > 0)
		{
			x_input[scastI(PAD::STICK_D)]++;
		}
		else
		{
			x_input[scastI(PAD::STICK_D)] = 0;
		}


		if (pKeyState.upflg == 1 && pKeyState.rightflg > 0)
		{
			x_input[scastI(PAD::STICK_U)] = 0;
			x_input[scastI(PAD::STICK_R)] = 0;
			x_input[scastI(PAD::DOWN)] = 0;
			x_input[scastI(PAD::RIGHT)] = 0;
			x_input[scastI(PAD::STICK_RUp)]++;
		}
		else
		{
			x_input[scastI(PAD::STICK_RUp)] = 0;
		}


		if (pKeyState.upflg == 1 && pKeyState.leftflg > 0)
		{
			x_input[scastI(PAD::STICK_U)] = 0;
			x_input[scastI(PAD::STICK_L)] = 0;
			x_input[scastI(PAD::DOWN)] = 0;
			x_input[scastI(PAD::LEFT)] = 0;
			x_input[scastI(PAD::STICK_LUp)]++;
		}
		else
		{
			x_input[scastI(PAD::STICK_LUp)] = 0;
		}



		if (pKeyState.downflg > 0 && pKeyState.rightflg > 0)
		{
			x_input[scastI(PAD::STICK_D)] = 0;
			x_input[scastI(PAD::STICK_R)] = 0;
			x_input[scastI(PAD::DOWN)] = 0;
			x_input[scastI(PAD::RIGHT)] = 0;
			x_input[scastI(PAD::STICK_RDown)]++;
		}
		else
		{
			x_input[scastI(PAD::STICK_RDown)] = 0;
		}


		if (pKeyState.downflg > 0 && pKeyState.leftflg > 0)
		{
			x_input[scastI(PAD::STICK_D)] = 0;
			x_input[scastI(PAD::STICK_L)] = 0;
			x_input[scastI(PAD::DOWN)] = 0;
			x_input[scastI(PAD::LEFT)] = 0;
			x_input[scastI(PAD::STICK_LDown)]++;
		}
		else
		{
			x_input[scastI(PAD::STICK_LDown)] = 0;
		}


		if (x_input[scastI(PAD::STICK_RDown)] == 1 || x_input[scastI(PAD::STICK_LDown)] == 1)
		{
			com_list.trigger = true;
			com_list.command_timer = 0;
		}

		if (com_list.trigger)
		{
			com_list.command_timer++;
		}

		/*if (Key[KEY_INPUT_O] > 0)
		{
			x_input[scastI(PAD::NONE1)]++;
		}
		else
		{
			x_input[scastI(PAD::NONE1)]=0;
		}*/
	}

	InputSave();
	if (!Hitcheak::hit)
	{
		InputUpdate();
	}
}

void GamePad2::InputSave()
{
	for (int i = 0; i < scastI(PAD::PAD_END); i++)
	{
		if (x_input[i] == 1)
		{
			que.push_back(InputListor(i));
			que.back().timer = 20;
		}
		if (que.size() > 10)
		{
			que.pop_front();
		}
	}

	if (!que.empty())
	{
		int j = 0;
		for (auto& it : que)
		{
			input_history[j] = it.kind;
			j++;
		}

		if (pre_input)
		{
			if (que.back().timer > 0)
			{
				x_input[que.back().kind] = 1;
			}
		}
	}
}
void GamePad2::InputUpdate()
{
	if (!que.empty())
	{
		for (auto& it : que)
		{
			it.timer--;
			if (it.timer < 0)
			{
				it.timer = 0;
			}
		}
	}
}
