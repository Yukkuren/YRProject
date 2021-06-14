#ifndef _WORLD_H_
#define _WORLD_H_

#include	"../YR_VectorMaster.h"

//-----------------------------------------------------------------
//			Limit�N���X
//-----------------------------------------------------------------
//�E�L�����̈ړ���������������N���X
//�E�L�����̈ʒu����J�����̍��W���Z�o����@�\���͂����Ă���
//-----------------------------------------------------------------

const float limit_size = 21.0f;		//�ړ��ʂ̐����l
const float adjustment_eye = 8.0f;	//�J�����̃Y�[�������␳�l
const float adjustment_y = 4.0f;	//�J�����̍��������␳�l
//const float adjustment_y = 6.0f;	//�J������Y�����̕␳�l

enum class P1P2_TOUCH : int
{
	NONE = 0,
	P1,
	P2,
};

class Limit
{
public:
	static float adjust_wall;		//�Е����ǂɒ���t���Ă��鎞�ɂ����Е��Ɋ|����␳�l
	static float Left_max;			//�X�N���[�����[
	static float Right_max;			//�X�N���[���E�[

	static P1P2_TOUCH	p1p2_first_touch;	//��ɉ�ʒ[�ɍs���Ă����v���C���[��ۑ�����

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


