#ifndef _TRACKING_H_
#define _TRACKING_H_

#include	"../YR_VectorMaster.h"
//#include	"../Chara/PlayerBase/PlayerBase.h"

constexpr float track_wait_time = 0.8f;
constexpr float track_adjust_up = 5.0f;
constexpr float track_adjust_x = 2.0f;

//----------------------------------------------------------------
//				�z�[�~���O�_�b�V���N���X
//----------------------------------------------------------------
//�E����̈ʒu�Ȃǂ�ۑ����A�z�[�~���O�_�b�V�����Ɏg�p����l���v�Z����
//----------------------------------------------------------------

class Tracking
{
public:
	YR_Vector3		rival_Pos;		//�ΐ푊��̍��W(SceneGame�Œl������)

	void Init()
	{
		rival_Pos = YR_Vector3(0.0f, 0.0f, 0.0f);
	};

	YR_Vector3	Veccalculate(YR_Vector3 pl_Pos, float decision)
	{
		YR_Vector3	track_vec;
		YR_Vector3 rival = rival_Pos;
		rival.y += track_adjust_up;
		rival.x += (track_adjust_x * (-decision));
		track_vec = rival - pl_Pos;
		track_vec.Normalize();

		return track_vec;
	};
};

#endif // !_TRACKING_H_
