//#include"DxLib.h"
#include "AttackBox.h"
//#include"Common.h"
#include "PlayerBase.h"
#include "framework.h"

void AttackBox::Init(AttackParameter& param, float rightOrleft)
{
	attack = true;
	fin = false;
	hit_ok = false;
	knock_start = false;
	plus = false;
	this->rightOrleft = rightOrleft;
	speed = YR_Vector3(0.0f, 0.0f, 0.0f);
	this->plus_speed = YR_Vector3(0.0f, 0.0f, 0.0f);
	parameter = param;
	if (parameter.damege != 0.0f)
	{
		hit_ok = true;
	}
}

void AttackBox::Init(AttackParameter& param, float rightOrleft, YR_Vector3 plus_speed)
{
	attack = true;
	fin = false;
	hit_ok = false;
	knock_start = false;
	plus = true;
	this->plus_speed = plus_speed;
	this->rightOrleft = rightOrleft;
	speed = YR_Vector3(0.0f, 0.0f, 0.0f);

	parameter = param;
	if (parameter.damege != 0.0f)
	{
		hit_ok = true;
	}
}


void AttackBox::Update(YR_Vector3 pl_pos, float elapsed_time)
{
	pos.x = pl_pos.x + ((parameter.distance.x + speed.x) * rightOrleft);	//X座標更新
	pos.y = pl_pos.y + ((parameter.distance.y + speed.y));	//Y座標更新

	if (plus)
	{
		speed += plus_speed * elapsed_time;
	}

	parameter.timer -= elapsed_time;			//持続を減らしていく

	if (!hit_ok)
	{
		//当たったからダメージなどの数値を消去する
		parameter.damege = 0.0f;
		parameter.HB_timer = 0.0f;
		parameter.hitback = YR_Vector3(0.0f, 0.0f);
	}

	if (parameter.timer < 0.0f)
	{
		fin = true;
		attack = false;
		parameter.timer = 0.0f;
		parameter.damege = 0.0f;
		hit_ok = false;
		parameter.HB_timer = 0.0f;
		parameter.hitback = YR_Vector3(0.0f, 0.0f);
		parameter.knockback = 0.0f;
	}
}

void AttackBox::SpeedPlus(YR_Vector3 plus_speed, float elapsed_time)
{
	speed += plus_speed * elapsed_time;
}

//void AttackBox::Update(
//	YR_Vector3 cent, 
//	YR_Vector3 range, 
//	float time, float frm, float late, float dage, float hitbackT, YR_Vector3 divhitback, int kind, float knock, float elapsed_time)
//{
//	if (!start)
//	{
//		timer = 0;
//		timer = time;
//		fream = frm;
//		later = late;
//		attack = false;
//		fin = false;
//		start = true;
//		damege = dage;
//		HB_timer = hitbackT;
//		hitback = divhitback;
//		type = kind;
//		knockback = knock;
//		knock_start = false;
//	}
//	center = cent;
//	size = range;
//
//	fream-=elapsed_time;
//
//	if (fream < 0.0f)
//	{
//		attack = true;
//		if (damege != 0.0f)
//		{
//			hit_ok = true;
//		}
//	}
//
//	if (attack)
//	{
//		if (timer > 0.0f)
//		{
//
//		}
//		timer -= elapsed_time;
//		if (!hit_ok)
//		{
//			damege = 0.0f;
//			HB_timer = 0.0f;
//			hitback = YR_Vector3(0.0f, 0.0f);
//		}
//	}
//
//
//
//	if (timer < 0.0f)
//	{
//		fin = true;
//		attack = false;
//		timer = 0.0f;
//		damege = 0.0f;
//		hit_ok = false;
//		HB_timer = 0.0f;
//		hitback = YR_Vector3(0.0f, 0.0f);
//		knockback = 0.0f;
//	}
//}
//
//void AttackBox::UpdateMissile(
//	YR_Vector3 cent, YR_Vector3 range, float time, float dage, float hitbackT, YR_Vector3 divhitback, int kind, float elapsed_time)
//{
//	if (!start)
//	{
//		timer = 0.0f;
//		timer = time;
//		attack = false;
//		fin = false;
//		start = true;
//		damege = dage;
//		HB_timer = hitbackT;
//		hitback = divhitback;
//		type = kind;
//		knock_start = false;
//	}
//	center = cent;
//	size = range;
//
//
//	attack = true;
//	if (damege != 0.0f)
//	{
//		hit_ok = true;
//	}
//
//	if (attack)
//	{
//		if (timer > 0.0f)
//		{
//
//		}
//		timer -= elapsed_time;
//		if (!hit_ok)
//		{
//			damege = 0.0f;
//			HB_timer = 0.0f;
//			hitback = YR_Vector3(0.0f, 0.0f);
//		}
//	}
//
//
//
//	if (timer < 0.0f)
//	{
//		fin = true;
//		attack = false;
//		timer = 0.0f;
//		damege = 0.0f;
//		hit_ok = false;
//		HB_timer = 0.0f;
//		hitback = YR_Vector3(0.0f, 0.0f);
//		knockback = 0.0f;
//	}
//}
//
//
//void AttackBox::UpdateMissile(
//	YR_Vector3 cent, YR_Vector3 range, float time, float dage, float hitbackT, YR_Vector3 divhitback, int kind, int gauge, float elapsed_time)
//{
//	if (!start)
//	{
//		timer = 0.0f;
//		timer = time;
//		attack = false;
//		fin = false;
//		start = true;
//		damege = dage;
//		HB_timer = hitbackT;
//		hitback = divhitback;
//		type = kind;
//		knock_start = false;
//		gaugeout = true;
//	}
//	center = cent;
//	size = range;
//
//
//	attack = true;
//	if (damege != 0.0f)
//	{
//		hit_ok = true;
//	}
//
//	if (attack)
//	{
//		if (timer > 0.0f)
//		{
//
//		}
//		timer -= elapsed_time;
//		if (!hit_ok)
//		{
//			damege = 0.0f;
//			HB_timer = 0.0f;
//			hitback = YR_Vector3(0.0f, 0.0f);
//		}
//	}
//
//
//
//	if (timer < 0.0f)
//	{
//		fin = true;
//		attack = false;
//		timer = 0.0f;
//		damege = 0.0f;
//		hit_ok = false;
//		HB_timer = 0.0f;
//		hitback = YR_Vector3(0.0f, 0.0f);
//		knockback = 0.0f;
//		gaugeout = false;
//	}
//}
//
//bool AttackBox::Update(
//	YR_Vector3 cent,
//	YR_Vector3 range,
//	float time, float frm, float late, float dage, float hitbackT, YR_Vector3 divhitback, int kind, float knock, bool fin, float elapsed_time)
//{
//	if (!start)
//	{
//		timer = 0.0f;
//		timer = time;
//		fream = frm;
//		later = late;
//		attack = false;
//		this->fin = false;
//		start = true;
//		damege = dage;
//		HB_timer = hitbackT;
//		hitback = divhitback;
//		type = kind;
//		knockback = knock;
//		knock_start = false;
//		stealtimer = 20.0f;
//	}
//	center = cent;
//	size = range;
//
//	fream -= elapsed_time;
//
//	if (fream < 0.0f)
//	{
//		attack = true;
//		if (damege != 0.0f)
//		{
//			hit_ok = true;
//		}
//	}
//
//	if (attack)
//	{
//		if (timer > 0.0f)
//		{
//
//		}
//		timer -= elapsed_time;
//		if (!hit_ok)
//		{
//			damege = 0.0f;
//			HB_timer = 0.0f;
//			hitback = YR_Vector3(0.0f, 0.0f);
//		}
//	}
//
//
//
//	if (timer < 0.0f)
//	{
//		attack = false;
//		timer = 0.0f;
//		damege = 0.0f;
//		hit_ok = false;
//		HB_timer = 0.0f;
//		hitback = YR_Vector3(0.0f, 0.0f);
//		knockback = 0.0f;
//		return true;
//	}
//
//	return false;
//}
//
//void AttackBox::Update(
//	YR_Vector3 cent,
//	YR_Vector3 range,
//	float time, float frm, float late, float dage, float hitbackT, YR_Vector3 divhitback, int kind, float knock, int gauge, float elapsed_time)
//{
//	if (!start)
//	{
//		timer = 0.0f;
//		timer = time;
//		fream = frm;
//		later = late;
//		attack = false;
//		fin = false;
//		start = true;
//		damege = dage;
//		HB_timer = hitbackT;
//		hitback = divhitback;
//		type = kind;
//		knockback = knock;
//		knock_start = false;
//		gaugeout = true;
//	}
//	center = cent;
//	size = range;
//
//	fream -= elapsed_time;
//
//	if (fream < 0.0f)
//	{
//		attack = true;
//		if (damege != 0.0f)
//		{
//			hit_ok = true;
//		}
//	}
//
//	if (attack)
//	{
//		if (timer > 0)
//		{
//
//		}
//		timer -= elapsed_time;
//		if (!hit_ok)
//		{
//			damege = 0.0f;
//			HB_timer = 0.0f;
//			hitback = YR_Vector3(0.0f, 0.0f);
//		}
//	}
//
//
//
//	if (timer < 0.0f)
//	{
//		fin = true;
//		attack = false;
//		timer = 0.0f;
//		damege = 0.0f;
//		hit_ok = false;
//		HB_timer = 0.0f;
//		hitback = YR_Vector3(0.0f, 0.0f);
//		knockback = 0.0f;
//		gaugeout = false;
//	}
//}

void AttackBox::Draw(YRShader* shader,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color)
{
	FRAMEWORK.scenegame.geo->render(
		shader,
		DirectX::XMFLOAT3(pos.x,pos.y,0.0f),
		DirectX::XMFLOAT3(parameter.size.x,parameter.size.y,0.0f),
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