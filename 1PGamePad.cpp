#include	"Key.h"
#include	"YRGamePad.h"
//#include	"Common.h"
//#include	"HitCheak.h"

InputListor::InputListor(int n)
{
	kind = n;
}


void GamePad1::Init()
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

void GamePad1::UnInit()
{
	if (!que.empty())
	{
		for (auto& it = que.begin(); it != que.end();)
		{
			it = que.erase(it);
		}
	}
}

void GamePad1::Update()
{
#pragma region hei

#pragma endregion

	////GetJoypadXInputState(DX_INPUT_PAD1, &input);
	//if (GetJoypadXInputState(DX_INPUT_PAD1, &input) != -1)
	//{

	//	/*for (int i=0;i<static_cast<int>(PAD::PAD_END);i++)
	//	{
	//		x_input[i] = 0;
	//	}*/

	//	GetJoypadAnalogInput(&Xbuf, &Ybuf, DX_INPUT_PAD1);
	//	for (int i = 0; i < static_cast<int>(PAD::BUTTOM_END); i++)
	//	{
	//		if (input.Buttons[i] != 0)
	//		{
	//			x_input[i]++;
	//		}
	//		else
	//		{
	//			x_input[i] = 0;
	//		}
	//	}

	//	if (Key[KEY_INPUT_0] > 0)
	//	{
	//		x_input[scastI(PAD::X)] = 1;
	//	}


	//	if (input.LeftTrigger > trigger_put)
	//	{
	//		x_input[static_cast<int>(PAD::L_TRIGGER)]++;
	//	}
	//	else
	//	{
	//		x_input[static_cast<int>(PAD::L_TRIGGER)] = 0;
	//	}



	//	if (input.RightTrigger > trigger_put)
	//	{
	//		x_input[static_cast<int>(PAD::R_TRIGGER)]++;
	//	}
	//	else
	//	{
	//		x_input[static_cast<int>(PAD::R_TRIGGER)] = 0;
	//	}



	//	if (Xbuf > deadzone || input.Buttons[scastI(PAD::RIGHT)])
	//	{
	//		if (dash_trigger)
	//		{
	//			x_input[static_cast<int>(PAD::R_DASH)]++;
	//			x_input[scastI(PAD::STICK_L)] = 0;
	//		}
	//		else
	//		{
	//			x_input[static_cast<int>(PAD::R_DASH)] = 0;
	//		}
	//		x_input[scastI(PAD::RIGHT)] = 0;
	//		x_input[static_cast<int>(PAD::STICK_R)]++;
	//	}
	//	else
	//	{
	//		x_input[static_cast<int>(PAD::STICK_R)] = 0;
	//		x_input[static_cast<int>(PAD::R_DASH)] = 0;
	//	}


	//	if (x_input[static_cast<int>(PAD::STICK_R)] == 1)
	//	{
	//		if (!que.empty())
	//		{
	//			if (que.back().kind == scastI(PAD::STICK_R) && que.back().timer > 0)
	//			{
	//				dash_trigger = true;
	//			}
	//		}
	//	}




	//	if (Xbuf < -deadzone || input.Buttons[scastI(PAD::LEFT)])
	//	{
	//		if (dash_trigger)
	//		{
	//			x_input[static_cast<int>(PAD::L_DASH)]++;
	//			x_input[scastI(PAD::STICK_R)] = 0;
	//		}
	//		else
	//		{
	//			x_input[static_cast<int>(PAD::L_DASH)] = 0;
	//		}
	//		x_input[scastI(PAD::LEFT)] = 0;
	//		x_input[static_cast<int>(PAD::STICK_L)]++;
	//	}
	//	else
	//	{
	//		x_input[static_cast<int>(PAD::STICK_L)] = 0;
	//		x_input[static_cast<int>(PAD::L_DASH)] = 0;
	//	}


	//	if (x_input[static_cast<int>(PAD::STICK_L)] == 1)
	//	{
	//		if (!que.empty())
	//		{
	//			if (que.back().kind == scastI(PAD::STICK_L) && que.back().timer > 0)
	//			{
	//				dash_trigger = true;
	//			}
	//		}
	//	}

	//	if (x_input[static_cast<int>(PAD::STICK_R)] == 0 && x_input[static_cast<int>(PAD::STICK_L)] == 0)
	//	{
	//		dash_trigger = false;
	//	}



	//	if (Ybuf > deadzoneY || input.Buttons[scastI(PAD::DOWN)])
	//	{
	//		x_input[scastI(PAD::DOWN)] = 0;
	//		x_input[static_cast<int>(PAD::STICK_D)]++;
	//	}
	//	else
	//	{
	//		x_input[static_cast<int>(PAD::STICK_D)] = 0;
	//	}




	//	if (Ybuf < -deadzoneY || input.Buttons[scastI(PAD::UP)])
	//	{
	//		x_input[scastI(PAD::UP)] = 0;
	//		if (high_trigger)
	//		{
	//			x_input[scastI(PAD::HIGH_UP)]++;
	//			x_input[scastI(PAD::STICK_U)] = 0;
	//		}
	//		else
	//		{
	//			x_input[static_cast<int>(PAD::STICK_U)]++;
	//			x_input[scastI(PAD::HIGH_UP)] = 0;
	//		}
	//	}
	//	else
	//	{
	//		x_input[static_cast<int>(PAD::STICK_U)] = 0;
	//		x_input[scastI(PAD::HIGH_UP)] = 0;
	//	}

	//	if (x_input[static_cast<int>(PAD::STICK_U)] == 1)
	//	{
	//		if (!que.empty())
	//		{
	//			if (que.back().timer > 0)
	//			{
	//				if (que.back().kind == scastI(PAD::STICK_D) || que.back().kind == scastI(PAD::STICK_RDown) || que.back().kind == scastI(PAD::STICK_LDown))
	//				{
	//					high_trigger = true;
	//					x_input[scastI(PAD::STICK_U)] = 0;
	//				}
	//			}
	//		}
	//	}

	//	if (Ybuf > -deadzoneY && input.Buttons[scastI(PAD::UP)] == 0)
	//	{
	//		high_trigger = false;
	//		x_input[scastI(PAD::HIGH_UP)] = 0;
	//	}

	//	if (x_input[scastI(PAD::HIGH_UP)] == 1 && x_input[scastI(PAD::STICK_R)] > 0)
	//	{
	//		x_input[scastI(PAD::HIGH_UP_R)]++;
	//		x_input[scastI(PAD::HIGH_UP)] = 0;
	//	}
	//	else
	//	{
	//		x_input[scastI(PAD::HIGH_UP_R)] = 0;
	//	}

	//	if (x_input[scastI(PAD::HIGH_UP)] == 1 && x_input[scastI(PAD::STICK_L)] > 0)
	//	{
	//		x_input[scastI(PAD::HIGH_UP_L)]++;
	//		x_input[scastI(PAD::HIGH_UP)] = 0;
	//	}
	//	else
	//	{
	//		x_input[scastI(PAD::HIGH_UP_L)] = 0;
	//	}

	//	/*if (Ybuf < -deadzoneY&&Xbuf > deadzone)
	//	{
	//		stickRU = true;
	//	}
	//	else
	//	{
	//		stickRU = false;
	//	}
	//	if (Ybuf < -deadzoneY&&Xbuf < -deadzone)
	//	{
	//		stickLU = true;
	//	}
	//	else
	//	{
	//		stickLU = false;
	//	}*/

	//	if (x_input[scastI(PAD::STICK_U)] == 1 && x_input[scastI(PAD::STICK_R)] > 0)
	//	{
	//		stickRU = true;
	//	}
	//	else
	//	{
	//		stickRU = false;
	//	}
	//	if (x_input[scastI(PAD::STICK_U)] == 1 && x_input[scastI(PAD::STICK_L)] > 0)
	//	{
	//		stickLU = true;
	//	}
	//	else
	//	{
	//		stickLU = false;
	//	}




	//	if (Ybuf > deadzoneY&& Xbuf > deadzone)
	//	{
	//		stickRD = true;
	//	}
	//	else
	//	{
	//		stickRD = false;
	//	}
	//	if (Ybuf > deadzoneY&& Xbuf < -deadzone)
	//	{
	//		stickLD = true;
	//	}
	//	else
	//	{
	//		stickLD = false;
	//	}



	//	if (input.Buttons[scastI(PAD::UP)] == 1 && input.Buttons[scastI(PAD::RIGHT)] > 0)
	//	{
	//		bottomRU = true;
	//	}
	//	else
	//	{
	//		bottomRU = false;
	//	}
	//	if (input.Buttons[scastI(PAD::UP)] == 1 && input.Buttons[scastI(PAD::LEFT)] > 0)
	//	{
	//		bottomLU = true;
	//	}
	//	else
	//	{
	//		bottomLU = false;
	//	}



	//	if (input.Buttons[scastI(PAD::DOWN)] && input.Buttons[scastI(PAD::RIGHT)])
	//	{
	//		bottomRD = true;
	//	}
	//	else
	//	{
	//		bottomRD = false;
	//	}
	//	if (input.Buttons[scastI(PAD::DOWN)] && input.Buttons[scastI(PAD::LEFT)])
	//	{
	//		bottomLD = true;
	//	}
	//	else
	//	{
	//		bottomLD = false;
	//	}



	//	if (stickRU || bottomRU)
	//	{
	//		x_input[scastI(PAD::STICK_U)] = 0;
	//		x_input[scastI(PAD::STICK_R)] = 0;
	//		x_input[scastI(PAD::UP)] = 0;
	//		x_input[scastI(PAD::RIGHT)] = 0;
	//		x_input[scastI(PAD::STICK_RUp)]++;
	//	}
	//	else
	//	{
	//		x_input[scastI(PAD::STICK_RUp)] = 0;
	//	}


	//	if (stickLU || bottomLU)
	//	{
	//		x_input[scastI(PAD::STICK_U)] = 0;
	//		x_input[scastI(PAD::STICK_L)] = 0;
	//		x_input[scastI(PAD::DOWN)] = 0;
	//		x_input[scastI(PAD::LEFT)] = 0;
	//		x_input[scastI(PAD::STICK_LUp)]++;
	//	}
	//	else
	//	{
	//		x_input[scastI(PAD::STICK_LUp)] = 0;
	//	}



	//	if (stickRD || bottomRD)
	//	{
	//		x_input[scastI(PAD::STICK_D)] = 0;
	//		x_input[scastI(PAD::STICK_R)] = 0;
	//		x_input[scastI(PAD::DOWN)] = 0;
	//		x_input[scastI(PAD::RIGHT)] = 0;
	//		x_input[scastI(PAD::STICK_RDown)]++;
	//	}
	//	else
	//	{
	//		x_input[scastI(PAD::STICK_RDown)] = 0;
	//	}


	//	if (stickLD || bottomLD)
	//	{
	//		x_input[scastI(PAD::STICK_D)] = 0;
	//		x_input[scastI(PAD::STICK_L)] = 0;
	//		x_input[scastI(PAD::DOWN)] = 0;
	//		x_input[scastI(PAD::LEFT)] = 0;
	//		x_input[scastI(PAD::STICK_LDown)]++;
	//	}
	//	else
	//	{
	//		x_input[scastI(PAD::STICK_LDown)] = 0;
	//	}





	//	if (x_input[scastI(PAD::STICK_RDown)] == 1 || x_input[scastI(PAD::STICK_LDown)] == 1)
	//	{
	//		com_list.trigger = true;
	//		com_list.command_timer = 0;
	//	}

	//	if (com_list.trigger)
	//	{
	//		com_list.command_timer++;
	//	}



	//	/*if (Key[KEY_INPUT_O] > 0)
	//	{
	//		x_input[scastI(PAD::NONE1)]++;
	//	}
	//	else
	//	{
	//		x_input[scastI(PAD::NONE1)] = 0;
	//	}*/

	//}
	//else
	//{
	//	/*for (int i = 0; i<static_cast<int>(PAD::PAD_END); i++)
	//	{
	//		x_input[i] = 0;
	//	}*/

	//	if (Key[KEY_INPUT_H] > 0)
	//	{
	//		x_input[static_cast<int>(PAD::X)]++;
	//	}
	//	else
	//	{
	//		x_input[static_cast<int>(PAD::X)] = 0;
	//	}



	//	if (Key[KEY_INPUT_J] > 0)
	//	{
	//		x_input[static_cast<int>(PAD::Y)]++;
	//	}
	//	else
	//	{
	//		x_input[static_cast<int>(PAD::Y)] = 0;
	//	}



	//	if (Key[KEY_INPUT_K] > 0)
	//	{
	//		x_input[static_cast<int>(PAD::B)]++;
	//	}
	//	else
	//	{
	//		x_input[static_cast<int>(PAD::B)] = 0;
	//	}



	//	if (Key[KEY_INPUT_L] > 0)
	//	{
	//		x_input[static_cast<int>(PAD::A)]++;
	//	}
	//	else
	//	{
	//		x_input[static_cast<int>(PAD::A)] = 0;
	//	}



	//	if (Key[KEY_INPUT_A] > 0)
	//	{
	//		if (dash_trigger)
	//		{
	//			x_input[static_cast<int>(PAD::L_DASH)]++;
	//		}
	//		else
	//		{
	//			x_input[static_cast<int>(PAD::L_DASH)] = 0;
	//		}
	//		x_input[static_cast<int>(PAD::STICK_L)]++;
	//	}
	//	else
	//	{
	//		x_input[static_cast<int>(PAD::STICK_L)] = 0;
	//		x_input[static_cast<int>(PAD::L_DASH)] = 0;
	//	}
	//	if (x_input[static_cast<int>(PAD::STICK_L)] == 1)
	//	{
	//		if (!que.empty())
	//		{
	//			if (que.back().kind == scastI(PAD::STICK_L) && que.back().timer > 0)
	//			{
	//				dash_trigger = true;
	//			}
	//		}
	//	}



	//	if (Key[KEY_INPUT_D] > 0)
	//	{
	//		if (dash_trigger)
	//		{
	//			x_input[static_cast<int>(PAD::R_DASH)]++;
	//		}
	//		else
	//		{
	//			x_input[static_cast<int>(PAD::R_DASH)] = 0;
	//		}
	//		x_input[static_cast<int>(PAD::STICK_R)]++;
	//	}
	//	else
	//	{
	//		x_input[static_cast<int>(PAD::STICK_R)] = 0;
	//		x_input[static_cast<int>(PAD::R_DASH)] = 0;
	//	}
	//	if (x_input[static_cast<int>(PAD::STICK_R)] == 1)
	//	{
	//		if (!que.empty())
	//		{
	//			if (que.back().kind == scastI(PAD::STICK_R) && que.back().timer > 0)
	//			{
	//				dash_trigger = true;
	//			}
	//		}
	//	}


	//	if (x_input[static_cast<int>(PAD::STICK_R)] == 0 && x_input[static_cast<int>(PAD::STICK_L)] == 0)
	//	{
	//		dash_trigger = false;
	//	}



	//	if (Key[KEY_INPUT_T] > 0)
	//	{
	//		x_input[static_cast<int>(PAD::START)]++;
	//	}
	//	else
	//	{
	//		x_input[static_cast<int>(PAD::START)] = 0;
	//	}



	//	if (Key[KEY_INPUT_W] > 0)
	//	{
	//		x_input[scastI(PAD::STICK_U)]++;
	//	}
	//	else
	//	{
	//		x_input[scastI(PAD::STICK_U)] = 0;
	//	}



	//	if (Key[KEY_INPUT_S] > 0)
	//	{
	//		x_input[scastI(PAD::STICK_D)]++;
	//	}
	//	else
	//	{
	//		x_input[scastI(PAD::STICK_D)] = 0;
	//	}


	//	if (Key[KEY_INPUT_W] == 1 && Key[KEY_INPUT_D] > 0)
	//	{
	//		x_input[scastI(PAD::STICK_U)] = 0;
	//		x_input[scastI(PAD::STICK_R)] = 0;
	//		x_input[scastI(PAD::DOWN)] = 0;
	//		x_input[scastI(PAD::RIGHT)] = 0;
	//		x_input[scastI(PAD::STICK_RUp)]++;
	//	}
	//	else
	//	{
	//		x_input[scastI(PAD::STICK_RUp)] = 0;
	//	}


	//	if (Key[KEY_INPUT_W] == 1 && Key[KEY_INPUT_A] > 0)
	//	{
	//		x_input[scastI(PAD::STICK_U)] = 0;
	//		x_input[scastI(PAD::STICK_L)] = 0;
	//		x_input[scastI(PAD::DOWN)] = 0;
	//		x_input[scastI(PAD::LEFT)] = 0;
	//		x_input[scastI(PAD::STICK_LUp)]++;
	//	}
	//	else
	//	{
	//		x_input[scastI(PAD::STICK_LUp)] = 0;
	//	}



	//	if (Key[KEY_INPUT_S] > 0 && Key[KEY_INPUT_D] > 0)
	//	{
	//		x_input[scastI(PAD::STICK_D)] = 0;
	//		x_input[scastI(PAD::STICK_R)] = 0;
	//		x_input[scastI(PAD::DOWN)] = 0;
	//		x_input[scastI(PAD::RIGHT)] = 0;
	//		x_input[scastI(PAD::STICK_RDown)]++;
	//	}
	//	else
	//	{
	//		x_input[scastI(PAD::STICK_RDown)] = 0;
	//	}


	//	if (Key[KEY_INPUT_S] > 0 && Key[KEY_INPUT_A] > 0)
	//	{
	//		x_input[scastI(PAD::STICK_D)] = 0;
	//		x_input[scastI(PAD::STICK_L)] = 0;
	//		x_input[scastI(PAD::DOWN)] = 0;
	//		x_input[scastI(PAD::LEFT)] = 0;
	//		x_input[scastI(PAD::STICK_LDown)]++;
	//	}
	//	else
	//	{
	//		x_input[scastI(PAD::STICK_LDown)] = 0;
	//	}


	//	if (x_input[scastI(PAD::STICK_RDown)] == 1 || x_input[scastI(PAD::STICK_LDown)] == 1)
	//	{
	//		com_list.trigger = true;
	//		com_list.command_timer = 0;
	//	}

	//	if (com_list.trigger)
	//	{
	//		com_list.command_timer++;
	//	}

	//	/*if (Key[KEY_INPUT_O] > 0)
	//	{
	//		x_input[scastI(PAD::NONE1)]++;
	//	}
	//	else
	//	{
	//		x_input[scastI(PAD::NONE1)]=0;
	//	}*/
	//}

	InputSave();
	/*if (!Hitcheak::hit)
	{
		InputUpdate();
	}*/
}

void GamePad1::InputSave()
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
void GamePad1::InputUpdate()
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
