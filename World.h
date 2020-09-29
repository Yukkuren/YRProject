#ifndef _WORLD_H_
#define _WORLD_H_

#include	"YR_VECTOR3.h"

const float limit_size = 21.0f;		//左右の移動量の制限値
const float adjustment_eye = 8.0f;	//カメラのズーム距離補正値
//const float adjustment_y = 6.0f;	//カメラのY方向の補正値

class Limit
{
public:
	static float Left_max;
	static float Right_max;
	static YR_Vector3 Set(YR_Vector3 p1, YR_Vector3 p2,YR_Vector3 start_eye);
	static void Stop(float& p1, float& p2);
	static void First_Camera_set(YR_Vector3 camera);

	static YR_Vector3 now_Scene_camera;
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


