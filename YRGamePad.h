#ifndef _GAMEPAD_H_
#define _GAMEPAD_H_

#include <vector>
#include <memory>
#include<array>
#include <list>
#include"CoList.h"


class InputListor
{
public:
	InputListor(int n);
	int kind;
	int timer = 0;
};

enum class PAD :int
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	START,
	SELECT,
	L_ON,
	R_ON,
	LB,
	RB,
	NONE1,
	NONE2,
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
	//Å´å„Ç≈protectedÇ…à⁄Ç∑
	//XINPUT_STATE input;
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
	virtual void Update() = 0;
	virtual void InputSave() = 0;
	virtual void InputUpdate() = 0;
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
	void Update();
	void InputSave();
	void InputUpdate();
	void UnInit();
};

class GamePad2 : public GamepadBase
{
public:
	void Init();
	void Update();
	void InputSave();
	void InputUpdate();
	void UnInit();
};

//#define PGAMEPAD	(Gamepad::GetInstance())


#endif // !_GAMEPAD_H_