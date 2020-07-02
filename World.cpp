#include"World.h"
//#include"Common.h"

//Vector2 camera = { world_max_x / 2 - (world_max_x / 2 / 2), world_max_y / 2 };

float Limit::Left_max = -100.0f;
float Limit::Right_max = 100.0f;

void Limit::Set(YR_Vector3 p1, YR_Vector3 p2)
{
	if (p1.x > p2.x)
	{
		float distance = (p1.x - p2.x) / 2;
		float correction = p2.x + distance;
		//カメラの調整
		/*camera.x = correction - (SCREEN_WIDTH / 2);
		if (camera.x < 0)
		{
			camera.x = 0;
		}
		if (camera.x > world_max_x - SCREEN_WIDTH)
		{
			camera.x = world_max_x - SCREEN_WIDTH;
		}*/
		//Left_max = camera.x;
		//Right_max = camera.x + SCREEN_WIDTH;
	}
	if (p1.x < p2.x)
	{
		float distance = (p2.x - p1.x) / 2;
		float correction = p1.x + distance;
		/*camera.x = correction - (SCREEN_WIDTH / 2);
		if (camera.x < 0)
		{
			camera.x = 0;
		}
		if (camera.x > world_max_x - SCREEN_WIDTH)
		{
			camera.x = world_max_x - SCREEN_WIDTH;
		}
		Left_max = camera.x;
		Right_max = camera.x + SCREEN_WIDTH;*/
	}

	if (p1.x == p2.x)
	{
		//float distance = (p2 - p1) / 2;
		float correction = p1.x;
		/*camera.x = correction - (SCREEN_WIDTH / 2);
		if (camera.x < 0)
		{
			camera.x = 0;
		}
		if (camera.x > world_max_x - SCREEN_WIDTH)
		{
			camera.x = world_max_x - SCREEN_WIDTH;
		}
		Left_max = camera.x;
		Right_max = camera.x + SCREEN_WIDTH;*/
	}



	//y
	if (p1.y > p2.y)
	{
		float distance = (p1.y - p2.y) / 2;
		float correction = p2.y + distance;
		/*camera.y = correction - (SCREEN_HEIGHT / 2);
		if (camera.y < 0)
		{
			camera.y = 0;
		}
		if (camera.y > world_max_y - SCREEN_HEIGHT)
		{
			camera.y = world_max_y - SCREEN_HEIGHT;
		}*/
	}
	if (p1.y < p2.y)
	{
		float distance = (p2.y - p1.y) / 2;
		float correction = p1.y + distance;
		/*camera.y = correction - (SCREEN_HEIGHT / 2);
		if (camera.y < 0)
		{
			camera.y = 0;
		}
		if (camera.y > world_max_y - SCREEN_HEIGHT)
		{
			camera.y = world_max_y - SCREEN_HEIGHT;
		}*/
	}

	if (p1.y == p2.y)
	{
		//float distance = (p2 - p1) / 2;
		float correction = p1.y;
		/*camera.y = correction - (SCREEN_HEIGHT / 2);
		if (camera.y < 0)
		{
			camera.y = 0;
		}
		if (camera.y > world_max_y - SCREEN_HEIGHT)
		{
			camera.y = world_max_y - SCREEN_HEIGHT;
		}*/
	}

}

void Limit::Stop(float& p1, float& p2)
{
	//プレイヤーの移動制限
	if (p1 > Right_max)
	{
		p1 = Right_max;
	}

	if (p1 < Left_max)
	{
		p1 = Left_max;
	}

	if (p2 > Right_max)
	{
		p2 = Right_max;
	}

	if (p2 < Left_max)
	{
		p2 = Left_max;
	}
}