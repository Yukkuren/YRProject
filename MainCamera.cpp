#include "camera.h"
//#include "key_input.h"
#include<stdlib.h>

void MainCamera::vibrate(float elapsedTime)
{
	if (timer > 0)
	{
		timer -= elapsedTime;

		DirectX::XMFLOAT3 focus;
		focus = moveTarget;
		float rax = rand() % 3 - 1.0f;
		float ray = rand() % 3 - 1.0f;
		float raz = rand() % 3 - 1.0f;

		float dx = rax*range*timer + moveTarget.x;
		float dy = ray*range*timer + moveTarget.y;
		float dz = raz*range*timer + moveTarget.z;
		focus.x = dx;
		focus.y = dy;
		focus.z = dz;

		SetFocus(focus);
	}
	/*else if (KeyInput::KeyTrigger() & KEY_G)
	{
		SetVibration(0.1f, 5.0f);
	}*/
}

void MainCamera::chase(float elapsedTime)
{
	moveTarget = GetFocus();
	DirectX::XMFLOAT3 eye = GetEye();
	float dx = eye.x - moveTarget.x;
	float dz = eye.z - moveTarget.z;

	//‹——£‚ð‘ª‚Á‚Ä(Y•ûŒü‚Í“ü‚ê‚È‚¢)³‹K‰»
	float d = sqrtf((dx*dx) + (dz*dz));
	

	//ƒJƒƒ‰ˆÊ’u‚ð‹tŽZ
	if (d > FAR_DIST)
	{
		d = FAR_DIST;
	}

	if (d < NEAR_DIST)
	{
		d = NEAR_DIST;
	}

	DirectX::XMFLOAT3 Cvector;

	Cvector.x = eye.x - moveTarget.x;
	Cvector.y = 0;
	Cvector.z = eye.z - moveTarget.z;

	DirectX::XMFLOAT3 Csei;

	Csei.x = (Cvector.x / sqrtf((Cvector.x*Cvector.x) + (Cvector.y*Cvector.y) + (Cvector.z*Cvector.z)));
	Csei.y = 0;
	Csei.z = (Cvector.z / sqrtf((Cvector.x*Cvector.x) + (Cvector.y*Cvector.y) + (Cvector.z*Cvector.z)));

	eye.x = moveTarget.x + Csei.x*d;
	eye.z = moveTarget.z + Csei.z*d;

	SetEye(eye);
	SetFocus(moveTarget);
}

void MainCamera::fix(float elapsedTime)
{
	SetFocus(moveTarget);
}

void MainCamera::Update(float elapsedTime)
{

	switch (mode)
	{
	case MainCamera::MODE_FIX:
		fix(elapsedTime);
		/*if (KeyInput::KeyTrigger() & KEY_V)
		{
			SetMode(MODE_CHASE);
		}*/
		break;
	case MainCamera::MODE_CHASE:
		chase(elapsedTime);
		/*if (KeyInput::KeyTrigger() & KEY_V)
		{
			SetMode(MODE_FIX);
		}*/
		break;
	case MainCamera::MODE_NUM:
		break;
	default:
		break;
	}
	vibrate(elapsedTime);
}

void MainCamera::SetMode(MODE mode)
{
	this->mode = mode;
}

void MainCamera::SetVibration(float range, float timer)
{
	this->range = range;
	this->timer = timer;
}

void MainCamera::SetTarget(const DirectX::XMFLOAT3& moveTarget)
{
	this->moveTarget = moveTarget;
}