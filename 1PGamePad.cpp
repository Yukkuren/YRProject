#include	"Key.h"
#include	"YRGamePad.h"
//#include	"Common.h"
#include	"HitCheak.h"

InputListor::InputListor(int n)
{
	kind = n;
}


void GamePad1::Init()
{
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

void GamePad1::Update(float elapsed_time)
{
#pragma region hei

#pragma endregion
	//X_Input取得
	if (XInputGetState(0, &input) == ERROR_SUCCESS)
	{
		for (int i = 0; i < scastI(PAD::BUTTOM_END); i++)
		{
			if (input.Gamepad.wButtons & X_INPUT_BOTTUN[i])
			{
				x_input[i]++;
			}
			else
			{
				x_input[i] = 0;
			}
		}

		//左トリガー
		if (input.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			x_input[scastI(PAD::L_TRIGGER)]++;
		}
		else
		{
			x_input[scastI(PAD::L_TRIGGER)] = 0;
		}

		//右トリガー
		if (input.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			x_input[scastI(PAD::R_TRIGGER)]++;
		}
		else
		{
			x_input[scastI(PAD::R_TRIGGER)] = 0;
		}


		//左スティック左倒し
		if (input.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || x_input[scastI(PAD::LEFT)]>0)
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

		//左スティック右倒し
		if (input.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || x_input[scastI(PAD::RIGHT)] > 0)
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

		if (x_input[scastI(PAD::STICK_R)] == 0 && x_input[scastI(PAD::STICK_L)] == 0)
		{
			dash_trigger = false;
		}


		//左スティック上倒し
		if (input.Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || x_input[scastI(PAD::UP)] > 0)
		{
			x_input[scastI(PAD::STICK_U)]++;
		}
		else
		{
			x_input[scastI(PAD::STICK_U)] = 0;
		}

		if (x_input[scastI(PAD::STICK_U)] == 1)
		{
			if (!que.empty())
			{
				if (que.back().timer > 0)
				{
					if (que.back().kind == scastI(PAD::STICK_D) || que.back().kind == scastI(PAD::STICK_RDown) || que.back().kind == scastI(PAD::STICK_LDown))
					{
						high_trigger = true;
						x_input[scastI(PAD::STICK_U)] = 0;
					}
				}
			}
		}
		else
		{
			high_trigger = false;
		}

		if (high_trigger)
		{
			x_input[scastI(PAD::HIGH_UP)]++;
		}
		else
		{
			x_input[scastI(PAD::HIGH_UP)] = 0;
		}

		//左スティック下倒し
		if (input.Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || x_input[scastI(PAD::DOWN)]>0)
		{
			x_input[scastI(PAD::STICK_D)]++;
		}
		else
		{
			x_input[scastI(PAD::STICK_D)] = 0;
		}


		//左スティック斜め右上倒し
		if (x_input[scastI(PAD::STICK_U)]==1
			&&
			x_input[scastI(PAD::STICK_R)] > 0)
		{
			/*x_input[scastI(PAD::STICK_U)] = 0;
			x_input[scastI(PAD::STICK_R)] = 0;
			x_input[scastI(PAD::DOWN)] = 0;
			x_input[scastI(PAD::RIGHT)] = 0;*/
			x_input[scastI(PAD::STICK_RUp)]++;
		}
		else
		{
			x_input[scastI(PAD::STICK_RUp)] = 0;
		}


		//左スティック斜め左上倒し
		if (x_input[scastI(PAD::STICK_U)] == 1
			&&
			x_input[scastI(PAD::STICK_L)] > 0)
		{
			/*x_input[scastI(PAD::STICK_U)] = 0;
			x_input[scastI(PAD::STICK_L)] = 0;
			x_input[scastI(PAD::DOWN)] = 0;
			x_input[scastI(PAD::LEFT)] = 0;*/
			x_input[scastI(PAD::STICK_LUp)]++;
		}
		else
		{
			x_input[scastI(PAD::STICK_LUp)] = 0;
		}


		//左スティック斜め右下倒し
		if (x_input[scastI(PAD::STICK_D)] > 0
			&&
			x_input[scastI(PAD::STICK_R)] > 0)
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


		//左スティック斜め左下倒し
		if (x_input[scastI(PAD::STICK_D)] > 0
			&&
			x_input[scastI(PAD::STICK_L)] > 0)
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
			com_list.command_timer+=elapsed_time;
		}
	}
	else
	{
		//キーボード入力(コントローラーなしのみ)

		if (pKeyState.hflg > 0)
		{
			x_input[static_cast<int>(PAD::X)]++;
		}
		else
		{
			x_input[static_cast<int>(PAD::X)] = 0;
		}



		if (pKeyState.jflg > 0)
		{
			x_input[static_cast<int>(PAD::Y)]++;
		}
		else
		{
			x_input[static_cast<int>(PAD::Y)] = 0;
		}



		if (pKeyState.kflg > 0)
		{
			x_input[static_cast<int>(PAD::B)]++;
		}
		else
		{
			x_input[static_cast<int>(PAD::B)] = 0;
		}



		if (pKeyState.lflg > 0)
		{
			x_input[static_cast<int>(PAD::A)]++;
		}
		else
		{
			x_input[static_cast<int>(PAD::A)] = 0;
		}


		if (pKeyState.qflg > 0)
		{
			x_input[static_cast<int>(PAD::L_TRIGGER)]++;
		}
		else
		{
			x_input[static_cast<int>(PAD::L_TRIGGER)] = 0;
		}


		if (pKeyState.eflg > 0)
		{
			x_input[static_cast<int>(PAD::R_TRIGGER)]++;
		}
		else
		{
			x_input[static_cast<int>(PAD::R_TRIGGER)] = 0;
		}




		if (pKeyState.aflg > 0)
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



		if (pKeyState.dflg > 0)
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



		if (pKeyState.tflg > 0)
		{
			x_input[static_cast<int>(PAD::START)]++;
		}
		else
		{
			x_input[static_cast<int>(PAD::START)] = 0;
		}

		if (pKeyState.yflg > 0)
		{
			x_input[static_cast<int>(PAD::SELECT)]++;
		}
		else
		{
			x_input[static_cast<int>(PAD::SELECT)] = 0;
		}



		if (pKeyState.wflg > 0)
		{
			x_input[scastI(PAD::STICK_U)]++;
		}
		else
		{
			x_input[scastI(PAD::STICK_U)] = 0;
		}
		if (x_input[scastI(PAD::STICK_U)] == 1)
		{
			if (!que.empty())
			{
				if (que.back().timer > 0)
				{
					if (que.back().kind == scastI(PAD::STICK_D) || que.back().kind == scastI(PAD::STICK_RDown) || que.back().kind == scastI(PAD::STICK_LDown))
					{
						high_trigger = true;
						x_input[scastI(PAD::STICK_U)] = 0;
					}
				}
			}
		}
		else
		{
			high_trigger = false;
		}

		if (high_trigger)
		{
			x_input[scastI(PAD::HIGH_UP)]++;
		}
		else
		{
			x_input[scastI(PAD::HIGH_UP)] = 0;
		}



		if (pKeyState.sflg > 0)
		{
			x_input[scastI(PAD::STICK_D)]++;
		}
		else
		{
			x_input[scastI(PAD::STICK_D)] = 0;
		}


		if (pKeyState.wflg == 1 && pKeyState.dflg > 0)
		{
			/*x_input[scastI(PAD::STICK_U)] = 0;
			x_input[scastI(PAD::STICK_R)] = 0;
			x_input[scastI(PAD::DOWN)] = 0;
			x_input[scastI(PAD::RIGHT)] = 0;*/
			x_input[scastI(PAD::STICK_RUp)]++;
		}
		else
		{
			x_input[scastI(PAD::STICK_RUp)] = 0;
		}


		if (pKeyState.wflg == 1 && pKeyState.aflg > 0)
		{
			/*x_input[scastI(PAD::STICK_U)] = 0;
			x_input[scastI(PAD::STICK_L)] = 0;
			x_input[scastI(PAD::DOWN)] = 0;
			x_input[scastI(PAD::LEFT)] = 0;*/
			x_input[scastI(PAD::STICK_LUp)]++;
		}
		else
		{
			x_input[scastI(PAD::STICK_LUp)] = 0;
		}



		if (pKeyState.sflg > 0 && pKeyState.dflg > 0)
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


		if (pKeyState.sflg > 0 && pKeyState.aflg > 0)
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
			com_list.command_timer += elapsed_time;
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
		InputUpdate(elapsed_time);
	}
}

void GamePad1::InputSave()
{
	for (int i = 0; i < scastI(PAD::PAD_END); i++)
	{
		if (x_input[i] == 1)
		{
			que.push_back(InputListor(i));
			que.back().timer = 0.2f;
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
			if (que.back().timer > 0.0f)
			{
				x_input[que.back().kind] = 1;
			}
		}
	}
}
void GamePad1::InputUpdate(float elapsed_time)
{
	if (!que.empty())
	{
		for (auto& it : que)
		{
			it.timer -= elapsed_time;
			if (it.timer < 0.0f)
			{
				it.timer = 0.0f;
			}
		}
	}
}
