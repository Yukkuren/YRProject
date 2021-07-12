#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#include <vector>
#include <memory>
#include<array>
#include <list>
#include"CoList.h"
#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib,"xinput.lib")


//-----------------------------------------------------------------
//			コントローラー規定クラス
//-----------------------------------------------------------------

constexpr int X_Input_LEFT_DEADZONE = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE + 3000;

class InputListor
{
public:
	InputListor(int n);
	int kind;
	float timer = 0.0f;
};

constexpr std::array<unsigned int, 14>	X_INPUT_BOTTUN =
{
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y,
};

enum class PAD :int
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	START,
	SELECT,
	L_ON, //L3
	R_ON, //R3
	LB,
	RB,
	A,
	B,
	X,
	Y,
	BUTTOM_END,
	L_TRIGGER,
	R_TRIGGER,
	STICK_R,
	STICK_L,
	STICK_U,
	STICK_D,
	STICK_RDown,
	STICK_LDown,
	STICK_RUp,
	STICK_LUp,
	HIGH_UP,
	HIGH_UP_R,
	HIGH_UP_L,
	R_DASH,
	L_DASH,
	PAD_END
};

class GamepadBase
{
protected:
	/*Gamepad()
	{

	}*/
	//↓後でprotectedに移す
	XINPUT_STATE input;
	int Xbuf;
	int Ybuf;
	const char trigger_put = 50;
	const int deadzone = 10;
	const int deadzoneY = 300;

	bool stickRD;
	bool bottomRD;
	bool stickLD;
	bool bottomLD;

	bool stickRU;
	bool bottomRU;
	bool stickLU;
	bool bottomLU;

public:


	bool dash_trigger;
	bool high_trigger;
	bool pre_input;
	int* lstick;
	std::array<int, static_cast<int>(PAD::PAD_END)> x_input = { 0 };
	std::list<InputListor>que;
	std::array<int, 10>input_history = { -1 };
	CommandList com_list;
	virtual void Init() = 0;
	virtual void Update(float elapsed_time) = 0;
	virtual void InputSave() = 0;
	virtual void InputUpdate(float elapsed_time) = 0;
	virtual void UnInit() = 0;
	/*static Gamepad *GetInstance()
	{
		static Gamepad inst;
		return &inst;
	}*/
};

class GamePad1 :public GamepadBase
{
public:
	void Init();
	void Update(float elapsed_time);
	void InputSave();
	void InputUpdate(float elapsed_time);
	void UnInit();
};

class GamePad2 : public GamepadBase
{
public:
	void Init();
	bool GetX_INPUT_ON();
	void Update(float elapsed_time);
	void InputSave();
	void InputUpdate(float elapsed_time);
	void UnInit();
};

//#define PGAMEPAD	(Gamepad::GetInstance())


#endif // !_GAMEPAD_H_