#include"HitBox.h"
#include"PlayerBase.h"

void HitBox::Init(YR_Vector3 cent, YR_Vector3 range)
{
	center = cent;
	size = range;
	hit = false;
	damege = 0.0f;
	//hitother = false;
	timer = 0;
	hitback = { 0.0f, 0.0f };
	guard = NOGUARD;
	guard_ok = false;
	steal = false;
	steal_timer = 0;
}

void HitBox::Update(YR_Vector3 cent, YR_Vector3 range)
{
	center = cent;
	size = range;
	//if (DEBUG_MODE)Draw();
	if (timer > 0)
	{
		timer--;
		if (hitback.x > 0)
		{
			hitback.x--;
			if (hitback.x < 0)
			{
				hitback.x = 0;
			}
		}
		if (hitback.x < 0)
		{
			hitback.x++;
			if (hitback.x > 0)
			{
				hitback.x = 0;
			}
		}
		if (hitback.y > -50)
		{
			hitback.y--;
		}
	}
	if (damege > 0)
	{

	}
	else
	{
		hit = false;
	}
}

void HitBox::Draw()
{
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	if (guard == INVINCIBLE)
	{
		/*DrawBoxAA(
			center.x - size.x - camera.x,
			center.y - size.y - camera.y,
			center.x + size.x - camera.x,
			center.y + size.y - camera.y,
			GetColor(255, 200, 0),
			TRUE);*/
	}
	else
	{
		if (timer == 0)
		{
			/*DrawBoxAA(
				center.x - size.x - camera.x,
				center.y - size.y - camera.y,
				center.x + size.x - camera.x,
				center.y + size.y - camera.y,
				GetColor(0, 0, 200),
				TRUE);*/
		}
		else
		{
			/*DrawBoxAA(
				center.x - size.x - camera.x,
				center.y - size.y - camera.y,
				center.x + size.x - camera.x,
				center.y + size.y - camera.y,
				GetColor(0, 200, 0),
				TRUE
			);*/
		}
	}
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

}