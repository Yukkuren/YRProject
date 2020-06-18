//#pragma once

#ifndef PLAYERBASE_H_
#define PLAYERBASE_H_

#include "YR_VECTOR3.h"
#include "Tracking.h"
#include <memory>
#include "YRGamePad.h"

//---------------------------------------------
// **�X�s�[�h�N���X**
//�E�v���C���[�ɉ��Z����Y�������̗͂���������
//�E�U���֐����s����Set�֐��𓮂����A���x������
//�E�󒆍U�����̃|�b�v�Ȃǂɗ��p����
//----------------------------------------------
class Speed
{
public:
	float speedY = 0;

	void Set(float speed);
	float Update();
};


class Player
{
public:
	float		hp = 0;
	int			jumpcount;
	bool		jumpflag;
	bool		max_jump_flag;
	bool		ground;			//TRUE�Ȃ�n�ʂɂ��Ă���
	bool		drawset;		//����̉摜��`�悵���Ƃ���true�ɂ��Ēʏ�̉摜��`�悵�Ȃ�
	bool		attack;			//TRUE�Ȃ�U����
	int			state;			//���������Ă��邩(enum)
	float		rightOrleft;	//�E�����Ȃ�*1�������Ȃ�*-1
	bool		moveflag;		//TRUE�Ȃ瓮���Ă���
	int			later;			//�㌄�t���[��
	int			knocktimer;		//��炢����
	float		gravity;		//�d�͒l
	int			specialfream;	//��R���{���Ńt���[�������炷�ۂ̐��l
	bool		finish;			//�s�����I�������Ƃ��̂�true
	bool		step;
	bool		hightrigger;	//�n�C�W�����v��true�A�ʏ펞false
	int			trackgauge;		//�ǔ��_�b�V���̎c���
	float		gauge;			//�Q�[�W
	int			power;			//�Q�[�W���ő�ɂȂ��1������B����Z�͂������������
	int			rival_state;	//����̃X�e�[�g
	int			steal_escape;	//���������\����
	int			combo_count;	//�R���{�J�E���g


	YR_Vector3		pos;
	YR_Vector3		speed;
	YR_Vector3		stop_pos;
	Tracking	tracking;
	Speed		speed_Y;

	std::unique_ptr<GamepadBase> pad;
	//Vector2 pos;
	virtual void Init(YR_Vector3 InitPos) = 0;
	virtual void Uninit() = 0;
	virtual void Update(float decision) = 0;
	virtual void Draw() = 0;
	virtual void Move(float decision) = 0;
	virtual bool Step() = 0;
	virtual void Jump() = 0;
	virtual void JumpUpdate() = 0;
	virtual void DamageCheck() = 0;
	virtual void Guard(float decision) = 0;
	virtual void Squat() = 0;
	virtual void GuardBack() = 0;
	virtual void FallUpdate() = 0;
	virtual void DownUpdate() = 0;
	virtual void PassiveUpdate() = 0;
	virtual void WakeUp() = 0;
	virtual void GaugeUp(float add) = 0;
	virtual void CancelList() = 0;
	virtual void StateNone() = 0;

	virtual void WinAnimSet() = 0;

	//virtual float GetPosX();
	virtual HitBox* GetHit() = 0;
	virtual AttackBox* GetAttack() = 0;
	virtual int GetMax(int n) = 0;
	//virtual float& GetPosX2();
	/*virtual float GetRivalPos(float &pos) = 0;*/

	virtual void NoneChange() = 0;

	virtual void StopUpdate() = 0;
	virtual void StopEnd() = 0;
	virtual void TrackDash(float decision) = 0;
};

#endif // !PLAYERBASE_H_

