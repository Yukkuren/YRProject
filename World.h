#ifndef _WORLD_H_
#define _WORLD_H_

#include	"YR_VECTOR3.h"

class Limit
{
public:
	static float Left_max;
	static float Right_max;
	static void Set(YR_Vector3 p1, YR_Vector3 p2);
	static void Stop(float& p1, float& p2);
};

static const float world_max_x = 3840;
static const float world_max_y = 2160;

//extern YR_Vector3 camera;

//class ScreenLimit
//{
//public:
//	static float Left_max;
//	static float Right_max;
//	static void Set(float p1,float p2);
//	//void Stop(float &p1, float &p2);
//}

#endif // !_WORLD_H_


