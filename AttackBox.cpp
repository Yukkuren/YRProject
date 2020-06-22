//#include"DxLib.h"
#include "AttackBox.h"
//#include"Common.h"
#include "PlayerBase.h"

void AttackBox::Init()
{
	timer = 0;
	center = YR_Vector3(0, 0);
	size = YR_Vector3(0, 0);
	start = false;
	fream = 0;
	later = 0;
	attack = false;
	fin = false;
	hit_ok = false;
	damege = 0;
	HB_timer = 0;
	hitback = YR_Vector3(0.0f, 0.0f);
	knockback = 0;
	knock_start = false;
	gaugeout = false;
	stealtimer = 20;
}

void AttackBox::Update(YR_Vector3 cent, YR_Vector3 range, int time, int frm, int late, float dage, int hitbackT, YR_Vector3 divhitback, int kind, float knock)
{
	if (!start)
	{
		timer = 0;
		timer = time;
		fream = frm;
		later = late;
		attack = false;
		fin = false;
		start = true;
		damege = dage;
		HB_timer = hitbackT;
		hitback = divhitback;
		type = kind;
		knockback = knock;
		knock_start = false;
	}
	center = cent;
	size = range;

	fream--;

	if (fream < 0)
	{
		attack = true;
		if (damege != 0.0f)
		{
			hit_ok = true;
		}
	}

	if (attack)
	{
		if (timer > 0)
		{

		}
		timer--;
		if (!hit_ok)
		{
			damege = 0;
			HB_timer = 0;
			hitback = YR_Vector3(0.0f, 0.0f);
		}
	}



	if (timer < 0)
	{
		fin = true;
		attack = false;
		timer = 0;
		damege = 0;
		hit_ok = false;
		HB_timer = 0;
		hitback = YR_Vector3(0.0f, 0.0f);
		knockback = 0;
	}
}

void AttackBox::UpdateMissile(YR_Vector3 cent, YR_Vector3 range, int time, float dage, int hitbackT, YR_Vector3 divhitback, int kind)
{
	if (!start)
	{
		timer = 0;
		timer = time;
		attack = false;
		fin = false;
		start = true;
		damege = dage;
		HB_timer = hitbackT;
		hitback = divhitback;
		type = kind;
		knock_start = false;
	}
	center = cent;
	size = range;


	attack = true;
	if (damege != 0.0f)
	{
		hit_ok = true;
	}

	if (attack)
	{
		if (timer > 0)
		{

		}
		timer--;
		if (!hit_ok)
		{
			damege = 0;
			HB_timer = 0;
			hitback = YR_Vector3(0.0f, 0.0f);
		}
	}



	if (timer < 0)
	{
		fin = true;
		attack = false;
		timer = 0;
		damege = 0;
		hit_ok = false;
		HB_timer = 0;
		hitback = YR_Vector3(0.0f, 0.0f);
		knockback = 0;
	}
}


void AttackBox::UpdateMissile(YR_Vector3 cent, YR_Vector3 range, int time, float dage, int hitbackT, YR_Vector3 divhitback, int kind, int gauge)
{
	if (!start)
	{
		timer = 0;
		timer = time;
		attack = false;
		fin = false;
		start = true;
		damege = dage;
		HB_timer = hitbackT;
		hitback = divhitback;
		type = kind;
		knock_start = false;
		gaugeout = true;
	}
	center = cent;
	size = range;


	attack = true;
	if (damege != 0.0f)
	{
		hit_ok = true;
	}

	if (attack)
	{
		if (timer > 0)
		{

		}
		timer--;
		if (!hit_ok)
		{
			damege = 0;
			HB_timer = 0;
			hitback = YR_Vector3(0.0f, 0.0f);
		}
	}



	if (timer < 0)
	{
		fin = true;
		attack = false;
		timer = 0;
		damege = 0;
		hit_ok = false;
		HB_timer = 0;
		hitback = YR_Vector3(0.0f, 0.0f);
		knockback = 0;
		gaugeout = false;
	}
}

bool AttackBox::Update(YR_Vector3 cent, YR_Vector3 range, int time, int frm, int late, float dage, int hitbackT, YR_Vector3 divhitback, int kind, float knock, bool fin)
{
	if (!start)
	{
		timer = 0;
		timer = time;
		fream = frm;
		later = late;
		attack = false;
		this->fin = false;
		start = true;
		damege = dage;
		HB_timer = hitbackT;
		hitback = divhitback;
		type = kind;
		knockback = knock;
		knock_start = false;
		stealtimer = 20;
	}
	center = cent;
	size = range;

	fream--;

	if (fream < 0)
	{
		attack = true;
		if (damege != 0.0f)
		{
			hit_ok = true;
		}
	}

	if (attack)
	{
		if (timer > 0)
		{

		}
		timer--;
		if (!hit_ok)
		{
			damege = 0;
			HB_timer = 0;
			hitback = YR_Vector3(0.0f, 0.0f);
		}
	}



	if (timer < 0)
	{
		attack = false;
		timer = 0;
		damege = 0;
		hit_ok = false;
		HB_timer = 0;
		hitback = YR_Vector3(0.0f, 0.0f);
		knockback = 0;
		return true;
	}

	return false;
}

void AttackBox::Update(YR_Vector3 cent, YR_Vector3 range, int time, int frm, int late, float dage, int hitbackT, YR_Vector3 divhitback, int kind, float knock, int gauge)
{
	if (!start)
	{
		timer = 0;
		timer = time;
		fream = frm;
		later = late;
		attack = false;
		fin = false;
		start = true;
		damege = dage;
		HB_timer = hitbackT;
		hitback = divhitback;
		type = kind;
		knockback = knock;
		knock_start = false;
		gaugeout = true;
	}
	center = cent;
	size = range;

	fream--;

	if (fream < 0)
	{
		attack = true;
		if (damege != 0.0f)
		{
			hit_ok = true;
		}
	}

	if (attack)
	{
		if (timer > 0)
		{

		}
		timer--;
		if (!hit_ok)
		{
			damege = 0;
			HB_timer = 0;
			hitback = YR_Vector3(0.0f, 0.0f);
		}
	}



	if (timer < 0)
	{
		fin = true;
		attack = false;
		timer = 0;
		damege = 0;
		hit_ok = false;
		HB_timer = 0;
		hitback = YR_Vector3(0.0f, 0.0f);
		knockback = 0;
		gaugeout = false;
	}
}

void AttackBox::Draw()
{
	/*SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBoxAA(
		center.x - size.x - camera.x,
		center.y - size.y - camera.y,
		center.x + size.x - camera.x,
		center.y + size.y - camera.y,
		GetColor(255, 0, 0),
		TRUE
	);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);*/
}