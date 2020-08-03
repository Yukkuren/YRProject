//#pragma once

#ifndef PLAYERBASE_H_
#define PLAYERBASE_H_

#include "Skinned_mesh.h"

#include "YR_VECTOR3.h"
#include "HitBox.h"
#include "AttackBox.h"
#include "Tracking.h"
#include <memory>
#include "YRGamePad.h"
#include "Texture.h"
#include <vector>
#include <string>


#define		POS_Y	(0.0f)

#define		GAUGE_MAX	(100.0f)

const int	Track_max = 2;		//�z�[�~���O�_�b�V���ő吔

//--------------------------------------
//	**�L�������ݒ�
//	�E�V�����L������ǉ�����Ȃ炱���ɋL��
//--------------------------------------
enum class PLSELECT :int
{
	KNIGHT = 0,
	KEN,
	PLSELECT_END,
};

//---------------------------------------------
// **�X�s�[�h�N���X**
//�E�v���C���[�ɉ��Z����Y�������̗͂���������
//�E�U���֐����s����Set�֐��𓮂����A���x������
//�E�󒆍U�����̃|�b�v�Ȃǂɗ��p����
//----------------------------------------------
class Speed
{
public:
	float speedY = 0.0f;

	void Set(float speed)
	{
		speedY = speed;
	};
	float Update(float elapsed_time)
	{
		if (speedY > 0.0f)
		{
			speedY -= elapsed_time;
		}
		else
		{
			speedY = 0.0f;
		}
		return speedY;
	};
};


//---------------------------------------------
// **�X�e�[�g�N���X**
//�EActState�͍U���ȊO�̔��f������B���f���Ȃ����̂�case�ɓ���Ȃ�(���̑��̏���������̂��߂Ɏg�p����)
//�EAttackState��ActState��ATTACK�������ꍇ�ǂ̍U�������Ă��邩�𔻒f����
//----------------------------------------------
enum class ActState : int
{
	NONE = 0,		//�����Ȃ�(default)
	WAIT,			//�ҋ@(default)
	GUARD,			//�K�[�h(default)
	SQUAT,			//���Ⴊ��(default)
	MOVER,			//�E�ړ�(default)
	MOVEL,			//���ړ�(default)
	DASH,			//�_�b�V��(default)
	BACK,			//�o�b�N�X�e�b�v(default)
	JUMP,			//�W�����v(default)
	AIR_F,			//�󒆑O�_�b�V��(default)
	AIR_B,			//�󒆌�_�b�V��(default)
	STATENONE,		//�X�e�[�g��D��ꂽ��ԁB����ɒ͂܂�Ă�or���K�H
	WAKE,			//�_�E������N���オ��
	PASSIVE,		//�󂯐g���Ƃ�
	DOWN,			//�_�E�����
	FALL,			//�󒆂Ń_�E�����Ă�����
	KNOCK,			//�U�����󂯂Ă̂�����
	ATTACK,			//�U����
};

enum class AttackState : int
{
	NONE,
	SLOW,
	TRACK_DASH,
	STEAL,
	JAKU,
	THU,
	KYO,
	D_JAKU,
	D_THU,
	U_KYO,
	JAKU_RHURF,
	THU_RHURF,
	KYO_RHURF,
	JAKU_LHURF,
	THU_LHURF,
	KYO_LHURF,
	EXTENDATK,
};



//--------------------------------------------------
// **�U���p�����[�^�[�\����**
//�E�U���̃p�����[�^�[�𓖂��蔻�育�Ƃɕۑ�����\����
//--------------------------------------------------
struct AttackParameter
{
public:
	YR_Vector3	center;		//�U������̒��S���W
	YR_Vector3	size;		//�U������̑傫��
	float		timer;		//�����t���[��
	float		fream;		//�����t���[��
	float		later;		//�㌄�t���[��
	float		damege;		//���̍U���ŗ^����_���[�W
	float		HB_timer;	//�U���𓖂Ă����̑���̂̂����莞��
	YR_Vector3	hitback;	//�U���𓖂Ă����̑���̐�����уx�N�g��
	int			type;		//�U���̑���(��i�E���i�E���i)
	float		knockback;	//�m�b�N�o�b�N(X�x�N�g���̂�)
	bool		gaugeout;	//false�Ȃ�Q�[�W�l���U��
	float		stealtimer; //�݂͂ʂ�����鎞��
};


//--------------------------------------------------
// **�U�����X�g�\����**
//�E�U���̃p�����[�^�[���L�����������ɓǂݍ��݁A
//�@�U�����s���ۂɓ����蔻��ɑΉ������p�����[�^�[�𑗂�
//--------------------------------------------------
struct AttackList
{
public:
	std::string						attack_name;	//�U����
	std::vector<AttackParameter>	parameter;		//�����p�����[�^�[
	int								quantity;		//�������铖���蔻��̐���
};


class Player
{
public:
	//�Q�[���������p�ϐ�
	float				hp = 0;
	int					jumpcount;
	bool				jumpflag;		//�W�����v���Ă���
	bool				max_jump_flag;	//�W�����v�̍ő�l�𒴂���
	bool				ground;			//TRUE�Ȃ�n�ʂɂ��Ă���
	bool				drawset;		//����̉摜��`�悵���Ƃ���true�ɂ��Ēʏ�̉摜��`�悵�Ȃ�
	bool				attack;			//TRUE�Ȃ�U����
	ActState			act_state;	//���s���\���B�܂��s���s�Ȃ�ǂ�������Ԃ�
	AttackState			attack_state;			//���������Ă��邩
	float				rightOrleft;	//�E�����Ȃ�*1�������Ȃ�*-1
	bool				moveflag;		//TRUE�Ȃ瓮���Ă���
	float				later;			//�㌄�t���[��
	float				knocktimer;		//��炢����
	float				gravity;		//�d�͒l
	float				specialfream;	//��R���{���Ńt���[�������炷�ۂ̐��l
	bool				finish;			//�s�����I�������Ƃ��̂�true
	bool				step;
	bool				hightrigger;	//�n�C�W�����v��true�A�ʏ펞false
	int					trackgauge;		//�ǔ��_�b�V���̎c���
	float				gauge;			//�Q�[�W
	int					power;			//�Q�[�W���ő�ɂȂ��1������B����Z�͂������������
	int					rival_state;	//����̃X�e�[�g
	float				steal_escape;	//���������\����
	int					combo_count;	//�R���{�J�E���g


	std::vector<AttackList>		attack_list;	//�U���̃��X�g�B�������ɓǂݍ��݁A�ۑ�����(�U���������Ƀp�����[�^�[�𑗂�)

public:
	//���f���p�ϐ�
	MeshMotion						motion;
	std::unique_ptr<Skinned_mesh>	base = nullptr;
public:
	//���W�A���f���`��p�ϐ�
	YR_Vector3		pos;
	YR_Vector3		scale;
	YR_Vector3		angle;
	YR_Vector3		speed;
	YR_Vector3		stop_pos;
	Tracking		tracking;
	Speed			speed_Y;
	std::unique_ptr<GamepadBase> pad;

public:
	//��{�����֐�
	virtual void Init(YR_Vector3 InitPos) = 0;
	virtual void LoadData(std::shared_ptr<Texture> texture =nullptr) = 0;
	virtual void Uninit() = 0;
	virtual void Update(float decision,float elapsed_time) = 0;
	virtual void Draw(
		YRShader				*geoshader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time ) = 0;

	virtual void DrawDEBUG(
		YRShader* geoshader,
		const DirectX::XMMATRIX& view,
		const DirectX::XMMATRIX& projection,
		const DirectX::XMFLOAT4& light_direction,
		const DirectX::XMFLOAT4& light_color,
		const DirectX::XMFLOAT4& ambient_color,
		float						elapsed_time) = 0;

public:
	//�s���n�֐�
	
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

	virtual void Attack(float decision, float elapsed_time) = 0;

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
	virtual void TrackDash(float decision, float elapsed_time) = 0;

	virtual bool Intro() = 0;
	virtual bool WinPerformance() = 0;

	virtual bool AttackLoad() = 0;
	virtual bool AttackWrite() = 0;
};

#endif // !PLAYERBASE_H_

