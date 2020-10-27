#ifndef _TRACKING_H_
#define _TRACKING_H_

#include	"YR_VectorMaster.h"

constexpr float track_wait_time = 0.8f;

class Tracking
{
public:
	YR_Vector3		rival_Pos;		//‘Îí‘Šè‚ÌÀ•W(SceneGame‚Å’l‚ğ“ü‚ê‚é)

	void Init()
	{
		rival_Pos = YR_Vector3(0.0f, 0.0f, 0.0f);
	};

	YR_Vector3	Veccalculate(YR_Vector3 pl_Pos)
	{
		YR_Vector3	track_vec;
		track_vec = rival_Pos - pl_Pos;
		track_vec.Normalize();

		return track_vec;
	};
};

#endif // !_TRACKING_H_
