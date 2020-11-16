//#include"DxLib.h"
#include "AttackBox.h"
//#include"Common.h"
#include "PlayerBase.h"
#include "framework.h"

void AttackBox::Init(int attack_name, AttackParameter& param, float rightOrleft,YR_Vector3 pl_pos)
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
	parameter.hitback.x *= rightOrleft;
	//parameter.knockback *= (rightOrleft);
	hit_result = HitResult::NONE;
	this->attack_name = attack_name;
	if (parameter.damege != 0.0f)
	{
		hit_ok = true;
	}

	effect_kind = EffectKind::NONE;

	pos.x = pl_pos.x + ((parameter.distance.x) * rightOrleft);	//X座標更新
	pos.y = pl_pos.y + ((parameter.distance.y));	//Y座標更新
}

void AttackBox::Init(int attack_name, AttackParameter& param, float rightOrleft, YR_Vector3 pl_pos, YR_Vector3 plus_speed)
{
	attack = true;
	fin = false;
	hit_ok = false;
	knock_start = false;
	plus = true;
	this->plus_speed = plus_speed;
	this->plus_speed.x *= rightOrleft;
	this->rightOrleft = rightOrleft;
	speed = YR_Vector3(0.0f, 0.0f, 0.0f);
	hit_result = HitResult::NONE;
	parameter = param;
	parameter.hitback.x *= rightOrleft;
	this->attack_name = attack_name;
	//parameter.knockback *= (rightOrleft);
	if (parameter.damege != 0.0f)
	{
		hit_ok = true;
	}

	if (parameter.type == AttackKind::PROJECTILE)
	{
		//属性が遠距離攻撃ならスピードを付与する
		speed = plus_speed;
	}

	effect_kind = EffectKind::NONE;

	pos.x = pl_pos.x + (parameter.distance.x * rightOrleft);	//X座標更新
	pos.y = pl_pos.y + (parameter.distance.y);	//Y座標更新
}


void AttackBox::Update(YR_Vector3 pl_pos, float elapsed_time)
{
	if (parameter.type == AttackKind::PROJECTILE)
	{
		pos.x += ((speed.x * rightOrleft) * elapsed_time);	//X座標更新
		pos.y += (speed.y * elapsed_time);	//Y座標更新
		YRGetEffect().SetLocation(EffectKind::DRILL, handle, DirectX::XMFLOAT3(pos.x + (rightOrleft * parameter.size.x), pos.y, pos.z));
	}
	else
	{
		pos.x = pl_pos.x + ((parameter.distance.x + speed.x) * rightOrleft);	//X座標更新
		pos.y = pl_pos.y + ((parameter.distance.y + speed.y));	//Y座標更新

		YRGetEffect().SetLocation(effect_kind,handle, DirectX::XMFLOAT3(pos.x + (rightOrleft * parameter.size.x), pos.y-5.0f, pos.z));

		if (plus)
		{
			speed += plus_speed * elapsed_time;
		}
	}

	parameter.timer -= elapsed_time;			//持続を減らしていく

	if (!hit_ok)
	{
		//当たったからダメージなどの数値を消去する
		parameter.damege = 0.0f;
		parameter.HB_timer = 0.0f;
		parameter.timer = -1.0f;
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
		if (parameter.type == AttackKind::PROJECTILE)
		{
			YRGetEffect().StopEffect(EffectKind::DRILL,handle);
		}
		else
		{
			YRGetEffect().StopEffect(effect_kind, handle);
		}
	}
}

void AttackBox::SpeedPlus(YR_Vector3 plus_speed, float elapsed_time)
{
	this->plus_speed += plus_speed * elapsed_time;
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