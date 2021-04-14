//#include"DxLib.h"
#include "AttackBox.h"
//#include"Common.h"
#include "PlayerBase.h"
#include "framework.h"

AttackBox::~AttackBox()
{
	//YRGetEffect().StopEffect(effect_kind, handle);
}


void AttackBox::Init(int attack_name, AttackParameter& param, float rightOrleft,YR_Vector3 pl_pos, EffectParameter effect_param)
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

	this->effect_param = effect_param;

	if (effect_param.rightORleft)
	{
		//プレイヤーの角度を依存させる場合
		YRGetEffect().PlayEffect(
			effect_param.effect_kind, handle,
			DirectX::XMFLOAT3(pos.x + effect_param.distance.x, pos.y + effect_param.distance.y, pos.z + effect_param.distance.z),
			effect_param.scale.GetDXFLOAT3(), effect_param.axis.GetDXFLOAT3(), effect_param.angle * rightOrleft);
	}
	else
	{
		//依存させない場合
		YRGetEffect().PlayEffect(
			effect_param.effect_kind, handle,
			DirectX::XMFLOAT3(pos.x + effect_param.distance.x, pos.y + effect_param.distance.y, pos.z + effect_param.distance.z),
			effect_param.scale.GetDXFLOAT3(), effect_param.axis.GetDXFLOAT3(), effect_param.angle);
	}
	//YRGetEffect().PlayEffect(effect_kind, handle, pos.GetDXFLOAT3(), DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), 0.0f);

	pos.x = pl_pos.x + ((parameter.distance.x) * rightOrleft);	//X座標更新
	pos.y = pl_pos.y + ((parameter.distance.y));	//Y座標更新
}

void AttackBox::Init(int attack_name, AttackParameter& param, float rightOrleft, YR_Vector3 pl_pos, YR_Vector3 plus_speed, EffectParameter effect_param)
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
	this->effect_param = effect_param;

	if (effect_param.rightORleft)
	{
		//プレイヤーの角度を依存させる場合
		YRGetEffect().PlayEffect(
			effect_param.effect_kind, handle,
			DirectX::XMFLOAT3(pos.x + effect_param.distance.x, pos.y + effect_param.distance.y, pos.z + effect_param.distance.z),
			effect_param.scale.GetDXFLOAT3(), effect_param.axis.GetDXFLOAT3(), effect_param.angle * rightOrleft);
	}
	else
	{
		//依存させない場合
		YRGetEffect().PlayEffect(
			effect_param.effect_kind, handle,
			DirectX::XMFLOAT3(pos.x + effect_param.distance.x, pos.y + effect_param.distance.y, pos.z + effect_param.distance.z),
			effect_param.scale.GetDXFLOAT3(), effect_param.axis.GetDXFLOAT3(), effect_param.angle);
	}

	pos.x = pl_pos.x + (parameter.distance.x * rightOrleft);	//X座標更新
	pos.y = pl_pos.y + (parameter.distance.y);	//Y座標更新
}


void AttackBox::Update(YR_Vector3 pl_pos, float elapsed_time)
{
	if (parameter.type == AttackKind::PROJECTILE)
	{
		pos.x += ((speed.x * rightOrleft) * elapsed_time);	//X座標更新
		pos.y += (speed.y * elapsed_time);	//Y座標更新
		YRGetEffect().SetLocation(effect_param.effect_kind, handle, DirectX::XMFLOAT3(pos.x + (rightOrleft * parameter.size.x) + effect_param.distance.x, pos.y + effect_param.distance.y, pos.z + effect_param.distance.z));
	}
	else
	{
		pos.x = pl_pos.x + ((parameter.distance.x + speed.x) * rightOrleft);	//X座標更新
		pos.y = pl_pos.y + ((parameter.distance.y + speed.y));	//Y座標更新

		YRGetEffect().SetLocation(effect_param.effect_kind, handle, DirectX::XMFLOAT3(pos.x + (rightOrleft * parameter.size.x) + effect_param.distance.x, pos.y + effect_param.distance.y, pos.z + effect_param.distance.z));

		/*if (effect_param.effect_kind == EffectKind::TORNADE)
		{
			YRGetEffect().SetLocation(effect_kind, handle, DirectX::XMFLOAT3(pos.x + (rightOrleft * parameter.size.x), pos.y - 5.0f, pos.z));
		}
		else
		{
			YRGetEffect().SetLocation(effect_kind, handle, DirectX::XMFLOAT3(pos.x + (rightOrleft * parameter.size.x), pos.y , pos.z));
		}*/

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
		YRGetEffect().StopEffect(effect_param.effect_kind, handle);
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

		YRGetEffect().StopEffect(effect_param.effect_kind, handle);
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