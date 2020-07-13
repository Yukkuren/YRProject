//#include"DxLib.h"
#include "AttackBox.h"
//#include"Common.h"
#include "PlayerBase.h"
#include "framework.h"

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

void AttackBox::Update(
	YR_Vector3 cent, 
	YR_Vector3 range, 
	float time, float frm, float late, float dage, float hitbackT, YR_Vector3 divhitback, int kind, float knock, float elapsed_time)
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

	fream-=elapsed_time;

	if (fream < 0.0f)
	{
		attack = true;
		if (damege != 0.0f)
		{
			hit_ok = true;
		}
	}

	if (attack)
	{
		if (timer > 0.0f)
		{

		}
		timer -= elapsed_time;
		if (!hit_ok)
		{
			damege = 0.0f;
			HB_timer = 0.0f;
			hitback = YR_Vector3(0.0f, 0.0f);
		}
	}



	if (timer < 0.0f)
	{
		fin = true;
		attack = false;
		timer = 0.0f;
		damege = 0.0f;
		hit_ok = false;
		HB_timer = 0.0f;
		hitback = YR_Vector3(0.0f, 0.0f);
		knockback = 0.0f;
	}
}

void AttackBox::UpdateMissile(
	YR_Vector3 cent, YR_Vector3 range, float time, float dage, float hitbackT, YR_Vector3 divhitback, int kind, float elapsed_time)
{
	if (!start)
	{
		timer = 0.0f;
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
		if (timer > 0.0f)
		{

		}
		timer -= elapsed_time;
		if (!hit_ok)
		{
			damege = 0.0f;
			HB_timer = 0.0f;
			hitback = YR_Vector3(0.0f, 0.0f);
		}
	}



	if (timer < 0.0f)
	{
		fin = true;
		attack = false;
		timer = 0.0f;
		damege = 0.0f;
		hit_ok = false;
		HB_timer = 0.0f;
		hitback = YR_Vector3(0.0f, 0.0f);
		knockback = 0.0f;
	}
}


void AttackBox::UpdateMissile(
	YR_Vector3 cent, YR_Vector3 range, float time, float dage, float hitbackT, YR_Vector3 divhitback, int kind, int gauge, float elapsed_time)
{
	if (!start)
	{
		timer = 0.0f;
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
		if (timer > 0.0f)
		{

		}
		timer -= elapsed_time;
		if (!hit_ok)
		{
			damege = 0.0f;
			HB_timer = 0.0f;
			hitback = YR_Vector3(0.0f, 0.0f);
		}
	}



	if (timer < 0.0f)
	{
		fin = true;
		attack = false;
		timer = 0.0f;
		damege = 0.0f;
		hit_ok = false;
		HB_timer = 0.0f;
		hitback = YR_Vector3(0.0f, 0.0f);
		knockback = 0.0f;
		gaugeout = false;
	}
}

bool AttackBox::Update(
	YR_Vector3 cent,
	YR_Vector3 range,
	float time, float frm, float late, float dage, float hitbackT, YR_Vector3 divhitback, int kind, float knock, bool fin, float elapsed_time)
{
	if (!start)
	{
		timer = 0.0f;
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
		stealtimer = 20.0f;
	}
	center = cent;
	size = range;

	fream -= elapsed_time;

	if (fream < 0.0f)
	{
		attack = true;
		if (damege != 0.0f)
		{
			hit_ok = true;
		}
	}

	if (attack)
	{
		if (timer > 0.0f)
		{

		}
		timer -= elapsed_time;
		if (!hit_ok)
		{
			damege = 0.0f;
			HB_timer = 0.0f;
			hitback = YR_Vector3(0.0f, 0.0f);
		}
	}



	if (timer < 0.0f)
	{
		attack = false;
		timer = 0.0f;
		damege = 0.0f;
		hit_ok = false;
		HB_timer = 0.0f;
		hitback = YR_Vector3(0.0f, 0.0f);
		knockback = 0.0f;
		return true;
	}

	return false;
}

void AttackBox::Update(
	YR_Vector3 cent,
	YR_Vector3 range,
	float time, float frm, float late, float dage, float hitbackT, YR_Vector3 divhitback, int kind, float knock, int gauge, float elapsed_time)
{
	if (!start)
	{
		timer = 0.0f;
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

	fream -= elapsed_time;

	if (fream < 0.0f)
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
		timer -= elapsed_time;
		if (!hit_ok)
		{
			damege = 0.0f;
			HB_timer = 0.0f;
			hitback = YR_Vector3(0.0f, 0.0f);
		}
	}



	if (timer < 0.0f)
	{
		fin = true;
		attack = false;
		timer = 0.0f;
		damege = 0.0f;
		hit_ok = false;
		HB_timer = 0.0f;
		hitback = YR_Vector3(0.0f, 0.0f);
		knockback = 0.0f;
		gaugeout = false;
	}
}

void AttackBox::Draw(YRShader* shader,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color)
{
	FRAMEWORK.scenegame.geo->render(
		shader,
		center.GetDXFLOAT3(),
		DirectX::XMFLOAT3(size.x,size.y,5.0f),
		DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		view,
		projection,
		DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f)
	);
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