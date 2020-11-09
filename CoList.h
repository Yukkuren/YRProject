#ifndef _COLIST_H_
#define _COLIST_H_

#include<array>
//#include"Common.h"

constexpr float TIMER_LIMIT = 0.5;
constexpr float timer_max_command = 1.0f;		//この数以下ならコマンドタイマーを増やす
#define		scastI				static_cast<int>

enum class Command : int
{
	NOCOMMAND = 0,
	RHURF,
	LHURF,
	END,
};

class CommandList
{
private:
	int						lastinput;
	int						command_start;
	std::array<int, 10>		histry = { 0 };
	Command					state;
	int						numI;
public:
	bool trigger = false;
	float command_timer = 0.0f;

	Command CommandCheack(int last, int* hist, float distans);
	void Reset();
};



#endif // !_COLIST_H_


