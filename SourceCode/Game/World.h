#ifndef _WORLD_H_
#define _WORLD_H_

#include	"../YR_VectorMaster.h"

//-----------------------------------------------------------------
//			Limitクラス
//-----------------------------------------------------------------
//・キャラの移動制限を処理するクラス
//・キャラの位置からカメラの座標を算出する機能もはたしている
//-----------------------------------------------------------------

const float limit_size = 21.0f;		//移動量の制限値
const float adjustment_eye = 8.0f;	//カメラのズーム距離補正値
const float adjustment_y = 4.0f;	//カメラの高さ距離補正値
//const float adjustment_y = 6.0f;	//カメラのY方向の補正値

enum class P1P2_TOUCH : int
{
	NONE = 0,
	P1,
	P2,
};

class Limit
{
public:
	static float adjust_wall;		//片方が壁に張り付いている時にもう片方に掛ける補正値
	static float Left_max;			//スクロール左端
	static float Right_max;			//スクロール右端

	static P1P2_TOUCH	p1p2_first_touch;	//先に画面端に行っていたプレイヤーを保存する

	static YR_Vector3 Set(YR_Vector3 p1, YR_Vector3 p2,YR_Vector3 start_eye);
	static void Stop(float& p1, float& p2);
	static void First_Camera_set(YR_Vector3 camera);

	static YR_Vector3 now_Scene_camera;
};

static const float world_max_x = 130;
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


