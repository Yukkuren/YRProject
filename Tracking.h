#ifndef _TRACKING_H_
#define _TRACKING_H_

#include	"YR_Vector3.h"

class Tracking
{
private:
	bool		fin;
public:
	YR_Vector3		rival_Pos;
	YR_Vector3		track_vec;
	int				timer;
	bool			attack;
	bool			start = false;

	void Init()
	{
		attack = false;
		fin = false;
		start = true;
		timer = 0;
	};

	YR_Vector3	Veccalculate(YR_Vector3 pl_Pos, float division)
	{
		timer++;
		track_vec = { 0,-0.5f };
		if (timer < 20)
		{
			return track_vec;
		}
		track_vec = rival_Pos - pl_Pos;
		track_vec.Normalize();
		track_vec *= 70.0f;

		//¶‚É‚¢‚é
		if (division > 0)
		{
			if (pl_Pos.x + track_vec.x + 150 >= rival_Pos.x)
			{
				if (pl_Pos.y >= rival_Pos.y)
				{
					if (pl_Pos.y + track_vec.y - 150 <= rival_Pos.y)
					{
						fin = true;
					}
				}
				if (pl_Pos.y <= rival_Pos.y)
				{
					if (pl_Pos.y + track_vec.y + 150 >= rival_Pos.y)
					{
						fin = true;
					}
				}
			}
		}
		//‰E‚É‚¢‚é
		else
		{
			if (pl_Pos.x + track_vec.x - 150 <= rival_Pos.x)
			{
				if (pl_Pos.y > rival_Pos.y)
				{
					if (pl_Pos.y + track_vec.y - 150 <= rival_Pos.y)
					{
						fin = true;
					}
				}
				else
				{
					if (pl_Pos.y + track_vec.y + 150 >= rival_Pos.y)
					{
						fin = true;
					}
				}
			}
		}

		if (fin)
		{
			attack = true;
			start = false;
			track_vec = { 0,0 };
		}

		return track_vec;
	};
};

#endif // !_TRACKING_H_
