#include "HitBox.h"
#include "PlayerBase.h"
#include "framework.h"

void HitBox::Init(YR_Vector3 cent, YR_Vector3 range)
{
	center = cent;
	size = range;
	hit = false;
	damege = 0.0f;
	//hitother = false;
	timer = 0.0f;
	hitback = { 0.0f, 0.0f };
	guard = NOGUARD;
	guard_ok = false;
	steal = false;
	steal_timer = 0.0f;
}

void HitBox::Update(YR_Vector3 cent, YR_Vector3 range,float elapsed_time)
{
	center = cent;
	size = range;
	//if (DEBUG_MODE)Draw();
	if (timer > 0.0f)
	{
		timer -= elapsed_time;
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
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	if (guard == INVINCIBLE)
	{
		//ƒK[ƒhŽž
		FRAMEWORK.scenegame.geo->render(
			shader,
			center.GetDXFLOAT3(),
			DirectX::XMFLOAT3(size.x, size.y, 5.0f),
			DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			view,
			projection,
			DirectX::XMFLOAT4(1.0f, 0.8f, 0.0f, 0.5f)
		);
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
		if (timer == 0.0f)
		{
			//’ÊíŽž
			FRAMEWORK.scenegame.geo->render(
				shader,
				center.GetDXFLOAT3(),
				DirectX::XMFLOAT3(size.x, size.y, 10.0f),
				DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
				view,
				projection,
				DirectX::XMFLOAT4(0.0f, 0.0f, 0.8f, 0.5f)
			);
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
			//‚Ì‚¯‚¼‚è’†
			FRAMEWORK.scenegame.geo->render(
				shader,
				center.GetDXFLOAT3(),
				DirectX::XMFLOAT3(size.x, size.y, 5.0f),
				DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
				view,
				projection,
				DirectX::XMFLOAT4(0.0f, 0.8f, 0.0f, 0.5f)
			);
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