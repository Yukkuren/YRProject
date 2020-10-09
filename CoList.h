#ifndef _COLIST_H_
#define _COLIST_H_

#include<array>
//#include"Common.h"

#define TIMER_LIMIT		(0.5)
#define		scastI				static_cast<int>

class CommandList
{
private:
	int lastinput;
	int command_start;
	std::array<int, 10> histry = { 0 };
	int state;
	int numI;
public:
	bool trigger = false;
	float command_timer = 0;

	int CommandCheack(int last, int* hist, float distans);
	void Reset();

	enum
	{
		NOCOMMAND = 0,
		RHURF,
		LHURF,
		END,
	};
};

#endif // !_COLIST_H_


