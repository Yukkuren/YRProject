#include "HitBox.h"
#include "PlayerBase.h"
#include "framework.h"

void HitBox::Init()
{
	center = { 0.0f,0.0f };
	hit = false;
	damege = 0.0f;
	//hitother = false;
	timer = 0.0f;
	hitback = { 0.0f, 0.0f };
	guard_ok = false;
	//steal = false;
	hit_state = HitStateKind::NORMAL;
	steal_timer = 0.0f;
}

void HitBox::Update(YR_Vector3 pl_pos, HitParameter& param, float rightOrleft, float elapsed_time)
{
	parameter = param;
	center.x = pl_pos.x + (parameter.distance.x * rightOrleft);
	center.y = pl_pos.y + parameter.distance.y;
	//size = range;
	//if (DEBUG_MODE)Draw();
	if (timer > 0.0f)
	{
		timer -= elapsed_time;
		if (hitback.x > 0.0f)
		{
			hitback.x -= (elapsed_time * hitback_Adjustment);
			if (hitback.x < 0.0f)
			{
				hitback.x = 0.0f;
			}
		}
		if (hitback.x < 0.0f)
		{
			hitback.x += (elapsed_time * hitback_Adjustment);
			if (hitback.x > 0.0f)
			{
				hitback.x = 0.0f;
			}
		}
		if (hitback.y > 0.0f)
		{
			hitback.y -= (elapsed_time * hitback_Adjustment);
			if (hitback.y < 0.0f)
			{
				hitback.y = 0.0f;
			}
		}
		if (timer < 0.0f)
		{
			timer = 0.0f;
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

void HitBox::Update(YR_Vector3 pl_pos, YR_Vector3& distance, YR_Vector3& size, float rightOrleft, float elapsed_time)
{
	parameter.size = size;
	parameter.distance = distance;
	center.x = pl_pos.x + (parameter.distance.x * rightOrleft);
	center.y = pl_pos.y + parameter.distance.y;
	//size = range;
	//if (DEBUG_MODE)Draw();
	if (timer > 0.0f)
	{
		timer -= elapsed_time;
		if (hitback.x > 0.0f)
		{
			hitback.x -= (elapsed_time * hitback_Adjustment);
			if (hitback.x < 0.0f)
			{
				hitback.x = 0.0f;
			}
		}
		if (hitback.x < 0.0f)
		{
			hitback.x += (elapsed_time * hitback_Adjustment);
			if (hitback.x > 0.0f)
			{
				hitback.x = 0.0f;
			}
		}
		if (hitback.y > 0.0f)
		{
			hitback.y -= (elapsed_time * hitback_Adjustment);
			if (hitback.y < 0.0f)
			{
				hitback.y = 0.0f;
			}
		}
		if (timer < 0.0f)
		{
			timer = 0.0f;
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

void HitBox::Draw(
	YRShader *shader,
	const DirectX::XMMATRIX& view,
	const DirectX::XMMATRIX& projection,
	const DirectX::XMFLOAT4& light_direction,
	const DirectX::XMFLOAT4& light_color,
	const DirectX::XMFLOAT4& ambient_color)
{
	if (state == HitBoxState::INVINCIBLE)
	{
		//無敵時
		FRAMEWORK.scenegame.geo->render(
			shader,
			center.GetDXFLOAT3(),
			DirectX::XMFLOAT3(parameter.size.x, parameter.size.y, 0.0f),
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			view,
			projection,
			DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f)
		);
	}
	else if (state == HitBoxState::PROJECTILE_ARMOR)
	{
		//飛び道具アーマー時
		FRAMEWORK.scenegame.geo->render(
			shader,
			center.GetDXFLOAT3(),
			DirectX::XMFLOAT3(parameter.size.x, parameter.size.y, 0.0f),
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			view,
			projection,
			DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 0.5f)
		);
	}
	else if (state == HitBoxState::UP_INVINCIBLE)
	{
		//上段無敵時
		FRAMEWORK.scenegame.geo->render(
			shader,
			center.GetDXFLOAT3(),
			DirectX::XMFLOAT3(parameter.size.x, parameter.size.y, 0.0f),
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			view,
			projection,
			DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f)
		);
	}
	else if (state == HitBoxState::SLIDE)
	{
		//滑り時
		FRAMEWORK.scenegame.geo->render(
			shader,
			center.GetDXFLOAT3(),
			DirectX::XMFLOAT3(parameter.size.x, parameter.size.y, 0.0f),
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			view,
			projection,
			DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 0.5f)
		);
	}
	else
	{
		if (timer == 0.0f)
		{
			//通常時
			FRAMEWORK.scenegame.geo->render(
				shader,
				center.GetDXFLOAT3(),
				DirectX::XMFLOAT3(parameter.size.x, parameter.size.y, 10.0f),
				DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
				view,
				projection,
				DirectX::XMFLOAT4(0.0f, 0.0f, 0.8f, 0.5f)
			);
		}
		else
		{
			if (guard_ok)
			{
				//ガード成功
				FRAMEWORK.scenegame.geo->render(
					shader,
					center.GetDXFLOAT3(),
					DirectX::XMFLOAT3(parameter.size.x, parameter.size.y, 0.0f),
					DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
					view,
					projection,
					DirectX::XMFLOAT4(0.8f, 0.0f, 0.8f, 0.5f)
				);
			}
			else
			{
				//のけぞり中
				FRAMEWORK.scenegame.geo->render(
					shader,
					center.GetDXFLOAT3(),
					DirectX::XMFLOAT3(parameter.size.x, parameter.size.y, 5.0f),
					DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
					view,
					projection,
					DirectX::XMFLOAT4(0.0f, 0.8f, 0.0f, 0.5f)
				);
			}
		}
	}
}