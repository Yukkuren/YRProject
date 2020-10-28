//#pragma once

#ifndef PLAYERBASE_H_
#define PLAYERBASE_H_

#include "Skinned_mesh.h"

#include "YR_VectorMaster.h"
#include "HitBox.h"
#include "AttackBox.h"
#include "Tracking.h"
#include <memory>
#include "YRGamePad.h"
#include "Texture.h"
#include <vector>
#include <string>


constexpr float POS_Y = 0.0f;		//�Œ�̍���
constexpr float GAUGE_MAX = 100.0f;	//�Q�[�W�ő��
constexpr int	Track_max = 2;		//�z�[�~���O�_�b�V���ő吔
constexpr float	non_target = 110.0f;//���̒l������ꂽ�t���[���͏�������O���悤�ɂ���
constexpr float target_max = 100.0f;//�������ł��̒l�ȏ�͊O���悤�ɂ���

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


//--------------------------------------
//	**�A�j���[�V�����I��
//	�ENodeChange���ɑ���A�A�j���[�V�������w�肷��
//--------------------------------------
enum class AnimAtk :int
{
	FREAM = 0,	//����
	LATER,		//�㌄
	TIMER,		//����
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
	const float decrease_value = 100.0f;		//�X�s�[�h�̌����l

	void Set(float speed)
	{
		speedY = speed;
	};
	float Update(float elapsed_time)
	{
		if (speedY > 0.0f)
		{
			speedY -= (decrease_value * elapsed_time);
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
	//�E�����̎�
	NONE = 0,		//�����Ȃ�(default)
	WAIT,			//�ҋ@(default)m
	GUARD,			//�K�[�h(default)
	SQUAT,			//���Ⴊ��(default)m
	MOVER,			//�E�ړ�(default)m
	MOVEL,			//���ړ�(default)m
	DASH,			//�_�b�V��(default)m
	BACK,			//�o�b�N�X�e�b�v(default)m
	JUMP,			//�W�����v(default)m
	AIR_F,			//�󒆑O�_�b�V��(default)
	AIR_B,			//�󒆌�_�b�V��(default)
	STATENONE,		//�X�e�[�g��D��ꂽ��ԁB����ɒ͂܂�Ă�or���K�H
	WAKE,			//�_�E������N���オ��
	PASSIVE,		//�󂯐g���Ƃ�
	DOWN,			//�_�E�����
	FALL,			//�󒆂Ń_�E�����Ă�����
	KNOCK,			//�U�����󂯂Ă̂�����
	ATTACK,			//�U����
	ACT_END,		//(�I�_)
};

enum class AttackState : int
{
	NONE = 0,		//�U������
	JAKU,			//��m
	THU,			//��(����)m
	D_KYO,			//����(���̎��ɏo�鋭�U��)
	D_JAKU,			//����m
	D_THU,			//����m
	U_KYO,			//�㋭(���U��)m
	A_JAKU,			//���U��m
	A_THU,			//�󒆍U��m
	A_KYO,			//�󋭍U��m
	A_UKYO,			//��㋭�U��(�ł��グ�U��)m
	STEAL,			//�͂�m
	SLOW,			//����m
	TRACK_DASH,		//�z�[�~���O�_�b�V��m
	JAKU_RHURF,		//�O��K�E
	THU_RHURF,		//�O���K�E
	KYO_RHURF,		//�O���K�E
	JAKU_LHURF,		//���K�E
	THU_LHURF,		//�㒆�K�E
	KYO_LHURF,		//�㋭�K�E
	SPECIAL_ATTACK,	//�O���K�Em
	DESIRE_SPECIAL,	//�㒴�K�E
	DESIRE_METEOR,	//�����Z
	EXTENDATK,		//���G�U��
	JAKU_THU,		//��̎��ɏo�钆�U��m
	JAKU_KYO,		//���̎��ɏo�鋭�U��m

	ATTACK_END,		//�ő�T�C�Y
};

//Desire(�~�]�A�]��)��Crystal(����)

//---------------------------------------------------------------
// **�U�����e�ۑ��\����**
//�E�U���̔����t���[���A�����p�����[�^�ϐ��A���̐�������ۑ�����\����
//---------------------------------------------------------------
struct AttackSingle
{
public:
	std::vector<AttackParameter>	parameter;		//�����p�����[�^�[
	int								quantity;		//��x�̍U���Ő������铖���蔻��̐���
	float							fream;			//�����t���[��
};


//------------------------------------------------------------------------
// **�U�����X�g�N���X**
//�E�U���̃p�����[�^�[���L�����������ɓǂݍ��݁A
//�@�U�����s���ۂɓ����蔻��ɑΉ������p�����[�^�[�𑗂�
//			�ȉ��A�����p
//�P�F�U�������{�^��������
//�Q�F�v���C���[�ɔ����t���[���𑗂�
//�R�F�����t���[���ɂȂ�����SetAttack�֐��œ����蔻��𐶐�
//�S�F�����蔻��̎����͓����蔻��{�̂Œ�������׉����G��Ȃ�
//�T�F�U���������񔭐�����ꍇ��attack_max�ɂ��̉񐔂����Ă��̕����[�v������
//
//		�U�������蔻��
//�P�F��������Init�֐��Ńp�����[�^�[�𑗂�B
//		���̎��Adistance�ɓ����̂̓v���C���[����ǂꂾ������Ă��邩�Ƃ����l
//�Q�FUpdate�֐��̈����Ƀv���C���[��pos��n���A��Ɉʒu���X�V�����悤�ɂ���
//�R�FAttackBox�N���X�Ɂu�U������ɕt�^����X�s�[�h�𑝉�������֐��v�����AUpdate�Œl��ς���
//etc...Update���Fpos=player_pos+distance+speed;
//						speed�͊֐��ŕς����邽�߁Apos�̈ʒu���ς���Ă���
//-------------------------------------------------------------------------
class AttackList
{
public:
	AttackState						attack_name;	//�U����(�v���C���[�������ɕۑ�)
	int								attack_max;		//�U����
	std::vector<AttackSingle>		attack_single;	//�U�����e�ۑ�
	float							later;			//�㌄�t���[��
	int								now_attack_num;	//���ݎ��s���Ă���U���ԍ��͉��Ԃ�������(attack_max�܂ŉ������later���v���C���[��)
	PAD								linkage_button;	//�ǂ̃{�^���ōU���𔭐������邩
	Command							linkage_command;//�ǂ̃R�}���h�ōU���𔭐������邩
	bool							ground_on;		//�U���͋󒆁A�n��ǂ���Ŕ��������邩(true�Œn�㔭��)
	bool							squat_on;		//���Ⴊ�ݍU�����ǂ���(true�ł��Ⴊ�ݍU��)
	float							need_gauge;		//�U�����s���̂ɕK�v�ȃQ�[�W��(1.0�P�ʂŋL�q)
	PAD								linkage_stick;	//�ǂ̕����ւ̓��͂ōU���𔭐������邩
	AttackState						aid_attack_name;//�Q�[�W������Ȃ������ꍇ�o���Z
	AttackState						real_attack;	//���ۂ̍U��(��{��attack_name�Ɠ������̂����A����̍U���Ɠ����U�����o���ꍇ�͂��̍U����������)
	bool							speed_on;		//�X�s�[�h������ꍇ��true
	YR_Vector3						speed;			//�U������ɉ��Z���鑬�x
public:
	AttackList() : now_attack_num(0), attack_name(AttackState::NONE), later(0.0f),
		attack_max(0), linkage_button(PAD::BUTTOM_END), linkage_command(Command::NOCOMMAND), ground_on(true), squat_on(false),
		need_gauge(0.0f), linkage_stick(PAD::BUTTOM_END), aid_attack_name(AttackState::NONE), real_attack(attack_name),
		speed_on(false), speed(0.0f, 0.0f, 0.0f) {};
	//�U�������蔻��𐶐�����
	void SetAttack(std::vector<AttackBox> *atk, float rightOrleft, YR_Vector3 pl_pos)
	{
		for (int quantity = 0; quantity < attack_single[now_attack_num].quantity; quantity++)
		{
			atk->push_back(AttackBox());
			atk->back().Init(attack_single[now_attack_num].parameter[quantity], rightOrleft, pl_pos);
		}
		now_attack_num++;
	}

	//��ѓ���U�������蔻����Z�b�g����
	void SetAttack(std::vector<AttackBox>* atk, float rightOrleft, YR_Vector3 pl_pos, YR_Vector3 plus_speed)
	{
		for (int quantity = 0; quantity < attack_single[now_attack_num].quantity; quantity++)
		{
			atk->push_back(AttackBox());
			atk->back().Init(attack_single[now_attack_num].parameter[quantity], rightOrleft, pl_pos, plus_speed);
		}
		now_attack_num++;
	}
};




//---------------------------------------------------------------------------
// **�����蔻�胊�X�g�N���X**
//�E�����蔻��̃p�����[�^�[���e�s�����Ƃɕۑ�����N���X
//�E�v���C���[�N���X��Update�Ŗ���p�����[�^�[�����Ă���
//�E�����������蔻��̐��̓L�����ɂ���ĕς��ׁA�ϐ���vector�^�Ŏ������A
//�@�L�����������ɕK�v�Ȍ�����������
//----------------------------------------------------------------------------
class HitParameterList
{
public:
	std::array<HitParameter,scastI(ActState::ACT_END)>			act_parameter;		//�U���ȊO�̍s�����̓����蔻��
	std::array<HitParameter, scastI(AttackState::ATTACK_END)>	attack_parameter;	//�U�����̓����蔻��
};




//---------------------------------------------------------------
// **�A�j���[�V���������l�ۑ��\����**
//�E�����A�����A�㌄���ꂼ��̃A�j���[�V�������x�����l��ۑ�����\����
//---------------------------------------------------------------
struct Animation_Coordinate
{
public:
	float							fream;			//����
	float							timer;			//����
	float							later;			//�㌄
};


class Player
{
protected:
	const float jump_max_time = 0.05f;	//�W�����v������jump_can_timer�ɓ����l
public:
	//�Q�[���������p�ϐ�
	float				hp = 0;
	int					jumpcount;		//�W�����v�̉񐔕ێ�
	bool				jumpflag;		//�W�����v���Ă���
	bool				max_jump_flag;	//�W�����v�̍ő�l�𒴂���
	float				jump_can_timer;	//�W�����v�\�ɂȂ�܂ł̎��Ԃ𑪒肷��(0.0�ȉ��ɂȂ�΃W�����v�\)
	bool				ground;			//TRUE�Ȃ�n�ʂɂ��Ă���
	bool				drawset;		//����̉摜��`�悵���Ƃ���true�ɂ��Ēʏ�̉摜��`�悵�Ȃ�
	bool				attack;			//TRUE�Ȃ�U����
	ActState			act_state;		//���s���\���B�܂��s���s�Ȃ�ǂ�������Ԃ�
	AttackState			attack_state;	//�����̍U�������Ă��邩
	float				rightOrleft;	//�E�����Ȃ�*1�������Ȃ�*-1
	bool				moveflag;		//TRUE�Ȃ瓮���Ă���
	float				fream;			//�����t���[��
	float				later;			//�㌄�t���[��(�����l��non_target�����Ă���)
	float				knocktimer;		//��炢����
	float				gravity;		//�d�͒l
	float				down_force;		//�W�����v���ɏd�͂Ƃ͕ʂɓ����������̗�
	float				fall_force;		//�󒆃_�E�����ɏd�͂Ƃ͕ʂɓ����������̗�
	float				specialfream;	//��R���{���Ńt���[�������炷�ۂ̐��l
	bool				finish;			//�s�����I�������Ƃ��̂�true
	bool				step;			//�X�e�b�v����true(���̏������s��Ȃ�)
	bool				hightrigger;	//�n�C�W�����v��true�A�ʏ펞false
	int					trackgauge;		//�ǔ��_�b�V���̎c���
	float				gauge;			//�Q�[�W
	int					power;			//�Q�[�W���ő�ɂȂ��1������B����Z�͂������������
	int					rival_state;	//����̃X�e�[�g
	float				steal_escape;	//���������\����
	int					combo_count;	//�R���{�J�E���g
	int					now_player;		//�ǂ̃v���C���[�����̃L�����𑀍삵�Ă��邩(1:1P�A2:2P)
	float				anim_ccodinate;	//�A�j���[�V�������x�𒲐�����ϐ�
	int					stop_state;		//�q�b�g�X�g�b�v���̏����Ŏg�p
	HitResult			hit_result;		//�U�������������ꍇ�̌��ʂ�ۑ�����
	std::vector<AttackBox> atk;			//�����蔻��
	
	std::array<Animation_Coordinate, scastI(AttackState::ATTACK_END)>	ac_attack;	//�U�����Ƃ̃A�j���[�V���������l
	std::array<Animation_Coordinate, scastI(ActState::ACT_END)>			ac_act;		//�s�����Ƃ̃A�j���[�V���������l

	float												jump_later = 1.0f;			//�W�����v�̒��n��
	float												dash_later = 1.0f;			//�_�b�V���̌㌄

	std::vector<AttackList>								attack_list;	//�U���̃��X�g�B�������ɓǂݍ��݁A�ۑ�����(�U���������Ƀp�����[�^�[�𑗂�)
	std::vector<HitParameterList>						hitparam_list;	//�����蔻��̃��X�g�B�������ɓǂݍ��݁A�ۑ�����(�����蔻��̐�������������)

public:
	//���f���p�ϐ�
	//MeshMotion						motion;
	//std::unique_ptr<Skinned_mesh>	base = nullptr;
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

	int				chara_color;		//�L�����̃J���[�ԍ�

public:
	//��{�����֐�
	virtual void Init(YR_Vector3 InitPos) = 0;
	virtual void LoadData(int color_number) = 0;
	virtual void Uninit() = 0;
	virtual void Update(float decision,float elapsed_time) = 0;
	virtual void Draw(
		YRShader				*parallel_shader,
		YRShader				*shader,
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

	virtual void TextDraw()=0;

public:
	//�s���n�֐�
	
	virtual void Move(float decision) = 0;
	virtual bool Step() = 0;
	virtual void Jump() = 0;
	virtual void JumpUpdate(float elapsed_time) = 0;
	virtual void DamageCheck() = 0;
	virtual void Guard(float decision) = 0;
	virtual void Squat() = 0;
	virtual void GuardBack(float elapsed_time) = 0;
	virtual void FallUpdate(float elapsed_time) = 0;
	virtual void DownUpdate() = 0;
	virtual void PassiveUpdate() = 0;
	virtual void WakeUp() = 0;
	virtual void GaugeUp(float add) = 0;
	virtual void CancelList() = 0;
	virtual void StateNone() = 0;
	virtual void KnockUpdate(float elapsed_time) = 0;

	virtual void HitBoxTransition(HitBoxState state) = 0;

	virtual void AttackInput() = 0;
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

	virtual bool Intro(float elapsed_time) = 0;
	virtual void IntroDEBUG() = 0;
	virtual bool WinPerformance() = 0;

	virtual void ReadySet() = 0;

	virtual bool AttackLoad() = 0;
	virtual bool AttackWrite() = 0;

public:
	virtual bool AttackEndCheck() = 0;				//�U�������蔻�肪�S�ďI�����Ă��邩�m�F����
	virtual void EndAttackErase() = 0;				//�I�������U�������蔻���S�ď�������B
	virtual void AllAttackClear() = 0;				//�S�Ă̍U�������蔻�����������
	virtual void AttackUpdate(float elapsed_time) = 0;				//�U�����肪���݂��鎞�̂ݍX�V

};

#endif // !PLAYERBASE_H_

